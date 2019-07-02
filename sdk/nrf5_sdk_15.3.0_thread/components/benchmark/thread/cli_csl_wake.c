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

#include "commands.h"
#include "cli_api.h"
#include "cpu_utilization.h"
#include "csl_wake.h"
#include "nrf_cli.h"

static nrf_cli_t const * mp_cli_print;

static void display_csl_wake_burst_info(nrf_cli_t const * p_cli, size_t idx)
{
    bool     csma_ca_enabled = csl_wake_burst_csma_ca_get(idx);
    uint32_t period          = csl_wake_burst_period_get(idx);
    uint32_t ifs             = csl_wake_burst_ifs_get(idx);

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== CSL wake TX burst %d configuration ===\r\n", idx);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Period:              %u us\r\n", period);    
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "CSMA-CA:             %s\r\n", csma_ca_enabled ? "Enabled" : "Disabled");

    if (!csma_ca_enabled)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Inter-frame spacing: %u us\r\n", ifs);
    }
}

static void display_csl_wake_info(nrf_cli_t const * p_cli)
{
    size_t cnt    = csl_wake_burst_cnt_get();
    bool   ar_bit = csl_wake_burst_ar_bit_get();

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n\t=== CSL wake TX configuration ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Count:               %d\r\n", cnt);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "ACK request bit:     %s\r\n", ar_bit ? "Set" : "Clear");

    for (size_t i = 0; i < cnt; i++)
    {
        display_csl_wake_burst_info(p_cli, i);
    }
}

static void cmd_csl_wake_info(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    display_csl_wake_info(p_cli);
}

static void cmd_csl_wake_cnt(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", csl_wake_burst_cnt_get());
    }
    else if (argc == 2)
    {
        switch (csl_wake_burst_cnt_set(atoi(argv[1])))
        {
            case CSL_WAKE_BURST_OK:
                break;
            case CSL_WAKE_BURST_ALREADY_RUNNING:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst TX already in progress\r\n");
                break;
            case CSL_WAKE_BURST_BAD_IDX:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst count too high\r\n");
                break;
            default:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Error\r\n");
                break;
        }
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_period(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Enter burst number\r\n");
    }
    else if (argc == 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", csl_wake_burst_period_get(atoi(argv[1])));
    }
    else if (argc == 3)
    {
        switch (csl_wake_burst_period_set(atoi(argv[1]), atoi(argv[2])))
        {
            case CSL_WAKE_BURST_OK:
                break;
            case CSL_WAKE_BURST_ALREADY_RUNNING:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst TX already in progress\r\n");
                break;
            case CSL_WAKE_BURST_BAD_IDX:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst number too high\r\n");
                break;
            default:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Error\r\n");
                break;
        }
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_ifs(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Enter burst number\r\n");
    }
    else if (argc == 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", csl_wake_burst_ifs_get(atoi(argv[1])));
    }
    else if (argc == 3)
    {
        switch (csl_wake_burst_ifs_set(atoi(argv[1]), atoi(argv[2])))
        {
            case CSL_WAKE_BURST_OK:
                break;
            case CSL_WAKE_BURST_ALREADY_RUNNING:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst TX already in progress\r\n");
                break;
            case CSL_WAKE_BURST_BAD_IDX:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst number too high\r\n");
                break;
            default:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Error\r\n");
                break;
        }
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_csmaca(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Enter burst number\r\n");
    }
    else if (argc == 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", csl_wake_burst_csma_ca_get(atoi(argv[1])) ? "Enabled" : "Disabled");
    }
    else if (argc == 3)
    {
        switch (csl_wake_burst_csma_ca_set(atoi(argv[1]), (bool)atoi(argv[2])))
        {
            case CSL_WAKE_BURST_OK:
                break;
            case CSL_WAKE_BURST_ALREADY_RUNNING:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst TX already in progress\r\n");
                break;
            case CSL_WAKE_BURST_BAD_IDX:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst number too high\r\n");
                break;
            default:
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Error\r\n");
                break;
        }
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_ar(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s\r\n", csl_wake_burst_ar_bit_get() ? "Set" : "Clear");
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_ar_set(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        (void)csl_wake_burst_ar_bit_set(true);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }
}

static void cmd_csl_wake_ar_clear(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        (void)csl_wake_burst_ar_bit_set(false);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Too many arguments\r\n");
    }   
}

static void cmd_csl_wake_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    // Remeber cli used to start the test so results can be printed on the same interface.
    mp_cli_print = p_cli;

    switch (csl_wake_burst_send())
    {
        case CSL_WAKE_BURST_OK:
            break;
        case CSL_WAKE_BURST_ALREADY_RUNNING:
            nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst TX already in progress\r\n");
            break;
        case CSL_WAKE_BURST_BAD_IDX:
            nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Wake burst count set to 0\r\n");
            break;
        default:
            nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Cannot start wake burst\r\n");
            break;
    }
}

void csl_wake_burst_ended(csl_wake_burst_res_t result)
{
    uint32_t cpu_local          = cpu_utilization_get();
    uint32_t cpu_local_int      = cpu_local / 100;
    uint32_t cpu_local_residuum = cpu_local % 100;
    uint32_t frames             = csl_wake_txed_frames();
    uint32_t ifs_errors         = csl_wake_ifs_errors();

    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Burst ended,            %s\r\n", result == CSL_WAKE_BURST_OK ? "Ok" : "Error");
    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Transmitted frames:     %u, IFS errors: %u\r\n", frames, ifs_errors);
    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Average CPU utilization %d.%02d%%\r\n", cpu_local_int, cpu_local_residuum);
}

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_csl_wake_ar_cmds)
{
    NRF_CLI_CMD(clear, NULL, "Clear ACK request bit in 15.4 frame", cmd_csl_wake_ar_clear),
    NRF_CLI_CMD(set,   NULL, "Set ACK request bit in 15.4 frame"  , cmd_csl_wake_ar_set),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_csl_wake_cmds)
{
    NRF_CLI_CMD(ar, &m_test_configure_csl_wake_ar_cmds, "Enable/disable setting ACK request bit in 15.4 frame", cmd_csl_wake_ar),

    NRF_CLI_CMD(count,   NULL, "Set count of CSL wake TX bursts",                      cmd_csl_wake_cnt),
    NRF_CLI_CMD(csma-ca, NULL, "Enable/disable CSMA-CA for burst/bursts",              cmd_csl_wake_csmaca),
    NRF_CLI_CMD(ifs,     NULL, "Set inter frame spacing for non CSMA-CA burst/bursts", cmd_csl_wake_ifs),
    NRF_CLI_CMD(info,    NULL, "Display current CSL wake TX configuration",            cmd_csl_wake_info),
    NRF_CLI_CMD(period,  NULL, "Set period of CSL wake TX burst/bursts",               cmd_csl_wake_period),
    NRF_CLI_CMD(start,   NULL, "Start sending CSL wake TX",                            cmd_csl_wake_start),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_csl_cmds)
{
    NRF_CLI_CMD(wake, &m_test_configure_csl_wake_cmds, "Configure CSL wake TX options", cmd_default),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(csl, &m_test_configure_csl_cmds, "Configure CSL options", cmd_default);
