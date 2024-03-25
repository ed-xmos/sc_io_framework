// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#pragma once
#include <xccompat.h>
#include <stdint.h>

// XC safe wrapper for adsp

void app_dsp_init(void);

// send data to dsp
void app_dsp_source(REFERENCE_PARAM(int32_t, data));

// read output
void app_dsp_sink(REFERENCE_PARAM(int32_t, data));

// do dsp
void app_dsp_main(chanend c_control);



