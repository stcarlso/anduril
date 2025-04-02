// Hank Emisar/Noctigon Lume-X1 config options for Anduril with RGB button instead of aux
// Copyright (C) 2018-2024 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// For flashlights using the Loneoceans Lume-X1-40W boost driver (AVR32DD20)
// - Same firmware for 6V, 9V, or 12V configs

// same as loneoceans lume-x1-avr32dd20 but with Hank-specific defaults
#include "loneoceans/lume-x1-avr32dd20/anduril.h"
#include "hank/anduril.h"

#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50

// no aux, sorry, but the button lights up!
#define USE_AUX_RGB_LEDS_WHILE_ON  25
#define USE_INDICATOR_LED_WHILE_RAMPING

// disable beacontower mode
#ifdef USE_BEACONTOWER_MODE
#undef USE_BEACONTOWER_MODE
#endif

// blink numbers on the green LEDs by default
#ifdef DEFAULT_BLINK_CHANNEL
#undef DEFAULT_BLINK_CHANNEL
#endif
#define DEFAULT_BLINK_CHANNEL  CM_AUXGRN
