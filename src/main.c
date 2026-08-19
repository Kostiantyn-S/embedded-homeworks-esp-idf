#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_log.h>
#include <stdatomic.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#define FAN_GPIO 4
static const char *TAG = "FAN";

#define CYCLE_INTERVAL_US       (10ULL * 1000000ULL)
#define ON_DURATION_US          (3ULL * 1000000ULL)
#define MAX_SAFE_ON_DURATION_US (ON_DURATION_US + 5000000ULL)

esp_timer_handle_t off_timer;
esp_timer_handle_t cycle_timer;
esp_timer_handle_t safety_off_timer;

void safety_off_callback(void* arg) {
    if (gpio_get_level(FAN_GPIO) == 1) {
        gpio_set_level(FAN_GPIO, 0);
        ESP_LOGW(TAG, "SAFETY WATCHDOG: Fan OFF!");
    }
}

void off_timer_callback(void* arg) {
    gpio_set_level(FAN_GPIO, 0);
    ESP_LOGI(TAG, "Fan OFF");
}

void cycle_timer_callback(void* arg) {
    if (esp_timer_is_active(off_timer)) {
        ESP_LOGW(TAG, "off_timer already active");
        return;
    }

    gpio_set_level(FAN_GPIO, 1);
    ESP_LOGI(TAG, "Fan ON");
    esp_timer_start_once(off_timer, ON_DURATION_US);
    esp_timer_start_once(safety_off_timer, MAX_SAFE_ON_DURATION_US);
}

void app_main() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << FAN_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    const esp_timer_create_args_t off_timer_args = {
        .callback = &off_timer_callback,
        .name = "off_timer"
    };
    esp_timer_create(&off_timer_args, &off_timer);

    const esp_timer_create_args_t cycle_timer_args = {
        .callback = &cycle_timer_callback,
        .name = "cycle_timer"
    };
    esp_timer_create(&cycle_timer_args, &cycle_timer);

    const esp_timer_create_args_t safety_off_timer_args = {
        .callback = &safety_off_callback,
        .name = "safety_off_timer"
    };
    esp_timer_create(&safety_off_timer_args, &safety_off_timer);

    esp_timer_start_periodic(cycle_timer, CYCLE_INTERVAL_US);
}