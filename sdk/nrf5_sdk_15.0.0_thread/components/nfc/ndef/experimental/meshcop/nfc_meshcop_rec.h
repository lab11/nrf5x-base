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

#ifndef NFC_MESHCOP_REC_H__
#define NFC_MESHCOP_REC_H__

/**@file
 *
 * @defgroup nfc_meshcop_rec Thread MeshCoP records
 * @{
 * @ingroup  nfc_meshcop_msg
 *
 * @brief    Generation of NFC NDEF MeshCoP record descriptions.
 *
 */

#include "nfc_ndef_record.h"

#include <openthread/types.h>

#define PSKD_MAX_LEN    32

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Thread MeshCoP data descriptor.
 */
typedef struct
{
    char          psk_d[PSKD_MAX_LEN + 1]; 	// Pre-Shared Key for the Device.
    otExtAddress  eui64; // 64-bit Extended Unique Identifier.
} nfc_meshcop_data_t;

/** @brief Function for generating a description of a MeshCoP record.
 *
 * This function declares and initializes a static instance of an NFC NDEF record description
 * of a MeshCoP record.
 *
 * @note The record payload data (@p p_meshcop_data) should be declared as static.
 *       If it is declared as automatic, the NDEF message encoding (see @ref nfc_meshcop_msg_encode)
 *       must be done in the same variable scope.
 *
 * @param[in]  p_meshcop_data       Pointer to MeshCoP data structure
 *
 * @return Pointer to the description of the record.
 */
nfc_ndef_record_desc_t * nfc_meshcop_rec_declare(nfc_meshcop_data_t const * const p_meshcop_data);
/** @} */
#ifdef __cplusplus
}
#endif

#endif // NFC_MESHCOP_REC_H__
