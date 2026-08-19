#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_log.h>
#include <stdatomic.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define BUTTON_GPIO 4
volatile int counter = 0;
static const char *TAG = "BUTTON";

gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << BUTTON_GPIO),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE,
};

void IRAM_ATTR button_handler(void* arg) {
    atomic_fetch_add(&counter, 1);
}

void app_main() {
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_handler, NULL);
    int previous_counter = atomic_load(&counter);

    while (true) {
        int current = atomic_load(&counter);

        if (previous_counter != current) {
            ESP_LOGI(TAG, "Counter: %d", current);
            previous_counter = current;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}