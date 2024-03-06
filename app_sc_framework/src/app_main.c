// Copyright 2023 XMOS LIMITED.
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
#include "i2s_wrapper.h"
#include "xua_wrapper.h"
#include "xua_conf.h"

///////// Tile main functions where we par off the threads ///////////

void main_tile_0(chanend_t c_cross_tile[2]){
    PAR_JOBS(
        PJOB(adc_task, ())
    );
}

void main_tile_1(chanend_t c_cross_tile[2]){
    channel_t c_aud = chan_alloc();

    PAR_JOBS(
        PJOB(i2s_wrapper, (c_aud.end_b)),
        PJOB(xua_wrapper, (c_aud.end_a)) // This spawns 4 tasks
    );
}