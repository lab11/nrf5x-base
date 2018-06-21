/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
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
#include "test_transmission_udp.h"
#include "thread_benchmark.h"
#include "thread_utils.h"

#include "boards.h"
#include "nrf_log.h"

#include <openthread/openthread.h>
#include <openthread/ip6.h>
#include <openthread/coap.h>
#include <openthread/platform/alarm-milli.h>

#define DISCOVERY_TIMEOUT 1000 /**< Time after which application stops waiting for discovery responses in ms */

APP_TIMER_DEF(m_discovery_timer);

static uint32_t                            m_start_time;
static thread_benchmark_evt_t              m_benchmark_evt;
static thread_benchmark_result_t           m_result;
static thread_benchmark_callback_t         mp_callback;
static thread_benchmark_peer_information_t m_peer_information;

typedef struct
{
    otCoapResource discovery_resource; /**< CoAP discovery resource. */
    otCoapResource test_ctrl_resource; /**< CoAP test control resource. */
    otCoapResource results_resource;   /**< CoAP results resource */
} thread_benchmark_resources_t;


static thread_benchmark_configuration_t m_test_configuration =
{
    .length      = 64,
    .ack_timeout = 5000,
    .count       = 1000,
    .mode        = THREAD_BENCHMARK_MODE_ACK,
};

static thread_benchmark_status_t m_test_status =
{
    .p_selected_peer    = NULL,
    .test_in_progress   = false,
    .waiting_for_ack    = 0,
    .packets_left_count = 0,
    .acks_lost          = 0,
};

uint64_t thread_benchmark_local_device_id_get(void)
{
    uint32_t dev_id_hi = NRF_FICR->DEVICEID[1];
    uint32_t dev_id_lo = NRF_FICR->DEVICEID[0];

    return (((uint64_t)dev_id_hi) << 32) | dev_id_lo;
}

static void result_clear(void)
{
    memset(&m_result, 0, sizeof(m_result));

    otMacCounters counters = *otLinkGetCounters(thread_ot_instance_get());

    m_result.rx_counters.rx_total  = counters.mRxTotal;
    m_result.rx_counters.rx_error  = counters.mRxErrNoFrame + counters.mRxErrFcs +
                                     counters.mRxErrSec + counters.mRxErrOther;
}

static otError thread_benchmark_peer_discovery_response_send(void                * p_context,
                                                             const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otCoapHeader header;
    otMessage  * p_response;
    uint64_t     device_id;

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapHeaderGenerateToken(&header, 2);
        UNUSED_VARIABLE(otCoapHeaderAppendUriPathOptions(&header, "discovery"));
        otCoapHeaderSetPayloadMarker(&header);

        p_response = otCoapNewMessage(thread_ot_instance_get(), &header);
        if (p_response == NULL)
        {
            break;
        }

        error = otMessageAppend(
            p_response, otThreadGetMeshLocalEid(thread_ot_instance_get()), sizeof(otIp6Address));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        device_id = thread_benchmark_local_device_id_get();
        error = otMessageAppend(p_response, &device_id, sizeof(device_id));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(p_context, p_response, p_message_info);

    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

static void thread_benchmark_peer_discovery_response_handler(void                * p_context,
                                                             otCoapHeader        * p_header,
                                                             otMessage           * p_message,
                                                             const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_header);
    UNUSED_VARIABLE(p_message_info);

    if (m_peer_information.current_peer_number >= THREAD_BENCHMARK_MAX_PEER_NUMBER)
    {
        NRF_LOG_INFO("Can't add peer to the list, list full");
        return;
    }

    NRF_LOG_INFO("Adding peer: %d", m_peer_information.current_peer_number);

    int bytes_read = otMessageRead(p_message,
                                   otMessageGetOffset(p_message),
                                   &m_peer_information.peer_table[m_peer_information.current_peer_number++],
                                   sizeof(thread_benchmark_peer_entry_t));

    if (bytes_read != sizeof(thread_benchmark_peer_entry_t))
    {
        NRF_LOG_ERROR("Invalid discovery response read");
    }
}

static void thread_benchmark_peer_discovery_handler(void                * p_context,
                                                    otCoapHeader        * p_header,
                                                    otMessage           * p_message,
                                                    const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_header);

    // Information returning from the peer.
    if (otCoapHeaderGetType(p_header) == OT_COAP_TYPE_NON_CONFIRMABLE)
    {
        if (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_PUT)
        {
            thread_benchmark_peer_discovery_response_handler(p_context, p_header, p_message, p_message_info);
        }

        // Information request to be processed on peer.
        if (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_GET)
        {
            otError err = thread_benchmark_peer_discovery_response_send(p_context, p_message_info);

            if (err == OT_ERROR_NONE)
            {
                NRF_LOG_INFO("Discovery response sent");
            }
        }
    }
}

static otError thread_benchmark_ctrl_response_send(void                  * p_context,
                                                   otCoapHeader          * p_request_header,
                                                   const otMessageInfo   * p_message_info,
                                                   thread_benchmark_ctrl_t cmd)
{
    otError      error = OT_ERROR_NO_BUFS;
    otCoapHeader header;
    otMessage  * p_response;

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);
        otCoapHeaderSetToken(&header,
                             otCoapHeaderGetToken(p_request_header),
                             otCoapHeaderGetTokenLength(p_request_header));
        otCoapHeaderSetPayloadMarker(&header);

        p_response = otCoapNewMessage(thread_ot_instance_get(), &header);
        if (p_response == NULL)
        {
            break;
        }

        error = otMessageAppend(p_response, &cmd, sizeof(thread_benchmark_ctrl_t));
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

    m_result.rx_counters.rx_total  = rx_total - m_result.rx_counters.rx_total;
    m_result.rx_counters.rx_error  = rx_error - m_result.rx_counters.rx_error;
}

static otError thread_benchmark_results_response_send(void                * p_context,
                                                      otCoapHeader        * p_request_header,
                                                      const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otCoapHeader header;
    otMessage  * p_response;

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);
        otCoapHeaderSetToken(&header,
                             otCoapHeaderGetToken(p_request_header),
                             otCoapHeaderGetTokenLength(p_request_header));
        otCoapHeaderSetPayloadMarker(&header);

        p_response = otCoapNewMessage(thread_ot_instance_get(), &header);
        if (p_response == NULL)
        {
            break;
        }

        error = otMessageAppend(p_response, &m_result.rx_counters, sizeof(m_result.rx_counters));
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

static void thread_benchmark_ctrl_handler(void                * p_context,
                                          otCoapHeader        * p_header,
                                          otMessage           * p_message,
                                          const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_header);
    UNUSED_VARIABLE(p_message);
    UNUSED_VARIABLE(p_message_info);

    thread_benchmark_ctrl_t cmd;

    if ((otCoapHeaderGetType(p_header) == OT_COAP_TYPE_CONFIRMABLE) &&
        (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_GET))
    {
        int bytes_read = otMessageRead(p_message, otMessageGetOffset(p_message), &cmd, sizeof(thread_benchmark_ctrl_t));

        if (bytes_read != sizeof(thread_benchmark_ctrl_t))
        {
            NRF_LOG_ERROR("Invalid test ctrl message read");
        }

        switch (cmd)
        {
            case TEST_START_REQUEST:
                NRF_LOG_INFO("Test start command received");
                result_clear();
                UNUSED_VARIABLE(thread_benchmark_ctrl_response_send(p_context, p_header, p_message_info, cmd));
                break;

            case TEST_STOP_REQUEST:
                NRF_LOG_INFO("Test stop command received");
                frame_counters_calculate();
                UNUSED_VARIABLE(thread_benchmark_ctrl_response_send(p_context, p_header, p_message_info, cmd));
                break;

            default:
                NRF_LOG_ERROR("Unknown Test ctrl command received%d", cmd);
                break;
        }
    }
}

static void thread_benchmark_results_handler(void                * p_context,
                                             otCoapHeader        * p_header,
                                             otMessage           * p_message,
                                             const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_header);
    UNUSED_VARIABLE(p_message);
    UNUSED_VARIABLE(p_message_info);

    if ((otCoapHeaderGetType(p_header) == OT_COAP_TYPE_CONFIRMABLE) &&
        (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_GET))
    {
        NRF_LOG_INFO("Results request received");
        UNUSED_VARIABLE(thread_benchmark_results_response_send(p_context, p_header, p_message_info));
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
    otCoapHeader  header;
    otMessage   * p_request;
    otMessageInfo message_info;

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapHeaderGenerateToken(&header, 2);

        error = otCoapHeaderAppendUriPathOptions(&header, "discovery");
        ASSERT(error == OT_ERROR_NONE);

        p_request = otCoapNewMessage(p_instance, &header);
        if (p_request == NULL)
        {
            break;
        }

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

static void per_calculate(void)
{
    m_result.per = 100.0 - ((100.0 * m_result.rx_counters.packets_received) /
                            (m_test_configuration.count - m_test_status.packets_left_count));
}

static void throughput_calculate(void)
{
    m_result.throughput = ((1000.0 * m_result.rx_counters.bytes_received) /
                                                m_result.duration);
}

static void results_received(void                * p_context,
                             otCoapHeader        * p_header,
                             otMessage           * p_message,
                             const otMessageInfo * p_message_info,
                             otError               error)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_message_info);
    UNUSED_PARAMETER(error);

    int offset;

    // Response to the send results command.
    if ((otCoapHeaderGetType(p_header) == OT_COAP_TYPE_NON_CONFIRMABLE &&
        (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_CONTENT)))
    {
        NRF_LOG_INFO("Results reveived");

        offset = otMessageGetOffset(p_message);

        int bytes_read = otMessageRead(p_message, offset, &m_result.rx_counters, sizeof(m_result.rx_counters));

        if (bytes_read != sizeof(m_result.rx_counters))
        {
            NRF_LOG_ERROR("Invalid results response read");
        }

        per_calculate();
        throughput_calculate();

        m_benchmark_evt.evt              = THREAD_BENCHMARK_TEST_COMPLETED;
        m_benchmark_evt.context.p_result = &m_result;
        mp_callback(&m_benchmark_evt);
    }
}

/**@brief   Function sending test results request. */
static otError thread_benchmark_peer_results_request_send(otInstance * p_instance)
{
    otError       error = OT_ERROR_NO_BUFS;
    otCoapHeader  header;
    otMessage   * p_request;
    otMessageInfo message_info;
    NRF_LOG_INFO("Sending test results request");

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapHeaderGenerateToken(&header, 2);

        error = otCoapHeaderAppendUriPathOptions(&header, "results");
        ASSERT(error == OT_ERROR_NONE);

        p_request = otCoapNewMessage(p_instance, &header);
        if (p_request == NULL)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr    = m_test_status.p_selected_peer->peer_address;

        error = otCoapSendRequest(p_instance, p_request, &message_info, results_received, p_instance);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

    return error;
}

static void command_response_received_handler(void                * p_context,
                                              otCoapHeader        * p_header,
                                              otMessage           * p_message,
                                              const otMessageInfo * p_message_info,
                                              otError               error)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_message_info);

    thread_benchmark_ctrl_t cmd;

    // Response to the test start/stop command.
    if (((otCoapHeaderGetType(p_header) == OT_COAP_TYPE_NON_CONFIRMABLE) &&
        (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_CONTENT)))
    {
        int bytes_read = otMessageRead(p_message, otMessageGetOffset(p_message), &cmd, sizeof(thread_benchmark_ctrl_t));

        if (bytes_read != sizeof(thread_benchmark_ctrl_t))
        {
            NRF_LOG_ERROR("Invalid peer test comand response read");
        }

        switch (cmd)
        {
            case TEST_START_REQUEST:
                NRF_LOG_INFO("Peer received test start command, starting test");
                result_clear();
                m_start_time = otPlatAlarmMilliGetNow();

                m_test_status.test_in_progress = true;
                m_test_status.acks_lost        = 0;

                m_benchmark_evt.evt           = THREAD_BENCHMARK_TEST_STARTED;
                m_benchmark_evt.context.error = OT_ERROR_NONE;

                mp_callback(&m_benchmark_evt);
                break;

            case TEST_STOP_REQUEST:
                NRF_LOG_INFO("Peer received test stop command, requesting results");

                error = thread_benchmark_peer_results_request_send(thread_ot_instance_get());

                m_benchmark_evt.evt           = THREAD_BENCHMARK_TEST_STOPPED;
                m_benchmark_evt.context.error = error;

                mp_callback(&m_benchmark_evt);
                break;

            default:
                NRF_LOG_INFO("Incorrect peer command response received: %d", cmd);
                break;
        }
    }
}

/**@brief   Function sending start/stop test request. */
static otError thread_benchmark_ctrl_request_send(otInstance * p_instance, thread_benchmark_ctrl_t ctrl_cmd)
{
    otError       error = OT_ERROR_NO_BUFS;
    otCoapHeader  header;
    otMessage   * p_request;
    otMessageInfo message_info;

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapHeaderGenerateToken(&header, 2);

        error = otCoapHeaderAppendUriPathOptions(&header, "test_ctrl");
        ASSERT(error == OT_ERROR_NONE);

        otCoapHeaderSetPayloadMarker(&header);

        p_request = otCoapNewMessage(p_instance, &header);
        if (p_request == NULL)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr    = m_test_status.p_selected_peer->peer_address;

        error = otMessageAppend(p_request, &ctrl_cmd, sizeof(thread_benchmark_ctrl_t));

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
    m_benchmark_evt.evt                        = THREAD_BENCHMARK_DISCOVERY_COMPLETED;
    m_benchmark_evt.context.p_peer_information = &m_peer_information;
    mp_callback(&m_benchmark_evt);
}

/**@brief   Function for initializing the CoAP resources used for test management. */
void thread_benchmark_init(void)
{
    test_transmission_udp_init();

    m_thread_benchmark_resources.test_ctrl_resource.mContext = thread_ot_instance_get();
    m_thread_benchmark_resources.discovery_resource.mContext = thread_ot_instance_get();
    m_thread_benchmark_resources.results_resource.mContext   = thread_ot_instance_get();

    otError error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.test_ctrl_resource);
    ASSERT(error == OT_ERROR_NONE);

    error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.discovery_resource);
    ASSERT(error == OT_ERROR_NONE);

    error = otCoapAddResource(thread_ot_instance_get(), &m_thread_benchmark_resources.results_resource);
    ASSERT(error == OT_ERROR_NONE);

    uint32_t err_code = app_timer_create(&m_discovery_timer, APP_TIMER_MODE_SINGLE_SHOT, discovery_timeout);
    ASSERT(err_code == NRF_SUCCESS);
}

void thread_benchmark_test_init(thread_benchmark_callback_t callback)
{
    mp_callback                      = callback;
    m_test_status.frame_number       = 1;
    m_test_status.packets_left_count = m_test_configuration.count;
}

otError thread_benchmark_peer_discover(void)
{
    memset(&m_peer_information, 0, sizeof(m_peer_information));

    otError error = thread_benchmark_peer_discovery_request_send(thread_ot_instance_get());

    uint32_t err_code = app_timer_start(m_discovery_timer, APP_TIMER_TICKS(DISCOVERY_TIMEOUT), NULL);
    ASSERT(err_code == NRF_SUCCESS);

    return error;
}

const thread_benchmark_peer_information_t * thread_benchmark_peer_table_get(void)
{
    return &m_peer_information;
}

thread_benchmark_configuration_t * thread_benchmark_configuration_get(void)
{
    return &m_test_configuration;
}

thread_benchmark_status_t * thread_benchmark_status_get(void)
{
    return &m_test_status;
}

void thread_benchmark_peer_select(uint16_t peer_index)
{
    if (peer_index > (m_peer_information.current_peer_number - 1))
    {
        NRF_LOG_ERROR("Peer index out of bounds");
        return;
    }

    m_test_status.p_selected_peer = &m_peer_information.peer_table[peer_index];
}

const thread_benchmark_peer_entry_t * thread_benchmark_peer_selected_get(void)
{
    return m_test_status.p_selected_peer;
}

uint16_t thread_benchmark_peer_selected_id_get(void)
{
    uint16_t selected_peer_id = 0;

    if (m_test_status.p_selected_peer)
    {
        selected_peer_id = (m_test_status.p_selected_peer - m_peer_information.peer_table);
    }

    return selected_peer_id;
}

otError thread_benchmark_peer_ctrl_request_send(thread_benchmark_ctrl_t cmd)
{
    return thread_benchmark_ctrl_request_send(thread_ot_instance_get(), cmd);
}

otError thread_benchmark_results_request_send(void)
{
    return thread_benchmark_peer_results_request_send(thread_ot_instance_get());
}

void thread_benchmark_process(void)
{
    thread_benchmark_test_status_t status = TEST_IDLE;
    
    if (m_test_status.test_in_progress && m_test_status.packets_left_count)
    {
        status = test_transmission_udp_packet_transmit(&m_test_configuration, &m_test_status);

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
            NRF_LOG_ERROR("Error occured during the test transmission: %d\r\n Sent %d packets",
                          status,
                          (m_test_configuration.count - m_test_status.packets_left_count));
            break;

        default:
            ASSERT(false);
            break;
        }
    }
    else if(m_test_status.test_in_progress)
    {
        NRF_LOG_INFO("All frames sent, sending test stop request to the peer");
        uint32_t err_code = thread_benchmark_ctrl_request_send(thread_ot_instance_get(), TEST_STOP_REQUEST);
        ASSERT(err_code == NRF_SUCCESS);

        thread_benchmark_test_duration_calculate();
        
        m_test_status.test_in_progress = false;
    }
}

thread_benchmark_result_t * thread_benchmark_result_get(void)
{
    return &m_result;
}

void thread_benchmark_test_duration_calculate(void)
{
    NRF_LOG_INFO("Duration calculate\r\n");
    m_result.duration = otPlatAlarmMilliGetNow() - m_start_time;
}
