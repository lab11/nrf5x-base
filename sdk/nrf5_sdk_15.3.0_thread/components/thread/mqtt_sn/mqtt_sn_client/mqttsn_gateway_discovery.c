/**
 * Copyright (c) 2017 - 2019, Nordic Semiconductor ASA
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

#include "mqttsn_packet_internal.h"
#include "mqttsn_client.h"
#include "mqttsn_platform.h"
#include "nrf_error.h"
#include "nrf_log.h"

#define SEC_TO_MILLISEC(PARAM) (PARAM * 1000)

typedef struct discovery_timeout_t
{
    bool rnd_jitter_timeout_reached;
    bool search_gw_timeout_reached;
} discovery_timeout_t;

static discovery_timeout_t m_timeout;

static void discovery_event_handle(mqttsn_client_t * p_client, mqttsn_event_searchgw_t discovery_event)
{
    mqttsn_event_t evt;
    memset(&evt, 0, sizeof(mqttsn_event_t));

    evt.event_id             = MQTTSN_EVENT_SEARCHGW_TIMEOUT;
    evt.event_data.discovery = discovery_event;

    p_client->evt_handler(p_client, &evt);
}

uint32_t mqttsn_client_search_gateway(mqttsn_client_t * p_client, uint32_t timeout_s)
{
    if (p_client == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (p_client->client_state != MQTTSN_CLIENT_DISCONNECTED)
    {
        return NRF_ERROR_FORBIDDEN;
    }

    if (p_client->gateway_discovery.pending)
    {
        return NRF_ERROR_BUSY;
    }

    uint32_t rnd_jitter = mqttsn_platform_rand(MQTTSN_SEARCH_GATEWAY_MAX_DELAY_IN_MS);
    p_client->gateway_discovery.rnd_jitter_timeout = mqttsn_platform_timer_set_in_ms(rnd_jitter);

    m_timeout.rnd_jitter_timeout_reached = false;
    m_timeout.search_gw_timeout_reached  = false;

    p_client->gateway_discovery.found   = false;
    p_client->gateway_discovery.pending = true;
    p_client->gateway_discovery.search_gw_timeout =
        mqttsn_platform_timer_set_in_ms(SEC_TO_MILLISEC(timeout_s));

    uint32_t err_code = mqttsn_client_timeout_schedule(p_client);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("SEARCH GATEWAY message could not be sent. Error code: 0x%x\r\n", err_code);
        discovery_event_handle(p_client, MQTTSN_SEARCH_GATEWAY_PLATFORM_FAILED);
    }

    mqttsn_client_state_update(p_client, SENT_SEARCHGW);

    return NRF_SUCCESS;
}

void mqttsn_client_gateway_discovery_start(mqttsn_client_t * p_client)
{
    /* The random jitter has just passed. Send SEARCH GATEWAY message. */
    if (!m_timeout.rnd_jitter_timeout_reached)
    {
        m_timeout.rnd_jitter_timeout_reached = true;

        uint32_t err_code = mqttsn_packet_sender_searchgw(p_client);
        if (err_code != NRF_SUCCESS)
        {
            NRF_LOG_ERROR("SEARCH GATEWAY message could not be sent. Error code: 0x%x\r\n", err_code);
            discovery_event_handle(p_client, MQTTSN_SEARCH_GATEWAY_TRANSPORT_FAILED);
        }
    }
}

void mqttsn_client_gateway_discovery_finish(mqttsn_client_t * p_client)
{
    /* The gateway discovery timeout has passed. */
    if (!m_timeout.search_gw_timeout_reached)
    {
        m_timeout.search_gw_timeout_reached = true;

        /* Gateway discovery procedure is over. */
        p_client->gateway_discovery.pending = false;

        /* No gateways have been found. Notify the application. */
        if (!p_client->gateway_discovery.found)
        {
            mqttsn_client_state_update(p_client, TIMEOUT_SEARCHGW);

            discovery_event_handle(p_client, MQTTSN_SEARCH_GATEWAY_NO_GATEWAY_FOUND);
        }
        /* Gateway discovery finished succesfully. Notify the application. */
        else
        {
            discovery_event_handle(p_client, MQTTSN_SEARCH_GATEWAY_FINISHED);
        }
    }
}
