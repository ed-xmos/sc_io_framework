// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xcore/parallel.h>
#include <xcore/channel.h>

typedef struct control_input_t
{
    uint32_t vu[2];
}control_input_t;

DECLARE_JOB(control_task, (chanend_t, control_input_t *));
void control_task(chanend_t c_uart, control_input_t *control_input);

DECLARE_JOB(uart_task, (chanend_t));
void uart_task(chanend_t c_uart);