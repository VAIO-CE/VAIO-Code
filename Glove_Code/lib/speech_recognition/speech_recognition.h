#ifndef SPEECHRECOGNITION_H
#define SPEECHRECOGNITION_H

#include "constant.h"
#include "data_types.h"
#include "pin.h"

#ifdef SPEECH_USE_ONLY
#include "speech_recognition_inferencing.h"
#endif

#include <driver/i2s.h>
#include <esp32-hal-gpio.h>
#include <esp_now.h>
#include <pins_arduino.h>

#define EIDSP_QUANTIZE_FILTERBANK 0
#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 2

class SpeechRecognition {
private:
  typedef struct {
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int buf_count;
    unsigned int n_samples;
  } inference_t;

  static inline inference_t inference{};
  static inline SpeechRecognition_Data speechRecognition_Data{};
  static constexpr uint32_t sample_buffer_size{2048};
  static inline signed short sampleBuffer[sample_buffer_size];
  static const bool debug_nn = false; // Set this to true to see e.g. features
                                      // generated from the raw signal
  static inline int print_results{-EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW};
  static inline bool record_status{true};
  static inline bool recording_started{false};
  static inline bool send_esp_now{false};

  static void audio_inference_callback(uint32_t n_bytes);
  static void capture_samples(void *arg);
  static bool microphone_inference_start(uint32_t n_samples);
  static int microphone_audio_signal_get_data(size_t offset, size_t length,
                                              float *out_ptr);
  static bool microphone_inference_record(void);
  static void microphone_inference_end(void);
  static int i2s_init(uint32_t sampling_rate);
  static int i2s_deinit(void);

public:
  static void setupSpeechRecognition();
  static void vTaskSpeechRecognition(void *pvParameters);
};

#endif // SPEECHRECOGNITION_H
