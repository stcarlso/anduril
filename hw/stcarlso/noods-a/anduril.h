// Driver for Adafruit Noods LEDs RevA config options for Anduril
// Copyright (C) 2024 Stephen Carlson
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "stcarlso/noods-a/hwdef.h"
#include "stcarlso/anduril.h"

#define RAMP_SIZE 150

// No aux, sorry, but the button lights up!
#define USE_INDICATOR_LED
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

// Off mode: blinking (3)
// Lockout: low (1)
#ifdef INDICATOR_LED_DEFAULT_MODE
#undef INDICATOR_LED_DEFAULT_MODE
#endif
#define INDICATOR_LED_DEFAULT_MODE ((1<<2) + 3)

// generated using spreadsheet
#define PWM1_LEVELS 30,55,55,55,56,56,56,57,57,57,58,58,58,59,59,110,110,110,111,111,111,112,112,113,113,113,114,114,114,115,115,115,116,116,116,117,117,117,118,118,118,118,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,119,119,119,119,118,118,118,117,117,117,116,116,115,116,116,117,118,118,119,120,121,121,122,123,124,125,126,127,128,129,130,131,132,133,135,136,137,138,140,141,143,144,146,147,149,151,153,154,156,158,160,162,164,166,169,171,173,175,178,180,183,186,188,191,194,197,200,203,206,209,213,216,220,223,227,231,234,238,242,246,251,255
#define PWM_TOPS 4095,3956,3821,3691,3565,3443,3326,3213,3103,2998,2896,2797,2702,2610,2521,2435,2352,2272,2194,2120,2048,1978,1910,1845,1782,1722,1663,1606,1552,1499,1448,1398,1351,1305,1260,1217,1176,1136,1097,1060,1024,989,955,923,891,861,832,803,776,749,724,699,675,652,630,609,588,568,549,530,512,494,478,461,446,430,416,402,388,375,362,350,338,326,315,304,294,284,274,265,256,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
#define MIN_THERM_STEPDOWN 60
#define DEFAULT_LEVEL 60
#define MAX_1x7135 150
#define HALFSPEED_LEVEL 16
#define QUARTERSPEED_LEVEL 2

#define RAMP_SMOOTH_FLOOR    1
#define RAMP_SMOOTH_CEIL     120
// 1 30 [60] 90 120
#define RAMP_DISCRETE_FLOOR  RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL   RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS  5

// 1 30 [60] 90 120
#define SIMPLE_UI_FLOOR  RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL   RAMP_DISCRETE_CEIL
#define SIMPLE_UI_STEPS  RAMP_DISCRETE_STEPS

#define DEFAULT_MANUAL_MEMORY 60
#define DEFAULT_MANUAL_MEMORY_TIMER 5

// Stop panicking at ~70% power
#define THERM_FASTER_LEVEL 120

// Puts the frequency of whine at > 5 KHz
#define BLINK_BRIGHTNESS 42
#define BLINK_ONCE_TIME 12

// Blink numbers on the main string by default (but allow user to change it)
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

#define CANDLE_AMPLITUDE 36
#define USE_HEARTBEAT_MODE
#define HEARTBEAT_MODE_MIN 8

#define USE_SMOOTH_STEPS

// don't blink while ramping
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif
