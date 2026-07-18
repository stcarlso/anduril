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
// Originally level_calc.py 5.01 1 150 7135 0 0.2 2000 --pwm 32640
// New max is half (128 << 7) = 16384 so use level_calc.py 5.0 1 177 7135 0 0.2 2000 --pwm 32640 and chop off the upper 27 values
#define PWM1_LEVELS 0,1,2,3,4,5,6,7,8,9,10,11,13,14,16,19,21,23,26,29,32,35,39,43,47,52,56,62,67,73,79,86,93,101,109,117,126,136,146,157,168,180,193,206,220,235,251,267,284,302,321,341,362,384,407,431,456,483,510,539,569,601,633,667,703,740,779,819,861,904,950,997,1045,1096,1149,1204,1261,1320,1381,1444,1510,1578,1648,1721,1797,1875,1956,2039,2126,2215,2307,2403,2501,2603,2708,2816,2928,3043,3162,3284,3410,3541,3675,3813,3955,4101,4252,4407,4567,4731,4900,5073,5252,5436,5624,5818,6017,6222,6432,6647,6869,7096,7329,7569,7814,8066,8325,8590,8861,9140,9425,9718,10017,10324,10639,10961,11291,11629,11975,12329,12691,13062,13441,13830,14227,14633,15048,15473,15908,16352
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

