#pragma once

#include <driver/gpio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DEBOUNCING,
    BUTTON_STATE_PRESSED,
} ButtonState;

typedef struct {
    gpio_num_t pin;
    ButtonState state;
    int64_t lastChangeTime;
} Button;

void button_init(Button* btn, gpio_num_t pin);
bool button_update(Button* btn);