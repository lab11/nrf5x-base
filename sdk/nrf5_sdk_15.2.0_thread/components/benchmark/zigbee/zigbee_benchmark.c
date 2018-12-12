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

#include "benchmark_api.h"
#include "benchmark_zigbee_common.h"
#include "cpu_utilization.h"
#include "zboss_api.h"
#include "zigbee_cli.h"
#include "zigbee_cli_ping.h"
#include "app_timer.h"
#include "nrf_log.h"

typedef union
{
    zb_ieee_addr_t arr_addr;
    uint64_t       uint_addr;
} eui64_map_addr_t;

APP_TIMER_DEF(m_discovery_timer);
static zb_time_t                     m_start_time;
static benchmark_evt_t               m_benchmark_evt;
static benchmark_result_t            m_local_result;
static benchmark_callback_t          mp_callback;
static benchmark_peer_db_t           m_peer_information;
static benchmark_address_context_t   m_peer_addresses[BENCHMARK_MAX_PEER_NUMBER];
static benchmark_configuration_t   * mp_test_configuration;
static benchmark_test_state_t        m_state = TEST_IDLE;

static benchmark_status_t m_test_status =
{
    .test_in_progress   = false,
    .reset_counters     = false,
    .waiting_for_ack    = 0,
    .packets_left_count = 0,
    .acks_lost          = 0,
};

/**@brief Function that triggers calculation of the test duration. */
static void benchmark_test_duration_calculate(void)
{
    m_local_result.duration = ZB_TIME_BEACON_INTERVAL_TO_MSEC(ZB_TIMER_GET() - m_start_time);
}

/**@brief Function that resets all test counters. */
static void result_clear(void)
{
    memset(&m_local_result, 0, sizeof(m_local_result));
    cpu_utilization_clear();

    m_local_result.rx_counters.rx_total = BENCHMARK_COUNTERS_VALUE_NOT_SUPPORTED;
    m_local_result.rx_counters.rx_error = BENCHMARK_COUNTERS_VALUE_NOT_SUPPORTED;
}

/**
 * @brief Callback for discovery timeout event.
 *
 * @param[in] p_context Unused parameter.
 */
static void discovery_timeout(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    m_benchmark_evt.evt                        = BENCHMARK_DISCOVERY_COMPLETED;
    m_benchmark_evt.context.p_peer_information = &m_peer_information;

    if (mp_callback)
    {
        mp_callback(&m_benchmark_evt);
    }
}

/**
 * @brief A callback called on match descriptor response.
 *
 * @param[in] a reference number to ZBOSS memory buffer
 */
static void zb_find_peers_cb(zb_uint8_t param)
{
    zb_buf_t                   * p_buf  = ZB_BUF_FROM_REF(param);
    zb_zdo_match_desc_resp_t   * p_resp = (zb_zdo_match_desc_resp_t *)ZB_BUF_BEGIN(p_buf);
    zb_apsde_data_indication_t * p_ind  = ZB_GET_BUF_PARAM(p_buf, zb_apsde_data_indication_t);

    if (p_resp->status == ZB_ZDP_STATUS_SUCCESS)
    {
        zb_uint8_t * p_match_ep = (zb_uint8_t *)(p_resp + 1);

        while (p_resp->match_len > 0)
        {
            /* Match EP list follows right after response header */
            if (*p_match_ep == zb_get_cli_endpoint())
            {
                uint16_t peer_number = m_peer_information.peer_count;

                NRF_LOG_INFO("Peer found: %0hx\r\n", p_ind->src_addr);

                if (peer_number < BENCHMARK_MAX_PEER_NUMBER)
                {
                    m_peer_information.peer_table[peer_number].device_id = 0; //TODO: Resolve nwk_address into EUI64 while scanning for peers.
                    m_peer_information.peer_table[peer_number].p_address = &m_peer_addresses[peer_number];
                    m_peer_information.peer_table[peer_number].p_address->nwk_addr = p_ind->src_addr;
                    m_peer_information.peer_count++;
                }
                else
                {
                    NRF_LOG_INFO("Can't add peer to the list, list full");
                }
            }

            p_match_ep += 1;
            p_resp->match_len -= 1;
        }
    }

    ZB_FREE_BUF_BY_REF(param);
}

/**@brief Function that constructs and sends discovery request. */
static zb_ret_t zigbee_benchmark_peer_discovery_request_send(void)
{
    zb_zdo_match_desc_param_t * p_req;
    zb_buf_t                  * p_buf;

    p_buf = ZB_GET_OUT_BUF();
    if (!p_buf)
    {
        NRF_LOG_ERROR("Failed to execute command (buffer allocation failed)");
        return RET_ERROR;
    }

    /* Initialize pointers inside buffer and reserve space for zb_zdo_match_desc_param_t request */
    UNUSED_RETURN_VALUE(ZB_BUF_INITIAL_ALLOC(p_buf, sizeof(zb_zdo_match_desc_param_t), p_req));

    p_req->nwk_addr         = ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE; // Send to all non-sleepy devices
    p_req->addr_of_interest = ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE; // Get responses from all non-sleepy devices
    p_req->profile_id       = ZB_AF_HA_PROFILE_ID;              // Look for Home Automation profile clusters

    /* We are searching for 1 input cluster: Basic */
    p_req->num_in_clusters  = 1;
    p_req->num_out_clusters = 0;
    p_req->cluster_list[0]  = ZB_ZCL_CLUSTER_ID_BASIC;

    uint8_t tsn = zb_zdo_match_desc_req(ZB_REF_FROM_BUF(p_buf), zb_find_peers_cb);
    if (tsn == ZB_ZDO_INVALID_TSN)
    {
        ZB_FREE_BUF(p_buf);
        return RET_ERROR;
    }

    return RET_OK;
}

/**@brief  Ping event handler. Updates RX counters and changes state to send next request.
 *
 * @param[in] evt_type  Type of received  ping acknowledgment
 * @param[in] delay_us  Time, in microseconds, between ping request and the event.
 * @param[in] p_request Pointer to the ongoing ping request context structure.
 */
static void benchmark_ping_evt_handler(ping_time_evt_t evt, zb_uint32_t delay_us, ping_request_t * p_request)
{
    switch (evt)
    {
        case PING_EVT_FRAME_SENT:
            if (m_state == TEST_IN_PROGRESS_FRAME_SENDING)
            {
                m_state = TEST_IN_PROGRESS_FRAME_SENT;
            }
            break;

        case PING_EVT_ACK_RECEIVED:
            if (m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK)
            {
                m_test_status.waiting_for_ack = 0;
                m_state = TEST_IN_PROGRESS_FRAME_SENT;
                NRF_LOG_INFO("Transmission time: %u us", delay_us / 2);
            }
            break;

        case PING_EVT_ECHO_RECEIVED:
            if (m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO)
            {
                m_test_status.waiting_for_ack = 0;
                m_state = TEST_IN_PROGRESS_FRAME_SENT;
                NRF_LOG_INFO("Transmission time: %u us", delay_us / 2);
            }
            break;

        case PING_EVT_FRAME_TIMEOUT:
            if ((m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK) ||
                (m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO))
            {
                m_test_status.waiting_for_ack = 0;
                m_test_status.acks_lost++;
                m_state = TEST_IN_PROGRESS_FRAME_SENT;
                NRF_LOG_INFO("Transmission timed out");
            }
            break;

        case PING_EVT_FRAME_SCHEDULED:
            if (m_test_status.packets_left_count)
            {
                m_test_status.packets_left_count--;
            }

            if (m_state == TEST_IN_PROGRESS_FRAME_SENDING)
            {
                if (mp_test_configuration)
                {
                    if (mp_test_configuration->mode != BENCHMARK_MODE_UNIDIRECTIONAL)
                    {
                        m_state = TEST_IN_PROGRESS_FRAME_SENT;
                    }
                }
                else
                {
                    m_state = TEST_IN_PROGRESS_FRAME_SENT;
                }
            }
            else
            {
                m_test_status.waiting_for_ack = p_request->ping_seq;
                m_test_status.frame_number    = p_request->ping_seq;
            }
            break;

        case PING_EVT_ERROR:
            if ((m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK) ||
                (m_state == TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO) ||
                (m_state == TEST_IN_PROGRESS_FRAME_SENDING))
            {
                m_state = TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER;
            }
            break;

        case PING_EVT_REQUEST_RECEIVED:
            if (m_test_status.reset_counters)
            {
                // First packet in slave mode received.
                cpu_utilization_clear();
                m_test_status.reset_counters = false;
            }

            // Update CPU utilization value in order to get more accurate values inside slave mode.
            m_local_result.cpu_utilization = cpu_utilization_get();
            m_local_result.rx_counters.bytes_received += p_request->count;
            m_local_result.rx_counters.packets_received ++;

            break;
        default:
            break;
    }
}

/**@brief Function that constructs and sends ping request. */
static void zigbee_benchmark_send_ping_req(nrf_cli_t * p_cli)
{
    ping_request_t * p_ping_req;

    if (mp_test_configuration->role == BENCHMARK_ROLE_SLAVE)
    {
        // Only master node shall send requests.
        return;
    }

    if (m_test_status.packets_left_count == 0)
    {
        return;
    }

    if (m_state != TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER)
    {
        return;
    }

    p_ping_req = zb_ping_acquire_request();
    if (p_ping_req == NULL)
    {
        return;
    }

    if (mp_test_configuration->length > PING_MAX_LENGTH)
    {
        m_state = TEST_ERROR;
        zb_ping_release_request(p_ping_req);
        return;
    }

    p_ping_req->remote_addr.addr_short = m_peer_information.peer_table[m_peer_information.selected_peer].p_address->nwk_addr;
    p_ping_req->remote_addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    p_ping_req->timeout_ms = mp_test_configuration->ack_timeout;
    p_ping_req->count = mp_test_configuration->length;
    p_ping_req->p_cli = p_cli;
    p_ping_req->p_cb = benchmark_ping_evt_handler;

    switch (mp_test_configuration->mode)
    {
        case BENCHMARK_MODE_UNIDIRECTIONAL:
            p_ping_req->request_ack  = 0;
            p_ping_req->request_echo = 0;
            m_state = TEST_IN_PROGRESS_FRAME_SENDING;
            break;

        case BENCHMARK_MODE_ECHO:
            p_ping_req->request_ack  = 0;
            p_ping_req->request_echo = 1;
            m_state = TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO;
            break;

        case BENCHMARK_MODE_ACK:
            p_ping_req->request_ack  = 1;
            p_ping_req->request_echo = 0;
            m_state = TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK;
            break;

        default:
            NRF_LOG_ERROR("Unsupported test mode - fall back to the default mode.");
            mp_test_configuration->mode = BENCHMARK_MODE_ECHO;
            p_ping_req->request_ack     = 0;
            p_ping_req->request_echo    = 1;
            m_state = TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO;
            break;
    }

    ping_request_send(p_ping_req);
}

/**@brief Function that starts benchmark test in master mode. */
static void zigbee_benchmark_test_start_master(void)
{
    m_state = TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER;
    zigbee_benchmark_send_ping_req(NULL);

    m_benchmark_evt.evt           = BENCHMARK_TEST_STARTED;
    m_benchmark_evt.context.error = RET_OK;
    mp_callback(&m_benchmark_evt);
}

/**@brief Function that starts benchmark test in slave mode. */
static void zigbee_benchmark_test_start_slave(void)
{
    m_state = TEST_IN_PROGRESS_WAITING_FOR_STOP_CMD;

    m_benchmark_evt.evt           = BENCHMARK_TEST_STARTED;
    m_benchmark_evt.context.error = RET_OK;
    mp_callback(&m_benchmark_evt);
}

/**@brief Function that stops benchmark test in master mode. */
static void zigbee_benchmark_test_stop_master(void)
{
    m_local_result.cpu_utilization = cpu_utilization_get();

    if (mp_test_configuration->peer_control)
    {
        //TODO: Implement peer control or remove that option from benchmark.
        m_benchmark_evt.evt           = BENCHMARK_TEST_STOPPED;
        m_benchmark_evt.context.error = RET_OK;
    }
    else
    {
        m_benchmark_evt.evt                             = BENCHMARK_TEST_COMPLETED;
        m_benchmark_evt.context.results.p_remote_result = NULL;
        m_benchmark_evt.context.results.p_local_result  = &m_local_result;
        m_benchmark_evt.context.results.p_local_status  = &m_test_status;
    }

    mp_callback(&m_benchmark_evt);
}

/**@brief Function that stops benchmark test in slave mode. */
static void zigbee_benchmark_test_stop_slave(void)
{
    if (mp_test_configuration->peer_control == false)
    {
        m_benchmark_evt.evt                             = BENCHMARK_TEST_COMPLETED;
        m_benchmark_evt.context.results.p_remote_result = NULL;
        m_benchmark_evt.context.results.p_local_result  = &m_local_result;
        m_benchmark_evt.context.results.p_local_status  = &m_test_status;
        mp_callback(&m_benchmark_evt);
    }
    else
    {
        m_local_result.cpu_utilization = cpu_utilization_get();
    }
}

void benchmark_init(void)
{
    mp_test_configuration = NULL;
    zb_ping_set_ping_indication_cb(benchmark_ping_evt_handler);

    uint32_t err_code = app_timer_create(&m_discovery_timer, APP_TIMER_MODE_SINGLE_SHOT, discovery_timeout);
    APP_ERROR_CHECK(err_code);
}

uint32_t benchmark_test_init(benchmark_configuration_t * p_configuration, benchmark_callback_t callback)
{
    if (m_state != TEST_IDLE)
    {
        NRF_LOG_ERROR("Stop current test in order to modify test settings.");
        return (uint32_t)RET_ERROR;
    }

    if ((p_configuration == NULL) || (callback == NULL))
    {
        NRF_LOG_ERROR("Both configuration and callback have to be passed.");
        return (uint32_t)RET_ERROR;
    }

    if (callback == NULL)
    {
        NRF_LOG_ERROR("Event callback have to be passed.");
        return (uint32_t)RET_ERROR;
    }

    mp_callback                      = callback;
    mp_test_configuration            = p_configuration;
    m_test_status.packets_left_count = p_configuration->count;
    m_test_status.waiting_for_ack    = 0;
    m_test_status.frame_number       = 0;

    return (uint32_t)RET_OK;
}

uint32_t benchmark_test_start(void)
{
    if (m_state != TEST_IDLE)
    {
        NRF_LOG_ERROR("Stop current test in order to start different suite.");
        return (uint32_t)RET_ERROR;
    }

    if (mp_test_configuration == NULL)
    {
        NRF_LOG_ERROR("Provide test configuration before starting a test suite.");
        return (uint32_t)RET_ERROR;
    }

    if (mp_test_configuration->peer_control)
    {
        NRF_LOG_ERROR("Currently peer control is not supported in Zigbee benchmark.");
        return (uint32_t)RET_ERROR;
    }

    result_clear();
    UNUSED_RETURN_VALUE(cpu_utilization_start());

    memset(&m_benchmark_evt, 0, sizeof(benchmark_evt_t));
    m_test_status.reset_counters = false;
    m_test_status.acks_lost      = 0;
    m_start_time                 = ZB_TIMER_GET();

    if (mp_test_configuration->role == BENCHMARK_ROLE_SLAVE)
    {
        if (mp_test_configuration->peer_control == false)
        {
            m_test_status.reset_counters = true;
        }
        zigbee_benchmark_test_start_slave();
        return (uint32_t)RET_OK;
    }
    m_test_status.test_in_progress = true;

    /* If peer control is disabled, do not send a request to start the test on the other side. */
    if (!mp_test_configuration->peer_control)
    {
        zigbee_benchmark_test_start_master();
        return (uint32_t)RET_OK;
    }

    //TODO: Implement peer control or remove that option from benchmark.
    return (uint32_t)RET_ERROR;
}

uint32_t benchmark_test_stop(void)
{
    zb_ret_t err_code = RET_OK;

    if (mp_test_configuration == NULL)
    {
        return (uint32_t)RET_ERROR;
    }

    if (m_state == TEST_IDLE)
    {
        NRF_LOG_ERROR("There is no ongoing test.");
        return (uint32_t)RET_ERROR;
    }

    m_test_status.test_in_progress = false;
    m_state = TEST_IDLE;
    benchmark_test_duration_calculate();
    UNUSED_RETURN_VALUE(cpu_utilization_stop());

    if (mp_test_configuration->role == BENCHMARK_ROLE_SLAVE)
    {
        if (mp_test_configuration->peer_control == false)
        {
            zigbee_benchmark_test_stop_slave();
        }
        return (uint32_t)err_code;
    }

    // If peer control is disabled, do not send a request to start the test on the other side.
    if (!mp_test_configuration->peer_control)
    {
        zigbee_benchmark_test_stop_master();
        return (uint32_t)err_code;
    }

    //TODO: Implement peer control or remove that option from benchmark.
    return (uint32_t)err_code;
}

uint32_t benchmark_peer_discover(void)
{
    if (m_state != TEST_IDLE)
    {
        NRF_LOG_ERROR("Stop current test in order to start peer discovery.");
        return (uint32_t)RET_ERROR;
    }

    if (mp_test_configuration == NULL)
    {
        NRF_LOG_ERROR("Provide test configuration before starting a peer discovery.");
        return (uint32_t)RET_ERROR;
    }

    memset(&m_peer_information, 0, sizeof(m_peer_information));

    zb_ret_t error = zigbee_benchmark_peer_discovery_request_send();
    if (error != RET_OK)
    {
        return (uint32_t)error;
    }

    uint32_t err_code = app_timer_start(m_discovery_timer, APP_TIMER_TICKS(BENCHMARK_DISCOVERY_TIMEOUT), NULL);
    APP_ERROR_CHECK(err_code);

    return (uint32_t)RET_OK;
}

const benchmark_peer_db_t * benchmark_peer_table_get(void)
{
    return &m_peer_information;
}

benchmark_status_t * benchmark_status_get(void)
{
    return &m_test_status;
}

void benchmark_process(void)
{
    switch (m_state)
    {
        // fall through
        case TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK:
        case TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ECHO:
        case TEST_IN_PROGRESS_FRAME_SENDING:
        case TEST_IN_PROGRESS_WAITING_FOR_STOP_CMD:
        case TEST_IDLE:
            break;

        case TEST_ERROR:
            if (mp_test_configuration)
            {
                NRF_LOG_ERROR("Error occurred during the test transmission. Sent %d packets",
                              (mp_test_configuration->count - m_test_status.packets_left_count));
            }
            m_test_status.test_in_progress = false;
            m_state = TEST_IDLE;

            if (mp_callback)
            {
                m_benchmark_evt.evt           = BENCHMARK_TEST_STOPPED;
                m_benchmark_evt.context.error = (uint32_t)RET_ERROR;
                mp_callback(&m_benchmark_evt);
            }
            break;

        case TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER:
            // Retry sending the next buffer.
            zigbee_benchmark_send_ping_req(NULL);
            break;

        case TEST_FINISHED:
            UNUSED_RETURN_VALUE(benchmark_test_stop());
            break;

        case TEST_IN_PROGRESS_FRAME_SENT:
            if (m_test_status.packets_left_count > 0)
            {
                NRF_LOG_DEBUG("Test frame sent, prepare next frame.");
                m_state = TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER;
                zigbee_benchmark_send_ping_req(NULL);
            }
            else
            {
                NRF_LOG_DEBUG("Test frame sent, test finished.");
                m_state = TEST_FINISHED;
            }
            break;

        default:
            NRF_LOG_ERROR("Invalid test state. Fall back to the idle state.");
            m_test_status.test_in_progress = false;
            m_state = TEST_IDLE;
            break;
    }
}

uint64_t benchmark_local_device_id_get(void)
{
    eui64_map_addr_t dev_addr;

    zb_osif_get_ieee_eui64(dev_addr.arr_addr);

    return dev_addr.uint_addr;
}

uint32_t benchmark_peer_results_request_send(void)
{
    /* Peer control is not implemented in Zigbee benchmark. */
    return (uint32_t)RET_ERROR;
}
