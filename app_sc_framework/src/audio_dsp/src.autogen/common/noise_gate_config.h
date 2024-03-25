// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef NOISE_GATE_CONFIG_H
#define NOISE_GATE_CONFIG_H

#include <stages/adsp_module.h>


typedef struct
{
    int32_t attack_alpha;
    int32_t release_alpha;
    int32_t envelope;
    int32_t threshold;
    int32_t gain;
}noise_gate_config_t;

#endif

