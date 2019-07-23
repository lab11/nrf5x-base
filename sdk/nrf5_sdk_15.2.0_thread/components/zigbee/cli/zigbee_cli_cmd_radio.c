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

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_fem)
{
    NRF_CLI_CMD(fem, NULL, "front-end module", cmd_zb_fem),
    NRF_CLI_SUBCMD_SET_END
};
NRF_CLI_CMD_REGISTER(radio, &m_sub_fem, "Radio manipulation", NULL);

/** @} */
