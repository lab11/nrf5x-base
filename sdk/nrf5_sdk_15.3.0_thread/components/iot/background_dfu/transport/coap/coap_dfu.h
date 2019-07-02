/**
 * Copyright (c) 2017 - 2019, Nordic Semiconductor ASA
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

/** @file
 *
 * @defgroup background_dfu_coap_api background_dfu_coap_api.h
 * @{
 * @ingroup background_dfu
 * @brief Background DFU CoAP transport api.
 *
 */

#ifndef BACKGROUND_DFU_COAP_API_H_
#define BACKGROUND_DFU_COAP_API_H_

#include "coap_api.h"

struct background_dfu_diagnostic;

/** @brief Initialize DFU client.
 *
 *  @param[in] Application context for CoAP.
 *
 *  @return NRF_SUCCESS on success, otherwise an error code is returned.
 */
uint32_t coap_dfu_init(const void * p_context);

/** @brief Process CoAP DFU pending operations.
 *
 * This function shall be called in the main loop.
 */
void coap_dfu_process(void);

/** @brief Trigger DFU.
 *
 *  @param[in] p_remote Address of a host which should be queried for DFU. If NULL then the
 *                      function will attempt to discover any DFU hosts by sending
 *                      a trigger request to a multicast address.
 *
 *  @return NRF_SUCCESS on success, otherwise an error code is returned.
 */
uint32_t coap_dfu_trigger(const coap_remote_t * p_remote);

/** @brief Reset DFU state. */
void coap_dfu_reset_state(void);

/** @brief Get DFU diagnostic information.
 *
 *  @param[out] p_diag a pointer to a structure where diagnostic information
 *                     will be copied.
 */
void coap_dfu_diagnostic_get(struct background_dfu_diagnostic * p_diag);

/** @brief Handle DFU error.
 *
 *  This function can be implemented in the application to undertake application-specific action on DFU error.
 */
extern void coap_dfu_handle_error(void);

#endif /* BACKGROUND_DFU_COAP_API_H_ */

/** @} */
