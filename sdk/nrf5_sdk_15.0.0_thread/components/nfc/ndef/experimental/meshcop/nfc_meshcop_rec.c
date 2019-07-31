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

#include "nfc_meshcop_rec.h"

#include <string.h>

#include "nrf_assert.h"
#include "nrf_error.h"

#define MESHCOP_NDEF_TYPE "thread/meshcop/data"

/**
 * @brief Type of description of the payload of a MeshCoP record.
 */
typedef struct
{
    uint8_t const * p_meshcop_data;
    uint8_t         meshcop_data_len;
} meshcop_payload_desc_t;

/**
 * @brief Function for constructing the payload for a MeshCoP record.
 *
 * This function encodes the payload according to the MeshCoP record definition. It implements an API
 * compatible with @ref p_payload_constructor_t.
 *
 * @param[in]     p_input       Pointer to the description of the payload.
 * @param[out]    p_buff        Pointer to payload destination. If NULL, function will
 *                              calculate the expected size of the MeshCoP record payload.
 * @param[in,out] p_len         Size of available memory to write as input. Size of generated
 *                              payload as output.
 *
 * @retval NRF_SUCCESS          If the payload was encoded successfully.
 * @retval NRF_ERROR_NO_MEM     If the predicted payload size is bigger than the provided buffer space.
 */
static ret_code_t nfc_meshcop_payload_constructor(meshcop_payload_desc_t * p_input,
                                                  uint8_t                * p_buff,
                                                  uint32_t               * p_len)
{
    if (p_buff != NULL)
    {
        /* Verify if there is enough available memory */
        if (p_input->meshcop_data_len > *p_len)
        {
            return NRF_ERROR_NO_MEM;
        }

        /* Copy descriptor content into the buffer */
        memcpy(p_buff, p_input->p_meshcop_data, p_input->meshcop_data_len);
    }

    *p_len = p_input->meshcop_data_len;

    return NRF_SUCCESS;
}

static meshcop_payload_desc_t m_meshcop_payload_desc;
static const uint8_t          m_static_meshcop_type[] = MESHCOP_NDEF_TYPE;

static NFC_NDEF_GENERIC_RECORD_DESC_DEF(m_meshcop_rec,
                                        TNF_MEDIA_TYPE,
                                        NULL,
                                        0,  // no id
                                        m_static_meshcop_type,
                                        (sizeof(m_static_meshcop_type) - 1),
                                        nfc_meshcop_payload_constructor,
                                        &m_meshcop_payload_desc);

nfc_ndef_record_desc_t * nfc_meshcop_rec_declare(nfc_meshcop_data_t const * const p_meshcop_data)
{
    ASSERT(p_meshcop_data != NULL);

    m_meshcop_payload_desc.p_meshcop_data   = (uint8_t const *)p_meshcop_data;
    m_meshcop_payload_desc.meshcop_data_len = sizeof(nfc_meshcop_data_t);

   return &NFC_NDEF_GENERIC_RECORD_DESC(m_meshcop_rec);
}
