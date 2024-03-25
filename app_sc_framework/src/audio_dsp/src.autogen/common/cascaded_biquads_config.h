// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef CASCADED_BIQUADS_CONFIG_H
#define CASCADED_BIQUADS_CONFIG_H

#include <stdint.h>
#include <stages/adsp_module.h>


typedef struct
{
    int left_shift[8];
    int32_t DWORD_ALIGNED filter_coeffs[40];
}cascaded_biquads_config_t;

#endif

