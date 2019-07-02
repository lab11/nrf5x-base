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
#include "nrf_cli.h"
#include "zboss_api.h"
#include "zb_error_handler.h"
#include "zigbee_cli.h"
#include "zigbee_cli_utils.h"

/**
 * @defgroup zb_cli_cmd_bdb BDB commands
 * @ingroup zb_cli
 *
 * @{
 */

static zb_nwk_device_type_t m_default_role      = ZB_NWK_DEVICE_TYPE_ROUTER;
static zb_bool_t            m_stack_is_started  = ZB_FALSE;
static zb_bool_t            m_legacy_mode       = ZB_FALSE;

/**
 * @brief Set or get Zigbee role of the device.
 *
 * @code
 * bdb role [<role>]
 * @endcode
 *
 * @pre Setting only before @ref start "bdb start". Reading only after @ref start "bdb start".
 *
 * If the optional argument is not provided, get the state of the device.<br>
 *    Returns:
 *    - `zc` if it is a coordinator,
 *    - `zr` it it is a router,
 *    - `zed` if it is an end device.
 *
 * If the optional argument is provided, set the device role to `role`. Can be
 * either `zc` or `zr`. <br>
 *
 * @note Zigbee End Device is not currently supported on the CLI Agent.
 *
 */
static void cmd_zb_role(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_nwk_device_type_t role;

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc == 1)
    {
        role = zb_get_network_role();
        if (role == ZB_NWK_DEVICE_TYPE_NONE)
        {
            role = m_default_role;
        }

        if (role == ZB_NWK_DEVICE_TYPE_COORDINATOR)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "zc");
        }
        else if (role == ZB_NWK_DEVICE_TYPE_ROUTER)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "zr");
        }
        else if (role == ZB_NWK_DEVICE_TYPE_ED)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "zed");
        }
    }
    else if (argc == 2)
    {
        if (m_stack_is_started)
        {
            print_error(p_cli, "Stack already started");
            return;
        }

        if (!strcmp(argv[1], "zc"))
        {
            m_default_role = ZB_NWK_DEVICE_TYPE_COORDINATOR;
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Coordinator set");
        }
        else if (!strcmp(argv[1], "zr"))
        {
            m_default_role = ZB_NWK_DEVICE_TYPE_ROUTER;
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Router role set");
        }
        else
        {
            print_error(p_cli, "Role unsupported");
            return;
        }
    }
    else
    {
        print_error(p_cli, "Wrong number of arguments");
        return;
    }

    print_done(p_cli, ZB_TRUE);
}

/**
 * @brief Start bdb top level commissioning process.
 *
 * This command can be called multiple times. If device is not on a network
 * then it will attempt to join or form (depending on the role).
 *
 * If the device is on the network then the command will open the network for
 * new devices to join.
 *
 * See Base Device Behaviour specification for details.
 *
 * @code
 * > bdb start
 * Started coordinator
 * Done
 * @endcode
 */
static void cmd_zb_start(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    uint32_t   channel;
    zb_bool_t  ret;
    zb_uint8_t mode_mask = ZB_BDB_NETWORK_STEERING;

    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0], "start - start top level commissioning");
        return;
    }

    if (m_stack_is_started == ZB_FALSE)
    {
        channel = zb_get_bdb_primary_channel_set();

        switch (m_default_role)
        {
            case ZB_NWK_DEVICE_TYPE_ROUTER:
                zb_set_network_router_role(channel);
                nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Started router\r\n");
                break;

            case ZB_NWK_DEVICE_TYPE_COORDINATOR:
                zb_set_network_coordinator_role(channel);
                mode_mask |= ZB_BDB_NETWORK_FORMATION;
                nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Started coordinator\r\n");
                break;

            default:
                print_error(p_cli, "Role unsupported\r\n");
                return;
        }


        UNUSED_RETURN_VALUE(zboss_start_no_autostart());
        /* Iterate one time to propagate the signal to the zboss_signal_handler */
        zboss_main_loop_iteration();
    }

    ret = bdb_start_top_level_commissioning(mode_mask);

    if (ret)
    {
        m_stack_is_started = ZB_TRUE;
        print_done(p_cli, ZB_FALSE);
    }
    else
    {
        print_error(p_cli, "Could not start top level commissioning\r\n");
    }
}

/**@brief Set or get the Zigbee Extended Pan ID value.
 *
 * @code
 * bdb extpanid [<h:id>]
 * @endcode
 *
 * @pre Setting only before @ref start "bdb start". Reading only after @ref start "bdb start".
 *
 * If the optional argument is not provided, gets the extended PAN ID of the joined network.
 *
 * If the optional argument is provided, gets the extended PAN ID to `id`.
 *
 *
 */
static void cmd_zb_extpanid(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    zb_ext_pan_id_t extpanid;

    if (argc == 1)
    {
        zb_get_extended_pan_id(extpanid);
        print_hexdump(p_cli, extpanid, sizeof(extpanid), true);
        print_done(p_cli, ZB_TRUE);
    }
    else if (argc == 2)
    {
        if (parse_long_address(argv[1], extpanid))
        {
            zb_set_extended_pan_id(extpanid);
            print_done(p_cli, ZB_FALSE);
        }
        else
        {
            print_error(p_cli, "Failed to parse extpanid");
        }
    }
    else
    {
        print_error(p_cli, "Unsupported format. Expected extpanid <extpanid>");
    }
}

/**@brief Set or get the Zigbee Pan ID value.
 *
 * @code
 * bdb panid [<h:id>]
 * @endcode
 *
 * @pre Setting only before @ref start "bdb start". Reading only after @ref start "bdb start".
 *
 * If the optional argument is not provided, gets the PAN ID of the joined network.
 * If the optional argument is provided, sets the PAN ID to `id`.
 *
 */
static void cmd_zb_panid(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (argc == 1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%0x", ZB_PIBCACHE_PAN_ID());
        print_done(p_cli, ZB_TRUE);
    }
    else if (argc == 2)
    {
        zb_uint16_t pan_id;

        if (sscanf(argv[1], "%hx", &pan_id))
        {
            ZB_PIBCACHE_PAN_ID() = pan_id;
            print_done(p_cli, ZB_FALSE);
        }
        else
        {
            print_error(p_cli, "Failed to parse PAN ID");
        }
    }
    else
    {
        print_error(p_cli, "Unsupported format. Expected panid <h:id>");
    }
}

/** @brief Set or get 802.15.4 channel.
 *
 * @code
 * bdb channel <n>
 * @endcode
 *
 *  @pre Setting only before @ref start "bdb start".
 *
 * If the optional argument is not provided, get the current number and bitmask of the channel.
 *
 * If the optional argument is provided:
 *   - If `n` is in [11:26] range, set to that channel.
 *   - Otherwise, treat `n` as bitmask (logical or of a single bit shifted by channel number).
 *
 *
 * Example:
 * @code
 * > bdb channel  0x110000
 * Setting channel bitmask to 110000
 * Done
 * @endcode
 */
static void cmd_zb_channel(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    uint32_t chan[2];
    uint32_t channel_number;
    uint32_t channel_mask;

    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0],
                    "channel - get the channel number\r\n"
                    "channel <d:channel> - set the channel to <n>\r\n"
                    "If n is [11:26], set to that channel. Otherwise, treat n as bitmask.");
        return;
    }

    if (argc == 1)
    {
        int i;
        int c;

        chan[0] = zb_get_bdb_primary_channel_set();
        chan[1] = zb_get_bdb_secondary_channel_set();

        // Print for both channels
        for (c = 0; c < 2; c++)
        {
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%s channel(s):", c == 0 ? "Primary" : "Secondary");
            for (i = 11; i <= 26; i++)
            {
                if ((1 << i) & chan[c])
                {
                    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, " %d", i);
                }
            }
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "\r\n");
        }
        
        print_done(p_cli, ZB_FALSE);
        return;
    }
    else if (argc == 2)
    {
        if (m_stack_is_started)
        {
            print_error(p_cli, "Stack already started");
            return;
        }

        sscanf(argv[1], "%ld", &channel_number);
        if (channel_number < 11 || channel_number > 26)
        {
            // Treat as a bitmask
            channel_number = 0;
            sscanf(argv[1], "%lx", &channel_mask);

            if ((!(channel_mask & 0x7FFF800)) || (channel_mask & (~0x7FFF800)))
            {
                print_error(p_cli, "Bitmask invalid");
                return;
            }
        }
        else
        {
            // Treat as number
            channel_mask = 1 << channel_number;
        }

        if (zb_get_bdb_primary_channel_set() != channel_mask)
        {
            if (channel_number)
            {
                nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Setting channel to %d\r\n", channel_number);
            }
            else
            {
                nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Setting channel bitmask to %x\r\n", channel_mask);
            }

            zb_set_bdb_primary_channel_set(channel_mask);
            zb_set_bdb_secondary_channel_set(channel_mask);
            zb_set_channel(channel_mask);
        }

        print_done(p_cli, ZB_FALSE);
    }
    else
    {
        print_error(p_cli, "Wrong number of arguments");
    }
}


/** @brief Set install code on the device, add information about the install code
 *  on the trust center, set the trust center install code policy.
 *
 * @code
 * bdb ic add <h:install code> <h:eui64>
 * bdb ic set <h:install code>
 * bdb ic policy <enable|disable>
 * @endcode
 *
 * @pre Setting and defining policy only before @ref start "bdb start".
 * Adding only after @ref start "bdb start".
 * 
 * <tt>bdb ic set</tt> must only be used on a joining device.
 *
 * <tt>bdb ic add</tt> must only be used on a coordinator.
 *
 * <tt>bdb ic policy</tt> must only be used on a coordinator.
 *
 * Provide the install code as an ASCII-encoded hex including CRC16.
 *
 * For production devices, an install code must be installed by the production
 * configuration present in flash.
 *
 *
 * Example:
 * @code
 * > bdb ic add 83FED3407A939723A5C639B26916D505C3B5 0B010E2F79E9DBFA
 * Done
 * @endcode
 */
static void cmd_zb_install_code(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    const char *   p_err_msg = NULL;
    zb_ieee_addr_t addr;
    zb_uint8_t     ic[ZB_CCM_KEY_SIZE + 2]; // +2 for CRC16

    if (nrf_cli_help_requested(p_cli) || (argc == 1))
    {
        print_usage(p_cli, argv[0],
                    "ic - set or add install code. Enable IC policy.\r\n"
                    "ic set <h:install code> - set the ic code to <install_code>\r\n"
                    "ic add <h:install code> <h:eui64> - add ic for device with given eui43\r\n"
                    "ic policy - set Trust Center install code policy");
        return;
    }

    if ((argc == 2) && (strcmp(argv[0], "set") == 0))
    {
        if (!parse_hex_str(argv[1], ic, 2*sizeof(ic), false))
        {
            p_err_msg = "Failed to parse IC";
            goto exit;
        }

        if (zb_secur_ic_set(ic) != RET_OK)
        {
            p_err_msg = "Failed to set IC";
            goto exit;
        }
    }
    else if ((argc == 3) && (strcmp(argv[0], "add") == 0))
    {
        if (!parse_hex_str(argv[1], ic, 2*sizeof(ic), false))
        {
            p_err_msg = "Failed to parse IC";
            goto exit;
        }

        if (!parse_long_address(argv[2], addr))
        {
            p_err_msg = "Failed to parse eui64";
            goto exit;
        }

        if (zb_secur_ic_add(addr, ic) != RET_OK)
        {
            p_err_msg = "Failed to add IC";
            goto exit;
        }
    }
    else if ((argc == 2) && (strcmp(argv[0], "policy") == 0))
    {
        if (strcmp(argv[1], "enable") == 0)
        {
            zb_set_installcode_policy(ZB_TRUE);
        }
        else if (strcmp(argv[1], "disable") == 0)
        {
            zb_set_installcode_policy(ZB_FALSE);
        }
        else
        {
            p_err_msg = "Syntax error";
            goto exit;
        }
    }
    else
    {
        p_err_msg ="Syntax error";
    }

exit:
    if (p_err_msg)
    {
        print_error(p_cli, p_err_msg);
    }
    else
    {
        print_done(p_cli, ZB_FALSE);
    }
}

/** @brief Enable or disable the legacy device support.
 *
 * @code
 * bdb legacy <enable|disable>
 * @endcode
 *
 * Allow or disallow legacy pre-r21 devices on the Zigbee network.
 *
 * @pre Use only after @ref start "bdb start".
 *
 * Example:
 * @code
 * > bdb legacy enable
 * Done
 * @endcode
 */
static void cmd_zb_legacy(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0],
                    "enable - allow for legacy (pre ZB 3.0 devices)\r\n"
                    "disable - disallow for legacy devices");
        return;
    }

    if (!m_stack_is_started)
    {
        print_error(p_cli, "Stack not started");
        return;
    }

    if (argc == 1)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%s\r\n", m_legacy_mode ? "on" : "off");
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "enable") == 0)
        {
            zb_bdb_set_legacy_device_support(1);
            m_legacy_mode = ZB_TRUE;
        }
        else if (strcmp(argv[1], "disable") == 0)
        {
            zb_bdb_set_legacy_device_support(0);
            m_legacy_mode = ZB_FALSE;
        }
        else
        {
            print_error(p_cli, "Unrecognized option");
            return;
        }
    }
    else
    {
        print_error(p_cli, "Wrong number of arguments");
        return;
    }

    print_done(p_cli, ZB_FALSE);
}

/** @brief Set network key.
 *
 * @code
 * bdb nwkkey <h:key>>
 * @endcode
 *
 * Set a pre-defined network key instead of a random one.
 *
 * @pre Setting only before @ref start "bdb start".
 *
 * Example:
 * @code
 * > bdb nwkkey 00112233445566778899aabbccddeeff
 * Done
 * @endcode
 */
static void cmd_zb_nwkkey(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0],
                    "<h:key>");
        return;
    }

    if (m_stack_is_started)
    {
        print_error(p_cli, "Stack already started");
        return;
    }

    if (argc == 2)
    {
        zb_uint8_t key[ZB_CCM_KEY_SIZE];
        if (parse_hex_str(argv[1], key, 2*sizeof(key), false))
        {
            zb_secur_setup_nwk_key(key, 0);
        }
        else
        {
            print_error(p_cli, "Failed to parse key");
            return;
        }
    }
    else
    {
        print_error(p_cli, "Syntax error");
        return;
    }

    print_done(p_cli, ZB_FALSE);
}

/**
 * @brief Perform a factory reset via local action
 *
 * See Base Device Behavior specification chapter 9.5 for details.
 *
 * @code
 * > bdb factory_reset
 * Done
 * @endcode
 */
static void cmd_zb_factory_reset(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0], "factory_reset - Perform factory reset");
        return;
    }

    zb_bdb_reset_via_local_action(0U);
    print_done(p_cli, ZB_FALSE);
}


zb_bool_t zb_cli_is_stack_started(void)
{
    return m_stack_is_started;
}

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_ic)
{
    NRF_CLI_CMD(add, NULL, "add install code", cmd_zb_install_code),
    NRF_CLI_CMD(set, NULL, "set install code", cmd_zb_install_code),
    NRF_CLI_CMD(policy, NULL, "set TC install code policy", cmd_zb_install_code),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_bdb)
{
    NRF_CLI_CMD(role, NULL, "role get/set", cmd_zb_role),
    NRF_CLI_CMD(extpanid, NULL, "extpanid get/set", cmd_zb_extpanid),
    NRF_CLI_CMD(panid, NULL, "panid get/set", cmd_zb_panid),
    NRF_CLI_CMD(start, NULL, "start commissionning", cmd_zb_start),
    NRF_CLI_CMD(channel, NULL, "channel get/set", cmd_zb_channel),
    NRF_CLI_CMD(ic, &m_sub_ic, "install code manipulation", NULL),
    NRF_CLI_CMD(legacy, NULL, "legacy mode enable/disable", cmd_zb_legacy),
    NRF_CLI_CMD(nwkkey, NULL, "network key set", cmd_zb_nwkkey),
    NRF_CLI_CMD(factory_reset, NULL, "factory reset", cmd_zb_factory_reset),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(bdb, &m_sub_bdb, "base device behaviour manipulation", NULL);

/** @} */
