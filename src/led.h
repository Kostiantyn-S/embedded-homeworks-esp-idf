#pragma once

#include <driver/gpio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_ON,
    LED_OFF
} LedState;

typedef struct {
    gpio_num_t pin;
    LedState state;
} Led;

void led_init(Led* btn, gpio_num_t pin);