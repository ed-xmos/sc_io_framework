// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include "app_main.h"
#include "i2c.h"
#include "control_task.h"
#include "neopixel.h"

#define VU_GREEN    0x000010
#define VU_RED      0x001000
#define VU_OFF      0x000000


void vu_to_pixels(control_input_t *control_input, neopixel_state *np_state){
    for(int i = 0; i < 12; i++){
        int32_t threshold = 1 << (i + 20);
        for(int ch = 0; ch < 2; ch++){
            if(control_input->vu[ch] > threshold){
                if(i < 8){
                    np_state->data[i] = VU_GREEN;
                } else {
                    np_state->data[i] = VU_RED;
                }
            } else {
                np_state->data[i] = VU_OFF;
            }
        }
    }    
}

void control_task(chanend_t c_uart, control_input_t *control_input){
    printstrln("control_task");

    port_enable(XS1_PORT_4F);
    port_out(XS1_PORT_4F, 0xf); // Drive 3.3V to these pins & disable WiFi chip
    set_pad_properties(XS1_PORT_4F, DRIVE_12MA, PULL_NONE, 0, 0);

    xclock_t cb = XS1_CLKBLK_3;
    clock_enable(cb);
    port_t p_neopixel = WIFI_MISO;
    port_enable(p_neopixel);
    port_start_buffered(p_neopixel, 32);
    neopixel_state np_state = {{0}};
    const unsigned length = 24;

    hwtimer_realloc_xc_timer();
    neopixel_init(&np_state, length, p_neopixel, cb, 3);

    while(1){
        // for(unsigned i=0;i<length;i++){
        //     np_state.data[i] = i;
        // }
        vu_to_pixels(control_input, &np_state);
        while(!neopixel_drive_pins(&np_state, p_neopixel)); // Takes about 1.2 ms for 24 neopixels
    }
}


void uart_task(chanend_t c_uart){
    printstrln("uart_task");

}
