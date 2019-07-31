/**
 * ZBOSS Zigbee 3.0
 * 
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * 
 * All rights reserved.
 * 
 * 
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 * 
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * PURPOSE: ZLL additions to ZCL Color control cluster
*/

#include "zcl/zb_zcl_color_control.h"

#if ! defined ZB_ZCL_COLOR_CONTROL_ZLL_ADDS_H
#define ZB_ZCL_COLOR_CONTROL_ZLL_ADDS_H

/**
 *  @defgroup ZB_ZLL_COLOR_CONTROL_ADDS ZLL additions to Color control cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_COLOR_CONTROL_ADDS
 *  ZLL additions to Color control cluster.
 *  @{
 */

/*! @name Extended Color Control cluster attributes
    @{
*/

/** @brief Color Control cluster information extended attribute identifiers
    @see ZLL spec 6.8.1.1
*/

/**
 * @brief Color control attribute list
 */
enum zb_zcl_color_control_attr_zll_e
{
  /*! @internal Enhanced Current Hue attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID = 0x4000,
  /*! @internal Enhanced Color Mode attribute,
   * see @ref zb_zcl_color_control_enhanced_color_mode_zll_e */
  ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ZLL_ID = 0x4001,
  /*! @internal Color Loop Active attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID = 0x4002,
  /*! @internal Color Loop Direction attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID = 0x4003,
  /*! @internal Color Loop Time attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID = 0x4004,
  /*! @internal Color Loop Start Enhanced Hue attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ZLL_ID = 0x4005,
  /*! @internal Color Loop Stored Enhanced Hue attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ZLL_ID = 0x4006,
  /*! @internal Color Capabilities attribute,
   * see @ref zb_zcl_color_control_color_capabilities_zll_e */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ZLL_ID = 0x400a,
  /*! @internal Color Temp Physical Min attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_ZLL_ID = 0x400b,
  /*! @internal Color Temp Physical Max attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_ZLL_ID = 0x400c,
};

/** @brief Enhanced Hue Max attribute default value */
#define ZB_ZCL_COLOR_CONTROL_ENHANCED_HUE_MAX_DEF_VALUE        0xffff

/** @brief Current Hue attribute red value */
#define ZB_ZCL_COLOR_CONTROL_ENHANCED_HUE_RED   (zb_uint16_t)(ZB_ZCL_COLOR_CONTROL_HUE_RED << 8)

/** @brief Current Hue attribute green value */
#define ZB_ZCL_COLOR_CONTROL_ENHANCED_GREEN     (zb_uint16_t)(ZB_ZCL_COLOR_CONTROL_HUE_GREEN << 8)

/** @brief Current Hue attribute blue value */
#define ZB_ZCL_COLOR_CONTROL_ENHANCED_BLUE      (zb_uint16_t)(ZB_ZCL_COLOR_CONTROL_HUE_BLUE << 8)


/** @brief Color Loop Time attribute default value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_LOOP_TIME_DEF_VALUE          0x0019

/** @brief Color Loop Start Enhanced Hue attribute default value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_LOOP_START_DEF_VALUE         0x2300

/** @brief Color Capabilities attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_CAPABILITIES_MAX_VALUE       0x001f

/** @brief Color Temp Physical Min attribute default value */
#define ZB_ZCL_COLOR_CONTROL_TEMP_PHYSICAL_MIN_DEF_VALUE        0

/** @brief Color Temp Physical Max attribute default value */
#define ZB_ZCL_COLOR_CONTROL_TEMP_PHYSICAL_MAX_DEF_VALUE        0xffff

/*! @brief Enhanced Color Mode attribute enumeration
    @see spec, subclause 6.8.1.1.2
*/
enum zb_zcl_color_control_enhanced_color_mode_zll_e
{
  /** CurrentHue and CurrentSaturation */
  ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION    = 0x00,
  /** CurrentX and CurrentY */
  ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_CURRENT_X_Y       = 0x01,
  /** ColorTemperature */
  ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_TEMPERATURE       = 0x02,
  /** EnhancedCurrentHue and EnhancedCurrentSaturation */
  ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX = 0x03,
};

/*! @brief Enhanced Color Capabilities attribute flags
    @see spec, subclause 6.8.1.1.8
*/
enum zb_zcl_color_control_color_capabilities_zll_e
{
  /** Hue/saturation supported*/
  ZB_ZCL_COLOR_CONTROL_CAPABILITIES_HUE_SATURATION  = 1 << 0,
  /** Enhanced hue supported*/
  ZB_ZCL_COLOR_CONTROL_CAPABILITIES_EX_HUE          = 1 << 1,
  /** Color loop supported*/
  ZB_ZCL_COLOR_CONTROL_CAPABILITIES_COLOR_LOOP      = 1 << 2,
  /** XY supported*/
  ZB_ZCL_COLOR_CONTROL_CAPABILITIES_X_Y             = 1 << 3,
  /** Color temperature supported*/
  ZB_ZCL_COLOR_CONTROL_CAPABILITIES_COLOR_TEMP      = 1 << 4,
};

/*! @} */ /* Color Control cluster attributes */

/*! @name Color Control cluster internals
    Internal structures for Color Control cluster
    @internal
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID(data_ptr)  \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID,                                        \
  ZB_ZCL_ATTR_TYPE_U16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ZLL_ID(data_ptr)   \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ZLL_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U8,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                 \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID(data_ptr)     \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U8,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID(data_ptr)  \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID,                                        \
  ZB_ZCL_ATTR_TYPE_U8,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID(data_ptr)       \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID,                                             \
  ZB_ZCL_ATTR_TYPE_U16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ZLL_ID(data_ptr)      \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ZLL_ID,                                            \
  ZB_ZCL_ATTR_TYPE_U16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                 \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ZLL_ID(data_ptr)     \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ZLL_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                 \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ZLL_ID(data_ptr)    \
{                                                                                               \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ZLL_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                 \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_ZLL_ID(data_ptr)   \
{                                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_ZLL_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                     \
  (zb_voidp_t) data_ptr                                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_ZLL_ID(data_ptr)   \
{                                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_ZLL_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                     \
  (zb_voidp_t) data_ptr                                                                             \
}

/*!
  Declare attribute list for Color Control cluster
  @param attr_list - attribure list name
  @param current_hue - pointer to variable to store current_hue attribute value
  @param current_saturation - pointer to variable to store current_saturation attribute value
  @param remaining_time - pointer to variable to store remaining_time attribute value
  @param current_X - pointer to variable to store current_X attribute value
  @param current_Y - pointer to variable to store current_Y attribute value
  @param color_temperature - pointer to variable to store color_temperature attribute value
  @param color_mode - pointer to variable to store color_mode attribute value
  @param number_primaries - pointer to variable to store number_primaries attribute value
  @param primary_1_X - pointer to variable to store primary_1_X attribute value
  @param primary_1_Y - pointer to variable to store primary_1_Y attribute value
  @param primary_1_intensity - pointer to variable to store primary_1_intensity attribute value
  @param primary_2_X - pointer to variable to store primary_2_X attribute value
  @param primary_2_Y - pointer to variable to store primary_2_Y attribute value
  @param primary_2_intensity - pointer to variable to store primary_2_intensity attribute value
  @param primary_3_X - pointer to variable to store primary_3_X attribute value
  @param primary_3_Y - pointer to variable to store primary_3_Y attribute value
  @param primary_3_intensity - pointer to variable to store primary_3_intensity attribute value
  @param primary_4_X - pointer to variable to store primary_4_X attribute value
  @param primary_4_Y - pointer to variable to store primary_4_Y attribute value
  @param primary_4_intensity - pointer to variable to store primary_4_intensity attribute value
  @param primary_5_X - pointer to variable to store primary_5_X attribute value
  @param primary_5_Y - pointer to variable to store primary_5_Y attribute value
  @param primary_5_intensity - pointer to variable to store primary_5_intensity attribute value
  @param primary_6_X - pointer to variable to store primary_6_X attribute value
  @param primary_6_Y - pointer to variable to store primary_6_Y attribute value
  @param primary_6_intensity - pointer to variable to store primary_6_intensity attribute value
  @param enhanced_current_hue - pointer to variable to store Enhanced Current Hue attribute value
  @param enhanced_color_mode - pointer to variable to store Enhanced Color Mode attribute value
  @param color_loop_active - pointer to variable to store Color Loop Active attribute value
  @param color_loop_direction - pointer to variable to store Color Loop Direction attribute value
  @param color_loop_time - pointer to variable to store Color Loop Time attribute value
  @param color_loop_start - pointer to variable to store Color Loop Start Enhanced Hue attribute value
  @param color_loop_stored - pointer to variable to store Color Loop Stored Enhanced Hue attribute value
  @param color_capabilities - pointer to variable to store Color Capabilities attribute value
  @param color_temp_physical_min - pointer to variable to store Color Temp Physical Min attribute value
  @param color_temp_physical_max - pointer to variable to store Color Temp Physical Max attribute value
*/
#define ZB_ZCL_DECLARE_COLOR_CONTROL_ATTRIB_LIST_ZLL(attr_list, current_hue,                    \
    current_saturation, remaining_time, current_X, current_Y, color_temperature,                \
    color_mode, number_primaries, primary_1_X, primary_1_Y, primary_1_intensity,                \
    primary_2_X, primary_2_Y, primary_2_intensity, primary_3_X, primary_3_Y,                    \
    primary_3_intensity, primary_4_X, primary_4_Y, primary_4_intensity, primary_5_X,            \
    primary_5_Y, primary_5_intensity, primary_6_X, primary_6_Y, primary_6_intensity,            \
    enhanced_current_hue, enhanced_color_mode, color_loop_active, color_loop_direction,         \
    color_loop_time, color_loop_start, color_loop_stored, color_capabilities,                   \
    color_temp_physical_min, color_temp_physical_max)                                           \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, (current_hue))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID, (current_saturation))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID, (remaining_time))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, (current_X))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID, (current_Y))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID, (color_temperature))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID, (color_mode))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID, (number_primaries))    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_X_ID, (primary_1_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_Y_ID, (primary_1_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_INTENSITY_ID, (primary_1_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_X_ID, (primary_2_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_Y_ID, (primary_2_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_INTENSITY_ID, (primary_2_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_X_ID, (primary_3_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_Y_ID, (primary_3_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_INTENSITY_ID, (primary_3_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_X_ID, (primary_4_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_Y_ID, (primary_4_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_INTENSITY_ID, (primary_4_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_X_ID, (primary_5_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_Y_ID, (primary_5_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_INTENSITY_ID, (primary_5_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_X_ID, (primary_6_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_Y_ID, (primary_6_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_INTENSITY_ID, (primary_6_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID, (enhanced_current_hue))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ZLL_ID, (enhanced_color_mode))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID, (color_loop_active))             \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID, (color_loop_direction))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID, (color_loop_time))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ZLL_ID, (color_loop_start))               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ZLL_ID, (color_loop_stored))             \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ZLL_ID, (color_capabilities))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_ZLL_ID, (color_temp_physical_min)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_ZLL_ID, (color_temp_physical_max)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Color Control cluster internals */

/*! @name Color Control cluster commands
    @{
*/

/************************** Color Control cluster command definitions ****************************/

/*! @brief Color control cluster extended command identifiers
    @see ZLL spec, subclause 6.8.1.3
*/
enum zb_zcl_color_control_cmd_zll_e
{
  /** Enhanced move to hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE             = 0x40,
  /** Enhanced move hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE                = 0x41,
  /** Enhanced step hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE                = 0x42,
  /** Enhanced move to hue and saturation command */
  ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION  = 0x43,
  /** Color loop set command */
  ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET                   = 0x44,
  /** Stop move step command */
  ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP                   = 0x47,
  /** Move color temperature command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE           = 0x4b,
  /** Step color temperature command */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE           = 0x4c,
};

/* command request structure */

/******************************* ENHANCED_MOVE_TO_HUE ******************************/

/*! @brief Structure representsation of Enhanced Move To Hue command payload
    @see ZLL spec, subclause 6.8.1.3.2 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_enhanced_move_to_hue_req_s
{
  /** Enhanced Hue */
  zb_uint16_t ehnanced_hue;
  /** Direction, see @ref zb_zcl_color_control_move_to_hue_direction_e */
  zb_uint8_t direction;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_enhanced_move_to_hue_req_t;

/*! @brief Send Enhanced Move to Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param ex_hue - Enhanced Hue value
    @param direction - Direction value, see @ref zb_zcl_color_control_move_to_hue_direction_e
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_ENHANCED_MOVE_TO_HUE_REQ(buffer, addr, dst_addr_mode,             \
  dst_ep, ep, prfl_id, def_resp, cb, ex_hue, direction, transition_time)                            \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE); \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (ex_hue));                                                      \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Enhanced Move to Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_enhanced_move_to_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_REQ(data_ptr, buffer, status)     \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_enhanced_move_to_hue_req_t))  \
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_color_control_enhanced_move_to_hue_req_t *src_ptr =                          \
          (zb_zcl_color_control_enhanced_move_to_hue_req_t*)ZB_BUF_BEGIN((buffer));     \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    ZB_HTOLE16(&((data_ptr)->ehnanced_hue), &(src_ptr->ehnanced_hue));                  \
    (data_ptr)->direction = src_ptr->direction;                                         \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));            \
  }                                                                                     \
}

/******************************* ENHANCED_MOVE_HUE ******************************/

/*! @brief Structure representsation of Enhanced Move Hue command payload
    @see ZLL spec, subclause 6.8.1.3.3 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_enhanced_move_hue_req_s
{
  /** Move mode, see @ref zb_zcl_color_control_move_direction_e */
  zb_uint8_t move_mode;
  /** Rate */
  zb_uint16_t rate;
} ZB_PACKED_STRUCT zb_zcl_color_control_enhanced_move_hue_req_t;

/*! @brief Send Enhanced Move Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param move_mode - Move mode, see @ref zb_zcl_color_control_move_direction_e
    @param rate - Rate
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_ENHANCED_MOVE_HUE_REQ(buffer, addr, dst_addr_mode,                \
  dst_ep, ep, prfl_id, def_resp, cb, move_mode, rate)                                               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));                                                        \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Enhanced Move Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_enhanced_move_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_HUE_REQ(data_ptr, buffer, status)    \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_enhanced_move_hue_req_t)) \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_enhanced_move_hue_req_t *src_ptr =                         \
             (zb_zcl_color_control_enhanced_move_hue_req_t*)ZB_BUF_BEGIN((buffer)); \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->move_mode = src_ptr->move_mode;                                     \
    ZB_HTOLE16(&((data_ptr)->rate), &(src_ptr->rate));                              \
  }                                                                                 \
}

/******************************* ENHANCED_STEP_HUE ******************************/

/*! @brief Structure representsation of Enhanced Step Hue command payload
    @see ZCL spec, subclause 6.8.1.3.4 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_enhanced_step_hue_req_s
{
  /** Step mode, see @ref zb_zcl_color_control_step_mode_e */
  zb_uint8_t step_mode;
  /** Step size */
  zb_uint16_t step_size;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_enhanced_step_hue_req_t;

/*! @brief Send Enhanced Step Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param step_mode - step mode value, see @ref zb_zcl_color_control_step_mode_e
    @param step_size - step size value
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_ENHANCED_STEP_HUE_REQ(buffer, addr, dst_addr_mode,                \
  dst_ep, ep, prfl_id, def_resp, cb, step_mode, step_size, transition_time)                         \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));                                                   \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Enhanced Step Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_enhanced_step_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_STEP_HUE_REQ(data_ptr, buffer, status)    \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_enhanced_step_hue_req_t)) \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_enhanced_step_hue_req_t *src_ptr =                         \
          (zb_zcl_color_control_enhanced_step_hue_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->step_mode = src_ptr->step_mode;                                     \
    ZB_HTOLE16(&((data_ptr)->step_size), &(src_ptr->step_size));                    \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));        \
  }                                                                                 \
}

/******************************* Enhanced Move to Hue and Saturation ******************************/

/*! @brief Structure representsation of Move To Hue and Saturation command payload
    @see ZLL spec, subclause 6.8.1.3.5 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_enhanced_move_to_hue_saturation_req_s
{
  /** Enhanced Hue */
  zb_uint16_t enhanced_hue;
  /** Saturation */
  zb_uint8_t saturation;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_enhanced_move_to_hue_saturation_req_t;

/*! @brief Send Enhanced Move to Hue and Saturation command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param enhanced_hue - enhanced hue
    @param saturation - saturation
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_ENHANCED_MOVE_TO_HUE_SATURATION_REQ(buffer, addr, dst_addr_mode,  \
  dst_ep, ep, prfl_id, def_resp, cb, enhanced_hue, saturation, transition_time)                     \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (enhanced_hue));                                                \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Enhanced Move To Hue and Saturation command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_enhanced_move_to_hue_saturation_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_SATURATION_REQ(data_ptr, buffer, status)      \
{                                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_enhanced_move_to_hue_saturation_req_t))   \
  {                                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                                          \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    zb_zcl_color_control_enhanced_move_to_hue_saturation_req_t *src_ptr =                           \
         (zb_zcl_color_control_enhanced_move_to_hue_saturation_req_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                         \
    ZB_HTOLE16(&((data_ptr)->enhanced_hue), &(src_ptr->enhanced_hue));                              \
    (data_ptr)->saturation = src_ptr->saturation;                                                   \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));                        \
  }                                                                                                 \
}

/******************************* Color Loop Set ******************************/

/*! @brief Structure representsation of Color Loop Set command payload
    @see ZLL spec, subclause 6.8.1.3.6 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_color_loop_set_req_s
{
  /** Update flags, see @ref zb_zcl_color_control_color_loop_update_e */
  zb_uint8_t update_flags;
  /** Action, see @ref zb_zcl_color_control_color_loop_action_e */
  zb_uint8_t action;
  /** Direction, see @ref zb_zcl_color_control_color_loop_direction_e */
  zb_uint8_t direction;
  /** Time */
  zb_uint16_t time;
  /** Start hue */
  zb_uint16_t start_hue;
} ZB_PACKED_STRUCT zb_zcl_color_control_color_loop_set_req_t;

/** @brief Update flag for Color Loop Set command
 *  See specs 6.8.1.3.6.1 */
enum zb_zcl_color_control_color_loop_update_e
{
  /** Update action */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_ACTION       = 1 << 0,
  /** Update direction */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_DIRECTION    = 1 << 1,
  /** Update time */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_TIME         = 1 << 2,
  /** Update start hue */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_START_HUE    = 1 << 3,
};

/** @brief Action enumerate for Color Loop Set command
 *  See specs 6.8.1.3.6.2 */
enum zb_zcl_color_control_color_loop_action_e
{
  /** De-activate the color loop */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_DEACTIVATE   = 0,
  /** Activate the color loop from the value in the Color Loop Start Enhanced Hue field*/
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_START_HUE    = 1,
  /** Activate the color loop from the value of the Enhanced Current Hue attribute*/
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_CURRENT_HUE  = 2,
};

/** @brief Direction enumerate for Color Loop Set command
 *  See specs 6.8.1.3.6.3 */
enum zb_zcl_color_control_color_loop_direction_e
{
  /** Decrement the hue in the color loop */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_DIRECTION_DECREMENT     = 0,
  /** Increment the hue in the color loop */
  ZB_ZCL_CMD_COLOR_CONTROL_LOOP_DIRECTION_INCREMENT     = 1,
};

/*! @internal
 *  @brief Structure for loop of Color Loop Set command */
typedef struct zb_zcl_color_control_color_loop_set_s
{
  /** Command header */
  zb_zcl_parsed_hdr_t cmd_info;

  /** Last time - beacon interval */
  zb_time_t last_time;

} zb_zcl_color_control_color_loop_set_t;

#define ZB_ZCL_COLOR_CONTROL_FILL_COLOR_LOOP_SET_DATA(buf, cmd_info_)           \
{                                                                               \
  zb_zcl_color_control_color_loop_set_t *loop_data =                            \
          ZB_GET_BUF_PARAM((buf), zb_zcl_color_control_color_loop_set_t);       \
  ZB_MEMMOVE(&(loop_data->cmd_info), (cmd_info_), sizeof(zb_zcl_parsed_hdr_t)); \
  loop_data->last_time = ZB_TIMER_GET();                                        \
}

/*! @brief Send Color Loop Set command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param update_flags - update flags, see @ref zb_zcl_color_control_color_loop_update_e
    @param action - action, see @ref zb_zcl_color_control_color_loop_action_e
    @param direction - direction, see @ref zb_zcl_color_control_color_loop_direction_e
    @param time - time
    @param start_hue - start hue
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_COLOR_LOOP_SET_REQ(buffer, addr, dst_addr_mode,                   \
  dst_ep, ep, prfl_id, def_resp, cb, update_flags, action, direction, time, start_hue)              \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET);  \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (update_flags));                                                     \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (action));                                                           \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (time));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (start_hue));                                                   \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Color Loop Set command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_color_loop_set_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_COLOR_LOOP_SET_REQ(data_ptr, buffer, status)           \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_color_loop_set_req_t))        \
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_color_control_color_loop_set_req_t *src_ptr =                                \
         (zb_zcl_color_control_color_loop_set_req_t*)ZB_BUF_BEGIN((buffer));            \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_color_loop_set_req_t));  \
    ZB_HTOLE16(&((data_ptr)->time), &(src_ptr->time));                                  \
    ZB_HTOLE16(&((data_ptr)->start_hue), &(src_ptr->start_hue));                        \
  }                                                                                     \
}

/******************************* Stop move step ******************************/

/*! @brief Stop move step command, see spec 6.8.1.3.7
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_STOP_MOVE_STEP_REQ(buffer, addr, dst_addr_mode,                   \
  dst_ep, ep, prfl_id, def_resp, cb)                                                                \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP);  \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/******************************* Move color temperature command ******************************/

/*! @brief Structure representsation of Move color temperature command payload
    @see ZLL spec, subclause 6.8.1.3.8 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_color_temp_req_s
{
  /** Move mode, see @ref zb_zcl_color_control_move_direction_e */
  zb_uint8_t move_mode;
  /** Rate */
  zb_uint16_t rate;
  /** Color temperature minimum */
  zb_uint16_t color_temp_min;
  /** Color temperature maximum */
  zb_uint16_t color_temp_max;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_color_temp_req_t;

/*! @brief Send Move color temperature command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param move_mode - move mode, see @ref zb_zcl_color_control_move_direction_e
    @param rate - rate
    @param color_temp_min - color temperature minimum
    @param color_temp_max - color temperature maximum
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_COLOR_TEMP_REQ(buffer, addr, dst_addr_mode,                  \
  dst_ep, ep, prfl_id, def_resp, cb, move_mode, rate, color_temp_min, color_temp_max)               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE);  \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));                                              \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));                                              \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move color temperature command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_color_temp_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_TEMP_REQ(data_ptr, buffer, status)      \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_color_temp_req_t))   \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_move_color_temp_req_t *src_ptr =                           \
         (zb_zcl_color_control_move_color_temp_req_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->move_mode = src_ptr->move_mode;                                     \
    ZB_HTOLE16(&((data_ptr)->rate), &(src_ptr->rate));                              \
    ZB_HTOLE16(&((data_ptr)->color_temp_min), &(src_ptr->color_temp_min));          \
    ZB_HTOLE16(&((data_ptr)->color_temp_max), &(src_ptr->color_temp_max));          \
  }                                                                                 \
}

/******************************* Step color temperature command ******************************/

/*! @brief Structure representsation of Step color temperature command command payload
    @see ZLL spec, subclause 8.8.1.3.9 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_step_color_temp_req_s
{
  /** Step mode, see @ref zb_zcl_color_control_step_mode_e */
  zb_uint8_t step_mode;
  /** Step size */
  zb_uint16_t step_size;
  /** Transition time field */
  zb_uint16_t transition_time;
  /** Color temperature minimum */
  zb_uint16_t color_temp_min;
  /** Color temperature maximum */
  zb_uint16_t color_temp_max;
} ZB_PACKED_STRUCT zb_zcl_color_control_step_color_temp_req_t;

/*! @brief Step color temperature command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param step_mode - move mode, see @ref zb_zcl_color_control_step_mode_e
    @param step_size - step size
    @param transition_time - Transition Time value
    @param color_temp_min - color temperature minimum
    @param color_temp_max - color temperature maximum
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_STEP_COLOR_TEMP_REQ(buffer, addr, dst_addr_mode,                  \
  dst_ep, ep, prfl_id, def_resp, cb, step_mode, step_size, transition_time, color_temp_min, color_temp_max) \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE);  \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));                                                   \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));                                              \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));                                              \
  ZB_ZCL_FINISH_PACKET((buffer), ptr);                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move color temperature command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_step_color_temp_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_TEMP_REQ(data_ptr, buffer, status)      \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_step_color_temp_req_t))   \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_step_color_temp_req_t *src_ptr =                           \
         (zb_zcl_color_control_step_color_temp_req_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->step_mode = src_ptr->step_mode;                                     \
    ZB_HTOLE16(&((data_ptr)->step_size), &(src_ptr->step_size));                    \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));        \
    ZB_HTOLE16(&((data_ptr)->color_temp_min), &(src_ptr->color_temp_min));          \
    ZB_HTOLE16(&((data_ptr)->color_temp_max), &(src_ptr->color_temp_max));          \
  }                                                                                 \
}

/**
 * @}
 */

/**
 * @}
 */

#endif /* ! defined ZB_ZCL_COLOR_CONTROL_ZLL_ADDS_H */
