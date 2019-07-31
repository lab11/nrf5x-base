#include "benchmark_api.h"
#include "benchmark_zigbee_common.h"
#include "zboss_api.h"
#include "zigbee_cli.h"
#include "zigbee_cli_utils.h"
#include "nrf_log.h"

/**@brief ZCL Frame control field of Zigbee benchmark commands, that expects command execution result.
 */
#define ZIGBEE_BENCHMARK_FRAME_CONTROL_FIELD  ZB_ZCL_CONSTRUCT_FRAME_CONTROL(ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC, \
                                                                             ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,   \
                                                                             ZB_ZCL_FRAME_DIRECTION_TO_SRV,      \
                                                                             ZB_ZCL_ENABLE_DEFAULT_RESPONSE)

/**@brief ZCL Frame control field of Zigbee benchmark commands, that does not expect command execution result.
 */
#define ZIGBEE_BENCHMARK_FRAME_CONTROL_FIELD_NORESP  ZB_ZCL_CONSTRUCT_FRAME_CONTROL(ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC, \
                                                                                    ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,   \
                                                                                    ZB_ZCL_FRAME_DIRECTION_TO_SRV,      \
                                                                                    ZB_ZCL_DISABLE_DEFAULT_RESPONSE)

/**@brief Custom benchmark cluster ID, used inside all control commands.
 */
#define BENCHMARK_CUSTOM_CLUSTER        0xBEC4


static zb_uint8_t m_seq_num = 0; /* ZCL sequence number for benchmark commands. */

/**@brief Forward declaration of function that sends local test results over Zigbee network. */
static zb_zcl_status_t zigbee_benchmark_peer_results_response_send(zb_uint8_t param, zb_uint16_t peer_addr, benchmark_result_t * p_results);


/**@bried Default handler for incoming benchmark command APS acknowledgments.
 *
 * @param[in] param  Reference to a ZBoss buffer containing APC ACK data,
 */
static zb_void_t zigbee_benchmark_frame_error_handler(zb_uint8_t param)
{
    zb_buf_t                     * p_asdu;
    zb_zcl_command_send_status_t * p_cmd_status;

    if (param == 0)
    {
        return;
    }

    p_asdu       = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    p_cmd_status = ZB_GET_BUF_PARAM(p_asdu, zb_zcl_command_send_status_t);
    NRF_LOG_DEBUG("Frame acknowledged. Status: %u", p_cmd_status->status);

    if (p_cmd_status->status != RET_OK)
    {
        /* No APS-ACK received. Abort benchmark test execution. */
        zigbee_benchmark_test_abort();
    }

    ZB_FREE_BUF_BY_REF(param);
}

/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param    Reference to a ZBoss buffer
 */
static zb_uint8_t zigbee_benchmark_ep_handler(zb_uint8_t param)
{
    zb_uint16_t           remote_short_addr;
    zb_buf_t            * p_zcl_cmd_buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
    zb_zcl_parsed_hdr_t * p_cmd_info    = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
    zb_zcl_status_t       zb_zcl_status = ZB_ZCL_STATUS_SUCCESS;
    zb_bool_t             send_response = ZB_FALSE;

    if (p_cmd_info->cluster_id != BENCHMARK_CUSTOM_CLUSTER ||
        p_cmd_info->profile_id != ZB_AF_HA_PROFILE_ID)
    {
        return ZB_FALSE;
    }

    NRF_LOG_DEBUG("New benchmark frame received, param: %u", param);
    if (p_cmd_info->addr_data.common_data.source.addr_type != ZB_ZCL_ADDR_TYPE_SHORT)
    {
        return ZB_FALSE;
    }
    remote_short_addr = p_cmd_info->addr_data.common_data.source.u.short_addr;

    if (p_cmd_info->cmd_id == ZB_ZCL_CMD_DEFAULT_RESP)
    {
        zb_zcl_default_resp_payload_t * p_def_resp;
        p_def_resp = ZB_ZCL_READ_DEFAULT_RESP(p_zcl_cmd_buf);
        NRF_LOG_DEBUG("Default Response received. Command: %u, Status: %u",
                      p_def_resp->command_id, p_def_resp->status);
        zigbee_benchmark_command_response_handler((zigbee_benchmark_ctrl_t)p_def_resp->command_id, (zb_zcl_status_t)p_def_resp->status);
    }
    else if (p_cmd_info->cmd_id == TEST_START_REQUEST)
    {
        NRF_LOG_DEBUG("Remote peer 0x%04x started benchmark test.", remote_short_addr);
        zb_zcl_status = zigbee_benchmark_test_start_slave();
        send_response = ZB_TRUE;
    }
    else if (p_cmd_info->cmd_id == TEST_STOP_REQUEST)
    {
        NRF_LOG_DEBUG("Remote peer 0x%04x stopped benchmark test.", remote_short_addr);
        zb_zcl_status = zigbee_benchmark_test_stop_slave();
        send_response = ZB_TRUE;
    }
    else if (p_cmd_info->cmd_id == TEST_RESULTS_REQUEST)
    {
        NRF_LOG_DEBUG("Remote peer 0x%04x asked for benchmark results.", remote_short_addr);
        zb_zcl_status = zigbee_benchmark_peer_results_response_send(param, remote_short_addr, zigbee_benchmark_local_result_get());
        if (zb_zcl_status != ZB_ZCL_STATUS_SUCCESS)
        {
            send_response = ZB_TRUE;
        }
        else
        {
            param = 0;
        }
    }
    else if (p_cmd_info->cmd_id == TEST_RESULTS_RESPONSE)
    {
        zb_uint8_t len = ZB_BUF_LEN(p_zcl_cmd_buf);
        NRF_LOG_DEBUG("Remote peer 0x%04x sent benchmark results.", remote_short_addr);
        if (len != sizeof(benchmark_result_t))
        {
            zigbee_benchmark_test_abort();
        }
        else
        {
            zigbee_benchmark_results_received((benchmark_result_t *)ZB_BUF_BEGIN(p_zcl_cmd_buf));
        }
    }
    else
    {
        NRF_LOG_WARNING("Unsupported benchmark command received, cmd_id %u", p_cmd_info->cmd_id);
        zb_zcl_status = ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD;
        send_response = ZB_TRUE;
    }

    if (param)
    {
        if (send_response)
        {
            zb_uint8_t cli_ep = zb_get_cli_endpoint();
            zb_uint8_t cmd_id = p_cmd_info->cmd_id;

            ZB_BUF_REUSE(p_zcl_cmd_buf);
            NRF_LOG_DEBUG("Send (benchmark) default response. Command: %u, status: %u", p_cmd_info->cmd_id, zb_zcl_status);
            ZB_ZCL_SEND_DEFAULT_RESP(p_zcl_cmd_buf, remote_short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT, cli_ep, cli_ep, ZB_AF_HA_PROFILE_ID, BENCHMARK_CUSTOM_CLUSTER, m_seq_num++, cmd_id, zb_zcl_status);
        }
        else
        {
            ZB_FREE_BUF_BY_REF(param);
        }
    }
    return ZB_TRUE;
}

/**@brief Function to send benchmark control commands.
 *
 * @param[in] param      Reference number to ZBOSS memory buffer.
 * @param[in] peer_addr  Network address of the peer to control.
 * @param[in] ctrl_cmd   Control command Id.
 *
 * @retval RET_OK if command was successfully send, error code otherwise.
 */
static zb_ret_t zigbee_benchmark_ctrl_request_send(zb_uint8_t param, zb_uint16_t peer_addr, zigbee_benchmark_ctrl_t ctrl_cmd)
{
    zb_uint8_t   cli_ep = zb_get_cli_endpoint();
    zb_ret_t     zb_err_code;
    zb_buf_t   * p_buf;
    zb_uint8_t * p_cmd_buf;
    zb_addr_u    remote_addr = {.addr_short = peer_addr};

    if (!param)
    {
        return RET_INVALID_PARAMETER_1;
    }

    p_buf = ZB_BUF_FROM_REF(param);
    p_cmd_buf = ZB_ZCL_START_PACKET(p_buf);

    // Add frame control field based on request type.
    switch (ctrl_cmd)
    {
        case TEST_RESULTS_REQUEST:
            *(p_cmd_buf++) = ZIGBEE_BENCHMARK_FRAME_CONTROL_FIELD_NORESP;
            break;
            
        default:
            *(p_cmd_buf++) = ZIGBEE_BENCHMARK_FRAME_CONTROL_FIELD;
            break;
    }

    *(p_cmd_buf++) = m_seq_num; /* Sequence Number Field */
    *(p_cmd_buf++) = ctrl_cmd;  /* Command ID field */
    m_seq_num++;

    NRF_LOG_DEBUG("Send benchmark control command. ID: %u", ctrl_cmd);
    zb_err_code = zb_zcl_finish_and_send_packet(p_buf, p_cmd_buf, &remote_addr,
                                                ZB_APS_ADDR_MODE_16_ENDP_PRESENT, cli_ep, cli_ep,
                                                ZB_AF_HA_PROFILE_ID, BENCHMARK_CUSTOM_CLUSTER,
                                                zigbee_benchmark_frame_error_handler);
    return zb_err_code;
}

/**@brief Function to send local results to the other peer.
 *
 * @param[in] param      Reference number to ZBOSS memory buffer.
 * @param[in] peer_addr  Network address of the peer that asked for test results.
 * @param[in] p_result   Pointer to the benchmark test results, that will be copied to the Zigbee command frame.
 *
 * @retval ZB_ZCL_STATUS_SUCCESS if results were successfully send.
 * @retval ZB_ZCL_STATUS_FAIL    if failed to send results.
 */
static zb_zcl_status_t zigbee_benchmark_peer_results_response_send(zb_uint8_t param, zb_uint16_t peer_addr, benchmark_result_t * p_result)
{
    zb_uint8_t   cli_ep = zb_get_cli_endpoint();
    zb_ret_t     zb_err_code;
    zb_buf_t   * p_buf;
    zb_uint8_t * p_cmd_buf;
    zb_addr_u    remote_addr = {.addr_short = peer_addr};

    if (!param)
    {
        return ZB_ZCL_STATUS_FAIL;
    }

    p_buf = ZB_BUF_FROM_REF(param);
    p_cmd_buf = ZB_ZCL_START_PACKET(p_buf);
    *(p_cmd_buf++) = ZIGBEE_BENCHMARK_FRAME_CONTROL_FIELD_NORESP;
    *(p_cmd_buf++) = m_seq_num; /* Sequence Number Field */
    *(p_cmd_buf++) = TEST_RESULTS_RESPONSE;
    memcpy(p_cmd_buf, p_result, sizeof(benchmark_result_t));
    p_cmd_buf += sizeof(benchmark_result_t);
    m_seq_num++;

    NRF_LOG_DEBUG("Send benchmark results from the remote peer to 0x%04x", peer_addr);
    zb_err_code = zb_zcl_finish_and_send_packet(p_buf, p_cmd_buf, &remote_addr,
                                                ZB_APS_ADDR_MODE_16_ENDP_PRESENT, cli_ep, cli_ep,
                                                ZB_AF_HA_PROFILE_ID, BENCHMARK_CUSTOM_CLUSTER,
                                                zigbee_benchmark_frame_error_handler);
    return (zb_err_code == RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);
}


zb_void_t zigbee_benchmark_peer_results_request_send(zb_uint8_t param, zb_uint16_t peer_addr)
{
    zb_ret_t err_code = zigbee_benchmark_ctrl_request_send(param, peer_addr, TEST_RESULTS_REQUEST);
    if (err_code != RET_OK)
    {
        NRF_LOG_WARNING("Sending results request (ID: %u) to the remote peer (nwk_addr: 0x%04x) failed with error: %d",
                        TEST_RESULTS_REQUEST, peer_addr, err_code);
        zigbee_benchmark_test_abort();
    }
}

zb_void_t zigbee_benchmark_peer_start_request_send(zb_uint8_t param, zb_uint16_t peer_addr)
{
    zb_ret_t err_code = zigbee_benchmark_ctrl_request_send(param, peer_addr, TEST_START_REQUEST);
    if (err_code != RET_OK)
    {
        NRF_LOG_WARNING("Sending test start request (ID: %u) to the remote peer (nwk_addr: 0x%04x) failed with error: %d",
                        TEST_START_REQUEST, peer_addr, err_code);
        zigbee_benchmark_test_abort();
    }
}

zb_void_t zigbee_benchmark_peer_stop_request_send(zb_uint8_t param, zb_uint16_t peer_addr)
{
    zb_ret_t err_code = zigbee_benchmark_ctrl_request_send(param, peer_addr, TEST_STOP_REQUEST);
    if (err_code != RET_OK)
    {
        NRF_LOG_WARNING("Sending test stop request (ID: %u) to the remote peer (nwk_addr: 0x%04x) failed with error: %d",
                        TEST_STOP_REQUEST, peer_addr, err_code);
        zigbee_benchmark_test_abort();
    }
}


/**@brief Endpoint handlers
 */
NRF_ZIGBEE_EP_HANDLER_REGISTER(benchmark, zigbee_benchmark_ep_handler);
