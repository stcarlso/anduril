// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2024 loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//**********************************************
//**    HARDWARE DEF FOR 2024 LUME1-15AF      **
//**********************************************

/*  Loneoceans Lume1-15AF with AVR32DD20

    The following pins are invariant (20 QFN package 3x3 0.4mm BSC)
    - PD6 / PP11 - DAC OUT
    - PF6 / PP15 - nRST
    - PF7 / PP16 - UPDI
    - xx  / PP13 - VDD (read voltage from VDD, PFET RPP, no voltage drop)
    - xx  / PP8  - VDDIO2
    - xx  / PP14 - GND

    Used Pins
    - PA0 / PP17 - FET via PWM (TCA0-WO0)
    - PA5 / PP2  - PATH1 - Moon Range
    - PA6 / PP3  - PATH2 - Low Range
    - PA7 / PP4  - PATH3 - High Range
    - PC1 / PP5  - Enable for Buck, Amplifier 
    - PC2 / PP6  - Enable for Microphone
    - PC3 / PP7  - Neo
    - PD4 / PP9  - E-Switch 
    - PD5 / PP10 - Mic Output
    - PD6 / PP11 - DAC Out 
    - PD7 / PP12 - Enable for Power Bank
    - PA1 / PP18 - AUX R LED
    - PA2 / PP19 - AUX G LED
    - PA3 / PP20 - AUX B LED
    - PA4 / PP1  - AUX SW LED

*/

#define HWDEF_C  loneoceans/lume1-15af/hwdef.c

// allow using aux LEDs as extra channel modes
#include "fsm/chan-rgbaux.h"

// channel modes:
// * 0. main LEDs
// * 1+. aux RGB
#define NUM_CHANNEL_MODES   (1 + NUM_AUXRGB_CHANNEL_MODES)
enum CHANNEL_MODES {
    CM_MAIN = 0,
    AUXRGB_CM_ENUMS
};

#define DEFAULT_CHANNEL_MODE  CM_MAIN

// right-most bit first, modes are in fedcba9876543210 order
#define CHANNEL_MODES_ENABLED 0b0000000000000001

//***************************************
//**       SET UP DAC AND PWM          **
//***************************************

// Define DAC control
#define PWM_BITS      16        // 10-bit DAC
#define PWM_DATATYPE  uint16_t
#define PWM_DATATYPE2 uint32_t
#define PWM1_DATATYPE uint16_t  // main regulated ramp 10-bit
#define PWM1_GET(x)   PWM_GET16(pwm1_levels, x)
#define PWM2_DATATYPE uint8_t   // DAC Vref table (4/6 options)
#define PWM2_GET(x)   PWM_GET8(pwm2_levels, x)
#define PWM3_DATATYPE uint8_t   // DD FET ramp 8-bit
#define PWM3_GET(x)   PWM_GET8(pwm3_levels, x)

// Set up FET PWM Pin - PA0 - TCA0 WO0
//#ifndef PWM3_PIN
#define PWM3_TOP TCA0.SINGLE.PER
#define PWM3_PIN PA0                // PA0 FET pin
#define PWM3_LVL TCA0.SINGLE.CMP0   // CMP0 is the output compare register
//#endif

//***************************************
//**         PIN DEFINITIONS           **
//***************************************

// Buck and Amplifier Enable (PC1)
#define BCK_ENABLE_PIN   PIN1_bp
#define BCK_ENABLE_PORT  PORTC_OUT
#define BCK_ON_DELAY 8  // ms delay turning on the led after enable

// Ultra Dynamic Range (UDR)
/* 
    UDR makes use of the concept of multiple power paths. 3 are used in this
    design to achieve extremely low moonlight levels. This is combined with 
    dynamic Vref for smoother brightness level transitions that would
    normally be limited by the 10-bit DAC resolution. 

    The Lume1 uses the internal DAC to generate a reference voltage for 
    a current-regulated amplifier which drives the LED. Each power path 
    routes current through different sense resistors, allowing for improved
    current sense resolution especially at the low end. Using UDR, Lume1
    is capable of ultra-low firefly / moonlight levels with a dynamic range 
    on the order of >>10 million : 1. 
*/

// For UDR Path 1 (firefly mode) - PA5
#define LED_PATH1_PIN PIN5_bm
#define LED_PATH1_PORT PORTA_OUT

// For UDR Path 2 (low mode) - PA6
#define LED_PATH2_PIN PIN6_bm
#define LED_PATH2_PORT PORTA_OUT

// For UDR Path 3 (high mode) - PA7
#define LED_PATH3_PIN   PIN7_bm
#define LED_PATH3_PORT  PORTA_OUT

// Define Aux LED Pins

// lighted switch button aux led (PA4)
#define AUX1_LED_PIN  PIN4_bp
#define AUX1_LED_PORT PORTA

// this driver allows for aux LEDs under the optic
#define AUXRGB_R_PIN   PIN1_bp
#define AUXRGB_R_PORT PORTA

#define AUXRGB_G_PIN   PIN2_bp
#define AUXRGB_G_PORT PORTA

#define AUXRGB_B_PIN   PIN3_bp
#define AUXRGB_B_PORT PORTA

// Define e-switch Pin and ISR
#ifndef SWITCH_PIN   // PD4
#define SWITCH_PIN     PIN4_bp
#define SWITCH_PORT    VPORTD.IN
#define SWITCH_ISC_REG PORTD.PIN4CTRL
#define SWITCH_VECT    PORTD_PORT_vect
#define SWITCH_INTFLG  VPORTD.INTFLAGS
#endif

// average drop across diode on this hardware
#ifndef VOLTAGE_FUDGE_FACTOR
#define VOLTAGE_FUDGE_FACTOR 1  // PFET for RRP, essentially 0 v-drop, but experimentally add 0.05V for better UX
#endif

//***************************************
//**          HARDWARE INIT            **
//***************************************

inline void hwdef_setup() {

    // TODO: for this DAC controlled-light, try to decrease the clock speed
    // to reduce overall system power
    mcu_clock_speed();

    // set output pins
    VPORTA.DIR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | 
                 PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    VPORTC.DIR = PIN1_bm | PIN2_bm | PIN3_bm;
    VPORTD.DIR = PIN6_bm | PIN7_bm;

    // now set pullups on input pins, and unused pins (reduce power)
    //PORTA.PIN0CTRL = PORT_PULLUPEN_bm;  // AUX R --> FET
    //PORTA.PIN1CTRL = PORT_PULLUPEN_bm;  // AUX G
    //PORTA.PIN2CTRL = PORT_PULLUPEN_bm;  // AUX B
    //PORTA.PIN3CTRL = PORT_PULLUPEN_bm;  // AUX SW
    //PORTA.PIN4CTRL = PORT_PULLUPEN_bm;  // FET   --> AUX R
    //PORTA.PIN5CTRL = PORT_PULLUPEN_bm;  // PATH1
    //PORTA.PIN6CTRL = PORT_PULLUPEN_bm;  // PATH2
    //PORTA.PIN7CTRL = PORT_PULLUPEN_bm;  // PATH3

    //PORTC.PIN1CTRL = PORT_PULLUPEN_bm;  // ENABLE
    //PORTC.PIN2CTRL = PORT_PULLUPEN_bm;  // MIC ENABLE
    PORTC.PIN3CTRL = PORT_PULLUPEN_bm;  // NEO

    //PORTD.PIN4CTRL = PORT_PULLUPEN_bm;  // ESW (100kR PULLUP)
    //PORTD.PIN5CTRL = PORT_PULLUPEN_bm;  // MIC OUT
    //PORTD.PIN6CTRL = PORT_PULLUPEN_bm;  // DAC OUT
    //PORTD.PIN7CTRL = PORT_PULLUPEN_bm;  // PWR BNK ENABLE

    // clear some pins we don't need for now or want to initialize as low
    PORTC_OUT &= ~(1 << PIN1_bp);   // ENABLE
    PORTC_OUT &= ~(1 << PIN2_bp);   // MIC ENABLE
    PORTD_OUT &= ~(1 << PIN7_bp);   // PWR BNK ENABLE

    //E-Switch (now uses external 100k pullup)
    PORTD.DIRCLR = PIN4_bm; // set ESW as input pin
    PORTD.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;

    // set up the DAC (used for the switching regulator)
    // https://www.microchip.com/en-us/product/avr32dd20
    // DAC ranges from 0V to (255 * Vref) / 256
    
    // Datasheet 34.3.1-2/2: input for DAC must be disabled
    PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    //VREF.CTRLA |= VREF_DAC0REFSEL_2V5_gc; // also VREF_DAC0REFSEL_0V55_gc and VREF_DAC0REFSEL_1V1_gc and VREF_DAC0REFSEL_2V5_gc 
    //VREF.CTRLB |= VREF_DAC0REFEN_bm;            // enable vref
    DAC_VREF = V10;
    DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;  // enable DAC
    DAC_LVL = 0;                                // lowest output during boot
    // TODO: instead of enabling the DAC at boot, pull pin down
    //       to generate a zero without spending power on the DAC
    //       (and do this in set_level_zero() too)


    // set up the 8-bit PWM for FET
    // PA0 <--> TCA, WO0
    // use single slope pwm generation
    // CLK is 10MHz. Div 2 and 8-bit = 19.6kHz PWM freq.
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm; // clk_f div 2 & enable (CLK is 10MHz)
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;   // PA0 <-> TCA WO0
    TCA0.SINGLE.PER = 255-1;  // top value, subtract 1 so CMP=255 --> static high

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

    // enable MVIO because VDDIO2 pin isn't connected
    // set startup time to 64ms to allow power to stabilize
    .SYSCFG1 = MVSYSCFG_DUAL_gc | SUT_64MS_gc,

    .CODESIZE = FUSE_CODESIZE_DEFAULT,
    .BOOTSIZE = FUSE_BOOTSIZE_DEFAULT,
};

#define LAYOUT_DEFINED