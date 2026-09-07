#include <stdio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cmath>
#include <driver/gpio.h>

#include "config.h"

static adc_oneshot_unit_handle_t adc_handle;
static int sma_buffer[SMA_WINDOW_SIZE];
static int sma_index = 0;
static int sma_sum = 0;
static bool sma_buffer_filled = false;
static int counter = 0;
static bool led_is_on = false;
static const gpio_num_t LED_GPIO = GPIO_NUM_18;

void update_led_state(int sma_value) {
    if (!led_is_on && sma_value < THRESHOLD_DARK) {
        led_is_on = true;
        gpio_set_level(LED_GPIO, 1);
    } else if (led_is_on && sma_value > THRESHOLD_LIGHT) {
        led_is_on = false;
        gpio_set_level(LED_GPIO, 0);
    }
}

static void sma_add_value(int raw) {
    if (!sma_buffer_filled) {
        sma_buffer[counter] = raw;
        counter++;
        sma_sum += raw;

        if (counter >= SMA_WINDOW_SIZE) {
            sma_buffer_filled = true;
        }
    } else {
        sma_sum -= sma_buffer[sma_index];
        sma_buffer[sma_index] = raw;
        sma_sum += raw;
        sma_index++;
        if (sma_index % SMA_WINDOW_SIZE == 0) {
            sma_index = 0;
        }
    }
}

static void adc_read_callback(void* arg) {
    int raw = 0;
    adc_oneshot_read(adc_handle, POT_ADC_CHANNEL, &raw);

    sma_add_value(raw);

    int divisor = sma_buffer_filled ? SMA_WINDOW_SIZE : counter;
    if (divisor > 0) {
        printf("%4d\n", sma_sum / divisor);
        update_led_state(sma_sum / divisor);
    }
}

extern "C" void app_main(void) {
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t chan_config = {};
    chan_config.bitwidth = ADC_BITWIDTH_CONFIG;
    chan_config.atten = ADC_ATTEN;
    adc_oneshot_config_channel(adc_handle, POT_ADC_CHANNEL, &chan_config);

    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << LED_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);

    esp_timer_create_args_t read_timer_args = {};
    esp_timer_handle_t read_timer;
    read_timer_args.callback = &adc_read_callback;
    read_timer_args.name = "adc_read_timer";
    esp_timer_create(&read_timer_args, &read_timer);
    esp_timer_start_periodic(read_timer, READ_INTERVAL_US);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}