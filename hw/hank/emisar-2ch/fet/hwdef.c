// Emisar generic 2-channel + DD FET w/ tint ramping
// Copyright (C) 2021-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "fsm/chan-rgbaux.c"


void set_level_zero();

void set_level_ch1(uint8_t level);
void set_level_ch2(uint8_t level);
void set_level_both(uint8_t level);
void set_level_blend(uint8_t level);
void set_level_auto(uint8_t level);

bool gradual_tick_ch1(uint8_t gt);
bool gradual_tick_ch2(uint8_t gt);
bool gradual_tick_both(uint8_t gt);
bool gradual_tick_blend(uint8_t gt);
bool gradual_tick_auto(uint8_t gt);


Channel channels[] = {
    { // channel 1 only
        .set_level    = set_level_ch1,
        .gradual_tick = gradual_tick_ch1,
        .has_args     = 0
    },
    { // channel 2 only
        .set_level    = set_level_ch2,
        .gradual_tick = gradual_tick_ch2,
        .has_args     = 0
    },
    { // both channels, tied together (max "200%" power)
        .set_level    = set_level_both,
        .gradual_tick = gradual_tick_both,
        .has_args     = 0
    },
    { // both channels, manual blend (max "100%" power)
        .set_level    = set_level_blend,
        .gradual_tick = gradual_tick_blend,
        .has_args     = 1
    },
    { // both channels, auto blend
        .set_level    = set_level_auto,
        .gradual_tick = gradual_tick_auto,
        .has_args     = 1
    },
    RGB_AUX_CHANNELS
};


void set_level_zero() {
    CH1_PWM = 0;  // linear
    CH2_PWM = 0;  // linear
    CH3_PWM = 0;  // DD FET
    PWM_TOP = PWM_TOP_INIT;
    PWM_CNT = 0;
    CH1_ENABLE_PORT &= ~(1 << CH1_ENABLE_PIN);  // disable opamp
    CH2_ENABLE_PORT &= ~(1 << CH2_ENABLE_PIN);  // disable opamp
}

// set new values for both channels,
// handling any possible combination
// and any before/after state
void set_pwms(uint16_t level, uint8_t ch1_pwm, uint8_t ch2_pwm, uint8_t ch3_pwm, uint16_t top) {
    bool was_on = (CH1_PWM>0) | (CH2_PWM>0) | (CH3_PWM>0);
    bool now_on = (ch1_pwm>0) | (ch2_pwm>0) | (ch3_pwm>0);
#ifdef USE_ULTRA_LOW_MODE
    level_1_mode_e mode = cfg.ultra_low_mode;
#endif

    if (! now_on) {
        set_level_zero();
        return;
    }

#if defined(USE_ULTRA_LOW_MODE) && defined(USE_AUX_RGB_LEDS)
    if (level == 0 && mode == level_1_redaux) {
        set_level_auxred(1);
        set_level_zero();
        return;
    }
#endif

    if (ch1_pwm)
        CH1_ENABLE_PORT |= (1 << CH1_ENABLE_PIN);  // enable opamp
    else
        CH1_ENABLE_PORT &= ~(1 << CH1_ENABLE_PIN);  // disable opamp

    if (ch2_pwm)
        CH2_ENABLE_PORT |= (1 << CH2_ENABLE_PIN);  // enable opamp
    else
        CH2_ENABLE_PORT &= ~(1 << CH2_ENABLE_PIN);  // disable opamp

    if (0) { }
#ifdef USE_ULTRA_LOW_MODE
    else if (level == 0 && mode >= level_1_s0) {
        CH1_PWM = 0;
        CH2_PWM = 0;
        CH3_PWM = 0;
    }
#endif
    else {
        CH1_PWM = ch1_pwm;
        CH2_PWM = ch2_pwm;
        CH3_PWM = ch3_pwm;
    }

    // manual phase sync when changing level while already on
    if (was_on && now_on) while(PWM_CNT > (top - 32)) {}

    PWM_TOP = top;

    // reset phase when turning on or off
    //if ((! was_on) | (! now_on)) PWM_CNT = 0;
    if (! was_on) PWM_CNT = 0;
}

void set_level_ch1(uint8_t level) {
    uint8_t  pwm1 = PWM_GET8 (pwm1_levels, level);
    uint8_t  pwm3 = PWM_GET8 (pwm2_levels, level);
    uint16_t top  = PWM_GET16(pwm3_levels, level);

    set_pwms(level, pwm1, 0, pwm3, top);
}

void set_level_ch2(uint8_t level) {
    uint8_t  pwm2 = PWM_GET8 (pwm4_levels, level);
    uint16_t top  = PWM_GET16(pwm5_levels, level);

    set_pwms(level, 0, pwm2, 0, top);
}

void set_level_both(uint8_t level) {
    uint8_t  pwm1 = PWM_GET8 (pwm1_levels, level);
    uint8_t  pwm3 = PWM_GET8 (pwm2_levels, level);
    uint16_t top  = PWM_GET16(pwm3_levels, level);

    set_pwms(level, pwm1, pwm1, pwm3, top);
}

void set_level_blend(uint8_t level) {
    uint16_t pwm1, pwm2;
    uint8_t  pwm3       = PWM_GET8 (pwm2_levels, level);  // DD FET
    //uint16_t brightness = PWM_GET8 (pwm1_levels, level) << 1;
    uint16_t brightness = PWM_GET8 (pwm1_levels, level) + pwm3;
    uint16_t top        = PWM_GET16(pwm3_levels, level);
    uint8_t  blend      = cfg.channel_mode_args[channel_mode];

    calc_2ch_blend(&pwm1, &pwm2, brightness, top, blend);

    set_pwms(level, pwm1, pwm2, pwm3, top);
}

void set_level_auto(uint8_t level) {
    uint16_t pwm1, pwm2;
    uint8_t  brightness = PWM_GET8 (pwm4_levels, level);
    uint16_t top        = PWM_GET16(pwm5_levels, level);
    uint8_t  blend      = 255 * (uint16_t)level / RAMP_SIZE;
    if (cfg.channel_mode_args[channel_mode] & 0b01000000)
        blend = 255 - blend;

    calc_2ch_blend(&pwm1, &pwm2, brightness, top, blend);

    set_pwms(level, pwm1, pwm2, 0, top);
}


///// bump each channel toward a target value /////
bool gradual_adjust(uint8_t ch1_pwm, uint8_t ch2_pwm, uint8_t ch3_pwm) {
    GRADUAL_ADJUST_STACKED(ch1_pwm, CH1_PWM, PWM_TOP_INIT);
    GRADUAL_ADJUST_STACKED(ch2_pwm, CH2_PWM, PWM_TOP_INIT);
    GRADUAL_ADJUST_SIMPLE (ch3_pwm, CH3_PWM);

    // check for completion
    if ((ch1_pwm == CH1_PWM)
     && (ch2_pwm == CH2_PWM)
     && (ch3_pwm == CH3_PWM)) {
        return true;  // done
    }
    return false;  // not done yet
}

static uint8_t adjust_pwm(uint8_t gt, uint8_t pwm_in) {
#ifdef USE_ULTRA_LOW_MODE
    if (gt == 0 && cfg.ultra_low_mode >= level_1_s0)
        pwm_in = 0U;
#endif
    return pwm_in;
}

bool gradual_tick_ch1(uint8_t gt) {
    uint8_t pwm1 = adjust_pwm(PWM_GET8(pwm1_levels, gt), gt);
    uint8_t pwm3 = adjust_pwm(PWM_GET8(pwm2_levels, gt), gt);
    return gradual_adjust(pwm1, 0, pwm3);
}

bool gradual_tick_ch2(uint8_t gt) {
    uint8_t pwm2 = adjust_pwm(PWM_GET8(pwm4_levels, gt), gt);
    return gradual_adjust(0, pwm2, 0);
}

bool gradual_tick_both(uint8_t gt) {
    uint8_t pwm1 = adjust_pwm(PWM_GET8(pwm1_levels, gt), gt);
    uint8_t pwm3 = adjust_pwm(PWM_GET8(pwm2_levels, gt), gt);
    return gradual_adjust(pwm1, pwm1, pwm3);
}

bool gradual_tick_blend(uint8_t level) {
    uint16_t pwm1, pwm2;
    uint8_t  pwm3       = PWM_GET8 (pwm2_levels, level);  // DD FET
    //uint16_t brightness = PWM_GET8 (pwm1_levels, level) << 1;
    uint16_t brightness = adjust_pwm(PWM_GET8 (pwm1_levels, level) + pwm3, level);
    uint16_t top        = PWM_GET16(pwm3_levels, level);
    uint8_t  blend      = cfg.channel_mode_args[channel_mode];

    calc_2ch_blend(&pwm1, &pwm2, brightness, top, blend);

    return gradual_adjust(pwm1, pwm2, pwm3);
}

bool gradual_tick_auto(uint8_t level) {
    uint16_t pwm1, pwm2;
    uint8_t  brightness = adjust_pwm(PWM_GET8 (pwm4_levels, level), level);
    uint16_t top        = PWM_GET16(pwm5_levels, level);
    uint8_t  blend      = 255 * (uint16_t)level / RAMP_SIZE;
    if (cfg.channel_mode_args[channel_mode] & 0b01000000)
        blend = 255 - blend;

    calc_2ch_blend(&pwm1, &pwm2, brightness, top, blend);

    return gradual_adjust(pwm1, pwm2, 0);
}


