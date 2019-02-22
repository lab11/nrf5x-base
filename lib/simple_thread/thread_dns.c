#include "thread_dns.h"

#include <string.h>
#include "nrf_log.h"

#include "config.h"

void __attribute__((weak)) dns_response_handler(void         * p_context,
                                 const char   * p_hostname,
                                 otIp6Address * p_resolved_address,
                                 uint32_t       ttl,
                                 otError        error)
{
    if (error != OT_ERROR_NONE)
    {
        NRF_LOG_INFO("DNS response error %d.", error);
        return;
    }

    NRF_LOG_INFO("Successfully resolved address");
    memcpy(p_context, p_resolved_address, sizeof(otIp6Address));
}

otError thread_dns_hostname_resolve(otInstance * p_instance,
                                    const char         * p_dns_host,
                                    const char         * p_hostname,
                                    otDnsResponseHandler p_dns_response_handler,
                                    void               * p_context)
{
    otError       error;
    otMessageInfo message_info;

    memset(&message_info, 0, sizeof(message_info));
    message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
    message_info.mPeerPort    = OT_DNS_DEFAULT_SERVER_PORT;
    error = otIp6AddressFromString(p_dns_host, &message_info.mPeerAddr);

    if (error == OT_ERROR_NONE)
    {
        otDnsQuery query;

        query.mHostname    = p_hostname;
        query.mMessageInfo = &message_info;
        query.mNoRecursion = false;

        error = otDnsClientQuery(p_instance, &query, p_dns_response_handler, p_context);
    }

    if (error != OT_ERROR_NONE)
    {
        NRF_LOG_INFO("Failed to perform DNS Query.\r\n");
    }

    return error;
}
