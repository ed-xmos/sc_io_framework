// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "app_dsp.h"
#include "stdbool.h"
#include "xcore/chanend.h"
#include "xcore/parallel.h"
#include "dspt_control.h"

#include <stages/adsp_pipeline.h>
#include "adsp_generated_auto.h"

static adsp_pipeline_t * m_dsp;

// send data to dsp
void app_dsp_source(REFERENCE_PARAM(int32_t, data)) {
    int32_t* in_data[] = {
        &data[0],
        &data[1],
        &data[2],
        &data[3]
    };
    adsp_pipeline_source(m_dsp, in_data);
}

// read output
void app_dsp_sink(REFERENCE_PARAM(int32_t, data)) {
    int32_t* out_data[] = {
        &data[0],
        &data[1],
        &data[2],
        &data[3]
    };
    adsp_pipeline_sink(m_dsp, out_data);
}



// do dsp
void app_dsp_main(chanend_t c_control) {
    m_dsp = adsp_auto_pipeline_init();

    PAR_JOBS(
        PJOB(adsp_auto_pipeline_main, (m_dsp)),
        PJOB(dsp_control_thread, (c_control, m_dsp->modules, m_dsp->n_modules))
    );
}

