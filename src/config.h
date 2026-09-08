#pragma once

#include <driver/gpio.h>

constexpr static gpio_num_t LED_GPIO = GPIO_NUM_18;
constexpr static gpio_num_t POT_GPIO = GPIO_NUM_6;
constexpr static gpio_num_t BASE_TRANSISTOR_GPIO = GPIO_NUM_4;
constexpr static int READ_INTERVAL_US = (100ULL * 1000ULL);
constexpr static adc_channel_t POT_ADC_CHANNEL = ADC_CHANNEL_5;
constexpr static adc_atten_t ADC_ATTEN = ADC_ATTEN_DB_12;
constexpr static adc_bitwidth_t ADC_BITWIDTH_CONFIG = ADC_BITWIDTH_12;