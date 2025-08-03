// hwdef for stcarlso's TS10C "FLARE 1" driver
// Copyright (C) 2025 stcarlso, Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

/* Buck driver based on TPSM82866A (ch 1)
 * with high dynamic range and DAC control + aux RGB
 *
 * Pin   Name   Function
 *   1   GND    GND
 *   2   PF6    RESET
 *   3   PF7    UPDI
 *   4   PA0    HDR: high/low Rsense range
 *   5   PA1    -
 *   6   PA2    -
 *   7   PA3    -
 *   8   PA4    -
 *   9   PA5    e-switch
 *  10   PA6    -
 *  11   PA7    -
 *  12   PC1    R: aux red
 *  13   PC2    G: aux green
 *  14   PC3    B: aux blue
 *  15   VDDIO2 VCC2
 *  16   PD4    -
 *  17   PD5    buck enable
 *  18   PD6    DAC: control voltage out
 *  19   PD7    -
 *  20   VCC    VCC
 *
 * BATT LVL : Vbat * (100.0/(200+100))
 */

#define HWDEF_C  stcarlso/ts10c/hwdef.c

// allow using aux LEDs as extra channel modes
#include "fsm/chan-rgbaux.h"

// channel modes:
// * 0. main LEDs
// * 1+. aux RGB
#define NUM_CHANNEL_MODES   (1 + NUM_RGB_AUX_CHANNEL_MODES)
enum CHANNEL_MODES {
    CM_MAIN = 0,
    RGB_AUX_ENUMS
};

#define DEFAULT_CHANNEL_MODE  CM_MAIN

// right-most bit first, modes are in fedcba9876543210 order
#define CHANNEL_MODES_ENABLED 0b0000000000000001


#undef  GRADUAL_ADJUST_SPEED
#define GRADUAL_ADJUST_SPEED  4

#define PWM_BITS      16        // 10-bit DAC
#define PWM_DATATYPE  uint16_t
#define PWM_DATATYPE2 uint32_t  // only needs 32-bit if ramp values go over 255
#define PWM1_DATATYPE uint16_t  // main LED ramp
#define PWM1_GET(l)   PWM_GET16(pwm1_levels, l)
#define PWM2_DATATYPE uint8_t   // DAC Vref table
#define PWM2_GET(l)   PWM_GET8(pwm2_levels, l)

// main LED outputs
// (DAC_LVL + DAC_VREF + Vref values are defined in arch/*.h)

// BUCK enable
#define BCK_ENABLE_PIN   PIN5_bp
#define BCK_ENABLE_PORT  PORTD_OUT

// HDR
// turns on HDR FET for the high current range
#define HDR_ENABLE_PIN   PIN0_bp
#define HDR_ENABLE_PORT  PORTA_OUT

// e-switch
#ifndef SWITCH_PIN
#define SWITCH_PIN      PIN5_bp
#define SWITCH_PORT     VPORTA.IN
#define SWITCH_ISC_REG  PORTA.PIN5CTRL
#define SWITCH_VECT     PORTA_PORT_vect
#define SWITCH_INTFLG   VPORTA.INTFLAGS
#endif

// average drop across diode on this hardware
#ifndef VOLTAGE_FUDGE_FACTOR
#define VOLTAGE_FUDGE_FACTOR 0  // using a PFET so no appreciable drop
#endif

// this driver allows for aux LEDs under the optic
#define AUXLED_R_PIN  PIN1_bp
#define AUXLED_G_PIN  PIN2_bp
#define AUXLED_B_PIN  PIN3_bp
#define AUXLED_RGB_PORT PORTC

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS


inline void hwdef_setup() {
    mcu_clock_speed();

    VPORTA.DIR = PIN0_bm;  // HDR
    VPORTC.DIR = PIN1_bm   // R
               | PIN2_bm   // G
               | PIN3_bm;  // B
    VPORTD.DIR = PIN5_bm;  // EN

    // enable pullups on the unused and input pins to reduce power
    PORTA.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm
                   | PORT_ISC_BOTHEDGES_gc;  // e-switch
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
    
    // AVR datasheet 34.3.1 #2, DAC pin must have input disable set
	PORTD.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;  // DAC
	PORTD.PIN7CTRL = PORT_PULLUPEN_bm;

    // set up the DAC
    // DAC ranges from 0V to (255 * Vref) / 256
    DAC_VREF = V10;
    DAC_LVL = 0;  // turn off output at boot
    DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;

    // TCA/TCB/TCD aren't being used, so turn them off
    TCA0.SINGLE.CTRLA = 0;
    TCB0.CTRLA = 0;
    TCB1.CTRLA = 0;
    TCD0.CTRLA = 0;
}


// set fuses, these carry over to the ELF file
// we need this for enabling BOD in Active Mode from the factory.
// settings can be verified / dumped from the ELF file using this
// command: avr-objdump -d -S -j .fuse anduril.elf
FUSES = {
    .WDTCFG  = FUSE_WDTCFG_DEFAULT,   // Watchdog Configuration

    // enable BOD (continuous) in active mode
    .BODCFG  = ACTIVE_ENABLE_gc,      // BOD Configuration

    .OSCCFG  = FUSE_OSCCFG_DEFAULT,   // Oscillator Configuration
    .SYSCFG0 = FUSE_SYSCFG0_DEFAULT,  // System Configuration 0

    // set startup time to 64ms to allow power to stabilize
    .SYSCFG1 = MVSYSCFG_DUAL_gc | SUT_64MS_gc,

    .CODESIZE = FUSE_CODESIZE_DEFAULT,
    .BOOTSIZE = FUSE_BOOTSIZE_DEFAULT,
};


#define LAYOUT_DEFINED

