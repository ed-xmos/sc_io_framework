// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include "app_main.h"
#include "xua_conf.h"
#include "dsp_wrapper.h"

// void dsp_tile_0_exchange(chanend_t c_dsp_0, int32_t *samples, size_t n_ch){
//     transacting_chanend_t tc = chan_init_transaction_master(c_dsp_0);
//     t_chan_out_word(&tc, n_ch);
//     t_chan_out_buf_word(&tc, (uint32_t*)samples, n_ch);
//     t_chan_in_buf_word(&tc, (uint32_t*)samples, n_ch);
//     c_dsp_0 = chan_complete_transaction(tc);
// }

typedef struct vu_state_t{
    int32_t vu;
}vu_state_t;

#define ABS(x) ((x)<0 ? (-x) : (x))

// Quick and dirty VU
void process_vu(int32_t *samples, size_t n_ch, vu_state_t vu_state[NUM_USB_CHAN_OUT]){
    for(int ch = 0; ch < n_ch; ch++){
        // Apply decay
        const int32_t decay_constant = (int32_t)((float)((1<<31) - 1) * 0.9995);
        vu_state[ch].vu = ((int64_t)decay_constant * (int64_t)vu_state[ch].vu) >> 31;
        int32_t abs_samp = ABS(samples[ch]);
        if(abs_samp > vu_state[ch].vu){
            vu_state[ch].vu = abs_samp;
        }
    }
}

void dsp_task_0(chanend_t c_dsp_0, control_input_t *control_input){
    printstr("dsp_task_0\n");

    vu_state_t vu_state[NUM_USB_CHAN_OUT] = {{0}};

    int32_t samples[NUM_USB_CHAN_OUT];
    while(1){
        transacting_chanend_t tc = chan_init_transaction_slave(c_dsp_0);
        size_t n_ch = t_chan_in_word(&tc);
        t_chan_in_buf_word(&tc, (uint32_t*)samples, n_ch);
        t_chan_out_buf_word(&tc, (uint32_t*)samples, n_ch);
        c_dsp_0 = chan_complete_transaction(tc);

        process_vu(samples, n_ch, vu_state);
        control_input->vu[0] = vu_state[0].vu;
        control_input->vu[1] = vu_state[1].vu;
    }
}

void dsp_task_1(chanend_t c_dsp_1){
    printstr("dsp_task_1\n");
}
