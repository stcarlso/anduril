// Emisar D1 12V boost config options for Anduril
// Copyright (C) 2021-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "hank/emisar-d1v2/boost/hwdef.h"
#include "hank/anduril.h"

#define RAMP_SIZE 150

// power channels:
// - boost: 8A?

// no aux, sorry, but the button lights up!
#define USE_INDICATOR_LED
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

// blink numbers on the main LEDs by default (but allow user to change it)
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

// off mode: high (2)
// lockout: low (1)
#define INDICATOR_LED_DEFAULT_MODE ((1<<2) + 2)

// delta-sigma modulated PWM (0b0HHHHHHHHLLLLLLL = 0, 8xHigh, 7xLow bits)
// level_calc.py 5.01 1 150 7135 0 0.2 2000 --pwm 32640
// (max is (255 << 7), because it's 8-bit PWM plus 7 bits of DSM)
#define PWM1_LEVELS 0,1,2,3,4,5,6,7,9,10,12,14,17,19,22,25,28,32,36,41,45,50,56,62,69,76,84,92,101,110,121,132,143,156,169,184,199,215,232,251,270,291,313,336,360,386,414,442,473,505,539,574,612,651,693,736,782,829,880,932,987,1045,1105,1168,1233,1302,1374,1449,1527,1608,1693,1781,1873,1969,2068,2172,2279,2391,2507,2628,2753,2883,3018,3158,3303,3454,3609,3771,3938,4111,4289,4475,4666,4864,5068,5280,5498,5724,5957,6197,6445,6701,6965,7237,7518,7808,8106,8413,8730,9056,9392,9737,10093,10459,10835,11223,11621,12031,12452,12884,13329,13786,14255,14737,15232,15741,16262,16798,17347,17911,18489,19082,19691,20314,20954,21609,22281,22969,23674,24397,25137,25895,26671,27465,28279,29111,29963,30835,31727,32640
#define MIN_THERM_STEPDOWN 50
#define DEFAULT_LEVEL 30
#define MAX_1x7135 150
// moon at 1/4th speed for max runtime
// the rest at half speed, to reduce ripple
// but not full speed because 8 kHz PWM is enough for this circuit
// and speed changes make a big visible bump
#define HALFSPEED_LEVEL     255
#define QUARTERSPEED_LEVEL  2

#define RAMP_SMOOTH_FLOOR 1  // low levels may be unreliable
#define RAMP_SMOOTH_CEIL  120

// 1 [30] 60 90 120
#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL  RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 5

// safe limit ~75% power
#define SIMPLE_UI_FLOOR RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL RAMP_DISCRETE_CEIL
#define SIMPLE_UI_STEPS 5

#define DEFAULT_MANUAL_MEMORY 30
#define DEFAULT_MANUAL_MEMORY_TIMER 5

// stop panicking at ~70% power or ~600 lm
#define THERM_FASTER_LEVEL 130

#define THERM_CAL_OFFSET 5

// the power regulator seems to "jump start" the LEDs all on its own,
// so the firmware doesn't have to
// (and unfortunately the power regulator jumps it a bit too hard)
#define DEFAULT_JUMP_START_LEVEL 1
#define BLINK_BRIGHTNESS 50
#define BLINK_ONCE_TIME 12

// slow down party strobe; this driver can't pulse for 2ms or less
#define PARTY_STROBE_ONTIME 3

// the default of 26 looks a bit rough, so increase it to make it smoother
#define CANDLE_AMPLITUDE 30

// don't blink while ramping
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

// added for convenience
#define USE_SOFT_FACTORY_RESET

