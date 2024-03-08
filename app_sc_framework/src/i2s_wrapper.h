// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xcore/channel.h>
#include <xcore/parallel.h>

#include "app_main.h"           // audio_frame_t
#include "i2s.h"


DECLARE_JOB(i2s_wrapper, (chanend_t, chanend_t, chanend_t));
void i2s_wrapper(chanend_t c_xua, chanend_t c_dsp_0, chanend_t c_dsp_1);