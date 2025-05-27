// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2024 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//*********************************************
//**   CONFIGURATION OPTIONS FOR LUME1-6AF   **
//*********************************************

#define HWDEF_H  loneoceans/lume1-15af/hwdef.h

//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 160

// PWM1: DAC Data
// UDR x^4 curves specifically for Lume1-15AF (with FET for turbo only)

#define PWM1_LEVELS 1, 4, 19, 61, 148, 306, 567, 968, 635, 968, 14, 8, \
        27, 15, 48, 62, 79, 100, 124, 152, 185, 91, 266, 129, 371, 434, \
        504, 239, 275, 769, 359, 995, 461, 519, 583, 653, 729, 811, 899, \
        995, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 27, 12, 13, 34, \
        15, 16, 42, 45, 48, 21, 55, 24, 62, 66, 70, 75, 79, 84, 89, 94, \
        100, 105, 111, 48, 124, 130, 137, 59, 152, 160, 168, 176, 185, \
        194, 83, 87, 91, 233, 243, 254, 266, 277, 289, 302, 129, 328, 342, \
        356, 371, 158, 401, 171, 434, 451, 468, 486, 504, 523, 543, 563, \
        239, 604, 626, 648, 275, 695, 719, 743, 769, 795, 821, 849, 359, \
        905, 934, 395, 995, 420, 434, 447, 461, 475, 490, 504, 519, 535, \
        551, 567, 583, 600, 617, 635, 653, 671, 690, 709, 729, 748, 769, \
        789, 811, 832, 854, 876, 899, 923, 946, 970, 0

#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, \
        V10, V25, V10, V25, V10, V10, V10, V10, V10, V10, V10, V25, V10, \
        V25, V10, V10, V10, V25, V25, V10, V25, V10, V25, V25, V25, V25, \
        V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, \
        V10, V10, V10, V25, V25, V10, V25, V25, V10, V10, V10, V25, V10, \
        V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, \
        V10, V10, V10, V25, V10, V10, V10, V10, V10, V10, V25, V25, V25, \
        V10, V10, V10, V10, V10, V10, V10, V25, V10, V10, V10, V10, V25, \
        V10, V25, V10, V10, V10, V10, V10, V10, V10, V10, V25, V10, V10, \
        V10, V25, V10, V10, V10, V10, V10, V10, V10, V25, V10, V10, V25, \
        V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, \
        V25, V25, V25, V25, V25, V25, V25

// PWM3_LEVELS - 8-bit PWM values for FET
/*
    Generally used for Turbo only by default, this driver uses 
    the regulated buck for levels 1-149, and 100% FET for turbo
    at level 150. Customize it to your liking. 
    Do not turn on FET and Buck at the same time; ensure ramp tables line up. 
    It is possible to run FET at 8-bit PWM if desired. Operational frequency
    is the standard ~20kHz. 
*/
#define PWM3_LEVELS 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 255

// Enable usage of multiple power paths 
//#define USE_MULTIPLE_POWER_PATH (no longer required, defined in hwdef.c for >2023 Anduril)
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 11
#define LED_PATH3_PIN_LEVEL_MIN 41
#define LED_FET_PIN_LEVEL_MIN 159

#define HALFSPEED_LEVEL 42
#define QUARTERSPEED_LEVEL 36   // check with TK about the mechanics of this

#define DEFAULT_LEVEL 65        // about same as 1 AMC7135 
#define MAX_1x7135 65           // about ~350mA
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 9     // about 128uA, lowest is 100nA
//#define RAMP_SMOOTH_CEIL 159    // about 12.5A
#define RAMP_SMOOTH_CEIL 149    // about 10A

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 15      // about ~1mA
#define SIMPLE_UI_CEIL 125      // about ~5A
#define SIMPLE_UI_STEPS 5

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

//***************************************
//**       THERMAL SETTINGS            **
//***************************************

// set thermal ceiling
#ifdef DEFAULT_THERM_CEIL
#undef DEFAULT_THERM_CEIL
#endif
#define DEFAULT_THERM_CEIL 50

// extra boost during turbo, lower when temp = temp_ceil + extra
#define TURBO_TEMP_EXTRA 5

// stop panicking at ~2.0A (not sure of this numbers yet since it depends on the host..)
#define THERM_FASTER_LEVEL 100          // about 2000mA
#define MIN_THERM_STEPDOWN 71           // about 500mA
#define THERM_NEXT_WARNING_THRESHOLD 24 // 24 by default -> increase for fewer adjustments (more stable output on large time scale)
#define THERM_RESPONSE_MAGNITUDE 64     // 64 by default -> decrease for smaller adjustments (removes dip post turbo)
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

#define DEFAULT_2C_STYLE 1 // enable 2 click to turbo from on

// set this light to use stepped ramp by default (instead of smooth)
#undef RAMP_STYLE
#define RAMP_STYLE 1

// uncomment to disable smooth steps
//#ifdef USE_SMOOTH_STEPS
//#undef USE_SMOOTH_STEPS
//#endif

#define BLINK_BRIGHTNESS (MAX_LEVEL/6)  // increase blink brightness from max/6
#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_AUX1_LED

// this light has three aux LED channels: R, G, B
#define USE_AUXRGB_LEDS

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

// RGB aux LEDs should use rainbow cycling mode
// to impress new customers
// (people usually change it to voltage mode later though)
#ifdef RGB_LED_OFF_DEFAULT
#undef RGB_LED_OFF_DEFAULT
#endif
#ifdef RGB_LED_LOCKOUT_DEFAULT
#undef RGB_LED_LOCKOUT_DEFAULT
#endif
#define RGB_LED_OFF_DEFAULT auxrgb_cfg_byte(aux_low_e, aux_rgb_rainbow_e)
#define RGB_LED_LOCKOUT_DEFAULT auxrgb_cfg_byte(aux_blinking_e, aux_rgb_rainbow_e)

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
#define PARTY_STROBE_ONTIME 2
#define STROBE_OFF_LEVEL 1  // keep regulators on between strobes

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 30 // default 25

// attiny1616 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can reset the normal way because power is not connected before the button, but do this anyway! 
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// Uncomment to use Microphone mode
//#define USE_MICROPHONE_MODE
//#define MICROPHONE_SCALER 10

// ======================================