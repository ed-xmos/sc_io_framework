cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)

project(sc_framework)

set(XMOS_SANDBOX_DIR    ${CMAKE_CURRENT_LIST_DIR}/..)

set(APP_HW_TARGET       xk-audio-316-mc.xn)

include(${CMAKE_CURRENT_LIST_DIR}/deps.cmake)

set(APP_COMPILER_FLAGS  -O2
                        -g 
                        -report
                        -DUART_TX_VALUE=0x4
                        -DUSB_TILE=tile[0]
                        )

# Workaround for now until cmake xcommon supports this
file(GLOB LIB_ADSP_STAGES   ../lib_audio_dsp/lib_audio_dsp/src/stages/*.c
                            ../lib_audio_dsp/lib_audio_dsp/src/dsp/*.c
                            ../lib_audio_dsp/lib_audio_dsp/src/dsp/*.S
                            )
list(TRANSFORM LIB_ADSP_STAGES PREPEND ../../../../..)

# Needed by this application and not part of library
set(APP_C_SRCS          ../lib_neopixel/lib_neopixel/src/neopixel.xc
                        ../lib_uart/lib_uart/src/uart_tx.xc
                        ../lib_gpio/lib_gpio/src/gpio.xc
                        ../sw_usb_audio/app_usb_aud_xk_316_mc/src/extensions/audiohw.xc
                        
                        src/dsp_wrapper.c
                        src/gpio_control_task.c
                        src/audio_dsp/app_dsp.c
                        src/audio_dsp/dspt_control.c
                        src/audio_dsp/dsp_pipeline/adsp_generated_auto.c

                        ${LIB_ADSP_STAGES}
                        )

set(APP_INCLUDES        ../sw_usb_audio/app_usb_aud_xk_316_mc/src
                        ../lib_neopixel/lib_neopixel/api
                        ../lib_uart/lib_uart/api
                        ../lib_uart/lib_uart/src
                        ../lib_gpio/lib_gpio/api

                        ../lib_audio_dsp/lib_audio_dsp/api                        
                        ../lib_audio_dsp/lib_audio_dsp/api/dsp
                        ../lib_audio_dsp/lib_audio_dsp/api/stages
                        src/
                        src/audio_dsp
                        src/audio_dsp/dsp_pipeline
                        src/audio_dsp/src.autogen/common
                        src/audio_dsp/src.autogen/device
                        
                        )

XMOS_REGISTER_APP()