// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef VOLUME_CONTROL_CONFIG_H
#define VOLUME_CONTROL_CONFIG_H

#include <stdint.h>
#include <stages/adsp_module.h>


typedef struct
{
    int32_t target_gain;
    int32_t gain;
    int32_t slew_shift;
    uint8_t mute;
}volume_control_config_t;

#endif

