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
#include "sdk_config.h"
#include "zigbee_cli_ping.h"
#include "zb_nrf52840_internal.h"
#include "zb_error_handler.h"
#include "zigbee_cli.h"
#include "nrf_log_instance.h"
#include "nrf_log.h"

/**
 * @defgroup zb_cli_cmd_zcl ZCL commands
 * @ingroup zb_cli
 *
 * @{
 */

#include "zigbee_cli_utils.h"

/** @brief ZCL Frame control field of Zigbee PING commands.
 */
#define ZIGBEE_PING_FRAME_CONTROL_FIELD 0x11

#if NRF_LOG_ENABLED
/** @brief Name of the submodule used for logger messaging.
 */
#define NRF_LOG_SUBMODULE_NAME ping

NRF_LOG_INSTANCE_REGISTER(ZIGBEE_CLI_LOG_NAME, NRF_LOG_SUBMODULE_NAME,
                          ZIGBEE_CLI_CONFIG_INFO_COLOR,
                          ZIGBEE_CLI_CONFIG_DEBUG_COLOR,
                          ZIGBEE_CLI_CONFIG_LOG_INIT_FILTER_LEVEL,
                          ZIGBEE_CLI_CONFIG_LOG_ENABLED ?
                                  ZIGBEE_CLI_CONFIG_LOG_LEVEL : NRF_LOG_SEVERITY_NONE);

// This structure keeps reference to the logger instance used by this module.
typedef struct {
    NRF_LOG_INSTANCE_PTR_DECLARE(p_log)
} log_ctx_t;
#endif // NRF_LOG_ENABLED

/**@brief The row of the table which holds the replies which are to be sent.
 *
 * @details We use the table to temporarily store the parameters of the ping reply
 *          while it is traversing the ZBOSS callback system.
 *          The key parameter is the sequence number.
 */
typedef struct ping_reply_s
{
    zb_bool_t     taken;
    zb_uint16_t   remote_short_addr;
    zb_uint8_t    ping_seq;
    zb_uint8_t    count;
    zb_uint8_t    send_ack;
    nrf_cli_t   * p_cli;
} ping_reply_t;


static ping_request_t m_ping_request_table[PING_TABLE_SIZE];
static ping_reply_t   m_ping_reply_table[PING_TABLE_SIZE];
static uint8_t        m_ping_seq_num;
static ping_time_cb_t mp_ping_ind_cb = NULL;

#if NRF_LOG_ENABLED
// Logger instance used by this module.
static log_ctx_t m_log = {
    NRF_LOG_INSTANCE_PTR_INIT(p_log, ZIGBEE_CLI_LOG_NAME, NRF_LOG_SUBMODULE_NAME)
};
#endif // NRF_LOG_ENABLED

static zb_uint32_t get_request_duration(ping_request_t * p_request);

ping_request_t * zb_ping_acquire_request(void)
{
    int i;

    for (i = 0; i < PING_TABLE_SIZE; i++)
    {
        if (m_ping_request_table[i].taken == ZB_FALSE)
        {
            m_ping_request_table[i].taken = ZB_TRUE;
            return &(m_ping_request_table[i]);
        }
    }

    return NULL;
}

zb_void_t zb_ping_release_request(ping_request_t * p_reply)
{
    if (p_reply != NULL)
    {
        ZB_MEMSET(p_reply, 0x00, sizeof(ping_request_t));
    }
}

/**@brief Acquire ping reply context.
 *
 * @return  Pointer to a free ping reply context or NULL on failure.
 */
static ping_reply_t * ping_aquire_reply(void)
{
    int i;

    for (i = 0; i < PING_TABLE_SIZE; i++)
    {
        if (m_ping_reply_table[i].taken == ZB_FALSE)
        {
            m_ping_reply_table[i].taken = ZB_TRUE;
            return &(m_ping_reply_table[i]);
        }
    }

    return NULL;
}

/**@brief Release ping reply context.
 *
 * @param p_reply Pointer to the reply context structure to release.
 */
zb_void_t ping_release_reply(ping_reply_t * p_reply)
{
    if (p_reply != NULL)
    {
        ZB_MEMSET(p_reply, 0x00, sizeof(ping_reply_t));
    }
}

/**@brief Invalidate Ping Request row after the timeout - ZBOSS callback
 *
 * @param row     Number of row to invalidate
 */
static zb_void_t invalidate_row_cb(zb_uint8_t row)
{
    ping_request_t * p_request = &(m_ping_request_table[row]);
    uint32_t         delay_us = get_request_duration(p_request);

    /* Inform user about timeout event. */
    if (p_request->p_cb)
    {
        p_request->p_cb(PING_EVT_FRAME_TIMEOUT, delay_us, p_request);
    }

    zb_ping_release_request(p_request);
}

/**@brief Get the first row with request sent to addr_short, return NULL if none
 *
 * @param addr_short  Short network address to look for.
 *
 * @return  Pointer to the ping request context.
 */
static ping_request_t * find_request_by_short(zb_uint16_t addr_short)
{
    int i;

    for (i = 0; i < PING_TABLE_SIZE; i++)
    {
        if (m_ping_request_table[i].taken == ZB_TRUE)
        {
            if (m_ping_request_table[i].remote_addr_mode == ZB_APS_ADDR_MODE_16_ENDP_PRESENT)
            {
                if (m_ping_request_table[i].remote_addr.addr_short == addr_short)
                {
                    return &(m_ping_request_table[i]);
                }
            }
            else
            {
                if (zb_address_short_by_ieee(m_ping_request_table[i].remote_addr.addr_long) == addr_short)
                {
                    return &(m_ping_request_table[i]);
                }
            }
        }
    }

    return NULL;
}

/**@brief Get the taken row with the selected seq_num, return NULL if none
 *
 * @param seqnum Sequence Number to look for
 */
static ping_request_t * find_request_by_sn(zb_uint8_t seqnum)
{
    int i;

    for (i = 0; i < PING_TABLE_SIZE; i++)
    {
        if (m_ping_request_table[i].taken == ZB_TRUE)
        {
            if (m_ping_request_table[i].ping_seq == seqnum)
            {
                return &m_ping_request_table[i];
            }
        }
    }

    return NULL;
}

/**@brief Get row number for the ping request.
 *
 * @param[in] p_row Pointer to the ping request context.
 *
 * @return Row number, -1 if not found.
 */
static zb_int8_t get_request_row(ping_request_t * p_request)
{
    if (p_request != NULL)
    {
        return (p_request - m_ping_request_table);
    }

    return -1;
}

/**@brief Get current abs time.
 */
static time_abs_t abs_time_now(void)
{
    time_abs_t now;
    now.time_zb = ZB_TIMER_GET();
    now.time_tim = nrf_drv_timer_capture(zb_nrf_cfg_get_zboss_timer(), NRF_TIMER_CC_CHANNEL2);
    return now;
}

/**@brief Get time difference, in microseconds between ping request identified
 *        by row number and current time.
 *
 * @param[in] p_row  Pointer to the ping request structure, from which the time difference should be calculated.
 *
 * @return  Time difference in microseconds.
 */
static zb_uint32_t get_request_duration(ping_request_t * p_request)
{
    uint32_t time_diff;

    /* Calculate the time difference between request being sent and reply being received */
    zb_uint32_t recv_tim = nrf_drv_timer_capture(zb_nrf_cfg_get_zboss_timer(), NRF_TIMER_CC_CHANNEL2);
    zb_time_t   recv_zb  = ZB_TIMER_GET();
    zb_uint32_t sent_tim = p_request->sent.time_tim;
    zb_time_t   sent_zb  = p_request->sent.time_zb;
    time_diff = (ZB_TIME_BEACON_INTERVAL_TO_USEC(ZB_TIME_SUBTRACT(recv_zb, sent_zb)) +
                 recv_tim) - sent_tim;

    return time_diff;
}

/**@brief Default handler for incoming ping request APS acknowledgments.
 *
 * @details  If there is a user callback defined for the acknowledged request,
 *           the callback with PING_EVT_ACK_RECEIVED event will be called.
 *
 * @param[in] param  Reference to a ZBoss buffer containing APC ACK data,
 */
static zb_void_t dispatch_user_callback(zb_uint8_t param)
{
    zb_uint16_t                    short_addr;
    zb_buf_t                     * p_asdu;
    zb_zcl_command_send_status_t * p_cmd_ping_status;
    zb_ret_t                       zb_err_code = RET_OK;
    ping_request_t               * p_request = NULL;

    if (param == 0)
    {
        return;
    }

    p_asdu            = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    p_cmd_ping_status = ZB_GET_BUF_PARAM(p_asdu, zb_zcl_command_send_status_t);

    if (p_cmd_ping_status->dst_addr.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
    {
        short_addr = p_cmd_ping_status->dst_addr.u.short_addr;
    }
    else if (p_cmd_ping_status->dst_addr.addr_type == ZB_ZCL_ADDR_TYPE_IEEE)
    {
        short_addr = zb_address_short_by_ieee(p_cmd_ping_status->dst_addr.u.ieee_addr);
    }
    else
    {
        NRF_LOG_INST_ERROR(m_log.p_log, "Ping request acknowledged with an unknown destination address type: %d", p_cmd_ping_status->dst_addr.addr_type);
        ZB_FREE_BUF_BY_REF(param);
        return;
    }

    p_request = find_request_by_short(short_addr);

    if (p_request != NULL)
    {
        uint32_t delay_us = get_request_duration(p_request);

        if (p_cmd_ping_status->status == RET_OK)
        {
            /* Inform user about ACK reception. */
            if (p_request->p_cb)
            {
                if (p_request->request_ack == 0)
                {
                    p_request->p_cb(PING_EVT_FRAME_SENT, delay_us, p_request);
                }
                else
                {
                    p_request->p_cb(PING_EVT_ACK_RECEIVED, delay_us, p_request);
                }
            }

            /* If only ACK was requested, cancel ongoing alarm. */
            if (p_request->request_echo == 0)
            {
                zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(invalidate_row_cb,
                                                       get_request_row(p_request));
                if (zb_err_code == RET_OK)
                {
                    zb_ping_release_request(p_request);
                }
            }
        }
        else
        {
            NRF_LOG_INST_ERROR(m_log.p_log, "Ping request returned error status: %d",
                                             p_cmd_ping_status->status);
        }
    }
    else
    {
        NRF_LOG_INST_WARNING(m_log.p_log, "Unknown ping command callback called with status: %d",
                             p_cmd_ping_status->status);
    }

    ZB_FREE_BUF_BY_REF(param);
}

/**@brief  Default ping event handler. Prints out measured time on the CLI and exits.
 *
 * @param[in] evt_type  Type of received  ping acknowledgment
 * @param[in] delay_us  Time, in microseconds, between ping request and the event.
 * @param[in] p_request Pointer to the ongoing ping request context structure.
 */
static void ping_cli_evt_handler(ping_time_evt_t evt, zb_uint32_t delay_us, ping_request_t * p_request)
{
    switch (evt)
    {
        case PING_EVT_FRAME_SCHEDULED:
            break;

        case PING_EVT_FRAME_TIMEOUT:
            nrf_cli_fprintf(p_request->p_cli,
                            NRF_CLI_ERROR,
                            "Error: request timed out after %ld ms.\r\n", delay_us/1000);
            break;

        case PING_EVT_ECHO_RECEIVED:
            nrf_cli_fprintf(p_request->p_cli,
                            NRF_CLI_NORMAL,
                            "Ping time: %ld ms\r\n", delay_us/1000);
            print_done(p_request->p_cli, ZB_FALSE);
            break;

        case PING_EVT_ACK_RECEIVED:
            if (p_request->request_echo == 0)
            {
                nrf_cli_fprintf(p_request->p_cli,
                                NRF_CLI_NORMAL, "Ping time: %ld ms\r\n",
                                delay_us/1000);
                print_done(p_request->p_cli, ZB_FALSE);
            }
            break;

        case PING_EVT_FRAME_SENT:
            if ((p_request->request_echo == 0) &&
                (p_request->request_ack  == 0))
            {
                print_done(p_request->p_cli, ZB_FALSE);
            }
            break;

        case PING_EVT_ERROR:
            print_error(p_request->p_cli, "Unable to send ping request");
            break;

        default:
          NRF_LOG_INST_ERROR(m_log.p_log, "Unknown ping event received: %d", evt);
          break;
    }
}

void zb_ping_set_ping_indication_cb(ping_time_cb_t p_cb)
{
    mp_ping_ind_cb = p_cb;
}

zb_void_t ping_request_send(ping_request_t * p_request)
{
    zb_uint8_t   cli_ep = zb_get_cli_endpoint();
    zb_ret_t     zb_err_code;
    zb_buf_t   * p_buf;
    zb_uint8_t * p_cmd_buf;

    if (p_request->count > PING_MAX_LENGTH)
    {
        if (p_request->p_cb)
        {
            p_request->p_cb(PING_EVT_ERROR, 0, p_request);
        }

        return;
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        if (p_request->p_cb)
        {
            p_request->p_cb(PING_EVT_ERROR, 0, p_request);
        }
        return;
    }

    /* We're constructing the Ping Frame by 'overloading' the common ZCL frame.
     * Basically every frame which comes addressed to the PING_CUSTOM_CLUSTER
     * is considered a Ping Frame. The FCF is being set to 0x00, the sequence
     * number field is being used as a Ping Sequence Number, while the Command
     * field is used to distinguish request/reply. The farther payload of the
     * ping is filled with bytes PING_ECHO_REQUEST_BYTE/PING_ECHO_REPLY_BYTE.
     */
    p_cmd_buf = ZB_ZCL_START_PACKET(p_buf);
    *(p_cmd_buf++) = ZIGBEE_PING_FRAME_CONTROL_FIELD;
    *(p_cmd_buf++) = m_ping_seq_num; /* Sequence Number Field */

    /* Fill Command Field */
    if ((p_request->request_echo) && (p_request->request_ack))
    {
        *(p_cmd_buf++) = PING_ECHO_REQUEST;
    }
    else if ((p_request->request_echo) && (!p_request->request_ack))
    {
        *(p_cmd_buf++) = PING_ECHO_NO_ACK_REQUEST;
    }
    else
    {
        *(p_cmd_buf++) = PING_NO_ECHO_REQUEST;
    }

    memset(p_cmd_buf, PING_ECHO_REQUEST_BYTE, p_request->count);
    p_cmd_buf += p_request->count;
    p_request->ping_seq = m_ping_seq_num;
    m_ping_seq_num++;

    /* Capture the sending time */
    p_request->sent = abs_time_now();

    /* Actually send the frame */
    if (p_request->request_ack)
    {
        zb_err_code = zb_zcl_finish_and_send_packet(p_buf, p_cmd_buf, &(p_request->remote_addr),
                                                    p_request->remote_addr_mode, cli_ep, cli_ep,
                                                    ZB_AF_HA_PROFILE_ID, PING_CUSTOM_CLUSTER,
                                                    dispatch_user_callback);
        ZB_ERROR_CHECK(zb_err_code);
    }
    else
    {
        zb_zcl_finish_and_send_packet_no_ack(p_buf, p_cmd_buf, &(p_request->remote_addr),
                                             p_request->remote_addr_mode, cli_ep, cli_ep,
                                             ZB_AF_HA_PROFILE_ID, PING_CUSTOM_CLUSTER,
                                             dispatch_user_callback);
    }

    if (p_request->p_cb)
    {
        uint32_t time_diff = get_request_duration(p_request);
        p_request->p_cb(PING_EVT_FRAME_SCHEDULED, time_diff, p_request);
    }

    zb_err_code = ZB_SCHEDULE_ALARM(invalidate_row_cb,
                                    get_request_row(p_request),
                                    ZB_MILLISECONDS_TO_BEACON_INTERVAL(p_request->timeout_ms));
    ZB_ERROR_CHECK(zb_err_code);
}

/**@brief Actually construct the Ping Reply frame and send it.
 *
 * @param p_row  Pointer to the ping reply context structure.
 */
static zb_void_t ping_reply_send(ping_reply_t * p_reply)
{
    zb_buf_t   * p_buf;
    zb_uint8_t * p_cmd_buf;
    zb_uint8_t   cli_ep = zb_get_cli_endpoint();
    zb_ret_t     zb_err_code;

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        NRF_LOG_INST_WARNING(m_log.p_log, "Drop ping request due to the lack of output buffers");
        ping_release_reply(p_reply);
        return;
    }

    NRF_LOG_INST_DEBUG(m_log.p_log, "Send ping reply");
    p_cmd_buf = ZB_ZCL_START_PACKET(p_buf);
    *(p_cmd_buf++) = ZIGBEE_PING_FRAME_CONTROL_FIELD;
    *(p_cmd_buf++) = p_reply->ping_seq;
    *(p_cmd_buf++) = PING_ECHO_REPLY;
    memset(p_cmd_buf, PING_ECHO_REPLY_BYTE, p_reply->count);
    p_cmd_buf += p_reply->count;

    /* Actually send the frame */
    if (p_reply->send_ack)
    {
        zb_err_code = zb_zcl_finish_and_send_packet(p_buf, p_cmd_buf, (zb_addr_u *)(&(p_reply->remote_short_addr)),
                                                    ZB_APS_ADDR_MODE_16_ENDP_PRESENT, cli_ep, cli_ep,
                                                    ZB_AF_HA_PROFILE_ID, PING_CUSTOM_CLUSTER,
                                                    frame_acked_cb);
        ZB_ERROR_CHECK(zb_err_code);
    }
    else
    {
        zb_zcl_finish_and_send_packet_no_ack(p_buf, p_cmd_buf, (zb_addr_u *)(&(p_reply->remote_short_addr)),
                                             ZB_APS_ADDR_MODE_16_ENDP_PRESENT, cli_ep, cli_ep,
                                             ZB_AF_HA_PROFILE_ID, PING_CUSTOM_CLUSTER,
                                             frame_acked_cb);
    }

    /* We don't need the row in this table anymore, since we're not expecting
     * any reply to a Ping Reply. */
    ping_release_reply(p_reply);
}

/**@brief Indicate ping request reception.
 *
 * @param p_zcl_cmd_buf  Pointer to the received ZCL packet.
 */
static void ping_req_indicate(zb_buf_t * p_zcl_cmd_buf)
{
    ping_request_t        tmp_request;
    zb_zcl_parsed_hdr_t * p_cmd_info = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);

    if (mp_ping_ind_cb == NULL)
    {
        return;
    }

    memset(&tmp_request, 0, sizeof(ping_request_t));

    switch (p_cmd_info->cmd_id)
    {
        case PING_ECHO_REQUEST:
            tmp_request.request_echo = 1;
            tmp_request.request_ack  = 1;
            break;

        case PING_ECHO_NO_ACK_REQUEST:
            tmp_request.request_echo = 1;
            break;

        case PING_NO_ECHO_REQUEST:
            break;

        default:
            // Received frame is not a ping request.
            return;
    }

    tmp_request.taken    = ZB_TRUE;
    tmp_request.ping_seq = p_cmd_info->seq_number;
    tmp_request.count    = ZB_BUF_LEN(p_zcl_cmd_buf);
    tmp_request.sent     = abs_time_now();

    if (p_cmd_info->addr_data.common_data.source.addr_type != ZB_ZCL_ADDR_TYPE_SHORT)
    {
        NRF_LOG_INFO("Ping request received, but indication will not be generated due to the unsupported address type.")
        return; // Not supported.
    }
    tmp_request.remote_addr_mode       = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    tmp_request.remote_addr.addr_short = p_cmd_info->addr_data.common_data.source.u.short_addr;

    mp_ping_ind_cb(PING_EVT_REQUEST_RECEIVED,
                   ZB_TIME_BEACON_INTERVAL_TO_USEC(tmp_request.sent.time_zb),
                   &tmp_request);
}

/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param    Reference to a ZBoss buffer
 */
static zb_uint8_t cli_agent_ep_handler_ping(zb_uint8_t param)
{
    zb_buf_t * p_zcl_cmd_buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    zb_zcl_parsed_hdr_t * p_cmd_info = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    zb_uint32_t time_diff;

    if (p_cmd_info->cluster_id != PING_CUSTOM_CLUSTER ||
        p_cmd_info->profile_id != ZB_AF_HA_PROFILE_ID)
    {
        return ZB_FALSE;
    }

    NRF_LOG_INST_DEBUG(m_log.p_log, "New ping frame received, param: %d", param);
    ping_req_indicate(p_zcl_cmd_buf);

    if (p_cmd_info->cmd_id == PING_ECHO_REPLY)
    {
        zb_uint16_t remote_short_addr = 0x0000;

        /* We have our ping reply */
        ping_request_t * p_request = find_request_by_sn(p_cmd_info->seq_number);
        if (p_request == NULL)
        {
            return ZB_FALSE;
        }

        if (p_request->remote_addr_mode == ZB_APS_ADDR_MODE_16_ENDP_PRESENT)
        {
            remote_short_addr = p_request->remote_addr.addr_short;
        }
        else
        {
            remote_short_addr = zb_address_short_by_ieee(p_request->remote_addr.addr_long);
        }

        if (p_cmd_info->addr_data.common_data.source.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
        {
            if (remote_short_addr != p_cmd_info->addr_data.common_data.source.u.short_addr)
            {
                return ZB_FALSE;
            }
        }
        else
        {
            return ZB_FALSE;
        }

        /* Catch the timers value */
        time_diff = get_request_duration(p_request);

        /* Cancel the ongoing alarm which was to erase the row... */
        zb_ret_t zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(invalidate_row_cb,
                                                        get_request_row(p_request));
        ZB_ERROR_CHECK(zb_err_code);

        /* Call callback function in order to indicate echo response reception. */
        if (p_request->p_cb)
        {
            p_request->p_cb(PING_EVT_ECHO_RECEIVED, time_diff, p_request);
        }

        /* ...and erase it manually */
        if (zb_err_code == RET_OK)
        {
            zb_ping_release_request(p_request);
        }

    }
    else if ((p_cmd_info->cmd_id == PING_ECHO_REQUEST) ||
             (p_cmd_info->cmd_id == PING_ECHO_NO_ACK_REQUEST))
    {
        zb_uint8_t     len = ZB_BUF_LEN(p_zcl_cmd_buf);
        ping_reply_t * p_reply = ping_aquire_reply();

        if (p_reply == NULL)
        {
            NRF_LOG_INST_WARNING(m_log.p_log, "Cannot obtain new row for incoming ping request");
            return ZB_FALSE;
        }

        /* Save the Ping Reply information in the table and schedule a sending function */
        p_reply->count = len;
        p_reply->ping_seq = p_cmd_info->seq_number;

        if (p_cmd_info->cmd_id == PING_ECHO_REQUEST)
        {
            NRF_LOG_INST_DEBUG(m_log.p_log,
                               "PING echo request with APS ACK received");
            p_reply->send_ack = 1;
        }
        else
        {
            NRF_LOG_INST_DEBUG(m_log.p_log,
                               "PING echo request without APS ACK received");
            p_reply->send_ack = 0;
        }

        if (p_cmd_info->addr_data.common_data.source.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
        {
            p_reply->remote_short_addr = p_cmd_info->addr_data.common_data.source.u.short_addr;
        }
        else
        {
            NRF_LOG_INST_WARNING(m_log.p_log, "Drop ping request due to incorrect address type");
            ping_release_reply(p_reply);
            ZB_FREE_BUF(p_zcl_cmd_buf);
            return ZB_TRUE;
        }

        /* Send the Ping Reply, if not possible then invalidate the row */
        ping_reply_send(p_reply);
    }
    else if (p_cmd_info->cmd_id == PING_NO_ECHO_REQUEST)
    {
        NRF_LOG_INST_DEBUG(m_log.p_log,
                          "PING request without ECHO received");
    }
    else
    {
        NRF_LOG_INST_WARNING(m_log.p_log,
                             "Unsupported Ping message received, cmd_id %d\r\n",
                             p_cmd_info->cmd_id);
    }

    ZB_FREE_BUF(p_zcl_cmd_buf);
    return ZB_TRUE;
}

/** @brief ping over ZCL
 *
 * @code
 * zcl ping [--no-echo] [--aps-ack] <h:dst_addr> <d:payload size>
 * @endcode
 *
 * Example:
 * @code
 * zcl ping 0b010eaafd745dfa 32
 * @endcode
 *
 * @pre Ping only after starting @ref zigbee.
 *
 * Issue a ping-style command to another CLI device of the address `dst_addr`
 * by using `payload_size` bytes of payload.<br>
 *
 * Optionally, the device can request an APS acknowledgment (`--aps-ack`) or
 * ask destination not to sent ping reply (`--no-echo`).<br>
 *
 * To implement the ping-like functionality, a new custom cluster has been
 * defined with ID 64. There are four custom commands defined inside it,
 * each with its own ID.
 *
 * See the following flow graphs for details.
 *
 * - <b>Case 1:</b> Ping with echo, without the APS acknowledgment (default mode):
 *   @code
 *       App 1          Node 1                 Node 2
 *         |  -- ping ->  |  -- ping request ->  |   (command ID: 0x02 - ping request without the APS acknowledgment)
 *         |              |    <- MAC ACK --     |
 *         |              | <- ping reply --     |   (command ID: 0x01 - ping reply)
 *         |              |    -- MAC ACK ->     |
 *         |  <- Done --  |                      |
 *   @endcode
 *
 *   In this default mode, the `ping` command measures the time needed for a Zigbee frame to travel between two nodes in the network (there and back again). The command uses a custom "overloaded" ZCL frame, which is constructed as a ZCL frame of the new custom ping ZCL cluster (ID 64).
 *
 * - <b>Case 2:</b> Ping with echo, with the APS acknowledgment:
 *     @code
 *       App 1          Node 1                 Node 2
 *         |  -- ping ->  |  -- ping request ->  |   (command ID: 0x00 - ping request with the APS acknowledgment)
 *         |              |    <- MAC ACK --     |
 *         |              |    <- APS ACK --     |
 *         |              |    -- MAC ACK ->     |
 *         |              | <- ping reply --     |   (command ID: 0x01 - ping reply)
 *         |              |    -- MAC ACK ->     |
 *         |              |    -- APS ACK ->     |
 *         |              |    <- MAC ACK --     |
 *         |  <- Done --  |                      |
 *     @endcode
 *
 * - <b>Case 3:</b> Ping without echo, with the APS acknowledgment:
 *     @code
 *       App 1          Node 1                 Node 2
 *         |  -- ping ->  |  -- ping request ->  |   (command ID: 0x03 - ping request without echo)
 *         |              |    <- MAC ACK --     |
 *         |              |    <- APS ACK --     |
 *         |              |    -- MAC ACK ->     |
 *         |  <- Done --  |                      |
 *     @endcode
 *
 * - <b>Case 4:</b> Ping without echo, without the APS acknowledgment:
 *     @code
 *       App 1          Node 1                 Node 2
 *         |  -- ping ->  |  -- ping request ->  |   (command ID: 0x03 - ping request without echo)
 *         |  <- Done --  |                      |
 *         |              |    <- MAC ACK --     |
 *     @endcode
 */
void cmd_zb_ping(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    ping_request_t * p_row;
    uint8_t          i;

    if (nrf_cli_help_requested(p_cli) || (argc == 1))
    {
        print_usage(p_cli, argv[0],
                    "[--no-echo] [--aps-ack] <h:addr> <d:payload size>");
        return;
    }

    if ((argc > 5) || (argc < 3))
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong number of arguments\r\n");
        return;
    }

    p_row = zb_ping_acquire_request();
    if (p_row == NULL)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: request pool empty - wait a bit\r\n");
        return;
    }

    p_row->p_cb         = ping_cli_evt_handler;
    p_row->request_ack  = 0;
    p_row->request_echo = 1;
    p_row->timeout_ms   = PING_ECHO_REQUEST_TIMEOUT_S * 1000;

    for (i = 1; i < (argc - 2); i++)
    {
        if (strcmp(argv[i], "--aps-ack") == 0)
        {
            p_row->request_ack = 1;
        }
        else if (strcmp(argv[i], "--no-echo") == 0)
        {
            p_row->request_echo = 0;
        }
    }

    p_row->remote_addr_mode = parse_address(argv[argc - 2], &(p_row->remote_addr), ADDR_ANY);
    if (p_row->remote_addr_mode == ADDR_INVALID)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Error: wrong address format\r\n");
        zb_ping_release_request(p_row);
        return;
    }

    if (!sscanf(argv[argc - 1], "%hd", &(p_row->count)))
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Error: incorrect ping payload size\r\n");
        zb_ping_release_request(p_row);
        return;
    }

    if (p_row->count > PING_MAX_LENGTH)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Note: Ping payload size exceeds maximum possible, assuming maximum\r\n");
        p_row->count = PING_MAX_LENGTH;
    }

    /* Put the CLI instance to be used later */
    p_row->p_cli = (nrf_cli_t*)p_cli;

    ping_request_send(p_row);
}


/**@brief Endpoint handlers
 */
NRF_ZIGBEE_EP_HANDLER_REGISTER(ping, cli_agent_ep_handler_ping);

/** @} */
