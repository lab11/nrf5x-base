/**
 * Copyright (c) 2018, Nordic Semiconductor ASA
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

#include "commands.h"
#include "benchmark_api.h"

#include "nrf_cli.h"
#include "nrf_cli_coap.h"
#include "nrf_error.h"
#include "nrf_log.h"
#include "thread_utils.h"

#include <openthread/ip6.h>
#include <openthread/thread.h>

#define IPV6_STRING_SIZE       (41 * sizeof(char))
#define REMOTE_CMD_BUFFER_SIZE 128

static const otIp6Address m_unspecified_ipv6 =
{
    .mFields =
    {
        .m8 = {0}
    }
};

static void address_to_string(const otIp6Address *addr, char * ipstr)
{
    UNUSED_RETURN_VALUE(snprintf(ipstr, IPV6_STRING_SIZE, "%x:%x:%x:%x:%x:%x:%x:%x",
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 0)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 1)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 2)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 3)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 4)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 5)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 6)),
                        uint16_big_decode((uint8_t *)(addr->mFields.m16 + 7))));
}

static const char * thread_role_get(void)
{
    switch (otThreadGetDeviceRole(thread_ot_instance_get()))
    {
        case OT_DEVICE_ROLE_DISABLED:
        return "Disabled";

        case OT_DEVICE_ROLE_DETACHED:
        return "Detached";

        case OT_DEVICE_ROLE_CHILD:
        return "Child";

        case OT_DEVICE_ROLE_ROUTER:
        return "Router";

        case OT_DEVICE_ROLE_LEADER:
        return "Leader";

        default:
        return "Unknown role";
    }
}

void protocol_cmd_peer_db_get(const nrf_cli_t * p_cli, const benchmark_peer_db_t * p_peers)
{
    char ipv6[IPV6_STRING_SIZE];

    nrf_cli_fprintf(p_cli,
                    NRF_CLI_INFO,
                    "\r\n# ||    Device ID   || IPv6 Mesh-Local Endpoint ID\r\n");

    for (uint16_t i = 0; i < p_peers->peer_count; i++)
    {
        memset(ipv6, '\0', IPV6_STRING_SIZE);
        address_to_string((otIp6Address *)(p_peers->peer_table[i].p_address), ipv6);

        nrf_cli_fprintf(p_cli,
                        NRF_CLI_INFO,
                        "%d:  %08x%08x  %s\r\n",
                        i,
                        DEVICE_ID_HI(p_peers->peer_table[i].device_id),
                        DEVICE_ID_LO(p_peers->peer_table[i].device_id),
                        ipv6);
    }
}

void protocol_cmd_config_get(const nrf_cli_t * p_cli)
{
    char     ipv6[IPV6_STRING_SIZE];
    uint64_t device_id = benchmark_local_device_id_get();

    memset(ipv6, '\0', IPV6_STRING_SIZE);
    address_to_string(otThreadGetMeshLocalEid(thread_ot_instance_get()), ipv6);

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== Local node information ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO,
                    "Device ID:   %08x%08x\r\n",
                    DEVICE_ID_HI(device_id),
                    DEVICE_ID_LO(device_id));
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "IPv6 ML-EID: %s\r\n", ipv6);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Thread Role: %s\r\n", thread_role_get());
}

void protocol_cmd_peer_get(const nrf_cli_t * p_cli, const benchmark_peer_entry_t * p_peer)
{
    char     ipv6[IPV6_STRING_SIZE];
    uint32_t device_id_lo = 0;
    uint32_t device_id_hi = 0;

    if (p_peer)
    {
        device_id_lo = DEVICE_ID_LO(p_peer->device_id);
        device_id_hi = DEVICE_ID_HI(p_peer->device_id);

        address_to_string((otIp6Address *)(p_peer->p_address), ipv6);
    }
    else
    {
        address_to_string(&m_unspecified_ipv6, ipv6);
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== Peer information ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Device ID:   %08x%08x\r\n", device_id_hi, device_id_lo);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "IPv6 ML-EID: %s\r\n", ipv6);
}

void protocol_cmd_remote_send(const nrf_cli_t * p_cli, const nrf_cli_t * p_peer_cli, size_t argc, char ** argv)
{
    char   rx_buffer[REMOTE_CMD_BUFFER_SIZE];
    char * buf_ptr   = rx_buffer;
    size_t buf_len   = REMOTE_CMD_BUFFER_SIZE;
    size_t arg_len;

    cli_coap_response_cli_set(p_cli);

    for (size_t i = 0; i < argc; i++)
    {
        arg_len = snprintf(buf_ptr, buf_len, "%s ", argv[i]);

        if (arg_len >= buf_len)
        {
            NRF_LOG_ERROR("Remote command buffer full");
            return;
        }

        buf_len -= arg_len;
        buf_ptr += arg_len;
    }

    // As we have spare space char at the end, replace it with \n char
    buf_ptr[-1] = '\n';

    cli_coap_send_cmd_to_peer(p_peer_cli, rx_buffer, REMOTE_CMD_BUFFER_SIZE - buf_len);
}
