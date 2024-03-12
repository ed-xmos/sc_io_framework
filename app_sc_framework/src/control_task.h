// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>
#ifndef __XC__
#include <xcore/parallel.h>
#include <xcore/channel.h>
#include <xcore/interrupt_wrappers.h>
#endif

typedef struct control_input_t
{
    uint32_t vu[2];
}control_input_t;

#ifdef __XC__
void control_task(chanend c_uart, chanend c_adc, control_input_t * unsafe control_input);

void uart_task(chanend c_uart);
#else

DECLARE_JOB(control_task, (chanend_t, chanend_t, control_input_t *));
void control_task(chanend_t c_uart, chanend_t c_adc, control_input_t *control_input);

DECLARE_JOB(INTERRUPT_PERMITTED(uart_task), (chanend_t));
void uart_task(chanend_t c_uart);
#endif