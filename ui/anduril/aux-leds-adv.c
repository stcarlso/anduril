// aux-leds-adv.c: Adv. Aux LED functions for Anduril.
// Copyright (C) 2017-2023 Selene ToyKeeper
//               2025-2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "anduril/aux-leds-adv.h"

#if defined(USE_INDICATOR_LED)
static const PROGMEM uint8_t indicator_blinking[] = {
    0, 1, 2, 1,  0, 0, 0, 0,
    0, 0, 1, 0,  0, 0, 0, 0
};

void indicator_led_update(uint8_t mode, uint8_t tick) {
    // turn off when battery is too low
    #ifdef DUAL_VOLTAGE_FLOOR
    if (((voltage < VOLTAGE_LOW) && (voltage > DUAL_VOLTAGE_FLOOR))
        || (voltage < DUAL_VOLTAGE_LOW_LOW)) {
    #else
    if (voltage < VOLTAGE_LOW) {
    #endif
        indicator_led(0);
    }
    //#ifdef USE_INDICATOR_LOW_BAT_WARNING
    #ifndef DUAL_VOLTAGE_FLOOR // this isn't set up for dual-voltage lights like the Sofirn SP10 Pro
    // fast blink a warning when battery is low but not critical
    else if (voltage < VOLTAGE_RED) {
        indicator_led(mode & (((tick & 0b0010)>>1) - 3));
    }
    #endif
    //#endif
    // normal steady output, 0/1/2 = off / low / high
    else if ((mode & 0b00001111) < 3) {
        indicator_led(mode);
    }
    // beacon-like blinky mode
    else {
        // fancy blink, set off/low/high levels here
        indicator_led(pgm_read_byte(indicator_blinking + (tick & 15)));
    }
}
#endif

#if defined(USE_AUX_RGB_LEDS) && defined(TICK_DURING_STANDBY)
void rgb_led_advance_color(rgb_aux_setting_t *cfg, uint16_t arg) {
    uint8_t hue = cfg->hue, pattern = cfg->pat, mode = pattern >> RGB_ADV_MODE_SHIFT;

    if (mode == RGB_ADV_MODE_FIXED && 0U == (arg & 0x1FU)){
        if (hue == 255U){
            mode = RGB_ADV_MODE_DISCO;
        }
        else{
            hue += 16U;
            // overflowed from max hue
            if (hue == 0U) hue = 255;
        }
    }
    else if (0U == (arg & 0x3FU)){
        mode++;
        hue = 0U;
        if (mode > RGB_ADV_MODE_VOLTAGE + 1){
            mode = RGB_ADV_MODE_FIXED;
        }
    }
    cfg->hue = hue;
    cfg->pat = (pattern & RGB_ADV_LEVEL_MASK) | (mode << RGB_ADV_MODE_SHIFT);
}

void rgb_led_advance_level(rgb_aux_setting_t *cfg) {
    uint8_t pattern = cfg->pat, level = pattern & RGB_ADV_LEVEL_MASK;

    level++;
    if (level > RGB_ADV_LEVEL_BREATHING){
        level = RGB_ADV_LEVEL_OFF;
    }
    cfg->pat = (pattern & ~RGB_ADV_LEVEL_MASK) | level;
}

static void voltage_to_rgb(uint8_t level) {
    /*
     * 0-9:   off
     * (dual voltage)
     *   9-16:  smooth hue ramp
     *   16-20: white
     *   20-29: off
     * 29-44: smooth hue ramp
     * 44+:   white
     */
    uint8_t volts = voltage, hue = 0, sat = 255;

    if (volts > 44U*dV){
        sat = 0;
    }
    else if (volts > 29U*dV){
        // 15*dV maps to ~225 hues, 225/dV is 45, 45/15 is 3
        hue = (volts - 29U*dV) * 3U;
    }
    #ifdef DUAL_VOLTAGE_FLOOR
        else if (volts > 20U*dV){
            level = RGB_ADV_LEVEL_OFF;
        }
        else if (volts > 16U*dV){
            sat = 0;
        }
        else if (volts > 9U*dV){
            // 7*dV maps to ~210 hues, 210/dV is 42, 42/7 is 6
            hue = (volts - 9U*dV) * 6U;
        }
    #endif
    else{
        level = RGB_ADV_LEVEL_OFF;
    }
    aux_rgb_active.hue = hue;
    aux_rgb_active.sat = sat;
    aux_rgb_active.pat = (RGB_ADV_MODE_VOLTAGE << RGB_ADV_MODE_SHIFT) | level;
}

rgb_aux_setting_t aux_rgb_active;
uint8_t aux_rgb_ramp_level;
static uint8_t aux_disco_frame = 0U;

static const PROGMEM uint8_t rgb_led_colors[] = {
    RGB_HUE_RED, RGB_HUE_YELLOW, RGB_HUE_GREEN, RGB_HUE_CYAN, RGB_HUE_BLUE, RGB_HUE_PURPLE
};

static inline void rgb_led_action(uint8_t pattern, uint8_t hue, uint8_t sat) {
    uint8_t mode = pattern >> RGB_ADV_MODE_SHIFT;

    if (mode == RGB_ADV_MODE_DISCO){
        aux_disco_frame = (aux_disco_frame + 1 + pseudo_rand() % 5) % 6;
        hue = pgm_read_byte(rgb_led_colors + aux_disco_frame);
        sat = 255;
    }

    // encode white as hue 255
    if (hue == 255) sat = 0;
    aux_rgb_active.hue = hue;
    aux_rgb_active.sat = sat;
    aux_rgb_active.pat = pattern;
}

void rgb_led_update(rgb_aux_setting_t *mode, uint16_t arg) {
    uint8_t hue = mode->hue, sat = mode->sat, pattern = mode->pat;
    // turn off aux LEDs when battery is empty
    // (but if voltage==0, that means we just booted and don't know yet)
    uint8_t volts = voltage;  // save a few bytes by caching volatile value

    #ifdef DUAL_VOLTAGE_FLOOR
    if ((volts) && (((voltage < VOLTAGE_LOW) && (voltage > DUAL_VOLTAGE_FLOOR)) || (voltage < DUAL_VOLTAGE_LOW_LOW))) {
    #else
    if ((volts) && (volts < VOLTAGE_LOW)) {
    #endif
        aux_rgb_active.pat = RGB_ADV_LEVEL_OFF;
        rgb_led_set(0);
    }
    else{
        uint8_t auxlvl = pattern & RGB_ADV_LEVEL_MASK;

        if (setting_rgb_mode_now){
            // always preview in high mode
            auxlvl = RGB_ADV_LEVEL_HIGH;
        }

        #ifdef USE_POST_OFF_VOLTAGE
        // use voltage high mode for a few seconds after initial poweroff
        // (but not after changing aux LED settings and other similar actions)
        if ((arg < (cfg.post_off_voltage * SLEEP_TICKS_PER_SECOND))
            && (ticks_since_on < (cfg.post_off_voltage * SLEEP_TICKS_PER_SECOND))
            && (ticks_since_on > 0)  // don't blink red on 1st frame
            ) {
            if (auxlvl != RGB_ADV_LEVEL_HIGH){
            // use high mode if regular aux level is high or prev level was high
                #ifdef USE_AUX_THRESHOLD_CONFIG
                // always high if configured for high aux
                // otherwise 0/1/2 depending on recent main LED brightness
                // (using >= makes it off by 1, but allows POVD at boot time)
                    auxlvl = (prev_level >= cfg.button_led_low_ramp_level)
                        << (prev_level > cfg.button_led_high_ramp_level);
                #else
                    if (prev_level >= POST_OFF_VOLTAGE_BRIGHTNESS)
                        auxlvl = RGB_ADV_LEVEL_HIGH;
                #endif
            }
            // force voltage mode, disable blink pattern
            pattern = RGB_ADV_MODE_VOLTAGE << RGB_ADV_MODE_SHIFT;
        }
        #endif
        pattern = (pattern & ~RGB_ADV_LEVEL_MASK) | auxlvl;

        if ((pattern >> RGB_ADV_MODE_SHIFT) >= RGB_ADV_MODE_VOLTAGE){
            if (setting_rgb_mode_now){
                // fast RGBRGB in preview mode
                hue = pgm_read_byte(rgb_led_colors + (((arg>>1) % 3) << 1));
                sat = 255;
            }
            else{
                // RGB needs to update every time since voltage can change
                voltage_to_rgb(auxlvl);
                rgb_led_set(0);
                return;
            }
        }

        if (pattern != aux_rgb_active.pat || (auxlvl != RGB_ADV_LEVEL_OFF && (hue != aux_rgb_active.hue || sat != aux_rgb_active.sat))){
            // Aux has changed
            rgb_led_action(pattern, hue, sat);
            rgb_led_set(0);
        }
    }
}

void rgb_led_voltage_readout(uint8_t bright) {
    #ifdef USE_CHANNEL_USES_AUX
    if (!channel_uses_aux(channel_mode)){
    #endif
    voltage_to_rgb(bright ? RGB_ADV_LEVEL_HIGH : RGB_ADV_LEVEL_LOW);
    rgb_led_set(1);
    #ifdef USE_CHANNEL_USES_AUX
    }
    #endif
}
#endif