/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include <string.h>
#include "sdk_errors.h"
#include "nrf_error.h"
#include "coap_transport.h"
#include "coap.h"
#include "mem_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "sdk_config.h"

#include <openthread/udp.h>
#include <openthread/instance.h>
#include <openthread/message.h>

static otUdpSocket  m_socket_table[COAP_PORT_COUNT];
static otInstance * m_pinstance;

static void port_data_callback(void                * p_context,
                               otMessage           * p_message,
                               const otMessageInfo * p_message_info)
{
    const coap_port_t  port     = { .port_number = p_message_info->mSockPort };
    coap_remote_t      remote_endpoint;
    coap_remote_t      local_endpoint;

    memcpy(remote_endpoint.addr, p_message_info->mPeerAddr.mFields.m8, OT_IP6_ADDRESS_SIZE);
    remote_endpoint.port_number = p_message_info->mPeerPort;

    memcpy(local_endpoint.addr, p_message_info->mSockAddr.mFields.m8, OT_IP6_ADDRESS_SIZE);
    local_endpoint.port_number = p_message_info->mSockPort;

	uint16_t payload_size = otMessageGetLength(p_message) - otMessageGetOffset(p_message);
	uint8_t * p_msg = nrf_malloc(payload_size);

	if (p_msg)
	{
	    if (otMessageRead(p_message, otMessageGetOffset(p_message), p_msg, payload_size) == payload_size)
	    {
            COAP_MUTEX_LOCK();
            coap_transport_read(&port,
                                &remote_endpoint,
                                &local_endpoint,
                                NRF_SUCCESS,
                                p_msg,
                                payload_size);

            COAP_MUTEX_UNLOCK();
	    }

	    nrf_free(p_msg);
	}
}

static uint32_t port_create(uint32_t index, coap_port_t  * p_port)
{
    otError       err_code;
   	otSockAddr    addr;
    otUdpSocket * p_socket = &m_socket_table[index];

    err_code = otUdpOpen(m_pinstance, p_socket, port_data_callback, p_socket);
    if (err_code == OT_ERROR_NONE)
    {
        // Bind the socket to the local port.
    	memset(&addr, 0, sizeof(addr));
    	addr.mPort = p_port->port_number;

        err_code = otUdpBind(p_socket, &addr);
    }

    return (err_code == OT_ERROR_NONE) ? NRF_SUCCESS : NRF_ERROR_INTERNAL;
}

uint32_t coap_transport_init(const coap_transport_init_t * p_param)
{
    uint32_t    err_code = NRF_ERROR_NO_MEM;
    uint32_t    index;

    NULL_PARAM_CHECK(p_param);
    NULL_PARAM_CHECK(p_param->p_port_table);
    NULL_PARAM_CHECK(p_param->p_arg);

    m_pinstance = (otInstance *)p_param->p_arg;
    memset(m_socket_table, 0, sizeof(m_socket_table));

    for(index = 0; index < COAP_PORT_COUNT; index++)
    {
        // Create end point for each of the CoAP ports.
        err_code = port_create(index, &p_param->p_port_table[index]);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }
    }

    return err_code;
}

uint32_t coap_transport_write(const coap_port_t    * p_port,
                              const coap_remote_t  * p_remote,
                              const uint8_t        * p_data,
                              uint16_t               datalen)
{
    NULL_PARAM_CHECK(p_port);
    NULL_PARAM_CHECK(p_remote);
    NULL_PARAM_CHECK(p_data);

    uint32_t      err_code = NRF_SUCCESS;
    otMessage   * p_msg    = NULL;
    otUdpSocket * p_socket = NULL;

    //Search for the corresponding port.
    for (uint32_t index = 0; index < COAP_PORT_COUNT; index ++)
    {
        if (m_socket_table[index].mSockName.mPort == p_port->port_number)
        {
        	p_socket = &m_socket_table[index];
        	break;
        }
    }

    do {
        p_msg = otUdpNewMessage(m_pinstance, true);
		if (p_msg == NULL)
		{
			NRF_LOG_ERROR("Failed to allocate message\r\n");
			err_code = NRF_ERROR_NO_MEM;
			break;
		}

		if (otMessageSetLength(p_msg, datalen) != OT_ERROR_NONE)
		{
			NRF_LOG_ERROR("Failed to allocate message buffer\r\n");
			err_code = NRF_ERROR_INTERNAL;
			break;
		}

		if (otMessageWrite(p_msg, 0, p_data, datalen) != datalen)
		{
			NRF_LOG_ERROR("Failed to write message payload\r\n");
			err_code = NRF_ERROR_INTERNAL;
			break;
		}

		otMessageInfo msg_info;
		memset(&msg_info, 0, sizeof(msg_info));

		msg_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
		msg_info.mPeerPort = p_remote->port_number;
		memcpy(msg_info.mPeerAddr.mFields.m8, p_remote->addr, OT_IP6_ADDRESS_SIZE);

		COAP_MUTEX_UNLOCK();
		if (otUdpSend(p_socket, p_msg, &msg_info) != OT_ERROR_NONE)
		{
		    NRF_LOG_ERROR("Message not sent!\r\n");
			err_code = NRF_ERROR_INTERNAL;
		}
		COAP_MUTEX_LOCK();

    } while (0);

	if ((p_msg != NULL) && (err_code != NRF_SUCCESS))
	{
		otMessageFree(p_msg);
	}

    return err_code;
}


void coap_transport_process(void)
{
    return;
}

#if (COAP_ENABLE_DTLS_API == 1)
uint32_t coap_security_setup(uint16_t                       local_port,
                             nrf_tls_role_t                 role,
                             coap_remote_t          * const p_remote,
                             nrf_tls_key_settings_t * const p_settings)
{
    return API_NOT_IMPLEMENTED;
}


uint32_t coap_security_destroy(uint16_t              local_port,
                               coap_remote_t * const p_remote)
{
    return API_NOT_IMPLEMENTED;
}
#endif
