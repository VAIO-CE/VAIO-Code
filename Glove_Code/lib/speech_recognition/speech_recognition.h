#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include <pin.h>
#include <constant.h>
#include <data_types.h>
#include <driver/i2s.h>
#include <esp_now.h>


class SpeechRecognition{
    public:
        typedef struct {
            signed short *buffers[2];
            unsigned char buf_select;
            unsigned char buf_ready;
            unsigned int buf_count;
            unsigned int n_samples;
        } inference_t;

        static inference_t inference;
        static struct SpeechRecognition_Data speechRecognition_Data;
        static const uint32_t sample_buffer_size = 2048;
        static signed short sampleBuffer[sample_buffer_size];
        static const bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
        static int print_results;
        static bool record_status;
        static bool recording_started;
        static bool send_esp_now;

        static void setupSpeechRecognition();
        static void vTaskSpeechRecognition(void *pvParameters);
        static void audio_inference_callback(uint32_t n_bytes);
        static void capture_samples(void* arg);
        static bool microphone_inference_start(uint32_t n_samples);
        static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
        static bool microphone_inference_record(void);
        static void microphone_inference_end(void);
        static int i2s_init(uint32_t sampling_rate);
        static int i2s_deinit(void);
};

#endif // SPEECHRECOGNITION_H