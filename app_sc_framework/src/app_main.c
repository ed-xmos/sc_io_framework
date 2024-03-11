// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <limits.h>

#include <xcore/channel.h>
#include <xcore/parallel.h>
#include <print.h>

#include "app_main.h"
#include "adc_task.h"
#include "control_task.h"
#include "i2s_wrapper.h"
#include "xua_wrapper.h"
#include "dsp_wrapper.h"
#include "xua_conf.h"
#include "sw_pll.h"
#include "aic3204.h"
#include "xua_conf.h"

void aic3204_board_init();

///////// Tile main functions where we par off the threads ///////////

void main_tile_0(chanend_t c_cross_tile[3]){
    chan_in_word(c_cross_tile[0]); // Synch with init on tile[1]
    aic3204_board_init();

    // Memory shared between dsp_task_0 and control_task
    control_input_t control_input;

    // ADC to control thread channel
    channel_t c_adc = chan_alloc(); 

    PAR_JOBS(
        PJOB(adc_task, (c_adc.end_b)),
        PJOB(control_task, (c_cross_tile[2], c_adc.end_a, &control_input)),
        PJOB(dsp_task_0, (c_cross_tile[1], &control_input)),
        PJOB(xua_wrapper, (c_cross_tile[0])) // This spawns 4 tasks
    );
}

void main_tile_1(chanend_t c_cross_tile[2]){
    sw_pll_fixed_clock(MCLK_48);
    aic3204_codec_reset();
    chan_out_word(c_cross_tile[0], 0); // Synch with codec reset

    // I2S to DSP 1 channel
    channel_t c_dsp_1 = chan_alloc(); 

    PAR_JOBS(
        PJOB(i2s_wrapper, (c_cross_tile[0], c_cross_tile[1], c_dsp_1.end_a)),
        PJOB(dsp_task_1, (c_dsp_1.end_b)),
        PJOB(uart_task, (c_cross_tile[2]))
    );
}