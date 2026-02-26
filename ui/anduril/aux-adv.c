// aux-adv.c: Advanced aux functions
// Copyright (C) 2017-2023 Selene ToyKeeper
//           (C) 2025-2026 Loneoceans
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef USE_AUXRGB_LEDS_ADV

uint8_t is_running_adv_rainbow = 0;
uint8_t is_adv_rgb_driver_awake = 0;
uint8_t to_update_rgb = 0;

void set_auxrgb_power(uint8_t value) {
    // value: 0b00BBGGRR
    if (is_running_adv_rainbow){
        /* Check to see if advanced rainbow was previously running.
           If so, clear adv. aux settings, clear rainbow running flag.
           Set flag to allow regular rgb values to be set 
        */
        aw2016_init();               // clear settings
        is_running_adv_rainbow = 0;  // clear flag for aux-leds-adv.c control
        is_adv_rgb_driver_awake = 0; // init sets driver to sleep mode
    }
    
    // add handling for sleep and wake if value = 0x00
    if (value == 0 && is_adv_rgb_driver_awake == 0){
        ; // value is 0 and driver is asleep, stay asleep, do nothing here
    }
    else if (value == 0 && is_adv_rgb_driver_awake == 1){
        // driver is awake but value is now 0; set values to 0; go to sleep
        // but don't sleep when in some modes like red/blue strobe for fast response
        aw2016_set_rgb(0,0,0);
        if (adv_rgb_do_not_sleep == 0){
            aw2016_disable_leds();
            aw2016_sleep();                 // good night
            is_adv_rgb_driver_awake = 0;
        }
        to_update_rgb = 0;                  // sleeping, no need to update rgb
    }
    else if (is_adv_rgb_driver_awake == 0 && value != 0){
        // value is not 0 but driver is currently sleeping
        aw2016_wake();                      // wake up!
        aw2016_set_rgb(0,0,0);              // ensure no rgb blip before turning on
        aw2016_enable_leds();
        is_adv_rgb_driver_awake = 1;
        to_update_rgb = 1;
    }
    else {
        // driver is already awake
        to_update_rgb = 1;
    }

    if (to_update_rgb){
        // time to set the adv aux rgb leds
        uint8_t chs[] = {AUX_CH_RED, AUX_CH_GRN, AUX_CH_BLU};
        for (uint8_t i=0; i<3; i++) {
            uint8_t lvl = (value >> (i<<1)) & 0x03;
            uint8_t ch = chs[i];
            switch (lvl) { // each pair of bits: 0=off, 1=low, 2=high
                case 0:  // LED off
                    aw2016_set_led(ch, 0);
                    break;
                case 1:  // LED low
                    aw2016_set_led(ch, AUX_LOW_LVL_LEGACY);
                    aw2016_set_rgb_calibration(RED_LED_CAL_LOW_VAL,GRN_LED_CAL_LOW_VAL,BLU_LED_CAL_LOW_VAL);
                    aw2016_set_global_current(AW2016_ISET_5mA);
                    break;
                default:  // LED high
                    aw2016_set_led(ch, AUX_HIGH_LVL_LEGACY);
                    aw2016_set_rgb_calibration(RED_LED_CAL_VAL,GRN_LED_CAL_VAL,BLU_LED_CAL_VAL);
                    aw2016_set_global_current(AW2016_ISET_10mA);
                    break;
            }
        }
        to_update_rgb = 0;  // rgb update is done
    }
}
#endif  // ifdef USE_AUX_RGB_ADV
