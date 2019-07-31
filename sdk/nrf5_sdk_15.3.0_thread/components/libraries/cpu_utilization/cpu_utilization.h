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

#ifndef _CPU_UTILIZATION_H__
#define _CPU_UTILIZATION_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Function for initializing the CPU utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_init(void);

/**@brief Function for deinitializing the CPU utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_deinit(void);

/**@brief Function for starting the CPU utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_start(void);

/**@brief Function for stopping the CPU utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_stop(void);

/**@brief Function for getting the CPU utilization.
 *
 * @retval  CPU utilization in units of 0.01%.
 */
uint32_t cpu_utilization_get(void);

/**@brief Function for clearing the CPU utilization timer values. */
void cpu_utilization_clear(void);

#ifdef __cplusplus
}
#endif

#endif // _CPU_UTILIZATION_H__

/** @} */
