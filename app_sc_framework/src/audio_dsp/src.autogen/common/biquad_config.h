// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef BIQUAD_CONFIG_H
#define BIQUAD_CONFIG_H

#include <stdint.h>
#include <stages/adsp_module.h>


typedef struct
{
    int left_shift;
    int32_t DWORD_ALIGNED filter_coeffs[5];
    int32_t reserved[3];
}biquad_config_t;

#endif

