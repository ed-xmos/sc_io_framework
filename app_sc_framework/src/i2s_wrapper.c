// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <string.h>

#include "app_main.h"
#include "xua_conf.h"
#include "i2s_wrapper.h"
#include "xua_wrapper.h"
#include "dsp_wrapper.h"

#include <math.h>


port_t p_mclk  = PORT_MCLK_IN;
port_t p_bclk = PORT_I2S_BCLK;
port_t p_lrclk = PORT_I2S_LRCLK;

port_t p_din [I2S_NUM_LINES_IN] = {PORT_I2S_ADC_DATA};
port_t p_dout[I2S_NUM_LINES_OUT] = {PORT_I2S_DAC_DATA};

xclock_t clk_bclk = XS1_CLKBLK_1;


typedef struct i2s_cb_t{
    chanend_t c_xua;
    int32_t samples_in[NUM_USB_CHAN_IN];
    int32_t samples_out[NUM_USB_CHAN_OUT];
    chanend_t c_dsp_0;
    chanend_t c_dsp_1;
}i2s_cb_t;

I2S_CALLBACK_ATTR
void i2s_init(void *app_data, i2s_config_t *i2s_config)
{
    printf("i2s_init\n");

    i2s_config->mclk_bclk_ratio = MCLK_48 / (MIN_FREQ * 32 * 2);
    i2s_config->mode = I2S_MODE_I2S;

    (void) app_data;
}

I2S_CALLBACK_ATTR
void i2s_send(void *app_data, size_t n, int32_t *send_data)
{   
    i2s_cb_t *i2s_cb_args = app_data;
    
    for(unsigned ch = 0; ch < n; ch++){
        send_data[ch] = i2s_cb_args->samples_out[ch];
    }
}

I2S_CALLBACK_ATTR
void i2s_receive(void *app_data, size_t n, int32_t *receive_data)
{
    i2s_cb_t *i2s_cb_args = app_data;

    for(unsigned ch = 0; ch < n; ch++){
        i2s_cb_args->samples_in[ch] = receive_data[ch];
    }
}

I2S_CALLBACK_ATTR
i2s_restart_t i2s_restart_check(void *app_data)
{
    i2s_cb_t *i2s_cb_args = app_data;

    xua_exchange(i2s_cb_args->c_xua, i2s_cb_args->samples_out, i2s_cb_args->samples_in);
    dsp_tile_0_exchange(i2s_cb_args->c_dsp_0, i2s_cb_args->samples_out, NUM_USB_CHAN_OUT);

    return I2S_NO_RESTART;
}


void i2s_wrapper(chanend_t c_xua, chanend_t c_dsp_0, chanend_t c_dsp_1) {

    i2s_cb_t i2s_cb_args = {
        .c_xua = c_xua,
        .samples_in = {0},
        .samples_out = {0},
        .c_dsp_0 = c_dsp_0,
        .c_dsp_1 = c_dsp_1
    };


    i2s_callback_group_t i_i2s = {
            .init = (i2s_init_t) i2s_init,
            .restart_check = (i2s_restart_check_t) i2s_restart_check,
            .receive = (i2s_receive_t) i2s_receive,
            .send = (i2s_send_t) i2s_send,
            .app_data = &i2s_cb_args,
    };

    printf("i2s_master\n");
    i2s_master(
            &i_i2s,
            p_dout,
            I2S_NUM_LINES_OUT,
            p_din,
            I2S_NUM_LINES_IN,
            p_bclk,
            p_lrclk,
            p_mclk,
            clk_bclk);
}
