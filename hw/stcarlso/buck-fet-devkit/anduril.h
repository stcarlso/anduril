// stcarlso's avr32dd20 buck+FET devkit board
// Copyright (C) 2024 stcarlso, Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#define HWDEF_H stcarlso/buck-fet-devkit/hwdef.h

// HPRsense : 1.7+0.3+5 = 10mR (HSCE2530+trace resistance+5mR)
// LPRsense : 1R

#define RAMP_SIZE 150

// 4 ramp segments:
//   - low  1.024V
//   - low  2.5  V
//   - high 1.024V
//   - high 2.5  V
// HDR ratio: 143
// level_calc.py 4.3287 1 150 7135 1 0.01 2000 --pwm 357143

// top level for each "gear": 32 41 120 150
#define PWM1_LEVELS \
           1,   2,   4,   5,   8,  11,  14,  19,  25,  32,  40,  50,  62,  75,  91, 109, 130, 154, 181, 211, 245, 283, 325, 373, 425, 483, 547, 617, 693, 777, 868, 968, \
         440, 488, 540, 596, 656, 721, 791, 865, 946, \
          17,  19,  20,  22,  24,  26,  28,  30,  33,  35,  38,  41,  44,  47,  50,  54,  58,  61,  66,  70,  74,  79,  84,  89,  95, 100, 106, 112, 119, 125, 132, 140, 147, 155, 164, 172, 181, 190, 200, 210, 220, 231, 242, 254, 266, 278, 291, 304, 318, 332, 347, 362, 378, 394, 411, 428, 446, 465, 484, 503, 524, 545, 566, 588, 611, 634, 659, 683, 709, 735, 762, 790, 819, 848, 878, 909, 941, 974,1007, \
         426, 441, 456, 471, 486, 502, 519, 536, 553, 570, 589, 607, 626, 645, 665, 686, 706, 728, 749, 772, 794, 818, 842, 866, 891, 916, 942, 969, 996,1023
#define PWM2_LEVELS \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25
#define MAX_1x7135            41
#define HDR_ENABLE_LEVEL_MIN  42

#define DEFAULT_LEVEL         30

// no PWM, so MCU clock speed can be slow
#define HALFSPEED_LEVEL      42
#define QUARTERSPEED_LEVEL   41

#define RAMP_SMOOTH_FLOOR    1
#define RAMP_SMOOTH_CEIL     120
// 1 [30] 60 90 130
#define RAMP_DISCRETE_FLOOR  1
#define RAMP_DISCRETE_CEIL   120
#define RAMP_DISCRETE_STEPS  5

// 1 [30] 60 90 130
#define SIMPLE_UI_FLOOR      1
#define SIMPLE_UI_CEIL       120
#define SIMPLE_UI_STEPS      5

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

// thermal config

// temperature limit
#define THERM_FASTER_LEVEL  130 // stop panicking at 50%/3A
#define MIN_THERM_STEPDOWN  MAX_1x7135


// UI

#define SIMPLE_UI_ACTIVE 0 // advanced UI by default, because it's a dev board

// Allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

#define DEFAULT_2C_STYLE 1 // enable 2 click turbo


// AUX

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS
// turn on the aux LEDs while main LEDs are on
// because this is a dev board and it's useful to see that
#define USE_AUX_RGB_LEDS_WHILE_ON  20
#define USE_INDICATOR_LED_WHILE_RAMPING

// show each channel while it scroll by in the menu
#define USE_CONFIG_COLORS

// blink numbers on the main LEDs by default
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

// use aux red + aux blue for police strobe
#define USE_POLICE_COLOR_STROBE_MODE
#define POLICE_STROBE_USES_AUX
#define POLICE_COLOR_STROBE_CH1        CM_AUXRED
#define POLICE_COLOR_STROBE_CH2        CM_AUXBLU


// Misc

#define PARTY_STROBE_ONTIME 1  // slow down party strobe
#define STROBE_OFF_LEVEL 1  // keep the regulator chip on between pulses
#define BLIP_LEVEL 1  // same

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 40

// enable 13H factory reset so it can be used on tail e-switch lights
#define USE_SOFT_FACTORY_RESET
