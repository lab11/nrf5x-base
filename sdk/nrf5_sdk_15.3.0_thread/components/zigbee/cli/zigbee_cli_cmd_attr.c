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
#include "zigbee_cli.h"
#include "zigbee_cli_utils.h"

/**
 * @defgroup zb_cli_cmd_zcl ZCL commands
 * @ingroup zb_cli
 *
 * @{
 */

#define ATTRIBUTE_TABLE_SIZE     20
#define ATTRIBUTE_ROW_TIMEOUT_S  10

typedef enum attr_type_e
{
    ATTR_READ_REQUEST,
    ATTR_WRITE_REQUEST
} attr_req_type_t;

/**@brief The row of the table which holds the requests which were sent.
 *
 * @details We compare the incoming responses with the rows contents to determine
 *          if it is the answer. The structure corresponds to both read and write
 *          requests.
 *          The key parameter is the sequence number.
 */
typedef struct attr_query_s
{
    zb_bool_t                  taken;
    zb_uint8_t                 seq_num;
    attr_req_type_t            req_type;
    zb_addr_u                  remote_node;
    zb_uint8_t                 remote_addr_mode;
    zb_uint8_t                 remote_ep;
    zb_uint16_t                profile_id;
    zb_uint16_t                cluster_id;
    zb_uint16_t                attr_id;
    zb_uint8_t                 attr_type;
    zb_uint8_t                 attr_value[32];
    zb_zcl_frame_direction_t   direction;
    nrf_cli_t                * p_cli;
} attr_query_t;

static attr_query_t m_attr_table[ATTRIBUTE_TABLE_SIZE];

/**@brief Get the first free row in the attributes table, return -1 if none
 */
static zb_int8_t get_free_row_attr_table()
{
    int i;
    for (i = 0; i < ATTRIBUTE_TABLE_SIZE; i++)
    {
        if (m_attr_table[i].taken == ZB_FALSE)
        {
            return i;
        }
    }
    return -1;
}

/**@brief Get the taken row with the selected seq_num, return -1 if none
 * @param sernum Sequence Number to look for
 */
static zb_int8_t get_attr_table_row_by_sn(zb_uint8_t sernum)
{
    int i;
    for (i = 0; i < ATTRIBUTE_TABLE_SIZE; i++)
    {
        if (m_attr_table[i].taken == ZB_TRUE)
        {
            if (m_attr_table[i].seq_num == sernum)
            {
                return i;
            }
        }
    }
    return -1;
}

/**@brief Invalidate row after the timeout.
 *
 * @param row     Number of row to invalidate
 */
static zb_void_t invalidate_row(zb_uint8_t row)
{
    if (row < ATTRIBUTE_TABLE_SIZE)
    {
        ZB_MEMSET(&(m_attr_table[row]), 0x00, sizeof(attr_query_t));
    }
}

/**@brief Invalidate row after the timeout - ZBOSS callback
 *
 * @param row     Number of row to invalidate
 */
static zb_void_t invalidate_row_cb(zb_uint8_t row)
{
    nrf_cli_fprintf(m_attr_table[row].p_cli, NRF_CLI_ERROR, "Error: request timed out.\r\n");
    invalidate_row(row);
}

/**@brief Check if the frame we received is the response to our request in the table
 *
 * @param p_hdr  Pointer to the parsed header of the frame
 * @param p_row  Pointer to the row in the table to check against
 *
 * @return Whether it is response or not
 */
static zb_bool_t is_response(zb_zcl_parsed_hdr_t * p_hdr, attr_query_t * p_row)
{
    zb_uint16_t remote_node_short = 0;
    if (p_row->remote_addr_mode == ZB_APS_ADDR_MODE_64_ENDP_PRESENT)
    {
        remote_node_short = zb_address_short_by_ieee(p_row->remote_node.addr_long);
    }
    else
    {
        remote_node_short = p_row->remote_node.addr_short;
    }

    if (p_hdr->cluster_id != p_row->cluster_id)
    {
        return ZB_FALSE;
    }

    if (p_hdr->profile_id != p_row->profile_id)
    {
        return ZB_FALSE;
    }

    if (p_hdr->addr_data.common_data.src_endpoint != p_row->remote_ep)
    {
        return ZB_FALSE;
    }

    if (p_hdr->addr_data.common_data.source.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
    {
        if (p_hdr->addr_data.common_data.source.u.short_addr != remote_node_short)
        {
            return ZB_FALSE;
        }
    }
    else
    {
        return ZB_FALSE;
    }

    if (p_hdr->cmd_id != ZB_ZCL_CMD_DEFAULT_RESP &&
        p_hdr->cmd_id != ZB_ZCL_CMD_READ_ATTRIB_RESP &&
        p_hdr->cmd_id != ZB_ZCL_CMD_WRITE_ATTRIB_RESP)
    {
        return ZB_FALSE;
    }

    return ZB_TRUE;
}

/**@brief Print the Read Attribute Response
 *
 * @param p_zcl_cmd_buf    Pointer to a ZBOSS buffer
 * @param p_row            Pointer to a row in attr table
 */
static void print_read_attr_response(zb_buf_t * p_zcl_cmd_buf, attr_query_t * p_row)
{
    zb_zcl_read_attr_res_t * p_attr_resp;
    /* Get the contents of Read Attribute Response frame */
    ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(p_zcl_cmd_buf, p_attr_resp);
    if (p_attr_resp->status == ZB_ZCL_STATUS_SUCCESS)
    {
        char attr_buf[40];
        int bytes_written = zcl_attr_to_str(attr_buf, sizeof(attr_buf), p_attr_resp->attr_type, p_attr_resp->attr_value);

        if (bytes_written < 0)
        {
            print_error(p_row->p_cli, "Unable to print attribute value");
        }
        else
        {
            nrf_cli_fprintf(p_row->p_cli, NRF_CLI_NORMAL, "ID: %d Type: %x Value: %s\r\n",
                            p_attr_resp->attr_id, p_attr_resp->attr_type, attr_buf);
            print_done(p_row->p_cli, ZB_FALSE);
        }
    }
    else
    {
        nrf_cli_fprintf(p_row->p_cli, NRF_CLI_NORMAL, "Error: status %d\r\n",
                        p_attr_resp->status);
    }
}

/**@brief Print the Write Attribute Response
 *
 * @param p_zcl_cmd_buf    Pointer to a ZBOSS buffer
 * @param p_row            Pointer to a row in attr table
 */
static void print_write_attr_response(zb_buf_t * p_zcl_cmd_buf, attr_query_t * p_row)
{
    zb_zcl_write_attr_res_t * p_attr_resp;
    /* Get the contents of Write Attribute Response frame */
    ZB_ZCL_GET_NEXT_WRITE_ATTR_RES(p_zcl_cmd_buf, p_attr_resp);

    if (!p_attr_resp)
    {
        nrf_cli_fprintf(p_row->p_cli, NRF_CLI_NORMAL, "Error: no attribute could be retrieved.\r\n");
        return;
    }

    if (p_attr_resp->status != ZB_ZCL_STATUS_SUCCESS)
    {
        nrf_cli_fprintf(p_row->p_cli, NRF_CLI_NORMAL, "Error: status %d\r\n",
                        p_attr_resp->status);
        return;
    }

    print_done(p_row->p_cli, ZB_FALSE);
}

/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param    Reference to a ZBOSS buffer
 */
static zb_uint8_t cli_agent_ep_handler_attr(zb_uint8_t param)
{
    zb_buf_t * p_zcl_cmd_buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    zb_zcl_parsed_hdr_t * p_cmd_info = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    zb_int8_t row;

    /* Get the row in the requests table according by the sequence number */
    row = get_attr_table_row_by_sn(p_cmd_info->seq_number);
    if (row == -1)
    {
        return ZB_FALSE;
    }

    attr_query_t * p_row = &(m_attr_table[row]);
    if (!is_response(p_cmd_info, p_row))
    {
        return ZB_FALSE;
    }

    if (p_cmd_info->cmd_id == ZB_ZCL_CMD_DEFAULT_RESP)
    {
        zb_zcl_default_resp_payload_t * p_def_resp;
        p_def_resp = ZB_ZCL_READ_DEFAULT_RESP(p_zcl_cmd_buf);
        nrf_cli_fprintf(p_row->p_cli, NRF_CLI_ERROR, "Error: Default Response received; ");
        nrf_cli_fprintf(p_row->p_cli, NRF_CLI_ERROR, "Command: %d, Status: %d ",
                        p_def_resp->command_id, p_def_resp->status);
    }
    else
    {
        if (p_row->req_type == ATTR_READ_REQUEST)
        {
            print_read_attr_response(p_zcl_cmd_buf, p_row);
        }
        else
        {
            print_write_attr_response(p_zcl_cmd_buf, p_row);
        }
    }
    /* Cancel the ongoing alarm which was to erase the row... */
    UNUSED_RETURN_VALUE(ZB_SCHEDULE_ALARM_CANCEL(invalidate_row_cb, row));
    /* ...and erase it manually */
    invalidate_row(row);

    ZB_FREE_BUF(p_zcl_cmd_buf);
    return ZB_TRUE;
}


/**@brief Actually construct the Read Attribute frame and send it.
 *
 * @param param ZBOSS buffer to fill.
 * @param cb_param Row of the read attribute table to refer to.
 */
static zb_void_t readattr_send(zb_uint8_t param, zb_uint16_t cb_param)
{
    zb_ret_t zb_err_code;
    zb_buf_t * p_buf = ZB_BUF_FROM_REF(param);
    zb_uint8_t row = cb_param;
    zb_uint8_t * p_cmd_buf;
    attr_query_t * p_row = &(m_attr_table[row]);

    p_row->seq_num = ZCL_CTX().seq_number;
    ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ_A(p_buf, p_cmd_buf, p_row->direction, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(p_cmd_buf, p_row->attr_id);
    ZB_ZCL_GENERAL_SEND_READ_ATTR_REQ(p_buf, p_cmd_buf, p_row->remote_node,
                                      p_row->remote_addr_mode, p_row->remote_ep,
                                      zb_get_cli_endpoint(), p_row->profile_id,
                                      p_row->cluster_id, frame_acked_cb);

    zb_err_code = ZB_SCHEDULE_ALARM(invalidate_row_cb, row,
                                    ATTRIBUTE_ROW_TIMEOUT_S * ZB_TIME_ONE_SECOND);
    ZB_ERROR_CHECK(zb_err_code);
}

/**@brief Actually construct the Write Attribute frame and send it.
 *
 * @param param ZBOSS buffer to fill.
 * @param cb_param Row of the read attribute table to refer to.
 */
static zb_void_t writeattr_send(zb_uint8_t param, zb_uint16_t cb_param)
{
    zb_ret_t zb_err_code;
    zb_buf_t * p_buf = ZB_BUF_FROM_REF(param);
    zb_uint8_t row = cb_param;
    zb_uint8_t * p_cmd_buf;
    attr_query_t * p_row = &(m_attr_table[row]);
    p_row->seq_num = ZCL_CTX().seq_number;

    ZB_ZCL_GENERAL_INIT_WRITE_ATTR_REQ_A(p_buf, p_cmd_buf, p_row->direction, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
    ZB_ZCL_GENERAL_ADD_VALUE_WRITE_ATTR_REQ(p_cmd_buf, p_row->attr_id, p_row->attr_type,
                                            p_row->attr_value);
    ZB_ZCL_GENERAL_SEND_WRITE_ATTR_REQ(p_buf, p_cmd_buf, p_row->remote_node,
                                       p_row->remote_addr_mode, p_row->remote_ep,
                                       zb_get_cli_endpoint(), p_row->profile_id,
                                       p_row->cluster_id, frame_acked_cb);

    zb_err_code = ZB_SCHEDULE_ALARM(invalidate_row_cb, row,
                                    ATTRIBUTE_ROW_TIMEOUT_S * ZB_TIME_ONE_SECOND);
    ZB_ERROR_CHECK(zb_err_code);
}

/**@brief Retrieve the attribute value of the remote node.
 *
 * @code
 * zcl attr read <h:dst_addr> <d:ep> <h:cluster> <h:profile> <h:attr_id>
 * @endcode
 *
 * Read the value of the attribute `attr_id` in the cluster `cluster`.
 * The cluster belongs to the profile `profile`, which resides on the endpoint
 * `ep` of the remote node `dst_addr`.
 *
 */
void cmd_zb_readattr(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ret_t zb_err_code;
    zb_int8_t row = get_free_row_attr_table();

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "  h: is for hex, d: is for decimal, -c switches the server-to-client direction\r\n");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "  readattr <h:dst_addr> <d:ep> <h:cluster> [-c] ");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "<h:profile> <h:attr ID>\r\n");
        return;
    }

    bool is_direction_present = ((argc == 7) && !strcmp(argv[4], "-c"));

    if (argc != 6 && !is_direction_present)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong number of arguments\r\n");
        return;
    }

    if (row == -1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: request pool empty - wait a bit\r\n");
        return;
    }

    attr_query_t * p_row = &(m_attr_table[row]);

    p_row->remote_addr_mode = parse_address(*(++argv), &(p_row->remote_node), ADDR_ANY);
    if (p_row->remote_addr_mode == ADDR_INVALID)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong address format\r\n");
        return;
    }

    UNUSED_RETURN_VALUE(sscan_uint8(*(++argv), &(p_row->remote_ep)));
    sscanf(*(++argv), "%hx", &(p_row->cluster_id));
    if (is_direction_present)
    {
        p_row->direction = ZB_ZCL_FRAME_DIRECTION_TO_CLI;
        ++argv;
    }
    else
    {
        p_row->direction = ZB_ZCL_FRAME_DIRECTION_TO_SRV;
    }
    sscanf(*(++argv), "%hx", &(p_row->profile_id));
    sscanf(*(++argv), "%hx", &(p_row->attr_id));
    p_row->req_type = ATTR_READ_REQUEST;
    p_row->taken = ZB_TRUE;
    /* Put the CLI instance to be used later */
    p_row->p_cli = (nrf_cli_t*)p_cli;

    zb_err_code = ZB_GET_OUT_BUF_DELAYED2(readattr_send, row);
    if (zb_err_code != RET_OK)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: no frame left - wait a bit\r\n");
        /* Invalidate row so that we can reuse it */
        invalidate_row(row);
    }
}

/**@brief Write the attribute value to the remote node.
 *
 * @code
 * zcl attr write <h:dst_addr> <d:ep> <h:cluster> <h:profile> <h:attr_id> <h:attr_type> <h:attr_value>
 * @endcode
 *
 * Write the `attr_value` value of the attribute `attr_id` of the type
 * `attr_type` in the cluster `cluster`. The cluster belongs to the profile
 * `profile`, which resides on the endpoint `ep` of the remote node `dst_addr`.
 *
 * @note The `attr_value` value must be in hexadecimal format, unless it is a
 * string (`attr_type == 42`), then it must be a string.
 *
 */
void cmd_zb_writeattr(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ret_t zb_err_code;
    zb_int8_t row = get_free_row_attr_table();

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "  h: is for hex, d: is for decimal, -c switches the server-to-client direction\r\n");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "  writeattr <h:dst_addr> <d:ep> <h:cluster> [-c] ");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "<h:profile> <h:attr ID> <h:attr type> ");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "<h:attr value>\r\n");
        return;
    }

    bool is_direction_present = ((argc == 9) && !strcmp(argv[4], "-c"));

    if (argc != 8 && !is_direction_present)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong number of arguments\r\n");
        return;
    }

    if (row == -1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: request pool empty - wait a bit\r\n");
        return;
    }

    attr_query_t * p_row = &(m_attr_table[row]);

    p_row->remote_addr_mode = parse_address(*(++argv), &(p_row->remote_node), ADDR_ANY);
    if (p_row->remote_addr_mode == ADDR_INVALID)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong address format\r\n");
        return;
    }

    UNUSED_RETURN_VALUE(sscan_uint8(*(++argv), &(p_row->remote_ep)));
    sscanf(*(++argv), "%hx", &(p_row->cluster_id));
    if (is_direction_present)
    {
        p_row->direction = ZB_ZCL_FRAME_DIRECTION_TO_CLI;
        ++argv;
    }
    else
    {
        p_row->direction = ZB_ZCL_FRAME_DIRECTION_TO_SRV;
    }
    sscanf(*(++argv), "%hx", &(p_row->profile_id));
    sscanf(*(++argv), "%hx", &(p_row->attr_id));
    sscanf(*(++argv), "%x", (zb_uint32_t*)&(p_row->attr_type)); // newlib-nano sscanf limitations
    switch (p_row->attr_type)
    {
        /* Let's handle gracefully the string */
        case ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            p_row->attr_value[0] = strlen(*(++argv));
            strncpy((zb_char_t*)(p_row->attr_value + 1), *argv, sizeof(p_row->attr_value) - 1);
            break;
        /* All others for now can be just a hexdump */
        default:
            sscanf(*(++argv), "%x", (zb_uint32_t*)p_row->attr_value);
            break;
    }
    p_row->req_type = ATTR_WRITE_REQUEST;
    p_row->taken = ZB_TRUE;
    /* Put the CLI instance to be used later */
    p_row->p_cli = (nrf_cli_t*)p_cli;

    zb_err_code = ZB_GET_OUT_BUF_DELAYED2(writeattr_send, row);
    if (zb_err_code != RET_OK)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: no frame left - wait a bit\r\n");
        /* Invalidate row so that we can reuse it */
        invalidate_row(row);
    }
}

/**@brief Endpoint handlers
 */
NRF_ZIGBEE_EP_HANDLER_REGISTER(attr, cli_agent_ep_handler_attr);

/** @} */
