// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef CMDS_H
#define CMDS_H

// biquad module commands
#define CMD_BIQUAD_LEFT_SHIFT             1
#define CMD_BIQUAD_FILTER_COEFFS             2
#define CMD_BIQUAD_RESERVED             3
#define NUM_CMDS_BIQUAD    3    // Number of commands in the biquad module

// cascaded_biquads module commands
#define CMD_CASCADED_BIQUADS_LEFT_SHIFT             1
#define CMD_CASCADED_BIQUADS_FILTER_COEFFS             2
#define NUM_CMDS_CASCADED_BIQUADS    2    // Number of commands in the cascaded_biquads module

// compressor_rms module commands
#define CMD_COMPRESSOR_RMS_ATTACK_ALPHA             1
#define CMD_COMPRESSOR_RMS_RELEASE_ALPHA             2
#define CMD_COMPRESSOR_RMS_ENVELOPE             3
#define CMD_COMPRESSOR_RMS_THRESHOLD             4
#define CMD_COMPRESSOR_RMS_GAIN             5
#define CMD_COMPRESSOR_RMS_SLOPE             6
#define NUM_CMDS_COMPRESSOR_RMS    6    // Number of commands in the compressor_rms module

// dsp_thread module commands
#define CMD_DSP_THREAD_MAX_CYCLES             1
#define NUM_CMDS_DSP_THREAD    1    // Number of commands in the dsp_thread module

// envelope_detector_peak module commands
#define CMD_ENVELOPE_DETECTOR_PEAK_ATTACK_ALPHA             1
#define CMD_ENVELOPE_DETECTOR_PEAK_RELEASE_ALPHA             2
#define CMD_ENVELOPE_DETECTOR_PEAK_ENVELOPE             3
#define NUM_CMDS_ENVELOPE_DETECTOR_PEAK    3    // Number of commands in the envelope_detector_peak module

// envelope_detector_rms module commands
#define CMD_ENVELOPE_DETECTOR_RMS_ATTACK_ALPHA             1
#define CMD_ENVELOPE_DETECTOR_RMS_RELEASE_ALPHA             2
#define CMD_ENVELOPE_DETECTOR_RMS_ENVELOPE             3
#define NUM_CMDS_ENVELOPE_DETECTOR_RMS    3    // Number of commands in the envelope_detector_rms module

// fixed_gain module commands
#define CMD_FIXED_GAIN_GAIN             1
#define NUM_CMDS_FIXED_GAIN    1    // Number of commands in the fixed_gain module

// limiter_peak module commands
#define CMD_LIMITER_PEAK_ATTACK_ALPHA             1
#define CMD_LIMITER_PEAK_RELEASE_ALPHA             2
#define CMD_LIMITER_PEAK_ENVELOPE             3
#define CMD_LIMITER_PEAK_THRESHOLD             4
#define CMD_LIMITER_PEAK_GAIN             5
#define NUM_CMDS_LIMITER_PEAK    5    // Number of commands in the limiter_peak module

// limiter_rms module commands
#define CMD_LIMITER_RMS_ATTACK_ALPHA             1
#define CMD_LIMITER_RMS_RELEASE_ALPHA             2
#define CMD_LIMITER_RMS_ENVELOPE             3
#define CMD_LIMITER_RMS_THRESHOLD             4
#define CMD_LIMITER_RMS_GAIN             5
#define NUM_CMDS_LIMITER_RMS    5    // Number of commands in the limiter_rms module

// mixer module commands
#define CMD_MIXER_GAIN             1
#define NUM_CMDS_MIXER    1    // Number of commands in the mixer module

// noise_gate module commands
#define CMD_NOISE_GATE_ATTACK_ALPHA             1
#define CMD_NOISE_GATE_RELEASE_ALPHA             2
#define CMD_NOISE_GATE_ENVELOPE             3
#define CMD_NOISE_GATE_THRESHOLD             4
#define CMD_NOISE_GATE_GAIN             5
#define NUM_CMDS_NOISE_GATE    5    // Number of commands in the noise_gate module

// pipeline module commands
#define CMD_PIPELINE_CHECKSUM             1
#define NUM_CMDS_PIPELINE    1    // Number of commands in the pipeline module

// switch module commands
#define CMD_SWITCH_POSITION             1
#define NUM_CMDS_SWITCH    1    // Number of commands in the switch module

// volume_control module commands
#define CMD_VOLUME_CONTROL_TARGET_GAIN             1
#define CMD_VOLUME_CONTROL_GAIN             2
#define CMD_VOLUME_CONTROL_SLEW_SHIFT             3
#define CMD_VOLUME_CONTROL_MUTE             4
#define NUM_CMDS_VOLUME_CONTROL    4    // Number of commands in the volume_control module


#endif
