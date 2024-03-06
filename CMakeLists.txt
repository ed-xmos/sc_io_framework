cmake_minimum_required(VERSION 3.21)

## Project declaration
project(sc_framework)

## Enable languages for project
enable_language(CXX C ASM)

## Add frameworks
add_subdirectory(modules)

## Add top level project targets
if(PROJECT_IS_TOP_LEVEL)
    include(app_sc_framework/app_sc_framework.cmake)
endif()
