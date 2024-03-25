// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>
#include "i2c.h"
#include "uart.h"
#include "xua_conf.h"

typedef struct control_input_t
{
    uint32_t vu[NUM_USB_CHAN_OUT];
    int32_t output_gain[NUM_USB_CHAN_OUT];
}control_input_t;

#ifdef __XC__

void gpio_control_task( client uart_tx_if i_uart_tx,
                        chanend c_adc, control_input_t * unsafe control_input,
                        out buffered port:32 p_neopixel, clock cb_neo,
                        client input_gpio_if i_gpio_mc_buttons,
                        client output_gpio_if i_gpio_mc_leds
                        );
#endif

void set_volume(int32_t vol);
