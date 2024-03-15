// Copyright (c) 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

#include <platform.h>
#include <assert.h>

#include "app_config.h"
#include "i2c.h"
#include "aic3204.h"

// i2c_master_t i2c_master_ctx;
port codec_rst_port = PORT_CODEC_RST_N;
port p_scl = PORT_I2C_SCL;
port p_sda = PORT_I2C_SDA;

void aic3204_board_init(void)
{
  int res = 0;
  res = aic3204_init();
  assert( res == 0 );
}

int aic3204_reg_write(uint8_t reg, uint8_t val)
{
    i2c_regop_res_t ret;
    i2c_master_if i2c[1];

    par{
        i2c_master(i2c, 1, p_scl, p_sda, 10);
        {
            ret = i2c[0].write_reg(AIC3204_I2C_DEVICE_ADDR, reg, val);
            i2c[0].shutdown();
        }
    }

    if (ret == I2C_REGOP_SUCCESS) {
        return 0;
    } else {
        return -1;
    }
}

void aic3204_codec_reset(void)
{
    codec_rst_port <: 0xF;
}

void aic3204_wait(uint32_t wait_ms)
{
    delay_milliseconds(wait_ms);
}
