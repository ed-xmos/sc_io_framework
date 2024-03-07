// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include "app_main.h"
#include "i2c.h"
#include "neopixel.h"

void control_task(void){
    printstrln("control_task");

    port_enable(WIFI_WIRQ);
    port_out(WIFI_WIRQ, 1); // Drive 3.3V to this pin
    set_pad_properties(WIFI_WIRQ, DRIVE_12MA, PULL_NONE, 0, 0);

    xclock_t cb = XS1_CLKBLK_3;
    clock_enable(cb);
    port_t p_neopixel = WIFI_MOSI;
    port_enable(p_neopixel);
    port_start_buffered(p_neopixel, 32);
    neopixel_state state;
    unsigned length = 24;

    hwtimer_realloc_xc_timer();
    neopixel_init(&state, length, p_neopixel, cb, 3);

    while(1){
        for(unsigned i=0;i<length;i++){
            state.data[i] = i;
        }
        while(!neopixel_drive_pins(&state, p_neopixel)); // Takes about 1.2 ms for 24 neopixels
    }
}
