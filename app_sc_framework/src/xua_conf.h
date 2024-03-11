// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

// This file contains defines relating to the USB build only

#ifndef _XUA_CONF_H_ 
#define _XUA_CONF_H_

#define NUM_USB_CHAN_OUT 2
#define NUM_USB_CHAN_IN 2
#define I2S_CHANS_DAC 2
#define I2S_CHANS_ADC 2
#define MCLK_441 (256 * 44100)
#define MCLK_48 (256 * 48000)
#define MIN_FREQ 48000
#define MAX_FREQ 48000

#define EXCLUDE_USB_AUDIO_MAIN

#define VENDOR_STR "XMOS"
#define VENDOR_ID 0x20B1
#define PRODUCT_STR_A2 "SC Framework"
#define PRODUCT_STR_A1 "SC Framework"
#define PID_AUDIO_1 0x4003
#define PID_AUDIO_2 0x4004
#define AUDIO_CLASS 2
#define UAC_FORCE_FEEDBACK_EP 1
#define AUDIO_CLASS_FALLBACK 0
#define BCD_DEVICE 0x1234
#define XUA_DFU_EN 0
#define CODEC_MASTER 0
#define INPUT_VOLUME_CONTROL 1

#endif