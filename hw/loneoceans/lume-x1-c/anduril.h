// Copyright (C) 2017-2023 Selene ToyKeeper
//               2021-2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

//**************************************************
//** CONFIGURATION OPTIONS FOR LUME-X1-C          **
//**************************************************

#define HWDEF_H  loneoceans/lume-x1-c/hwdef.h

//***************************************
//**    RAMP TABLE AND OPERATIONS      **
//***************************************

#define RAMP_SIZE 150

// PWM1: DAC Data
// UDR x^4 curves specifically for Lume-X1 (AVR32DD20)

// 150 levels:
#define PWM1_LEVELS 1, 2, 10, 31, 76, 157, 291, 497, 796, 497, 727, 1023, 14, 19, 25, 32, 41, 51, 64, 78, 95, 115, 137, 162, 191, 223, 260, 300, 346, 396, 451, 513, 580, 653, 734, 821, 916, 1019, 463, 513, 566, 623, 685, 751, 821, 897, 977, 25, 27, 30, 32, 35, 38, 40, 44, 47, 50, 54, 58, 62, 66, 70, 75, 80, 85, 90, 96, 102, 108, 114, 121, 128, 135, 143, 150, 159, 167, 176, 185, 195, 205, 215, 226, 237, 248, 260, 272, 285, 298, 312, 326, 341, 356, 371, 387, 404, 421, 439, 457, 476, 495, 515, 535, 556, 578, 600, 623, 647, 671, 696, 722, 748, 775, 803, 832, 861, 891, 922, 954, 986, 1019, 432, 446, 461, 476, 491, 507, 523, 539, 556, 574, 591, 609, 628, 647, 666, 686, 706, 727, 748, 770, 792, 815, 838, 861, 885, 910, 935, 960, 986

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

// 150 levels:
#define PWM2_LEVELS V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V10, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25, V25

// Multiple Power Paths
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 13
#define LED_PATH3_PIN_LEVEL_MIN 48 // 51 for 160 levels

#define HALFSPEED_LEVEL 50
#define QUARTERSPEED_LEVEL 40   // check with TK about the mechanics of this

#define DEFAULT_LEVEL 59        // about same as 1 AMC7135
#define MAX_1x7135 59           // about same as 1 AMC7135 
// MIN_THERM_STEPDOWN defined as MAX_1x7135 in ramp-mode.h if not otherwise defined

#define RAMP_SMOOTH_FLOOR 7
#define RAMP_SMOOTH_CEIL 126    // about ~18-20W (Turbo ~40W)

#define RAMP_DISCRETE_FLOOR RAMP_SMOOTH_FLOOR
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 6

#define SIMPLE_UI_FLOOR 11
#define SIMPLE_UI_CEIL 114      // about ~12W
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
//#define BLINK_ONCE_TIME 32              // increase from 10 to make brighter

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

#define USE_AUX1_LED

// this light has three aux LED channels: R, G, B
#define USE_AUXRGB_LEDS

// this light introduces advanced aux 
#define USE_AUXRGB_LEDS_ADV

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
#define PARTY_STROBE_ONTIME 3
#define STROBE_OFF_LEVEL 1  // keep regulators on between strobes

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 30 // default 25

// avr32dd20 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can reset the normal way because power is not connected before the button, but do this anyway! 
#define USE_SOFT_FACTORY_RESET

// enable long-blink as negative sign
#define USE_LONG_BLINK_FOR_NEGATIVE_SIGN

// enable OTG mode in momentary mode
//#define USE_OTG_IN_MOMENTARY

// ======================================