// Hank Emisar/Noctigon Lume-N1 config options for Anduril
// Copyright (C) 2018-2026 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// For flashlights using the Loneoceans Lume-N1 Fusion Driver (AVR32DD20)
// Default config regulates to 24A in turbo

// same as loneoceans lume-n1 but with Hank-specific defaults
#include "loneoceans/lume-n1/anduril.h"
#include "hank/anduril.h"

#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50

#undef TURBO_TEMP_EXTRA
#define TURBO_TEMP_EXTRA 7

// disable beacontower mode for Hank
#ifdef USE_BEACONTOWER_MODE
#undef USE_BEACONTOWER_MODE
#endif

// set smooth ramping by default
#ifdef RAMP_STYLE
#undef RAMP_STYLE
#endif

// reset to anduril default number of steps
#ifdef RAMP_DISCRETE_STEPS
#undef RAMP_DISCRETE_STEPS
#endif
#ifdef SIMPLE_UI_STEPS
#undef SIMPLE_UI_STEPS
#endif
