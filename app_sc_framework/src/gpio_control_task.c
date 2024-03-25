// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>
#include <string.h>
#include <platform.h>

#include "adsp_pipeline.h"
#include "adsp_module.h"
#include "adsp_control.h"
#include "cmds.h"


extern adsp_pipeline_t * m_dsp;

// See adsp_generated_auto.c for RES IDs. Its the index into adsp_auto.modules
// See cmds.h for command IDs


void set_biquad_left_shift(int32_t ls){

    ls = ls / 100;

    printf("ls: %ld\n", ls);

    adsp_stage_control_cmd_t cmd;

    cmd.instance_id = 2; // CASCADED_BIQUADS
    const unsigned NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT = 8;
    const unsigned TYPE_INT32 = 4;
    const unsigned len = NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT * TYPE_INT32;
    cmd.cmd_id = CMD_CASCADED_BIQUADS_LEFT_SHIFT;
    cmd.payload_len = len;

    int32_t payload_rd[NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT] = {0};
    int32_t payload_wr[NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT] = {ls, ls, ls, ls, ls, ls, ls, ls};

    printf("m_dsp->n_modules: %d\n", m_dsp->n_modules);


    cmd.payload = (int8_t *)payload_rd;

    adsp_control_status_t ret = ADSP_CONTROL_BUSY;
    do{
        // Handle this
        ret = adsp_read_module_config(m_dsp->modules, m_dsp->n_modules, &cmd);
        delay_milliseconds(1);
        printf("ADSP_CONTROL READ\n");
    }while(ret == ADSP_CONTROL_BUSY);

    for(int i = 0; i < NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT; i++){
        printf("read %d: %ld\n", i, payload_rd[i]);
    }

    cmd.payload = (int8_t *)payload_wr;

    ret = ADSP_CONTROL_BUSY;
    do{
        ret = adsp_write_module_config(m_dsp->modules, m_dsp->n_modules, &cmd);
        printf("ADSP_CONTROL WRITE\n");
    }while(ret == ADSP_CONTROL_BUSY);

}