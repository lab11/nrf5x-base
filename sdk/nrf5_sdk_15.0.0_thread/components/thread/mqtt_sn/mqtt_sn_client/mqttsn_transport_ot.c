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

#include "mqttsn_transport.h"
#include "mqttsn_packet_internal.h"
#include "mem_manager.h"
#include "nrf_log.h"
#include "nrf_error.h"

#include <openthread/udp.h>
#include <openthread/openthread.h>

#include <stdint.h>
#include <string.h>

#define NULL_PARAM_CHECK(PARAM)                                                                    \
    if ((PARAM) == NULL)                                                                           \
    {                                                                                              \
        return (NRF_ERROR_NULL);                                                                   \
    }

/**@brief OpenThread transport instance. */
static otInstance * m_p_instance;
/**@brief OpenThread UDP socket. */
static otUdpSocket  m_socket;

/**@brief Callback from the OpenThread port. */
static void port_data_callback(void                * p_context,
                               otMessage           * p_message,
                               const otMessageInfo * p_message_info)
{
    const mqttsn_port_t port = p_message_info->mSockPort;
    mqttsn_remote_t     remote_endpoint;

    memcpy(remote_endpoint.addr,  p_message_info->mPeerAddr.mFields.m8, OT_IP6_ADDRESS_SIZE);
    remote_endpoint.port_number = MQTTSN_DEFAULT_GATEWAY_PORT;

    uint16_t  payload_size = otMessageGetLength(p_message) - otMessageGetOffset(p_message);
    uint8_t * p_msg        = nrf_malloc(payload_size);

    if (p_msg)
    {
        if (otMessageRead(p_message, otMessageGetOffset(p_message), p_msg, payload_size) == payload_size)
        {
            if (mqttsn_transport_read(p_context, &port, &remote_endpoint, p_msg, payload_size) != 
                NRF_SUCCESS)
            {
                NRF_LOG_ERROR("MQTT-SN message could not be processed.\r\n");
            }
        }
        else
        {
            NRF_LOG_ERROR("Openthread message cannot be read.\r\n");
        }

        nrf_free(p_msg);
    }
    else
    {
        NRF_LOG_ERROR("Openthread message buffer cannot be allocated (receive_data_callback)\r\n");
    }
}

/**@brief Creates OpenThread network port. */
static uint32_t port_create(mqttsn_client_t * p_client, uint16_t port)
{
    otError    err_code;
    otSockAddr addr;

    memset(&addr, 0, sizeof(addr));

    err_code = otUdpOpen(m_p_instance, &m_socket, port_data_callback, p_client);
    if (err_code == OT_ERROR_NONE)
    {
        addr.mPort = port;
        err_code   = otUdpBind(&m_socket, &addr);
    }
    else
    {
        NRF_LOG_ERROR("UDP Socket cannot be opened\r\n");
    }

    return (err_code == OT_ERROR_NONE) ? NRF_SUCCESS : NRF_ERROR_INTERNAL;
}

uint32_t mqttsn_transport_init(mqttsn_client_t * p_client, uint16_t port, const void * p_context)
{
    m_p_instance = (otInstance *)p_context;
    return port_create(p_client, port);
}

uint32_t mqttsn_transport_write(mqttsn_client_t       * p_client,
                                const mqttsn_remote_t * p_remote,
                                const uint8_t         * p_data,
                                uint16_t                datalen)
{
    NULL_PARAM_CHECK(p_remote);
    NULL_PARAM_CHECK(p_data);

    uint32_t      err_code = NRF_ERROR_INVALID_STATE;
    otMessage   * p_msg    = NULL;
    otUdpSocket * p_socket = &m_socket;

    do
    {
        p_msg = otUdpNewMessage(m_p_instance, true);
        if (p_msg == NULL)
        {
            NRF_LOG_ERROR("Failed to allocate OT message\r\n");
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (otMessageAppend(p_msg, p_data, datalen) != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to append message payload\r\n");
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        otMessageInfo msg_info;

        memset(&msg_info, 0, sizeof(msg_info));
        msg_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        msg_info.mPeerPort    = p_remote->port_number;

        memcpy(msg_info.mPeerAddr.mFields.m8, p_remote->addr, OT_IP6_ADDRESS_SIZE);

        if (otUdpSend(p_socket, p_msg, &msg_info) != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Failed to send message\r\n");
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = NRF_SUCCESS;

    } while(0);

    if ((p_msg != NULL) && (err_code != NRF_SUCCESS))
    {
        otMessageFree(p_msg);
    }

    return err_code;
}

uint32_t mqttsn_transport_read(void                   * p_context,
                               const mqttsn_port_t    * p_port,
                               const mqttsn_remote_t  * p_remote,
                               const uint8_t          * p_data,
                               uint16_t                 datalen)
{
    mqttsn_client_t * p_client = (mqttsn_client_t *)p_context;
    return mqttsn_packet_receiver(p_client, p_port, p_remote, p_data, datalen);
}

uint32_t mqttsn_transport_uninit(mqttsn_client_t * p_client)
{
    return otUdpClose(&m_socket) == OT_ERROR_NONE ? NRF_SUCCESS : NRF_ERROR_INTERNAL;
}
