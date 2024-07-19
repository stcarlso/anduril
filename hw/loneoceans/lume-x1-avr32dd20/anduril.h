// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2024 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//**************************************************
//** CONFIGURATION OPTIONS FOR LUME-X1-AVR32DD20  **
//**************************************************

#define HWDEF_H  loneoceans/lume-x1-avr32dd20/hwdef.h

//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 156

// PWM1: DAC Data
// UDR x^4 curves specifically for Lume-X1 (AVR32DD20)

#define PWM1_LEVELS 1, 2, 8, 25, 25, 127, 236, 402, 644, 402, 588, 833, \
        11, 15, 20, 26, 33, 17, 21, 26, 77, 38, 111, 131, 155, 74, 210, \
        243, 280, 320, 365, 415, 469, 529, 594, 272, 741, 825, 915, \
        1013, 458, 504, 554, 607, 664, 726, 791, 860, 934, 1013, \
        24, 26, 28, 12, 13, 34, 15, 39, 17, 44, 47, 50, \
        22, 57, 61, 64, 28, 72, 77, 81, 86, 37, 96, 101, 106, 112, \
        118, 51, 131, 137, 59, 62, 159, 167, 175, 75, 192, 201, 210, \
        220, 230, 240, 251, 262, 112, 285, 297, 310, 323, 336, 350, \
        364, 379, 394, 410, 426, 442, 459, 477, 495, 514, 533, 552, \
        573, 243, 615, 637, 270, 683, 706, 731, 756, 782, 331, 835, \
        863, 892, 921, 951, 402, 415, 428, 442, 456, 470, 484, 499, \
        515, 530, 546, 563, 579, 596, 614, 632, 650, 669, 688, 708, \
        728, 748, 769, 790, 812, 834, 857

// PWM2_LEVELS - used to set the internal voltage reference for the DAC
//  Makes use the of concept of dynamic Vref for higher resolution output
//  despite DAC being only 10 bits. 
// Bits 2:0 of register ADC0REF set the reference voltage:
//  0x0 - 1.024V - ..000
//  0x1 - 2.048V - ..001
//  0x2 - 4.096V - ..010
//  0x3 - 2.500V-  ..011
//  0x5 - VDD   -  ..101
//  0x6 - VREFA -  ..111
// Bit 7 controls if the reference is always on or not. 
// Hence set PWM2_LEVELS to be 1xxx000 (0d64) for 1.024V DAC vref (V10)
//       set PWM2_LEVELS to be 1xxx011 (0d67) for 2.500V DAC vref (V25)
// Other references are not so important since we have a 10-bit DAC

#define PWM2_LEVELS V10, V10, V10, V10, V25, V10, V10, V10, V10, V25, \
        V25, V25, V10, V10, V10, V10, V10, V25, V25, V25, V10, V25, \
        V10, V10, V10, V25, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V25, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V10, V10, V10, V25, \
        V25, V10, V25, V10, V25, V10, V10, V10, V25, V10, V10, V10, \
        V25, V10, V10, V10, V10, V25, V10, V10, V10, V10, V10, V25, \
        V10, V10, V25, V25, V10, V10, V10, V25, V10, V10, V10, V10, \
        V10, V10, V10, V10, V25, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V25, V10, V10, V25, V10, V10, V10, V10, V10, V25, V10, V10, \
        V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25

// Enable usage of multiple power paths 
//#define USE_MULTIPLE_POWER_PATH (no longer required, defined in hwdef.c for >2023 Anduril)
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 13
#define LED_PATH3_PIN_LEVEL_MIN 51

#define HALFSPEED_LEVEL 50
#define QUARTERSPEED_LEVEL 40   // check with TK about the mechanics of this

#define DEFAULT_LEVEL 61        // about same as 1 AMC7135 
#define MAX_1x7135 61           // about same as 1 AMC7135 
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 10    // about 50uA, almost 1000x brighter than lowest
#define RAMP_SMOOTH_CEIL 131    // about ~18-20W (Turbo ~40W)

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 15      // about ~0.24mA
#define SIMPLE_UI_CEIL 119      // about ~12W
#define SIMPLE_UI_STEPS 5

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

//***************************************
//**       THERMAL SETTINGS            **
//***************************************

//#define USE_THERMAL_REGULATION

// set thermal ceiling

//#ifdef DEFAULT_THERM_CEIL
//#undef DEFAULT_THERM_CEIL
//#endif
#define DEFAULT_THERM_CEIL 50

// extra boost during turbo, lower when temp = temp_ceil + extra
//#define TURBO_TEMP_EXTRA 0

// stop panicking at 6W (not sure of this numbers yet since it depends on the host..)
#define THERM_FASTER_LEVEL 98          // about 6W
#define MIN_THERM_STEPDOWN 62           // similar to single amc7135 in a 3V light
//#define THERM_NEXT_WARNING_THRESHOLD 24 // 24 by default -> increase for fewer adjustments (more stable output on large time scale)
//#define THERM_RESPONSE_MAGNITUDE 64     // 64 by default -> decrease for smaller adjustments (removes dip post turbo)
//#define THERM_WINDOW_SIZE 1           // 2 by default -> decrease for tighter temperature regulation
//#define THERM_LOOKAHEAD 4             // 4 by default -> decrease for longer turbo

// easier access to thermal config mode, similar to Emisar, Noctigon
//#define USE_TENCLICK_THERMAL_CONFIG

//***************************************
//**          USER INTERFACE           **
//***************************************

//#define SIMPLE_UI_ACTIVE 0 // advanced UI by default

// allow Aux Config and Strobe Modes in Simple UI
//#define USE_EXTENDED_SIMPLE_UI

// Allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// double click while on goes to full-power turbo, not ramp ceiling
#define DEFAULT_2C_STYLE 1

// set this light to use stepped ramp by default (instead of smooth)
#undef RAMP_STYLE
#define RAMP_STYLE 1

// uncomment to disable smooth steps
//#ifdef USE_SMOOTH_STEPS
//#undef USE_SMOOTH_STEPS
//#endif

#define BLINK_BRIGHTNESS (MAX_LEVEL/5)  // increase blink brightness from max/6
#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_BUTTON_LED

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// show each channel while it scroll by in the menu
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

// RGB aux LEDs should use rainbow cycling mode
// to impress new customers
// (people usually change it to voltage mode later though)
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

// enable Beacontower blinky mode 
#define USE_BEACONTOWER_MODE

// party strobe on-time
#define PARTY_STROBE_ONTIME 4
#define STROBE_OFF_LEVEL 1  // keep regulators on between strobes

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 30 // default 25

// avr32dd20 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can reset the normal way because power is not connected before the button, but do this anyway! 
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// Uncomment to use Microphone mode
//#define USE_MICROPHONE_MODE
//#define MICROPHONE_SCALER 10

// ======================================