/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
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
#ifndef NFC_MESHCOP_MSG_H__
#define NFC_MESHCOP_MSG_H__

/**@file
 *
 * @defgroup nfc_meshcop_msg Thread MeshCoP messages
 * @{
 * @ingroup  nfc_ndef_messages
 *
 * @brief    Generation of NFC NDEF messages with a Thread MeshCoP record.
 *
 */

#include <stdint.h>

#include "nfc_ndef_msg.h"
#include "nfc_meshcop_rec.h"
#include "nrf_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Function for encoding an NFC NDEF MeshCoP message.
 *
 * @note The payload data (@p p_meshcop_data) should be declared as static.
 *       If it is declared as automatic, the NDEF message encoding (see @ref nfc_meshcop_msg_encode)
 *       must be done in the same variable scope.
 *
 * @param[in]       p_meshcop_data      Pointer to MeshCoP data structure
 * @param[out]      p_buf               Pointer to the buffer for the message. When NULL p_len returns
 *                                      required buffer size.
 * @param[in,out]   p_len               Size of the available memory for the message as input.
 *                                      Size of the generated message as output.
 *                                      Size of the required memory for the message when p_buf is NULL.
 *
 * @retval NRF_SUCCESS                  If the description was successfully created.
 * @retval NRF_ERROR_NO_MEM             If the predicted message size is bigger than the provided
 *                                      buffer space.
 * @retval Other                        Other codes might be returned depending on
 *                                      the function @ref nfc_ndef_msg_encode.
 */
ret_code_t nfc_meshcop_msg_encode(nfc_meshcop_data_t const * const p_meshcop_data,
                                  uint8_t                        * p_buf,
                                  uint32_t                       * p_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // NFC_MESHCOP_MSG_H__
