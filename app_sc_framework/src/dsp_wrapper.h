// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "gpio_control_task.h"

#ifdef __XC__
void dsp_task_0(chanend c_dsp_0);
void dsp_task_1(chanend c_dsp_1, control_input_t * unsafe control_input);
#else
#include <xcore/parallel.h>
#include <xcore/channel.h>
#include <xcore/channel_transaction.h>

DECLARE_JOB(dsp_task_0, (chanend_t));
void dsp_task_0(chanend_t c_dsp_0);

DECLARE_JOB(dsp_task_1, (chanend_t, control_input_t *));
void dsp_task_1(chanend_t c_dsp_1, control_input_t *control_input);

#endif

