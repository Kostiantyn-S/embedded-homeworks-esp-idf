// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/ledc.h"
// #include "esp_err.h"

// #include "config.h"

// typedef struct {
//     uint16_t freq;     
//     uint16_t duration;  
// } note_t;

// static const note_t melody[] = {
//     {NOTE_E7, 400}, {REST, 50},
//     {NOTE_E7, 200}, {NOTE_G7, 400}, {NOTE_E7, 400},
//     {NOTE_D7, 400}, {NOTE_C7, 600}, {REST, 100},
//     {NOTE_C7, 800}, {REST, 300},    
//     {NOTE_E7, 200}, {NOTE_G7, 400}, {NOTE_E7, 400},
//     {NOTE_D7, 400}, {NOTE_C7, 600}, {REST, 10},{NOTE_C7, 300},
//     {NOTE_C7, 300}, {NOTE_C7, 800}, {REST, 300},
// };
// #define MELODY_LEN (sizeof(melody) / sizeof(note_t))

// static void buzzer_init(void)
// {
//     ledc_timer_config_t t = {};
//     t.speed_mode      = LEDC_MODE;
//     t.timer_num       = LEDC_TIMER;
//     t.duty_resolution = LEDC_DUTY_RES;
//     t.freq_hz         = 2700;
//     t.clk_cfg         = LEDC_AUTO_CLK;
    
//     ESP_ERROR_CHECK(ledc_timer_config(&t));

//     ledc_channel_config_t c = {};
//     c.gpio_num   = BUZZER_GPIO;
//     c.speed_mode = LEDC_MODE;
//     c.channel    = LEDC_CHANNEL;
//     c.timer_sel  = LEDC_TIMER;
//     c.duty       = 0;
//     c.hpoint     = 0;

//     ESP_ERROR_CHECK(ledc_channel_config(&c));
// }

// static void tone_on(uint16_t freq)
// {
//     ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
//     ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 512);
//     ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
// }

// static void tone_off(void)
// {
//     ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
//     ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
// }

// static void play_note(uint16_t freq, uint16_t duration)
// {
//     uint16_t play = duration * 85 / 100;
//     uint16_t gap  = duration - play;

//     if (freq == REST) {
//         tone_off();
//         vTaskDelay(pdMS_TO_TICKS(duration));
//         return;
//     }
//     tone_on(freq);
//     vTaskDelay(pdMS_TO_TICKS(play));
//     tone_off();
//     vTaskDelay(pdMS_TO_TICKS(gap));
// }

// extern "C" void app_main(void)
// {
//     buzzer_init();

//     while (1) {
//         for (int i = 0; i < MELODY_LEN; i++) {
//             play_note(melody[i].freq, melody[i].duration);
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

/*--------------------------------------------------------------------*/
 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#include "config.h"

typedef struct {
    uint16_t freq;     
    uint16_t duration;  
} note_t;


static const note_t melody[] = {
    {E3,1},{E3,1},{E3,1},{G3,1},{E3,1},{E3,1},{AS3,1},{E3,1},
    {E3,1},{E3,1},{E3,1},{G3,1},{AS3,1},{A3,1},{G3,1},{FS3,1},

    {E3,1},{E3,1},{E3,1},{G3,1},{E3,1},{E3,1},{AS3,1},{E3,1},
    {E3,2},{G3,2},{AS3,2},{B3,2},

    {G3,1},{G3,1},{G3,1},{AS3,1},{G3,1},{G3,1},{CS4,1},{G3,1},
    {FS3,1},{FS3,1},{FS3,1},{A3,1},{FS3,1},{FS3,1},{C4,1},{FS3,1},

    {E3,1},{E3,1},{G3,1},{AS3,1},{B3,1},{AS3,1},{G3,1},{E3,1},
    {E3,6},{REST,2},

    {REST,20},

    {D4,1},{D4,1},{D4,1},{A4,1},{D4,1},{D4,1},{AS4,1},{A4,1},
    {D4,1},{D4,1},{D4,1},{A4,1},{C5,1},{AS4,1},{A4,1},{G4,1},

    {F4,2},{E4,2},{D4,2},{CS4,2},
    {D4,2},{F4,2},{A4,2},{D5,2},

    {D5,1},{C5,1},{AS4,1},{A4,1},{G4,1},{F4,1},{E4,1},{D4,1},
    {CS4,2},{D4,2},{CS4,2},{D4,2},

    {D4,4},{REST,4},

    {REST,20},

    {E4,4},{REST,1},{F4,3},{E4,4},{AS3,4},
    {E4,4},{REST,1},{G4,3},{FS4,4},{E4,4},

    {C4,4},{B3,4},{AS3,4},{B3,4},
    {E4,6},{DS4,2},{E4,8},

    {A4,4},{GS4,4},{G4,4},{FS4,4},
    {F4,6},{E4,2},{AS3,8},

    {E4,8},{REST,4},
    {REST,20},
};
#define MELODY_LEN (sizeof(melody) / sizeof(note_t))

static void buzzer_init(void)
{
    ledc_timer_config_t t = {};
    t.speed_mode = LEDC_MODE;
    t.duty_resolution = LEDC_DUTY_RES;
    t.timer_num = LEDC_TIMER;
    t.freq_hz = 2700;
    t.clk_cfg = LEDC_AUTO_CLK;

    ESP_ERROR_CHECK(ledc_timer_config(&t));

    ledc_channel_config_t c = {};
    c.gpio_num = BUZZER_GPIO;
    c.speed_mode = LEDC_MODE;
    c.channel = LEDC_CHANNEL;
    c.timer_sel  = LEDC_TIMER;
    c.duty = 0;
    c.hpoint = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&c));
}

static void tone_on(uint16_t freq)
{
    ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 512);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

static void tone_off(void)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

static void play_note(uint16_t freq, uint16_t duration)
{
    uint16_t play = duration  * 85 / 100;
    uint16_t gap  = duration - play;

    if (freq == REST) {
        tone_off();
        vTaskDelay(pdMS_TO_TICKS(duration));
        return;
    }
    tone_on(freq);
    vTaskDelay(pdMS_TO_TICKS(play));
    tone_off();
    vTaskDelay(pdMS_TO_TICKS(gap));
}

extern "C" void app_main(void)
{
    buzzer_init();

    while (1) {
        for (int i = 0; i < MELODY_LEN; i++) {
            play_note(melody[i].freq, melody[i].duration*30+100);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}