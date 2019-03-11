#include "simple_thread.h"

#include "openthread/sntp.h"
#include "openthread/ip6.h"

otError thread_ntp_request(otInstance * p_instance,
                   otIp6Address* ntp_addr,
                   uint32_t* port,
                   otSntpResponseHandler handler);
