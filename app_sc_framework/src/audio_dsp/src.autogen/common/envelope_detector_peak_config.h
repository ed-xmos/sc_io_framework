// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef ENVELOPE_DETECTOR_PEAK_CONFIG_H
#define ENVELOPE_DETECTOR_PEAK_CONFIG_H

#include <stages/adsp_module.h>


typedef struct
{
    int32_t attack_alpha;
    int32_t release_alpha;
    int32_t envelope;
}envelope_detector_peak_config_t;

#endif

