#pragma once

#include "driver/ledc.h"

constexpr static int BUZZER_GPIO = 16;
constexpr static ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
constexpr static ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;
constexpr static ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;
constexpr static ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_10_BIT;

constexpr static int TICK_MS = 50;

constexpr static int REST = 0;
constexpr static int NOTE_C7 = 2093;
constexpr static int NOTE_D7 = 2349;
constexpr static int NOTE_E7 = 2637;
constexpr static int NOTE_F7 = 2794;
constexpr static int NOTE_G7 = 3136;
constexpr static int NOTE_A7 = 3520;
constexpr static int NOTE_B7 = 3951;
constexpr static int NOTE_C8 = 4186;

/* ---------------------------------------------------------------- октава 0 */
constexpr static int C0 = 16;
constexpr static int CS0 = 17;
constexpr static int D0 = 18;
constexpr static int DS0 = 19;
constexpr static int E0 = 21;
constexpr static int F0 = 22;
constexpr static int FS0 = 23;
constexpr static int G0 = 25;
constexpr static int GS0 = 26;
constexpr static int A0 = 28;
constexpr static int AS0 = 29;
constexpr static int B0 = 31;
 
/* ---------------------------------------------------------------- октава 1 */
constexpr static int C1 = 33;
constexpr static int CS1 = 35;
constexpr static int D1 = 37;
constexpr static int DS1 = 39;
constexpr static int E1 = 41;
constexpr static int F1 = 44;
constexpr static int FS1 = 46;
constexpr static int G1 = 49;
constexpr static int GS1 = 52;
constexpr static int A1 = 55;
constexpr static int AS1 = 58;
constexpr static int B1 = 62;
 
/* ---------------------------------------------------------------- октава 2 */
constexpr static int C2 = 65;
constexpr static int CS2 = 69;
constexpr static int D2 = 73;
constexpr static int DS2 = 78;
constexpr static int E2 = 82;
constexpr static int F2 = 87;
constexpr static int FS2 = 92;
constexpr static int G2 = 98;
constexpr static int GS2 = 104;
constexpr static int A2 = 110;
constexpr static int AS2 = 117;
constexpr static int B2 = 123;
 
/* ---------------------------------------------------------------- октава 3 */
constexpr static int C3 = 131;
constexpr static int CS3 = 139;
constexpr static int D3 = 147;
constexpr static int DS3 = 156;
constexpr static int E3 = 165;
constexpr static int F3 = 175;
constexpr static int FS3 = 185;
constexpr static int G3 = 196;
constexpr static int GS3 = 208;
constexpr static int A3 = 220;
constexpr static int AS3 = 233;
constexpr static int B3 = 247;

/* ---------------------------------------------------------------- октава 4 */

constexpr static int C4 = 262;
constexpr static int CS4 = 277;
constexpr static int D4 = 294;
constexpr static int DS4 = 311;
constexpr static int E4 = 330;
constexpr static int F4 = 349;
constexpr static int FS4 = 370;
constexpr static int G4 = 392;
constexpr static int GS4 = 415;
constexpr static int A4 = 440;       /* еталон міжнародного настроювання */
constexpr static int AS4 = 466;
constexpr static int B4 = 494;
 
/* ---------------------------------------------------------------- октава 5 */
constexpr static int C5 = 523;
constexpr static int CS5 = 554;
constexpr static int D5 = 587;
constexpr static int DS5 = 622;
constexpr static int E5 = 659;
constexpr static int F5 = 698;
constexpr static int FS5 = 740;
constexpr static int G5 = 784;
constexpr static int GS5 = 831;
constexpr static int A5 = 880;
constexpr static int AS5 = 932;
constexpr static int B5 = 988;
 
/* ---------------------------------------------------------------- октава 6 */
constexpr static int C6 = 1047;
constexpr static int CS6 = 1109;
constexpr static int D6 = 1175;
constexpr static int DS6 = 1245;
constexpr static int E6 = 1319;
constexpr static int F6 = 1397;
constexpr static int FS6 = 1480;
constexpr static int G6 = 1568;
constexpr static int GS6 = 1661;
constexpr static int A6 = 1760;
constexpr static int AS6 = 1865;
constexpr static int B6 = 1976;
 
/* ---------------------------------------------------------------- октава 7 */
constexpr static int C7 = 2093;
constexpr static int CS7 = 2217;
constexpr static int D7 = 2349;
constexpr static int DS7 = 2489;
constexpr static int E7 = 2637;
constexpr static int F7 = 2794;
constexpr static int FS7 = 2960;
constexpr static int G7 = 3136;
constexpr static int GS7 = 3322;
constexpr static int A7 = 3520;
constexpr static int AS7 = 3729;
constexpr static int B7 = 3951;