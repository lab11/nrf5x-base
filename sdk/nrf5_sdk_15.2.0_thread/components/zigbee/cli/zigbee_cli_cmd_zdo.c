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
#include <stdbool.h>
#include "nrf_cli.h"
#include "zboss_api.h"
#include "zb_error_handler.h"
#include "zigbee_cli.h"
#include "zigbee_cli_utils.h"

/**
 * @defgroup zb_cli_cmd_zdo ZDO commands
 * @ingroup zb_cli
 *
 * @{
 */

// Defines how many ZDO requests can be run concurrently.
#define ZIGBEE_CLI_ZDO_TSN                 3
// Defines how long to wait, in seconds, for Match Descriptor Response.
#define ZIGBEE_CLI_MATCH_DESC_RESP_TIMEOUT 1
// Defines how long to wait, in seconds, for Bind Response.
#define ZIGBEE_CLI_BIND_RESP_TIMEOUT       5
// Defines how long to wait, in seconds, for Network Addrees Response.
#define ZIGBEE_CLI_NWK_ADDR_RESP_TIMEOUT   5
// Defines how long to wait, in seconds, for IEEE (EUI64) Addrees Response.
#define ZIGBEE_CLI_IEEE_ADDR_RESP_TIMEOUT  5

// This structure allows for binding ZBOSS transaction and CLI object.
typedef struct {
    nrf_cli_t const * p_cli;
    uint8_t           tsn;
    bool              taken;
} tsn_ctx_t;

static tsn_ctx_t m_tsn_ctx[ZIGBEE_CLI_ZDO_TSN];

/**
 * @brief Return a pointer to context with the given transaction sequence number.
 *
 * @param[in] tsn ZBOSS transaction sequence number.
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
static tsn_ctx_t * get_free_ctx(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(m_tsn_ctx); i++)
    {
        if (!m_tsn_ctx[i].taken)
        {
            m_tsn_ctx[i].taken = true;

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
 * @brief Parse a list of cluster IDs.
 *
 * @param[in]  pp_argv Pointer to argument table.
 * @param[in]  num     Number of cluster IDs to scan.
 * @param[out] pp_id   Pointer to an array to store cluster IDs.
 *
 * @return 1 if parsing succeeded, 0 otherwise.
 *
 */
static int sscan_cluster_list(char ** pp_argv, uint8_t num, uint16_t * pp_id)
{
    uint16_t len = 0;

    while ((len < num) && sscanf(pp_argv[len], "%hd", pp_id))
    {
        len += 1;
        pp_id += 1;
    }

    return (len == num);
}

/**
 * @brief Prints 'Done' and invalidates transaction.
 *
 * @param[in] param ZBOSS transaction sequence number.
 */
static void cmd_zb_match_desc_timeout(zb_uint8_t param)
{
    tsn_ctx_t * p_tsn_ctx = get_ctx_by_tsn(param);
    print_done(p_tsn_ctx->p_cli, ZB_TRUE);
    invalidate_ctx(p_tsn_ctx);
}

/**
 * @brief A callback called on match descriptor response.
 *
 * @param[in] param Reference number to ZBOSS memory buffer.
 */
static void cmd_zb_match_desc_cb(zb_uint8_t param)
{
    zb_buf_t                   * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zdo_match_desc_resp_t   * p_resp = (zb_zdo_match_desc_resp_t *)ZB_BUF_BEGIN(p_buf);
    zb_apsde_data_indication_t * p_ind  = ZB_GET_BUF_PARAM(p_buf, zb_apsde_data_indication_t);
    tsn_ctx_t                  * p_tsn_ctx;
    zb_ret_t                     zb_err_code;

    p_tsn_ctx = get_ctx_by_tsn(p_ind->tsn);

    if (p_tsn_ctx && (p_resp->status == ZB_ZDP_STATUS_SUCCESS))
    {
        zb_uint8_t * p_match_ep = (zb_uint8_t *)(p_resp + 1);

        while (p_resp->match_len > 0)
        {
            /* Match EP list follows right after response header */
            nrf_cli_fprintf(p_tsn_ctx->p_cli,
                            NRF_CLI_NORMAL,
                            "src_addr=%0hx ep=%d\r\n",
                            p_ind->src_addr,
                            *p_match_ep);

            p_match_ep += 1;
            p_resp->match_len -= 1;
        }

        // Reschedule printing out 'Done'.
        zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(cmd_zb_match_desc_timeout, ZB_ALARM_ANY_PARAM);
        if (zb_err_code != RET_OK)
        {
            print_error(p_tsn_ctx->p_cli, "Unable to reschedule timeout timer - no buffer left");
        }
        else
        {
            zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_match_desc_timeout,
                                            p_tsn_ctx->tsn,
                                            ZIGBEE_CLI_MATCH_DESC_RESP_TIMEOUT*ZB_TIME_ONE_SECOND);
            if (zb_err_code != RET_OK)
            {
                print_error(p_tsn_ctx->p_cli, "Unable to schedule timeout timer - no buffer left");
                invalidate_ctx(p_tsn_ctx);
            }
        }
    }

    ZB_FREE_BUF_BY_REF(param);
}

/**
 * @brief Handles timeout error and invalidates binding transaction.
 *
 * @param[in] param ZBOSS transaction sequence number.
 */
static void cmd_zb_bind_unbind_timeout(zb_uint8_t param)
{
    tsn_ctx_t * p_tsn_ctx = get_ctx_by_tsn(param);

    if (!p_tsn_ctx)
    {
        return;
    }

    print_error(p_tsn_ctx->p_cli, "Bind/unbind request timed out");
    invalidate_ctx(p_tsn_ctx);
}

/**
 * @brief A callback called on bind/unbind response.
 *
 * @param[in] param Reference number to ZBOSS memory buffer.
 */
zb_void_t cmd_zb_bind_unbind_cb(zb_uint8_t param)
{
    zb_buf_t           * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zdo_bind_resp_t * p_resp = (zb_zdo_bind_resp_t *)ZB_BUF_BEGIN(p_buf);
    tsn_ctx_t          * p_tsn_ctx;
    zb_ret_t             zb_err_code;

    p_tsn_ctx = get_ctx_by_tsn(p_resp->tsn);
    if (!p_tsn_ctx)
    {
        ZB_FREE_BUF_BY_REF(param);
        return;
    }

    zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(cmd_zb_bind_unbind_timeout, ZB_ALARM_ANY_PARAM);
    if (zb_err_code != RET_OK)
    {
        print_error(p_tsn_ctx->p_cli, "Unable to cancel timeout timer");
    }

    if (p_resp->status == ZB_ZDP_STATUS_SUCCESS)
    {
        print_done(p_tsn_ctx->p_cli, ZB_TRUE);
    }
    else
    {
        nrf_cli_fprintf(p_tsn_ctx->p_cli,
                        NRF_CLI_ERROR,
                        "Error: Unable to modify binding. Status: %d\r\n",
                        p_resp->status);
    }

    invalidate_ctx(p_tsn_ctx);
    ZB_FREE_BUF_BY_REF(param);
}

/**
 * @brief Handles timeout error and invalidates network address request transaction.
 *
 * @param[in] param ZBOSS transaction sequence number.
 */
static void cmd_zb_nwk_addr_timeout(zb_uint8_t param)
{
    tsn_ctx_t * p_tsn_ctx = get_ctx_by_tsn(param);

    if (!p_tsn_ctx)
    {
        return;
    }

    print_error(p_tsn_ctx->p_cli, "Network address request timed out");
    invalidate_ctx(p_tsn_ctx);
}

/**
 * @brief A callback called on network address response.
 *
 * @param[in] param Reference number to ZBOSS memory buffer.
 */
zb_void_t cmd_zb_nwk_addr_cb(zb_uint8_t param)
{
    zb_buf_t                    * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zdo_nwk_addr_resp_head_t * p_resp = (zb_zdo_nwk_addr_resp_head_t *)ZB_BUF_BEGIN(p_buf);
    tsn_ctx_t                   * p_tsn_ctx;
    zb_ret_t                      zb_err_code;

    p_tsn_ctx = get_ctx_by_tsn(p_resp->tsn);
    if (!p_tsn_ctx)
    {
        ZB_FREE_BUF_BY_REF(param);
        return;
    }

    zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(cmd_zb_nwk_addr_timeout, ZB_ALARM_ANY_PARAM);
    if (zb_err_code != RET_OK)
    {
        print_error(p_tsn_ctx->p_cli, "Unable to cancel timeout timer");
    }

    if (p_resp->status == ZB_ZDP_STATUS_SUCCESS)
    {
        zb_uint16_t nwk_addr;

        ZB_LETOH16(&nwk_addr, &(p_resp->nwk_addr));
        nrf_cli_fprintf(p_tsn_ctx->p_cli,
                        NRF_CLI_NORMAL,
                        "%hx",
                        nwk_addr);
        print_done(p_tsn_ctx->p_cli, ZB_TRUE);
    }
    else
    {
        nrf_cli_fprintf(p_tsn_ctx->p_cli,
                        NRF_CLI_ERROR,
                        "Error: Unable to resolve EUI64 source address. Status: %d\r\n",
                        p_resp->status);
    }

    invalidate_ctx(p_tsn_ctx);
    ZB_FREE_BUF_BY_REF(param);
}

/**
 * @brief Handles timeout error and invalidates IEEE (EUI64) address request transaction.
 *
 * @param[in] param ZBOSS transaction sequence number.
 */
static void cmd_zb_ieee_addr_timeout(zb_uint8_t param)
{
    tsn_ctx_t * p_tsn_ctx = get_ctx_by_tsn(param);

    if (!p_tsn_ctx)
    {
        return;
    }

    print_error(p_tsn_ctx->p_cli, "IEEE address request timed out");
    invalidate_ctx(p_tsn_ctx);
}

/**
 * @brief A callback called on IEEE (EUI64) address response.
 *
 * @param[in] param Reference number to ZBOSS memory buffer.
 */
zb_void_t cmd_zb_ieee_addr_cb(zb_uint8_t param)
{
    zb_buf_t                    * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zdo_nwk_addr_resp_head_t * p_resp = (zb_zdo_nwk_addr_resp_head_t *)ZB_BUF_BEGIN(p_buf);
    tsn_ctx_t                   * p_tsn_ctx;
    zb_ret_t                      zb_err_code;

    p_tsn_ctx = get_ctx_by_tsn(p_resp->tsn);
    if (!p_tsn_ctx)
    {
        ZB_FREE_BUF_BY_REF(param);
        return;
    }

    zb_err_code = ZB_SCHEDULE_ALARM_CANCEL(cmd_zb_ieee_addr_timeout, ZB_ALARM_ANY_PARAM);
    if (zb_err_code != RET_OK)
    {
        print_error(p_tsn_ctx->p_cli, "Unable to cancel timeout timer");
    }

    if (p_resp->status == ZB_ZDP_STATUS_SUCCESS)
    {
        zb_address_ieee_ref_t addr_ref;
        zb_ieee_addr_t        ieee_addr;
        zb_uint16_t           nwk_addr;
        zb_ret_t              ret;

        ZB_LETOH64(ieee_addr, p_resp->ieee_addr);
        ZB_LETOH16(&nwk_addr, &(p_resp->nwk_addr));

        // Update local IEEE address resolution table.
        ret = zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
        if (ret == RET_OK)
        {
            print_64(p_tsn_ctx->p_cli, ieee_addr);
            print_done(p_tsn_ctx->p_cli, ZB_TRUE);
        }
        else
        {
            nrf_cli_fprintf(p_tsn_ctx->p_cli,
                            NRF_CLI_ERROR,
                            "Failed to updated address table. Status: %d\r\n",
                            ret);
        }
    }
    else
    {
        nrf_cli_fprintf(p_tsn_ctx->p_cli,
                        NRF_CLI_ERROR,
                        "Error: Unable to resolve IEEE address. Status: %d\r\n",
                        p_resp->status);
    }

    invalidate_ctx(p_tsn_ctx);
    ZB_FREE_BUF_BY_REF(param);
}

/**
 * @brief Send match descriptor request.
 *
 * @code
 * zdo match_desc <h:16-bit destination address>
                  <h:requested address/type> <h:profile ID>
                  <d:number of input clusters> [<h:input cluster IDs> ...]
                  <d:number of output clusters> [<h:output cluster IDs> ...]
 *
 * @endcode
 *
 * Send Match Descriptor Request to the `dst_addr` node that is a
 * query about the `req_addr` node of the `prof_id` profile ID,
 * which must have at least one of `n_input_clusters`(whose IDs are listed in `{...}`)
 * or `n_output_clusters` (whose IDs are listed in `{...}`).
 * The IDs can be either a decimal value or a hexadecimal string.
 *
 * Example:
 * @code
 * zdo match_desc 0xfffd 0xfffd 0x0104 1 6 0
 * @endcode
 *
 * In this example, the command sends a Match Descriptor Request to all non-sleeping
 * nodes regarding all non-sleeping nodes that have 1 input cluster ON/OFF (ID 6) and 0 output clusters.
 *
 */
static void cmd_zb_match_desc(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_zdo_match_desc_param_t * p_req;
    tsn_ctx_t                 * p_tsn_cli;
    zb_buf_t                  * p_buf;
    uint16_t                  * p_cluster_list = NULL;
    uint8_t                     len = sizeof(p_req->cluster_list);
    zb_ret_t                    zb_err_code;

    // We use p_cluster_list for calls to ZBOSS API but we're not using
    // p_cluster_list value in any way.
    UNUSED_VARIABLE(p_cluster_list);

    if ((argc == 1) || (nrf_cli_help_requested(p_cli)))
    {
        print_usage(p_cli, argv[0],
                    "<h:16-bit destination address>\r\n"
                    "<h:requested address/type> <h:profile ID>\r\n"
                    "<d:number of input clusters> [<h:input cluster IDs> ...]\r\n"
                    "<d:number of output clusters> [<h:output cluster IDs> ...]\r\n");
        return;
    }

    if (argc < 6)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        print_error(p_cli, "Failed to execute command (buf alloc failed)");
        return;
    }

    ZB_BUF_INITIAL_ALLOC(p_buf, sizeof(*p_req), p_req);

    if (!sscanf(argv[1], "%hx", &(p_req->nwk_addr)))
    {
        print_error(p_cli, "Incorrect network address");
        goto error;
    }

    if (!sscanf(argv[2], "%hx", &(p_req->addr_of_interest)))
    {
        print_error(p_cli, "Incorrect address of interest");
        goto error;
    }

    if (!sscanf(argv[3], "%hx", &(p_req->profile_id)))
    {
        print_error(p_cli, "Incorrect profile id");
        goto error;
    }

    // The following functions don't perform any checks on the cluster list
    // assuming that the CLI isn't abused. In practice the list length is limited
    // by @p NRF_CLI_ARGC_MAX which defaults to 12 arguments.

    if (!sscan_uint8(argv[4], &(p_req->num_in_clusters)))
    {
        print_error(p_cli, "Incorrect number of input clusters");
        goto error;
    }

    if (p_req->num_in_clusters)
    {
        // Allocate additional space for cluster IDs. Space for 1 one cluster ID
        // is already in the structure, hence we subtract len.
        ZB_BUF_ALLOC_RIGHT(p_buf,
                           p_req->num_in_clusters * sizeof(uint16_t) - len,
                           p_cluster_list);

        // We have used the space, set to 0 so that space for output clusters
        // is calculated correctly.
        len = 0;

        // Use p_req->cluster_list as destination rather that p_cluster_list which
        // points to the second element.
        if (!sscan_cluster_list(argv + 5, p_req->num_in_clusters, (uint16_t *)p_req->cluster_list))
        {
            print_error(p_cli, "Failed to parse input cluster list");
            goto error;
        }

    }

    if (!sscan_uint8(argv[5 + p_req->num_in_clusters], &(p_req->num_out_clusters)))
    {
        print_error(p_cli, "Incorrect number of output clusters");
        goto error;
    }

    if (p_req->num_out_clusters)
    {
        ZB_BUF_ALLOC_RIGHT(p_buf,
                           p_req->num_out_clusters * sizeof(uint16_t) - len,
                           p_cluster_list);

        if (!sscan_cluster_list(argv + 5 + p_req->num_in_clusters + 1,
                                p_req->num_out_clusters,
                                (uint16_t *)p_req->cluster_list + p_req->num_in_clusters))
        {
            print_error(p_cli, "Failed to parse output cluster list");
            goto error;
        }
    }

    p_tsn_cli = get_free_ctx();
    if (!p_tsn_cli)
    {
        print_error(p_cli, "Too many ZDO transactions");
        goto error;
    }

    p_tsn_cli->p_cli = p_cli;
    p_tsn_cli->tsn   = zb_zdo_match_desc_req(ZB_REF_FROM_BUF(p_buf),
                                             cmd_zb_match_desc_cb);
    if (p_tsn_cli->tsn == ZB_ZDO_INVALID_TSN)
    {
        print_error(p_cli, "Failed to send match descriptor request");
        goto error;
    }

    zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_match_desc_timeout,
                                    p_tsn_cli->tsn,
                                    ZIGBEE_CLI_MATCH_DESC_RESP_TIMEOUT * ZB_TIME_ONE_SECOND);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "Unable to schedule timeout timer");
        invalidate_ctx(p_tsn_cli);
    }

    return;

error:
    ZB_FREE_BUF(p_buf);
}

/**
 * @brief Create or remove a binding between two endpoints on two nodes.
 *
 * @code
 * zdo bind {on,off} <h:source eui64> <d:source ep> <h:destination eui64>
 *                   <d:destination ep> <h:source cluster id> <h:request dst addr>`
 * @endcode
 *
 * Create bound connection between a device identified by `source eui64` and
 * endpoint `source ep`, and a device identified by `destination eui64` and
 * endpoint `destination ep`. The connection is created for ZCL commands and
 * attributes assigned to the ZCL cluster `source cluster id` on the
 * `request dst addr` node (usually the same address as for the `source eui64` device).
 *
 * Example:
 * @code
 * zdo bind on 0B010E0405060708 1 0B010E4050607080 2 8
 * @endcode
 *
 */
static void cmd_zb_bind(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_zdo_bind_req_param_t * p_req;
    tsn_ctx_t               * p_tsn_cli = NULL;
    zb_buf_t                * p_buf;
    zb_ret_t                  zb_err_code;
    zb_uint8_t                addr_mode;
    zb_bool_t                 bind;

    if (strcmp(argv[0], "on") == 0)
    {
        bind = ZB_TRUE;
    }
    else
    {
        bind = ZB_FALSE;
    }

    if ((argc == 1) || (nrf_cli_help_requested(p_cli)))
    {
        print_usage(p_cli, argv[0],
                    "<h:source eui64> <d:source ep>\r\n"
                    "<h:destination eui64> <d:destination ep>\r\n"
                    "<h:source cluster id> <h:request dst addr>");
        return;
    }

    if (argc != 7)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        print_error(p_cli, "Failed to execute command (buf alloc failed)");
        return;
    }

    p_req = ZB_GET_BUF_PARAM(p_buf, zb_zdo_bind_req_param_t);

    if (parse_address_string(argv[1], (zb_addr_u*)(p_req->src_address), &addr_mode) == ZB_FALSE)
    {
        print_error(p_cli, "Incorrect EUI64 source address format");
        goto error;
    }

    if (addr_mode != ZB_APS_ADDR_MODE_64_ENDP_PRESENT)
    {
        print_error(p_cli, "Incorrect source address type");
        goto error;
    }

    if (!sscan_uint8(argv[2], &(p_req->src_endp)))
    {
        print_error(p_cli, "Incorrect source endpoint");
        goto error;
    }

    if (parse_address_string(argv[3], &(p_req->dst_address), &(p_req->dst_addr_mode)) == ZB_FALSE)
    {
        print_error(p_cli, "Incorrect EUI64 destination address format");
        goto error;
    }

    if (!sscan_uint8(argv[4], &(p_req->dst_endp)))
    {
        print_error(p_cli, "Incorrect destination endpoint");
        goto error;
    }

    if (!sscanf(argv[5], "%hx", &(p_req->cluster_id)))
    {
        print_error(p_cli, "Incorrect cluster ID");
        goto error;
    }

    if (!sscanf(argv[6], "%hx", &(p_req->req_dst_addr)))
    {
        print_error(p_cli, "Incorrect destination network address for the request");
        goto error;
    }

    p_tsn_cli = get_free_ctx();
    if (!p_tsn_cli)
    {
        print_error(p_cli, "Too many ZDO transactions");
        goto error;
    }

    // Initialize context and send a request.
    p_tsn_cli->p_cli = p_cli;

    if (bind)
    {
        p_tsn_cli->tsn = zb_zdo_bind_req(ZB_REF_FROM_BUF(p_buf), cmd_zb_bind_unbind_cb);
    }
    else
    {
        p_tsn_cli->tsn = zb_zdo_unbind_req(ZB_REF_FROM_BUF(p_buf), cmd_zb_bind_unbind_cb);
    }

    if (p_tsn_cli->tsn == ZB_ZDO_INVALID_TSN)
    {
        print_error(p_cli, "Failed to send request");
        goto error;
    }

    zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_bind_unbind_timeout,
                                    p_tsn_cli->tsn,
                                    ZIGBEE_CLI_BIND_RESP_TIMEOUT * ZB_TIME_ONE_SECOND);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "Unable to schedule timeout timer");
        invalidate_ctx(p_tsn_cli);
    }

    return;

error:
    if (p_tsn_cli != NULL)
    {
        invalidate_ctx(p_tsn_cli);
    }
    ZB_FREE_BUF(p_buf);
}

/**
 * @brief Resolve eui64 address to a short network address.
 *
 * @code
 * zdo nwk_addr <h:eui64>
 * @endcode
 *
 * Example:
 * @code
 * zdo nwk_addr 0B010E0405060708
 * @endcode
 *
 */
static void cmd_zb_nwk_addr(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_zdo_nwk_addr_req_param_t * p_req;
    tsn_ctx_t                   * p_tsn_cli = NULL;
    zb_buf_t                    * p_buf;
    zb_ret_t                      zb_err_code;
    zb_uint8_t                    addr_mode;

    if ((argc == 1) || (nrf_cli_help_requested(p_cli)))
    {
        print_usage(p_cli, argv[0], "<h:EUI64>");
        return;
    }

    if (argc != 2)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        print_error(p_cli, "Failed to execute command (buf alloc failed)");
        return;
    }

    p_req = ZB_GET_BUF_PARAM(p_buf, zb_zdo_nwk_addr_req_param_t);

    if (parse_address_string(argv[1], (zb_addr_u*)(p_req->ieee_addr), &addr_mode) == ZB_FALSE)
    {
        print_error(p_cli, "Incorrect EUI64 address format");
        goto error;
    }

    if (addr_mode != ZB_APS_ADDR_MODE_64_ENDP_PRESENT)
    {
        print_error(p_cli, "Incorrect source address type");
        goto error;
    }

    p_tsn_cli = get_free_ctx();
    if (!p_tsn_cli)
    {
        print_error(p_cli, "Too many ZDO transactions");
        goto error;
    }

    // Construct network address request.
    p_req->dst_addr     = ZB_NWK_BROADCAST_ALL_DEVICES;
    p_req->request_type = ZB_ZDO_SINGLE_DEVICE_RESP;
    p_req->start_index  = 0;

    // Initialize context and send a request.
    p_tsn_cli->p_cli = p_cli;
    p_tsn_cli->tsn   = zb_zdo_nwk_addr_req(ZB_REF_FROM_BUF(p_buf), cmd_zb_nwk_addr_cb);
    if (p_tsn_cli->tsn == ZB_ZDO_INVALID_TSN)
    {
        print_error(p_cli, "Failed to send request");
        goto error;
    }

    zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_nwk_addr_timeout,
                                    p_tsn_cli->tsn,
                                    ZIGBEE_CLI_NWK_ADDR_RESP_TIMEOUT * ZB_TIME_ONE_SECOND);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "Unable to schedule timeout timer");
        invalidate_ctx(p_tsn_cli);
    }

    return;

error:
    if (p_tsn_cli != NULL)
    {
        invalidate_ctx(p_tsn_cli);
    }
    ZB_FREE_BUF(p_buf);
}

/**
 * @brief Resolve EUI64 by sending IEEE address request.
 *
 * @code
 * zdo ieee_addr <h:short_addr>
 * @endcode
 *
 */
static void cmd_zb_ieee_addr(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_zdo_ieee_addr_req_t * p_req;
    tsn_ctx_t              * p_tsn_cli = NULL;
    zb_buf_t               * p_buf;
    zb_ret_t                 zb_err_code;

    if ((argc == 1) || (nrf_cli_help_requested(p_cli)))
    {
        print_usage(p_cli, argv[0],
                    "<h:short address>");
        return;
    }

    if (argc != 2)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        print_error(p_cli, "Failed to execute command (buf alloc failed)");
        return;
    }

    // Create new IEEE address request and fill with default values.
    ZB_BUF_INITIAL_ALLOC(p_buf, sizeof(zb_zdo_ieee_addr_req_t), p_req);
    p_req->start_index  = 0;
    p_req->request_type = 0;

    if (!sscanf(argv[1], "%hx", &(p_req->nwk_addr)))
    {
        print_error(p_cli, "Incorrect network address");
        goto error;
    }

    p_tsn_cli = get_free_ctx();
    if (!p_tsn_cli)
    {
        print_error(p_cli, "Too many ZDO transactions");
        goto error;
    }

    // Initialize context and send a request.
    p_tsn_cli->p_cli = p_cli;
    p_tsn_cli->tsn   = zb_zdo_ieee_addr_req(ZB_REF_FROM_BUF(p_buf), cmd_zb_ieee_addr_cb);
    if (p_tsn_cli->tsn == ZB_ZDO_INVALID_TSN)
    {
        print_error(p_cli, "Failed to send request");
        goto error;
    }

    zb_err_code = ZB_SCHEDULE_ALARM(cmd_zb_ieee_addr_timeout,
                                    p_tsn_cli->tsn,
                                    ZIGBEE_CLI_IEEE_ADDR_RESP_TIMEOUT * ZB_TIME_ONE_SECOND);
    if (zb_err_code != RET_OK)
    {
        print_error(p_cli, "Unable to schedule timeout timer");
        invalidate_ctx(p_tsn_cli);
    }

    return;

error:
    if (p_tsn_cli != NULL)
    {
        invalidate_ctx(p_tsn_cli);
    }
    ZB_FREE_BUF(p_buf);
}

/**
 * @brief Get the short 16-bit address of the Zigbee device.
 *
 * @code
 * > zdo short
 * 0000
 * Done
 * @endcode
 */
static void cmd_zb_short(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argv);

    zb_ieee_addr_t addr;
    zb_uint16_t short_addr;
    int i;

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 1)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    zb_get_long_address(addr);

    short_addr = zb_address_short_by_ieee(addr);
    if (short_addr != ZB_UNKNOWN_SHORT_ADDR)
    {
        /* We got a valid address */
        for (i = sizeof(zb_uint16_t) - 1; i >= 0; i--)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%02x", *((zb_uint8_t*)(&short_addr) + i));
        }

        print_done(p_cli, ZB_TRUE);
    }
    else
    {
        /* Most probably there was no network to join */
        print_error(p_cli, "Check if device was commissioned\r\n");
    }
}

/**
 * @brief Get the EUI64 address of the Zigbee device.
 *
 * @code
 * > zdo eui64
 * 0b010eaafd745dfa
 * Done
 * @endcode
 */
static void cmd_zb_eui64(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ieee_addr_t addr;

    UNUSED_PARAMETER(argv);

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 1)
    {
        print_error(p_cli, "Incorrect number of arguments");
        return;
    }

    zb_get_long_address(addr);
    print_64(p_cli, addr);
    print_done(p_cli, ZB_TRUE);
}


NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_bind)
{
    NRF_CLI_CMD(on, NULL, "create bind entry", cmd_zb_bind),
    NRF_CLI_CMD(off, NULL, "remove bind entry", cmd_zb_bind),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_zdo)
{
    NRF_CLI_CMD(match_desc, NULL, "send match descripor request", cmd_zb_match_desc),
    NRF_CLI_CMD(nwk_addr, NULL, "resolve EUI64 address to short network address", cmd_zb_nwk_addr),
    NRF_CLI_CMD(ieee_addr, NULL, "resolve network short address to EUI64 address", cmd_zb_ieee_addr),
    NRF_CLI_CMD(eui64, NULL, "get the eui64 address of the node", cmd_zb_eui64),
    NRF_CLI_CMD(short, NULL, "get the short address of the node", cmd_zb_short),
    NRF_CLI_CMD(bind, &m_sub_bind, "create or remove the binding entry in the remote node", NULL),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(zdo, &m_sub_zdo, "ZDO manipulation", NULL);


/** @} */
