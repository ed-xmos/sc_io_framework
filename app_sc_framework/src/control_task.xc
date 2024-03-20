// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>
#include <string.h>
#include <platform.h>

#include "app_main.h"
#include "control_task.h"
#include "neopixel.h"
#include "adc_pot.h"

#define VU_GREEN    0x000010
#define VU_RED      0x002000
#define VU_OFF      0x000000


#define BAUD_RATE 115200

unsafe void vu_to_pixels(control_input_t * unsafe control_input, neopixel_state &np_state){
    for(int i = 0; i < 12; i++){
        int32_t threshold = 1 << (2 * i + 7);
        if(control_input->vu[0] > threshold){
            if(i < 8){
                np_state.data[i] = VU_GREEN;
            } else {
                np_state.data[i] = VU_RED;
            }
        } else {
            np_state.data[i] = VU_OFF;
        }

        if(control_input->vu[1] > threshold){
            if(i < 8){
                np_state.data[23 - i] = VU_GREEN;
            } else {
                np_state.data[23 - i] = VU_RED;
            }
        } else {
            np_state.data[23 - i] = VU_OFF;
        }
    }
}


void control_task(  client uart_tx_if i_uart_tx,
                    chanend c_adc, control_input_t * unsafe control_input,
                    out buffered port:32 p_neopixel, clock cb_neo,
                    client input_gpio_if i_gpio_mc_buttons,
                    client output_gpio_if i_gpio_mc_leds
                    ){
    printf("control_task\n");

    // Neopixel setup
    neopixel_state np_state = {0};
    const unsigned length = 24;
    neopixel_init(np_state, length, p_neopixel, cb_neo, 3);

    // UART data
    const uint8_t msg[] = "Hello world!\n";
    unsigned msg_idx = 0;

    while(1){
        unsafe{vu_to_pixels(control_input, np_state);}
        while(!neopixel_drive_pins(np_state, p_neopixel)); // Takes about 1.2 ms for 24 neopixels
        
        unsigned ch = 0;
        c_adc <: (uint32_t)(ADC_CMD_READ | ch);
        unsigned adc0;
        c_adc :> adc0;
        printf("ADC ch %u : %u\n", ch, adc0);

        // // Read buttons
        // unsigned pb;
        // p_buttons :> pb;
        // if((pb & 0x1) == 0){ // Button 0
        // }
        // if((pb & 0x2) == 0){ // Button 1
        // }

        // Send a character to the UART
        i_uart_tx.write(msg[msg_idx]);
        if(++msg_idx == strlen((const char*)msg)){
            msg_idx = 0;
        }

        delay_milliseconds(100);
    }
}


void uart_task(server uart_tx_if i_uart_tx, out port p_uart_tx){
    printstrln("uart_task");
    output_gpio_if i_gpio_tx[1];
    char pin_map[] = {0}; // We output on bit 0 of the 4b port

    [[combine]]
    par{
        uart_tx(i_uart_tx, null,
                BAUD_RATE, UART_PARITY_NONE, 8, 1,
                i_gpio_tx[0]);
        output_gpio(i_gpio_tx, 1, p_uart_tx, pin_map);
    }
}
