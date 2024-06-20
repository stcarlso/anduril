// fsm-standby.c: standby mode functions for SpaghettiMonster.
// Copyright (C) 2017-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "fsm/adc.h"
#include "fsm/wdt.h"
#include "fsm/pcint.h"

// low-power standby mode used while off but power still connected
#define standby_mode sleep_until_eswitch_pressed
void sleep_until_eswitch_pressed()
{
    #ifdef TICK_DURING_STANDBY
    WDT_slow();
    #else
    WDT_off();
    #endif

    ADC_off();

    // make sure switch isn't currently pressed
    while (button_is_pressed()) {}
    empty_event_sequence();  // cancel pending input on suspend

    PCINT_on();  // wake on e-switch event

    #ifdef TICK_DURING_STANDBY
    // detect which type of event caused a wake-up
    irq_adc = 0;
    irq_wdt = 0;
    irq_pcint = 0;
    #ifdef USE_ULTRA_LOW_MODE
    while (go_to_standby || ul_standby_mode) {
    #else
    while (go_to_standby) {
    #endif
    #else
        go_to_standby = 0;
        #ifdef USE_ULTRA_LOW_MODE
        ul_standby_mode = 0;
        #endif
    #endif

        // configure sleep mode
        #ifdef TICK_DURING_STANDBY
            // needs a special sleep mode during measurements
            if (adc_active_now) adc_sleep_mode();
            else
        #endif
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);

        #ifdef BODCR  // only do this on MCUs which support it
        sleep_bod_disable();
        #endif
        sleep_enable();
        sleep_cpu();  // wait here

        // something happened; wake up
        sleep_disable();

    #ifdef TICK_DURING_STANDBY
        // determine what woke us up...
        if (irq_pcint) {  // button pressed; wake up
            go_to_standby = 0;
            #ifdef USE_ULTRA_LOW_MODE
            ul_standby_mode = 0;
            #endif
        }
        if (irq_adc) {  // ADC done measuring
            #ifndef USE_LOWPASS_WHILE_ASLEEP
            adc_reset = 1;  // don't lowpass while asleep
            #endif
            adc_deferred_enable = 1;
            adc_deferred();
            //ADC_off();  // takes care of itself
            //irq_adc = 0;  // takes care of itself
        }
        if (irq_wdt) {  // generate a sleep tick
            WDT_inner();
        }
    }
    #endif

    // don't lowpass immediately after waking
    // also, reset thermal history
    adc_reset = 2;

    // go back to normal running mode
    // PCINT not needed any more, and can cause problems if on
    // (occasional reboots on wakeup-by-button-press)
    PCINT_off();
    // restore normal awake-mode interrupts
    ADC_on();
    WDT_on();
}

#ifdef USE_IDLE_MODE
void idle_mode()
{
    // configure sleep mode
    if (0) { }
#ifdef USE_ULTRA_LOW_MODE
    else if (cfg.ultra_low_mode == level_1_s0 && actual_level == 1) {
        adc_sleep_mode();
    }
#endif
    else {
        set_sleep_mode(SLEEP_MODE_IDLE);
    }

    sleep_enable();
    sleep_cpu();  // wait here

    // something happened; wake up
    sleep_disable();
}
#endif

