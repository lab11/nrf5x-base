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

#ifndef _CSL_WAKE_H__
#define _CSL_WAKE_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CSL_WAKE_BURST_MAX_CNT             10
#define CSL_WAKE_BURST_DEFAULT_CNT         5
#define CSL_WAKE_BURST_DEFAULT_PERIOD      4000000UL
#define CSL_WAKE_BURST_DEFAULT_IFS         2500
#define CSL_WAKE_BURST_DEFAULT_NO_CSMA_CNT 2

typedef enum {
    CSL_WAKE_BURST_OK,
    CSL_WAKE_BURST_BAD_IDX,
    CSL_WAKE_BURST_ALREADY_RUNNING,
    CSL_WAKE_BURST_ABORTED,
    CSL_WAKE_BURST_TIMING_ERROR
} csl_wake_burst_res_t;

/**@brief Set number of CSL wake bursts.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_cnt_set(size_t cnt);

/**@brief Get number of CSL wake bursts.
 *
 * @return   Number of CSL wake bursts.
 */
size_t csl_wake_burst_cnt_get(void);

/**@brief Set period of given CSL wake bursts.
 *
 * @param[in] idx     Index of CSL wake burst.
 * @param[in] period  Period of CSL wake burst.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_period_set(size_t idx, uint32_t period);

/**@brief Get period of given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 *
 * @return   Period of given CSL wake burst.
 */
uint32_t csl_wake_burst_period_get(size_t idx);

/**@brief Set interframe spacing of given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 * @param[in] ifs  Interframe spacing of CSL wake burst.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_ifs_set(size_t idx, uint32_t ifs);

/**@brief Get interframe spacing of given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 *
 * @return   Interframe spacing of given CSL wake burst.
 */
uint32_t csl_wake_burst_ifs_get(size_t idx);

/**@brief Select if CSMA-CA should be used during transmissions in given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 * @param[in] ifs  If CSMA-CA should be enabled.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_csma_ca_set(size_t idx, bool cca);

/**@brief Check if CSMA-CA is enabled for given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 *
 * @retval  true  if CSMA-CA is enabled for given CSL wake burst.
 * @retval  false if CSMA-CA is disabled for given CSL wake burst.
 */
bool csl_wake_burst_csma_ca_get(size_t idx);

/**@brief Select if ACK request (AR) bit should be set during transmissions in given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 * @param[in] ifs  If AR bit should be set.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_ar_bit_set(bool ar);

/**@brief Check if AR bit is set for given CSL wake bursts.
 *
 * @param[in] idx  Index of CSL wake burst.
 *
 * @retval  true  if CSMA-CA is enabled for given CSL wake burst.
 * @retval  false if CSMA-CA is disabled for given CSL wake burst.
 */
bool csl_wake_burst_ar_bit_get(void);

/**@brief Init CSL wake module. */
void csl_wake_burst_init(void);

/**@brief Start sending CSL wake bursts.
 *
 * @return   CSL_WAKE_BURST_OK on success, otherwise an error code.
 */
csl_wake_burst_res_t csl_wake_burst_send(void);

/**@brief Abort CSL wake procedure. */
void csl_wake_burst_abort(void);

/**@brief Get number of transmitted frames in last CSL wake procedure.
 * 
 * @return    Number of transmitted frames.
 */
uint32_t csl_wake_txed_frames(void);

/**@brief Get number of interframe spacing errors in last CSL wake procedure.
 *
 * @return    Number of interframe spacing errors.
 */
uint32_t csl_wake_ifs_errors(void);


/**@brief This function is called to notify upper layer that CSL wake procedure has finished.
 *
 * @param[in] result  Result of CSL wake procedure.
 */
extern void csl_wake_burst_ended(csl_wake_burst_res_t result);

#endif //_CSL_WAKE_H__
