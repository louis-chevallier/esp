#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "dig_i2s_adc.h"
#include "soc/gpio_periph.h"

#define I2S_ADC_UNIT            (ADC_UNIT_1)

/*
This example set up I2S ADC mode, The ADC scans 4 channels (ADC unit0: ch3, ch0, ch7 and ch6) in sequence, and the sampling frequency is 2M. 
each sample takes 2 bytes, low 12bit is the result of AD conversion and the high 4bit is the channel num.

        |-400K-|
         __     __     __     __     __     __     __     __     __  
WS    __|  |___|  |___|  |___|  |___|  |___|  |___|  |___|  |___|  |__

       CH0   CH1   CH2   CH3   CH4   CH5   CH6   CH7  ...

receive buffer: [ CH1 ][ CH0 ][ CH3 ][ CH2 ][ CH5 ][ CH4 ][ CH7 ][ CH6 ]...

*/

//Out put WS signal from gpio18(only for debug mode)
void i2s_adc_check_clk(void)
{
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[18], PIN_FUNC_GPIO);
    gpio_set_direction(18, GPIO_MODE_DEF_OUTPUT);
    gpio_matrix_out(18, I2S0I_WS_OUT_IDX, 0, 0);
}

esp_err_t i2s_adc_setup(void)
{
    adc_channel_t channel[] = {
        ADC1_CHANNEL_0,
        ADC1_CHANNEL_1,
        ADC1_CHANNEL_2,
        ADC1_CHANNEL_3,
        ADC1_CHANNEL_4,
        ADC1_CHANNEL_5,
        ADC1_CHANNEL_6,
        ADC1_CHANNEL_7,
    };
    if (i2s_adc_init(I2S_NUM_0) != ESP_OK) {
        printf("i2s adc init fail\n");
        return ESP_FAIL;
    }
    //Configuring scan 8 channels, you can only scan 1 channel. see dig_i2s_adc.h
    adc_i2s_scale_mode_init(I2S_ADC_UNIT, channel, 8);
    //rate = 400K
    //ADC sampling rate = 20,000,000 / clkm_num. (sampling rate < 2M)
    i2s_adc_set_clk(I2S_NUM_0, 50);
    if (i2s_adc_driver_install(I2S_NUM_0, NULL, NULL) != ESP_OK){
        printf("driver install fail\n");
        return ESP_FAIL;
    }
    //Uncomment this line only in debug mode.
    //i2s_adc_check_clk();
    return ESP_OK;
}

void ledc_init(void)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0            // timer index
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 4096,
        .gpio_num   = 19,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0,
    };
    ledc_channel_config(&ledc_channel);
}

void app_main()
{
    ledc_init();
    uint16_t *buf = malloc(sizeof(uint16_t) * 300);
    if (!buf) {
        printf("buffer malloc fail\n");
        goto error;
    }
    if (i2s_adc_setup() != ESP_OK) {
        printf("i2s adc setup fail\n");
        goto error;
    }
    while(1) {
        if (i2s_adc_start(I2S_NUM_0, buf, 300*2) != ESP_OK) {
            goto error;
        }
        i2s_wait_adc_done(I2S_NUM_0, portMAX_DELAY);
        for (int i = 0; i < 300; i++) {
            printf("ch %d  %fv\n", (buf[i] >> 12), 3.3 - (3.14* (buf[i] & 0xfff)  / 4095) );
        }
        printf("------------------------\n");
        memset(buf, 0, 600);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
error:
    while(1) vTaskDelay(1000/portTICK_PERIOD_MS);
}
