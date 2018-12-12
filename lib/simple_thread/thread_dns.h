#pragma once

#include <openthread/dns.h>

otError thread_dns_hostname_resolve(otInstance         * p_instance,
                                    const char         * p_dns_host,
                                    const char         * p_hostname,
                                    otDnsResponseHandler p_dns_response_handler,
                                    void               * p_context);
