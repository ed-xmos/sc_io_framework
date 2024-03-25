// Copyright 2023-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/channel.h>
#include "xua.h"
#include "xud.h"
#include "vendorrequests.h"
#include "xassert.h"
#include "debug_print.h"
#include "dspt_control.h"

#define EP0_MAX_REQUEST_SIZE 256 // max allowed USB recv size
#define EP0_MAX_REQUEST_BUF_SIZE EP0_MAX_REQUEST_SIZE+2 // add 2 bytes for the CRC

void VendorRequests_Init(VENDOR_REQUESTS_PARAMS_DEC)
{
    chanend_t c = c_control;
    chan_out_byte(c, 88);
}


int VendorRequests(XUD_ep ep0_out, XUD_ep ep0_in, USB_SetupPacket_t *sp VENDOR_REQUESTS_PARAMS_DEC_)
{
    chanend_t c = c_control;
    XUD_Result_t result = XUD_RES_ERR;

    unsigned char request_data[EP0_MAX_REQUEST_BUF_SIZE] = {0};
    size_t len = 0;

    switch ((sp->bmRequestType.Direction << 7) | (sp->bmRequestType.Type << 5) | (sp->bmRequestType.Recipient)) {
        case USB_BMREQ_H2D_VENDOR_DEV:
            if (sp->wLength <= EP0_MAX_REQUEST_SIZE) {
                control_req_t req;
                req.direction = USB_BMREQ_H2D_VENDOR_DEV;
                req.res_id = sp->wIndex;
                req.cmd_id = sp->wValue;
                req.payload_len = sp->wLength;
                chan_out_buf_byte(c, (uint8_t*)&req, sizeof(control_req_t));

                result = XUD_RES_OKAY;
                while (result == XUD_RES_OKAY && len < sp->wLength) {
                    unsigned packet_len;
                    result = XUD_GetBuffer(ep0_out, request_data + len, &packet_len);

                    len += packet_len;
                    debug_printf("received %d bytes. total so far %d out of %d\n", packet_len, len, sp->wLength);
                }
                chan_out_buf_byte(c, request_data, req.payload_len);
            } else {
                result = XUD_RES_ERR;
                debug_printf("usb receive size of %d bytes exceeds %d\n", sp->wLength, EP0_MAX_REQUEST_SIZE);
            }

            if (result == XUD_RES_OKAY) {
                result = XUD_DoSetRequestStatus(ep0_in);
            }
            break;

        case USB_BMREQ_D2H_VENDOR_DEV:
            /* application retrieval latency inside the control library call
             * XUD task defers further calls by NAKing USB transactions
             */
            len = sp->wLength;
            control_req_t req;
            req.direction = USB_BMREQ_D2H_VENDOR_DEV;
            req.res_id = sp->wIndex;
            req.cmd_id = sp->wValue;
            req.payload_len = sp->wLength;
            chan_out_buf_byte(c, (uint8_t*)&req, sizeof(control_req_t));

            chan_in_buf_byte(c, request_data, req.payload_len);

            result = XUD_DoGetRequest(ep0_out, ep0_in, request_data, len, len);
            /* on control error, go to standard requests, which will issue STALL */
            break;
    }
    return result;
}
