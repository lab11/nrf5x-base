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
/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#include "app_timer.h"
#include "app_scheduler.h"
#include "benchmark_api.h"
#include "nordic_common.h"
#include "nrf_assert.h"
#include "nrf_log.h"
#include "test_transmission.h"
#include "cpu_utilization.h"
#include "thread_utils.h"
#include "iot_defines.h"

#include <openthread/ip6.h>
#include <openthread/platform/alarm-micro.h>
#include <openthread/thread.h>
#include <openthread/udp.h>

#define TEST_TRANSMISSION_UDP_PAYLOAD            0xAA  /**< Sequence that is used to fill the application payload space. */
#define TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE   1200  /**< Maximum size of UDP payload in the test packet [B].  */
#define TEST_TRANSMISSION_UDP_FREE_BUFFERS_MIN   10    /**< Minimal number of free OT buffer slots so the test does not consume all available slots.  */
#define TEST_TRANSMISSION_UDP_SOURCE_PORT        22222 /**< Peer UDP port used as the source port for all tests. */
#define TEST_TRANSMISSION_UDP_LISTENER_PORT      33333 /**< Peer UDP port used for the UNIDIRECTIONAL test. */
#define TEST_TRANSMISSION_UDP_REPEATER_PORT      44444 /**< Peer UDP port used for the ECHO test. */
#define TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT 55555 /**< Peer UDP port used for the ACK reply test. */

APP_TIMER_DEF(m_ack_timer);

/**@brief Structure storing information about current test frame payload. */
typedef union
{
    uint8_t  payload[TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE];
    uint32_t current_frame_number;
} frame_payload_t;

/**@brief Structure storing information about current test frame used in sending acknowledgments. */
typedef struct
{
    uint16_t          payload_length;
    frame_payload_t * p_payload;
    otMessageInfo     message_info;
} frame_information_t;

static otUdpSocket m_source_udp_socket;
static otUdpSocket m_listener_udp_socket;
static otUdpSocket m_repeater_udp_socket;
static otUdpSocket m_ack_responder_udp_socket;

static benchmark_configuration_t * mp_test_configuration;

static uint32_t m_send_timestamp;

static frame_payload_t     m_frame_payload;
static frame_information_t m_frame_information =
{
    .payload_length = 0,
    .p_payload      = &m_frame_payload,
};

static inline void increment_result_counters(benchmark_result_t * p_result, otMessage * p_message)
{
    p_result->rx_counters.packets_received++;
    p_result->rx_counters.bytes_received += (otMessageGetLength(p_message) -
                                             otMessageGetOffset(p_message));
}

/**@brief This function is called when UDP message is received.
 *
 * @param[inout] p_context       Pointer to application specific context.
 * @param[in]    p_message       Pointer to OpenThread message buffer.
 * @param[in]    p_message_info  Pointer to OpenThread message information.
 */
static void udp_source_receive_callback(void                * p_context,
                                        otMessage           * p_message,
                                        const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_message_info);

    benchmark_status_t * p_status = benchmark_status_get();
    uint32_t             current_frame_number;

    if (mp_test_configuration == NULL)
    {
        // There is no ongoing transmission.
        return;
    }

    if ((mp_test_configuration->mode == BENCHMARK_MODE_ACK) ||
        (mp_test_configuration->mode == BENCHMARK_MODE_ECHO))
    {
        uint16_t read = otMessageRead(p_message,
                                      otMessageGetOffset(p_message),
                                      &current_frame_number,
                                      sizeof(current_frame_number));
        ASSERT(read == sizeof(current_frame_number));

        current_frame_number = NTOHL(current_frame_number);

        if (p_status->waiting_for_ack == current_frame_number)
        {
            benchmark_update_latency(&p_status->latency, (otPlatAlarmMicroGetNow() - m_send_timestamp) / 2);
            NRF_LOG_DEBUG("ACK received: 0x%08x\r\n", current_frame_number);
            UNUSED_RETURN_VALUE(app_timer_stop(m_ack_timer));
            p_status->waiting_for_ack = 0;

            if (current_frame_number == mp_test_configuration->count)
            {
                // Recalculate test duration, last ACK arrives after the test finishes.
                benchmark_test_duration_calculate();
                NRF_LOG_INFO("Last ACK received!\r\n");
            }
        }
        else
        {
            NRF_LOG_ERROR("Unexpected ACK received: 0x%08x\r\n", current_frame_number);
        }
    }
    else
    {
        NRF_LOG_ERROR("Unexpected message received\r\n");
    }
}

/**@brief This function is used to send the ACK or ECHO response.
 *
 * @param[in]    p_event_data Pointer to the event data.
 * @param[in]    event_size   Size of the event.
 */
static void send_response(void * p_event_data, uint16_t event_size)
{
    otError             err_code       = OT_ERROR_NONE;
    otMessage         * p_message      = NULL;
    otUdpSocket       * p_ack_socket   = NULL;
    otMessageInfo       message_info;
    uint32_t            payload_length = 0;

    do
    {
        p_message = otUdpNewMessage(thread_ot_instance_get(), NULL);
        if (p_message == NULL)
        {
            NRF_LOG_ERROR("Failed to allocate message for UDP port.\r\n");
            break;
        }

        memset(&message_info, 0, sizeof(message_info));

        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = m_frame_information.message_info.mPeerPort;
        message_info.mPeerAddr    = m_frame_information.message_info.mPeerAddr;
        message_info.mSockPort    = m_frame_information.message_info.mSockPort;
        message_info.mSockAddr    = *otThreadGetMeshLocalEid(thread_ot_instance_get());

        if (m_frame_information.message_info.mSockPort == TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT)
        {
            p_ack_socket   = &m_ack_responder_udp_socket;
            payload_length = sizeof(m_frame_information.p_payload->current_frame_number);
        }
        else if (m_frame_information.message_info.mSockPort == TEST_TRANSMISSION_UDP_REPEATER_PORT)
        {
            p_ack_socket   = &m_repeater_udp_socket;
            payload_length = m_frame_information.payload_length;
        }
        else
        {
            ASSERT(false);
        }

        err_code = otMessageAppend(p_message, m_frame_information.p_payload, payload_length);
        if (err_code != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to append message payload for UDP port.\r\n");
            break;
        }

        err_code = otUdpSend(p_ack_socket, p_message, &message_info);
        if (err_code != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to send UDP message.\r\n");
            break;
        }
        else
        {
            NRF_LOG_DEBUG("Response sent!\r\n");
        }

    } while (false);

    if ((p_message != NULL) && (err_code != OT_ERROR_NONE))
    {
        otMessageFree(p_message);
    }
}

/**@brief This function is called when UDP message is handled.
 *
 * @param[inout] p_context       Pointer to application specific context.
 * @param[in]    p_message       Pointer to OpenThread message buffer.
 * @param[in]    p_message_info  Pointer to OpenThread message information.
 */
static void udp_destination_receive_callback(void                * p_context,
                                             otMessage           * p_message,
                                             const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);

    benchmark_status_t * p_status      = benchmark_status_get();
    benchmark_result_t * p_result      = benchmark_local_result_get();
    m_frame_information.payload_length = otMessageGetLength(p_message) - otMessageGetOffset(p_message);
    m_frame_information.message_info   = *p_message_info;
    uint32_t err_code                  = NRF_SUCCESS;

    uint16_t read = otMessageRead(p_message, otMessageGetOffset(p_message), m_frame_information.p_payload->payload, m_frame_information.payload_length);
    ASSERT(read == m_frame_information.payload_length);

    if (p_status->reset_counters)
    {
        // First packet in slave mode received.
        cpu_utilization_clear();
        p_status->reset_counters = false;
    }

    // Update CPU utilization value in order to get more accurate values inside slave mode.
    p_result->cpu_utilization = cpu_utilization_get();

    increment_result_counters(p_result, p_message);

    switch (p_message_info->mSockPort)
    {
        case TEST_TRANSMISSION_UDP_LISTENER_PORT:
            NRF_LOG_DEBUG("UDP listener received packets: %d, bytes: %d, frame_number: 0x%08x\r\n",
                 p_result->rx_counters.packets_received,
                 p_result->rx_counters.bytes_received,
                 NTOHL(m_frame_information.p_payload->current_frame_number));
            break;

        case TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT:
            NRF_LOG_DEBUG("UDP ACK responder received packets: %d, bytes: %d, frame_number: 0x%08x\r\n",
                         p_result->rx_counters.packets_received,
                         p_result->rx_counters.bytes_received,
                         NTOHL(m_frame_information.p_payload->current_frame_number));

            err_code = app_sched_event_put(&m_frame_information, sizeof(m_frame_information), send_response);
            ASSERT(err_code == NRF_SUCCESS);
            break;

        case TEST_TRANSMISSION_UDP_REPEATER_PORT:
            NRF_LOG_DEBUG("UDP repeater received packets: %d, bytes: %d, frame_number: 0x%08x\r\n",
                         p_result->rx_counters.packets_received,
                         p_result->rx_counters.bytes_received,
                         NTOHL(m_frame_information.p_payload->current_frame_number));

            err_code = app_sched_event_put(&m_frame_information, sizeof(m_frame_information), send_response);
            ASSERT(err_code == NRF_SUCCESS);
            break;


        default:
            ASSERT(false);
            break;
    }
}

/**@brief This function is used to create the UDP port and connect reception on this port with given callback function. */
static void udp_port_create(otUdpSocket * p_socket, uint16_t port, otUdpReceive callback)
{
    otError    error = OT_ERROR_NONE;
    otSockAddr addr;

    memset(&addr, 0, sizeof(addr));
    memset(p_socket, 0, sizeof(otSockAddr));

    error = otUdpOpen(thread_ot_instance_get(), p_socket, callback, NULL);
    if (error == OT_ERROR_NONE)
    {
        addr.mPort = port;
        error      = otUdpBind(p_socket, &addr);
    }

    if (error != OT_ERROR_NONE)
    {
        NRF_LOG_INFO("Failed to create UDP port: %d\r\n", error);
    }

    ASSERT(error == OT_ERROR_NONE);
}

/**@brief Function that checks if the next packet can be added to send queue. */
static bool is_buffer_capacity_available(void)
{
    otBufferInfo buffer_info;
    otMessageGetBufferInfo(thread_ot_instance_get(), &buffer_info);

    return ((buffer_info.mFreeBuffers >= TEST_TRANSMISSION_UDP_FREE_BUFFERS_MIN) ? true : false);
}

/**@brief Function that returns receiver UDP port number for specified test modes. */
static uint16_t peer_port_get(benchmark_mode_t test_mode)
{
    uint16_t port = 0;

    switch (test_mode)
    {
        case BENCHMARK_MODE_UNIDIRECTIONAL:
            port = TEST_TRANSMISSION_UDP_LISTENER_PORT;
            break;

        case BENCHMARK_MODE_ECHO:
            port = TEST_TRANSMISSION_UDP_REPEATER_PORT;
            break;

        case BENCHMARK_MODE_ACK:
            port = TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT;
            break;

        default:
            ASSERT(false);
            break;
    }

    return port;
}

/**@brief This function is called when acknowledgment does not arrive within ack_timeout. */
static void ack_timeout_handler(void * p_context)
{
    benchmark_status_t * p_test_status = p_context;
    if (p_test_status->waiting_for_ack)
    {
        p_test_status->waiting_for_ack = 0;
        p_test_status->acks_lost++;

        NRF_LOG_INFO("Missed ACK: frame_number: 0x%04x", p_test_status->frame_number);
    }
}

/**@brief This function is used to create sockets for different test modes. */
void test_transmission_init(void)
{
    uint32_t err_code;

    mp_test_configuration = NULL;

    udp_port_create(&m_source_udp_socket,
                    TEST_TRANSMISSION_UDP_SOURCE_PORT,
                    udp_source_receive_callback);

    udp_port_create(&m_listener_udp_socket,
                    TEST_TRANSMISSION_UDP_LISTENER_PORT,
                    udp_destination_receive_callback);

    udp_port_create(&m_repeater_udp_socket,
                    TEST_TRANSMISSION_UDP_REPEATER_PORT,
                    udp_destination_receive_callback);

    udp_port_create(&m_ack_responder_udp_socket,
                    TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT,
                    udp_destination_receive_callback);

    err_code = app_timer_create(&m_ack_timer, APP_TIMER_MODE_SINGLE_SHOT, ack_timeout_handler);
    ASSERT(err_code == NRF_SUCCESS);
}

/**@brief Function for sending UDP test packets. */
benchmark_test_state_t test_transmission_packet_transmit(benchmark_configuration_t * p_test_conf,
                                                         benchmark_status_t        * p_test_status)
{
    otError                        err_code  = OT_ERROR_NONE;
    otMessage                    * p_message = NULL;
    const benchmark_peer_entry_t * p_selcected_peer = NULL;
    otMessageInfo                  message_info;
    benchmark_test_state_t         status;

    mp_test_configuration = p_test_conf;

    if (p_test_status->waiting_for_ack)
    {
        return TEST_IN_PROGRESS_WAITING_FOR_ACK;
    }

    if (!is_buffer_capacity_available())
    {
        return TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER;
    }

    if (p_test_conf->length > TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE)
    {
        NRF_LOG_ERROR("Too big payload size defined. Using maximal supported value: %d\r\n", TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE);

        p_test_conf->length = TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE;
    }

    uint8_t payload[TEST_TRANSMISSION_UDP_MAX_PAYLOAD_SIZE];
    memset(payload, TEST_TRANSMISSION_UDP_PAYLOAD, sizeof(char) * p_test_conf->length);

    if ((p_test_conf->mode == BENCHMARK_MODE_ECHO) || (p_test_conf->mode == BENCHMARK_MODE_ACK))
    {
        if (sizeof(char) * p_test_conf->length < sizeof(p_test_status->frame_number))
        {
            NRF_LOG_ERROR("Too small payload size for chosen mode defined. Using minimal required value: %d\r\n", sizeof(p_test_status->frame_number));

            p_test_conf->length = sizeof(p_test_status->frame_number);
        }
    }

    if (sizeof(char) * p_test_conf->length >= sizeof(p_test_status->frame_number))
    {
        *(uint32_t *)payload = HTONL(p_test_status->frame_number);
    }

    do
    {
        p_message = otUdpNewMessage(thread_ot_instance_get(), NULL);

        if (p_message == NULL)
        {
            NRF_LOG_ERROR("Failed to allocate message for UDP port.\r\n");
            status = TEST_ERROR;
            break;
        }

        err_code = otMessageAppend(p_message, payload, p_test_conf->length);

        if (err_code != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to append message payload for UDP port.\r\n");
            status = TEST_ERROR;
            break;
        }

        p_selcected_peer = benchmark_peer_selected_get();
        if (p_selcected_peer == NULL)
        {
            NRF_LOG_ERROR("There is no selected peer.\r\n");
            status = TEST_ERROR;
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = peer_port_get(p_test_conf->mode);
        message_info.mPeerAddr    = *(otIp6Address *)(p_selcected_peer->p_address);
        message_info.mSockAddr    = *otThreadGetMeshLocalEid(thread_ot_instance_get());

        m_send_timestamp = otPlatAlarmMicroGetNow();

        // Use the source socket to transmit UDP message, acknowledgments will be send back to the same port.
        err_code = otUdpSend(&m_source_udp_socket, p_message, &message_info);
        status = TEST_IN_PROGRESS_FRAME_SENT;

        if (err_code != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to send UDP message.\r\n");
            status = TEST_ERROR;
            break;
        }

        if ((p_test_conf->mode == BENCHMARK_MODE_ECHO) || (p_test_conf->mode == BENCHMARK_MODE_ACK))
        {
            p_test_status->waiting_for_ack = p_test_status->frame_number++;
            uint32_t error_code = app_timer_start(m_ack_timer, APP_TIMER_TICKS(p_test_conf->ack_timeout), p_test_status);
            ASSERT(error_code == NRF_SUCCESS);

            status = TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK;
        }
    } while(false);

    if ((p_message != NULL) && (err_code != OT_ERROR_NONE))
    {
        otMessageFree(p_message);
    }

    return status;
}
/** @}
 *  @endcond
 */
