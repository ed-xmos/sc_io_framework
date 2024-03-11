// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>
#include <xcore/interrupt_wrappers.h>

#include "app_main.h"
#include "i2c.h"
#include "control_task.h"
#include "adc_task.h"
#include "neopixel.h"
#include "uart.h"

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

void control_task(chanend_t c_uart, chanend_t c_adc, control_input_t *control_input){
    printstrln("control_task");

    // Buttons
    port_t p_buttons = XS1_PORT_4D;
    port_enable(p_buttons);

    // Drive a line high on WiFi to provide power from IO pin
    port_enable(XS1_PORT_4F);
    port_out(XS1_PORT_4F, 0xf); // Drive 3.3V to these pins & disable WiFi chip
    set_pad_properties(XS1_PORT_4F, DRIVE_12MA, PULL_NONE, 0, 0);

    // Neopixel setup
    xclock_t cb = XS1_CLKBLK_3;
    clock_enable(cb);
    port_t p_neopixel = WIFI_MISO;
    port_enable(p_neopixel);
    port_start_buffered(p_neopixel, 32);
    neopixel_state np_state = {0};
    const unsigned length = 24;

    hwtimer_realloc_xc_timer();
    neopixel_init(&np_state, length, p_neopixel, cb, 3);

    // UART data
    const uint8_t msg[] = "Hello world!\n";
    unsigned msg_idx = 0;

    chan_out_word(c_adc, ADC_CMD_CAL_MODE_START);

    while(1){
        vu_to_pixels(control_input, &np_state);
        while(!neopixel_drive_pins(&np_state, p_neopixel)); // Takes about 1.2 ms for 24 neopixels
        chan_out_word(c_adc, ADC_CMD_READ | 0);
        unsigned adc0 = chan_in_word(c_adc);
        printuintln(adc0);

        // Read buttons
        unsigned pb = port_in(p_buttons);
        if((pb & 0x1) == 0){ // Button 0
            chan_out_word(c_adc, ADC_CMD_CAL_MODE_START);
        }
        if((pb & 0x2) == 0){ // Button 1
            chan_out_word(c_adc, ADC_CMD_CAL_MODE_FINISH);
        }

        // Send a character to the UART
        chan_out_byte(c_uart, msg[msg_idx]);
        if(++msg_idx == strlen((char*)msg)){
            msg_idx = 0;
        }

        delay_milliseconds(100);
    }
}

HIL_UART_TX_CALLBACK_ATTR
void tx_fifo_empty(void *app_data){
    // Do nothing
}

DEFINE_INTERRUPT_PERMITTED(UART_TX_INTERRUPTABLE_FUNCTIONS, void, uart_task, chanend_t c_uart){
    printstrln("uart_task");

    port_t p_uart_tx = XS1_PORT_4B; // Bit 2 X1D06
    hwtimer_t tmr = hwtimer_alloc();
    const unsigned uart_buff_size = 64;

    uint8_t buffer[uart_buff_size + 1] = {0};
    uart_tx_t uart;
    uart_tx_init(&uart, p_uart_tx, 115200, 8, UART_PARITY_NONE, 1, tmr, buffer, sizeof(buffer), tx_fifo_empty, &uart);
    uart.tx_port_high_val = (1 << 2); // Use bit 2 for signalling

    while(1){
        // Wait on channel for next char
        uint8_t tx_data = chan_in_byte(c_uart);
        uart_tx(&uart, tx_data);
    }
}
