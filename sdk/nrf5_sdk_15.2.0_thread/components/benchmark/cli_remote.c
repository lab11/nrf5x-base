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

#include "benchmark_api.h"
#include "nrf_cli.h"
#include "cli_api.h"
#include "commands.h"
#include "protocol_api.h"


static void cmd_remote_attach(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    const benchmark_peer_db_t * p_peer_db = benchmark_peer_table_get();

    if (p_peer_db->peer_count == 0)
    {
        print_error(p_cli, "The list of known peers is empty. Discover peers and try again");
        return;
    }

    for (size_t i = 0; i < CLI_REMOTE_INSTANCES && i < p_peer_db->peer_count; i++)
    {
        cli_remote_peer_set(cli_remote_get(i),
                            p_peer_db->peer_table[i].p_address);
    }
}

static void cmd_remote(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    nrf_cli_t const * p_peer_cli;

    if (argc < 3)
    {
       nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Required arguments not provided\r\n");
       return;
    }
    else
    {
        p_peer_cli = cli_remote_get(atoi(argv[1]));

        if (p_peer_cli == NULL)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Remote number out of range\r\n");
            return;
        }
    }

    protocol_cmd_remote_send(p_cli, p_peer_cli, argc - 2, &(argv[2]));
}


NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_remote_cmds)
{
    NRF_CLI_CMD(attach, NULL, "Attach remote CLI connections to all available peers", cmd_remote_attach),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(remote, &m_remote_cmds, "Send command to remote peer", cmd_remote);
