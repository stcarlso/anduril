// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//***************************************************
//** CONFIGURATION OPTIONS FOR LUME-N1             **
//***************************************************

#define HWDEF_H  loneoceans/lume-n1/hwdef.h

//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 150

// PWM1: DAC Data
// UDR x^4 curves specifically for Lume-N1

// 150 levels Lume N1 24A:
#define PWM1_LEVELS 1, 5, 24, 76, 185, 384, 711, 497, 796, 12, 18, 25, 35, 47, 62, 80, 102, 128, 158, 195, 237, 285, 340, 403, 475, 556, 646, 748, 860, 985, 460, 522, 591, 666, 748, 837, 934, 20, 23, 25, 28, 31, 34, 37, 41, 45, 49, 53, 58, 63, 68, 74, 80, 87, 93, 101, 108, 116, 125, 134, 144, 154, 164, 176, 187, 200, 213, 226, 241, 256, 271, 288, 305, 323, 342, 361, 382, 403, 425, 448, 472, 497, 523, 551, 579, 608, 639, 670, 703, 737, 773, 809, 847, 886, 927, 969, 1013, 433, 453, 472, 493, 514, 536, 558, 581, 605, 630, 656, 682, 709, 737, 766, 796, 826, 858, 890, 924, 958, 993, 1020, 260, 275, 290, 306, 322, 339, 357, 375, 394, 414, 434, 455, 477, 500, 524, 548, 573, 599, 626, 654, 683, 712, 743, 775, 807, 841, 876, 912, 948, 986

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

// 150 levels Lume N1 24A:
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10

// Enable usage of multiple power paths 
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 10
#define LED_PATH3_PIN_LEVEL_MIN 38
#define LED_FUSION_LEVEL_MIN 121    // new for Lume N1

#define HALFSPEED_LEVEL 50
#define QUARTERSPEED_LEVEL 40       // check with TK about the mechanics of this

#define DEFAULT_LEVEL 59            // about same as 1 AMC7135 ~1W
#define MAX_1x7135 59               // about same as 1 AMC7135 
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 7         // ~70uA 0.025lm, lvl 8 = ~120uA 0.045 lumens
#define RAMP_SMOOTH_CEIL 120        // 6.0A output or about 18W

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 12          // about 0.6mA - 0.22 lumens
#define SIMPLE_UI_CEIL 109          // 4.0A output or about ~12W
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

//#ifdef DEFAULT_THERM_CEIL
//#undef DEFAULT_THERM_CEIL
//#endif
#define DEFAULT_THERM_CEIL 50

// extra boost during turbo, lower when temp = temp_ceil + extra
#define TURBO_TEMP_EXTRA 5

// stop panicking at ~10W (not sure of this numbers yet since it depends on the host..)
#define THERM_FASTER_LEVEL 99           // about 10W for D4SV2 // 91
#define MIN_THERM_STEPDOWN 73           // about 2.5W // 59
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
//#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_BUTTON_LED

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// this light introduces advanced aux 
#define USE_AUX_RGB_ADV

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
//#define TICK_DURING_STANDBY
//#define STANDBY_TICK_SPEED 3  // every 0.128 s
//#define STANDBY_TICK_SPEED 4  // every 0.256 s
//#define STANDBY_TICK_SPEED 5  // every 0.512 s

// enable SOS mode
#define USE_SOS_MODE
#define USE_SOS_MODE_IN_BLINKY_GROUP

// enable Beacontower blinky mode 
#define USE_BEACONTOWER_MODE

// party strobe on-time
#define PARTY_STROBE_ONTIME 1
#define STROBE_OFF_LEVEL 1  // keep regulators on between strobes

// smaller candle oscillations
#define CANDLE_AMPLITUDE 22 // default 25

// avr32dd20 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can reset the normal way because power is not connected before the button, but do this anyway! 
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// enable OTG mode in momentary mode
//#define USE_OTG_IN_MOMENTARY

// disable POVD by default since it's causing many customers a lot of issues
#define DEFAULT_POST_OFF_VOLTAGE_SECONDS  0

// ======================================