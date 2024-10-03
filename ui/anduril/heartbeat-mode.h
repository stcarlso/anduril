// heartbeat-mode.h: Heartbeat/Pulse mode for Anduril.
// Copyright (C) 2017-2023 Selene ToyKeeper
//               2024      stcarlso
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// heartbeat mode

// heartbeat flash timing (30FPM default)
uint8_t heartbeat_seconds = 2;

// heartbeat mode
uint8_t heartbeat_state(Event event, uint16_t arg);

inline void heartbeat_mode_iter();
