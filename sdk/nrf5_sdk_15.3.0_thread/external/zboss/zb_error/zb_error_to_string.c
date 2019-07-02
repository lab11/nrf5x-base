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

/**
 * @addtogroup zb_error
 *
 * @brief Module for converting ZBOSS error code into a printable string.
 * @{
 */
#include "sdk_common.h"

#if NRF_MODULE_ENABLED(ZB_ERROR_TO_STRING)
#include "zb_error_to_string.h"

/**
 * @brief Macro for adding an entity to the description array.
 *
 * Macro that helps to create a single entity in the description array.
 */
#define ZB_ERROR_STRING(mnemonic) {.code = -mnemonic, .name = #mnemonic}

/**
 * @brief Array entity element that describes an error.
 */
typedef struct
{
    zb_ret_t     code; /**< Error code. */
    char const * name; /**< Descriptive name (the same as the internal error mnemonic). */
} zb_strerror_desc_t;

/**
 * @brief Unknown error code.
 *
 * The constant string used by @ref zb_strerror_get when the error description was not found.
 */
static char const m_unknown_str[] = "Unknown error code";

/**
 * @brief Array with error codes.
 *
 * Array that describes error codes.
 *
 * @note It is required for this array to have error codes placed in ascending order.
 *       This condition is checked in automatic unit test before the release.
 */
static zb_strerror_desc_t const zb_strerror_array[] =
{
    ZB_ERROR_STRING(RET_OK),
    ZB_ERROR_STRING(RET_ERROR),
    ZB_ERROR_STRING(RET_BLOCKED),
    ZB_ERROR_STRING(RET_EXIT),
    ZB_ERROR_STRING(RET_BUSY),
    ZB_ERROR_STRING(RET_EOF),
    ZB_ERROR_STRING(RET_OUT_OF_RANGE),
    ZB_ERROR_STRING(RET_EMPTY),
    ZB_ERROR_STRING(RET_CANCELLED),

    ZB_ERROR_STRING(RET_INVALID_PARAMETER_1),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_2),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_3),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_4),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_5),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_6),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_7),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_8),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_9),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_10),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER_11_OR_MORE),
    ZB_ERROR_STRING(RET_PENDING),
    ZB_ERROR_STRING(RET_NO_MEMORY),
    ZB_ERROR_STRING(RET_INVALID_PARAMETER),
    ZB_ERROR_STRING(RET_OPERATION_FAILED),
    ZB_ERROR_STRING(RET_BUFFER_TOO_SMALL),
    ZB_ERROR_STRING(RET_END_OF_LIST),
    ZB_ERROR_STRING(RET_ALREADY_EXISTS),
    ZB_ERROR_STRING(RET_NOT_FOUND),
    ZB_ERROR_STRING(RET_OVERFLOW),
    ZB_ERROR_STRING(RET_TIMEOUT),
    ZB_ERROR_STRING(RET_NOT_IMPLEMENTED),
    ZB_ERROR_STRING(RET_NO_RESOURCES),
    ZB_ERROR_STRING(RET_UNINITIALIZED),
    ZB_ERROR_STRING(RET_NO_SERVER),
    ZB_ERROR_STRING(RET_INVALID_STATE),
    ZB_ERROR_STRING(RET_DOES_NOT_EXIST),
    ZB_ERROR_STRING(RET_CONNECTION_FAILED),
    ZB_ERROR_STRING(RET_CONNECTION_LOST),
    ZB_ERROR_STRING(RET_CANCELLED_BY_USER),
    ZB_ERROR_STRING(RET_UNAUTHORIZED),
    ZB_ERROR_STRING(RET_CONFLICT),
    ZB_ERROR_STRING(RET_COULD_NOT_OPEN_FILE),
    ZB_ERROR_STRING(RET_NO_MATCH),
    ZB_ERROR_STRING(RET_PROTOCOL_ERROR),
    ZB_ERROR_STRING(RET_VERSION),
    ZB_ERROR_STRING(RET_MALFORMED_ADDRESS),
    ZB_ERROR_STRING(RET_COULD_NOT_READ_FILE),
    ZB_ERROR_STRING(RET_FILE_NOT_FOUND),
    ZB_ERROR_STRING(RET_DIRECTORY_NOT_FOUND),
    ZB_ERROR_STRING(RET_CONVERSION_ERROR),
    ZB_ERROR_STRING(RET_INCOMPATIBLE_TYPES),
    ZB_ERROR_STRING(RET_INCOMPATIBLE_TYPES_IN_COMPARE),
    ZB_ERROR_STRING(RET_INCOMPATIBLE_TYPES_IN_ASSIGNMENT),
    ZB_ERROR_STRING(RET_INCOMPATIBLE_TYPES_IN_EXPRESSION),
    ZB_ERROR_STRING(RET_ILLEGAL_COMPARE_OPERATION),
    ZB_ERROR_STRING(RET_FILE_CORRUPTED),
    ZB_ERROR_STRING(RET_PAGE_NOT_FOUND),
    ZB_ERROR_STRING(RET_FILE_WRITE_ERROR),
    ZB_ERROR_STRING(RET_FILE_READ_ERROR),
    ZB_ERROR_STRING(RET_FILE_PARTIAL_WRITE),
    ZB_ERROR_STRING(RET_TOO_MANY_OPEN_FILES),
    ZB_ERROR_STRING(RET_ILLEGAL_REQUEST),
    ZB_ERROR_STRING(RET_INVALID_BINDING),
    ZB_ERROR_STRING(RET_INVALID_GROUP),
    ZB_ERROR_STRING(RET_TABLE_FULL),
    ZB_ERROR_STRING(RET_NO_ACK),
    ZB_ERROR_STRING(RET_ACK_OK),
    ZB_ERROR_STRING(RET_NO_BOUND_DEVICE),
    ZB_ERROR_STRING(RET_IGNORE),
    ZB_ERROR_STRING(RET_AGAIN),
    ZB_ERROR_STRING(RET_DEVICE_NOT_FOUND)
};

char const * zb_error_to_string_get(zb_ret_t code)
{
    char const * p_ret = zb_error_to_string_find(code);
    return (p_ret == NULL) ? m_unknown_str : p_ret;
}

char const * zb_error_to_string_find(zb_ret_t code)
{
    zb_strerror_desc_t const * p_start;
    zb_strerror_desc_t const * p_end;
    p_start = zb_strerror_array;
    p_end   = zb_strerror_array + ARRAY_SIZE(zb_strerror_array);

    while (p_start < p_end)
    {
        zb_strerror_desc_t const * p_mid = p_start + ((p_end - p_start) / 2);
        zb_ret_t mid_c = p_mid->code;
        if (mid_c > code)
        {
            p_end = p_mid;
        }
        else if (mid_c < code)
        {
            p_start = p_mid + 1;
        }
        else
        {
            return p_mid->name;
        }
    }
    return NULL;
}

#endif /* ZB_ERROR_TO_STRING_ENABLE */

/** @} */
