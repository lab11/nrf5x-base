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

#include "nfc_meshcop_msg.h"

#include <string.h>

static NFC_NDEF_MSG_DEF(m_nfc_meshcop_msg, 1); /**< NFC NDEF message description, capacity - 1 record. */

/** @brief Function for generating a description of an NFC NDEF meshcop message.
 *
 * This function declares and initializes a static instance of an NFC NDEF message description
 * and NFC NDEF records descriptions.
 *
 * @note The payload data (@p p_meshcop_data) should be declared as static.
 *       If it is declared as automatic, the NDEF message encoding (see @ref nfc_meshcop_msg_encode)
 *       must be done in the same variable scope.
 *
 * @param[in]  p_meshcop_data       Pointer to meshcop data structure
 * @param[out] pp_meshcop_msg_desc      Pointer to pointer to the NDEF message description.
 *
 * @retval NRF_SUCCESS              If the description was successfully created.
 * @retval NRF_ERROR_INVALID_PARAM  If the meshcop data structure pointer was invalid (equal to NULL).
 */
static ret_code_t nfc_meshcop_msg_declare(nfc_meshcop_data_t const * const p_meshcop_data,
                                          nfc_ndef_msg_desc_t           ** pp_meshcop_msg_desc)
{
    ret_code_t               err_code;
    nfc_ndef_record_desc_t * p_meshcop_rec;

    // The message description is static, therefore.
    // you must clear the message (needed for supporting multiple calls).
    nfc_ndef_msg_clear(&NFC_NDEF_MSG(m_nfc_meshcop_msg));

    if (p_meshcop_data != NULL)
    {
        // Create NFC NDEF MeshCoP Record description.
        p_meshcop_rec = nfc_meshcop_rec_declare(p_meshcop_data);

        // Add MeshCoP record as lone record to message.
        err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(m_nfc_meshcop_msg), p_meshcop_rec);

        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    *pp_meshcop_msg_desc = &NFC_NDEF_MSG(m_nfc_meshcop_msg);

    return NRF_SUCCESS;
}

ret_code_t nfc_meshcop_msg_encode(nfc_meshcop_data_t const * const p_meshcop_data,
                                  uint8_t                        * p_buf,
                                  uint32_t                       * p_len)
{
    ret_code_t err_code;
    nfc_ndef_msg_desc_t * p_meshcop_msg_desc;

    // Create NFC NDEF message description with MeshCoP record.
    err_code = nfc_meshcop_msg_declare(p_meshcop_data, &p_meshcop_msg_desc);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Encode whole message into buffer.
    err_code = nfc_ndef_msg_encode(p_meshcop_msg_desc, p_buf, p_len);

    return err_code;
}
