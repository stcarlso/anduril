// aw2016.c: Device driver for AW2016 pheripheral 
// Copyright (C) 2025 - 2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drivers/twi/twi_master.h"
#include <util/delay.h>

#define AW2016_REG_GCR2 0x04

// default channels (user can modify in hwdef.h)
#ifndef AUX_CH_RED
#define AUX_CH_RED  2
#endif
#ifndef AUX_CH_GRN
#define AUX_CH_GRN  1
#endif
#ifndef AUX_CH_BLU
#define AUX_CH_BLU  0
#endif

// allow sleep by default, set to 1 if fast response required from driver
uint8_t adv_rgb_do_not_sleep = 0;

// internal private state (not exposed)
static uint8_t current_r = 0;
static uint8_t current_g = 0;
static uint8_t current_b = 0;

// ####################################
// ##      HELPER ROUTINES           ##
// ####################################

uint8_t twi_write(uint8_t addr, uint8_t *data, uint8_t len)
{
    TWI0.MADDR = (addr << 1);                   // send address (write)
    while (!(TWI0.MSTATUS & TWI_WIF_bm));       // wait for write interrupt flag
    if (TWI0.MSTATUS & TWI_RXACK_bm) return 1;  // NACK received
    for (uint8_t i = 0; i < len; i++)
    {
        TWI0.MDATA = data[i];
        while (!(TWI0.MSTATUS & TWI_WIF_bm));   // wait
        if (TWI0.MSTATUS & TWI_RXACK_bm) return 1;
    }
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;             // send stop
    return 0;                                   // success
}

uint8_t twi_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint32_t timeout;

    // ---- START + slave addr (write) ----
    TWI0.MADDR = (addr << 1) | 0;   // LSB=0 for WRITE

    timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
        if (--timeout == 0) return 1;
    }
    if (TWI0.MSTATUS & TWI_RXACK_bm) return 2; // NACK

    // ---- Send register address ----
    TWI0.MDATA = reg;

    timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
        if (--timeout == 0) return 3;
    }
    if (TWI0.MSTATUS & TWI_RXACK_bm) return 4;

    // ---- Repeated START + slave addr (read) ----
    TWI0.MADDR = (addr << 1) | 1;   // LSB=1 for READ

    timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
        if (--timeout == 0) return 5;
    }
    if (TWI0.MSTATUS & TWI_RXACK_bm) return 6;

    // ---- Read bytes ----
    for (uint8_t i = 0; i < len; i++)
    {
        timeout = TWI_TIMEOUT;
        while (!(TWI0.MSTATUS & TWI_RIF_bm)) {
            if (--timeout == 0) return 7;
        }

        buf[i] = TWI0.MDATA;

        if (i == (len - 1)) {
            // Last byte → send NACK + STOP
            TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_STOP_gc;
        } else {
            // More bytes → send ACK
            TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
        }
    }
    return 0; // success
}

static void aw2016_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    twi_write(AW2016_ADDR, buf, 2);
}

// ####################################
// ##      EXTERNAL API              ##
// ####################################

void aw2016_init(void) 
// AW2016 initialization 
{
    aw2016_write_reg(0x00, 0x55);  // reset internal registers
    _delay_ms(2);
    aw2016_wake();

    // initialize default values and rgb calibration 
    aw2016_set_global_current(AW2016_ISET_10mA);
    aw2016_set_rgb_calibration(RED_LED_CAL_VAL,GRN_LED_CAL_VAL,BLU_LED_CAL_VAL);

    // default to all 3 channels as manual mode
    aw2016_set_manual_pattern_mode(AUX_CH_RED,0);
    aw2016_set_manual_pattern_mode(AUX_CH_GRN,0);
    aw2016_set_manual_pattern_mode(AUX_CH_BLU,0);

    aw2016_set_global_brightness(AUX_HIGH_LVL_LEGACY);
    aw2016_sleep();
}

void aw2016_set_rgb_calibration(uint8_t r, uint8_t g, uint8_t b)
// uses the Anduril mode configuration register's CUR bits to for a 4 bit calibration
{
    uint8_t reg_r;
    uint8_t reg_g;
    uint8_t reg_b;

    r &= 0x0F;  // clamp to 0-15
    g &= 0x0F;
    b &= 0x0F;

    twi_read(AW2016_ADDR, 0x31+AUX_CH_RED, &reg_r, 1);
    twi_read(AW2016_ADDR, 0x31+AUX_CH_GRN, &reg_g, 1);
    twi_read(AW2016_ADDR, 0x31+AUX_CH_BLU, &reg_b, 1);

    // replace lower 4 bits with value
    reg_r = (reg_r & 0xF0) | r;
    reg_g = (reg_g & 0xF0) | g;
    reg_b = (reg_b & 0xF0) | b;

    aw2016_write_reg(0x31+AUX_CH_RED,reg_r);
    aw2016_write_reg(0x31+AUX_CH_GRN,reg_g);
    aw2016_write_reg(0x31+AUX_CH_BLU,reg_b);
}

void aw2016_set_global_current(aw2016_current_t cur)
// set the global current limit from 5mA to 30mA
{
    static uint8_t reg;
    // Step 1: Read the current value of GCR2
    twi_read(AW2016_ADDR, AW2016_REG_GCR2, &reg, 1);
    // Step 2: Clear bits 1:0 (mask 0b11111100)
    reg &= ~0x03;
    // Step 3: Insert new setting into bits 1:0
    reg |= (uint8_t)cur;
    // Step 4: Write back
    aw2016_write_reg(AW2016_REG_GCR2,reg);
}

void aw2016_set_manual_pattern_mode(uint8_t channel, uint8_t mode)
// set individual pattern mode for each channel
// mode = 0 : manual, 1: pattern mode
{
    static uint8_t reg;
    twi_read(AW2016_ADDR, 0x31+channel, &reg, 1);
    if (mode==0){
        // manual mode
        reg &= ~(1 << 4);  // clear bit 4
    }
    else{
        // pattern mode
        reg |= (1 << 4);   // set bit 4
    }
    aw2016_write_reg(0x31+channel,reg);
}

void aw2016_set_global_brightness(uint8_t pwm)
// sets all channels to the same pwm value
{
    aw2016_set_led(0, pwm);
    aw2016_set_led(1, pwm);
    aw2016_set_led(2, pwm);
}

void aw2016_set_led(uint8_t channel, uint8_t pwm)
// sets pwm of a single channel
{
    aw2016_write_reg(0x34 + channel, pwm);
}

void aw2016_set_rgb(uint8_t r, uint8_t g, uint8_t b)
// manually sets pwm value for r g and b
{
    current_r = r;
    current_g = g;
    current_b = b;

    aw2016_set_led(AUX_CH_RED, r);
    aw2016_set_led(AUX_CH_GRN, g);
    aw2016_set_led(AUX_CH_BLU, b);
}

void aw2016_set_hsv(uint8_t h, uint8_t s, uint8_t v)
// set rgb led based on hsv values
/* Example:
    red = hsv(0,255,255)
    green = hsv(85,255,255)
    blue = hsv(170,255,255)
    white = hsv(xx,0,255)
    dim purple = hsv(200,80,100)
*/
{
    uint8_t region, remainder, p, q, t;
    uint8_t r, g, b;

    if (s == 0) {
        // achromatic grey-white
        r = g = b = v;
        aw2016_set_rgb(r, g, b);
        return;
    }

    // Hue sector: 0–5
    region = h / 43;    // 256 / 6 ≈ 43
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default: // region 5
            r = v; g = p; b = q;
            break;
    }
    aw2016_set_rgb(r, g, b);
}

void aw2016_sleep(void)
// chip enters standby mode of 10uA
{
    aw2016_write_reg(0x01, 0x00);  
}

void aw2016_wake(void)
// chip exits standby mode
{
    aw2016_write_reg(0x01, 0x01);  
}

void aw2016_enable_leds(void)
// enables all leds
{
    aw2016_write_reg(0x30, 0x07);
}

void aw2016_disable_leds(void)
// disables all leds but chip is awake
{
    aw2016_write_reg(0x30, 0x00);
}

// ####################################
// ##      PRE-DEFINED EFFECTS       ##
// ####################################

void aw2016_test(void)
// hello world
{
    aw2016_set_rgb(10,255,255); // cyan with a dash of red
    aw2016_enable_leds();
}

void aw2016_rainbow_rgb(uint8_t pwm)
// rainbow fade but pausing at r,g and b
{
    // set all three channels to manual mode 
    aw2016_set_manual_pattern_mode(AUX_CH_RED,0);
    aw2016_set_manual_pattern_mode(AUX_CH_GRN,0);
    aw2016_set_manual_pattern_mode(AUX_CH_BLU,0);

    // set PWM levels (out of 255)
    aw2016_set_global_brightness(pwm);

    // set T1/T2
    aw2016_write_reg(0x37+(AUX_CH_RED)*3, 0x66);
    aw2016_write_reg(0x37+(AUX_CH_GRN)*3, 0x66);
    aw2016_write_reg(0x37+(AUX_CH_BLU)*3, 0x66);

    // set T3/T4
    aw2016_write_reg(0x38+(AUX_CH_RED)*3, 0x6A);
    aw2016_write_reg(0x38+(AUX_CH_GRN)*3, 0x6A);
    aw2016_write_reg(0x38+(AUX_CH_BLU)*3, 0x6A);

    // set T0/Repeat
    aw2016_write_reg(0x39+(AUX_CH_RED)*3, 0x00);
    aw2016_write_reg(0x39+(AUX_CH_GRN)*3, 0x80);
    aw2016_write_reg(0x39+(AUX_CH_BLU)*3, 0xB0);

    // set all three channels to pattern mode 
    aw2016_set_manual_pattern_mode(AUX_CH_RED,1);
    aw2016_set_manual_pattern_mode(AUX_CH_GRN,1);
    aw2016_set_manual_pattern_mode(AUX_CH_BLU,1);

    // enable leds
    aw2016_enable_leds();
}

void aw2016_rainbow_blend(uint8_t pwm)
// rainbow fade smoothly, pwm needs to be 255 to look good
// use analog control to adjust brightness
{
    // set all three channels to manual mode 
    aw2016_set_manual_pattern_mode(AUX_CH_RED,0);
    aw2016_set_manual_pattern_mode(AUX_CH_GRN,0);
    aw2016_set_manual_pattern_mode(AUX_CH_BLU,0);

    // set PWM levels (out of 255)
    aw2016_set_global_brightness(pwm);

    // set T1/T2
    aw2016_write_reg(0x37+(AUX_CH_RED)*3, 0x80);
    aw2016_write_reg(0x37+(AUX_CH_GRN)*3, 0x80);
    aw2016_write_reg(0x37+(AUX_CH_BLU)*3, 0x80);

    // set T3/T4
    aw2016_write_reg(0x38+(AUX_CH_RED)*3, 0x88);
    aw2016_write_reg(0x38+(AUX_CH_GRN)*3, 0x88);
    aw2016_write_reg(0x38+(AUX_CH_BLU)*3, 0x88);

    // set T0/Repeat
    aw2016_write_reg(0x39+(AUX_CH_RED)*3, 0x00);
    aw2016_write_reg(0x39+(AUX_CH_GRN)*3, 0x80);
    aw2016_write_reg(0x39+(AUX_CH_BLU)*3, 0xB0);

    // set all three channels to pattern mode 
    aw2016_set_manual_pattern_mode(AUX_CH_RED,1);
    aw2016_set_manual_pattern_mode(AUX_CH_GRN,1);
    aw2016_set_manual_pattern_mode(AUX_CH_BLU,1);

    // enable leds
    aw2016_enable_leds();
}