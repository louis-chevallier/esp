/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_task_info.h"
#include "esp_log.h"
#include "esp_random.h"

//#define NOEKO 1
#include "utillc.h"

#define EXAMPLE_ADC_UNIT                    ADC_UNIT_1
#define _EXAMPLE_ADC_UNIT_STR(unit)         #unit
#define EXAMPLE_ADC_UNIT_STR(unit)          _EXAMPLE_ADC_UNIT_STR(unit)
#define EXAMPLE_ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define EXAMPLE_ADC_ATTEN                   ADC_ATTEN_DB_0
#define EXAMPLE_ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define EXAMPLE_ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_ADC_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define EXAMPLE_ADC_GET_DATA(p_data)        ((p_data)->type1.data)
#else
#define EXAMPLE_ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE2
#define EXAMPLE_ADC_GET_CHANNEL(p_data)     ((p_data)->type2.channel)
#define EXAMPLE_ADC_GET_DATA(p_data)        ((p_data)->type2.data)
#endif

//#define EXAMPLE_READ_LEN                    256
#define EXAMPLE_READ_LEN                    8

#if CONFIG_IDF_TARGET_ESP32
static adc_channel_t channel[4] = {ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7};
#else
static adc_channel_t channel[2] = {ADC_CHANNEL_2, ADC_CHANNEL_3};
#endif

static TaskHandle_t s_task_handle;
static const char *TAG = "EXAMPLE";


long count1 = 0;

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
  BaseType_t mustYield = pdFALSE;
  //Notify that ADC continuous driver has done enough number of conversions
  vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
  //count1 += 1;
  return (mustYield == pdTRUE);
}

static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
  adc_continuous_handle_t handle = NULL;

  adc_continuous_handle_cfg_t adc_config = {
    .max_store_buf_size = 1024,
    .conv_frame_size = EXAMPLE_READ_LEN,
  };
  ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

  adc_continuous_config_t dig_cfg = {
    .sample_freq_hz = 200 * 1000,
    .conv_mode = EXAMPLE_ADC_CONV_MODE,
    .format = EXAMPLE_ADC_OUTPUT_TYPE,
  };

  adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
  dig_cfg.pattern_num = channel_num;
  for (int i = 0; i < channel_num; i++) {
    adc_pattern[i].atten = EXAMPLE_ADC_ATTEN;
    adc_pattern[i].channel = channel[i] & 0x7;
    adc_pattern[i].unit = EXAMPLE_ADC_UNIT;
    adc_pattern[i].bit_width = EXAMPLE_ADC_BIT_WIDTH;

    ESP_LOGI(TAG, "adc_pattern[%d].atten is :%" PRIx8, i, adc_pattern[i].atten);
    ESP_LOGI(TAG, "adc_pattern[%d].channel is :%" PRIx8, i, adc_pattern[i].channel);
    ESP_LOGI(TAG, "adc_pattern[%d].unit is :%" PRIx8, i, adc_pattern[i].unit);
  }
  dig_cfg.adc_pattern = adc_pattern;
  ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));
  //EKOX((int)out_handle);
  *out_handle = handle;
}


static void example_task(void *args)
{
  const auto KKK = 100000;
  uint32_t size = 0;
  const char *TAG = "example_task";

  esp_err_t results = esp_wifi_stop();
  
  //EKOT("begin");
  esp_err_t ret;
  uint32_t ret_num = 0;
  uint8_t result[EXAMPLE_READ_LEN] = {0};
  memset(result, 0xcc, EXAMPLE_READ_LEN);
  s_task_handle = xTaskGetCurrentTaskHandle();
  adc_continuous_handle_t handle = NULL;
  continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);

  adc_continuous_evt_cbs_t cbs = {
    .on_conv_done = s_conv_done_cb,
  };
  EKO();
  vTaskDelay(10);
  //EKO();
  ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
  //EKO();
  ESP_ERROR_CHECK(adc_continuous_start(handle));
  vTaskDelay(1);
  long count = 0;
  EKOT("go");

  int chans[10] = {0};
  
  for (int ii = 0; ii < 100000000; ii++)  {
    
    /**
     * This is to show you the way to use the ADC continuous mode driver event callback.
     * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
     * However in this example, the data processing (print) is slow, so you barely block here.
     *
     * Without using this event callback (to notify this task), you can still just call
     * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
     */
    //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    char unit[] = EXAMPLE_ADC_UNIT_STR(EXAMPLE_ADC_UNIT);

    for (int jj = 0; jj < 1000000000; jj++) {
      //EKOX(jj);
      ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
      //EKOX(ret_num);

      
      if (ret == ESP_OK) {
        count += ret_num / SOC_ADC_DIGI_RESULT_BYTES;
        
        //ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32" bytes", ret, ret_num);
        for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
          adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
          uint32_t chan_num = EXAMPLE_ADC_GET_CHANNEL(p);
          uint32_t data = EXAMPLE_ADC_GET_DATA(p);

          //EKOX(chan_num);
          chans[chan_num] ++;
          
          /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
          if (chan_num < SOC_ADC_CHANNEL_NUM(EXAMPLE_ADC_UNIT)) {
            //ESP_LOGI(TAG, "Unit: %s, Channel: %" PRIu32", Value: %" PRIx32, unit, chan_num, data);
          } else {
            ESP_LOGW(TAG, "Invalid data [%s_%" PRIu32"_%" PRIx32"]", unit, chan_num, data);
          }
        }
        /**
         * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
         * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
         * usually you don't need this delay (as this task will block for a while).
         */
        //vTaskDelay(1);
        if (count > KKK) {
          EKOX(count);
          break;
        }
      } else if (ret == ESP_ERR_TIMEOUT) {
        //We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
        break;
      }
      //EKOX(jj);
    }
    //EKOX(count);
    if (count > KKK) {
      EKOX(count);
      break;
    }
    
  }
  EKOX(count1 - count);
  std::vector<int> vec(chans,chans+10);  
  for (auto e : vec) {
    EKOX(e);
  }

  EKOT("end");
  printf("count %ld\n", count);
  ESP_ERROR_CHECK(adc_continuous_stop(handle));
  ESP_ERROR_CHECK(adc_continuous_deinit(handle));
  
  while (1) {
    /*
     * Allocate random amount of memory for demonstration
     */
    size = (esp_random() % 1000);
    void *ptr = malloc(size);
    if (ptr == NULL) {
      ESP_LOGE(TAG, "Could not allocate heap memory");
      abort();
    }
    //esp_dump_per_task_heap_info();
    free(ptr);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

/*
 * 16KHz : 3 channels ADC 
 */

extern "C" void app_main(); 
void app_main(void)
{

  xTaskCreate(&example_task, "example_task", 5072, NULL, 5, NULL);

}  
