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
#ifndef ZIGBEE_DFU_TRANSPORT_H
#define ZIGBEE_DFU_TRANSPORT_H

#include "zboss_api.h"
#include "background_dfu_transport.h"

#define TOTAL_HEADER_LEN        sizeof(zb_zcl_ota_upgrade_file_header_t) + sizeof(zb_zcl_ota_upgrade_sub_element_t) - 1 + sizeof(background_dfu_trigger_t)
#define SUBELEMENT_TRIGGER_TYPE 0xCDEF

typedef struct zigbee_subelement_desc
{
    int start;
    int length;
} zigbee_subelement_desc_t;

typedef struct zigbee_ota_dfu_context
{
    zb_uint8_t                         resume_buffer;
    zb_uint8_t                         endpoint;
    zb_zcl_ota_upgrade_sub_element_t * p_trigger;
    zigbee_subelement_desc_t           init_cmd;
    zigbee_subelement_desc_t           firmware;
    zb_bool_t                          is_download_suspended;
    zb_uint8_t                         ota_header[TOTAL_HEADER_LEN];
    uint32_t                           ota_header_fill_level;
    uint8_t                            block[BACKGROUND_DFU_DEFAULT_BLOCK_SIZE];
} zigbee_ota_dfu_context_t;

/** @brief Initialize DFU client.
 *
 *  @param[in] ep Endpoint on which OTA is dispatched.
 */
void zb_dfu_init(zb_uint8_t ep);

/** @brief Abort the ongoing DFU process.
 */
void zb_abort_dfu(void);

/** @brief Process the OTA incoming payload.
 *
 *  @param[in] ota OTA frame details passed from callback handler
 *  @param[in] param Zigbee buffer passed from callback handler.
 *
 *  @return ZB_ZCL_OTA_UPGRADE_STATUS_BUSY if OTA has to be suspended, ZB_ZCL_OTA_UPGRADE_STATUS_OK otherwise
 */
zb_uint8_t zb_process_chunk(const zb_zcl_ota_upgrade_value_param_t* ota, zb_uint8_t param);

#endif /* ZIGBEE_DFU_TRANSPORT_H */
