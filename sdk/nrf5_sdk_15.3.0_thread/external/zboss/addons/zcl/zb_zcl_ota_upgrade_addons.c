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

#include "zboss_api.h"
#include "zb_zcl_ota_upgrade_addons.h"
#include "sdk_macros.h"
#include <string.h>


static void ota_server_discovery_handler(void * p_ctx)
{
    zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context = (zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t *)p_ctx;

    // The user callback function is not invoked upon OTA server discovery failure.
    // This is the reason why an explicit attribute value check must be used.
    zb_zcl_attr_t * attr_desc = zb_zcl_get_attr_desc_a(p_context->endpoint, ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ID);

    // A periodical discovery was started on an endpoint that does not implement OTA client. Abort the application.
    ZB_ASSERT(attr_desc);

    if (ZB_IS_64BIT_ADDR_UNKNOWN(attr_desc->data_p))
    {
        // Restart OTA server discovery. In case of OOM state, the discovery mechanism is restarted in the next interval.
        UNUSED_RETURN_VALUE(ZB_GET_OUT_BUF_DELAYED(zb_zcl_ota_upgrade_init_client));
    }
    else
    {
        // Periodical OTA server discovery is finished. Stop the timer.
        UNUSED_RETURN_VALUE(zb_zcl_ota_upgrade_client_periodical_discovery_stop(p_context));
    }
}

ret_code_t zb_zcl_ota_upgrade_client_with_periodical_discovery_init(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context, app_timer_id_t const * p_timer_id, zb_uint8_t endpoint)
{
    VERIFY_TRUE((p_context != NULL), NRF_ERROR_INVALID_PARAM);
    VERIFY_TRUE((p_timer_id != NULL), NRF_ERROR_INVALID_PARAM);

    memset(p_context, 0, sizeof(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t));
    p_context->p_timer_id = p_timer_id;
    p_context->endpoint   = endpoint;

    return app_timer_create(p_context->p_timer_id, APP_TIMER_MODE_REPEATED, ota_server_discovery_handler);
}

ret_code_t zb_zcl_ota_upgrade_client_with_periodical_discovery_start(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context)
{
    ret_code_t err_code;

    VERIFY_TRUE((p_context != NULL), NRF_ERROR_INVALID_PARAM);

    err_code = app_timer_start(*(p_context->p_timer_id), APP_TIMER_TICKS(ZB_ZCL_OTA_SERVER_DISCOVERY_INTERVAL_S * 1000), p_context);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Start OTA server discovery immediately.
    p_context->is_running = 1;
    ota_server_discovery_handler(p_context);

    return err_code;
}

zb_bool_t zb_zcl_ota_upgrade_client_periodical_discovery_status(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context)
{
    if (p_context == NULL)
    {
        return ZB_FALSE;
    }

    return (p_context->is_running ? ZB_TRUE : ZB_FALSE);
}

ret_code_t zb_zcl_ota_upgrade_client_periodical_discovery_stop(zb_zcl_ota_upgrade_client_periodical_discovery_ctx_t * p_context)
{
    ret_code_t err_code;

    VERIFY_TRUE((p_context != NULL), NRF_ERROR_INVALID_PARAM);

    err_code = app_timer_stop(*(p_context->p_timer_id));
    if (err_code == NRF_SUCCESS)
    {
        p_context->is_running = 0;
    }

    return err_code;
}
