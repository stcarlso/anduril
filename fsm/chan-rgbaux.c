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
    rgb_led_set(0b000001 << !(!(level)));  // red, high (level > 0) or low
}

void set_level_auxyel(uint8_t level) {
    rgb_led_set(0b000101 << !(!(level)));  // red+green, high (level > 0) or low
}

void set_level_auxgrn(uint8_t level) {
    rgb_led_set(0b000100 << !(!(level)));  // green, high (level > 0) or low
}

void set_level_auxcyn(uint8_t level) {
    rgb_led_set(0b010100 << !(!(level)));  // green+blue, high (level > 0) or low
}

void set_level_auxblu(uint8_t level) {
    rgb_led_set(0b010000 << !(!(level)));  // blue, high (level > 0) or low
}

void set_level_auxprp(uint8_t level) {
    rgb_led_set(0b010001 << !(!(level)));  // red+blue, high (level > 0) or low
}

void set_level_auxwht(uint8_t level) {
    rgb_led_set(0b010101 << !(!(level)));  // red+green+blue, high (level > 0) or low
}
#endif

bool gradual_tick_null(uint8_t gt) { return true; }  // do nothing

