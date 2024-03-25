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
#include "adsp.h"


extern adsp_pipeline_t * m_dsp;

// See adsp_generated_auto.c for RES IDs. Its the index into adsp_auto.modules
// See cmds.h for command IDs


void set_volume(int32_t vol){
    int32_t volume_shift = -SIG_EXP - 10; // 10 bit ADC 
    vol <<= volume_shift;

    printf("vol: %ld\n", vol);

    adsp_stage_control_cmd_t cmd;

    cmd.instance_id = 2; // VOLUME
    const unsigned NUM_VALUES_VOLUME = 1;
    const unsigned TYPE_INT32 = 4;
    const unsigned len = NUM_VALUES_VOLUME * TYPE_INT32;
    cmd.cmd_id = CMD_VOLUME_CONTROL_TARGET_GAIN;
    cmd.payload_len = len;

    int32_t payload_rd[NUM_VALUES_VOLUME] = {0};
    int32_t payload_wr[NUM_VALUES_VOLUME] = {vol};

    printf("m_dsp->n_modules: %d\n", m_dsp->n_modules);


    cmd.payload = (int8_t *)payload_rd;

    adsp_control_status_t ret = ADSP_CONTROL_BUSY;
    do{
        // Handle this
        ret = adsp_read_module_config(m_dsp->modules, m_dsp->n_modules, &cmd);
        delay_milliseconds(1);
        printf("ADSP_CONTROL READ\n");
    }while(ret == ADSP_CONTROL_BUSY);

    for(int i = 0; i < NUM_VALUES_VOLUME; i++){
        printf("read %d: %ld\n", i, payload_rd[i]);
    }

    cmd.payload = (int8_t *)payload_wr;

    ret = ADSP_CONTROL_BUSY;
    do{
        ret = adsp_write_module_config(m_dsp->modules, m_dsp->n_modules, &cmd);
        printf("ADSP_CONTROL WRITE\n");
    }while(ret == ADSP_CONTROL_BUSY);

}