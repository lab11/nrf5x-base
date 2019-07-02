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

 /**@file
 *
 * @defgroup bsp_thread Thread BSP Module
 * @{
 * @ingroup bsp
 *
 * @brief Module for setting LEDs according to Thread protocol state.
 */

#ifndef BSP_THREAD_H__
#define BSP_THREAD_H__

#include <stdint.h>
#include "bsp.h"
#include "sdk_errors.h"
#include <openthread/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief BSP indication of Thread connection states.
 */
typedef enum
{
    BSP_INDICATE_THREAD_DISABLED = BSP_INDICATE_IDLE,
    BSP_INDICATE_THREAD_DETACHED = BSP_INDICATE_BONDING,
    BSP_INDICATE_THREAD_CONNECTED = BSP_INDICATE_CONNECTED,
} bsp_indication_thread_t;

/** @brief BSP indication of Thread commissioning states.
 */
typedef enum
{
    BSP_INDICATE_COMMISSIONING_NONE,
    BSP_INDICATE_COMMISSIONING_NOT_COMMISSIONED,
    BSP_INDICATE_COMMISSIONING_IN_PROGRESS,
    BSP_INDICATE_COMMISSIONING_SUCCESS,
} bsp_indication_commissioning_t;

/**@brief Function for initializing the Thread BSP Module.
 *
 * Before calling this function, the BSP module must be initialized.
 *
 * @param[in]  p_instance       A pointer to the OpenThread instance.
 *
 * @retval     NRF_SUCCESS      If initialization was successful. Otherwise, a propagated error
 *                              code is returned.
 */
ret_code_t bsp_thread_init(otInstance * p_instance);

/**@brief Function for deinitializing the Thread BSP Module.
 *
 * @param[in]  p_instance       A pointer to the OpenThread instance.
 */
void bsp_thread_deinit(otInstance * p_instance);

/**@brief Function for configuring connection indicators to required states.
 *
 * @details     This function indicates the required state by means of LEDs (if enabled).
 *
 * @param[in]   indicate   State to be indicated.
 *
 * @retval      NRF_SUCCESS               If the state was successfully indicated.
 * @retval      NRF_ERROR_NO_MEM          If the internal timer operations queue was full.
 * @retval      NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized,
 *                                        or internal timer has not been created.
 */
ret_code_t bsp_thread_indication_set(bsp_indication_thread_t indicate);

/**@brief Function for configuring commissioning indicators to required states.
 *
 * @details     This function indicates the required state by means of LEDs (if enabled).
 *
 * @param[in]   indicate   State to be indicated.
 *
 * @retval      NRF_SUCCESS               If the state was successfully indicated.
 * @retval      NRF_ERROR_NO_MEM          If the internal timer operations queue was full.
 * @retval      NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized,
 *                                        or internal timer has not been created.
 */
ret_code_t bsp_thread_commissioning_indication_set(bsp_indication_commissioning_t indicate);

/**@brief Function to indicate received PING packet.
 *
 * @details     This function indicates the required state by means of LEDs (if enabled).
 *
 * @retval      NRF_SUCCESS               If the state was successfully indicated.
 * @retval      NRF_ERROR_NO_MEM          If the internal timer operations queue was full.
 * @retval      NRF_ERROR_INVALID_STATE   If the application timer module has not been initialized,
 *                                        or internal timer has not been created.
 */
ret_code_t bsp_thread_ping_indication_set(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_THREAD_H__ */

/** @} */
