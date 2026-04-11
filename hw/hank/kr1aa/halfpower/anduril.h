// Emisar / Noctigon KR1AA config options for Anduril
// Copyright (C) 2026 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#define HWDEF_H hank/emisar-d3aa/hwdef.h
#include "hank/anduril.h"
#include "hank/emisar-d3aa/anduril.h"

// wait this many ms at boot before testing the battery
// (or doing anything else)
// (to give the user more time to make a solid electrical connection)
#define WEAK_BATTERY_TEST_DELAY  500

// 4 ramp segments:
//   - low  1.024V
//   - low  2.5  V
//   - high 1.024V
//   - high 2.5  V
// 2nd gear starts at 24:
// ./bin/dac-scale.py $( ./bin/level_calc.py 3.856 1 150 7135 3 0.01 1400 --pwm 273200 | grep PWM1 | cut -d : -f 2- )
#undef PWM1_LEVELS
#undef PWM2_LEVELS
#undef MAX_1x7135
#undef HDR_ENABLE_LEVEL_MIN

// Generated a 170 step ramp and chopped off the top 20
// top level for each "gear": 33 44 131 166
#define PWM1_LEVELS \
           3,   4,   6,   8,  11,  14,  18,  23,  29,  36,  45,  55,  66,  79,  94, 111, 131, 153, 177, 205, 236, 270, 307, 349, 395, 445, 500, 559, 624, 695, 772, 854, 944, \
         425, 468, 513, 562, 614, 670, 729, 793, 860, 932,1008, \
          19,  21,  22,  24,  26,  28,  30,  32,  34,  37,  39,  42,  45,  48,  51,  54,  57,  61,  64,  68,  72,  76,  80,  85,  89,  94,  99, 105, 110, 116, 122, 128, 134, 140, 147, 154, 162, 169, 177, 185, 193, 202, 211, 220, 230, 239, 250, 260, 271, 282, 293, 305, 317, 330, 343, 356, 370, 384, 398, 413, 428, 444, 460, 477, 494, 511, 529, 548, 566, 586, 606, 626, 647, 668, 690, 713, 736, 759, 783, 808, 833, 859, 886, 913, 941, 969, 998, \
         421, 433, 446, 459, 472, 486, 499, 514, 528, 543, 558, 573, 589, 605, 621, 638, 655, 672, 690
#define PWM2_LEVELS \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
         V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
         V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25
#define MAX_1x7135            44
#define HDR_ENABLE_LEVEL_MIN  45

// no PWM, so MCU clock speed can be slow
#undef HALFSPEED_LEVEL
#define HALFSPEED_LEVEL      41
#undef QUARTERSPEED_LEVEL
#define QUARTERSPEED_LEVEL   40  // seems to run fine at 10kHz/4, try reducing more?

// AUX

// there is no lighted button
#ifdef USE_BUTTON_LED
#undef USE_BUTTON_LED
#endif
