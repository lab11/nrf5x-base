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

#ifndef SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COMMON_ADDONS_H_
#define SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COMMON_ADDONS_H_

#include "sdk_config.h"
#include "zboss_api.h"

/*! \addtogroup zb_zcl_common_addons */
/*! @{ */

/**@brief Macro equivalent to @ref ZB_ZCL_FINISH_PACKET, but command is sent without APS ACK.
 * @note This macro may be used when constructing other ZCL macros like in Zboss library replacing
 * @ref ZB_ZCL_FINISH_PACKET with @ref ZB_ZCL_FINISH_PACKET_NO_APS_ACK.
 */
#define ZB_ZCL_FINISH_PACKET_NO_APS_ACK(zbbuf, ptr) (void) zb_zcl_finish_and_send_packet_no_ack((zbbuf), (ptr),

/** @brief Send ZCL command without APS acknowledgment request. */
void zb_zcl_finish_and_send_packet_no_ack(zb_buf_t * p_buf, zb_uint8_t * p_cmd_ptr,
                                   zb_addr_u * p_dst_addr, zb_uint8_t dst_addr_mode,
                                   zb_uint8_t dst_ep, zb_uint8_t src_ep,
                                   zb_uint16_t profile_id, zb_uint16_t cluster_id,
                                   zb_callback_t p_cb);

/** @} */

#endif /* SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COMMON_ADDONS_H_ */
