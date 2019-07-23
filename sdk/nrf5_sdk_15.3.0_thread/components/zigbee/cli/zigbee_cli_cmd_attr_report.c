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
#include "nrf_cli.h"
#include "zboss_api.h"
#include "zb_error_handler.h"
#include "zigbee_cli.h"
#include "nrf_log_instance.h"
#include "nrf_log.h"
#include "zigbee_cli_utils.h"

/**
 * @defgroup zb_cli_cmd_zcl ZCL commands
 * @ingroup zb_cli
 *
 * @{
 */

// Defines how many report attribute requests can be run concurrently.
#define ZIGBEE_CLI_CONFIGURE_REPORT_TSN                  3
// Defines how long to wait, in seconds, for Configure Reporting Response.
#define ZIGBEE_CLI_CONFIGURE_REPORT_RESP_TIMEOUT         5

// Defines default value for minimum interval inside configure reporting request.
#define ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_MIN_INTERVAL 1
// Defines default value for maximum interval inside configure reporting request.
#define ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_MAX_INTERVAL 60
// Defines default value for minimum value change inside configure reporting request.
#define ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_VALUE_CHANGE NULL

// Defines default value for minimum interval configured in order to turn off reporting. See ZCL specification, sec. 2.5.7.1.5.
#define ZIGBEE_CLI_CONFIGURE_REPORT_OFF_MIN_INTERVAL 0x000F // This can be any value, only max_interval parameters is relevant.
// Defines default value for maximum interval inside configure reporting request. See ZCL specification, sec. 2.5.7.1.6.
#define ZIGBEE_CLI_CONFIGURE_REPORT_OFF_MAX_INTERVAL 0xFFFF

#if NRF_LOG_ENABLED
/** @brief Name of the submodule used for logger messaging.
 */
#define NRF_LOG_SUBMODULE_NAME report

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

// This structure allows for binding ZBOSS transaction and CLI object.
typedef struct {
    nrf_cli_t const * p_cli;
    uint8_t           tsn;
    bool              taken;
    bool              is_broadcast;
} tsn_ctx_t;

// This structure representing all fields required to construct configure reporting requests.
typedef struct {
    zb_uint16_t profile_id;
    zb_uint16_t cluster_id;
    zb_uint16_t attr_id;
    zb_uint8_t  attr_type;
    zb_uint16_t interval_min;
    zb_uint16_t interval_max;
    zb_addr_u   remote_node;
    addr_type_t remote_addr_mode;
    zb_uint8_t  remote_ep;
} configure_reporting_req_t;


#if NRF_LOG_ENABLED
// Logger instance used by this module.
static log_ctx_t m_log = {
    NRF_LOG_INSTANCE_PTR_INIT(p_log, ZIGBEE_CLI_LOG_NAME, NRF_LOG_SUBMODULE_NAME)
};
#endif // NRF_LOG_ENABLED

static tsn_ctx_t m_tsn_ctx[ZIGBEE_CLI_CONFIGURE_REPORT_TSN];


/**
 * @brief Return a pointer to context with the given transaction sequence number.
 *
 * @param[in] tsn ZBOSS transaction sequence number
 *
 * @return a pointer to context or NULL if context for given TSN wasn't found.
 */
static tsn_ctx_t * get_ctx_by_tsn(uint8_t tsn)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(m_tsn_ctx); i++)
    {
        if ((m_tsn_ctx[i].taken == true) && (m_tsn_ctx[i].tsn == tsn))
        {
            return &m_tsn_ctx[i];
        }
    }

    return NULL;
}

/**
 * @brief Get a pointer to a free context.
 *
 * @return a pointer to context structure or NULL if all contexts are taken.
 */
static tsn_ctx_t * get_free_tsn_ctx(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(m_tsn_ctx); i++)
    {
        if (!m_tsn_ctx[i].taken)
        {
            return &m_tsn_ctx[i];
        }
    }

    return NULL;
}

/**
 * Invalidate context.
 *
 * @param[in] p_tsn_ctx a pointer to transaction context.
 */
static void invalidate_ctx(tsn_ctx_t * p_tsn_ctx)
{
    p_tsn_ctx->taken = false;
    p_tsn_ctx->tsn   = 0xFF;
    p_tsn_ctx->p_cli = NULL;
}

/**
 * @brief Handles timeout error and invalidates configure reporting transaction.
 *
 * @param[in] param ZBOSS transaction sequence number
 */
static void cmd_zb_subscribe_unsubscribe_timeout(zb_uint8_t param)
{
    tsn_ctx_t * p_tsn_ctx = get_ctx_by_tsn(param);

    if (!p_tsn_ctx)
    {
        return;
    }

    print_error(p_tsn_ctx->p_cli, "Request timed out");
    invalidate_ctx(p_tsn_ctx);
}

/**@brief Print the Configure Reporting Response
 *
 * @param p_zcl_cmd_buf[in]  Pointer to a ZBOSS buffer
 * @param p_row[in]          Pointer to a row in attr table
 */
static void cmd_zb_subscribe_unsubscribe_cb(tsn_ctx_t * p_tsn_ctx, zb_uint8_t param)
{
    zb_ret_t                           zb_err_code;
    zb_buf_t                         * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zcl_configure_reporting_res_t * p_resp = NULL;
    zb_bool_t                          failed = ZB_FALSE;

    zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(cmd_zb_subscribe_unsubscribe_timeout, ZB_ALARM_ANY_PARAM);
    if (zb_err_code != RET_OK)
    {
        print_error(p_tsn_ctx->p_cli, "Unable to cancel timeout timer");
        goto free_tsn_ctx;
    }

    // Check if response contains only status code.
    if (sizeof(zb_zcl_configure_reporting_res_t) > ZB_BUF_LEN(p_buf))
    {
        p_resp = (zb_zcl_configure_reporting_res_t*)ZB_BUF_BEGIN(p_buf);
        if (p_resp->status == ZB_ZCL_STATUS_SUCCESS)
        {
            print_done(p_tsn_ctx->p_cli, ZB_FALSE);
        }
        else
        {
            nrf_cli_fprintf(p_tsn_ctx->p_cli,
                            NRF_CLI_ERROR,
                            "Error: Unable to configure reporting. Status: %d\r\n",
                            p_resp->status);
        }
        goto free_tsn_ctx;
    }


    // Received a full Configure Reporting Response frame.
    ZB_ZCL_GENERAL_GET_NEXT_CONFIGURE_REPORTING_RES(p_buf, p_resp);
    if (p_resp == NULL)
    {
        print_error(p_tsn_ctx->p_cli, "Unable to parse configure reporting response");
        goto free_tsn_ctx;
    }


    while (p_resp != NULL)
    {
        if (p_resp->status == ZB_ZCL_STATUS_SUCCESS)
        {
            switch (p_resp->direction)
            {
                case ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT:
                    nrf_cli_fprintf(p_tsn_ctx->p_cli, NRF_CLI_NORMAL,
                                    "Local subscription to attribute ID %hx updated\r\n",
                                    p_resp->attr_id);
                    break;

                case ZB_ZCL_CONFIGURE_REPORTING_RECV_REPORT:
                    nrf_cli_fprintf(p_tsn_ctx->p_cli, NRF_CLI_NORMAL,
                                    "Remote node subscription to receive attribute ID %hx updated\r\n",
                                    p_resp->attr_id);
                    break;

                default:
                    nrf_cli_fprintf(p_tsn_ctx->p_cli,
                                    NRF_CLI_ERROR,
                                    "Unknown reporting configuration direction for attribute %hx",
                                    p_resp->attr_id);
                    failed = ZB_TRUE;
                    break;
            }
        }
        else
        {
            nrf_cli_fprintf(p_tsn_ctx->p_cli,
                            NRF_CLI_ERROR,
                            "Unable to configure attribute %hx reporting. Status: %hd\r\n",
                            p_resp->attr_id,
                            p_resp->status);
            failed = ZB_TRUE;
        }
        ZB_ZCL_GENERAL_GET_NEXT_CONFIGURE_REPORTING_RES(p_buf, p_resp);
    }

    if (failed == ZB_TRUE)
    {
        print_error(p_tsn_ctx->p_cli, "One or more attributes reporting were not configured successfully");
    }
    else
    {
        print_done(p_tsn_ctx->p_cli, ZB_FALSE);
    }

free_tsn_ctx:
    invalidate_ctx(p_tsn_ctx);
    ZB_FREE_BUF_BY_REF(param);
}

/**@brief Print the Report Attribute Command
 *
 * @param p_zcl_cmd_buf[in]  Pointer to a ZBOSS buffer
 * @param p_row[in]          Pointer to a row in attr table
 */
static void print_attr_update(zb_zcl_parsed_hdr_t * p_zcl_hdr, zb_uint8_t param)
{
    zb_buf_t                 * p_buf         = ZB_BUF_FROM_REF(param);
    zb_zcl_report_attr_req_t * p_attr_resp   = NULL;
    int                        bytes_written = 0;
    char                       print_buf[255];

    if (p_zcl_hdr->addr_data.common_data.source.addr_type == ZB_ZCL_ADDR_TYPE_SHORT)
    {
        NRF_LOG_INST_INFO(m_log.p_log, "Received value updates from the remote node 0x%04x",
                          p_zcl_hdr->addr_data.common_data.source.u.short_addr);
    }
    else
    {
        bytes_written = ieee_addr_to_str(print_buf, sizeof(print_buf), p_zcl_hdr->addr_data.common_data.source.u.ieee_addr);
        if (bytes_written < 0)
        {
            NRF_LOG_INST_INFO(m_log.p_log, "Received value updates from the remote node (unknown address)");
        }
        else
        {
            NRF_LOG_INST_INFO(m_log.p_log, "Received value updates from the remote node 0x%s", nrf_log_push(print_buf));
        }
    }

    /* Get the contents of Read Attribute Response frame */
    ZB_ZCL_GENERAL_GET_NEXT_REPORT_ATTR_REQ(p_buf, p_attr_resp);
    bytes_written = 0;
    while (p_attr_resp != NULL)
    {
        bytes_written = zcl_attr_to_str(&print_buf[bytes_written],
                                        sizeof(print_buf) - bytes_written,
                                        p_attr_resp->attr_type,
                                        p_attr_resp->attr_value);

        if (bytes_written < 0)
        {
            NRF_LOG_ERROR("    Unable to print updated attribute value");
        }
        else
        {
            NRF_LOG_INST_INFO(m_log.p_log, "    Profile: 0x%04x Cluster: 0x%04x Attribute: 0x%04x Type: %hu Value: %s",
                p_zcl_hdr->profile_id, p_zcl_hdr->cluster_id, p_attr_resp->attr_id,
                p_attr_resp->attr_type, nrf_log_push(print_buf));
        }

        ZB_ZCL_GENERAL_GET_NEXT_REPORT_ATTR_REQ(p_buf, p_attr_resp);
    }
}

/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param[in]  Reference to a ZBOSS buffer
 *
 * @returns ZB_TRUE if ZCL command was processed.
 */
static zb_uint8_t cli_agent_ep_handler_report(zb_uint8_t param)
{
    zb_buf_t            * p_zcl_cmd_buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    zb_zcl_parsed_hdr_t * p_cmd_info    = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    tsn_ctx_t           * p_tsn_ctx;

    if (p_cmd_info->cmd_id == ZB_ZCL_CMD_REPORT_ATTRIB)
    {
        print_attr_update(p_cmd_info, param);
        ZB_FREE_BUF_BY_REF(param);
        return ZB_TRUE;
    }
    else if (p_cmd_info->cmd_id == ZB_ZCL_CMD_CONFIG_REPORT_RESP)
    {
        // Find command context by ZCL sequence number.
        p_tsn_ctx = get_ctx_by_tsn(p_cmd_info->seq_number);
        if (p_tsn_ctx != NULL)
        {
            cmd_zb_subscribe_unsubscribe_cb(p_tsn_ctx, param);
            return ZB_TRUE;
        }
    }

    return ZB_FALSE;
}

/**@brief Subscribe to the attribute changes on the remote node.
 *
 * @code
 * zcl subscribe {on, off} <h:addr> <d:ep> <h:cluster> <h:profile>
 *                         <h:attr ID> <d:attr type>
 *                         [<d:min interval (s)>] [<d:max interval (s)>]
 * @endcode
 *
 * Enable or disable reporting on the node identified by `addr`, with the endpoint `ep`
 * that uses the profile `profile` of the attribute `attr ID` with the type
 * `attr type` in the cluster `cluster`.
 *
 * Reports must be generated in intervals not shorter than `min interval`
 * (1 second by default) and not longer than `max interval` (60 seconds by default).
 */
void cmd_zb_subscribe(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    configure_reporting_req_t   req;
    tsn_ctx_t                 * p_tsn_cli;
    zb_buf_t                  * p_buf;
    zb_uint8_t                * p_cmd_ptr;
    zb_ret_t                    zb_err_code;
    zb_bool_t                   subscribe;

    subscribe = (strcmp(argv[0], "on") == 0) ? ZB_TRUE : ZB_FALSE;

    if ((argc == 1) || (nrf_cli_help_requested(p_cli)))
    {
        print_usage(p_cli, argv[0],
                    "<h:addr> <d:ep> <h:cluster>\r\n"
                    "<h:profile> <h:attr ID> <d:attr type>\r\n"
                    "[<d:min interval (s)>] [<d:max interval (s)>]\r\n");
        return;
    }

    if ((((argc < 7) || (argc > 9)) && (subscribe == ZB_TRUE)) ||
        ((argc != 7) && (subscribe == ZB_FALSE)))
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    req.remote_addr_mode = parse_address(argv[1], &req.remote_node, ADDR_ANY);

    if (req.remote_addr_mode == ADDR_INVALID)
    {
        print_error(p_cli, "Invalid remote address");
        return;
    }

    if (!sscan_uint8(argv[2], &(req.remote_ep)))
    {
        print_error(p_cli, "Incorrect remote endpoint");
        return;
    }

    if (!sscanf(argv[3], "%hx", &(req.cluster_id)))
    {
        print_error(p_cli, "Incorrect cluster ID");
        return;
    }

    if (!sscanf(argv[4], "%hx", &(req.profile_id)))
    {
        print_error(p_cli, "Incorrect profile ID");
        return;
    }

    if (!sscanf(argv[5], "%hx", &(req.attr_id)))
    {
        print_error(p_cli, "Incorrect attribute ID");
        return;
    }

    if (!sscan_uint8(argv[6], &(req.attr_type)))
    {
        print_error(p_cli, "Incorrect attribyte type");
        return;
    }

    // Optional parameters parsing.
    if (subscribe == ZB_TRUE)
    {
        req.interval_min = ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_MIN_INTERVAL;
        req.interval_max = ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_MAX_INTERVAL;
    }
    else
    {
        req.interval_min = ZIGBEE_CLI_CONFIGURE_REPORT_OFF_MIN_INTERVAL;
        req.interval_max = ZIGBEE_CLI_CONFIGURE_REPORT_OFF_MAX_INTERVAL;
    }

    if (argc > 7)
    {
        if (!sscanf(argv[7], "%hu", &(req.interval_min)))
        {
            print_error(p_cli, "Incorrect minimum interval");
            return;
        }
    }

    if (argc > 8)
    {
        if (!sscanf(argv[8], "%hu", &(req.interval_max)))
        {
            print_error(p_cli, "Incorrect maximum interval");
            return;
        }
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        print_error(p_cli, "Failed to execute command (buf alloc failed)");
        return;
    }

    p_tsn_cli = get_free_tsn_ctx();
    if (!p_tsn_cli)
    {
        print_error(p_cli, "Too many configure reporting requests");
        ZB_FREE_BUF(p_buf);
        return;
    }

    // Configure new tsn context.
    p_tsn_cli->taken = true;
    p_tsn_cli->p_cli = p_cli;
    p_tsn_cli->tsn   = ZCL_CTX().seq_number;

    // Construct and send request.
    ZB_ZCL_GENERAL_INIT_CONFIGURE_REPORTING_SRV_REQ(p_buf,
                                                    p_cmd_ptr,
                                                    ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
    ZB_ZCL_GENERAL_ADD_SEND_REPORT_CONFIGURE_REPORTING_REQ(p_cmd_ptr,
        req.attr_id, req.attr_type, req.interval_min, req.interval_max,
        ZIGBEE_CLI_CONFIGURE_REPORT_DEFAULT_VALUE_CHANGE);
    ZB_ZCL_GENERAL_SEND_CONFIGURE_REPORTING_REQ(p_buf, p_cmd_ptr,
        req.remote_node, req.remote_addr_mode, req.remote_ep, zb_get_cli_endpoint(),
        req.profile_id, req.cluster_id, NULL);

    // Start timeout timer.
    zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_subscribe_unsubscribe_timeout, p_tsn_cli->tsn,
        ZIGBEE_CLI_CONFIGURE_REPORT_RESP_TIMEOUT * ZB_TIME_ONE_SECOND);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "Unable to schedule timeout timer");
        invalidate_ctx(p_tsn_cli);
    }
}

/**@brief Endpoint handlers
 */
NRF_ZIGBEE_EP_HANDLER_REGISTER(report, cli_agent_ep_handler_report);

/** @} */
