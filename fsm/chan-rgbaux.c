// channel modes for RGB aux LEDs
// Copyright (C) 2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#ifdef USE_AUX_RGB_ADV
static void set_rgbaux_level(uint8_t level, uint8_t hue, uint8_t sat) {
    aux_rgb_ramp_level = level;
    aux_rgb_active.hue = hue;
    aux_rgb_active.sat = sat;
    aux_rgb_active.pat = RGB_ADV_LEVEL_RAMP | RGB_ADV_MODE_FIXED;
    rgb_led_set(1);
}

void set_level_auxred(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_RED, 255);
}

void set_level_auxyel(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_YELLOW, 255);
}

void set_level_auxgrn(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_GREEN, 255);
}

void set_level_auxcyn(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_CYAN, 255);
}

void set_level_auxblu(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_BLUE, 255);
}

void set_level_auxprp(uint8_t level) {
    set_rgbaux_level(level, RGB_HUE_PURPLE, 255);
}

void set_level_auxwht(uint8_t level) {
    set_rgbaux_level(level, 0, 0);
}
#else
void set_level_auxred(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b000010);  // red, high (or off)
}

void set_level_auxyel(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b001010);  // red+green, high (or off)
}

void set_level_auxgrn(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b001000);  // green, high (or off)
}

void set_level_auxcyn(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b101000);  // green+blue, high (or off)
}

void set_level_auxblu(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b100000);  // blue, high (or off)
}

void set_level_auxprp(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b100010);  // red+blue, high (or off)
}

void set_level_auxwht(uint8_t level) {
    rgb_led_set(!(!(level)) * 0b101010);  // red+green+blue, high (or off)
}
#endif

bool gradual_tick_null(uint8_t gt) { return true; }  // do nothing

