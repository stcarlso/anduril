// heartbeat-mode.c: Heartbeat/Pulse mode for Anduril.
// Copyright (C) 2017-2023 Selene ToyKeeper
//               2024      stcarlso
// SPDX-License-Identifier: GPL-3.0-or-later

/*
    Heartbeat mode:

    Heartbeat mode is a lot like the beacontower mode, but never turns off
    completely, instead pulsing on and off like the standby LEDs of many
    devices. Access  this mode from the blinky group in this order: Batt
    Check, Temp Check, Beacon, SOS, Beacontower, Heartbeat. The brightness
    will be the last memorized level, and defaults to 30 beats-per-minute
    (BPM). 

       - Brightness is the user's last-ramped level. Set this in 
         ramping mode before starting beacon mode.

       - BPM is configured by holding the button. The light will blink
         once per second to aid in counting. Release it after the 
         desired amount of time has passed to set a new flashing period.

         For example, to set a 4-second (15BPM) interval, hold the button 
         for 4 seconds (4 blinks).

         Suppose the period of the light is set as t seconds, given a ramp
         level of ~60/150..
         
         - Turn on ramp = ~1/2 t
         - On at full brightness = time required to make interval perfect
         - Turn off ramp = ~1/2 t
         
         Increasing the brightness also increases the time taken to ramp up
         and down, just like an incandescent would.

 Access from blinky / utility modes; the sequence is:

  - Battery check.
  - Temperature check (if light has a temperature sensor).
  - Beacon mode.
  - SOS mode (if enabled).
  - Beacon Tower mode (if enabled).
  - Heartbeat mode (if enabled).
*/

#pragma once

#include "anduril/heartbeat-mode.h"

#ifndef HEARTBEAT_MODE_MIN
#define HEARTBEAT_MODE_MIN 0
#endif

inline void heartbeat_mode_iter() {
    // one iteration of main loop()
    if (!button_last_state) {
        uint8_t i, level = memorized_level;
        if (level > HEARTBEAT_MODE_MIN + 1) {
            // worst case: 2 levels, 255 seconds = 127.5 up/down = 63750 ms per loop
            uint8_t diff = level - HEARTBEAT_MODE_MIN;
            uint32_t t = 1000UL * heartbeat_seconds;
            uint16_t loop_delay = (uint16_t)((t >> 1) / diff), leftover =
                (uint16_t)(t - ((loop_delay * diff) << 1));
            // up
            for (i = HEARTBEAT_MODE_MIN; i < level; i++) {
                set_level(i);
                if (!nice_delay_ms(loop_delay)) goto end_state;
            }
            // make it almost perfect by waiting the remainder
            set_level(level);
            if (leftover && !nice_delay_ms(leftover)) goto end_state;
            // down
            for (i = level; i > HEARTBEAT_MODE_MIN; i--) {
                set_level(i);
                if (!nice_delay_ms(loop_delay)) goto end_state;
            }       
end_state:
            set_level(HEARTBEAT_MODE_MIN);
        } else
            // Not enough space to heartbeat, just sit at memorized level
            set_level(level);
    }
}

uint8_t heartbeat_state(Event event, uint16_t arg) {
    // 1 click: off
    if (event == EV_1click) {
        set_state(off_state, 0);
        return EVENT_HANDLED;
    }
    // 2 clicks: next blinky mode
    else if (event == EV_2clicks) {
    	#if defined(USE_BATTCHECK_MODE)
        set_state(battcheck_state, 0);
        #elif defined(USE_THERMAL_REGULATION)
        set_state(tempcheck_state, 0);
        #elif defined(USE_BEACON_MODE)
        set_state(beacon_state, 0);
        #elif defined(USE_SOS_MODE) && defined(USE_SOS_MODE_IN_BLINKY_GROUP)
        set_state(sos_state, 0);
        #elif defined(USE_BEACONTOWER_MODE)
        set_state(beacontower_state, 0);
        #endif
        return EVENT_HANDLED;
    }
    // hold: configure beacon tower timing
    else if (event == EV_click1_hold) {
        if (0 == (arg % TICKS_PER_SECOND))
            blink_once();
        else
            set_level(0);
        return EVENT_HANDLED;
    }
    // release hold: save new timing
    else if (event == EV_click1_hold_release) {
        heartbeat_seconds = 1 + (arg / TICKS_PER_SECOND);
        save_config();
        interrupt_nice_delays();
        return EVENT_HANDLED;
    }
    return EVENT_NOT_HANDLED;
}