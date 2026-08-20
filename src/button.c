#include "button.h"
#include <esp_timer.h>

void button_init(Button* btn, gpio_num_t pin) {
    btn->pin = pin;
    btn->state = BUTTON_STATE_IDLE;
    btn->lastChangeTime = 0;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

bool button_update(Button* btn) {
    int64_t now = esp_timer_get_time();
    int gpio_level = gpio_get_level(btn->pin);

    switch (btn->state)
    {
        case BUTTON_STATE_IDLE:
            if (gpio_level == 0) {
                btn->state = BUTTON_STATE_DEBOUNCING;
                btn->lastChangeTime = now;
            }
            break;

        case BUTTON_STATE_DEBOUNCING:
            if (gpio_level == 0) {
                if (now - btn->lastChangeTime >= 50000) {
                    btn->state = BUTTON_STATE_PRESSED;
                    return true;
                }
            } else {
                btn->state = BUTTON_STATE_IDLE;
            }
            break;

        case BUTTON_STATE_PRESSED:
            if (gpio_level != 0) {
                btn->state = BUTTON_STATE_IDLE;
            }
            break;
        
        default:
            break;
    }

    return false;
}