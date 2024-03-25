// Copyright 2023-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#pragma once

#include <stages/adsp_module.h>
#include "xcore/chanend.h"
#include <stdint.h>
#include <xcore/parallel.h>

typedef struct
{
    uint16_t res_id;
    uint16_t cmd_id;
    uint16_t payload_len;
    uint8_t direction;
}control_req_t;

DECLARE_JOB(dsp_control_thread, (chanend_t, module_instance_t**, size_t));

