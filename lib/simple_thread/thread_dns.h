#pragma once

#include <openthread/dns.h>
#include <openthread/dns.h>

void __attribute__((weak)) dns_response_handler(void         * p_context,
                                                const char   * p_hostname,
                                                otIp6Address * p_resolved_address,
                                                uint32_t       ttl,
                                                otError        error);

otError thread_dns_hostname_resolve(otInstance         * p_instance,
                                    const char         * p_dns_host,
                                    const char         * p_hostname,
                                    otDnsResponseHandler p_dns_response_handler,
                                    void               * p_context);
