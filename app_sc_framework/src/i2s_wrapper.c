// Copyright 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <string.h>

#include "app_main.h"
#include "xua_conf.h"
#include "i2s_wrapper.h"
#include "xua_wrapper.h"
#include "sw_pll.h"


port_t p_mclk  = PORT_MCLK_IN;
port_t p_bclk = PORT_I2S_BCLK;
port_t p_lrclk = PORT_I2S_LRCLK;

port_t p_din [NUM_IN] = {PORT_I2S_ADC_DATA};
port_t p_dout[NUM_OUT] = {PORT_I2S_DAC_DATA};

xclock_t clk_bclk = XS1_CLKBLK_1;

I2S_CALLBACK_ATTR
void i2s_init(void *app_data, i2s_config_t *i2s_config)
{
    printf("i2s_init\n");

    i2s_config->mclk_bclk_ratio = MCLK_48 / (MIN_FREQ * 32);
    i2s_config->mode = I2S_MODE_I2S;

    (void) app_data;
}

I2S_CALLBACK_ATTR
void i2s_send(void *app_data, size_t n, int32_t *send_data)
{
    chanend_t c_xua = *((unsigned *)app_data);
    (void)c_xua;
}

I2S_CALLBACK_ATTR
void i2s_receive(void *app_data, size_t n, int32_t *receive_data)
{
    chanend_t c_xua = *((unsigned *)app_data);
    *receive_data = 0x10000000;
    xua_exchange(c_xua, receive_data);
}

I2S_CALLBACK_ATTR
i2s_restart_t i2s_restart_check(void *app_data)
{
    return I2S_NO_RESTART;
}


void i2s_wrapper(chanend_t c_xua) {
    sw_pll_fixed_clock(MCLK_48);

    i2s_callback_group_t i_i2s = {
            .init = (i2s_init_t) i2s_init,
            .restart_check = (i2s_restart_check_t) i2s_restart_check,
            .receive = (i2s_receive_t) i2s_receive,
            .send = (i2s_send_t) i2s_send,
            .app_data = &c_xua,
    };

    printf("i2s_master\n");
    i2s_master(
            &i_i2s,
            p_dout,
            NUM_OUT,
            p_din,
            NUM_IN,
            p_bclk,
            p_lrclk,
            p_mclk,
            clk_bclk);
}