// stcarlso's config options for Anduril
// Copyright (C) 2021-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#ifdef RGB_LED_OFF_DEFAULT
#undef RGB_LED_OFF_DEFAULT
#endif
// Low, cyan
#define RGB_LED_OFF_DEFAULT 0x13
// Low, red
#define RGB_LED_LOCKOUT_DEFAULT 0x10

// Off mode: low (1)
// Lockout: low (1)
#ifdef INDICATOR_LED_DEFAULT_MODE
#undef INDICATOR_LED_DEFAULT_MODE
#endif
#define INDICATOR_LED_DEFAULT_MODE ((1<<2) + 1)

// Half a second per color in rainbow mode
#define RGB_RAINBOW_SPEED 0x03

// Allow 3C (or 6C) in Simple UI (toggle smooth or stepped ramping)
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// Double click while on goes to full-power turbo, not ramp ceiling
#define DEFAULT_2C_STYLE 1

#define USE_SOFT_FACTORY_RESET

// No post-off voltage display by default
#define DEFAULT_POST_OFF_VOLTAGE_SECONDS 0
