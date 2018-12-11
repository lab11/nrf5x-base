#include "thread_dns.h"

#include <string.h>
#include "nrf_log.h"

otError thread_dns_utils_hostname_resolve(otInstance         * p_instance,
                                          const char         * p_dns_host,
                                          const char         * p_hostname,
                                          otDnsResponseHandler p_dns_response_handler,
                                          void               * p_context)
{
    otError       error;
    otMessageInfo message_info;

    memset(&message_info, 0, sizeof(message_info));
    message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
    message_info.mPeerPort    = OT_DNS_DEFAULT_DNS_SERVER_PORT;
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
