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
 * PURPOSE: ZLL additions to ZCL On/Off cluster
*/

#if ! defined ZB_ZCL_ON_OFF_ZLL_ADDS_H
#define ZB_ZCL_ON_OFF_ZLL_ADDS_H

#include "zcl/zb_zcl_on_off.h"

/**
 *  @defgroup ZB_ZLL_ON_OFF_ADDS ZLL additions to On/Off cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_ON_OFF_ADDS
 *  ZLL additions to On/Off cluster.
 *  @{
 *    @details
 *    Extended On/Off cluster for Light Link (see spec 6.6.1).
 *
 *    @par Example
 *    Declare cluster:
 *    @snippet tp_cst_tc_03_zr.c ON_OFF_DECLARE
 *    Toggle command sending:
 *    @snippet tp_cst_tc_02_zed.c SEND_TOGGLE
 *    On with recall global scene command sending:
 *    @snippet tp_cst_tc_02_zed.c ON_WITH_RECALL_GLOBAL_SCENE
 *    @par
 *
 *    Other commands can be sent in the same way using appropriate macros.
 *
 *    For more information tp_cst_02, tp_cst_03 tests
 */

/**
 *  @name Attributes added to On/Off cluster.
 *  @{
 */

/*! @brief On/Off cluster extended attributes identifiers
    @see ZLL specification, subclause 6.6.1.2
*/
enum zb_zcl_on_off_attr_zll_e
{
  ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL_ZLL_ID = 0x4000,  /**< "Global Scene Control" attribute identifier. */
  ZB_ZCL_ATTR_ON_OFF_ON_TIME_ZLL_ID = 0x4001,               /**< "On Time" attribute identifier. */
  ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME_ZLL_ID = 0x4002,         /**< "Off Wait Time" attribute identifier. */
};


/** @} */ /* Attributes added to On/Off cluster. */

/**
 *  @name ZLL On/Off cluster additions internals.
 *  Internal structures for attribute representation in cluster additions.
 *  @internal
 *  @{
 */


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL_ZLL_ID(data_ptr) \
{                                                                                       \
  ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL_ZLL_ID,                                       \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                         \
  (zb_voidp_t) data_ptr                                                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_ON_TIME_ZLL_ID(data_ptr)  \
{                                                                           \
  ZB_ZCL_ATTR_ON_OFF_ON_TIME_ZLL_ID,                                        \
  ZB_ZCL_ATTR_TYPE_U16,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                            \
  (zb_voidp_t) data_ptr                                                     \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME_ZLL_ID(data_ptr)    \
{                                                                                   \
  ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME_ZLL_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                    \
  (zb_voidp_t) data_ptr                                                             \
}

#ifdef ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST_ZLL
#undef ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST_ZLL
#endif
/**
 *  @brief Declare attribute list for On/Off cluster with ZLL additions. Each attribute type corresponds
 *  to attribute description in ZLL specification (Zigbee Light Link Profile Specification Version 1.0,
 *	chapter 6.6 On/off cluster).
 *  @param attr_list [IN] - attribute list name being declared by this macro.
 *  @param on_off [IN] - pointer to a boolean variable storing on/off attribute value.
 *  @param global_scene_ctrl [IN] - pointer to a boolean variable storing global scene control attribute value.
 *  @param on_time [IN] - pointer to a unsigned 16-bit integer variable storing on time attribute value.
 *  @param off_wait_time [IN] - pointer to a unsigned 16-bit integer variable storing off wait time attribute value.
 */
#define ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST_ZLL(                                                  \
    attr_list, on_off, global_scene_ctrl, on_time, off_wait_time                                \
    )                                                                                           \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                 \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID, (on_off))                                \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL_ZLL_ID, (global_scene_ctrl))   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_ON_TIME_ZLL_ID, (on_time))                          \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME_ZLL_ID, (off_wait_time))              \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* On/Off cluster additions internals. */

/*! @brief On/Off cluster extended command identifiers
    @see ZLL specification, subclause 6.6.1.4
*/
enum zb_zcl_on_off_cmd_zll_e
{
  ZB_ZCL_ON_OFF_OFF_WITH_EFFECT_ID = 0x40,              /**< "Off with effect Mandatory" command identifier. */
  ZB_ZCL_ON_OFF_ON_WITH_RECALL_GLOBAL_SCENE_ID = 0x41,  /**< "On with recall global scene" command identifier. */
  ZB_ZCL_ON_OFF_ON_WITH_TIMED_OFF_ID = 0x42,            /**< "On with timed off" command identifier. */
};

/*! Specific macro for sending Off with effect command */
#define ZB_ZCL_ON_OFF_SEND_OFF_WITH_EFFECT_REQ(                                                 \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, effect_id, effect_var, cb)\
{                                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp);                   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_ON_OFF_OFF_WITH_EFFECT_ID); \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, effect_id);                                                      \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, effect_var);                                                     \
  ZB_ZCL_FINISH_PACKET(buffer, ptr);                                                            \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ON_OFF, cb); \
}

/*! Specific macro for sending On with recall global scene command */
#define ZB_ZCL_ON_OFF_SEND_ZB_ZCL_ON_OFF_ON_WITH_RECALL_GLOBAL_SCENE_REQ(                       \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb)                       \
  ZB_ZCL_ON_OFF_SEND_REQ(buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp,    \
      ZB_ZCL_ON_OFF_ON_WITH_RECALL_GLOBAL_SCENE_ID, cb)

/*! Specific macro for sending On with timed off command */
#define ZB_ZCL_ON_OFF_SEND_ON_WITH_TIMED_OFF_REQ(                                               \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, on_off, on_time, off_wait_time, cb)\
{                                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp);                   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_ON_OFF_OFF_WITH_EFFECT_ID); \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, on_off);                                                         \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, on_time);                                                   \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, off_wait_time);                                             \
  ZB_ZCL_FINISH_PACKET(buffer, ptr);                                                            \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ON_OFF, cb); \
}

/** Effect identifier enum
 * @see ZLL spec 6.6.1.4.4.1 */
enum zb_zcl_on_off_effect_e
{
/**< Effect identifier field value: Delayed all off */
  ZB_ZCL_ON_OFF_EFFECT_ID_DELAYED_ALL_OFF = 0x00,
/**< Effect identifier field value: Dying light */
  ZB_ZCL_ON_OFF_EFFECT_ID_DYING_LIGHT = 0x01
};

/** Effect variant field for delayed enum
* @see ZLL spec 6.6.1.4.4.2 */
enum zb_zcl_on_off_effect_variant_delayed_e
{
/**< Effect variant field value: Fade to off in 0.8 seconds */
  ZB_ZCL_ON_OFF_EFFECT_VARIANT_FADE = 0x00,
/**< Effect variant field value: No fade */
  ZB_ZCL_ON_OFF_EFFECT_VARIANT_NO_FADE = 0x01,
/**< Effect variant field value: 50% dim down in 0.8 seconds then fade to off in 12 seconds */
  ZB_ZCL_ON_OFF_EFFECT_VARIANT_50PART_FADE = 0x02
};

/** Effect variant field for delayed enum
* @see ZLL spec 6.6.1.4.4.2 */
enum zb_zcl_on_off_effect_variant_dying_e
{
/**< Effect variant field value: 20% dim up in 0.5s then fade to off in 1 second */
  ZB_ZCL_ON_OFF_EFFECT_VARIANT_20PART_FADE = 0x00
};

/** Accept only when on
* @see ZLL spec 6.6.1.4.6.1 */
#define ZB_ZCL_ON_OFF_ACCEPT_ONLY_WHEN_ON   1

/*! @brief Structured representsation of Off with effect command payload */
typedef ZB_PACKED_PRE struct zb_zcl_on_off_off_with_effect_req_s
{
  zb_uint8_t effect_id;         /*!< Effect identify */
  zb_uint8_t effect_variant;    /*!< Effect variant */
} ZB_PACKED_STRUCT zb_zcl_on_off_off_with_effect_req_t;

/*! @brief Structured representsation of On with timed off command payload */
typedef ZB_PACKED_PRE struct zb_zcl_on_off_on_with_timed_off_req_s
{
  zb_uint8_t on_off;            /*!< On/off control */
  zb_uint16_t on_time;          /*!< On time variable */
  zb_uint16_t off_wait_time;    /*!< Off wait time variable */
} ZB_PACKED_STRUCT zb_zcl_on_off_on_with_timed_off_req_t;

/** @brief Parses ZLL OnOff with Effect command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_on_off_off_with_effect_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_ON_OFF_GET_OFF_WITH_EFFECT_REQ(data_ptr, buffer, status)             \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_on_off_off_with_effect_req_t))          \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_on_off_off_with_effect_req_t *src_ptr =                                  \
                      (zb_zcl_on_off_off_with_effect_req_t*)ZB_BUF_BEGIN((buffer)); \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_on_off_off_with_effect_req_t));    \
  }                                                                                 \
}

/** @brief Parses ZLL On with timed off command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_on_off_on_with_timed_off_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_ON_OFF_GET_ON_WITH_TIMED_OFF_REQ(data_ptr, buffer, status)           \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_on_off_off_with_effect_req_t))          \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_on_off_on_with_timed_off_req_t *src_ptr =                                \
              (zb_zcl_on_off_on_with_timed_off_req_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->on_off = src_ptr->on_off;                                           \
    ZB_LETOH16(&((data_ptr)->on_time), &(src_ptr->on_time));                        \
    ZB_LETOH16(&((data_ptr)->off_wait_time), &(src_ptr->off_wait_time));            \
  }                                                                                 \
}

/**
 *  @name Inform User App about ZLL On/Off cluster command and change attributes.
 *  Internal structures and define-procedure for inform User App about ZLL On/Off
 *  cluster command and change attributes.
 *  @internal
 *  @{
 */

/** @brief Declare run Set Effect command for User Application
*/
typedef struct zb_zcl_on_off_set_effect_value_param_s
{
  zb_uint8_t effect_id;         /*!< Effect identify */
  zb_uint8_t effect_variant;    /*!< Effect variant */
} zb_zcl_on_off_set_effect_value_param_t;

/*! Struct for invoke User App & continue after */
typedef struct zb_zll_on_off_effect_user_app_schedule_e
{
  zb_zcl_parsed_hdr_t cmd_info;         /**< Parameters for continue command,
                                              see @ref zb_zcl_parsed_hdr_s */
  zb_zcl_on_off_set_effect_value_param_t param; /**< User App command parameters,
                                        see @ref zb_zcl_on_off_set_effect_value_param_s */
} zb_zll_on_off_effect_user_app_schedule_t;

#define ZB_ZLL_ON_OFF_EFFECT_SCHEDULE_USER_APP(buffer, pcmd_info, effectId, effectVar)      \
{                                                                                           \
  zb_zll_on_off_effect_user_app_schedule_t* user_data =                                     \
          ZB_GET_BUF_PARAM((buffer), zb_zll_on_off_effect_user_app_schedule_t);             \
  ZB_MEMMOVE(&(user_data->cmd_info), (pcmd_info), sizeof(zb_zcl_parsed_hdr_t));             \
  user_data->param.effect_id = (effectId);                                                  \
  user_data->param.effect_variant = (effectVar);                                            \
  ZB_SCHEDULE_CALLBACK(zb_zcl_on_off_effect_invoke_user_app, ZB_REF_FROM_BUF((buffer)));    \
}

/** @} */

/** @} */ /* ZLL additions to On/Off cluster. */

#endif /* ! defined ZB_ZCL_ON_OFF_ZLL_ADDS_H */
