#include <stdio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_timer.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cmath>

#include "config.h"

static const char *TAG = "ADC_CAL";

static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t cali_handle;
static bool table_header_printed = false;

static void print_table_header(void) {
    printf("RAW   U_manual(mV)   U_cali(mV)   Error(%%)\n");
    printf("------------------------------------------\n");
    table_header_printed = true;
}

static void adc_read_callback(void* arg) {
    int raw = 0;
    adc_oneshot_read(adc_handle, POT_ADC_CHANNEL, &raw);

    float u_manual_mv = raw * 3300.0f / 4095.0f;

    int u_cali_mv = 0;
    adc_cali_raw_to_voltage(cali_handle, raw, &u_cali_mv);

    float error_percent = 0.0f;
    if (u_cali_mv != 0) {
        error_percent = fabsf(u_manual_mv - u_cali_mv) / u_cali_mv * 100.0f;
    }

    if (!table_header_printed) {
        print_table_header();
    }

    printf("%4d      %6.1f          %4d        %5.2f\n",
           raw, u_manual_mv, u_cali_mv, error_percent);
}

extern "C" void app_main(void) {
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t chan_config = {};
    chan_config.bitwidth = ADC_BITWIDTH_CONFIG;
    chan_config.atten = ADC_ATTEN;
    adc_oneshot_config_channel(adc_handle, POT_ADC_CHANNEL, &chan_config);

    adc_cali_curve_fitting_config_t cali_config = {};
    cali_config.unit_id = ADC_UNIT_1;
    cali_config.chan = POT_ADC_CHANNEL;
    cali_config.atten = ADC_ATTEN;
    cali_config.bitwidth = ADC_BITWIDTH_CONFIG;
    esp_err_t cali_result = adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);
    if (cali_result != ESP_OK) {
        ESP_LOGE(TAG, "ADC calibration init failed!");
    }

    esp_timer_create_args_t read_timer_args = {};
    esp_timer_handle_t read_timer;
    read_timer_args.callback = &adc_read_callback;
    read_timer_args.name = "adc_read_timer";
    esp_timer_create(&read_timer_args, &read_timer);
    esp_timer_start_periodic(read_timer, READ_INTERVAL_US);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}