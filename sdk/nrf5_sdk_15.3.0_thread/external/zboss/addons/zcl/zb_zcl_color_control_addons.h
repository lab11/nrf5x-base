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

#ifndef SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COLOR_CONTROL_ADDONS_H_
#define SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COLOR_CONTROL_ADDONS_H_

#include "sdk_config.h"
#include "zboss_api.h"

/*! \addtogroup zb_zcl_color_control_addons */
/*! @{ */

/**@brief Color information set for Color cluster attributes according to ZCL Spec 5.2.2.2.1. */
typedef struct
{
    zb_uint8_t     current_hue;
    zb_uint8_t     current_saturation;
    zb_uint16_t    remaining_time;
    zb_uint16_t    current_X;
    zb_uint16_t    current_Y;
    zb_uint16_t    color_temperature;
    zb_uint8_t     color_mode;
    zb_uint8_t     options;
    zb_uint16_t    enhanced_current_hue;
    zb_uint8_t     enhanced_color_mode;
    zb_uint8_t     color_loop_active;
    zb_uint8_t     color_loop_direction;
    zb_uint16_t    color_loop_time;
    zb_uint16_t    color_loop_start_enhanced_hue;
    zb_uint16_t    color_loop_stored_enhanced_hue;
    zb_uint16_t    color_capabilities;
    zb_uint16_t    color_temp_physical_min_mireds;
    zb_uint16_t    color_temp_physical_max_mireds;
    zb_uint16_t    couple_color_temp_to_level_min_mireds;
    zb_uint16_t    start_up_color_temp_mireds;
} zb_zcl_color_ctrl_attrs_set_color_inf_t;

/**@brief Defined primaries information attribute set for Color cluster attributes according to ZCL Spec 5.2.2.2.2. */
typedef struct
{
    zb_uint8_t      number_primaries;
    zb_uint16_t     primary_1_X;
    zb_uint16_t     primary_1_Y;
    zb_uint8_t      primary_1_intensity;
    zb_uint16_t     primary_2_X;
    zb_uint16_t     primary_2_Y;
    zb_uint8_t      primary_2_intensity;
    zb_uint16_t     primary_3_X;
    zb_uint16_t     primary_3_Y;
    zb_uint8_t      primary_3_intensity;
} zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_t;

/**@brief Color cluster attributes according to ZCL Spec 5.2.2.2.3. */
typedef struct
{
    zb_uint16_t     primary_4_X;
    zb_uint16_t     primary_4_Y;
    zb_uint8_t      primary_4_intensity;
    zb_uint16_t     primary_5_X;
    zb_uint16_t     primary_5_Y;
    zb_uint8_t      primary_5_intensity;
    zb_uint16_t     primary_6_X;
    zb_uint16_t     primary_6_Y;
    zb_uint8_t      primary_6_intensity;
} zb_zcl_color_ctrl_attrs_set_add_prim_inf_t;

/**@brief Color cluster attributes according to ZCL Spec 5.2.2.2. */
typedef struct
{
    zb_zcl_color_ctrl_attrs_set_color_inf_t             set_color_info;
    zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_t set_defined_primaries_info;
    zb_zcl_color_ctrl_attrs_set_add_prim_inf_t          set_additional_defined_primaries_info;
} zb_zcl_color_control_attrs_t;

/**@brief Macro equivalent to @ref ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ, but command is sent without APS ACK. */
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ_NO_APS_ACK(buffer, addr, dst_addr_mode,\
  dst_ep, ep, prfl_id, def_resp, cb, hue, saturation, transition_time)                              \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                 \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                              \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));                                                              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET_NO_APS_ACK((buffer), ptr)                                                    \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @} */

#endif /* SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_COLOR_CONTROL_ADDONS_H_ */
