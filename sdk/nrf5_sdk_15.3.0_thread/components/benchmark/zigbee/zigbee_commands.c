/**
 * Copyright (c) 2018 - 2019, Nordic Semiconductor ASA
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

#include "protocol_api.h"
#include "benchmark_zigbee_common.h"
#include "commands.h"

void protocol_cmd_peer_db_get(const nrf_cli_t * p_cli, const benchmark_peer_db_t * p_peers)
{
    nrf_cli_fprintf(p_cli,
                    NRF_CLI_INFO,
                    "\r\n# ||    Device ID   || 16-bit network address\r\n");

    for (uint16_t i = 0; i < p_peers->peer_count; i++)
    {
        nrf_cli_fprintf(p_cli,
                        NRF_CLI_INFO,
                        "%d:  %08x%08x  %04x\r\n",
                        i,
                        DEVICE_ID_HI(p_peers->peer_table[i].device_id),
                        DEVICE_ID_LO(p_peers->peer_table[i].device_id),
                        p_peers->peer_table[i].p_address->nwk_addr);
    }
}

void protocol_cmd_config_get(const nrf_cli_t * p_cli)
{
    zb_nwk_device_type_t role;
    zb_ieee_addr_t       addr;
    zb_uint16_t          short_addr;
    uint64_t             device_id = benchmark_local_device_id_get();

    zb_get_long_address(addr);
    short_addr = zb_address_short_by_ieee(addr);

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== Local node information ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO,
                    "Device ID:   %08x%08x\r\n",
                    DEVICE_ID_HI(device_id),
                    DEVICE_ID_LO(device_id));

    if (short_addr != ZB_UNKNOWN_SHORT_ADDR)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Network address: %04x\r\n", short_addr);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Network address: none\r\n");
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Zigbee Role: ");
    role = zb_get_network_role();
    if (role == ZB_NWK_DEVICE_TYPE_NONE)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "unknown");
    }
    else if (role == ZB_NWK_DEVICE_TYPE_COORDINATOR)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "zc");
    }
    else if (role == ZB_NWK_DEVICE_TYPE_ROUTER)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "zr");
    }
    else if (role == ZB_NWK_DEVICE_TYPE_ED)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "zed");
    }
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\r\n");
}

void protocol_cmd_peer_get(const nrf_cli_t * p_cli, const benchmark_peer_entry_t * p_peer)
{
    uint32_t device_id_lo = 0;
    uint32_t device_id_hi = 0;
    uint16_t short_addr   = 0xFFFF;

    if (p_peer)
    {
        device_id_lo = DEVICE_ID_LO(p_peer->device_id);
        device_id_hi = DEVICE_ID_HI(p_peer->device_id);
        short_addr   = p_peer->p_address->nwk_addr;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== Peer information ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Device ID:   %08x%08x\r\n", device_id_hi, device_id_lo);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Network address: %04x\r\n", short_addr);
}

void protocol_cmd_remote_send(const nrf_cli_t * p_cli, const nrf_cli_t * p_peer_cli, size_t argc, char ** argv)
{
    /* Remote command execution is not implemented in Zigbee benchmark. */
    return;
}
