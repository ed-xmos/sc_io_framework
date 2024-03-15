// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef _XUA_APP_MAIN_H_ 
#define _XUA_APP_MAIN_H_

#include "xs3a_user.h"
#include <stdint.h>
#include "app_config.h"


// Macro to adjust input pad timing for the round trip delay. Supports 0 (default) to 5 core clock cycles.
// Larger numbers increase hold time but reduce setup time.
#define PORT_DELAY      0x7007
#define DELAY_SHIFT     0x3
// #define set_pad_delay(port, delay)  {__asm__ __volatile__ ("setc res[%0], %1": : "r" (port) , "r" ((delay << 0x3) | PORT_DELAY));}

// Macro to adjust input pad capture clock edge
#define PORT_SAMPLE     0x4007
#define SAMPLE_SHIFT    0x3
#define SAMPLE_RISING   0x0 // Default for input port. Reduces setup time, increases hold time.
#define SAMPLE_FALLING  0x1 // Increases setup time, reduces hold time
#define set_pad_sample_edge(port, edge)  {__asm__ __volatile__ ("setc res[%0], %1": : "r" (port) , "r" ((edge << 0x3) | PORT_SAMPLE));}


// Pad control defines
#define PULL_NONE       0x0
#define PULL_UP_WEAK    0x1 // BUG doesn't work. Can half set by using set_pad_properties
#define PULL_DOWN_WEAK  0x2 // BUG 
#define BUS_KEEP_WEAK   0x3 // BUG 
#define PULL_SHIFT      18
#define DRIVE_2MA       0x0
#define DRIVE_4MA       0x1
#define DRIVE_8MA       0x2
#define DRIVE_12MA      0x3
#define DRIVE_SHIFT     20
#define SLEW_SHIFT      22
#define SCHMITT_SHIFT   23
#define RECEIVER_EN_SHIFT 17 // Set this to enable the IO reveiver

#define PAD_MAKE_WORD(port, drive_strength, pull_config, slew, schmitt) ((drive_strength << DRIVE_SHIFT) | \
                                                                        (pull_config << PULL_SHIFT) | \
                                                                        ((slew ? 1 : 0) << SLEW_SHIFT) | \
                                                                        ((schmitt ? 1 : 0) << SCHMITT_SHIFT) | \
                                                                        (1 << RECEIVER_EN_SHIFT) | \
                                                                        XS1_SETC_MODE_SETPADCTRL) 

// Macro to setup the port drive characteristics
#define set_pad_properties(port, drive_strength, pull_config, slew, schmitt)  {__asm__ __volatile__ ("setc res[%0], %1": : "r" (port) , "r" PAD_MAKE_WORD(port, drive_strength, pull_config, slew, schmitt));}

// Drive control defines
#define DRIVE_BOTH                  0x0 // Default
#define DRIVE_HIGH_WEAK_PULL_DOWN   0x1 // Open source w/pulldown
#define DRIVE_LOW_WEAK_PULL_UP      0x2 // Open drain w/pullup
#define DRIVE_MODE_SHIFT            0x3

#define set_pad_drive_mode(port, drive_mode)  {__asm__ __volatile__ ("setc res[%0], %1": : "r" (port) , "r" ((drive_mode << DRIVE_MODE_SHIFT) | \
                                                                                                            XS1_SETC_DRIVE_DRIVE)) ;}


// Macros to convert between two bytes and U16
#define LOWER_BYTE_FROM_U16(u16)                ((uint8_t)(u16 & 0xff))
#define UPPER_BYTE_FROM_U16(u16)                ((uint8_t)(u16 >> 8))
#define U16_FROM_BYTES(upper_byte, lower_byte)  (((uint16_t)upper_byte << 8) | (uint16_t)lower_byte)

#endif