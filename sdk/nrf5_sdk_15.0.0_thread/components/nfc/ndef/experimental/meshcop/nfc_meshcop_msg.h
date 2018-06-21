/* Copyright (c) 2017 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
