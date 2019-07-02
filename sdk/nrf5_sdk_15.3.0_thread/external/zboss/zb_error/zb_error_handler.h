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
 *  @defgroup zb_error ZBOSS error handling utilities
 *  @{
 */

/**
 * @brief
 *  This file defines the error handler for ZBOSS error codes.
 */

#ifndef ZB_ERROR_HANDLER_H__
#define ZB_ERROR_HANDLER_H__

#include "sdk_config.h"
#include "app_error.h"
#include "zboss_api.h"
#include "zb_errors.h"

#define ZB_ERROR_BASE_NUM 20000 /*< ZBOSS stack error codes base. */

#if (defined ZB_ERROR_PRINT_TO_LOG) && (ZB_ERROR_PRINT_TO_LOG == 1)
#include "zb_error_to_string.h"
#include "nrf_log.h"

/**@brief Macro for calling error handler function if supplied error code any other than RET_OK.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define ZB_ERROR_CHECK(ERR_CODE)                                   \
    do                                                             \
    {                                                              \
        const uint32_t LOCAL_ERR_CODE = (uint32_t) (-ERR_CODE);    \
        if (LOCAL_ERR_CODE != RET_OK)                              \
        {                                                          \
            NRF_LOG_ERROR("ERROR %u [%s] at %s:%u",                \
                          LOCAL_ERR_CODE,                          \
                          zb_error_to_string_get(LOCAL_ERR_CODE),  \
                          (uint8_t*) __FILE__,                     \
                          __LINE__);                               \
            NRF_LOG_FLUSH();                                       \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE + ZB_ERROR_BASE_NUM); \
        }                                                          \
    } while (0)

/**@brief Macro for calling error handler function if bdb_start_top_level_commissioning
 *        return code indicates that it BDB procedure did not succeed.
 *
 * @param[in] COMM_STATUS Value returned by bdb_start_top_level_commissioning function.
 */
#define ZB_COMM_STATUS_CHECK(COMM_STATUS)                               \
    do                                                                  \
    {                                                                   \
        if (COMM_STATUS != ZB_TRUE)                                     \
        {                                                               \
            NRF_LOG_ERROR("Unable to start BDB commissioning at %s:%u", \
                               (uint8_t*) __FILE__,                     \
                               __LINE__);                               \
            ZB_ERROR_CHECK(RET_ERROR);                                  \
        }                                                               \
    } while (0)
#else
#define ZB_ERROR_CHECK(ERR_CODE)                                   \
    do                                                             \
    {                                                              \
        const uint32_t LOCAL_ERR_CODE = (uint32_t) (-ERR_CODE);    \
        if (LOCAL_ERR_CODE != RET_OK)                              \
        {                                                          \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE + ZB_ERROR_BASE_NUM); \
        }                                                          \
    } while (0)

#define ZB_COMM_STATUS_CHECK(COMM_STATUS)                               \
    do                                                                  \
    {                                                                   \
        if (COMM_STATUS != ZB_TRUE)                                     \
        {                                                               \
            ZB_ERROR_CHECK(RET_ERROR);                                  \
        }                                                               \
    } while (0)
#endif

/**
 * @}
 *
 */

#endif // ZB_ERROR_HANDLER_H__
