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

#ifndef CLI_H__
#define CLI_H__

#include "benchmark_api.h"
#include "nrf_cli.h"

#define CLI_REMOTE_INSTANCES 5       /**< Maximal number of remote CLI connections. */


/**@brief Function for initializing the CLI.
 *
 * Serial connection and RTT console are supported.
 */
void cli_init(void);

/**@brief Function for initializing the remote CLI.
 */
void cli_remote_init(void);

/**@brief Function for starting the CLI.
 */
void cli_start(void);

/**@brief Function for starting the remote CLI.
 */
void cli_remote_start(void);

/**@brief Function for processing CLI operations.
 */
void cli_process(void);

/**@brief Get remote CLI instance from index.
 */
nrf_cli_t const * cli_remote_get(size_t idx);

/**@brief Set address of a remote peer.
 *
 * @param[in] p_peer_cli      Pointer to a remote peer CLI instance.
 * @param[in] p_peer_address  Pointer to protocol-specific address structure.
 */
void cli_remote_peer_set(nrf_cli_t const * p_peer_cli, benchmark_address_context_t * p_peer_address);

#endif /* CLI_H__ */
