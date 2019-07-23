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

#include "cpu_utilization.h"
#include "cli_api.h"
#include "nrf_cli.h"
#include "commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static void cmd_cpu_utilization_init(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err = cpu_utilization_init();

    if (err != NRF_SUCCESS)
    {
        print_error(p_cli, "Failed to initialize CPU utilization module");
    }
    else
    {
        print_done(p_cli);
    }
}

static void cmd_cpu_utilization_deinit(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err = cpu_utilization_deinit();

    if (err != NRF_SUCCESS)
    {
        print_error(p_cli, "Failed to deinitialize CPU utilization module");
    }
    else
    {
        print_done(p_cli);
    }
}

static void cmd_cpu_utilization_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err = cpu_utilization_start();

    if (err != NRF_SUCCESS)
    {
        print_error(p_cli, "Failed to start CPU utilization module");
    }
    else
    {
        print_done(p_cli);
    }
}

static void cmd_cpu_utilization_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err = cpu_utilization_stop();

    if (err != NRF_SUCCESS)
    {
        print_error(p_cli, "Failed to initialize CPU utilization module");
    }
    else
    {
        print_done(p_cli);
    }
}

static void cmd_cpu_utilization_get(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t cpu_util_value = cpu_utilization_get();

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "CPU Utilization: %d.%02d%%\r\n", (cpu_util_value / 100), (cpu_util_value % 100));
    print_done(p_cli);
}

static void cmd_cpu_utilization_clear(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    cpu_utilization_clear();
    print_done(p_cli);
}

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_cpu_utilization_cmds)
{
    NRF_CLI_CMD(clear,  NULL, "Clear CPU utilization measurement",        cmd_cpu_utilization_clear),
    NRF_CLI_CMD(deinit, NULL, "Deinitialize CPU utilization measurement", cmd_cpu_utilization_deinit),
    NRF_CLI_CMD(get,    NULL, "Get current CPU utilization measurement",  cmd_cpu_utilization_get),
    NRF_CLI_CMD(init,   NULL, "Initialize CPU utilization measurement",   cmd_cpu_utilization_init),
    NRF_CLI_CMD(start,  NULL, "Start CPU utilization measurement",        cmd_cpu_utilization_start),
    NRF_CLI_CMD(stop,   NULL, "Stop CPU utilization measurement",         cmd_cpu_utilization_stop),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(cpu_utilization, &m_cpu_utilization_cmds, "CPU Utilization commands", cmd_default);
