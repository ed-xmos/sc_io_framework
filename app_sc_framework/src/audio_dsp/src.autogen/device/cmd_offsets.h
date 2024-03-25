// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef CMD_OFFSETS_H
#define CMD_OFFSETS_H

#include "stages/adsp_module.h"

#include "cmds.h"
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

typedef struct {
    uint32_t cmd_id; // CmdID
    uint32_t offset;    // offset
    uint32_t size;      //size
}module_config_offsets_t;

// Offset and size of fields in the biquad_config_t structure
static module_config_offsets_t biquad_config_offsets[] = {
{.cmd_id=CMD_BIQUAD_LEFT_SHIFT, .offset=offsetof(biquad_config_t, left_shift), .size=sizeof(int) * 1},
{.cmd_id=CMD_BIQUAD_FILTER_COEFFS, .offset=offsetof(biquad_config_t, filter_coeffs), .size=sizeof(int32_t) * 5},
{.cmd_id=CMD_BIQUAD_RESERVED, .offset=offsetof(biquad_config_t, reserved), .size=sizeof(int32_t) * 3},
};
// Offset and size of fields in the cascaded_biquads_config_t structure
static module_config_offsets_t cascaded_biquads_config_offsets[] = {
{.cmd_id=CMD_CASCADED_BIQUADS_LEFT_SHIFT, .offset=offsetof(cascaded_biquads_config_t, left_shift), .size=sizeof(int) * 8},
{.cmd_id=CMD_CASCADED_BIQUADS_FILTER_COEFFS, .offset=offsetof(cascaded_biquads_config_t, filter_coeffs), .size=sizeof(int32_t) * 40},
};
// Offset and size of fields in the compressor_rms_config_t structure
static module_config_offsets_t compressor_rms_config_offsets[] = {
{.cmd_id=CMD_COMPRESSOR_RMS_ATTACK_ALPHA, .offset=offsetof(compressor_rms_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_COMPRESSOR_RMS_RELEASE_ALPHA, .offset=offsetof(compressor_rms_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_COMPRESSOR_RMS_ENVELOPE, .offset=offsetof(compressor_rms_config_t, envelope), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_COMPRESSOR_RMS_THRESHOLD, .offset=offsetof(compressor_rms_config_t, threshold), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_COMPRESSOR_RMS_GAIN, .offset=offsetof(compressor_rms_config_t, gain), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_COMPRESSOR_RMS_SLOPE, .offset=offsetof(compressor_rms_config_t, slope), .size=sizeof(float) * 1},
};
// Offset and size of fields in the dsp_thread_config_t structure
static module_config_offsets_t dsp_thread_config_offsets[] = {
{.cmd_id=CMD_DSP_THREAD_MAX_CYCLES, .offset=offsetof(dsp_thread_config_t, max_cycles), .size=sizeof(uint32_t) * 1},
};
// Offset and size of fields in the envelope_detector_peak_config_t structure
static module_config_offsets_t envelope_detector_peak_config_offsets[] = {
{.cmd_id=CMD_ENVELOPE_DETECTOR_PEAK_ATTACK_ALPHA, .offset=offsetof(envelope_detector_peak_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_ENVELOPE_DETECTOR_PEAK_RELEASE_ALPHA, .offset=offsetof(envelope_detector_peak_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_ENVELOPE_DETECTOR_PEAK_ENVELOPE, .offset=offsetof(envelope_detector_peak_config_t, envelope), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the envelope_detector_rms_config_t structure
static module_config_offsets_t envelope_detector_rms_config_offsets[] = {
{.cmd_id=CMD_ENVELOPE_DETECTOR_RMS_ATTACK_ALPHA, .offset=offsetof(envelope_detector_rms_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_ENVELOPE_DETECTOR_RMS_RELEASE_ALPHA, .offset=offsetof(envelope_detector_rms_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_ENVELOPE_DETECTOR_RMS_ENVELOPE, .offset=offsetof(envelope_detector_rms_config_t, envelope), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the fixed_gain_config_t structure
static module_config_offsets_t fixed_gain_config_offsets[] = {
{.cmd_id=CMD_FIXED_GAIN_GAIN, .offset=offsetof(fixed_gain_config_t, gain), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the limiter_peak_config_t structure
static module_config_offsets_t limiter_peak_config_offsets[] = {
{.cmd_id=CMD_LIMITER_PEAK_ATTACK_ALPHA, .offset=offsetof(limiter_peak_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_PEAK_RELEASE_ALPHA, .offset=offsetof(limiter_peak_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_PEAK_ENVELOPE, .offset=offsetof(limiter_peak_config_t, envelope), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_PEAK_THRESHOLD, .offset=offsetof(limiter_peak_config_t, threshold), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_PEAK_GAIN, .offset=offsetof(limiter_peak_config_t, gain), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the limiter_rms_config_t structure
static module_config_offsets_t limiter_rms_config_offsets[] = {
{.cmd_id=CMD_LIMITER_RMS_ATTACK_ALPHA, .offset=offsetof(limiter_rms_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_RMS_RELEASE_ALPHA, .offset=offsetof(limiter_rms_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_RMS_ENVELOPE, .offset=offsetof(limiter_rms_config_t, envelope), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_RMS_THRESHOLD, .offset=offsetof(limiter_rms_config_t, threshold), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_LIMITER_RMS_GAIN, .offset=offsetof(limiter_rms_config_t, gain), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the mixer_config_t structure
static module_config_offsets_t mixer_config_offsets[] = {
{.cmd_id=CMD_MIXER_GAIN, .offset=offsetof(mixer_config_t, gain), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the noise_gate_config_t structure
static module_config_offsets_t noise_gate_config_offsets[] = {
{.cmd_id=CMD_NOISE_GATE_ATTACK_ALPHA, .offset=offsetof(noise_gate_config_t, attack_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_NOISE_GATE_RELEASE_ALPHA, .offset=offsetof(noise_gate_config_t, release_alpha), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_NOISE_GATE_ENVELOPE, .offset=offsetof(noise_gate_config_t, envelope), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_NOISE_GATE_THRESHOLD, .offset=offsetof(noise_gate_config_t, threshold), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_NOISE_GATE_GAIN, .offset=offsetof(noise_gate_config_t, gain), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the pipeline_config_t structure
static module_config_offsets_t pipeline_config_offsets[] = {
{.cmd_id=CMD_PIPELINE_CHECKSUM, .offset=offsetof(pipeline_config_t, checksum), .size=sizeof(uint8_t) * 16},
};
// Offset and size of fields in the switch_config_t structure
static module_config_offsets_t switch_config_offsets[] = {
{.cmd_id=CMD_SWITCH_POSITION, .offset=offsetof(switch_config_t, position), .size=sizeof(int32_t) * 1},
};
// Offset and size of fields in the volume_control_config_t structure
static module_config_offsets_t volume_control_config_offsets[] = {
{.cmd_id=CMD_VOLUME_CONTROL_TARGET_GAIN, .offset=offsetof(volume_control_config_t, target_gain), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_VOLUME_CONTROL_GAIN, .offset=offsetof(volume_control_config_t, gain), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_VOLUME_CONTROL_SLEW_SHIFT, .offset=offsetof(volume_control_config_t, slew_shift), .size=sizeof(int32_t) * 1},
{.cmd_id=CMD_VOLUME_CONTROL_MUTE, .offset=offsetof(volume_control_config_t, mute), .size=sizeof(uint8_t) * 1},
};

static module_config_offsets_t *ptr_module_offsets[] = {
    biquad_config_offsets,
    cascaded_biquads_config_offsets,
    compressor_rms_config_offsets,
    dsp_thread_config_offsets,
    envelope_detector_peak_config_offsets,
    envelope_detector_rms_config_offsets,
    fixed_gain_config_offsets,
    limiter_peak_config_offsets,
    limiter_rms_config_offsets,
    mixer_config_offsets,
    noise_gate_config_offsets,
    pipeline_config_offsets,
    switch_config_offsets,
    volume_control_config_offsets,
};

typedef enum
{
    e_dsp_stage_biquad,
    e_dsp_stage_cascaded_biquads,
    e_dsp_stage_compressor_rms,
    e_dsp_stage_dsp_thread,
    e_dsp_stage_envelope_detector_peak,
    e_dsp_stage_envelope_detector_rms,
    e_dsp_stage_fixed_gain,
    e_dsp_stage_limiter_peak,
    e_dsp_stage_limiter_rms,
    e_dsp_stage_mixer,
    e_dsp_stage_noise_gate,
    e_dsp_stage_pipeline,
    e_dsp_stage_switch,
    e_dsp_stage_volume_control,
    num_dsp_stages
}all_dsp_stages_t;

#endif
