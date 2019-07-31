/**
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
#include "app_timer.h"
#include "app_scheduler.h"

#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "background_dfu_transport.h"
#include "nrf_dfu_settings.h"
#include "nrf_dfu_req_handler.h"
#include "nrf_dfu_utils.h"
#include "background_dfu_block.h"

#include "zboss_api.h"
#include "zb_error_handler.h"

#include "zigbee_dfu_transport.h"

#define SCHED_QUEUE_SIZE      32                                                              /**< Maximum number of events in the scheduler queue. */
#define SCHED_EVENT_DATA_SIZE MAX(sizeof(nrf_dfu_request_t), APP_TIMER_SCHED_EVENT_DATA_SIZE) /**< Maximum app_scheduler event size. */

static background_dfu_context_t m_dfu_ctx;                                                    /**< Background DFU context. */
static zigbee_ota_dfu_context_t m_zb_ota_ctx;                                                 /**< Zigbee transport OTA-DFU context. */

/** @brief Function to abort the firmware upgrade process
 */
void zb_abort_dfu(void)
{
    /* Reset the counters */
    m_zb_ota_ctx.ota_header_fill_level = 0;
    m_zb_ota_ctx.init_cmd.start = 0;
    m_zb_ota_ctx.init_cmd.length = 0;
    m_zb_ota_ctx.firmware.start = 0;
    m_zb_ota_ctx.firmware.length = 0;
    m_zb_ota_ctx.p_trigger = NULL;

    /* Reset the background DFU state model */
    background_dfu_reset_state(&m_dfu_ctx);

    /* Halt the OTA process */
    zcl_ota_abort(m_zb_ota_ctx.endpoint, 0);
}

/** @brief Function to set the file offset to download
 *
 *  @param ep Endpoint on which the OTA Client is dispatched
 *  @param fo OTA file offset in bytes
 */
static void set_file_offset(zb_uint8_t ep, zb_uint32_t fo)
{
    /* We set the file offset by directly modifying the local attribute */
    zb_zcl_attr_t * p_attr_desc = zb_zcl_get_attr_desc_a(ep,
                                                         ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                                                         ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                                         ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
    ZB_ASSERT(p_attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(p_attr_desc, fo);
}

/** @brief Function to suspend the downloading process
 */
static void suspend_download(zb_uint8_t param)
{
    m_zb_ota_ctx.resume_buffer = param;
    m_zb_ota_ctx.is_download_suspended = ZB_TRUE;
}

/** @brief Function to resume the downloading process
 */
static void resume_download(void)
{
    if (m_zb_ota_ctx.is_download_suspended)
    {
        m_zb_ota_ctx.is_download_suspended = ZB_FALSE;
        zb_zcl_ota_upgrade_resume_client(m_zb_ota_ctx.resume_buffer, ZB_ZCL_OTA_UPGRADE_STATUS_OK);
    }
    else
    {
        NRF_LOG_WARNING("Wanted to resume download but it was already resumed");
    }
}

/** @brief Code to validate and process the DFU trigger == start the DFU process
 */
static void activate_trigger(zb_uint8_t param)
{
    UNUSED_VARIABLE(param);
    /* p_trigger points to the compile-time known place in RAM - assignment is safe */
    zb_zcl_ota_upgrade_sub_element_t * p_trigger = (zb_zcl_ota_upgrade_sub_element_t*)(m_zb_ota_ctx.ota_header + sizeof(zb_zcl_ota_upgrade_file_header_t));

    /* Sanity check for the received trigger - all the further checks are inside the functions below */
    if (p_trigger->tag_id != SUBELEMENT_TRIGGER_TYPE)
    {
        /* The image was combined in an improper way - discarding everything */
        NRF_LOG_ERROR("Trigger type wrong - image combined in an improper way");
        zb_abort_dfu();
    }

    if (background_dfu_validate_trigger(&m_dfu_ctx, p_trigger->value, p_trigger->length))
    {
        if (!background_dfu_process_trigger(&m_dfu_ctx, p_trigger->value, p_trigger->length))
        {
            NRF_LOG_ERROR("Could not process trigger");
            zb_abort_dfu();
        }
    }
}

/** @brief Code to process the incoming Zigbee OTA frame
 *
 *  @param ota   Pointer to the zb_zcl_ota_upgrade_value_param_t structure,
 *               passed from the handler
 *  @param param buffer number
 */
zb_uint8_t zb_process_chunk(const zb_zcl_ota_upgrade_value_param_t * ota, zb_uint8_t param)
{
    zb_uint8_t ret = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
    /* Process image block */
    if (ota->upgrade.receive.file_offset < TOTAL_HEADER_LEN)
    {
        /* Branch where we copy the OTA Header and Trigger */
        /* Figure out how much to copy */
        int bytes_left = TOTAL_HEADER_LEN - m_zb_ota_ctx.ota_header_fill_level;
        int bytes_to_copy = MIN(bytes_left, ota->upgrade.receive.data_length);

        memcpy(m_zb_ota_ctx.ota_header + m_zb_ota_ctx.ota_header_fill_level, ota->upgrade.receive.block_data, bytes_to_copy);
        m_zb_ota_ctx.ota_header_fill_level += bytes_to_copy;

        if (m_zb_ota_ctx.ota_header_fill_level == TOTAL_HEADER_LEN)
        {
            /* All the header is copied */
            m_zb_ota_ctx.p_trigger = (zb_zcl_ota_upgrade_sub_element_t *)(m_zb_ota_ctx.ota_header + sizeof(zb_zcl_ota_upgrade_file_header_t));
            suspend_download(param);
            ret = ZB_ZCL_OTA_UPGRADE_STATUS_BUSY;
            UNUSED_RETURN_VALUE(ZB_SCHEDULE_CALLBACK(activate_trigger, 0));
        }
    }
    else
    {
        /* Pass the payload straight to the DFU, since it is aligned and equal in size */
        background_dfu_block_t block;

        memcpy(m_zb_ota_ctx.block, ota->upgrade.receive.block_data, ota->upgrade.receive.data_length);

        block.size = BACKGROUND_DFU_DEFAULT_BLOCK_SIZE;
        block.p_payload = m_zb_ota_ctx.block;
        block.number    = m_dfu_ctx.block_num;

        UNUSED_RETURN_VALUE(background_dfu_process_block(&m_dfu_ctx, &block));
        suspend_download(param);
        ret = ZB_ZCL_OTA_UPGRADE_STATUS_BUSY;
    }

    return ret;
}

/** @brief Placeholder for the random number generator, unused
 */
uint32_t background_dfu_random(void)
{
    // Intentionally empty: multicast DFU not implemented.
    ASSERT(0);
    return 0;
}

/** @brief Error Handler, aborts the DFU
 */
void background_dfu_handle_error(void)
{
    NRF_LOG_ERROR("Error handler triggered");
    zb_abort_dfu();
}

/** @brief Function to send a request, sets the file offset and resumes OTA process
 *
 *  @param p_dfu_ctx Pointer to the background DFU context
 */
void background_dfu_transport_send_request(background_dfu_context_t * p_dfu_ctx)
{
    if (p_dfu_ctx->dfu_state == BACKGROUND_DFU_DOWNLOAD_INIT_CMD || p_dfu_ctx->dfu_state == BACKGROUND_DFU_DOWNLOAD_FIRMWARE)
    {
        int offset = (p_dfu_ctx->dfu_state == BACKGROUND_DFU_DOWNLOAD_INIT_CMD) ? m_zb_ota_ctx.init_cmd.start : m_zb_ota_ctx.firmware.start;
        set_file_offset(m_zb_ota_ctx.endpoint, p_dfu_ctx->block_num * BACKGROUND_DFU_DEFAULT_BLOCK_SIZE + offset);
        resume_download();
    }
}

/** @brief Function notifies certain events in DFU process.
 */
static void dfu_observer(nrf_dfu_evt_type_t evt_type)
{
    switch (evt_type)
    {
        case NRF_DFU_EVT_DFU_COMPLETED:
            NRF_LOG_INFO("Observer: Reset after DFU");
            /* Resume Download to send/receive End Upgrade Request/Response */
            resume_download();
            break;

        default:
            NRF_LOG_INFO("Observer: unhandled event: %d", evt_type);
            break;
    }
}

void background_dfu_transport_state_update(background_dfu_context_t * p_dfu_ctx)
{
    switch (p_dfu_ctx->dfu_state)
    {
        case BACKGROUND_DFU_DOWNLOAD_TRIG:
            NRF_LOG_INFO("Downloading trigger");
            break;

        case BACKGROUND_DFU_DOWNLOAD_INIT_CMD:
            m_zb_ota_ctx.init_cmd.start = TOTAL_HEADER_LEN + sizeof(zb_zcl_ota_upgrade_sub_element_t) - 1;
            m_zb_ota_ctx.init_cmd.length = p_dfu_ctx->init_cmd_size;
            NRF_LOG_INFO("Downloading init packet");
            break;

        case BACKGROUND_DFU_DOWNLOAD_FIRMWARE:
            m_zb_ota_ctx.firmware.start = TOTAL_HEADER_LEN + sizeof(zb_zcl_ota_upgrade_sub_element_t) - 1 + p_dfu_ctx->init_cmd_size + sizeof(zb_zcl_ota_upgrade_sub_element_t) - 1;
            m_zb_ota_ctx.firmware.length = p_dfu_ctx->firmware_size;
            NRF_LOG_INFO("Downloading firmware image");
            break;

        case BACKGROUND_DFU_WAIT_FOR_RESET:
            NRF_LOG_INFO("Waiting for reset");
            break;

        case BACKGROUND_DFU_IDLE:
            // Check if there was an ongoing DFU process.
            if (m_zb_ota_ctx.p_trigger != NULL)
            {
                NRF_LOG_INFO("Downloading firmware cancelled by nRF DFU");
                zb_abort_dfu();
            }
            break;

        default:
            NRF_LOG_WARNING("Unhandled state in background_dfu_transport_state_update (s: %s).",
                    (uint32_t)background_dfu_state_to_string(p_dfu_ctx->dfu_state));
    }
}

void background_dfu_transport_block_request_send(background_dfu_context_t        * p_dfu_ctx,
                                                 background_dfu_request_bitmap_t * p_req_bmp)
{
    NRF_LOG_INFO("Sending block request! - actually not supported, so not really sending");
    ASSERT(0);
}

/** @brief Function to initialize the transport.
 *
 *  @param ep Endpoint on which OTA is dispatched
 */
void zb_dfu_init(zb_uint8_t ep)
{
#ifdef BLE_STACK_SUPPORT_REQD
    UNUSED_RETURN_VALUE(nrf_dfu_settings_init(true));
#else
    UNUSED_RETURN_VALUE(nrf_dfu_settings_init(false));
#endif
    UNUSED_RETURN_VALUE(nrf_dfu_req_handler_init(dfu_observer));

    background_dfu_state_init(&m_dfu_ctx);

    m_zb_ota_ctx.endpoint  = ep;
    m_zb_ota_ctx.p_trigger = NULL;

    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}
