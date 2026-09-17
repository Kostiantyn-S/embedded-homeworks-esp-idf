#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_log.h"

#include "config.h"

static adc_oneshot_unit_handle_t adc1;

static bool is_within_servo_range(float pot_angle) {
    return pot_angle >= POT_SERVO_MIN_DEG && pot_angle <= POT_SERVO_MAX_DEG;
}

static float pot_angle_to_servo_angle(float pot_angle) {
    return pot_angle - POT_SERVO_MIN_DEG;
}

static float pot_raw_to_angle(int raw) {
    return (raw / 4095.0f) * POT_MAX_DEGREES;
}

static uint32_t angle_to_us(float servo_angle) {
    return SERVO_MIN_US + (uint32_t)((servo_angle / SERVO_MAX_DEGREES) * (SERVO_MAX_US - SERVO_MIN_US));
}

static void servo_init(void)
{
    ledc_timer_config_t t = {};
    t.speed_mode = LEDC_MODE;
    t.timer_num = LEDC_TIMER;
    t.duty_resolution = LEDC_DUTY_RES;
    t.freq_hz = 50;
    t.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&t));

    ledc_channel_config_t c = {};
    c.gpio_num = SERVO_GPIO;
    c.speed_mode = LEDC_MODE;
    c.channel = LEDC_CHANNEL;
    c.timer_sel = LEDC_TIMER;
    c.duty = 0;
    c.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&c));
}

static void pot_init(void)
{
    adc_oneshot_unit_init_cfg_t ucfg = {};
    ucfg.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&ucfg, &adc1);

    adc_oneshot_chan_cfg_t ccf = {};
    ccf.atten = ADC_ATTEN_DB_12;
    ccf.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(adc1, POT_CHANNEL, &ccf);
}

static void servo_set_us(uint32_t us)
{
    if (us > 2600) us = 2600;
    if (us < 400) us = 400;

    uint32_t duty = (uint32_t)(((uint64_t)us * SERVO_MAX_DUTY) / SERVO_PERIOD_US);

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

extern "C" void app_main(void)
{
    servo_init();
    pot_init();

    while (1)
    {
        int raw = 0;
        adc_oneshot_read(adc1, POT_CHANNEL, &raw);

        float pot_angle = pot_raw_to_angle(raw);

        if (is_within_servo_range(pot_angle)) {
            float servo_angle = pot_angle_to_servo_angle(pot_angle);
            uint32_t us = angle_to_us(servo_angle);
            servo_set_us(us);
            ESP_LOGI("SERVO", "servo angle: %.1f deg (us: %lu)", servo_angle, us);
        } else {
            float clamped_servo_angle = (pot_angle < POT_SERVO_MIN_DEG) ? 0.0f : SERVO_MAX_DEGREES;
            uint32_t us = angle_to_us(clamped_servo_angle);
            servo_set_us(us);
            ESP_LOGI("SERVO", "pot angle: %.1f deg - поза діапазоном серво мотора", pot_angle);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}