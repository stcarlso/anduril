// Hank Emisar/Noctigon Lume-X1-C config options for Anduril (32W)
// Copyright (C) 2018-2026 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// For flashlights using the Loneoceans Lume-X1-C-32W boost driver (AVR32DD20)
// - Same firmware for 6V, 9V, or 12V configs

// same as lume-x1 for Hank except for ramp tables
#include "hank/lume-x1-c/anduril.h"

#ifdef RAMP_SIZE
#undef RAMP_SIZE
#define RAMP_SIZE 143   // Reduced Ramp Table Size
#endif

// calibrated Ramp Table
#ifdef PWM1_LEVELS
#undef PWM1_LEVELS
#define PWM1_LEVELS 1, 2, 10, 31, 76, 157, 291, 497, 796, 497, 727, 1023, 14, 19, 25, 32, 41, 51, 64, 78, 95, 115, 137, 162, 191, 223, 260, 300, 346, 396, 451, 513, 580, 653, 734, 821, 916, 1019, 463, 513, 566, 623, 685, 751, 821, 885, 965, 26, 28, 30, 32, 35, 38, 40, 44, 47, 50, 54, 58, 62, 66, 70, 75, 80, 85, 90, 96, 102, 108, 114, 121, 128, 135, 143, 150, 159, 167, 176, 185, 195, 205, 215, 226, 237, 248, 260, 272, 285, 298, 312, 326, 341, 356, 371, 387, 404, 421, 439, 457, 476, 495, 515, 535, 556, 578, 600, 623, 647, 671, 696, 722, 748, 775, 803, 832, 861, 891, 922, 954, 986, 1019, 432, 446, 461, 476, 491, 507, 523, 539, 556, 574, 591, 609, 628, 647, 666, 686, 706, 727, 748, 770, 796, 823
#endif

#ifdef PWM2_LEVELS
#undef PWM2_LEVELS
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25
#endif

#ifdef RAMP_SMOOTH_CEIL
#undef RAMP_SMOOTH_CEIL
#define RAMP_SMOOTH_CEIL 120    // about ~15W
#endif

#ifdef SIMPLE_UI_CEIL
#undef SIMPLE_UI_CEIL
#define SIMPLE_UI_CEIL 109      // about ~10W
#endif
