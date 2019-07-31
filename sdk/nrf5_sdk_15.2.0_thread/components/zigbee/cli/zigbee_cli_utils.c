/**
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
#include <ctype.h>

NRF_SECTION_DEF(zb_ep_handlers, zb_device_handler_t);
#define ZB_EP_HANDLER_SECTION_ITEM_GET(i) NRF_SECTION_ITEM_GET(zb_ep_handlers, zb_device_handler_t, (i))
#define ZB_EP_HANDLER_SECTION_ITEM_COUNT  NRF_SECTION_ITEM_COUNT(zb_ep_handlers, zb_device_handler_t)

zb_uint8_t cli_agent_ep_handler(zb_uint8_t param)
{
    unsigned int idx;

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

zb_bool_t parse_address_string(char * input, zb_addr_u * output, zb_uint8_t * addr_mode)
{
    char byte[2 + 1]; /* Substring holding 1 byte */
    unsigned int chunk;
    unsigned int idx;

    /* Check the input string for validity */
    if (strlen(input) != sizeof(zb_ieee_addr_t) * 2)
    {
        return ZB_FALSE;
    }

    for (idx = 0; idx < sizeof(zb_ieee_addr_t) * 2; idx++)
    {
        if (!isxdigit(input[idx]))
        {
            return ZB_FALSE;
        }
    }

    /* Set the terminator */
    byte[2] = 0;
    for (idx = 0; idx < sizeof(zb_ieee_addr_t); idx++)
    {
        strncpy(byte, input + (2 * sizeof(zb_ieee_addr_t) - 2 * (idx + 1)), 2);
        sscanf(byte, "%x", &chunk);
        *(((zb_uint8_t*)output->addr_long) + idx) = chunk;
    }
    *addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;

    return ZB_TRUE;
}

zb_void_t frame_acked_cb(zb_uint8_t param)
{
    if (param)
    {
        ZB_FREE_BUF_BY_REF(param);
    }
}

int ieee_addr_to_str(char * p_str_buf, uint16_t buf_len, zb_ieee_addr_t p_addr)
{
    int bytes_written = 0;
    int status;

    for (int i = sizeof(zb_ieee_addr_t) - 1; i >= 0; i--)
    {
        status = snprintf(p_str_buf + bytes_written, buf_len - bytes_written, "%02x", p_addr[i]);
        if (status < 0)
        {
            return status;
        }

        bytes_written += status;
    }

    return bytes_written;
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

        default:
            bytes_written = snprintf(p_str_buf, buf_len, "Value type 0x%x unsupported", attr_type);
            break;
    }

    return bytes_written;
}

int sscan_uint8(const char * p_bp, uint8_t * p_u8)
{
    uint16_t u16;

    if (!sscanf(p_bp, "%hd", &u16))
    {
        return 0;
    }

    *p_u8 = (uint8_t)u16;

    return 1;
}
