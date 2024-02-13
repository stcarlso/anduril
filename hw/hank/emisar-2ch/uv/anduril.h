// Emisar 2-channel UV special config options for Anduril
// Copyright (C) 2021-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "hank/emisar-2ch/uv/hwdef.h"
#include "hank/anduril.h"

// no aux, sorry, but the button lights up!
#define USE_INDICATOR_LED
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

// off mode: high (2)
// lockout: low (1)
#define INDICATOR_LED_DEFAULT_MODE ((1<<2) + 2)

// channel modes...
// CM_CH1, CM_CH2, CM_BOTH, CM_BLEND, CM_AUTO
// enable max brightness out of the box
#define DEFAULT_CHANNEL_MODE           CM_CH1

#define USE_CONFIG_COLORS

// blink numbers on the aux LEDs by default (but allow user to change it)
#define DEFAULT_BLINK_CHANNEL  CM_CH1

// how much to increase total brightness at middle tint
// (0 = 100% brightness, 64 = 200% brightness)
#define TINT_RAMPING_CORRECTION 0  // none, linear regulator doesn't need it


// channel 1
//   output: unknown, 2000 lm?
//   FET: absent / unused
// channel 2
//   output: unknown, 2000 lm?
#define RAMP_SIZE 150

// abstract ramp (power is split between both sets of LEDs)
// 1-130: 0 to 100% power
// level_calc.py 5.01 1 130 7135 2 0.2 2000 --pwm dyn:64:16383:511
// 131 to 150: 101% to 200% power
// level_calc.py 6.44 1 150 7135 1 0.2 2000 --pwm dyn:74:16383:1022
#define PWM1_LEVELS 1,1,1,2,2,2,3,3,4,5,5,6,7,8,9,10,11,12,13,15,16,18,19,21,22,24,26,28,29,31,33,35,37,40,42,44,46,48,50,52,54,56,58,59,61,62,63,64,65,66,66,66,66,65,64,62,60,58,54,50,46,41,35,28,20,21,22,24,25,26,27,29,30,32,33,35,37,38,40,42,44,46,48,50,53,55,57,60,63,65,68,71,74,77,80,83,87,90,94,98,102,106,110,114,118,123,128,132,137,142,148,153,159,164,170,176,183,189,196,202,209,216,224,231,239,247,255,263,272,281,290,299,309,318,328,339,349,360,371,382,394,406,418,430,443,456,469,483,497,511
#define PWM_TOPS 16383,13650,10715,15388,11902,8195,12771,9834,12258,13423,11192,11947,12284,12363,12271,12064,11775,11428,11039,11469,10973,11132,10595,10601,10054,9971,9833,9655,9122,8923,8704,8473,8232,8196,7932,7668,7408,7152,6901,6656,6417,6186,5961,5647,5444,5163,4899,4653,4422,4206,3941,3697,3470,3210,2971,2707,2466,2245,1968,1717,1489,1251,1005,756,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511
#define DEFAULT_LEVEL      32
#define MAX_1x7135         150
#define HALFSPEED_LEVEL    10
#define QUARTERSPEED_LEVEL 2

#define RAMP_SMOOTH_FLOOR  2  // level 1 is unreliable (?)
#define RAMP_SMOOTH_CEIL   122
// 2 [32] 62 92 122
#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL  RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 5

#define SIMPLE_UI_FLOOR  RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL   RAMP_DISCRETE_CEIL
#define SIMPLE_UI_STEPS  5

#define DEFAULT_MANUAL_MEMORY 32
#define DEFAULT_MANUAL_MEMORY_TIMER 5

// stop panicking at ~1500 lm
#define THERM_FASTER_LEVEL 130
#define MIN_THERM_STEPDOWN 65  // should be above highest dyn_pwm level

#undef  TACTICAL_LEVELS
#define TACTICAL_LEVELS 130,30,(RAMP_SIZE+3)  // high, low, police strobe

// use the brightest setting for strobe
#define STROBE_BRIGHTNESS MAX_LEVEL
// slow down party strobe; this driver can't pulse for 1ms or less
#define PARTY_STROBE_ONTIME 2

// the default of 26 looks a bit flat, so increase it
#define CANDLE_AMPLITUDE 33

// the power regulator is a bit slow, so push it harder for a quick response from off
#define DEFAULT_JUMP_START_LEVEL 21
#define BLINK_BRIGHTNESS 32
#define BLINK_ONCE_TIME 12  // longer blink, since main LEDs are slow

#define THERM_CAL_OFFSET 5

// don't blink while ramping
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif
