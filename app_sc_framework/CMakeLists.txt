cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)

project(sc_framework)

set(XMOS_SANDBOX_DIR    ${CMAKE_CURRENT_LIST_DIR}/..)

set(APP_HW_TARGET       XCORE-AI-EXPLORER.xn)

include(${CMAKE_CURRENT_LIST_DIR}/deps.cmake)

set(APP_COMPILER_FLAGS  -O3 
                        -g 
                        -report
                        )


XMOS_REGISTER_APP()