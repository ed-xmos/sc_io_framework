// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>
#include <string.h>
#include <platform.h>

#include "app_main.h"
#include "i2c.h"
#include "control_task.h"
#include "neopixel.h"
#include "adc_task.h"
#include "uart.h"

#define VU_GREEN    0x000010
#define VU_RED      0x001000
#define VU_OFF      0x000000

on tile[0]: port p_buttons = XS1_PORT_4D;
on tile[0]: port p_wifi_ctl = XS1_PORT_4F;
on tile[0]: out buffered port:32 p_neopixel = WIFI_MISO;
on tile[0]: clock cb_neo = XS1_CLKBLK_3;


unsafe void vu_to_pixels(control_input_t * unsafe control_input, neopixel_state &np_state){
    for(int i = 0; i < 12; i++){
        int32_t threshold = 1 << (i + 20);
        for(int ch = 0; ch < 2; ch++){
            if(control_input->vu[ch] > threshold){
                if(i < 8){
                    np_state.data[i] = VU_GREEN;
                } else {
                    np_state.data[i] = VU_RED;
                }
            } else {
                np_state.data[i] = VU_OFF;
            }
        }
    }    
}

void control_task(chanend c_uart, chanend c_adc, control_input_t * unsafe control_input){
    printstrln("control_task");

    // Drive a line high on WiFi to provide power from IO pin
    p_wifi_ctl <: 0xf; // Drive 3.3V to these pins & disable WiFi chip
    // set_pad_properties(p_wifi_ctl, DRIVE_12MA, PULL_NONE, 0, 0);

    // Neopixel setup
    neopixel_state np_state = {0};
    const unsigned length = 24;
    neopixel_init(np_state, length, p_neopixel, cb_neo, 3);

    // UART data
    const uint8_t msg[] = "Hello world!\n";
    unsigned msg_idx = 0;

    c_adc <: ADC_CMD_CAL_MODE_START;

    while(1){
        unsafe{vu_to_pixels(control_input, np_state);}
        while(!neopixel_drive_pins(np_state, p_neopixel)); // Takes about 1.2 ms for 24 neopixels
        
        c_adc <: ADC_CMD_READ | 0;
        unsigned adc0;
        c_adc :> adc0;
        printuintln(adc0);

        // Read buttons
        unsigned pb;
        p_buttons :> pb;
        if((pb & 0x1) == 0){ // Button 0
            c_adc <: ADC_CMD_CAL_MODE_START;
        }
        if((pb & 0x2) == 0){ // Button 1
            c_adc <: ADC_CMD_CAL_MODE_START;
            c_adc <: ADC_CMD_CAL_MODE_FINISH;
        }

        // Send a character to the UART
        c_uart <: msg[msg_idx];
        if(++msg_idx == strlen((const char*)msg)){
            msg_idx = 0;
        }

        delay_milliseconds(100);
    }
}

// HIL_UART_TX_CALLBACK_ATTR
// void tx_fifo_empty(void *app_data){
//     // Do nothing
// }

void uart_task(chanend c_uart){
    printstrln("uart_task");

    // port_t p_uart_tx = XS1_PORT_4B; // Bit 2 X1D06
    // hwtimer_t tmr = hwtimer_alloc();
    // const unsigned uart_buff_size = 64;

    // uint8_t buffer[uart_buff_size + 1] = {0};
    // uart_tx_t uart;
    // uart_tx_init(&uart, p_uart_tx, 115200, 8, UART_PARITY_NONE, 1, tmr, buffer, sizeof(buffer), tx_fifo_empty, &uart);
    // uart.tx_port_high_val = (1 << 2); // Use bit 2 for signalling

    // while(1){
    //     // Wait on channel for next char
    //     uint8_t tx_data = chan_in_byte(c_uart);
    //     uart_tx(&uart, tx_data);
    // }
}
