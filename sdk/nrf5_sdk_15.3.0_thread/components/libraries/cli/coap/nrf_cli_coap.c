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

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(NRF_CLI_COAP)
#include "nrf_cli_coap.h"
#include "nrf_assert.h"
#include "nrf_log.h"

#include <openthread/coap.h>
#include <openthread/ip6.h>
#include <openthread/message.h>
#include <openthread/thread.h>

static void nrf_cli_coap_cmd_request_handler(void                * p_context,
                                             otMessage           * p_message,
                                             const otMessageInfo * p_message_info);

static void nrf_cli_coap_resp_request_handler(void                * p_context,
                                              otMessage           * p_message,
                                              const otMessageInfo * p_message_info);

static void cli_coap_send_to_peer(nrf_cli_coap_internal_t const * p_internal,
                                  otCoapResource                * p_resource,
                                  const void                    * p_data,
                                  size_t                          length);

static nrf_cli_coap_resources_t m_coap_resources = {
    .m_cmd_resource  = {"remote/cmd", nrf_cli_coap_cmd_request_handler, NULL, NULL},
    .m_resp_resource = {"remote/resp", nrf_cli_coap_resp_request_handler, NULL, NULL}
};

static nrf_cli_coap_internal_t * mp_instances_head            = NULL;
static const nrf_cli_t         * mp_response_cli              = NULL;
static bool                      m_coap_resources_initialized = false;

static otInstance * ot_instance_get(const nrf_cli_coap_internal_t * p_internal)
{
    return p_internal->p_cb->p_ot_instance;
}

static nrf_cli_coap_internal_t * cli_instance_get_before(nrf_cli_coap_internal_t * p_instance)
{
    nrf_cli_coap_internal_t * p_current = mp_instances_head;

    while (p_current && (p_current->p_cb->p_next != p_instance))
    {
        p_current = p_current->p_cb->p_next;
    }

    return p_current;
}

static void cli_instance_add(nrf_cli_coap_internal_t * p_instance)
{
    nrf_cli_coap_internal_t * p_last = cli_instance_get_before(NULL);

    p_instance->p_cb->p_next = NULL;

    if (p_last)
    {
        p_last->p_cb->p_next = p_instance;
    }
    else
    {
        mp_instances_head = p_instance;
    }
}

static void cli_instance_rm(nrf_cli_coap_internal_t * p_instance)
{
    nrf_cli_coap_internal_t * p_prev = cli_instance_get_before(p_instance);

    if (p_prev)
    {
        p_prev->p_cb->p_next = p_instance->p_cb->p_next;
    }
    else
    {
        mp_instances_head = NULL;
    }
}

static nrf_cli_coap_internal_t * cli_instance_find_by_peer(const otIp6Address * p_peer_address)
{
    nrf_cli_coap_internal_t * p_current = mp_instances_head;

    while (p_current && !otIp6IsAddressEqual(&p_current->p_cb->peer_address, p_peer_address))
    {
        p_current = p_current->p_cb->p_next;
    }

    return p_current;
}

static nrf_cli_coap_internal_t * cli_instance_get_free(void)
{
    otIp6Address unspecified_address;
    memset(&unspecified_address, 0, sizeof(unspecified_address));

    return cli_instance_find_by_peer(&unspecified_address);
}

static void nrf_cli_coap_cmd_request_handler(void                * p_context,
                                             otMessage           * p_message,
                                             const otMessageInfo * p_message_info)
{
    otCoapCode                response_code      = OT_COAP_CODE_VALID;
    otMessage               * p_response_message = NULL;
    uint8_t                   buff[32];
    size_t                    read_len;
    uint16_t                  offset;
    nrf_cli_coap_internal_t * p_instance;

    do
    {
        if (otCoapMessageGetType(p_message) != OT_COAP_TYPE_CONFIRMABLE)
        {
            break;
        }

        if (otCoapMessageGetCode(p_message) != OT_COAP_CODE_PUT)
        {
            break;
        }

        p_instance = cli_instance_find_by_peer(&p_message_info->mPeerAddr);

        if (!p_instance)
        {
            NRF_LOG_INFO("Received command from unknown peer, trying to allocate new CLI");

            p_instance = cli_instance_get_free();

            if (!p_instance)
            {
                NRF_LOG_ERROR("Unable to allocate new CLI");
                break;
            }

            p_instance->p_cb->peer_address = p_message_info->mPeerAddr;

            NRF_LOG_INFO("Allocated new CLI");
        }

        offset = otMessageGetOffset(p_message);

        while (true)
        {
            read_len = otMessageRead(p_message, offset, buff, sizeof(buff));

            if(!read_len)
            {
                break;
            }

            offset += read_len;

            if (nrf_ringbuf_cpy_put(p_instance->p_rx_ringbuf, buff, &read_len) != NRF_SUCCESS)
            {
                break;
            }
        }

        p_response_message = otCoapNewMessage(ot_instance_get(p_instance), NULL);

        if (p_response_message == NULL)
        {
            NRF_LOG_ERROR("No buffs for response message");
            break;
        }

        otCoapMessageInit(p_response_message, OT_COAP_TYPE_ACKNOWLEDGMENT, response_code);
        otCoapMessageSetMessageId(p_response_message, otCoapMessageGetMessageId(p_message));
        otCoapMessageSetToken(p_response_message, otCoapMessageGetToken(p_message), otCoapMessageGetTokenLength(p_message));

        if (otCoapSendResponse(ot_instance_get(p_instance), p_response_message, p_message_info) != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Unable to send response message");

            if (p_response_message != NULL)
            {
                otMessageFree(p_response_message);
            }

            break;
        }

        p_instance->p_cb->handler(NRF_CLI_TRANSPORT_EVT_RX_RDY, p_instance->p_cb->p_context);
    } while (false);
}

static void nrf_cli_coap_resp_request_handler(void                * p_context,
                                              otMessage           * p_message,
                                              const otMessageInfo * p_message_info)
{
    otCoapCode                response_code = OT_COAP_CODE_VALID;
    otMessage               * p_response_message;
    char                      buff[32];
    size_t                    read_len;
    uint16_t                  offset;
    bool                      colors;
    nrf_cli_coap_internal_t * p_instance;

    do
    {
        p_instance = cli_instance_find_by_peer(&p_message_info->mPeerAddr);

        if (!p_instance)
        {
            NRF_LOG_INFO("Received command response from unknown peer, dropping");
            break;
        }

        if (otCoapMessageGetType(p_message) != OT_COAP_TYPE_CONFIRMABLE)
        {
            break;
        }

        if (otCoapMessageGetCode(p_message) != OT_COAP_CODE_PUT)
        {
            break;
        }

        p_response_message = otCoapNewMessage(ot_instance_get(p_instance), NULL);

        if (p_response_message == NULL)
        {
            NRF_LOG_ERROR("No buffs for response message");
            break;
        }

        otCoapMessageInit(p_response_message, OT_COAP_TYPE_ACKNOWLEDGMENT, response_code);
        otCoapMessageSetMessageId(p_response_message, otCoapMessageGetMessageId(p_message));
        otCoapMessageSetToken(p_response_message, otCoapMessageGetToken(p_message), otCoapMessageGetTokenLength(p_message));

        if (otCoapSendResponse(ot_instance_get(p_instance), p_response_message, p_message_info) != OT_ERROR_NONE)
        {
            NRF_LOG_ERROR("Unable to send response message");

            if (p_response_message != NULL)
            {
                otMessageFree(p_response_message);
            }

            break;
        }

        if (mp_response_cli)
        {
            offset = otMessageGetOffset(p_message);
            colors = mp_response_cli->p_ctx->internal.flag.use_colors;

            mp_response_cli->p_ctx->internal.flag.use_colors = false;

            while(true)
            {
                read_len = otMessageRead(p_message, offset, buff, sizeof(buff) - 1);

                if (!read_len)
                {
                    break;
                }

                offset        += read_len;
                buff[read_len] = '\0';
                nrf_cli_fprintf(mp_response_cli, NRF_CLI_DEFAULT, "%s", buff);
            }

            mp_response_cli->p_ctx->internal.flag.use_colors = colors;

            nrf_cli_fprintf(mp_response_cli, NRF_CLI_INFO, "%s", mp_response_cli->p_name);
        }
    } while (false);
}

static void cli_coap_resources_initialize(const nrf_cli_coap_internal_t * p_internal)
{
    if (!m_coap_resources_initialized)
    {
        m_coap_resources.m_cmd_resource.mContext  = ot_instance_get(p_internal);
        m_coap_resources.m_resp_resource.mContext = ot_instance_get(p_internal);

        otError error = otCoapAddResource(ot_instance_get(p_internal), &m_coap_resources.m_cmd_resource);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapAddResource(ot_instance_get(p_internal), &m_coap_resources.m_resp_resource);
        ASSERT(error == OT_ERROR_NONE);

        m_coap_resources_initialized = true;
    }
}

static ret_code_t cli_coap_init(const nrf_cli_transport_t * p_transport,
                                const void                * p_config,
                                nrf_cli_transport_handler_t evt_handler,
                                void                      * p_context)
{
    nrf_cli_coap_internal_t * p_internal =
                                   CONTAINER_OF(p_transport, nrf_cli_coap_internal_t, transport);

    p_internal->p_cb->handler       = evt_handler;
    p_internal->p_cb->p_context     = p_context;
    p_internal->p_cb->p_ot_instance = (otInstance *)p_config;

    cli_coap_resources_initialize(p_internal);

    cli_instance_add(p_internal);

    return NRF_SUCCESS;
}

static ret_code_t cli_coap_uninit(const nrf_cli_transport_t * p_transport)
{
    nrf_cli_coap_internal_t * p_internal =
                                   CONTAINER_OF(p_transport, nrf_cli_coap_internal_t, transport);

    p_internal->p_cb->handler  = NULL;
    p_internal->p_cb->p_context = NULL;

    otCoapRemoveResource(ot_instance_get(p_internal), &m_coap_resources.m_cmd_resource);
    otCoapRemoveResource(ot_instance_get(p_internal), &m_coap_resources.m_resp_resource);

    cli_instance_rm(p_internal);

    return NRF_SUCCESS;
}

static ret_code_t cli_coap_enable(const nrf_cli_transport_t * p_transport,
                                  bool                        blocking)
{
    return NRF_SUCCESS;
}

static ret_code_t cli_coap_read(const nrf_cli_transport_t * p_transport,
                                void *                      p_data,
                                size_t                      length,
                                size_t *                    p_cnt)
{
    ASSERT(p_cnt);
    nrf_cli_coap_internal_t * p_instance =
                                 CONTAINER_OF(p_transport, nrf_cli_coap_internal_t, transport);

    *p_cnt = length;
    ret_code_t err_code = nrf_ringbuf_cpy_get(p_instance->p_rx_ringbuf, p_data, p_cnt);

    return err_code;
}

static ret_code_t cli_coap_write(const nrf_cli_transport_t * p_transport,
                                 const void *                p_data,
                                 size_t                      length,
                                 size_t *                    p_cnt)
{
    nrf_cli_coap_internal_t * p_internal =
                                   CONTAINER_OF(p_transport, nrf_cli_coap_internal_t, transport);

    *p_cnt = length;

    cli_coap_send_to_peer(p_internal, &m_coap_resources.m_resp_resource, p_data, length);

    p_internal->p_cb->handler(NRF_CLI_TRANSPORT_EVT_TX_RDY, p_internal->p_cb->p_context);

    return NRF_SUCCESS;
}

static void cli_coap_send_to_peer(const nrf_cli_coap_internal_t * p_internal,
                                  otCoapResource                * p_resource,
                                  const void                    * p_data,
                                  size_t                          length)
{
    otError       error = OT_ERROR_NONE;
    otMessage   * p_message;
    otMessageInfo message_info;

    do
    {
        if (otIp6IsAddressUnspecified(&p_internal->p_cb->peer_address))
        {
            break;
        }

        p_message = otCoapNewMessage(ot_instance_get(p_internal), NULL);
        if (p_message == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request");
            break;
        }

        otCoapMessageInit(p_message, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapMessageGenerateToken(p_message, 2);

        error = otCoapMessageAppendUriPathOptions(p_message, p_resource->mUriPath);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapMessageSetPayloadMarker(p_message);
        ASSERT(error == OT_ERROR_NONE);

        error = otMessageAppend(p_message, p_data, length);

        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr    = p_internal->p_cb->peer_address;
        message_info.mSockAddr    = *otThreadGetMeshLocalEid(ot_instance_get(p_internal));

        error = otCoapSendRequest(ot_instance_get(p_internal),
                                  p_message,
                                  &message_info,
                                  NULL,
                                  NULL);
    } while (false);

    if ((error != OT_ERROR_NONE) && (p_message != NULL))
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d", error);
        otMessageFree(p_message);
    }
}

void cli_coap_send_cmd_to_peer(const nrf_cli_t  * p_cli,
                               const void       * p_data,
                               size_t             length)
{
    if (p_cli)
    {
        nrf_cli_coap_internal_t * p_internal =
                                    CONTAINER_OF(p_cli->p_iface, nrf_cli_coap_internal_t, transport);

        cli_coap_send_to_peer(p_internal, &m_coap_resources.m_cmd_resource, p_data, length);
    }
}

void cli_coap_peer_set(const nrf_cli_t * p_cli, const otIp6Address * p_peer_address)
{
    if (p_cli)
    {
        nrf_cli_coap_internal_t * p_internal =
                                    CONTAINER_OF(p_cli->p_iface, nrf_cli_coap_internal_t, transport);

        p_internal->p_cb->peer_address = *p_peer_address;
    }
}

void cli_coap_response_cli_set(const nrf_cli_t * p_cli)
{
    mp_response_cli = p_cli;
}

const nrf_cli_transport_api_t nrf_cli_coap_transport_api = {
        .init   = cli_coap_init,
        .uninit = cli_coap_uninit,
        .enable = cli_coap_enable,
        .read   = cli_coap_read,
        .write  = cli_coap_write,
};

#endif // NRF_MODULE_ENABLED(NRF_CLI_COAP)
