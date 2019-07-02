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

#ifndef PROTOCOL_API_H__
#define PROTOCOL_API_H__

#include "benchmark_api.h"
#include "nrf_cli.h"
#include <stdbool.h>
#include <stdint.h>

/**@brief Function for determining if a protocol-specific function returned an error.
 *
 * @param[in]   error_code   Return value from a function to check.
 *
 * @retval      true         If @p error_code is not a success.
 * @retval      false        If @p error_code indicates a success.
 */
bool protocol_is_error(uint32_t error_code);

/**@brief Function for protocol initialization.
 *
 * @note  This function should initialize protocol-specific Board Support Package.
*/
void protocol_init(void);

/**@brief Function for protocol processing. */
void protocol_process(void);

/**@brief Function for protocol sleep. */
void protocol_sleep(void);

/**@brief Function for handling protocol-specific SoC events.
 *
 * @param[in]   soc_evt     SoC stack event.
 */
void protocol_soc_evt_handler(uint32_t soc_evt);

/**@brief   Function that prints protocol-specific information about known peers.
 *
 * @param[in]  p_cli    Pointer to a CLI instance.
 * @param[in]  p_peers  Pointer to a structure describing all known peers.
 */
void protocol_cmd_peer_db_get(const nrf_cli_t * p_cli, const benchmark_peer_db_t * p_peers);

/**@brief   Function that prints protocol-specific information about a peer.
 *
 * @param[in]  p_cli   Pointer to a CLI instance.
 * @param[in]  p_peer  Pointer to a peer information structure.
 */
void protocol_cmd_peer_get(const nrf_cli_t * p_cli, const benchmark_peer_entry_t * p_peer);

/**@brief   Function that prints protocol-specific information about the local node.
 *
 * @param[in]  p_cli    Pointer to a CLI instance.
 */
void protocol_cmd_config_get(const nrf_cli_t * p_cli);

/**@brief   Function that sends a command to a remote peer.
 *
 * @note    This function receives the whole CLI input as an argument. The command that ought to be sent
 *          to the specified peer should be parsed from a full CLI command.
 *
 * @param[in]  p_cli       Pointer to a CLI instance.
 * @param[in]  p_peer_cli  Pointer to a peer CLI instance.
 * @param[in]  argc        Number of words the command contains.
 * @param[in]  argv        Array of pointers to the words the command contains.
 */
void protocol_cmd_remote_send(const nrf_cli_t * p_cli, const nrf_cli_t * p_peer_cli, size_t argc, char ** argv);

#endif // PROTOCOL_API_H__

/**
 * @}
 */
