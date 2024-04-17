// Driver for Adafruit Noods LEDs RevA layout
// Copyright (C) 2024 Stephen Carlson
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

/*
 * Pin / Name / Function
 *   1    PA2   (none)
 *   2    PA3   (none)
 *   3    GND   GND
 *   4    VCC   VCC
 *   5    PA4   (none)
 *   6    PA5   L: button LED
 *   7    PA6   (none)
 *   8    PA7   (none)
 *   9    PB5   e-switch (PCINT7)
 *  10    PB4   (none)
 *  11    PB3   (none)
 *  12    PB2   (none)
 *  13    PB1   (none)
 *  14    PB0   driver enable / PWM
 *  15    PC0   (none)
 *  16    PC1   (none)
 *  17    PC2   (none)
 *  18    PC3   (none)
 *  19    PA0   RESET
 *  20    PA1   (none)
 *      ADC12   thermal sensor
 */

#define HWDEF_C  stcarlso/noods-a/hwdef.c

// Allow using button (aux) LEDs as a channel mode to blink out power
#include "fsm/chan-aux.h"

// Channel modes:
// * 0. main LEDs
// * 1. aux LEDs
#define NUM_CHANNEL_MODES   2
enum CHANNEL_MODES {
    CM_MAIN = 0,
    CM_AUX
};

#define DEFAULT_CHANNEL_MODE  CM_MAIN

// Right-most bit first, modes are in fedcba9876543210 order
#define CHANNEL_MODES_ENABLED 0b0000000000000001

#define PWM_BITS      16
#define PWM_GET       PWM_GET8
#define PWM_DATATYPE  uint16_t
#define PWM_DATATYPE2 uint16_t
#define PWM1_DATATYPE uint8_t

#define PWM_TOP       TCA0.SINGLE.PERBUF   // Holds the TOP value for variable-resolution PWM
#define PWM_TOP_INIT  255
#define PWM_CNT       TCA0.SINGLE.CNT

// Regulated channel
#define CH1_PIN  PB0
#define CH1_PWM  TCA0.SINGLE.CMP0BUF  // CMP0 is the output compare register for PB0

// E-switch
#define SWITCH_PIN      PIN5_bp
#define SWITCH_PORT     VPORTB.IN
#define SWITCH_ISC_REG  PORTB.PIN2CTRL
#define SWITCH_VECT     PORTB_PORT_vect
#define SWITCH_INTFLG   VPORTB.INTFLAGS

// Button LED (aux)
#define AUXLED_PIN  PIN5_bp
#define AUXLED_PORT PORTA

// Average parasitic voltage loss on this hardware
#ifndef VOLTAGE_FUDGE_FACTOR
// PFET for RPP, essentially no drop, but experimentally add 0.05V for other losses
#define VOLTAGE_FUDGE_FACTOR 1
#endif

inline void hwdef_setup() {
    // Set up the system clock to run at 10 MHz instead of the default 3.33 MHz
    _PROTECTED_WRITE( CLKCTRL.MCLKCTRLB,
                      CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm );

    // Enable output ports
    VPORTA.DIR = PIN5_bm;  // Button LED
    VPORTB.DIR = PIN0_bm;  // Main channel

    // Enable pullups on the unused pins to reduce power
    PORTA.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;

    PORTC.PIN0CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN3CTRL = PORT_PULLUPEN_bm;

    // Configure PWM
    // https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny1614-16-17-DataSheet-DS40002204A.pdf
    // PB0 is TCA0:WO0, use TCA_SINGLE_CMP0EN_bm
    // PB1 is TCA0:WO1, use TCA_SINGLE_CMP1EN_bm
    // PB2 is TCA0:WO2, use TCA_SINGLE_CMP2EN_bm
    // For Fast (Single Slope) PWM use TCA_SINGLE_WGMODE_SINGLESLOPE_gc
    // For Phase Correct (Dual Slope) PWM use TCA_SINGLE_WGMODE_DSBOTTOM_gc
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                      | TCA_SINGLE_WGMODE_DSBOTTOM_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc
                      | TCA_SINGLE_ENABLE_bm;

    PWM_TOP = PWM_TOP_INIT;
}

#define LAYOUT_DEFINED
