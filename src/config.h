#pragma once

#include "driver/ledc.h"
#include "driver/gpio.h"

constexpr static int SERVO_GPIO = 4;
constexpr static ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
constexpr static ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;
constexpr static ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;
constexpr static ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_14_BIT;

constexpr static adc_channel_t POT_CHANNEL = ADC_CHANNEL_4;
constexpr static float POT_MAX_DEGREES = 270.0f;

constexpr static int SERVO_MAX_DUTY = (1u << 14);
constexpr static int SERVO_PERIOD_US = 20000u;
constexpr static int SERVO_MIN_US = 400;
constexpr static int SERVO_MAX_US = 2600;
constexpr static float SERVO_MAX_DEGREES = 180.0f;

constexpr static float POT_SERVO_MIN_DEG = 45.0f;
constexpr static float POT_SERVO_MAX_DEG = 225.0f;