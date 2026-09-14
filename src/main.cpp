#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "config.h"

static void buzzer_init(void)
{
    ledc_timer_config_t t = {};
    t.speed_mode = LEDC_MODE;
    t.duty_resolution = LEDC_DUTY_RES;
    t.timer_num = LEDC_TIMER;
    t.freq_hz = 2700;
    t.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&t));

    ledc_channel_config_t c = {};
    c.gpio_num = BUZZER_GPIO;
    c.speed_mode = LEDC_MODE;
    c.channel = LEDC_CHANNEL;
    c.timer_sel  = LEDC_TIMER;
    c.duty = 0;
    c.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&c));
}

static void buttons_init(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << BTN1_GPIO) | (1ULL << BTN2_GPIO) | (1ULL << BTN_BOOT_GPIO);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
}

static void tone_on(uint16_t freq)
{
    ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 512);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

static void tone_off(void)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

extern "C" void app_main(void)
{
    buzzer_init();
    buttons_init();

    uint16_t current_freq = REST;

    while (true) {
        bool btn1_pressed = (gpio_get_level(BTN1_GPIO) == 0);
        bool btn2_pressed = (gpio_get_level(BTN2_GPIO) == 0);
        bool boot_pressed = (gpio_get_level(BTN_BOOT_GPIO) == 0);

        uint16_t target_freq = REST;

        if (btn1_pressed && btn2_pressed) {
            target_freq = F4;
        } else if (btn2_pressed && boot_pressed) {
            target_freq = G4;
        } else if (btn1_pressed && boot_pressed) {
            target_freq = A4;
        } else if (btn1_pressed) {
            target_freq = C4;
        } else if (btn2_pressed) {
            target_freq = D4;
        } else if (boot_pressed) {
            target_freq = E4;
        }

        if (target_freq != current_freq) {
            current_freq = target_freq;
            if (current_freq == REST) {
                tone_off();
            } else {
                tone_on(current_freq);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}