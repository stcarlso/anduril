// Hank Emisar/Noctigon Lume-X1 config options for Anduril with RGB button instead of aux
// Copyright (C) 2018-2026 Selene ToyKeeper, Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// For flashlights using the Loneoceans Lume-X1-40W boost driver (AVR32DD20)
// - Same firmware for 6V, 9V, or 12V configs

// same as hank lume-x1-avr32dd20 but with Hank-specific defaults
#include "hank/lume-x1/anduril.h"

#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50

// no aux, sorry, but the button lights up!
#define USE_AUXRGB_LEDS_WHILE_ON  25
#define DEFAULT_AUX_WHILE_ON  0b10
#define USE_AUX1_LED_WHILE_RAMPING

// blink numbers on the cyan LEDs by default
#ifdef DEFAULT_BLINK_CHANNEL
#undef DEFAULT_BLINK_CHANNEL
#endif
#define DEFAULT_BLINK_CHANNEL  CM_AUXCYN
