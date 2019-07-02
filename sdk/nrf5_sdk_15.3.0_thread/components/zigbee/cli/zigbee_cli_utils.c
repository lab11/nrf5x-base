/**
 * Copyright (c) 2018 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "nrf_cli.h"
#include "zboss_api.h"
#include "zb_error_handler.h"
#include "zigbee_logger_eprxzcl.h"
#include "zigbee_cli_utils.h"
#include <ctype.h>

NRF_SECTION_DEF(zb_ep_handlers, zb_device_handler_t);
#define ZB_EP_HANDLER_SECTION_ITEM_GET(i) NRF_SECTION_ITEM_GET(zb_ep_handlers, zb_device_handler_t, (i))
#define ZB_EP_HANDLER_SECTION_ITEM_COUNT  NRF_SECTION_ITEM_COUNT(zb_ep_handlers, zb_device_handler_t)

zb_uint8_t cli_agent_ep_handler(zb_uint8_t param)
{
    unsigned int idx;

#if defined(DEBUG_NRF) && NRF_LOG_ENABLED
    UNUSED_RETURN_VALUE(zigbee_logger_eprxzcl_ep_handler(param));
#endif

    for (idx = 0; idx < ZB_EP_HANDLER_SECTION_ITEM_COUNT; idx++)
    {
        zb_device_handler_t handler = *(ZB_EP_HANDLER_SECTION_ITEM_GET(idx));
        if (handler(param) == ZB_TRUE)
        {
            return ZB_TRUE;
        }
    }

    return ZB_FALSE;
}

zb_void_t frame_acked_cb(zb_uint8_t param)
{
    if (param)
    {
        ZB_FREE_BUF_BY_REF(param);
    }
}

int zcl_attr_to_str(char * p_str_buf, uint16_t buf_len, zb_uint16_t attr_type, zb_uint8_t * p_attr)
{
    int bytes_written = 0;
    int string_len;
    int i;

    if ((p_str_buf == NULL) || (p_attr == NULL))
    {
        return -1;
    }

    switch (attr_type)
    {
        /* Boolean */
        case ZB_ZCL_ATTR_TYPE_BOOL:
            bytes_written = snprintf(p_str_buf, buf_len, "%s", *((zb_bool_t *)p_attr) ? "True" : "False");
            break;

        /* 1 byte */
        case ZB_ZCL_ATTR_TYPE_8BIT:
        case ZB_ZCL_ATTR_TYPE_8BITMAP:
        case ZB_ZCL_ATTR_TYPE_U8:
        case ZB_ZCL_ATTR_TYPE_8BIT_ENUM:
            bytes_written = snprintf(p_str_buf, buf_len, "%hu", *((zb_uint8_t*)p_attr));
            break;

        case ZB_ZCL_ATTR_TYPE_S8:
            bytes_written = snprintf(p_str_buf, buf_len, "%hd", *((zb_int8_t*)p_attr));
            break;

        /* 2 bytes */
        case ZB_ZCL_ATTR_TYPE_16BIT:
        case ZB_ZCL_ATTR_TYPE_16BITMAP:
        case ZB_ZCL_ATTR_TYPE_U16:
        case ZB_ZCL_ATTR_TYPE_16BIT_ENUM:
            bytes_written = snprintf(p_str_buf, buf_len, "%hu", *((zb_uint16_t*)p_attr));
            break;

        case ZB_ZCL_ATTR_TYPE_S16:
            bytes_written = snprintf(p_str_buf, buf_len, "%hd", *((zb_int16_t*)p_attr));
            break;

        /* 4 bytes */
        case ZB_ZCL_ATTR_TYPE_32BIT:
        case ZB_ZCL_ATTR_TYPE_32BITMAP:
        case ZB_ZCL_ATTR_TYPE_U32:
            bytes_written = snprintf(p_str_buf, buf_len, "%u", *((zb_uint32_t*)p_attr));
            break;

        case ZB_ZCL_ATTR_TYPE_S32:
            bytes_written = snprintf(p_str_buf, buf_len, "%d", *((zb_int32_t*)p_attr));
            break;

        /* String */
        case ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            string_len = p_attr[0];
            p_attr++;

            if ((buf_len - bytes_written) < (string_len + 1))
            {
                return -1;
            }

            for (i = 0; i < string_len; i++)
            {
                /*lint -save -e661 */
                p_str_buf[bytes_written + i] = ((char *)p_attr)[i];
                /*lint -restore */
            }
            p_str_buf[bytes_written + i] = '\0';
            bytes_written += string_len + 1;
            break;

        case ZB_ZCL_ATTR_TYPE_IEEE_ADDR:
            /*lint -e661 -e662 -save */
            bytes_written = to_hex_str(p_str_buf, buf_len, (const uint8_t *)p_attr, sizeof(zb_64bit_addr_t), true);
            /*lint -restore */
            break;

        default:
            bytes_written = snprintf(p_str_buf, buf_len, "Value type 0x%x unsupported", attr_type);
            break;
    }

    return bytes_written;
}

int sscan_uint8(const char * p_bp, uint8_t * p_u8)
{
    unsigned int value;

    if (!sscanf(p_bp, "%u", &value) || (value > 0xff))
    {
        return 0;
    }

    *p_u8 = value & 0xFF;

    return 1;
}

void print_hexdump(nrf_cli_t const * p_cli,
                   const uint8_t * p_in, uint8_t size,
                   bool reverse)
{
    char addr_buf[2*size + 1];
    int bytes_written = 0;

    memset(addr_buf, 0, sizeof(addr_buf));

    bytes_written = to_hex_str(addr_buf, (uint16_t)sizeof(addr_buf), p_in, size, reverse);
    if (bytes_written < 0)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s", "Unable to print hexdump");
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%s", addr_buf);
    }
}
