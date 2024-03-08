// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xcore/parallel.h>
#include <xcore/channel.h>
#include <xcore/channel_transaction.h>
#include "control_task.h"

DECLARE_JOB(dsp_task_0, (chanend_t, control_input_t *));
void dsp_task_0(chanend_t c_dsp_0, control_input_t *control_input);

DECLARE_JOB(dsp_task_1, (chanend_t));
void dsp_task_1(chanend_t c_dsp_1);


void dsp_tile_0_exchange(chanend_t c_dsp_0, int32_t *samples, size_t n_samples);
