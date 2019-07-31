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

/* Payload size in bytes, payload read from string is twice the size. */
#define CMD_PAYLOAD_SIZE    25
#define CMD_TABLE_SIZE      20
#define CMD_ROW_TIMEOUT_S   10

/* Structure to store command's parameters to send. */
typedef struct cmd_query_s
{
    zb_bool_t                           taken;
    zb_uint8_t                          seq_num;
    zb_uint8_t                          remote_addr_mode;
    zb_addr_u                           remote_node;
    zb_uint8_t                          remote_ep;
    zb_uint16_t                         profile_id;
    zb_uint16_t                         cluster_id;
    zb_uint8_t                          payload[CMD_PAYLOAD_SIZE];
    zb_uint8_t                          payload_length;
    zb_uint16_t                         cmd_id;
    zb_zcl_disable_default_response_t   def_resp;
    nrf_cli_t const                   * p_cli;
} cmd_query_t;

/* Array of structures to store command parameters. */
cmd_query_t m_cmd_data[CMD_TABLE_SIZE];

/**@brief Get the first free row in the command table and claim it, return -1 if none.
 *
 * @note Successfully acquired table row must be released by using @ref invalidate_row.
 */
static zb_int8_t acquire_row_table()
{
    int i;
    for (i = 0; i < CMD_TABLE_SIZE; i++)
    {
        if (m_cmd_data[i].taken == ZB_FALSE)
        {
            m_cmd_data[i].taken = ZB_TRUE;
            return i;
        }
    }
    return -1;
}

/**@brief Get the taken row with the selected seq_num, return -1 if none.
 * @param sernum Sequence Number to look for.
 */
static zb_int8_t get_cmd_table_row_by_sn(zb_uint8_t sernum)
{
    int i;
    for (i = 0; i < CMD_TABLE_SIZE; i++)
    {
        if (m_cmd_data[i].taken == ZB_TRUE)
        {
            if (m_cmd_data[i].seq_num == sernum)
            {
                return i;
            }
        }
    }
    return -1;
}

/**@brief Free and invaidate row.
 *
 * @param row     Number of row to invalidate.
 */
static zb_void_t invalidate_row(zb_uint8_t row)
{
    if (row < CMD_TABLE_SIZE)
    {
        ZB_MEMSET(&(m_cmd_data[row]), 0x00, sizeof(cmd_query_t));
    }
}

/**@brief Invaidate row after the timeout - ZBOSS callback
 *
 * @param row     Number of row to invalidate
 */
static zb_void_t invalidate_row_cb(zb_uint8_t row)
{
    print_error(m_cmd_data[row].p_cli, "Request timed out.");
    invalidate_row(row);
}

/**@brief Check if the frame we received is the response to our request in the table
 *
 * @param p_hdr  Pointer to the parsed header of the frame
 * @param p_row  Pointer to the row in the table to check against
 *
 * @return Whether it is response or not
 */
static zb_bool_t is_response(zb_zcl_parsed_hdr_t * p_hdr, cmd_query_t * p_row)
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

    if (p_hdr->cmd_id != ZB_ZCL_CMD_DEFAULT_RESP)
    {
        return ZB_FALSE;
    }

    return ZB_TRUE;
}

/**@brief Construct the command frame and send it.
 *
 * @param param     ZBOSS buffer to fill.
 * @param cb_param  Row of the command parameter table.
 */
static zb_void_t generic_cmd_send(zb_uint8_t param, zb_uint16_t cb_param)
{
    zb_ret_t        zb_err_code;
    zb_buf_t      * p_buf       = ZB_BUF_FROM_REF(param);
    cmd_query_t   * p_cmd_query = &m_cmd_data[cb_param];

    /* Start filling buffer with packet data */
    p_cmd_query->seq_num = ZCL_CTX().seq_number;
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(p_buf)
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (p_cmd_query->def_resp))
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), p_cmd_query->cmd_id);

    /* Scan argument with payload and put in buffer with command to send */
    for (zb_uint8_t i = 0; i < p_cmd_query->payload_length; i++)
    {
        ZB_ZCL_PACKET_PUT_DATA8(ptr, (p_cmd_query->payload[i]));
    }

    ZB_ZCL_FINISH_PACKET((p_buf), ptr)
    ZB_ZCL_SEND_COMMAND_SHORT(p_buf,
                              p_cmd_query->remote_node,
                              p_cmd_query->remote_addr_mode,
                              p_cmd_query->remote_ep,
                              zb_get_cli_endpoint(),
                              p_cmd_query->profile_id,
                              p_cmd_query->cluster_id,
                              NULL);

    /* If default response is requested, schedule an alarm */
    if (p_cmd_query->def_resp == ZB_ZCL_ENABLE_DEFAULT_RESPONSE)
    {
        zb_err_code = ZB_SCHEDULE_ALARM(invalidate_row_cb, cb_param,
                                        CMD_ROW_TIMEOUT_S * ZB_TIME_ONE_SECOND);
        /* If no free buffer, print error and invalidate buffer to disble response from def_resp_cb */
        if (zb_err_code != RET_OK)
        {
            print_error(p_cmd_query->p_cli, "No free buffer.");
            invalidate_row(cb_param);
        }
    }
    else
    {
        print_done(p_cmd_query->p_cli, ZB_FALSE);
        invalidate_row(cb_param);
    }
}

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Construct the raw ZCL frame and send it.
 *
 * @param param     ZBOSS buffer to fill.
 * @param cb_param  Row of the parameter table.
 */
static zb_void_t raw_zcl_send(zb_uint8_t param, zb_uint16_t cb_param)
{
    zb_buf_t      * p_buf       = ZB_BUF_FROM_REF(param);
    cmd_query_t   * p_cmd_query = &m_cmd_data[cb_param];

    /* Start filling buffer with packet data */
    zb_uint8_t * ptr = ZB_ZCL_START_PACKET(p_buf);

    /* Scan argument with payload and put in buffer with command to send */
    for (zb_uint8_t i = 0; i < p_cmd_query->payload_length; i++)
    {
        ZB_ZCL_PACKET_PUT_DATA8(ptr, (p_cmd_query->payload[i]));
    }

    ZB_ZCL_FINISH_PACKET((p_buf), ptr)
    ZB_ZCL_SEND_COMMAND_SHORT(p_buf,
                              p_cmd_query->remote_node,
                              p_cmd_query->remote_addr_mode,
                              p_cmd_query->remote_ep,
                              zb_get_cli_endpoint(),
                              p_cmd_query->profile_id,
                              p_cmd_query->cluster_id,
                              NULL);

    print_done(p_cmd_query->p_cli, ZB_FALSE);
    invalidate_row(cb_param);
}
#endif


/**@brief Send generic command to the remote node.
 *
 * @code
 * zcl cmd [-d] <h:dst_addr> <d:ep> <h:cluster> [-p h:profile <h:cmd_ID> [-l h:payload]
 * @endcode
 *
 * Send generic command with ID `cmd_ID` with payload `payload` to the cluster `cluster`.
 * The cluster belongs to the profile `profile`, which resides on the endpoint `ep`
 * of the remote node `dst_addr`. Optional default response can be set with `-d`.
 *
 * @note By default profile is set to Home Automation Profile
 * @note By default payload is empty
 * @note To send via binding table, set `dst_addr` and `ep` to 0.
 *
 */
void cmd_zb_generic_cmd(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ret_t        zb_err_code;
    zb_int8_t       table_row;
    cmd_query_t   * p_cmd_data      = NULL;
    char         ** p_arg           = &argv[1];
    int             ret_val;
    size_t          len;

    /* Command options description */
    static const nrf_cli_getopt_option_t opt[] = {
        NRF_CLI_OPT(
            "",
            "-d",
            "Require default response"
        ),
        NRF_CLI_OPT(
            "",
            "-p",
            "Set profile ID, HA profile by default"
        ),
        NRF_CLI_OPT(
            "",
            "-l",
            "Send payload in command, Little Endian bytes order "
        )
    };

    /* Print usage/help if requested */
    if (argc == 1)
    {
        print_usage(p_cli, argv[0],
                    " [-d] <h:eui64> <d:ep> <h:cluster> [-p h:profile]"
                    " <h:cmd_ID> [-l h:payload]\r\n");
        return;
    }
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, opt, ARRAY_SIZE(opt));
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Usage:\r\n");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "   %s %s\r\n", argv[0],
                        " [-d] <h:dst_addr> <d:ep> <h:cluster> [-p h:profile] <h:cmd_ID> [-l h:payload]\r\n");
        return;
    }

    table_row = acquire_row_table();
    if (table_row < 0)
    {
        print_error(p_cli, "No free buffer. Wait a bit");
        return;
    }
    p_cmd_data = &m_cmd_data[table_row];

    /* Make sure that there is at least 4 arguments given and no more than 9 */
    if ((argc < 5) || (argc > 10))
    {
        print_error(p_cli, "Wrong number of arguments");
        goto error;
    }

    /*  Set default command values */
    p_cmd_data->def_resp         = ZB_ZCL_DISABLE_DEFAULT_RESPONSE;
    p_cmd_data->payload_length   = 0;
    p_cmd_data->profile_id       = ZB_AF_HA_PROFILE_ID;

    /* Check if default response should be requested */
    if (strcmp(*p_arg, "-d") == 0)
    {
        p_cmd_data->def_resp = ZB_ZCL_ENABLE_DEFAULT_RESPONSE;
        p_arg++;
    }

    /* Parse and check remote node address */
    p_cmd_data->remote_addr_mode = parse_address(*p_arg, &(p_cmd_data->remote_node), ADDR_ANY);
    if (p_cmd_data->remote_addr_mode == ADDR_INVALID)
    {
        /* Handle the Binding table case (address == '0'). */
        if (!strcmp(*p_arg, "0"))
        {
            p_cmd_data->remote_addr_mode = ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
            p_cmd_data->remote_node.addr_short = 0;
        }
        else
        {
            print_error(p_cli, "Wrong address format");
            goto error;
        }
    }
    p_arg++;

    /* Read endpoint and cluster id */
    ret_val = sscan_uint8(*(p_arg++), &(p_cmd_data->remote_ep));
    if (ret_val == 0)
    {
        print_error(p_cli, "Remote ep value");
        goto error;
    }

    ret_val = sscanf(*(p_arg++), "%hx", &(p_cmd_data->cluster_id));
    if (ret_val == 0)
    {
        print_error(p_cli, "Cluster id value");
        goto error;
    }

    /* Check if different from HA profile should be used */
    if (strcmp(*p_arg, "-p") == 0)
    {
        ret_val = sscanf(*(++p_arg), "%hx", &(p_cmd_data->profile_id));
        if (ret_val == 0)
        {
            print_error(p_cli, "Profile id value");
            goto error;
        }
        p_arg++;
    }

    /* Read command id */
    ret_val = sscanf(*(p_arg++), "%hx", &(p_cmd_data->cmd_id));
    if (ret_val == 0)
    {
        print_error(p_cli, "Cmd id value");
        goto error;
    }

    /* Check if payload should be sent */
    if (strcmp(*(p_arg++), "-l") == 0)
    {
        len = strlen(*p_arg);
        if (len > (2 * CMD_PAYLOAD_SIZE))
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_WARNING, "Payload length is too big, trimming it to first %d bytes\n", CMD_PAYLOAD_SIZE);
            len = (2 * CMD_PAYLOAD_SIZE);
        }
        if ((len != 0) && (len % 2) != 0)
        {
            print_error(p_cli, "Payload length is not even");
            goto error;
        }
        ret_val = parse_hex_str(*p_arg, p_cmd_data->payload, len, false);
        if (ret_val == 0)
        {
            print_error(p_cli, "Payload value");
            goto error;
        }
        p_cmd_data->payload_length = len/2;
    }

    /* Get buffer and send command */
    p_cmd_data->p_cli = p_cli;
    zb_err_code = ZB_GET_OUT_BUF_DELAYED2(generic_cmd_send, table_row);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "No frame left - wait a bit");
        /* Mark data structure as free. */
        goto error;
    }
    return;

    error:
        invalidate_row(table_row);
}


#ifdef ZIGBEE_CLI_DEBUG
/**@brief Send raw ZCL frame.
 *
 * @code
 * zcl raw <h:eui64> <d:ep> <h:cluster> <h:profile> <h:payload>
 * @endcode
 *
 * Send raw ZCL frame with payload `payload` to the cluster `cluster`.
 * The cluster belongs to the profile `profile`, which resides on the endpoint `ep`
 * of the remote node `dst_addr`. The payload represents the ZCL Header plus ZCL Payload.
 *
 * @note To send via binding table, set `eui64` and `ep` to 0.
 * 
 */
void cmd_zb_zcl_raw(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ret_t        zb_err_code;
    zb_int8_t       table_row;
    cmd_query_t   * p_cmd_data      = NULL;
    char         ** p_arg           = &argv[1];
    int             ret_val;
    size_t          len;

    /* Debug mode quick return. */
    if (!zb_cli_debug_get())
    {
        print_error(p_cli, "This command is available only in debug mode. Run 'debug on' to enable it.");
        return;
    }

    /* Print usage/help if requested */
    if (argc == 1)
    {
        print_usage(p_cli, argv[0],
                    " <h:eui64> <d:ep> <h:cluster> <h:profile>"
                    " <h:raw_data>\r\n");
        return;
    }

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Usage:\r\n");
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "   %s %s\r\n", argv[0],
                        " <h:eui64> <d:ep> <h:cluster> <h:profile> <h:raw_data>\r\n");
        return;
    }

    table_row = acquire_row_table();
    if (table_row < 0)
    {
        print_error(p_cli, "No free buffer. Wait a bit");
        return;
    }
    p_cmd_data = &m_cmd_data[table_row];

    /* Make sure that the exact number of arguments is given */
    if (argc != 6)
    {
        print_error(p_cli, "Wrong number of arguments");
        goto error;
    }

    /*  Reset the counter */
    p_cmd_data->payload_length = 0;

    /* Parse and check remote node address */
    p_cmd_data->remote_addr_mode = parse_address(*p_arg, &(p_cmd_data->remote_node), ADDR_ANY);
    if (p_cmd_data->remote_addr_mode == ADDR_INVALID)
    {
        /* Handle the Binding table case (address == '0'). */
        if (!strcmp(*p_arg, "0"))
        {
            p_cmd_data->remote_addr_mode = ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
            p_cmd_data->remote_node.addr_short = 0;
        }
        else
        {
            print_error(p_cli, "Wrong EUI64 address format");
            goto error;
        }
    }
    p_arg++;

    /* Read endpoint and cluster id */
    ret_val = sscan_uint8(*(p_arg++), &(p_cmd_data->remote_ep));
    if (ret_val == 0)
    {
        print_error(p_cli, "Remote ep value");
        goto error;
    }

    ret_val = sscanf(*(p_arg++), "%hx", &(p_cmd_data->cluster_id));
    if (ret_val == 0)
    {
        print_error(p_cli, "Cluster id value");
        goto error;
    }

    ret_val = sscanf(*(p_arg++), "%hx", &(p_cmd_data->profile_id));
    if (ret_val == 0)
    {
        print_error(p_cli, "Profile id value");
        goto error;
    }

    /* Reuse the payload field from the context. */
    len = strlen(*p_arg);
    if (len > (2 * CMD_PAYLOAD_SIZE))
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_WARNING, "Raw data length is too big, trimming it to first %d bytes\n", CMD_PAYLOAD_SIZE);
        len = (2 * CMD_PAYLOAD_SIZE);
    }

    if ((len != 0) && (len % 2) != 0)
    {
        print_error(p_cli, "Payload length is not even");
        goto error;
    }

    ret_val = parse_hex_str(*p_arg, p_cmd_data->payload, len, false);
    if (ret_val == 0)
    {
        print_error(p_cli, "Payload value");
        goto error;
    }
    p_cmd_data->payload_length = len/2;

    /* Get buffer and send command */
    p_cmd_data->p_cli = p_cli;
    zb_err_code = ZB_GET_OUT_BUF_DELAYED2(raw_zcl_send, table_row);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "No frame left - wait a bit");
        goto error;
    }
    return;

    error:
        invalidate_row(table_row);
}
#endif /* ZIGBEE_CLI_DEBUG */


/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param    Reference to a ZBOSS buffer
 */
static zb_uint8_t cli_agent_ep_handler_generic_cmd(uint8_t param)
{
    zb_buf_t              * p_zcl_cmd_buf   = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    zb_zcl_parsed_hdr_t   * p_cmd_info      = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    zb_int8_t               row;
    zb_ret_t                zb_err_code;

    /* Get the row in the requests table according by the sequence number */
    row = get_cmd_table_row_by_sn(p_cmd_info->seq_number);
    if (row == -1)
    {
        return ZB_FALSE;
    }

    cmd_query_t * p_row = &(m_cmd_data[row]);
    if (!is_response(p_cmd_info, p_row))
    {
        return ZB_FALSE;
    }

    if (p_cmd_info->cmd_id == ZB_ZCL_CMD_DEFAULT_RESP)
    {
        zb_zcl_default_resp_payload_t * p_def_resp;
        p_def_resp = ZB_ZCL_READ_DEFAULT_RESP(p_zcl_cmd_buf);

        /* Print info received from default response */
        nrf_cli_fprintf(p_row->p_cli,
                       (p_def_resp->status == ZB_ZCL_STATUS_SUCCESS) ? NRF_CLI_INFO : NRF_CLI_ERROR,
                       "Default Response received: ");
        nrf_cli_fprintf(p_row->p_cli,
                       (p_def_resp->status == ZB_ZCL_STATUS_SUCCESS) ? NRF_CLI_INFO : NRF_CLI_ERROR,
                       "Command: %d, Status: %d",
                        p_def_resp->command_id, p_def_resp->status);
        nrf_cli_fprintf(p_row->p_cli,
                       (p_def_resp->status == ZB_ZCL_STATUS_SUCCESS) ? NRF_CLI_INFO : NRF_CLI_ERROR,
                       "\r\n");

        if (p_def_resp->status != ZB_ZCL_STATUS_SUCCESS)
        {
            print_error(p_row->p_cli, "Command not successful.");
        }
        else
        {
            print_done(p_row->p_cli, ZB_FALSE);
        }
    }
    else /* In case of unknown response */
    {
        print_error(p_row->p_cli, "Unknown response.");
    }
    /* Cancel the ongoing alarm which was to erase the row... */
    if (m_cmd_data[row].def_resp == ZB_ZCL_ENABLE_DEFAULT_RESPONSE)
    {
        zb_err_code = (ZB_SCHEDULE_ALARM_CANCEL(invalidate_row_cb, row));
        ZB_ERROR_CHECK(zb_err_code);
    }
    /* ...and erase it manually */
    invalidate_row(row);

    ZB_FREE_BUF(p_zcl_cmd_buf);
    return ZB_TRUE;
}


/**@brief Endpoint handlers
 */
 NRF_ZIGBEE_EP_HANDLER_REGISTER(generic_cmd, cli_agent_ep_handler_generic_cmd);
/** @} */
