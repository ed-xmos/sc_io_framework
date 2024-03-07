// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xcore/channel.h>
#include <xcore/parallel.h>

#include "xua_conf.h"

DECLARE_JOB(xua_wrapper, (chanend_t));
void xua_wrapper(chanend_t c_aud);

void xua_exchange(chanend_t c_aud, int32_t samples_out[NUM_USB_CHAN_OUT], int32_t samples_in[NUM_USB_CHAN_IN]);