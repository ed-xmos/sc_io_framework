// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <platform.h>
#include <xs1.h>

#include "app_config.h"
#include "adc_task.h"
#include "control_task.h"
// #include "dsp_wrapper.h"
#include "xua_conf.h"
extern "C"{
    #include "sw_pll.h"
}
#include "aic3204.h"
#include "xua_conf.h"
#include "xua.h"
#include "xud.h"


on tile[0]: port p_for_mclk_count = XS1_PORT_16B;
on tile[0]: port p_for_mclk_in = WIFI_CLK;
on tile[0]: clock usb_mclk_in_clk = USB_MCLK_COUNT_CLK_BLK;

on tile[1]: buffered out port:32 p_i2s_dac[]    = {PORT_I2S_DAC_DATA};   /* I2S Data-line(s) */
on tile[1]: buffered in port:32 p_i2s_adc[]     = {PORT_I2S_ADC_DATA};   /* I2S Data-line(s) */
on tile[1]: buffered out port:32 p_lrclk        = PORT_I2S_LRCLK;    /* I2S Bit-clock */
on tile[1]: buffered out port:32 p_bclk         = PORT_I2S_BCLK;     /* I2S L/R-clock */
on tile[1]: clock clk_audio_mclk                = XS1_CLKBLK_1;
on tile[1]: clock clk_audio_bclk                = XS1_CLKBLK_2;
on tile[1]: port p_mclk_in                      = PORT_MCLK_IN;


void aic3204_board_init();


int main() {
    chan c_aud;
    chan c_uart;

    par
    {
        on tile[0]:{
            delay_milliseconds(500);
            aic3204_board_init(); // Drives I2C lines to init codec

            /* Control chans */
            chan c_adc;

            /* XUA chans */
            chan c_ep_out[2];
            chan c_ep_in[3];            
            chan c_sof;
            chan c_aud_ctl;

            /* Declare enpoint tables */
            XUD_EpType epTypeTableOut[2] = {XUD_EPTYPE_CTL | XUD_STATUS_ENABLE, XUD_EPTYPE_ISO};
            XUD_EpType epTypeTableIn[3] = {XUD_EPTYPE_CTL | XUD_STATUS_ENABLE, XUD_EPTYPE_ISO, XUD_EPTYPE_ISO};

            /* Connect master-clock clock-block to clock-block pin */
            set_clock_src(usb_mclk_in_clk, p_for_mclk_in);           /* Clock clock-block from mclk pin */
            set_port_clock(p_for_mclk_count, usb_mclk_in_clk);       /* Clock the "count" port from the clock block */
            start_clock(usb_mclk_in_clk);                            /* Set the clock off running */

            /* Memory shared by dsp_task_0 and read by control_task */
            control_input_t control_input;

            par{
                XUD_Main(c_ep_out, 2, c_ep_in, 3,
                     c_sof, epTypeTableOut, epTypeTableIn, 
                     XUD_SPEED_HS, XUD_PWR_SELF);
                XUA_Endpoint0(c_ep_out[0], c_ep_in[0], c_aud_ctl, null, null, null, null);
                XUA_Buffer(c_ep_out[1], c_ep_in[2], c_ep_in[1], c_sof, c_aud_ctl, p_for_mclk_count, c_aud);

                adc_task(c_adc);
                unsafe{control_task(c_uart, c_adc, &control_input);}
            }
        }
        on tile[1]: {
            sw_pll_fixed_clock(MCLK_48);
            aic3204_codec_reset(); // Toggles XS1_PORT_4B for codec reset

            par{
                XUA_AudioHub(c_aud, clk_audio_mclk, clk_audio_bclk, p_mclk_in, p_lrclk, p_bclk, p_i2s_dac, p_i2s_adc);
                uart_task(c_uart);
            }
        }
    }
    return 0;
}
