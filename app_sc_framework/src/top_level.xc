// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <platform.h>
#include <xs1.h>

#include "app_config.h"
#include "adc_pot.h"
#include "control_task.h"
#include "dsp_wrapper.h"
#include "xua_conf.h"
extern "C"{
    #include "sw_pll.h"
}
#include "i2c.h"
#include "xua_conf.h"
#include "xua.h"
#include "xud.h"

// Audio resources for USB and I2S
on tile[0]: port p_for_mclk_count = XS1_PORT_16B;
on tile[0]: port p_for_mclk_in = XS1_PORT_1D;
on tile[0]: clock usb_mclk_in_clk = XS1_CLKBLK_2;

on tile[1]: buffered out port:32 p_i2s_dac[]    = {PORT_I2S_DAC0};   /* I2S Data-line(s) */
on tile[1]: buffered in port:32 p_i2s_adc[]     = {PORT_I2S_ADC0};   /* I2S Data-line(s) */
on tile[1]: buffered out port:32 p_lrclk        = PORT_I2S_LRCLK;    /* I2S Bit-clock */
on tile[1]: buffered out port:32 p_bclk         = PORT_I2S_BCLK;     /* I2S L/R-clock */
on tile[1]: clock clk_audio_mclk                = XS1_CLKBLK_1;
on tile[1]: clock clk_audio_bclk                = XS1_CLKBLK_2;
on tile[1]: port p_mclk_in                      = PORT_MCLK_IN;

// I2C resources (defined in audiohw.c)
extern port p_scl;
extern port p_sda;

// GPIO resources
on tile[0]: in port p_mc_buttons                = XS1_PORT_4E;      // 3 buttons on MC board
on tile[0]: out port p_mc_leds                  = XS1_PORT_4F;      // 4 LEDs on MC board
on tile[1]: out buffered port:32 p_neopixel     = PORT_SPDIF_OUT;
on tile[1]: clock cb_neo                        = XS1_CLKBLK_3;
on tile[1]: port p_uart_tx                      = PORT_MIDI_OUT; // Bit 0
on tile[1]: port p_adc[]                        = {PORT_I2S_ADC2, PORT_I2S_ADC3}; // Sets which pins are to be used (channels 0..n)


int main() {
    chan c_aud;
    chan c_dsp;
    interface i2c_master_if i2c[1];
    input_gpio_if i_gpio_mc_buttons[1];
    output_gpio_if i_gpio_mc_leds[1];

    par
    {
        on tile[0]:{
            board_setup();

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



            /* I2C setup */


            par{
                XUD_Main(c_ep_out, 2, c_ep_in, 3,
                     c_sof, epTypeTableOut, epTypeTableIn, 
                     XUD_SPEED_HS, XUD_PWR_SELF);
                XUA_Endpoint0(c_ep_out[0], c_ep_in[0], c_aud_ctl, null, null, null, null);
                XUA_Buffer(c_ep_out[1], c_ep_in[2], c_ep_in[1], c_sof, c_aud_ctl, p_for_mclk_count, c_aud);

                dsp_task_0(c_dsp);

                i2c_master(i2c, 1, p_scl, p_sda, 100);
                output_gpio(i_gpio_mc_leds, 1, p_mc_leds, null);
                input_gpio(i_gpio_mc_buttons, 1, p_mc_buttons, null);
            }
        }
        on tile[1]: unsafe{            
            /* Control chans */
            chan c_adc;
            interface uart_tx_if i_uart_tx;

            unsafe{ i_i2c_client = i2c[0];}
            AudioHwInit();

            /* Quasi-ADC setup parameters */
            const unsigned capacitor_pf = 8800;
            const unsigned resistor_ohms = 10000; // nominal maximum value ned to end
            const unsigned resistor_series_ohms = 220;
            const float v_rail = 3.3;
            const float v_thresh = 1.14;
            const adc_pot_config_t adc_config = {capacitor_pf, resistor_ohms, resistor_series_ohms, v_rail, v_thresh};

            /* Memory shared by dsp_task_1 and read by control_task */
            control_input_t control_input;
            control_input_t * unsafe control_input_ptr = &control_input;

            par{
                XUA_AudioHub(c_aud, clk_audio_mclk, clk_audio_bclk, p_mclk_in, p_lrclk, p_bclk, p_i2s_dac, p_i2s_adc);
                dsp_task_1(c_dsp, control_input_ptr);
                adc_pot_task(c_adc, p_adc, 1, adc_config);
                control_task(i_uart_tx,
                            c_adc, control_input_ptr, 
                            p_neopixel, cb_neo,
                            i_gpio_mc_buttons[0],
                            i_gpio_mc_leds[0]);
                uart_task(i_uart_tx, p_uart_tx);
            }
        }
    }
    return 0;
}
