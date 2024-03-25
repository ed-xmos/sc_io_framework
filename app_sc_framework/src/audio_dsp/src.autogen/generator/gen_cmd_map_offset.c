// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>

#include "biquad_config.h"
#include "cascaded_biquads_config.h"
#include "compressor_rms_config.h"
#include "dsp_thread_config.h"
#include "envelope_detector_peak_config.h"
#include "envelope_detector_rms_config.h"
#include "fixed_gain_config.h"
#include "limiter_peak_config.h"
#include "limiter_rms_config.h"
#include "mixer_config.h"
#include "noise_gate_config.h"
#include "pipeline_config.h"
#include "switch_config.h"
#include "volume_control_config.h"

#if OFFSET_GEN
int main(int argc, char* argv[])
{
    FILE *fp;
    printf("argc = %d\n", argc);
    if(argc == 2)
    {
        printf("argv = %s\n", argv[1]);
        fp = fopen(argv[1], "w");
    }
    else
    {
        fp = fopen("cmd_offset_size.h", "w");
    }
    fprintf(fp, "#ifndef CMD_NUM_VALUES_H\n");
    fprintf(fp, "#define CMD_NUM_VALUES_H\n\n");

    // Number of values read or written as part of the command. Used in the host cmd_map.c file. Needs to be generated by running
    // an application since the config yaml files might specified size in terms of #defines (for eg. FILTERS * NUM_COEFFS_PER_BIQUAD)
    fprintf(fp, "#define NUM_VALUES_BIQUAD_LEFT_SHIFT %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_BIQUAD_FILTER_COEFFS %u\n", 5);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_BIQUAD_RESERVED %u\n", 3);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_CASCADED_BIQUADS_LEFT_SHIFT %u\n", 8);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_CASCADED_BIQUADS_FILTER_COEFFS %u\n", 40);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_THRESHOLD %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_COMPRESSOR_RMS_SLOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_DSP_THREAD_MAX_CYCLES %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_PEAK_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_PEAK_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_PEAK_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_RMS_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_RMS_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_ENVELOPE_DETECTOR_RMS_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_FIXED_GAIN_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_PEAK_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_PEAK_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_PEAK_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_PEAK_THRESHOLD %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_PEAK_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_RMS_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_RMS_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_RMS_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_RMS_THRESHOLD %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_LIMITER_RMS_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_MIXER_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_NOISE_GATE_ATTACK_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_NOISE_GATE_RELEASE_ALPHA %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_NOISE_GATE_ENVELOPE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_NOISE_GATE_THRESHOLD %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_NOISE_GATE_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_PIPELINE_CHECKSUM %u\n", 16);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_SWITCH_POSITION %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_VOLUME_CONTROL_TARGET_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_VOLUME_CONTROL_GAIN %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_VOLUME_CONTROL_SLEW_SHIFT %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#define NUM_VALUES_VOLUME_CONTROL_MUTE %u\n", 1);
    fprintf(fp, "\n");
    fprintf(fp, "#endif\n");
    fclose(fp);
    return 0;
}
#endif