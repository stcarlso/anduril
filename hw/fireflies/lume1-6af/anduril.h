// FireflyLite (Fireflies) config options for Anduril
// Copyright (C) 2018-2024 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// For flashlights using the Loneoceans Lume1-6AF driver (ATTINY1616)
// - NOTE: some flashlights with single or smaller emitters require the no-fet variant of firmware!
// - Models include: E07X Pro, E12R, X1S, X1L, NOV-Mu, T1R, T9R

// same as Loneoceans Lume1-6AF buck driver but with FireflyLite specific config
#include "loneoceans/lume1-6af/anduril.h"

#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50

#undef THERM_RESPONSE_MAGNITUDE
#define THERM_RESPONSE_MAGNITUDE 64

// Disable default loneoceans values
#undef DEFAULT_BLINK_CHANNEL
#undef DEFAULT_LEVEL
#undef RAMP_SMOOTH_FLOOR
#undef RAMP_SMOOTH_CEIL
#undef RAMP_DISCRETE_FLOOR
#undef RAMP_DISCRETE_CEIL
#undef RAMP_DISCRETE_STEPS
#undef SIMPLE_UI_FLOOR
#undef SIMPLE_UI_CEIL
#undef SIMPLE_UI_STEPS
#undef RGB_LED_OFF_DEFAULT
#undef RGB_LED_LOCKOUT_DEFAULT
#undef BLINK_BRIGHTNESS

// blink numbers on the aux LEDs by default (but allow user to change it)
#define DEFAULT_BLINK_CHANNEL  CM_AUXWHT

#define DEFAULT_LEVEL      41
// Default lowest is 9
#define RAMP_SMOOTH_FLOOR  5
#define RAMP_SMOOTH_CEIL   149
// 5 [41] 77 113 149
#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL  RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 5

// 5 [41] 77 113 149
#define SIMPLE_UI_FLOOR  RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL   RAMP_DISCRETE_CEIL
#define SIMPLE_UI_STEPS  5

#define DEFAULT_MANUAL_MEMORY 41
#define DEFAULT_MANUAL_MEMORY_TIMER 5

#define BLINK_BRIGHTNESS 41

// low, battery
#define RGB_LED_OFF_DEFAULT 0x19
// low, red
#define RGB_LED_LOCKOUT_DEFAULT 0x10

// Allow 3C (or 6C) in Simple UI (toggle smooth or stepped ramping)
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// No post-off voltage display by default
#define DEFAULT_POST_OFF_VOLTAGE_SECONDS 0
