// Noctigon KR4 (no DD FET) PWM helper functions
// Copyright (C) 2020-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "fsm/chan-rgbaux.c"

void set_level_zero();

void set_level_main(uint8_t level);
bool gradual_tick_main(uint8_t gt);

Channel channels[] = {
    { // channel 1 only
        .set_level    = set_level_main,
        .gradual_tick = gradual_tick_main,
        .uses_aux     = 0
    },
    RGB_AUX_CHANNELS
};


void set_level_zero() {
    CH1_PWM = 0;
    CH2_PWM = 0;
    PWM_CNT = 0;  // reset phase
    CH1_ENABLE_PORT &= ~(1 << CH1_ENABLE_PIN);  // disable opamp
}

// single set of LEDs with linear power channel
void set_level_main(uint8_t level) {
#ifdef USE_ULTRA_LOW_MODE
    level_1_mode_e mode = cfg.ultra_low_mode;

#ifdef USE_AUX_RGB_LEDS
    if (level == 0 && mode == level_1_redaux) {
        set_level_auxred(1);
        set_level_zero();
        return;
    }
#endif
#endif

    CH1_ENABLE_PORT |= (1 << CH1_ENABLE_PIN);  // enable opamp

    PWM_DATATYPE ch1_pwm = PWM_GET(pwm1_levels, level);
    // pulse frequency modulation, a.k.a. dynamic PWM
    uint16_t top = PWM_GET16(pwm_tops, level);

    if (0) { }
#ifdef USE_ULTRA_LOW_MODE
    else if (level == 0 && mode >= level_1_s0) {
        CH1_PWM = 0;
    }
#endif
    else {
        CH1_PWM = ch1_pwm;
    }
    CH2_PWM = 0;
    // wait to sync the counter and avoid flashes
    while(actual_level && (PWM_CNT > (top - 32))) {}
    PWM_TOP = top;
    // force reset phase when turning on from zero
    // (because otherwise the initial response is inconsistent)
    if (! actual_level) PWM_CNT = 0;
}

bool gradual_tick_main(uint8_t gt) {
    PWM_DATATYPE pwm1 = PWM_GET(pwm1_levels, gt);

    GRADUAL_ADJUST_SIMPLE(pwm1, CH1_PWM);

#ifdef USE_ULTRA_LOW_MODE
    if (gt == 0 && cfg.ultra_low_mode >= level_1_s0) {
        pwm1 = 0U;
    }
#endif

    if (   (pwm1 == CH1_PWM)
       ) {
        return true;  // done
    }
    return false;  // not done yet
}

