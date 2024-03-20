// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>
#include "i2c.h"
#include "xua_conf.h"

typedef struct control_input_t
{
    uint32_t vu[2];
}control_input_t;

#ifdef __XC__

void control_task(chanend c_uart, chanend c_adc, control_input_t * unsafe control_input);

void uart_task(chanend c_uart);

#endif

// Calculates the expected MCLK from a given normal sample rate
inline uint32_t get_master_clock_from_samp_rate(uint32_t sample_rate){
    return (sample_rate % 48000 == 0) ? MCLK_48 : MCLK_441;
}