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
 * @defgroup zb_cli_cmd_radio Radio commands
 * @ingroup zb_cli
 *
 * @{
 */

/* Fixes to make Lint passable - radio driver headers are not linted */
#ifndef RAAL_SOFTDEVICE
#define RAAL_SOFTDEVICE 0
#endif /* RAAL_SOFTDEVICE */

#include "nrf_802154.h"
#include "fem/nrf_fem_control_api.h"

/**@brief Activate or deactivate FEM
 *
 * @code
 * radio fem <enable|disable>
 * @endcode
 *
 * The configuration of the FEM is the default one (defined at compile time,
 * see the define NRF_802154_FEM_DEFAULT_SETTINGS).
 *
 * For more information, see the description of the FEM on the @link_radio_driver
 * Wiki.
 */
static void cmd_zb_fem(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
#ifndef ENABLE_FEM
    print_error(p_cli, "FEM support disabled");
#else

    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    /* Set the configuration */
    nrf_fem_control_cfg_set(&NRF_802154_FEM_DEFAULT_SETTINGS);

    if (argc == 2)
    {
        if (!strcmp(argv[1], "enable"))
        {
            nrf_fem_control_activate();
            print_done(p_cli, ZB_FALSE);
        }
        else if (!strcmp(argv[1], "disable"))
        {
            nrf_fem_control_deactivate();
            print_done(p_cli, ZB_FALSE);
        }
        else
        {
            print_error(p_cli, "Wrong usage. Use 'fem enable' or 'fem disable' for enabling/disabling");
        }
    }
    else
    {
        print_error(p_cli, "Wrong number of arguments");
    }
#endif
}

/**@brief Function to set the 802.15.4 channel directly.
 *
 * @code
 * radio channel set <n>
 * @endcode
 *
 * The <n> has to be between 11 and 26 included, since these channels are supported by the driver.
 *
 * @note This function sets the channel directly at runtime, contrary to the `bdb channel` function,
 *       which defines the channels allowed for the Zigbee network formation.
 */
static void cmd_zb_channel_set(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc == 2)
    {
        uint8_t channel;
        if (!sscan_uint8(argv[1], &channel))
        {
            print_error(p_cli, "Invalid channel");
        }
        else if ((channel < 11) || (channel > 26))
        {
            print_error(p_cli, "Only channels from 11 to 26 are supported");
        }
        else
        {
            nrf_802154_channel_set(channel);
            print_done(p_cli, ZB_TRUE);
        }
    }
    else
    {
        print_error(p_cli, "Wrong number of arguments");
    }
}

/**@brief Function to get the current 802.15.4 channel.
 *
 * @code
 * radio channel get
 * @endcode
 * 
 */
static void cmd_zb_channel_get(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "Current operating channel: %d", nrf_802154_channel_get());
    print_done(p_cli, ZB_TRUE);
}

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_channel)
{
    NRF_CLI_CMD(set, NULL, "set 802.15.4 channel", cmd_zb_channel_set),
    NRF_CLI_CMD(get, NULL, "get 802.15.4 channel", cmd_zb_channel_get),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_fem)
{
    NRF_CLI_CMD(fem, NULL, "front-end module", cmd_zb_fem),
    NRF_CLI_CMD(channel, &m_sub_channel, "get/set channel", NULL),
    NRF_CLI_SUBCMD_SET_END
};
NRF_CLI_CMD_REGISTER(radio, &m_sub_fem, "Radio manipulation", NULL);

/** @} */
