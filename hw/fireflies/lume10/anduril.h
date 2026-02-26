// Fireflies config options for Anduril
// Copyright (C) 2018-2026 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// for flashlights using the Loneoceans Lume10 driver (AVR32dd20)
// - note: some flashlights with single or smaller emitters require the no-fet variant of firmware!

// same as Loneoceans Lume10 driver but with Fireflies specific config
#include "loneoceans/lume10/anduril.h"

#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50

#undef TURBO_TEMP_EXTRA
#define TURBO_TEMP_EXTRA 8  // Lume10 flashlights can handle the heat

#undef RAMP_SMOOTH_FLOOR
#define RAMP_SMOOTH_FLOOR 8 // about 85uA
