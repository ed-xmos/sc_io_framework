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

# Needed by this application and not part of library
set(APP_C_SRCS          ../lib_neopixel/lib_neopixel/src/neopixel.xc
                        ../lib_uart/lib_uart/src/uart_tx.xc
                        ../lib_gpio/lib_gpio/src/gpio.xc
                        ../lib_gpio/lib_gpio/src/gpio.xc
                        ../sw_usb_audio/app_usb_aud_xk_316_mc/src/extensions/audiohw.xc
                        src/dsp_wrapper.c
                        )

set(APP_INCLUDES        src/
                        ../sw_usb_audio/app_usb_aud_xk_316_mc/src
                        ../lib_neopixel/lib_neopixel/api
                        ../lib_uart/lib_uart/api
                        ../lib_uart/lib_uart/src
                        ../lib_gpio/lib_gpio/api
                        )

XMOS_REGISTER_APP()