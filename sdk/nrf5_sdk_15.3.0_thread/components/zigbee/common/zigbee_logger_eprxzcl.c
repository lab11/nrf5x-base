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

#include <stdio.h>
#include <inttypes.h>

#include "sdk_config.h"
#include "nrf_log_instance.h"
#include "nrf_log.h"

#include "zigbee_helpers.h"
#include "zigbee_logger_eprxzcl.h"

#if NRF_LOG_ENABLED
/** @brief Name of the submodule used for logger messaging. */
#define NRF_LOG_SUBMODULE_NAME  eprxzcl

NRF_LOG_INSTANCE_REGISTER(ZIGBEE_NRF_LOG_MODULE_NAME, NRF_LOG_SUBMODULE_NAME,
                          ZIGBEE_CLI_CONFIG_INFO_COLOR,
                          ZIGBEE_CLI_CONFIG_DEBUG_COLOR,
                          ZIGBEE_CLI_CONFIG_LOG_INIT_FILTER_LEVEL,
                          ZIGBEE_CLI_CONFIG_LOG_ENABLED ?
                                  ZIGBEE_CLI_CONFIG_LOG_LEVEL : NRF_LOG_SEVERITY_NONE);

// This structure keeps reference to the logger instance used by this module.
typedef struct {
    NRF_LOG_INSTANCE_PTR_DECLARE(p_log)
} log_ctx_t;

// Logger instance used by this module.
static log_ctx_t m_log = {
    NRF_LOG_INSTANCE_PTR_INIT(p_log, ZIGBEE_NRF_LOG_MODULE_NAME, NRF_LOG_SUBMODULE_NAME)
};
#endif // NRF_LOG_ENABLED

/**@brief Number of logs that can be simultaneously buffered before processing by a backend */
#define PRV_LOG_CIRC_BUFFER_MAX_LOGS            8U
/**@brief Maximum length of log line.
 */
#define PRV_LOG_CIRC_BUFFER_MAX_MESSAGE_LENGTH  512U

typedef struct {
    size_t  idx;
    char buffers[PRV_LOG_CIRC_BUFFER_MAX_LOGS][PRV_LOG_CIRC_BUFFER_MAX_MESSAGE_LENGTH];
} prv_log_circ_buffer_t;

/**@brief ZCL received frames log counter.
 * @details Successive calls to @ref zigbee_logger_eprxzcl_ep_handler increment this value.
 * @ref zigbee_logger_eprxzcl_ep_handler puts this value in every log line
 */
static uint32_t m_log_counter;

/**@brief Circular buffer of logs produced by @ref zigbee_logger_eprxzcl_ep_handler
 * @details Successive calls to @ref prv_log_circ_buffer_get_next_buffer return successive buffers to be
 * filled in and that may be put into NRF_LOG_INFO with deferred processing.
 */
static prv_log_circ_buffer_t m_log_circ_buffer;

/**@brief Function returning next log buffer to be filled in.
 *
 * Due to problems with reliable logging of strings using @ref NRF_LOG_PUSH in current version of nRF5 SDK,
 * this function returns a buffer, which may be filled up with log message and passed to NRF_LOG_INFO
 * User must be aware that actually no 'free' operation happens anywhere. Buffer may get overwritten if
 * the backend processes logs slower than they are produced. Keep PRV_LOG_CIRC_BUFFER_MAX_LOGS big enough.
 *
 * @return Pointer to the log message buffer capable of storing LOG_CIRC_BUFFER_MAX_MESSAGE_LENGTH bytes
 *         (including null terminator).
 */
static char * prv_log_circ_buffer_get_next_buffer(void)
{
    char * result;
    size_t new_idx;

    new_idx = m_log_circ_buffer.idx;
    result  = m_log_circ_buffer.buffers[new_idx];

    /* Increment modulo now */
    new_idx++;
    if (new_idx >= PRV_LOG_CIRC_BUFFER_MAX_LOGS)
    {
        new_idx = 0U;
    }
    m_log_circ_buffer.idx = new_idx;

    memset(result, 0, PRV_LOG_CIRC_BUFFER_MAX_MESSAGE_LENGTH);

    return result;
}

/**@brief Macro used within zigbee_logger_eprxzcl_ep_handler to perform guarded p_log_message_curr advance
 * @note To be used within @ref zigbee_logger_eprxzcl_ep_handler only
 */
#define PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(s)             \
do {                                                    \
    if ((s) < 0)                                        \
    {                                                   \
        NRF_LOG_INST_ERROR(m_log.p_log, "Received ZCL command but encoding error occurred during log producing"); \
        return ZB_FALSE;                                \
    }                                                   \
    p_log_message_curr += (s);                          \
    if (p_log_message_curr >= p_log_message_end)        \
    {                                                   \
        NRF_LOG_INST_ERROR(m_log.p_log, "Received ZCL command but produced log is too long"); \
        return ZB_FALSE;                                \
    }                                                   \
} while(0)

zb_uint8_t zigbee_logger_eprxzcl_ep_handler(zb_uint8_t param)
{
    // Check compile-time and run-time instance-based log filters.
    if (NRF_LOG_ENABLED && (NRF_LOG_LEVEL >= NRF_LOG_SEVERITY_INFO) &&
        (NRF_LOG_DEFAULT_LEVEL >= NRF_LOG_SEVERITY_INFO) &&
        (NRF_LOG_INST_FILTER(m_log.p_log) >= NRF_LOG_SEVERITY_INFO))
    {
        int                   status;
        char                * p_log_message      = prv_log_circ_buffer_get_next_buffer();
        char                * p_log_message_curr = p_log_message;
        char                * p_log_message_end  = p_log_message_curr + PRV_LOG_CIRC_BUFFER_MAX_MESSAGE_LENGTH - 1U;
        zb_buf_t            * p_zcl_cmd_buf      = (zb_buf_t *)ZB_BUF_FROM_REF(param);
        zb_zcl_parsed_hdr_t * p_cmd_info         = ZB_GET_BUF_PARAM(p_zcl_cmd_buf, zb_zcl_parsed_hdr_t);
        size_t                payload_length     = ZB_BUF_LEN(p_zcl_cmd_buf);
        const zb_uint8_t    * p_payload          = ZB_BUF_BEGIN(p_zcl_cmd_buf);
        uint32_t              log_number         = m_log_counter++;

        status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                          "Received ZCL command (%"PRIu32"): src_addr=", log_number);
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        switch (ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).source.addr_type)
        {
            case ZB_ZCL_ADDR_TYPE_SHORT:
                status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                  "0x%04x(short)", ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).source.u.short_addr);
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                break;

            case ZB_ZCL_ADDR_TYPE_IEEE_GPD:
                status = ieee_addr_to_str(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).source.u.ieee_addr);
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

                status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                  "(ieee_gpd)");
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                break;

            case ZB_ZCL_ADDR_TYPE_SRC_ID_GPD:
                status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                  "0x%x(src_id_gpd)", ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).source.u.src_id);
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                break;

            case ZB_ZCL_ADDR_TYPE_IEEE:
                status = ieee_addr_to_str(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).source.u.ieee_addr);
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                  "(ieee)");
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                break;

            default:
                status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                                  "0(unknown)");
                PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);
                break;
        }

        status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                          " src_ep=%u dst_ep=%u cluster_id=0x%04x profile_id=0x%04x",
                          ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).src_endpoint,
                          ZB_ZCL_PARSED_HDR_SHORT_DATA(p_cmd_info).dst_endpoint,
                          p_cmd_info->cluster_id,
                          p_cmd_info->profile_id);
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                          " rssi=%u cmd_dir=%u common_cmd=%u cmd_id=0x%02x cmd_seq=%u disable_def_resp=%u",
                          p_cmd_info->rssi,
                          p_cmd_info->cmd_direction,
                          p_cmd_info->is_common_command,
                          p_cmd_info->cmd_id,
                          p_cmd_info->seq_number,
                          p_cmd_info->disable_default_response);
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        if (p_cmd_info->is_manuf_specific)
        {
            status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                              " manuf_code=0x%04x", p_cmd_info->manuf_specific);
        }
        else
        {
            status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                              " manuf_code=void");
        }
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                          " payload=[");
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        status = to_hex_str(p_log_message_curr, p_log_message_end - p_log_message_curr,
                            p_payload, payload_length, false);
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        // Put again m_log_counter to be able to simple check log consistency
        status = snprintf(p_log_message_curr, p_log_message_end - p_log_message_curr,
                          "] (%"PRIu32")", log_number);
        PRV_ADVANCE_CURR_LOG_MESSAGE_PTR(status);

        *p_log_message_curr = '\0';

        NRF_LOG_INST_INFO(m_log.p_log, "%s", p_log_message);
    }

    return ZB_FALSE;
}
