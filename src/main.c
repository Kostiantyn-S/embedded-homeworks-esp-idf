#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include "led.h"
#include "config.h"
#include "button.h"

typedef enum {
    GREEN_STATE,
    GREEN_BLINK_STATE,
    YELLOW_STATE,
    RED_STATE,
    RED_AND_YELLOW_STATE,
    YELLOW_BLINK
} TrafficLightsState;

static TrafficLightsState current_state;
static esp_timer_handle_t cycle_timer;
static esp_timer_handle_t green_blink_timer;
static bool green_blink_state = false;
static esp_timer_handle_t yellow_blink_timer;
static bool yellow_blink_flag = false;
static bool yellow_blink_state = false;

void set_leds(bool red, bool yellow, bool green) {
    gpio_set_level(RED_LED_PIN_4, red);
    gpio_set_level(YELLOW_LED_PIN_5, yellow);
    gpio_set_level(GREEN_LED_PIN_6, green);
}

void set_yellow_blink_state() {
    if (yellow_blink_flag) {
        current_state = YELLOW_BLINK;
        esp_timer_stop(cycle_timer);
        if (esp_timer_is_active(green_blink_timer)) {
            esp_timer_stop(green_blink_timer);
        }

        set_leds(false, false, false);
        yellow_blink_state = false;
        esp_timer_start_periodic(yellow_blink_timer, YELLOW_BLINK_INTERVAL_US);
    } else {
        esp_timer_stop(yellow_blink_timer);
        current_state = GREEN_STATE;
        esp_timer_start_once(cycle_timer, 0);
    }
}

void cycle_timer_callback(void* arg) {
    uint64_t next_interval_us = 0;

    switch (current_state) {
        case GREEN_STATE:
            set_leds(false, false, true);
            current_state = GREEN_BLINK_STATE;
            next_interval_us = GREEN_DURATION_US;
            break;

        case GREEN_BLINK_STATE:
            set_leds(false, false, false);
            green_blink_state = false;
            esp_timer_start_periodic(green_blink_timer, GREEN_BLINK_INTERVAL_US);
            current_state = YELLOW_STATE;
            next_interval_us = GREEN_BLINK_DURATION_US;
            break;

        case YELLOW_STATE:
            esp_timer_stop(green_blink_timer);
            set_leds(false, true, false);
            current_state = RED_STATE;
            next_interval_us = YELLOW_DURATION_US;
            break;

        case RED_STATE:
            set_leds(true, false, false);
            current_state = RED_AND_YELLOW_STATE;
            next_interval_us = RED_DURATION_US;
            break;

        case RED_AND_YELLOW_STATE:
            set_leds(true, true, false);
            current_state = GREEN_STATE;
            next_interval_us = RED_AND_YELLOW_DURATION_US;
            break;

        case YELLOW_BLINK:
            break;
    }

    esp_timer_start_once(cycle_timer, next_interval_us);
}

void green_blink_timer_callback(void* arg) {
    green_blink_state = !green_blink_state;
    gpio_set_level(GREEN_LED_PIN_6, green_blink_state);
}

void yellow_blink_timer_callback(void* arg) {
    yellow_blink_state = !yellow_blink_state;
    gpio_set_level(YELLOW_LED_PIN_5, yellow_blink_state);
}

void app_main() {
    Led red_led;
    led_init(&red_led, RED_LED_PIN_4);

    Led yellow_led;
    led_init(&yellow_led, YELLOW_LED_PIN_5);

    Led green_led;
    led_init(&green_led, GREEN_LED_PIN_6);

    current_state = GREEN_STATE;

    const esp_timer_create_args_t cycle_timer_args = {
        .callback = &cycle_timer_callback,
        .name = "cycle_timer"
    };
    esp_timer_create(&cycle_timer_args, &cycle_timer);

    const esp_timer_create_args_t green_blink_timer_args = {
        .callback = &green_blink_timer_callback,
        .name = "green_blink_timer"
    };
    esp_timer_create(&green_blink_timer_args, &green_blink_timer);

    const esp_timer_create_args_t yellow_blink_timer_args = {
        .callback = &yellow_blink_timer_callback,
        .name = "yellow_blink_timer"
    };
    esp_timer_create(&yellow_blink_timer_args, &yellow_blink_timer);

    esp_timer_start_once(cycle_timer, 0);

    Button btn;
    button_init(&btn, BUTTON_PIN);

    while (true) {
        if (button_update(&btn)) {
            yellow_blink_flag = !yellow_blink_flag;
            set_yellow_blink_state();
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}