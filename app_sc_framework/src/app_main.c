// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <limits.h>

#include <xcore/channel.h>
#include <xcore/channel_streaming.h>
#include <xcore/parallel.h>
#include <xcore/select.h>
#include <xcore/hwtimer.h>
#include <print.h>

#include "app_main.h"
#include "adc_task.h"
#include "control_task.h"
#include "i2s_wrapper.h"
#include "xua_wrapper.h"
#include "xua_conf.h"
#include "sw_pll.h"
#include "aic3204.h"
#include "xua_conf.h"

void aic3204_board_init();

///////// Tile main functions where we par off the threads ///////////

void main_tile_0(chanend_t c_cross_tile[2]){

    delay_milliseconds(1); // Wait for aic3204_codec_reset

    sw_pll_fixed_clock(MCLK_48);
    aic3204_board_init();

    PAR_JOBS(
        PJOB(adc_task, ()),
        PJOB(control_task, ()),
        PJOB(xua_wrapper, (c_cross_tile[0])) // This spawns 4 tasks
    );
}

void main_tile_1(chanend_t c_cross_tile[2]){

    aic3204_codec_reset();

    PAR_JOBS(
        PJOB(i2s_wrapper, (c_cross_tile[0]))
    );
}