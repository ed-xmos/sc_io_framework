// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#ifndef __XC__
#include <xcore/parallel.h>
#include <xcore/channel.h>
#include <xcore/port.h>
#endif

// ADC channels count and port declaraions
#define ADC_PINS                    {XS1_PORT_1A, XS1_PORT_1D}  // Sets which pins are to be used (channels 0..n) and defines channel count.  // X0D00, 11
#define ADC_NUM_CHANNELS            1

// ADC operation

#define ADC_READ_INTERVAL       (5 * XS1_TIMER_KHZ)    // Time in between individual conversions 1ms with 10nf / 10k is practical minimum
// #define ADC_BITS_TARGET         10                     // Resolution for conversion. 9/10 bits is practical maximum for above config to allow charge/discharge RC time 
#define NUM_CAL 1024 // Max 4096
#define RESULT_HISTORY_DEPTH    32                     // For filtering raw conversion values. Tradeoff between conversion speed and noise
#define RESULT_HYSTERESIS       2                      // Reduce final output noise. Applies a small "dead zone" to current setting

// Communication protocol
#define ADC_CMD_READ                0x01000000         
#define ADC_CMD_CAL_MODE_START      0x02000000
#define ADC_CMD_CAL_MODE_FINISH     0x03000000
#define ADC_CMD_MASK                0xff000000

#ifdef __XC__
void adc_task(chanend c_adc);
#else
DECLARE_JOB(adc_task, (chanend_t));
void adc_task(chanend_t c_adc);
#endif
