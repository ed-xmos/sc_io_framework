cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)

project(sc_framework)

set(XMOS_SANDBOX_DIR    ${CMAKE_CURRENT_LIST_DIR}/..)

set(APP_HW_TARGET       XCORE-AI-EXPLORER.xn)

include(${CMAKE_CURRENT_LIST_DIR}/deps.cmake)

set(APP_COMPILER_FLAGS  -O2
                        -g 
                        -report
                        -DUART_TX_VALUE=0x4
                        )

# Needed by this application and not part of library
set(APP_C_SRCS          ../lib_neopixel/lib_neopixel/src/neopixel.xc
                        ../lib_uart/lib_uart/src/uart_tx.xc
                        ../lib_gpio/lib_gpio/src/gpio.xc
                        src/dac_setup/aic3204.xc
                        src/dac_setup/dac_port.xc
                        src/dsp_wrapper.c
                        )

set(APP_INCLUDES        src/
                        src/dac_setup
                        ../lib_neopixel/lib_neopixel/api
                        ../lib_uart/lib_uart/api
                        ../lib_uart/lib_uart/src
                        ../lib_gpio/lib_gpio/api
                        )

XMOS_REGISTER_APP()