#include "nrf_log.h"

#include "thread_coap.h"

static const otIp6Address m_unspecified_ipv6 =
{
    .mFields =
    {
        .m8 = {0}
    }
};

void __attribute__((weak)) thread_coap_handler(
                                void                * p_context,
                                otCoapHeader        * p_header,
                                otMessage           * p_message,
                                const otMessageInfo * p_message_info)
{
    (void)p_context;
    (void)p_header;
    (void)p_message;
    (void)p_message_info;

    NRF_LOG_INFO("Received CoAP message that does not match any request or resource\r\n");
}

void thread_coap_client_init(otInstance* instance) {
    otError error = otCoapStart(instance, OT_DEFAULT_COAP_PORT);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(instance, thread_coap_handler, NULL);
}

void thread_coap_send(otInstance* instance, otCoapCode req, otCoapType type, const otIp6Address* dest, const char* path, const uint8_t* data, size_t len) {
    otError       error = OT_ERROR_NONE;
    otMessage   * message;
    otMessageInfo message_info;
    otCoapHeader  header;

    do
    {
        if (otIp6IsAddressEqual(dest, &m_unspecified_ipv6))
        {
            NRF_LOG_INFO("Failed to send the CoAP Request to the Unspecified IPv6 Address\r\n");
            break;
        }

        otCoapHeaderInit(&header, type, req);
        otCoapHeaderGenerateToken(&header, 2);
        otCoapHeaderAppendUriPathOptions(&header, path);
        otCoapHeaderSetPayloadMarker(&header);

        message = otCoapNewMessage(instance, &header);
        if (message == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        error = otMessageAppend(message, data, len);
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        memcpy(&message_info.mPeerAddr, dest, sizeof(message_info.mPeerAddr));

        error = otCoapSendRequest(instance,
                                  message,
                                  &message_info,
                                  NULL,
                                  instance);
    } while (false);

    if (error != OT_ERROR_NONE && message != NULL)
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(message);
    }
}
