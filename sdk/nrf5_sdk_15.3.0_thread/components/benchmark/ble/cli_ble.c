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

#include "ble_advertiser.h"
#include "ble_central.h"
#include "ble_ping.h"
#include "nrf_cli.h"
#include "nrf_error.h"
#include "nrf_log.h"
#include "commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static void cmd_ble_adv_info_get(nrf_cli_t const * p_cli)
{
    bool     is_started    = ble_advertiser_is_started();
    uint32_t curr_interval = ble_advertiser_interval_get_current();
    uint32_t req_interval  = ble_advertiser_interval_get_requested();
    size_t   data_len      = ble_advertiser_adv_data_len_get();

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\r\nBluetooth advertising configuration:\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Started:                        %s\r\n", is_started ? "true" : "false");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Current advertising interval:   %u ms\r\n", curr_interval);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Requested advertising interval: %u ms\r\n", req_interval);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Advertising data length:        %d bytes\r\n", data_len);
}

static void cmd_ble_conn_info_get(nrf_cli_t const * p_cli)
{
    uint32_t slave_latency     = ble_connection_slave_latency_get();
    size_t   connected_devices = ble_conn_count();
    uint8_t  length_extension  = ble_ping_gatt_data_length_get();
    bool     event_extension   = ble_connection_event_length_extension_get();
    char     addr_str[BDADDR_STRING_SIZE];

    ble_connection_bdaddr_get(addr_str, BDADDR_STRING_SIZE);

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\r\nBluetooth connection configuration:\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Slave latency:            %u\r\n", slave_latency);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Conn event extension:     %s\r\n", event_extension ? "enabled" : "disabled");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Bluetooth Device Address: %s\r\n", addr_str);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Connected devices:        %u\r\n", connected_devices);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Data length extension:    %u bytes\r\n", length_extension);

    for (size_t i = 0; i < connected_devices; i++)
    {
        peer_t * p_peer = ble_conn_get(i);

        address_to_string_convert(addr_str, &p_peer->m_addr, BDADDR_STRING_SIZE);

        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\r\nBluetooth connected device %u:\r\n", i);
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Connection interval:      %u\r\n", p_peer->m_conn_cur_interval);
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Bluetooth Device Address: %s\r\n", addr_str);
    }
}

static void cmd_ble_scan_info_get(nrf_cli_t const * p_cli)
{
    uint32_t scan_interval     = ble_scan_interval_get();
    uint32_t scan_window       = ble_scan_window_get();
    bool     scan_enabled      = ble_scan_enabled_get();

    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "\r\nBluetooth scan configuration:\r\n");
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Scan interval:            %u ms\r\n", scan_interval);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Scan window:              %u ms\r\n", scan_window);
    nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "  Scan state:               %s\r\n", scan_enabled ? "enabled" : "disabled");
}

static void cmd_ble_adv_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    ble_advertiser_start();
    print_done(p_cli);
}

static void cmd_ble_adv_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    ble_advertiser_stop();
    print_done(p_cli);
}

static void cmd_ble_adv_interval(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current:   %d\r\n", ble_advertiser_interval_get_current());
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Requested: %d\r\n", ble_advertiser_interval_get_requested());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_advertiser_interval_set(atoi(argv[1]));
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Error: Too many arguments.");
    }
}

static void cmd_ble_adv_data_len(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%d\r\n", ble_advertiser_adv_data_len_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        if (!ble_advertiser_adv_data_len_set(atoi(argv[1])))
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Data len can be in range 0 to %d, excluding 1\r\n", ble_advertiser_adv_data_len_max());
        }
        else
        {
            print_done(p_cli);
        }
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_adv_info(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    cmd_ble_adv_info_get(p_cli);
    print_done(p_cli);
}

static void cmd_ble_info(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    cmd_ble_adv_info_get(p_cli);
    cmd_ble_conn_info_get(p_cli);
    cmd_ble_scan_info_get(p_cli);
    print_done(p_cli);
}

static void cmd_ble_conn_bdaddr(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        char addr_str[BDADDR_STRING_SIZE];

        ble_connection_bdaddr_get(addr_str, BDADDR_STRING_SIZE);
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Bluetooth Device Address: %s\r\n", addr_str);
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_info(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    cmd_ble_conn_info_get(p_cli);
    print_done(p_cli);
}

static void cmd_ble_conn_interval(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current connection interval: %d\r\n", ble_connection_interval_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_connection_interval_set(atoi(argv[1]));
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_scan_interval(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current scan interval: %d\r\n", ble_scan_interval_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_scan_interval_set(atoi(argv[1]));
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_scan_window(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current scan window: %d\r\n", ble_scan_window_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_scan_window_set(atoi(argv[1]));
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_slave_latency(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current slave latency: %d\r\n", ble_connection_slave_latency_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_connection_slave_latency_set(atoi(argv[1]));
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_phy_1mbps(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        print_error(p_cli, "Not enough arguments.");
    }
    else if (argc == 2)
    {
        if (ble_conn_phy_mode_set(argv[1], BLE_GAP_PHY_1MBPS) == true)
        {
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Invalid peer address.");
        }
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_phy_2mbps(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        print_error(p_cli, "Not enough arguments.");
    }
    else if (argc == 2)
    {
        if (ble_conn_phy_mode_set(argv[1], BLE_GAP_PHY_2MBPS) == true)
        {
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Invalid peer address.");
        }
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_phy(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        print_error(p_cli, "Not enough arguments.");
    }
    else if (argc == 2)
    {
        ble_gap_addr_t gap_addr;
        peer_t       * p_peer = NULL;

        string_to_address_convert(&gap_addr, argv[1]);
        p_peer = ble_conn_find_by_addr(&gap_addr);

        if (p_peer == NULL)
        {
            print_error(p_cli, "Peer was not found.");
        }
        else
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Physical layer mode for given connection: %s\r\n", phy_mode_to_string_convert(p_peer->m_phy_mode));
            print_done(p_cli);
        }
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "No BLE peer address specified. Provide a BLE peer address. \r\n");
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_connection_start(argv[1]);
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_conn_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc < 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "No BLE peer address specified. Provide a BLE peer address. \r\n");
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        ble_connection_stop(argv[1]);
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_connection_evt_ext_disable(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 1)
    {
        print_error(p_cli, "Too many arguments.");
    }
    else
    {
        if (ble_connection_event_length_extension_set(false) == true)
        {
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Could not disable event length extension.");
        }
    }
}

static void cmd_ble_connection_evt_ext_enable(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 1)
    {
        print_error(p_cli, "Too many arguments.");
    }
    else
    {
        if (ble_connection_event_length_extension_set(true) == true)
        {
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Could not enable event length extension.");
        }
    }
}

static void cmd_ble_connection_evt_ext(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc == 1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current connection event extension mode: %s\r\n",
                        ble_connection_event_length_extension_get() ? "enabled" : "disabled");
        print_done(p_cli);
    }
    else
    {
        print_error(p_cli, "Unknown parameter.");
    }
}

static void cmd_ble_ping_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (ble_conn_count() == 0)
    {
        print_error(p_cli, "No Bluetooth connection established.");
    }
    else
    {
        /* Send single burst of default number of random bytes. */
        if (argc < 2)
        {
            ble_ping_send(0);
            print_done(p_cli);
        }
        /* Send single burst of provided number of random bytes. */
        else if (argc == 2)
        {
            ble_ping_send(atoi(argv[1]));
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Too many arguments.");
        }
    }
}

static void cmd_ble_flood(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (ble_conn_count() == 0)
    {
        print_error(p_cli, "No Bluetooth connection established.");
    }
    else
    {
        if (ble_connection_is_central() == true)
        {
            print_error(p_cli, "Only a Bluetooth peripheral can flood packets.");
        }
        else
        {
            if (argc == 1)
            {
                ble_flood_start();
                print_done(p_cli);
            }
            else
            {
                print_error(p_cli, "Too many arguments.");
            }
        }
    }
    
}

static void cmd_ble_flood_autostart_enable(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 1)
    {
        print_error(p_cli, "Too many arguments.");
    }
    else
    {
        ble_flood_autostart_set(true);
        print_done(p_cli);
    }
}

static void cmd_ble_flood_autostart_disable(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc > 1)
    {
        print_error(p_cli, "Too many arguments.");
    }
    else
    {
        ble_flood_autostart_set(false);
        print_done(p_cli);
    }
}

static void cmd_ble_flood_length_set(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (ble_conn_count() == 0)
    {
        print_error(p_cli, "No Bluetooth connection established.");
    }
    else
    {
        if (ble_connection_is_central() == true)
        {
            print_error(p_cli, "Only a Bluetooth peripheral can flood packets.");
        }
        else
        {
            uint32_t length_set = 0;

            if (argc == 1)
            {
                length_set = ble_flood_length_set(0);
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Flood packet size set to %d\r\n", length_set);
                print_done(p_cli);
            }
            else if (argc == 2)
            {
                length_set = ble_flood_length_set(atoi(argv[1]));
                nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Flood packet size set to %d\r\n", length_set);
                print_done(p_cli);
            }
            else
            {
                print_error(p_cli, "Too many arguments");
            }
        }
    }
    
}

static void cmd_ble_flood_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (ble_conn_count() == 0)
    {
        print_error(p_cli, "No Bluetooth connection established.");
    }
    else
    {
        if (argc < 2)
        {
            ble_flood_stop();
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Too many arguments.");
        }
    }
}

static void cmd_ble_ping_set_gatt_data_length(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    if (argc == 1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "Current maximum data length: %d\r\n", ble_ping_gatt_data_length_get());
        print_done(p_cli);
    }
    else if (argc == 2)
    {
        if (ble_ping_gatt_data_length_set(atoi(argv[1])) == true)
        {
            print_done(p_cli);
        }
        else
        {
            print_error(p_cli, "Invalid value.");
        }
    }
    else
    {
        print_error(p_cli, "Too many arguments.");
    }
}

static void cmd_ble_scan_info(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    cmd_ble_scan_info_get(p_cli);
    print_done(p_cli);
}

static void cmd_ble_scan_start(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    ble_scan_start();
    print_done(p_cli);
}

static void cmd_ble_scan_stop(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    scan_results_t * p_scan_results;
    char             addr[BDADDR_STRING_SIZE];

    p_scan_results = ble_scan_stop();

    for (size_t i = 0; i < p_scan_results->m_addr_count; ++i)
    {
        address_to_string_convert(addr, &p_scan_results->m_addrs[i], BDADDR_STRING_SIZE);
        nrf_cli_fprintf(p_cli, NRF_CLI_INFO, "%s\r\n", addr);
    }

    print_done(p_cli);
}

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_phy_cmds)
{
    NRF_CLI_CMD(1Mbps, NULL, "Set the speed of the physical layer to 1Mbps for given connection", cmd_ble_conn_phy_1mbps),
    NRF_CLI_CMD(2Mbps, NULL, "Set the speed of the physical layer to 2Mbps for given connection", cmd_ble_conn_phy_2mbps),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_evt_ext_cmds)
{
    NRF_CLI_CMD(disable, NULL, "Disable event length extension", cmd_ble_connection_evt_ext_disable),
    NRF_CLI_CMD(enable,  NULL, "Enable event length extension",  cmd_ble_connection_evt_ext_enable),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_advertising_cmds)
{
    NRF_CLI_CMD(info,     NULL, "Display summary of Bluetooth advertising configuration", cmd_ble_adv_info),
    NRF_CLI_CMD(interval, NULL, "Set Bluetooth advertising interval",                     cmd_ble_adv_interval),
    NRF_CLI_CMD(length,   NULL, "Set Bluetooth advertising data length",                  cmd_ble_adv_data_len),
    NRF_CLI_CMD(start,    NULL, "Start Bluetooth advertising",                            cmd_ble_adv_start),
    NRF_CLI_CMD(stop,     NULL, "Stop Bluetooth advertising",                             cmd_ble_adv_stop),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_connect_cmds)
{
    NRF_CLI_CMD(bdaddr,        NULL,                                     "Get Bluetooth Device Address of a peer",           cmd_ble_conn_bdaddr),
    NRF_CLI_CMD(dle,           NULL,                                     "Set Bluetooth data length extension",              cmd_ble_ping_set_gatt_data_length),
    NRF_CLI_CMD(evt_ext,       &m_test_configure_bluetooth_evt_ext_cmds, "Set Bluetooth connection event extension",         cmd_ble_connection_evt_ext),
    NRF_CLI_CMD(info,          NULL,                                     "Display summary of current Bluetooth connections", cmd_ble_conn_info),
    NRF_CLI_CMD(interval,      NULL,                                     "Set Bluetooth connection interval",                cmd_ble_conn_interval),
    NRF_CLI_CMD(phy,           &m_test_configure_bluetooth_phy_cmds,     "Set Bluetooth PHY mode for given connection",      cmd_ble_conn_phy),
    NRF_CLI_CMD(slave_latency, NULL,                                     "Set Bluetooth slave latency",                      cmd_ble_conn_slave_latency),
    NRF_CLI_CMD(start,         NULL,                                     "Start Bluetooth connection",                       cmd_ble_conn_start),
    NRF_CLI_CMD(stop,          NULL,                                     "Stop Bluetooth connection",                        cmd_ble_conn_stop),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_scan_cmds)
{
    NRF_CLI_CMD(info,     NULL, "Display summary of Bluetooth scan", cmd_ble_scan_info),
    NRF_CLI_CMD(interval, NULL, "Set Bluetooth scan interval",       cmd_ble_scan_interval),
    NRF_CLI_CMD(start,    NULL, "Start Bluetooth scan",              cmd_ble_scan_start),
    NRF_CLI_CMD(stop,     NULL, "Stop Bluetooth scan",               cmd_ble_scan_stop),
    NRF_CLI_CMD(window,   NULL, "Set Bluetooth scan window",         cmd_ble_scan_window),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_autostart_cmds)
{
    NRF_CLI_CMD(disable, NULL, "Disable autostart on benchmark test.", cmd_ble_flood_autostart_disable),
    NRF_CLI_CMD(enable,  NULL, "Enable autostart on benchmark test.",  cmd_ble_flood_autostart_enable),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_flood_cmds)
{
    NRF_CLI_CMD(autostart, &m_test_configure_bluetooth_autostart_cmds, "Autostart flood on benchmark test start.", cmd_default),
    NRF_CLI_CMD(length,    NULL,                                       "Set flood packet size.",                   cmd_ble_flood_length_set),
    NRF_CLI_CMD(stop,      NULL,                                       "Stop flooding packets",                    cmd_ble_flood_stop),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_test_configure_bluetooth_cmds)
{
    NRF_CLI_CMD(adv,   &m_test_configure_bluetooth_advertising_cmds, "Configure Bluetooth advertising",            cmd_default),
    NRF_CLI_CMD(conn,  &m_test_configure_bluetooth_connect_cmds,     "Configure Bluetooth connection",             cmd_default),
    NRF_CLI_CMD(flood, &m_test_configure_bluetooth_flood_cmds,       "Flood messages over Bluetooth",              cmd_ble_flood),
    NRF_CLI_CMD(info,  NULL,                                         "Display summary of Bluetooth configuration", cmd_ble_info),
    NRF_CLI_CMD(ping,  NULL,                                         "Ping over Bluetooth",                        cmd_ble_ping_start),
    NRF_CLI_CMD(scan,  &m_test_configure_bluetooth_scan_cmds,        "Configure Bluetooth scanning",               cmd_default),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(ble, &m_test_configure_bluetooth_cmds, "Configure Bluetooth options", cmd_default);
