#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include <esp_adc/adc_oneshot.h>
#include <esp_timer.h>

#include "config.h"

static int current_note_index = 0;
static int ticks_remaining = 0;

typedef struct {
    uint16_t freq;     
    uint16_t duration;  
} note_t;

/* MARIO */
// static const note_t melody[] = {
//     {E5, 3}, {E5, 3}, {REST, 3}, {E5, 3}, {REST, 3}, {C5, 3}, {E5, 3}, {REST, 3},
//     {G5, 6}, {REST, 6}, {G4, 6}, {REST, 6},

//     {C5, 5}, {REST, 2}, {G4, 5}, {REST, 2}, {E4, 5}, {REST, 2},
//     {A4, 4}, {B4, 4}, {AS4, 3}, {A4, 4},
//     {G4, 3}, {E5, 3}, {G5, 3}, {A5, 4}, {F5, 3}, {G5, 3},
//     {REST, 3}, {E5, 4}, {C5, 3}, {D5, 3}, {B4, 5}, {REST, 3},

//     {C5, 5}, {REST, 2}, {G4, 5}, {REST, 2}, {E4, 5}, {REST, 2},
//     {A4, 4}, {B4, 4}, {AS4, 3}, {A4, 4},
//     {G4, 3}, {E5, 3}, {G5, 3}, {A5, 4}, {F5, 3}, {G5, 3},
//     {REST, 3}, {E5, 4}, {C5, 3}, {D5, 3}, {B4, 5}, {REST, 5},

//     {REST, 4}, {G5, 3}, {FS5, 3}, {F5, 3}, {DS5, 4}, {E5, 4}, {REST, 3},
//     {GS4, 3}, {A4, 3}, {C5, 3}, {REST, 3}, {A4, 3}, {C5, 3}, {D5, 3},
//     {REST, 4}, {G5, 3}, {FS5, 3}, {F5, 3}, {DS5, 4}, {E5, 4}, {REST, 3},
//     {C6, 4}, {REST, 2}, {C6, 3}, {C6, 5}, {REST, 5},

//     {REST, 20}
// };

/* Police academy */
static const note_t melody[] = {
    {C4, 4}, {REST, 2}, {F4, 4}, {REST, 2}, {A4, 4}, {REST, 2},
    {C5, 6}, {A4, 4}, {F4, 4},
    {G4, 4}, {REST, 2}, {AS4, 4}, {REST, 2}, {G4, 4},
    {F4, 8}, {REST, 4},

    {C4, 4}, {F4, 6}, {A4, 4}, {C5, 8},
    {D5, 4}, {C5, 4}, {AS4, 4}, {A4, 4}, {G4, 6}, {REST, 2},
    {C4, 4}, {E4, 6}, {G4, 4}, {C5, 8},
    {D5, 4}, {C5, 4}, {B4, 4}, {C5, 4}, {D5, 6}, {REST, 2},

    {F5, 6}, {E5, 4}, {D5, 4}, {C5, 6}, {A4, 4},
    {AS4, 4}, {C5, 4}, {D5, 4}, {E5, 4}, {F5, 8},
    {C5, 4}, {A4, 4}, {G4, 6}, {F4, 10},

    {REST, 20}
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

static void start_note(const note_t& note) 
{
    if (note.freq == REST) {
        tone_off();
    } else {
        tone_on(note.freq);
    }
    ticks_remaining = note.duration;
}

static void player_tick_callback(void* arg) {
    ticks_remaining--;

    if (ticks_remaining <= 0) {
        current_note_index++;
        if (current_note_index >= MELODY_LEN) {
            current_note_index = 0;
        }
        start_note(melody[current_note_index]);
    }
}

extern "C" void app_main(void)
{
    buzzer_init();
    start_note(melody[0]);

    esp_timer_handle_t player_timer;
    esp_timer_create_args_t player_timer_args = {};
    player_timer_args.callback = &player_tick_callback;
    player_timer_args.name = "player_timer";
    esp_timer_create(&player_timer_args, &player_timer);
    esp_timer_start_periodic(player_timer, TICK_MS * 1000);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}