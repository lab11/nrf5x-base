/**
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#ifndef SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_OTA_UPGRADE_ADDONS_H_
#define SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_OTA_UPGRADE_ADDONS_H_

#include "zboss_api.h"
#include "sdk_config.h"
#include "app_timer.h"


/*! \addtogroup zb_zcl_ota_upgrade_addons */
/*! @{ */

/**@brief Periodical discovery interval for OTA server. The value must be lower than 24 hours, as defined in the OUPC9 requirement of the OTA cluster PICS.
 *        Once an OTA server in the Zigbee network is found, the periodical discovery process will end.
 */
#ifndef ZB_ZCL_OTA_SERVER_DISCOVERY_INTERVAL_S
#define ZB_ZCL_OTA_SERVER_DISCOVERY_INTERVAL_S  (5 * 60)
#endif

/** The periodical discovery interval must be lower than 24 hours, as defined in the OUPC9 requirement of the OTA cluster PICS. */
#if ZB_ZCL_OTA_SERVER_DISCOVERY_INTERVAL_S >= (24 * 3600)
#warning ZB_ZCL_OTA_SERVER_DISCOVERY_INTERVAL_S is greater than 24h
#endif


/**
 * @brief OTA server periodical discovery context structure.
 */
typedef struct {
    app_timer_id_t const * p_timer_id; /**< The uninitialized app_timer instance that will be used to schedule periodical OTA server discovery. */
    zb_uint8_t             endpoint;   /**< Endpoint number on which the OTA server discovery is to be performed. */
    zb_uint8_t             is_running; /**< Boolean flag indicating that periodical discovery is started on an endpoint. */
} zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t;


/**@brief Function for initializing OTA Upgrade cluster client and periodical OTA server discovery.
 *
 * @param[in] p_context   Pointer to the uninitialized context structure of the periodical OTA server discovery.
 * @param[in] p_timer_id  Pointer to the app_timer instance that will be used to schedule periodical OTA server discovery.
 * @param[in] endpoint    Endpoint number on which the OTA server discovery is to be performed.
 *
 * @retval NRF_SUCCESS               The discovery timer was successfully created.
 * @retval NRF_ERROR_INVALID_PARAM   A parameter is invalid.
 * @retval NRF_ERROR_INVALID_STATE   The discovery timer module has not been initialized or
 *                                   the timer is running.
 *
 * @note This function must be called exactly once before any other call to other API provided by this module.
 */
ret_code_t zb_zcl_ota_upgrade_client_with_periodical_discovery_init(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context, app_timer_id_t const * p_timer_id, zb_uint8_t endpoint);

/**@brief Function for starting periodical OTA server discovery.
 *
 * @param[in] p_context  Pointer to the initialized context structure of the periodical OTA server discovery.
 *
 * @retval NRF_SUCCESS               The discovery timer was successfully started.
 * @retval NRF_ERROR_INVALID_PARAM   A parameter is invalid.
 * @retval NRF_ERROR_INVALID_STATE   The discovery timer module has not been initialized or
 *                                   the timer has not been created.
 * @retval NRF_ERROR_NO_MEM          The queue for application timer operations is full.
 */
ret_code_t zb_zcl_ota_upgrade_client_with_periodical_discovery_start(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context);

/**@brief Function for getting the state of periodical OTA server discovery.
 *
 * @param[in] p_context  Pointer to the initialized context structure of the periodical OTA server discovery.
 *
 * @retval ZB_TRUE   If periodical discovery is active on an endpoint that is associated with the context structure.
 * @retval ZB_FALSE  If periodical discovery is inactive on an endpoint that is associated with the context structure.
 *                   or the p_context is NULL.
 */
zb_bool_t zb_zcl_ota_upgrade_client_periodical_discovery_status(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context);

/**@brief Function for stopping the periodical OTA server discovery.
 *
 * @param[in] p_context  Pointer to the initialized context structure of the periodical OTA server discovery.
 *
 * @retval NRF_SUCCESS               The discovery timer was successfully stopped.
 * @retval NRF_ERROR_INVALID_PARAM   A parameter is invalid.
 * @retval NRF_ERROR_INVALID_STATE   The discovery timer module has not been initialized or the timer
 *                                   has not been created.
 * @retval NRF_ERROR_NO_MEM          The queue for application timer operations is full.
 */
ret_code_t zb_zcl_ota_upgrade_client_periodical_discovery_stop(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context);

/** @} */

#endif /* SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_OTA_UPGRADE_ADDONS_H_ */
