#include <stdio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cmath>
#include <driver/gpio.h>
#include <driver/ledc.h>

#include "config.h"

static adc_oneshot_unit_handle_t adc_handle;

static void adc_read_callback(void* arg) {
    int raw = 0;
    adc_oneshot_read(adc_handle, POT_ADC_CHANNEL, &raw);

    int duty = raw * 1023 / 4095;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}

extern "C" void app_main(void) {
    /* LED */
    ledc_timer_config_t led_timer_conf = {};
    led_timer_conf.speed_mode = LEDC_LOW_SPEED_MODE;
    led_timer_conf.timer_num = LEDC_TIMER_0;
    led_timer_conf.duty_resolution = LEDC_TIMER_10_BIT;
    led_timer_conf.freq_hz = 5000;
    led_timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&led_timer_conf);

    ledc_channel_config_t led_channel_conf = {};
    led_channel_conf.gpio_num = LED_GPIO;
    led_channel_conf.speed_mode = LEDC_LOW_SPEED_MODE;
    led_channel_conf.channel = LEDC_CHANNEL_0;
    led_channel_conf.timer_sel = LEDC_TIMER_0;
    led_channel_conf.duty = 0;
    ledc_channel_config(&led_channel_conf);

    /* POT */
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t chan_config = {};
    chan_config.bitwidth = ADC_BITWIDTH_CONFIG;
    chan_config.atten = ADC_ATTEN;
    adc_oneshot_config_channel(adc_handle, POT_ADC_CHANNEL, &chan_config);

    /* TIMER */
    esp_timer_create_args_t read_timer_args = {};
    esp_timer_handle_t read_timer;
    read_timer_args.callback = &adc_read_callback;
    read_timer_args.name = "adc_read_timer";
    esp_timer_create(&read_timer_args, &read_timer);
    esp_timer_start_periodic(read_timer, READ_INTERVAL_US);

    /* MOTOR */
    ledc_timer_config_t motor_timer_conf = {};
    motor_timer_conf.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_timer_conf.timer_num = LEDC_TIMER_1;
    motor_timer_conf.duty_resolution = LEDC_TIMER_10_BIT;
    motor_timer_conf.freq_hz = 1000;
    motor_timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&motor_timer_conf);

    ledc_channel_config_t motor_channel_conf = {};
    motor_channel_conf.gpio_num = BASE_TRANSISTOR_GPIO;
    motor_channel_conf.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_channel_conf.channel = LEDC_CHANNEL_1;
    motor_channel_conf.timer_sel = LEDC_TIMER_1;
    motor_channel_conf.duty = 0;
    ledc_channel_config(&motor_channel_conf);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}