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

#include "app_timer.h"
#include "test_transmission.h"
#include "benchmark_api.h"
#include "cpu_utilization.h"
#include "thread_utils.h"

#include "boards.h"
#include "nrf_log.h"

#include <openthread/coap.h>
#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/thread.h>

APP_TIMER_DEF(m_discovery_timer);

struct benchmark_address_context_t
{
    otIp6Address addr;
};

typedef enum
{
    TEST_START_REQUEST = 0, /**< Notify the receiver that we want to start the test. */
    TEST_STOP_REQUEST,      /**< Notify the receiver that we want to stop the test. */
} benchmark_ctrl_t;

typedef struct
{
    otCoapResource discovery_resource; /**< CoAP discovery resource. */
    otCoapResource test_ctrl_resource; /**< CoAP test control resource. */
    otCoapResource results_resource;   /**< CoAP results resource */
} thread_benchmark_resources_t;

typedef struct
{
    otIp6Address addr;
    uint64_t     device_id;
} thread_benchmark_peer_entry_t;

static uint32_t                      m_start_time;
static benchmark_evt_t               m_benchmark_evt;
static benchmark_result_t            m_local_result;
static benchmark_result_t            m_remote_result;
static benchmark_callback_t          mp_callback;
static benchmark_peer_db_t           m_peer_information;
static benchmark_address_context_t   m_peer_addresses[BENCHMARK_MAX_PEER_NUMBER];
static benchmark_configuration_t   * mp_test_configuration;
static benchmark_mac_counters_t      m_total_mac_counters;

static benchmark_status_t m_test_status =
{
    .test_in_progress   = false,
    .reset_counters     = false,
    .waiting_for_ack    = 0,
    .packets_left_count = 0,
    .acks_lost          = 0,
};

void benchmark_test_duration_calculate(void)
{
    m_local_result.duration = otPlatAlarmMilliGetNow() - m_start_time;
}

void benchmark_mac_counters_update(void)
{
    const otMacCounters * counters  = otLinkGetCounters(thread_ot_instance_get());

    uint32_t tx_errors = counters->mTxRetry + counters->mTxErrCca;

    /* Normally mTxTotal or mTxErrCca is increased but there is
     * a possibility that both values are increased at the same time,
     * this is indicated by increased mTxErrBusyChannel
     * so we need to substract it here.
     */
    uint32_t tx_total = counters->mTxTotal + tx_errors - counters->mTxErrBusyChannel;

    NRF_LOG_INFO("MacErr: %lu", m_total_mac_counters.error);
    NRF_LOG_INFO("MacCounterTotal: %lu", m_total_mac_counters.total);

    m_local_result.mac_tx_counters.total = tx_total - m_total_mac_counters.total;
    m_local_result.mac_tx_counters.error = tx_errors - m_total_mac_counters.error;

    m_total_mac_counters.total += m_local_result.mac_tx_counters.total;
    m_total_mac_counters.error += m_local_result.mac_tx_counters.error;

    NRF_LOG_INFO("Current MAC Counters values::");
    NRF_LOG_INFO("TxErrCca: %lu", counters->mTxErrCca);
    NRF_LOG_INFO("TxErrBussyChannel: %lu", counters->mTxErrBusyChannel);
    NRF_LOG_INFO("TxErrRetry: %lu", counters->mTxRetry);
    NRF_LOG_INFO("TxTotal: %lu", counters->mTxTotal);
    NRF_LOG_INFO("TxErrTotal: %lu", m_local_result.mac_tx_counters.error);
    NRF_LOG_INFO("TxAttemptTotal: %lu", m_local_result.mac_tx_counters.total);
    NRF_LOG_INFO("MacErr: %lu", m_total_mac_counters.error);
    NRF_LOG_INFO("MacCounterTotal: %lu", m_total_mac_counters.total);

}

uint64_t benchmark_local_device_id_get(void)
{
    uint32_t dev_id_hi = NRF_FICR->DEVICEID[1];
    uint32_t dev_id_lo = NRF_FICR->DEVICEID[0];

    return (((uint64_t)dev_id_hi) << 32) | dev_id_lo;
}

static void result_clear(void)
{
    memset(&m_local_result, 0, sizeof(m_local_result));
    memset(&m_remote_result, 0, sizeof(m_remote_result));

    benchmark_clear_latency(&m_test_status.latency);

    otMacCounters counters = *otLinkGetCounters(thread_ot_instance_get());

    m_local_result.rx_counters.rx_total  = counters.mRxTotal;
    m_local_result.rx_counters.rx_error  = counters.mRxErrNoFrame + counters.mRxErrFcs +
                                     counters.mRxErrSec + counters.mRxErrOther;
    cpu_utilization_clear();
}

static otError thread_benchmark_peer_discovery_response_send(void                * p_context,
                                                             const otMessageInfo * p_message_info)
{
    otError       error = OT_ERROR_NO_BUFS;
    otMessage   * p_response;
    otMessageInfo message_info;
    otInstance  * p_instance = thread_ot_instance_get();
    uint64_t      device_id;

    UNUSED_PARAMETER(p_context);

    do
    {
        p_response = otCoapNewMessage(p_instance, NULL);
        if (p_response == NULL)
        {
            break;
        }

        otCoapMessageInit(p_response, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapMessageGenerateToken(p_response, 2);
        UNUSED_RETURN_VALUE(otCoapMessageAppendUriPathOptions(p_response, "discovery"));
        UNUSED_RETURN_VALUE(otCoapMessageSetPayloadMarker(p_response));

        error = otMessageAppend(
            p_response, otThreadGetMeshLocalEid(thread_ot_instance_get()), sizeof(otIp6Address));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        device_id = benchmark_local_device_id_get();
        error = otMessageAppend(p_response, &device_id, sizeof(device_id));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));

        message_info.mPeerAddr    = p_message_info->mPeerAddr;
        message_info.mPeerPort    = p_message_info->mPeerPort;
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;

        error = otCoapSendResponse(p_instance, p_response, &message_info);

    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

static void thread_benchmark_peer_discovery_response_handler(void                * p_context,
                                                             otMessage           * p_message,
                                                             const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_message_info);

    thread_benchmark_peer_entry_t peer;
    uint16_t                      peer_number = m_peer_information.peer_count;
    uint8_t                       num_of_bytes_to_read = sizeof(peer);

    if (peer_number >= BENCHMARK_MAX_PEER_NUMBER)
    {
        NRF_LOG_INFO("Can't add peer to the list, list full");
        return;
    }

    NRF_LOG_INFO("Adding peer: %d", peer_number);

    uint16_t bytes_read = otMessageRead(p_message,
                                   otMessageGetOffset(p_message),
                                   &peer,
                                   num_of_bytes_to_read);

    m_peer_information.peer_table[peer_number].device_id = peer.device_id;
    m_peer_information.peer_table[peer_number].p_address = &m_peer_addresses[peer_number];

    memcpy(m_peer_information.peer_table[peer_number].p_address, &peer.addr, sizeof(otIp6Address));

    m_peer_information.peer_count++;

    if (bytes_read != num_of_bytes_to_read)
    {
        NRF_LOG_ERROR("Invalid discovery response read");
    }
}

static void thread_benchmark_peer_discovery_handler(void                * p_context,
                                                    otMessage           * p_message,
                                                    const otMessageInfo * p_message_info)
{
    // Information returning from the peer.
    if (otCoapMessageGetType(p_message) == OT_COAP_TYPE_NON_CONFIRMABLE)
    {
        if (otCoapMessageGetCode(p_message) == OT_COAP_CODE_PUT)
        {
            thread_benchmark_peer_discovery_response_handler(p_context, p_message, p_message_info);
        }

        // Information request to be processed on peer.
        if (otCoapMessageGetCode(p_message) == OT_COAP_CODE_GET)
        {
            otError err = thread_benchmark_peer_discovery_response_send(p_context, p_message_info);

            if (err == OT_ERROR_NONE)
            {
                NRF_LOG_INFO("Discovery response sent");
            }
        }
    }
}

static otError thread_benchmark_ctrl_response_send(void                * p_context,
                                                   otMessage           * p_request_message,
                                                   const otMessageInfo * p_message_info,
                                                   benchmark_ctrl_t      cmd)
{
    otError      error = OT_ERROR_NO_BUFS;
    otMessage  * p_response;

    do
    {
        p_response = otCoapNewMessage(thread_ot_instance_get(), NULL);
        if (p_response == NULL)
        {
            break;
        }

        otCoapMessageInit(p_response, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);
        otCoapMessageSetToken(p_response,
                             otCoapMessageGetToken(p_request_message),
                             otCoapMessageGetTokenLength(p_request_message));
        UNUSED_RETURN_VALUE(otCoapMessageSetPayloadMarker(p_response));

        error = otMessageAppend(p_response, &cmd, sizeof(benchmark_ctrl_t));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(thread_ot_instance_get(), p_response, p_message_info);
    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

static void frame_counters_calculate(void)
{
    otMacCounters counters = *otLinkGetCounters(thread_ot_instance_get());

    uint32_t rx_total = counters.mRxTotal;
    uint32_t rx_error = counters.mRxErrNoFrame + counters.mRxErrFcs +
                        counters.mRxErrSec + counters.mRxErrOther;

    m_local_result.rx_counters.rx_total  = rx_total - m_local_result.rx_counters.rx_total;
    m_local_result.rx_counters.rx_error  = rx_error - m_local_result.rx_counters.rx_error;
}

static otError thread_benchmark_results_response_send(void                * p_context,
                                                      otMessage           * p_request_message,
                                                      const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otMessage  * p_response;

    do
    {
        p_response = otCoapNewMessage(thread_ot_instance_get(), NULL);
        if (p_response == NULL)
        {
            break;
        }

        otCoapMessageInit(p_response, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);
        otCoapMessageSetToken(p_response,
                             otCoapMessageGetToken(p_request_message),
                             otCoapMessageGetTokenLength(p_request_message));
        UNUSED_RETURN_VALUE(otCoapMessageSetPayloadMarker(p_response));

        error = otMessageAppend(p_response, &m_local_result, sizeof(m_local_result));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(thread_ot_instance_get(), p_response, p_message_info);
    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

static void thread_benchmark_test_start_master(void)
{
    NRF_LOG_INFO("Peer received test start command, starting test");
    result_clear();
    m_start_time = otPlatAlarmMilliGetNow();
    UNUSED_RETURN_VALUE(cpu_utilization_start());

    m_test_status.test_in_progress = true;
    m_test_status.acks_lost        = 0;

    m_benchmark_evt.evt           = BENCHMARK_TEST_STARTED;
    m_benchmark_evt.context.error = OT_ERROR_NONE;

    mp_callback(&m_benchmark_evt);
}

static void thread_benchmark_test_stop_master(void)
{
    otError error = OT_ERROR_NONE;

    NRF_LOG_INFO("Peer received test stop command, requesting results")
    UNUSED_RETURN_VALUE(cpu_utilization_stop());
    m_local_result.cpu_utilization = cpu_utilization_get();

    if (mp_test_configuration == NULL)
    {
        return;
    }

    error = (otError)benchmark_peer_results_request_send();
    m_benchmark_evt.evt           = BENCHMARK_TEST_STOPPED;
    m_benchmark_evt.context.error = error;

    mp_callback(&m_benchmark_evt);
}

static void thread_benchmark_test_start_slave(void)
{
    NRF_LOG_INFO("Test start command received (slave)");
    result_clear();
    m_test_status.reset_counters = true;
    m_start_time = otPlatAlarmMilliGetNow();
    UNUSED_RETURN_VALUE(cpu_utilization_start());
}

static void thread_benchmark_test_stop_slave(void)
{
    NRF_LOG_INFO("Test stop command received (slave)");
    frame_counters_calculate();
    UNUSED_RETURN_VALUE(cpu_utilization_stop());

    benchmark_test_duration_calculate();
    benchmark_mac_counters_update();
    m_local_result.cpu_utilization = cpu_utilization_get();
}

static void thread_benchmark_ctrl_handler(void                * p_context,
                                          otMessage           * p_message,
                                          const otMessageInfo * p_message_info)
{
    benchmark_ctrl_t cmd;

    if ((otCoapMessageGetType(p_message) == OT_COAP_TYPE_CONFIRMABLE) &&
        (otCoapMessageGetCode(p_message) == OT_COAP_CODE_GET))
    {
        uint16_t bytes_read = otMessageRead(p_message, otMessageGetOffset(p_message), &cmd, sizeof(benchmark_ctrl_t));

        if (bytes_read != sizeof(benchmark_ctrl_t))
        {
            NRF_LOG_ERROR("Invalid test ctrl message read");
        }

        switch (cmd)
        {
            case TEST_START_REQUEST:
                thread_benchmark_test_start_slave();
                UNUSED_RETURN_VALUE(thread_benchmark_ctrl_response_send(p_context, p_message, p_message_info, cmd));
                break;

            case TEST_STOP_REQUEST:
                thread_benchmark_test_stop_slave();
                UNUSED_RETURN_VALUE(thread_benchmark_ctrl_response_send(p_context, p_message, p_message_info, cmd));
                break;

            default:
                NRF_LOG_ERROR("Unknown Test ctrl command received%d", cmd);
                break;
        }
    }
}

static void thread_benchmark_results_handler(void                * p_context,
                                             otMessage           * p_message,
                                             const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_message_info);

    if ((otCoapMessageGetType(p_message) == OT_COAP_TYPE_CONFIRMABLE) &&
        (otCoapMessageGetCode(p_message) == OT_COAP_CODE_GET))
    {
        NRF_LOG_INFO("Results request received");
        UNUSED_RETURN_VALUE(thread_benchmark_results_response_send(p_context, p_message, p_message_info));
    }
}

static thread_benchmark_resources_t m_thread_benchmark_resources = {
    .discovery_resource = {"discovery", thread_benchmark_peer_discovery_handler, NULL, NULL},
    .test_ctrl_resource = {"test_ctrl", thread_benchmark_ctrl_handler, NULL, NULL},
    .results_resource   = {"results", thread_benchmark_results_handler, NULL, NULL},
};

/**@brief   Function for discovering peers that can be used during the test. */
static otError thread_benchmark_peer_discovery_request_send(otInstance * p_instance)
{
    otError       error = OT_ERROR_NO_BUFS;
    otMessage   * p_request;
    otMessageInfo message_info;

    do
    {
        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapMessageGenerateToken(p_request, 2);

        error = otCoapMessageAppendUriPathOptions(p_request, "discovery");
        ASSERT(error == OT_ERROR_NONE);

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;

        error = otIp6AddressFromString("FF03::1", &message_info.mPeerAddr);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapSendRequest(
            p_instance, p_request, &message_info, NULL, p_instance);

        if (error != OT_ERROR_NONE)
        {
            break;
        }

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

    return error;
}

static void results_received(void                * p_context,
                             otMessage           * p_message,
                             const otMessageInfo * p_message_info,
                             otError               error)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_message_info);
    UNUSED_PARAMETER(error);

    int offset;

    // Response to the send results command.
    if ((otCoapMessageGetType(p_message) == OT_COAP_TYPE_NON_CONFIRMABLE &&
        (otCoapMessageGetCode(p_message) == OT_COAP_CODE_CONTENT)))
    {
        NRF_LOG_INFO("Results received");

        offset = otMessageGetOffset(p_message);

        uint16_t bytes_read = otMessageRead(p_message, offset, &m_remote_result, sizeof(m_remote_result));

        if (bytes_read != sizeof(m_remote_result))
        {
            NRF_LOG_ERROR("Invalid results response received");
        }

        frame_counters_calculate();

        m_benchmark_evt.evt                             = BENCHMARK_TEST_COMPLETED;
        m_benchmark_evt.context.results.p_remote_result = &m_remote_result;
        m_benchmark_evt.context.results.p_local_result  = &m_local_result;
        m_benchmark_evt.context.results.p_local_status  = &m_test_status;
        mp_callback(&m_benchmark_evt);
    }
}

/**@brief   Function sending test results request. */
static otError thread_benchmark_peer_results_request_send(otInstance * p_instance)
{
    otError       error = OT_ERROR_NO_BUFS;
    otMessage   * p_request;
    otMessageInfo message_info;
    NRF_LOG_INFO("Sending test results request");

    do
    {
        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapMessageGenerateToken(p_request, 2);

        error = otCoapMessageAppendUriPathOptions(p_request, "results");
        ASSERT(error == OT_ERROR_NONE);

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr    = m_peer_information.peer_table[m_peer_information.selected_peer].p_address->addr;

        error = otCoapSendRequest(p_instance, p_request, &message_info, results_received, p_instance);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

    return error;
}

static void command_response_received_handler(void                * p_context,
                                              otMessage           * p_message,
                                              const otMessageInfo * p_message_info,
                                              otError               error)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_message_info);

    benchmark_ctrl_t cmd;

    // Response to the test start/stop command.
    if (((otCoapMessageGetType(p_message) == OT_COAP_TYPE_NON_CONFIRMABLE) &&
        (otCoapMessageGetCode(p_message) == OT_COAP_CODE_CONTENT)))
    {
        uint16_t bytes_read = otMessageRead(p_message, otMessageGetOffset(p_message), &cmd, sizeof(benchmark_ctrl_t));

        if (bytes_read != sizeof(benchmark_ctrl_t))
        {
            NRF_LOG_ERROR("Invalid peer test command response read");
        }

        switch (cmd)
        {
            case TEST_START_REQUEST:
                thread_benchmark_test_start_master();
                break;

            case TEST_STOP_REQUEST:
                thread_benchmark_test_stop_master();
                break;

            default:
                NRF_LOG_INFO("Incorrect peer command response received: %d", cmd);
                break;
        }
    }
}

/**@brief   Function sending start/stop test request. */
static otError thread_benchmark_ctrl_request_send(otInstance * p_instance, benchmark_ctrl_t ctrl_cmd)
{
    otError       error = OT_ERROR_NO_BUFS;
    otMessage   * p_request;
    otMessageInfo message_info;

    do
    {
        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapMessageGenerateToken(p_request, 2);

        error = otCoapMessageAppendUriPathOptions(p_request, "test_ctrl");
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapMessageSetPayloadMarker(p_request);
        ASSERT(error == OT_ERROR_NONE);

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr    = m_peer_information.peer_table[m_peer_information.selected_peer].p_address->addr;

        error = otMessageAppend(p_request, &ctrl_cmd, sizeof(benchmark_ctrl_t));

        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendRequest(p_instance, p_request, &message_info, command_response_received_handler, p_instance);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

    return error;
}

static void discovery_timeout(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    m_benchmark_evt.evt                        = BENCHMARK_DISCOVERY_COMPLETED;
    m_benchmark_evt.context.p_peer_information = &m_peer_information;
    mp_callback(&m_benchmark_evt);
}

/**@brief   Function for initializing the CoAP resources used for test management. */
void benchmark_init(void)
{
    test_transmission_init();

    m_thread_benchmark_resources.test_ctrl_resource.mContext = thread_ot_instance_get();
    m_thread_benchmark_resources.discovery_resource.mContext = thread_ot_instance_get();
    m_thread_benchmark_resources.results_resource.mContext   = thread_ot_instance_get();

    mp_test_configuration = NULL;

    otError error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.test_ctrl_resource);
    ASSERT(error == OT_ERROR_NONE);

    error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.discovery_resource);
    ASSERT(error == OT_ERROR_NONE);

    error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.results_resource);
    ASSERT(error == OT_ERROR_NONE);

    uint32_t err_code = app_timer_create(&m_discovery_timer, APP_TIMER_MODE_SINGLE_SHOT, discovery_timeout);
    ASSERT(err_code == NRF_SUCCESS);
}

uint32_t benchmark_test_init(benchmark_configuration_t * p_configuration, benchmark_callback_t callback)
{
    mp_callback                      = callback;
    mp_test_configuration            = p_configuration;
    m_test_status.frame_number       = 1;
    m_test_status.packets_left_count = p_configuration->count;

    benchmark_clear_latency(&m_test_status.latency);

    return OT_ERROR_NONE;
}

uint32_t benchmark_peer_discover(void)
{
    memset(&m_peer_information, 0, sizeof(m_peer_information));

    otError error = thread_benchmark_peer_discovery_request_send(thread_ot_instance_get());

    uint32_t err_code = app_timer_start(m_discovery_timer, APP_TIMER_TICKS(BENCHMARK_DISCOVERY_TIMEOUT), NULL);
    ASSERT(err_code == NRF_SUCCESS);

    return (uint32_t)error;
}

const benchmark_peer_db_t * benchmark_peer_table_get(void)
{
    return &m_peer_information;
}

benchmark_status_t * benchmark_status_get(void)
{
    return &m_test_status;
}

uint32_t benchmark_test_start(void)
{
    uint32_t err_code = OT_ERROR_NONE;

    if (mp_test_configuration == NULL)
    {
        return OT_ERROR_INVALID_ARGS;
    }

    memset(&m_benchmark_evt, 0, sizeof(benchmark_evt_t));

    err_code = (uint32_t)thread_benchmark_ctrl_request_send(thread_ot_instance_get(), TEST_START_REQUEST);
    return err_code;
}

uint32_t benchmark_test_stop(void)
{
    uint32_t err_code = OT_ERROR_NONE;

    if (mp_test_configuration == NULL)
    {
        return OT_ERROR_INVALID_ARGS;
    }

    m_test_status.test_in_progress = false;

    benchmark_test_duration_calculate();
    benchmark_mac_counters_update();
    m_test_status.reset_counters = false;

    err_code = (uint32_t)thread_benchmark_ctrl_request_send(thread_ot_instance_get(), TEST_STOP_REQUEST);
    return err_code;
}


uint32_t benchmark_peer_results_request_send(void)
{
    return (uint32_t)thread_benchmark_peer_results_request_send(thread_ot_instance_get());
}

void benchmark_process(void)
{
    benchmark_test_state_t status = TEST_IDLE;

    if (mp_test_configuration == NULL)
    {
        return;
    }

    if (m_test_status.test_in_progress && m_test_status.packets_left_count)
    {
        status = test_transmission_packet_transmit(mp_test_configuration, &m_test_status);

        switch (status)
        {
        // fall through
        case TEST_IN_PROGRESS_WAITING_FOR_ACK:
        case TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER:
        case TEST_IDLE:
            break;

        case TEST_IN_PROGRESS_FRAME_SENT:
            NRF_LOG_DEBUG("Test frame sent");
            m_test_status.packets_left_count--;
            break;

        case TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK:
            NRF_LOG_DEBUG("Test frame sent, waiting for ACK");
            m_test_status.packets_left_count--;
            break;

        case TEST_ERROR:
            NRF_LOG_ERROR("Error occurred during the test transmission: %d\r\n Sent %d packets",
                          status,
                          (mp_test_configuration->count - m_test_status.packets_left_count));
            break;

        default:
            ASSERT(false);
            break;
        }
    }
    else if(m_test_status.test_in_progress)
    {
        benchmark_test_duration_calculate();
        benchmark_mac_counters_update();
        m_test_status.test_in_progress = false;

        NRF_LOG_INFO("All frames sent, sending test stop request to the peer");
        uint32_t err_code = thread_benchmark_ctrl_request_send(thread_ot_instance_get(), TEST_STOP_REQUEST);
        ASSERT(err_code == NRF_SUCCESS);
    }
}

benchmark_result_t * benchmark_local_result_get(void)
{
    return &m_local_result;
}
