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
#include "zigbee_cli_cmd_zcl.h"

/**@brief Command set array
 */

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_attr)
{
    NRF_CLI_CMD(read, NULL, "read attribute", cmd_zb_readattr),
    NRF_CLI_CMD(write, NULL, "write attribute", cmd_zb_writeattr),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_subscribe)
{
    NRF_CLI_CMD(on, NULL, "subscribe to", cmd_zb_subscribe),
    NRF_CLI_CMD(off, NULL, "unsubscribe from", cmd_zb_subscribe),
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_zcl)
{
    NRF_CLI_CMD(attr, &m_sub_attr, "read/write attribute", NULL),
    NRF_CLI_CMD(ping, NULL, "ping over ZCL", cmd_zb_ping),
    NRF_CLI_CMD(subscribe, &m_sub_subscribe, "(un)subscribe to an attribute", NULL),
    NRF_CLI_CMD(cmd, NULL, "send generic command", cmd_zb_generic_cmd),
#ifdef ZIGBEE_CLI_DEBUG
    NRF_CLI_CMD(raw, NULL, "send raw ZCL frame", cmd_zb_zcl_raw),
#endif
    NRF_CLI_SUBCMD_SET_END
};

NRF_CLI_CMD_REGISTER(zcl, &m_sub_zcl, "ZCL manipulation", NULL);
