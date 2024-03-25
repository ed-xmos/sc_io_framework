// Copyright 2023-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xassert.h>
#include "debug_print.h"
#include <xcore/channel.h>
#include <xcore/chanend.h>
#include <xcore/select.h>
#include "dspt_control.h"
#include "stages/adsp_control.h"
#include "xud_std_requests.h" // For USB_BMREQ_D2H_VENDOR_DEV and USB_BMREQ_H2D_VENDOR_DEV defines


#define SERVICER_COMMAND_RETRY (64) // From XVF3800 since reusing host app

#define MAX_CONTROL_PAYLOAD_LEN 256
void dsp_control_thread(chanend_t c_control, module_instance_t** modules, size_t num_modules)
{
    int8_t payload[MAX_CONTROL_PAYLOAD_LEN] = {0};
    uint8_t prev_write_cmd_status = 0;

    uint8_t temp = chan_in_byte(c_control);
    printf("dsp_control_thread received init token %d\n", temp);

    SELECT_RES(
        CASE_THEN(c_control, event_do_control)
    )
    {
        event_do_control: {
            control_req_t req;
            chan_in_buf_byte(c_control, (uint8_t*)&req, sizeof(control_req_t));
            if(req.cmd_id & 0x80) // Read command
            {
                adsp_stage_control_cmd_t cmd;
                cmd.instance_id = req.res_id;
                cmd.cmd_id = req.cmd_id & 0x7f;
                cmd.payload_len = req.payload_len - 1; // 1 extra byte for status
                cmd.payload = &payload[1]; // payload[0] reserved for read command status

                adsp_control_status_t ret = adsp_read_module_config(modules, num_modules, &cmd);
                if(ret == ADSP_CONTROL_BUSY)
                {
                    // Return RETRY as status
                    payload[0] = SERVICER_COMMAND_RETRY;
                    chan_out_buf_byte(c_control, (uint8_t*)payload, req.payload_len);
                }
                else if(ret == ADSP_CONTROL_SUCCESS)
                {
                    payload[0] = 0; // status success
                    chan_out_buf_byte(c_control, (uint8_t*)payload, req.payload_len);
                }
            }
            else // write command
            {
                if(req.direction == USB_BMREQ_D2H_VENDOR_DEV) // Read request for a write command. This is the host querying the status of the write
                {
                    payload[0] = prev_write_cmd_status;
                    chan_out_buf_byte(c_control, (const uint8_t*)payload, req.payload_len);
                }
                else    // Write request for a write command
                {
                    chan_in_buf_byte(c_control, (uint8_t*)payload, req.payload_len);

                    adsp_stage_control_cmd_t cmd;
                    cmd.instance_id = req.res_id;
                    cmd.cmd_id = req.cmd_id;
                    cmd.payload_len = req.payload_len;
                    cmd.payload = &payload[0];

                    adsp_control_status_t ret = adsp_write_module_config(modules, num_modules, &cmd);
                    prev_write_cmd_status = 0;
                    if(ret == ADSP_CONTROL_BUSY)
                    {
                        // Save the status to return to the host if it queries for the write command status
                        prev_write_cmd_status = SERVICER_COMMAND_RETRY;
                    }
                }

            }
        }
        continue;
    }
}
