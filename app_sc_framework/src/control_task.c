// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include "app_main.h"
#include "i2c.h"
#include "neopixel.h"

void control_task(void){
    printstrln("control_task");

    port_enable(XS1_PORT_4F);
    port_out(XS1_PORT_4F, 0xf); // Drive 3.3V to these pins & disable WiFi chip
    set_pad_properties(XS1_PORT_4F, DRIVE_12MA, PULL_NONE, 0, 0);


    xclock_t cb = XS1_CLKBLK_3;
    clock_enable(cb);
    port_t p_neopixel = WIFI_MISO;
    port_enable(p_neopixel);
    port_start_buffered(p_neopixel, 32);
    neopixel_state state;
    unsigned length = 64;


    hwtimer_realloc_xc_timer();
    neopixel_init(&state, length, p_neopixel, cb, 3);

    while(1){

        for(unsigned i=0;i<length;i++){
            state.data[i] = i;
        }

        while(!neopixel_drive_pins(&state, p_neopixel));

        delay_milliseconds(50);
    }
}
