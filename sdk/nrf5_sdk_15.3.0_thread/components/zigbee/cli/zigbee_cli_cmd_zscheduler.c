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
#include "zigbee_cli.h"
#include "zigbee_cli_utils.h"

/**@brief Command set array
 */


#ifdef ZIGBEE_CLI_DEBUG
/**@brief Suspend Zigbee scheduler processing
 *
 * @code
 * zscheduler suspend
 * @endcode
 *
 */
static void cmd_zb_suspend(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 1)
    {
        print_error(p_cli, "Invalid number of arguments");
        return;
    }

    zb_cli_suspend();
    print_done(p_cli, ZB_TRUE);
}


/**@brief Resume Zigbee scheduler processing
 *
 * @code
 * zscheduler resume
 * @endcode
 *
 */
static void cmd_zb_resume(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    if (nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 1)
    {
        print_error(p_cli, "Invalid number of arguments");
        return;
    }

    zb_cli_resume();
    print_done(p_cli, ZB_TRUE);
}


NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_zigbee)
{
    NRF_CLI_CMD(suspend, NULL, "suspend Zigbee scheduler processing", cmd_zb_suspend),
    NRF_CLI_CMD(resume, NULL, "suspend Zigbee scheduler processing", cmd_zb_resume),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(zscheduler, &m_sub_zigbee, "Zigbee scheduler manipulation", NULL);
#endif
