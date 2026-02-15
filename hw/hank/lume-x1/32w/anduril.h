// Hank Emisar/Noctigon Lume-X1 config options for Anduril (32W)
// Copyright (C) 2018-2025 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// For flashlights using the Loneoceans Lume-X1-32W boost driver (AVR32DD20)
// - Same firmware for 6V, 9V, or 12V configs

// same as lume-x1 for Hank except for ramp tables
#include "hank/lume-x1/anduril.h"

#ifdef RAMP_SIZE
#undef RAMP_SIZE
#define RAMP_SIZE 143   // Reduced Ramp Table Size
#endif

// calibrated Ramp Table
#ifdef PWM1_LEVELS
#undef PWM1_LEVELS
#define PWM1_LEVELS 1, 2, 10, 31, 77, 159, 295, 504, 807, 504, 737, 1023, 14, 19, 25, 32, 41, 52, 65, 79, 96, 116, 139, 164, 194, 226, 263, 305, 351, 401, 458, 520, 588, 662, 744, 832, 929, 423, 470, 520, 574, 632, 694, 761, 832, 909, 991, 25, 27, 30, 33, 36, 39, 42, 45, 48, 51, 55, 59, 63, 67, 71, 76, 81, 86, 91, 96, 102, 108, 114, 121, 128, 135, 142, 149, 157, 165, 174, 183, 192, 201, 211, 221, 232, 242, 254, 265, 277, 289, 302, 315, 329, 343, 357, 372, 387, 403, 419, 435, 452, 470, 488, 506, 525, 545, 565, 586, 607, 628, 651, 673, 697, 721, 745, 770, 796, 822, 849, 877, 905, 934, 963, 994, 420, 432, 446, 459, 473, 487, 501, 516, 531, 546, 562, 578, 594, 610, 627, 644, 662, 680, 698, 716
#endif

#ifdef PWM2_LEVELS
#undef PWM2_LEVELS
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25
#endif

#ifdef RAMP_SMOOTH_CEIL
#undef RAMP_SMOOTH_CEIL
#define RAMP_SMOOTH_CEIL 119    // about ~15W
#endif

#ifdef SIMPLE_UI_CEIL
#undef SIMPLE_UI_CEIL
#define SIMPLE_UI_CEIL 107      // about ~10W
#endif
