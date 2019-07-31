#pragma once

#include <stdint.h>

#include <openthread/coap.h>

void thread_coap_client_init(otInstance* instance);
otError thread_coap_send(otInstance* instance, otCoapCode req, otCoapType type, const otIp6Address* dest, const char* path, const uint8_t* data, size_t len);
