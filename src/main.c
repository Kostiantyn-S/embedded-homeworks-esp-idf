#include <esp_log.h>
#include <stdatomic.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "button.h"

#define BUTTON_GPIO 4
volatile int counter = 0;
static const char *TAG = "BUTTON";

void app_main() {
    Button btn;
    button_init(&btn, BUTTON_GPIO);

    while (true) {
        if (button_update(&btn)) {
            atomic_fetch_add(&counter, 1);
            ESP_LOGI(TAG, "Counter: %d", atomic_load(&counter));
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}