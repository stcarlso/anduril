// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//*********************************************
//**   CONFIGURATION OPTIONS FOR LUME10      **
//*********************************************

#define HWDEF_H  loneoceans/lume10/hwdef.h

//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 150

// Lume10 Ramp Table (AVR32DD20)
#define PWM1_LEVELS 1, 3, 15, 46, 113, 235, 435, 742, 487, 742, 11, 15, \
        21, 28, 37, 47, 60, 76, 94, 116, 140, 169, 202, 240, 282, 330, \
        384, 444, 511, 585, 667, 757, 856, 965, 444, 497, 554, 617, 684, \
        757, 836, 920, 1011, 22, 24, 26, 28, 31, 34, 36, 39, 42, 46, 49, \
        53, 57, 61, 65, 69, 74, 79, 84, 90, 95, 101, 108, 114, 121, 128, \
        135, 143, 151, 159, 168, 177, 186, 196, 206, 216, 227, 238, 250, \
        262, 274, 287, 301, 314, 329, 343, 358, 374, 390, 407, 424, 441, \
        459, 478, 497, 517, 537, 558, 580, 602, 625, 648, 672, 696, 722, \
        747, 774, 801, 829, 858, 887, 917, 948, 979, 1011, 428, 442, 456, \
        470, 485, 500, 515, 531, 547, 564, 581, 598, 615, 633, 652, 670, \
        689, 709, 729, 749, 769, 790, 812, 834, 856, 879, 902, 925, 949, \
        973, 998, 0

/* 
  PWM2_LEVELS - used to set the internal voltage reference for the DAC
        use the concept of dynamic Vref for higher resolution output
        despite DAC being only 10 bits. 
   Bits 2:0 of register ADC0REF set the reference voltage:
        0x0 - 1.024V - ..000
        0x1 - 2.048V - ..001
        0x2 - 4.096V - ..010
        0x3 - 2.500V-  ..011
        0x5 - VDD   -  ..101
        0x6 - VREFA -  ..111
   Bit 7 controls if the reference is always on or not. 
        set PWM2_LEVELS to be 1xxx000 (0d64) for 1.024V DAC vref (V10)
        set PWM2_LEVELS to be 1xxx011 (0d67) for 2.500V DAC vref (V25)
        other references are not so important since we have a 10-bit DAC
*/
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25, V25, V25, V25

// PWM3_LEVELS - 8-bit PWM values for FET
/*
    generally used for turbo only by default, this driver uses 
    the regulated buck for levels 1-149, and 100% FET for turbo
    at level 150. customize it to your liking. 

    do not turn on FET and buck at the same time; ensure ramp tables 
    line up. it is possible to run FET at 8-bit PWM if desired. 
    operational frequency is the standard ~19.4kHz. 
*/
#define PWM3_LEVELS 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 255

// enable usage of multiple power paths 
//#define USE_MULTIPLE_POWER_PATH (no longer required, defined in hwdef.c for >2023 Anduril)
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 11
#define LED_PATH3_PIN_LEVEL_MIN 44
#define LED_FET_PIN_LEVEL_MIN 150

#define HALFSPEED_LEVEL 42
#define QUARTERSPEED_LEVEL 36   // check with TK about the mechanics of this

#define DEFAULT_LEVEL 65        // about same as 1 AMC7135 
#define MAX_1x7135 65           // about ~350mA
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 7     // about ~50uA ~0.015lm 
                                // lvl 8 about ~85uA, ~0.03lm, lowest is 100nA 
#define RAMP_SMOOTH_CEIL 149    // about ~10A

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 14      // about ~800uA ~.25lm
#define SIMPLE_UI_CEIL 131      // about ~6A
#define SIMPLE_UI_STEPS 5

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

//***************************************
//**       THERMAL SETTINGS            **
//***************************************

//#define USE_THERMAL_REGULATION
#define AVRXMEGA3   // used in factory-reset.c for thermal offset cal, probably should refactor

// set thermal ceiling
#ifdef DEFAULT_THERM_CEIL
#undef DEFAULT_THERM_CEIL
#endif
#define DEFAULT_THERM_CEIL 50

// extra boost during turbo = temp_ceil + extra
#define TURBO_TEMP_EXTRA 7

// stop panicking at ~2.0A (calibrate depending on the host..)
#define THERM_FASTER_LEVEL 100          // about 2000mA
#define MIN_THERM_STEPDOWN 71           // about 500mA
#define THERM_NEXT_WARNING_THRESHOLD 24 // 24 default -> increase for fewer adjustments (more stable output on large time scale)
#define THERM_RESPONSE_MAGNITUDE 64     // 64 default -> decrease for smaller adjustments (removes dip post turbo)
//#define THERM_WINDOW_SIZE 1           // 2 default -> decrease for tighter temperature regulation
//#define THERM_LOOKAHEAD 4             // 4 default -> decrease for longer turbo

// easier access to thermal config mode, similar to Emisar, Noctigon
//#define USE_TENCLICK_THERMAL_CONFIG

//***************************************
//**          USER INTERFACE           **
//***************************************

//#define SIMPLE_UI_ACTIVE 0 // advanced UI by default

// allow Aux Config and Strobe Modes in Simple UI
//#define USE_EXTENDED_SIMPLE_UI

// allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// enable 2 click to turbo from on
#define DEFAULT_2C_STYLE 1 // enable 2 click to turbo from on

// set this light to use stepped ramp by default (instead of smooth)
#undef RAMP_STYLE
#define RAMP_STYLE 1

// uncomment to disable smooth steps
//#ifdef USE_SMOOTH_STEPS
//#undef USE_SMOOTH_STEPS
//#endif

#define BLINK_BRIGHTNESS (MAX_LEVEL/6)  // default blink brightness max/6
#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_BUTTON_LED

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// this light introduces advanced aux
#define USE_AUX_RGB_ADV

// show each channel while it scrolls by in the menu
#define USE_CONFIG_COLORS

// blink numbers on the main LEDs by default
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

// this light only has one main led channel
// use aux red + aux blue for police strobe
#define USE_POLICE_COLOR_STROBE_MODE
#define POLICE_STROBE_USES_AUX
#define POLICE_COLOR_STROBE_CH1 CM_AUXRED
#define POLICE_COLOR_STROBE_CH2 CM_AUXBLU

// the aux LEDs are front-facing, so turn them off while main LEDs are on
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

// advanced aux LEDs default to rainbow blend mode 
// to impress new customers
// mode: 0bPPPPCCCC where PPPP is the pattern and CCCC is the color
// P: off, low, high, blinking
// C: R, Y, G, C, B, M, W, Rainbow, Voltage
#ifdef RGB_LED_OFF_DEFAULT
#undef RGB_LED_OFF_DEFAULT
#endif
#define RGB_LED_OFF_DEFAULT     0x18    // low, rainbow
#define RGB_LED_LOCKOUT_DEFAULT 0x37    // blinking, rainbow

// enable blinking aux LEDs
#define TICK_DURING_STANDBY
#define STANDBY_TICK_SPEED 3  // every 0.128 s
//#define STANDBY_TICK_SPEED 4  // every 0.256 s
//#define STANDBY_TICK_SPEED 5  // every 0.512 s

// enable SOS mode
#define USE_SOS_MODE
#define USE_SOS_MODE_IN_BLINKY_GROUP

// enable Beacontower blinky mode (after SOS)
#define USE_BEACONTOWER_MODE

// party strobe on-time
#define PARTY_STROBE_ONTIME 1
#define STROBE_OFF_LEVEL 1  // keep regulators on between strobes

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 30 // default 25

// avr32dd20 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can reset the normal way because power is not connected before the button, but do this anyway
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// enable OTG/powerbank in momentary mode
#define USE_OTG_IN_MOMENTARY

// disable POVD by default since it's causing many customers a lot of issues
#define DEFAULT_POST_OFF_VOLTAGE_SECONDS  0

// use 1 decimal place for voltage check to avoid false precision
// AVR32dd20 iVREF is only +-4% despite high ADC resolution
//#ifdef USE_EXTRA_BATTCHECK_DIGIT
//#undef USE_EXTRA_BATTCHECK_DIGIT
//#endif

// ======================================