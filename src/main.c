#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_log.h>
#include <stdatomic.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#define BUTTON_GPIO 4
volatile int counter = 0;
volatile int64_t lastAcceptedTime;
volatile bool eventFlag;
static const char *TAG = "BUTTON";

gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << BUTTON_GPIO),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE,
};

void IRAM_ATTR button_handler(void* arg) {
    eventFlag = true;
}

void app_main() {
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_handler, NULL);

    int64_t now = esp_timer_get_time();
    eventFlag = false;
    lastAcceptedTime = now;

    while (true) {
        now = esp_timer_get_time();

        if (eventFlag) {
            eventFlag = false;

            if (now - lastAcceptedTime >= (50000)) {
                lastAcceptedTime = now;
                atomic_fetch_add(&counter, 1);
                ESP_LOGI(TAG, "Counter: %d", atomic_load(&counter));
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}