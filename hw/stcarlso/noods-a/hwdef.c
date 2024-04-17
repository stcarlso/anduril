// Driver for Adafruit Noods LEDs RevA PWM helper functions
// Copyright (C) 2024 Stephen Carlson
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "fsm/chan-aux.c"

void set_level_zero();

void set_level_main(uint8_t level);
bool gradual_tick_main(uint8_t gt);


Channel channels[] = {
    { // main LEDs
        .set_level    = set_level_main,
        .gradual_tick = gradual_tick_main,
        .uses_aux     = 0
    },
    { // aux LEDs
        .set_level    = set_level_aux,
        .gradual_tick = gradual_tick_null,
        .uses_aux     = 1
    }
};


void set_level_zero() {
    CH1_PWM = 0;
    // Reset phase
    PWM_CNT = 0;
}

// Single LED with one power channel
void set_level_main(uint8_t level) {
    PWM_DATATYPE pwm = PWM_GET(pwm1_levels, level);
    // Pulse frequency modulation, a.k.a. dynamic PWM
    uint16_t top = PWM_GET16(pwm_tops, level);

    CH1_PWM = pwm;
    PWM_TOP = top;

    // Force reset phase when turning on from zero
    // (because otherwise the initial response is inconsistent)
    if (! actual_level) PWM_CNT = 0;
}

bool gradual_tick_main(uint8_t gt) {
    PWM_DATATYPE target = PWM_GET(pwm1_levels, gt), pwm = CH1_PWM;
    uint16_t top = PWM_GET16(pwm_tops, gt);

    GRADUAL_ADJUST_SIMPLE(target, pwm);
    CH1_PWM = pwm;
    PWM_TOP = top;

    return (pwm == target);
}
