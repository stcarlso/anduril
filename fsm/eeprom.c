// fsm-eeprom.c: EEPROM API for SpaghettiMonster.
// Copyright (C) 2017-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "fsm/eeprom.h"

#ifdef USE_EEPROM
#ifdef EEPROM_OVERRIDE
uint8_t *eeprom;
#else
uint8_t eeprom[EEPROM_BYTES];
#endif

static uint8_t EEMEM eeprom_ref[EEPROM_BYTES];

uint8_t load_eeprom() {
    #ifdef USE_EEP_DELAY
    delay_4ms(2);  // wait for power to stabilize
    #endif

    cli();
    // check if eeprom has been initialized; abort if it hasn't
    uint8_t marker = eeprom_read_byte(&eeprom_ref[EEP_START]);
    if (marker != EEP_MARKER) { sei(); return 0; }

    // load the actual data
    eeprom_read_block(eeprom, &eeprom_ref[EEP_START+1], EEPROM_BYTES);
    sei();
    return 1;
}

void save_eeprom() {
    #ifdef USE_EEP_DELAY
    delay_4ms(2);  // wait for power to stabilize
    #endif

    cli();

    // save the actual data
    eeprom_update_block(eeprom, &eeprom_ref[EEP_START+1], EEPROM_BYTES);

    // save the marker last, to indicate the transaction is complete
    eeprom_update_byte(&eeprom_ref[EEP_START], EEP_MARKER);
    sei();
}
#endif

#ifdef USE_EEPROM_WL
uint8_t eeprom_wl[EEPROM_WL_BYTES];
/* Could be up to 512 */
uint16_t eep_wl_prev_offset;

uint8_t load_eeprom_wl() {
    #ifdef USE_EEP_DELAY
    delay_4ms(2);  // wait for power to stabilize
    #endif

    cli();
    // check if eeprom has been initialized; abort if it hasn't
    uint8_t found = 0;
    uint8_t offset;
    for(offset = 0;
        offset < EEP_WL_SIZE - EEPROM_WL_BYTES - 1;
        offset += (EEPROM_WL_BYTES + 1)) {
        if (eeprom_read_byte(&eeprom_ref[offset]) == EEP_MARKER) {
            found = 1;
            eep_wl_prev_offset = offset;
            break;
        }
    }

    if (found) {
        // load the actual data
        eeprom_read_block(eeprom_wl, &eeprom_ref[offset+1], EEPROM_WL_BYTES);
    }
    sei();
    return found;
}

void save_eeprom_wl() {
    #ifdef USE_EEP_DELAY
    delay_4ms(2);  // wait for power to stabilize
    #endif

    cli();
    // erase old state
    uint8_t offset = eep_wl_prev_offset;
    for (uint8_t i = 0; i < EEPROM_WL_BYTES+1; i ++) {
        eeprom_update_byte(&eeprom_ref[offset + i], 0xFF);
    }

    // save new state
    offset += EEPROM_WL_BYTES+1;
    if (offset > (EEP_WL_SIZE-EEPROM_WL_BYTES-1)) offset = 0;
    eep_wl_prev_offset = offset;
    // marker byte
    // FIXME: write the marker last, to signal completed transaction
    eeprom_update_byte(&eeprom_ref[offset], EEP_MARKER);
    // user data
    eeprom_update_block(eeprom_wl, &eeprom_ref[offset+1], EEPROM_WL_BYTES);
    sei();
}
#endif

