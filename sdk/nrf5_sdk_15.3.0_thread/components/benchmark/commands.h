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

#ifndef COMMANDS_H__
#define COMMANDS_H__

#include "nrf_cli.h"
#include <stdint.h>

#define DEVICE_ID_HI(device_id) ((uint32_t)((device_id >> 32) & 0xFFFFFFFF))  /** Get more significat 32-bits out of 64-bit device ID value. */
#define DEVICE_ID_LO(device_id) ((uint32_t)(device_id & 0xFFFFFFFF))          /** Get less significat 32-bits out of 64-bit device ID value. */

/**@brief  Default command handler, prints error message and returns.
 *
 * @param[in]  p_cli  Pointer to a CLI instance.
 * @param[in]  argc   Number of words the unknown subcommand contains.
 * @param[in]  argv   Array of pointers to the words the unknown subcommand gets as arguments.
 */
void cmd_default(nrf_cli_t const * p_cli, size_t argc, char ** argv);

/**@brief  Prints the unified message to indicate that command was successfully executed.
 *
 * @param[in]  p_cli    Pointer to a CLI instance.
 */
void print_done(nrf_cli_t const * p_cli);

/**@brief  Prints the unified message to indicate that command execution has failed.
 *
 * @param[in]  p_cli     Pointer to a CLI instance.
 * @param[in]  p_reason  Pointer to an error message.
 */
void print_error(nrf_cli_t const * p_cli, char * p_reason);

#endif // COMMANDS_H__
