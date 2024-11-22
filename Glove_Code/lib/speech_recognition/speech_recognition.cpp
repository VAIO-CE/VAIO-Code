#include <speech_recognition.h>
#include <speech_recognition_inferencing.h>

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
    #error "Invalid model for current sensor."
#endif

#define EIDSP_QUANTIZE_FILTERBANK   0

#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 3


struct SpeechRecognition_Data SpeechRecognition::speechRecognition_Data;
SpeechRecognition::inference_t SpeechRecognition::inference;
signed short SpeechRecognition::sampleBuffer[SpeechRecognition::sample_buffer_size];
int SpeechRecognition::print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
bool SpeechRecognition::record_status = true;
bool SpeechRecognition::recording_started = false;
bool SpeechRecognition::send_esp_now = false;

void SpeechRecognition::setupSpeechRecognition(){
    // put your setup code here, to run once:
    // Serial.begin(115200);
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    // while (!Serial);
    // Serial.println("Edge Impulse Inferencing Demo");

    // summary of inferencing settings (from model_metadata.h)
    #ifdef PRINT_DEBUG
        ei_printf("Inferencing settings:\n");
        ei_printf("\tInterval: ");
        ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
        ei_printf(" ms.\n");
        ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
        ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
        ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
    #endif

    run_classifier_init();
    ei_printf("\nStarting continious inference in 2 seconds...\n");
    ei_sleep(2000);

    if (SpeechRecognition::microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false) {
        ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
        return;
    }

    ei_printf("Recording...\n");
    recording_started = true;
}
void SpeechRecognition::vTaskSpeechRecognition(void *pvParameters){
    while(1){
        while(recording_started){
            bool m = microphone_inference_record();
            if (!m) {
                ei_printf("ERR: Failed to record audio...\n");
                return;
            }

            signal_t signal;
            signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
            signal.get_data = &microphone_audio_signal_get_data;
            ei_impulse_result_t result = {0};

            EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result);
            if (r != EI_IMPULSE_OK) {
                ei_printf("ERR: Failed to run classifier (%d)\n", r);
                return;
            }
            // inference result
            if (result.classification[2].value > 0.7){
                Serial.println("Heard: Control");
            }
            if (result.classification[3].value > 0.7){
                Serial.println("Heard: Hand");
            }
            if (result.classification[4].value > 0.6){
                Serial.println("Heard: Move");
            }
            // if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)) {
                // print the predictions
            #ifdef PRINT_DEBUG
                ei_printf("Predictions ");
                ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
                    result.timing.dsp, result.timing.classification, result.timing.anomaly);
                ei_printf(": \n");
                for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                    ei_printf("    %s: ", result.classification[ix].label);
                    ei_printf_float(result.classification[ix].value);
                    ei_printf("\n");
                }
            #endif
                // }
            // #if EI_CLASSIFIER_HAS_ANOMALY == 1
            //         ei_printf("    anomaly score: ");
            //         ei_printf_float(result.anomaly);
            //         ei_printf("\n");
            // #endif

            // print_results = 0;
            // }

            // send data to ESP-NOW
            speechRecognition_Data.move = result.classification[4].value;
            speechRecognition_Data.control = result.classification[2].value;
            speechRecognition_Data.hand = result.classification[3].value;
            speechRecognition_Data.on = result.classification[6].value;
            speechRecognition_Data.off = result.classification[5].value;

            uint8_t data[1 + sizeof(SpeechRecognition_Data)];
            data[0] = SPEECH_DATA; // Header byte to identify the data type
            memcpy(data + 1, &speechRecognition_Data, sizeof(SpeechRecognition_Data));

            esp_err_t esp_now_result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
        }
        vTaskDelay(portTICK_PERIOD_MS / 10);
    }
}


void SpeechRecognition::audio_inference_callback(uint32_t n_bytes)
{
    for(int i = 0; i < n_bytes>>1; i++) {
        inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

        if(inference.buf_count >= inference.n_samples) {
            inference.buf_select ^= 1;
            inference.buf_count = 0;
            inference.buf_ready = 1;
        }
    }
}

void SpeechRecognition::capture_samples(void* arg) {

  const int32_t i2s_bytes_to_read = (uint32_t)arg;
  size_t bytes_read = i2s_bytes_to_read;

  while (record_status) {

    /* read data at once from i2s */
    i2s_read((i2s_port_t)1, (void*)sampleBuffer, i2s_bytes_to_read, &bytes_read, 100);

    if (bytes_read <= 0) {
      ei_printf("Error in I2S read : %d", bytes_read);
    }
    else {
        if (bytes_read < i2s_bytes_to_read) {
        ei_printf("Partial I2S read");
        }

        // scale the data (otherwise the sound is too quiet)
        for (int x = 0; x < i2s_bytes_to_read/2; x++) {
            sampleBuffer[x] = (int16_t)(sampleBuffer[x]) * 8;
        }

        if (record_status) {
            audio_inference_callback(i2s_bytes_to_read);
        }
        else {
            break;
        }
    }
  }
  vTaskDelete(NULL);
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 *
 * @param[in]  n_samples  The n samples
 *
 * @return     { description_of_the_return_value }
 */
bool SpeechRecognition::microphone_inference_start(uint32_t n_samples)
{
    inference.buffers[0] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL) {
        return false;
    }

    inference.buffers[1] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[1] == NULL) {
        ei_free(inference.buffers[0]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    if (i2s_init(EI_CLASSIFIER_FREQUENCY)) {
        ei_printf("Failed to start I2S!");
    }

    ei_sleep(100);

    record_status = true;

    xTaskCreatePinnedToCore(capture_samples, "CaptureSamples", 1024 * 32, (void*)sample_buffer_size, 10, NULL, 1);

    return true;
}

/**
 * @brief      Wait on new data
 *
 * @return     True when finished
 */
bool SpeechRecognition::microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1) {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0) {
        delay(1);
    }

    inference.buf_ready = 0;
    return true;
}

/**
 * Get raw audio signal data
 */
int SpeechRecognition::microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
void SpeechRecognition::microphone_inference_end(void)
{
    i2s_deinit();
    ei_free(inference.buffers[0]);
    ei_free(inference.buffers[1]);
}


int SpeechRecognition::i2s_init(uint32_t sampling_rate) {
  // Start listening for audio: MONO @ 8/16KHz
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
      .sample_rate = sampling_rate,
      .bits_per_sample = (i2s_bits_per_sample_t)16,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S,
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = 512,
      .use_apll = false,
      .tx_desc_auto_clear = false,
      .fixed_mclk = -1,
  };
  i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK_PIN,    // IIS_SCLK
      .ws_io_num = I2S_WS_PIN,     // IIS_LCLK
      .data_out_num = -1,  // IIS_DSIN
      .data_in_num = I2S_SD_PIN,   // IIS_DOUT
  };
  esp_err_t ret = 0;

  ret = i2s_driver_install((i2s_port_t)1, &i2s_config, 0, NULL);
  if (ret != ESP_OK) {
    ei_printf("Error in i2s_driver_install");
  }

  ret = i2s_set_pin((i2s_port_t)1, &pin_config);
  if (ret != ESP_OK) {
    ei_printf("Error in i2s_set_pin");
  }

  ret = i2s_zero_dma_buffer((i2s_port_t)1);
  if (ret != ESP_OK) {
    ei_printf("Error in initializing dma buffer with 0");
  }

  return int(ret);
}

int SpeechRecognition::i2s_deinit(void) {
    i2s_driver_uninstall((i2s_port_t)1); //stop & destroy i2s driver
    return 0;
}