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
#include "app_scheduler.h"
#include "nrf_log.h"
#include "protocol_api.h"
#include "cpu_utilization.h"
#include "cli_suppress.h"

#define DECIMAL_PRECISION 100
#define BPS_TO_KBPS       1024

static void print_test_results(benchmark_event_context_t * p_context);

nrf_cli_t const                  * mp_cli_print;
static benchmark_peer_db_t       * mp_peer_db;
static benchmark_configuration_t   m_test_configuration =
{
    .length       = 64,
    .ack_timeout  = 200,
    .count        = 1000,
    .mode         = BENCHMARK_MODE_ACK,
};

static const char * configuration_mode_name_get(benchmark_mode_t * p_mode)
{
    switch (*(p_mode))
    {
        case BENCHMARK_MODE_UNIDIRECTIONAL:
        return "Unidirectional";

        case BENCHMARK_MODE_ECHO:
        return "Echo";

        case BENCHMARK_MODE_ACK:
        return "Ack";

        default:
        return "Unknown mode";
    }
}

static void discovered_peers_print(void * pp_cli, uint16_t event_size)
{
    nrf_cli_t * p_cli = *((nrf_cli_t **)pp_cli);

    UNUSED_PARAMETER(event_size);

    if (mp_peer_db != NULL)
    {
        protocol_cmd_peer_db_get(p_cli, mp_peer_db);
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "The list of known peers is empty. Discover peers and try again.");
    }
}

static void benchmark_evt_handler(benchmark_evt_t * p_evt)
{
    uint16_t peer_count;
    uint32_t err_code;

    switch (p_evt->evt)
    {
        case BENCHMARK_TEST_COMPLETED:
            NRF_LOG_INFO("Test completed.");
            cli_suppress_disable();
            print_test_results(&(p_evt->context));
            benchmark_ble_flood_stop();
            break;

        case BENCHMARK_TEST_STARTED:
            ASSERT(!protocol_is_error(p_evt->context.error));
            NRF_LOG_INFO("Test started.");
            cli_suppress_enable();
            break;

        case BENCHMARK_TEST_STOPPED:
            cli_suppress_disable();

            if (!protocol_is_error(p_evt->context.error))
            {
                NRF_LOG_INFO("Test successfully stopped.");
            }
            else
            {
                NRF_LOG_INFO("Test stopped with errors. Error code: %u", p_evt->context.error);
                if (mp_cli_print)
                {
                    nrf_cli_fprintf(mp_cli_print, NRF_CLI_ERROR, "Error: Test stopped with errors. Error code: %u\r\n", p_evt->context.error);
                }
            }
            break;

        case BENCHMARK_DISCOVERY_COMPLETED:
            peer_count = p_evt->context.p_peer_information->peer_count;
            mp_peer_db = p_evt->context.p_peer_information;

            if (peer_count)
            {
                mp_peer_db->selected_peer  = peer_count - 1;
            }
            else
            {
                mp_peer_db = NULL;
            }

            NRF_LOG_INFO("Discovery completed, found %d peers.", peer_count);

            err_code = app_sched_event_put(&mp_cli_print, sizeof(mp_cli_print), discovered_peers_print);
            ASSERT(err_code == NRF_SUCCESS);
            break;

        default:
            NRF_LOG_ERROR("Unknown benchmark_evt.");
            break;
    };
}

const benchmark_peer_entry_t * benchmark_peer_selected_get(void)
{
    if (mp_peer_db == NULL)
    {
        return NULL;
    }

    return &(mp_peer_db->peer_table[mp_peer_db->selected_peer]);
}

/** Common commands, API used by all benchmark commands */
void print_done(nrf_cli_t const * p_cli)
{
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Done\r\n");
}

void print_error(nrf_cli_t const * p_cli, char * p_reason)
{
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Error: %s\r\n", p_reason);
}

void cmd_default(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
    }
    else
    {
        nrf_cli_fprintf(p_cli,
                        NRF_CLI_ERROR,
                        "Error: %s:%s%s\r\n",
                        argv[0],
                        " unknown parameter: ",
                        argv[1]);
    }
}

/** Test configuration commands */
void cmd_config_get(const nrf_cli_t * p_cli, size_t argc, char ** argv)
{
    if (argc > 1)
    {
        // If unknown subcommand was passed.
        cmd_default(p_cli, argc, argv);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\n    === Test settings ===\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Mode:               %s\r\n", configuration_mode_name_get(&m_test_configuration.mode));
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "ACK Timeout:        %d [ms]\r\n", m_test_configuration.ack_timeout);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Packet count:       %d\r\n", m_test_configuration.count);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Payload length [B]: %d\r\n", m_test_configuration.length);
}

static void cmd_info_get(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    // Test settings
    cmd_config_get(p_cli, 0, NULL);

    // Local node information
    protocol_cmd_config_get(p_cli);

    // Peer information
    protocol_cmd_peer_get(p_cli, benchmark_peer_selected_get());

    print_done(p_cli);
}

static void cmd_config_mode_get(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc > 1)
    {
        // If unknown subcommand was passed.
        cmd_default(p_cli, argc, argv);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s\r\n", configuration_mode_name_get(&m_test_configuration.mode));
    print_done(p_cli);
}

static void cmd_config_mode_unidirectional_set(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    m_test_configuration.mode = BENCHMARK_MODE_UNIDIRECTIONAL;
    print_done(p_cli);
}

static void cmd_config_mode_echo_set(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    m_test_configuration.mode = BENCHMARK_MODE_ECHO;
    print_done(p_cli);
}

static void cmd_config_mode_ack_set(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    m_test_configuration.mode = BENCHMARK_MODE_ACK;
    print_done(p_cli);
}

static void cmd_config_ack_timeout(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 2)
    {
        print_error(p_cli, "Too many arguments\r\n");
        return;
    }

    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO,"%d\r\n", m_test_configuration.ack_timeout);
    }
    else if (argc == 2)
    {
        m_test_configuration.ack_timeout = atoi(argv[1]);
    }

    print_done(p_cli);
}

static void cmd_config_packet_count(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 2)
    {
        print_error(p_cli, "Too many arguments\r\n");
        return;
    }

    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO,"%d\r\n", m_test_configuration.count);
    }
    else if (argc == 2)
    {
        m_test_configuration.count = atoi(argv[1]);
    }

    print_done(p_cli);
}

static void cmd_config_packet_length(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 2)
    {
        print_error(p_cli, "Too many arguments.");
        return;
    }

    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO,"%d\r\n", m_test_configuration.length);
    }
    else if (argc == 2)
    {
        m_test_configuration.length = atoi(argv[1]);
    }

    print_done(p_cli);
}

/** Peer configuration commands */
static void cmd_discover_peers(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err_code;

    // Remember cli used to start the test so results can be printed on the same interface.
    mp_cli_print = p_cli;

    err_code = benchmark_test_init(&m_test_configuration, benchmark_evt_handler);
    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to configure discovery parameters.");
    }

    err_code = benchmark_peer_discover();
    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to sent discovery message.");
    }
}

static void cmd_display_peers(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    discovered_peers_print(&p_cli, 0);
}

static void cmd_peer_select(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (mp_peer_db == NULL)
    {
        print_error(p_cli, "The list of known peers is empty. Discover peers and try again.");
        return;
    }

    if (argc > 2)
    {
        print_error(p_cli, "Too many arguments.");
        return;
    }

    if (argc == 1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", mp_peer_db->selected_peer);
        print_done(p_cli);
        return;
    }

    if (mp_peer_db->peer_count > atoi(argv[1]))
    {
        mp_peer_db->selected_peer = atoi(argv[1]);
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Peer index out of range.");
    }
}

static void print_int(nrf_cli_t const * p_cli, const char * p_description, const char * p_unit, uint32_t value)
{
    if (value != BENCHMARK_COUNTERS_VALUE_NOT_SUPPORTED)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s: %lu%s\r\n", p_description, value, p_unit);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s: Not supported\r\n", p_description);
    }
}

static void print_decimal(nrf_cli_t const * p_cli, const char * p_description, const char * p_unit, uint32_t value)
{

    if (value != BENCHMARK_COUNTERS_VALUE_NOT_SUPPORTED)
    {
        uint32_t value_int       = value / DECIMAL_PRECISION;
        uint32_t value_remainder = value % DECIMAL_PRECISION;

        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s: %lu.%02lu%s\r\n", p_description, value_int, value_remainder, p_unit);
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s: Not supported\r\n", p_description);
    }
}

static void dump_config(benchmark_configuration_t * p_config)
{
    const char * const modes[] = {"Unidirectional", "Echo", "ACK"};

    print_int(mp_cli_print, "        Length", "", p_config->length);
    print_int(mp_cli_print, "        ACK timeout", "ms", p_config->ack_timeout);
    print_int(mp_cli_print, "        Count", "", p_config->count);
    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        Mode: %s\r\n", modes[p_config->mode]);
}

static void dump_status(benchmark_status_t * p_status)
{
    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        Test in progress: %s\r\n", p_status->test_in_progress ? "True" : "False");
    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        Reset counters: %s\r\n", p_status->reset_counters ? "True" : "False");
    print_int(mp_cli_print, "        ACKs lost", "", p_status->acks_lost);
    print_int(mp_cli_print, "        Waiting for ACKs", "", p_status->waiting_for_ack);
    print_int(mp_cli_print, "        Packets left count", "", p_status->packets_left_count);
    print_int(mp_cli_print, "        Frame number", "", p_status->frame_number);


    if (m_test_configuration.mode == BENCHMARK_MODE_ECHO)
    {
        uint32_t avg = 0;
        if (p_status->latency.cnt > 0)
        {
            avg = (uint32_t)(p_status->latency.sum / p_status->latency.cnt);
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        Latency:\r\n");
        print_decimal(mp_cli_print, "            Min", "ms", p_status->latency.min * DECIMAL_PRECISION / 1000);
        print_decimal(mp_cli_print, "            Max", "ms", p_status->latency.max * DECIMAL_PRECISION / 1000);
        print_decimal(mp_cli_print, "            Avg", "ms", avg * DECIMAL_PRECISION / 1000);
    }
}

static void dump_result(benchmark_result_t * p_result)
{
    print_decimal(mp_cli_print, "        CPU utilization", "%", p_result->cpu_utilization * DECIMAL_PRECISION / 100);
    print_int(mp_cli_print, "        Duration", "ms", p_result->duration);

    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        App counters:\r\n");

    print_int(mp_cli_print, "            Bytes received", "B", p_result->rx_counters.bytes_received);
    print_int(mp_cli_print, "            Packets received", "", p_result->rx_counters.packets_received);
    print_int(mp_cli_print, "            RX error", "", p_result->rx_counters.rx_error);
    print_int(mp_cli_print, "            RX total", "", p_result->rx_counters.rx_total);

    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "        Mac counters:\r\n");
    print_int(mp_cli_print, "            TX error", "", p_result->mac_tx_counters.error);
    print_int(mp_cli_print, "            TX total", "", p_result->mac_tx_counters.total);
}

static void dump_ble_result(benchmark_ble_results_t * p_result)
{
    print_int(mp_cli_print,     "        Bytes transferred", "B", p_result->bytes_transfered);
    print_int(mp_cli_print,     "        Duration", "ms", p_result->duration);
    print_decimal(mp_cli_print, "        Throughput", "kbps", p_result->throughput * DECIMAL_PRECISION / BPS_TO_KBPS);
}

/** Test execution commands */
static void print_test_results(benchmark_event_context_t * p_context)
{
    benchmark_evt_results_t * p_results = &p_context->results;

    if (p_results->p_remote_result == NULL)
    {
        return;
    }

    nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n    === Test Finished ===\r\n");

    if ((p_results->p_local_status != NULL) && (p_results->p_local_result != NULL) && (p_results->p_local_result->duration != 0))
    {
        uint32_t test_duration                            = p_results->p_local_result->duration;
        uint32_t packets_sent                             = m_test_configuration.count - p_results->p_local_status->packets_left_count;
        uint32_t packets_acked                            = packets_sent - p_results->p_local_status->acks_lost;
        uint32_t txed_bytes                               = m_test_configuration.length * packets_sent;
        uint32_t acked_bytes                              = m_test_configuration.length * packets_acked;
        uint32_t throughput                               = (uint32_t)((txed_bytes * 1000ULL) / (test_duration * 128ULL));
        uint32_t throughput_rtx                           = (uint32_t)((acked_bytes * 1000ULL) / (test_duration * 128ULL));

        benchmark_ble_ping_results_t * p_ble_ping_results = benchmark_ble_continuous_results_get();

        print_int(mp_cli_print, "Test duration", "ms", p_results->p_local_result->duration);

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");

        if (m_test_configuration.mode == BENCHMARK_MODE_ECHO)
        {
            uint32_t avg = 0;
            if (p_results->p_local_status->latency.cnt > 0)
            {
                avg = (uint32_t)(p_results->p_local_status->latency.sum / p_results->p_local_status->latency.cnt);
            }

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Latency:\r\n");
            print_decimal(mp_cli_print, "    Min", "ms", p_results->p_local_status->latency.min * DECIMAL_PRECISION / 1000);
            print_decimal(mp_cli_print, "    Max", "ms", p_results->p_local_status->latency.max * DECIMAL_PRECISION / 1000);
            print_decimal(mp_cli_print, "    Avg", "ms", avg * DECIMAL_PRECISION / 1000);

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Average CPU utilization:\r\n");
        print_decimal(mp_cli_print, "    Local", "%", p_results->p_local_result->cpu_utilization * DECIMAL_PRECISION / 100);

        if (p_results->p_remote_result != NULL)
        {
            print_decimal(mp_cli_print, "    Remote", "%", p_results->p_remote_result->cpu_utilization * DECIMAL_PRECISION / 100);
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");

        if (m_test_configuration.mode == BENCHMARK_MODE_UNIDIRECTIONAL)
        {
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Unidirectional:\r\n");
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO,"    Throughput: %lu kbps\r\n", throughput);
        }
        else
        {
            uint32_t per = UINT32_MAX;

            if (packets_sent != 0)
            {
                per = (uint32_t)((DECIMAL_PRECISION * 100ULL * (packets_sent - packets_acked)) / packets_sent);
            }

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Without retransmissions:\r\n");
            print_decimal(mp_cli_print, "    PER", "%", per);
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Throughput: %lu kbps\r\n", throughput);

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "With retransmissions:\r\n");
            print_decimal(mp_cli_print, "    PER", "%", 0);
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Throughput: %lu kbps\r\n", throughput_rtx);
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");

        if (m_test_configuration.mode == BENCHMARK_MODE_UNIDIRECTIONAL)
        {
            uint32_t mac_tx_attempts = p_results->p_local_result->mac_tx_counters.total;
            uint32_t mac_tx_errors   = p_results->p_local_result->mac_tx_counters.error;
            uint32_t mac_per         = UINT32_MAX;
            if (mac_tx_attempts != 0)
            {
                mac_per = (uint32_t)((DECIMAL_PRECISION * 100ULL * mac_tx_errors) / mac_tx_attempts);
            }
            print_decimal(mp_cli_print, "MAC PER", "%", mac_per);
        }
        else
        {
            if (p_results->p_remote_result != NULL)
            {
                uint32_t mac_tx_attempts = p_results->p_local_result->mac_tx_counters.total + p_results->p_remote_result->mac_tx_counters.total;
                uint32_t mac_tx_errors   = p_results->p_local_result->mac_tx_counters.error + p_results->p_remote_result->mac_tx_counters.error;
                uint32_t mac_per         = UINT32_MAX;
                if (mac_tx_attempts != 0)
                {
                    mac_per = (uint32_t)((DECIMAL_PRECISION * 100ULL * mac_tx_errors) / mac_tx_attempts);
                }
                print_decimal(mp_cli_print, "MAC PER", "%", mac_per);
            }
            else
            {
                nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "MAC Counters:\r\n");
                print_int(mp_cli_print, "    MAC TX Total", "", p_results->p_local_result->mac_tx_counters.total);
                print_int(mp_cli_print, "    MAC TX Err", "", p_results->p_local_result->mac_tx_counters.error);
            }
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "Raw data:\r\n");

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Config:\r\n");
        dump_config(&m_test_configuration);

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Status:\r\n");
        dump_status(p_results->p_local_status);

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Local:\r\n");
        dump_result(p_results->p_local_result);

        if (p_results->p_remote_result != NULL)
        {
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    Remote:\r\n");
            dump_result(p_results->p_remote_result);
        }

        if (p_ble_ping_results != NULL)
        {
            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    BLE local:\r\n");
            dump_ble_result(&p_ble_ping_results->local_results);

            nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "    BLE remote:\r\n");
            dump_ble_result(&p_ble_ping_results->remote_results);
        }

        nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "\r\n");
    }

    print_done(mp_cli_print);
}

static void cmd_test_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err_code;

    if (mp_peer_db == NULL)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "No peer selected; run:\r\n     test peer discover \r\nto find peers\r\n");
        return;
    }

    // Remember cli used to start the test so results can be printed on the same interface.
    mp_cli_print = p_cli;

    benchmark_ble_flood_start();

    err_code = benchmark_test_init(&m_test_configuration, benchmark_evt_handler);
    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to configure test parameters.");
        return;
    }

    err_code = benchmark_test_start();
    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to start test.");
    }
}

static void cmd_test_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err_code = benchmark_test_stop();
    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to stop test.");
    }
    else
    {
        print_done(p_cli);
    }
}

static void cmd_peer_test_results(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    uint32_t err_code = benchmark_peer_results_request_send();

    if (protocol_is_error(err_code))
    {
        print_error(p_cli, "Failed to send test results request.");
        return;
    }
}


NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_mode_cmds)
{
    NRF_CLI_CMD(ack,            NULL, "Peer replies with a short acknowledgment", cmd_config_mode_ack_set),
    NRF_CLI_CMD(echo,           NULL, "Peer replies with the same data",          cmd_config_mode_echo_set),
    NRF_CLI_CMD(unidirectional, NULL, "Transmission in the single direction",     cmd_config_mode_unidirectional_set),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_cmds)
{
    NRF_CLI_CMD(ack-timeout,   NULL,                        "Set time after we stop waiting for the acknowledgment from the peer in milliseconds", cmd_config_ack_timeout),
    NRF_CLI_CMD(count,         NULL,                        "Set number of packets to be sent",                                                    cmd_config_packet_count),
    NRF_CLI_CMD(length,        NULL,                        "Set UDP payload length in bytes",                                                     cmd_config_packet_length),
    NRF_CLI_CMD(mode,          &m_test_configure_mode_cmds, "Set test type",                                                                       cmd_config_mode_get),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_peer_cmds)
{
    NRF_CLI_CMD(discover, NULL, "Discover available peers", cmd_discover_peers),
    NRF_CLI_CMD(list,     NULL, "Display found peers",      cmd_display_peers),
    NRF_CLI_CMD(results,  NULL, "Display results",          cmd_peer_test_results),
    NRF_CLI_CMD(select,   NULL, "Select peer from a list",  cmd_peer_select),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_cmds)
{
    NRF_CLI_CMD(configure, &m_test_configure_cmds,      "Configure the test",                cmd_default),
    NRF_CLI_CMD(info,      NULL,                        "Print current configuration",       cmd_info_get),
    NRF_CLI_CMD(peer,      &m_test_configure_peer_cmds, "Configure the peer receiving data", cmd_default),
    NRF_CLI_CMD(start,     NULL,                        "Start the test",                    cmd_test_start),
    NRF_CLI_CMD(stop,      NULL,                        "Stop the test",                     cmd_test_stop),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(test, &m_test_cmds, "Benchmark commands", cmd_default);
