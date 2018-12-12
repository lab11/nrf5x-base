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

/**
 * @brief Set or get Zigbee role of the device.
 *
 * @code
 * bdb role [<role>]
 * @endcode
 *
 * @pre Setting only before starting @ref zigbee. Reading only after starting @ref zigbee.
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
 * @brief Start the commissioning process.
 *
 * @code
 * > bdb start
 * Started coordinator
 * Done
 * @endcode
 */
static void cmd_zb_start(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    uint32_t channel;
    zb_bool_t ret;

    if (nrf_cli_help_requested(p_cli))
    {
        print_usage(p_cli, argv[0], "start - start top level commissioning");
        return;
    }

    if (m_stack_is_started == ZB_TRUE)
    {
        print_error(p_cli, "Device already commissioned or commissioning is in progress");
        return;
    }

    channel = zb_get_bdb_primary_channel_set();

    switch (m_default_role)
    {
        case ZB_NWK_DEVICE_TYPE_ROUTER:
            zb_set_network_router_role(channel);
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Started router\r\n");
            break;

        case ZB_NWK_DEVICE_TYPE_COORDINATOR:
            zb_set_network_coordinator_role(channel);
            nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Started coordinator\r\n");
            break;

        default:
            print_error(p_cli, "Role unsupported\r\n");
            return;
    }

    m_stack_is_started = ZB_TRUE;

    UNUSED_RETURN_VALUE(zboss_start_no_autostart());
    /* Iterate one time to propagate the signal to the zboss_signal_handler */
    zboss_main_loop_iteration();

    ret = bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING | ZB_BDB_NETWORK_FORMATION);

    if (ret)
    {
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
 * @pre Setting only before starting @ref zigbee. Reading only after starting @ref zigbee.
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
        print_64(p_cli, extpanid);
        print_done(p_cli, ZB_TRUE);
    }
    else if (argc == 2)
    {
        zb_uint8_t addr_mode;

        if ((parse_address_string(argv[1], (zb_addr_u *)extpanid, &addr_mode) == ZB_TRUE) &&
            (addr_mode == ZB_APS_ADDR_MODE_64_ENDP_PRESENT))
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

/** @brief Set or get 802.15.4 channel
 *
 * @code
 * bdb channel <n>
 * @endcode
 *
 *  @pre Setting only before starting @ref zigbee.
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

            if (!(channel_mask & 0x7FFF800))
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

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_bdb)
{
    NRF_CLI_CMD(role, NULL, "role get/set", cmd_zb_role),
    NRF_CLI_CMD(extpanid, NULL, "extpanid get/set", cmd_zb_extpanid),
    NRF_CLI_CMD(start, NULL, "start commissionning", cmd_zb_start),
    NRF_CLI_CMD(channel, NULL, "channel get/set", cmd_zb_channel),
    NRF_CLI_SUBCMD_SET_END
};
NRF_CLI_CMD_REGISTER(bdb, &m_sub_bdb, "base device behaviour manipulation", NULL);

/** @} */
