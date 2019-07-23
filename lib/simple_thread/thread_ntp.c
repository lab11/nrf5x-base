#include <string.h>

#include "thread_ntp.h"

otError thread_ntp_request(otInstance * p_instance,
                   otIp6Address* ntp_addr,
                   uint32_t* port,
                   otSntpResponseHandler handler) {

  otMessageInfo message_info;
  otSntpQuery query = {.mMessageInfo = &message_info};

  memset(&message_info, 0, sizeof(message_info));
  memcpy(&message_info.mPeerAddr, ntp_addr, sizeof(message_info.mPeerAddr));
  message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
  if (port == NULL) {
    message_info.mPeerPort    = OT_SNTP_DEFAULT_SERVER_PORT;
  } else {
    message_info.mPeerPort    = *port;
  }

  return otSntpClientQuery(p_instance, &query, handler, NULL);
}
