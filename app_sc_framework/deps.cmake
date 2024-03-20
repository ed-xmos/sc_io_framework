set(APP_DEPENDENT_MODULES   lib_i2c
                            lib_qadc
                            lib_logging
                            lib_xassert
                            "lib_xcore_math(xcommon_cmake)"
                            lib_xua
                            )

include(FetchContent)

if(NOT EXISTS ${XMOS_SANDBOX_DIR}/sw_usb_audio)
    FetchContent_Declare(
        sw_usb_audio
        GIT_REPOSITORY git@github.com:xmos/sw_usb_audio
        GIT_TAG develop
        SOURCE_DIR ${XMOS_SANDBOX_DIR}/sw_usb_audio
    )
    FetchContent_Populate(sw_usb_audio)
endif()

if(NOT EXISTS ${XMOS_SANDBOX_DIR}/lib_uart)
    FetchContent_Declare(
        lib_uart
        GIT_REPOSITORY git@github.com:xmos/lib_uart
        GIT_TAG develop
        SOURCE_DIR ${XMOS_SANDBOX_DIR}/lib_uart
    )
    FetchContent_Populate(lib_uart)
endif()

if(NOT EXISTS ${XMOS_SANDBOX_DIR}/lib_gpio)
    FetchContent_Declare(
        lib_gpio
        GIT_REPOSITORY git@github.com:xmos/lib_gpio
        GIT_TAG develop
        SOURCE_DIR ${XMOS_SANDBOX_DIR}/lib_gpio
    )
    FetchContent_Populate(lib_gpio)
endif()

if(NOT EXISTS ${XMOS_SANDBOX_DIR}/lib_neopixel)
    FetchContent_Declare(
        lib_neopixel
        GIT_REPOSITORY git@github.com:xmos/lib_neopixel
        GIT_TAG develop
        SOURCE_DIR ${XMOS_SANDBOX_DIR}/lib_neopixel
    )
    FetchContent_Populate(lib_neopixel)
endif()
