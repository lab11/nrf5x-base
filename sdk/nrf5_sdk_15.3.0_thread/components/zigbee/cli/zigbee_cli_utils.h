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
#ifndef ZIGBEE_CLI_UTILS_H__
#define ZIGBEE_CLI_UTILS_H__
#include "nrf_cli.h"
#include "zboss_api.h"
#include "zigbee_helpers.h"

/*@brief Macro which defines the Endpoint Handler section,
 *       which allows iterating over them.
 */
#define NRF_ZIGBEE_EP_HANDLER_REGISTER(desc, p_handler)           \
    NRF_SECTION_ITEM_REGISTER(zb_ep_handlers, zb_device_handler_t const CONCAT_2(zb_ep_, desc)) = p_handler;

static inline void print_usage(nrf_cli_t const * p_cli,
                               const char * p_command,
                               const char * p_help_string)
{
    nrf_cli_help_print(p_cli, NULL, 0);
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Usage:\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "   %s %s\r\n", p_command, p_help_string);
}

/**@brief Finish the command by dumping 'Done'.
 *
 * @param prepend_newline      Whether to prepend a newline.
 */
static inline void print_done(nrf_cli_t const * p_cli, zb_bool_t prepend_newline)
{
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, prepend_newline ? "\r\nDone\r\n" : "Done\r\n");
}

/**@brief Print error message to the console.
 *
 * @param p_message Poiter to the message which should be printed as an error.
 */
static inline void print_error(nrf_cli_t const * p_cli, const char * p_message)
{
    if (p_message)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: %s\r\n", p_message);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: Unknown error occured\r\n");
    }
}

/**@breif Print a list of items.
 * 
 * Individual items in the list are delimited by comma.
 *
 * @param p_cli a pointer to CLI instance
 * @param hdr   the list header string
 * @param fmt   a printf like format of an individual list item
 * @param type  type of the list item
 * @param size  the list size (in items)
 */
#define PRINT_LIST(p_cli, hdr, fmt, type, ptr, size)                 \
{                                                                    \
    /*lint -e662 */                                                  \
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, hdr);                     \
    for (type * p_item = (ptr); p_item < (ptr) + size - 1; p_item++) \
    {                                                                \
        nrf_cli_fprintf(p_cli,                                       \
                        NRF_CLI_NORMAL,                              \
                        fmt ",",                                     \
                        *p_item);                                    \
    }                                                                \
                                                                     \
    if (size > 0)                                                    \
    {                                                                \
        nrf_cli_fprintf(p_cli,                                       \
                        NRF_CLI_NORMAL,                              \
                        fmt " ",                                     \
                        *((ptr) + size - 1));                        \
    }                                                                \
    /*lint -restore */                                               \
} 

/**@brief The callback which is to be called whenever an APS ACK comes to a
 *        frame being sent. The buffer which informs the user about the delivery
 *        is being freed.
 */
zb_void_t frame_acked_cb(zb_uint8_t param);

/**@brief Convert ZCL attribute value to string.
 *
 * @param p_str_buf[out]  Pointer to a string buffer which will be filled.
 * @param buf_len[in]     String buffer length.
 * @param attr_type[in]   ZCL attribute type value.
 * @param p_attr[in]      Pointer to ZCL attribute value.
 *
 * @return number of bytes written into string buffer or negative value on error.
 */
int zcl_attr_to_str(char * p_str_buf, uint16_t buf_len, zb_uint16_t attr_type, zb_uint8_t * p_attr);

/**
 * @brief Parse uint8_t from input string.
 *
 * The reason for this explicit function is because newlib-nano sscanf() does not support 1-byte target.
 *
 * @param[in]  p_bp Pointer to input string.
 * @param[out] p_u8 Pointer to output value.
 *
 * @return 1 on success, 0 otherwise
 */
int sscan_uint8(const char * p_bp, uint8_t * p_u8);

/**@brief Print buffer as hex string.
 *
 * @param p_cli     Pointer to CLI instance.
 * @param p_in      Pointer to data to be printed out.
 * @param size      Data size in bytes
 * @param reverse   If True then data is printed out in reverse order.
 */
void print_hexdump(nrf_cli_t const * p_cli,
                   const uint8_t * p_in, uint8_t size,
                   bool reverse);

/**@brief Print 64bit value (address, extpan) as hex string.
 *
 * The value is expected to be low endian.
 *
 * @param p_cli     Pointer to CLI instance.
 * @param addr      64 data to be printed out.
 */
static inline void print_eui64(nrf_cli_t const * p_cli, const zb_64bit_addr_t addr)
{
    print_hexdump(p_cli, (const uint8_t *)addr, sizeof(zb_64bit_addr_t), true);
}

#endif /* ZIGBEE_CLI_UTILS_H__ */
