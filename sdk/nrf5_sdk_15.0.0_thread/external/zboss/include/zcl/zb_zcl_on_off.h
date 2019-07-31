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
 * PURPOSE: On/Off cluster defintions
*/

#if ! defined ZB_ZCL_ON_OFF_H
#define ZB_ZCL_ON_OFF_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

#if defined ZB_ZCL_SUPPORT_CLUSTER_SCENES
#include "zcl/zb_zcl_scenes.h"
#endif // defined ZB_ZCL_SUPPORT_CLUSTER_SCENES

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_ON_OFF
 *  @{
 *    @details
 *    All commands in the cluster have only request form, and carry no payload.
 *
 *    @par Example
 *    On command sending:
 *    @snippet 5.7.3_onoff_server/ha_th.c Send On command param
 *    @snippet 5.7.3_onoff_server/ha_th.c Send On command
 *    @par
 *
 *    Other two commands can be sent in the same way using appropriate macros.
 *
 *    For more information see 5.7.3_onoff_server sample
 */

/* Cluster ZB_ZCL_CLUSTER_ID_ON_OFF */

/*! @name On/Off cluster attributes
    @{
*/

/*! @brief On/Off cluster attribute identifiers
    @see ZCL spec, subclause 3.8.2.2
*/
enum zb_zcl_on_off_attr_e
{
  /*! @brief OnOff attribute */
  ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID = 0
};

/*! @brief Permissible values for OnOff attribute */
enum zb_zcl_on_off_on_off_e
{
  /*! "Off" value */
  ZB_ZCL_ON_OFF_IS_OFF = 0,
  /*! "On" value */
  ZB_ZCL_ON_OFF_IS_ON  = 1
};

/** @brief Default value for OnOff attribute */
#define ZB_ZCL_ON_OFF_ON_OFF_DEFAULT_VALUE (ZB_ZCL_ON_OFF_IS_OFF)

/*! @} */ /* On/Off cluster attributes */

/*! @name On/Off cluster commands
    @{
*/

/*! @brief On/Off cluster command identifiers
    @see ZCL spec, subclause 3.8.2.3
*/
enum zb_zcl_on_off_cmd_e
{
  ZB_ZCL_CMD_ON_OFF_OFF_ID    = 0x00, /**< "Turn off" command. */
  ZB_ZCL_CMD_ON_OFF_ON_ID     = 0x01, /**< "Turn on" command. */
  ZB_ZCL_CMD_ON_OFF_TOGGLE_ID = 0x02  /**< "Toggle state" command. */
};

/** @cond internals_doc */

/*! @brief General macro for sending On/Off cluster command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param command_id - command identifier
    @param cb - callback to call to report send status
*/
#define ZB_ZCL_ON_OFF_SEND_REQ(                                                         \
    buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, command_id, cb) \
{                                                                                       \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                        \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)           \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), command_id);               \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                    \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                            \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ON_OFF, cb);  \
}

/** @endcond */ /* internals_doc */

/*! Specific macro for sending Off command */
#define ZB_ZCL_ON_OFF_SEND_OFF_REQ(                                         \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, dis_default_resp, cb) \
  ZB_ZCL_ON_OFF_SEND_REQ(                                                   \
      buffer,                                                               \
      addr,                                                                 \
      dst_addr_mode,                                                        \
      dst_ep,                                                               \
      ep,                                                                   \
      prfl_id,                                                              \
      dis_default_resp,                                                     \
      ZB_ZCL_CMD_ON_OFF_OFF_ID,                                             \
      cb)

/*! Specific macro for sending On command */
#define ZB_ZCL_ON_OFF_SEND_ON_REQ(                                          \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, dis_default_resp, cb) \
  ZB_ZCL_ON_OFF_SEND_REQ(                                                   \
      buffer,                                                               \
      addr,                                                                 \
      dst_addr_mode,                                                        \
      dst_ep,                                                               \
      ep,                                                                   \
      prfl_id,                                                              \
      dis_default_resp,                                                     \
      ZB_ZCL_CMD_ON_OFF_ON_ID,                                              \
      cb)

/*! Specific macro for sending Toggle command */
#define ZB_ZCL_ON_OFF_SEND_TOGGLE_REQ(                                      \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, dis_default_resp, cb) \
  ZB_ZCL_ON_OFF_SEND_REQ(                                                   \
      buffer,                                                               \
      addr,                                                                 \
      dst_addr_mode,                                                        \
      dst_ep,                                                               \
      ep,                                                                   \
      prfl_id,                                                              \
      dis_default_resp,                                                     \
      ZB_ZCL_CMD_ON_OFF_TOGGLE_ID,                                          \
      cb)

/*! @} */ /* On/Off cluster commands */

/*! @cond internals_doc
    @internal @name On/Off cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID(data_ptr)                        \
{                                                                                            \
  ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,                                                              \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING | ZB_ZCL_ATTR_ACCESS_SCENE,    \
  (zb_voidp_t) data_ptr                                                                      \
}

/** @internal @brief Declare attribute list for On/Off cluster
    @param attr_list - attribure list name
    @param on_off - pointer to variable to store On/Off attribute value
*/
#define ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST(attr_list, on_off)            \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID, (on_off))          \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#ifndef ZB_ENABLE_ZLL
#define ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST_ZLL(                                                  \
    attr_list, on_off, global_scene_ctrl, on_time, off_wait_time                                \
    )                                                                                           \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                 \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID, (on_off))                                \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST
#endif /* ZB_ENABLE_ZLL */

/*! @internal Number of attributes mandatory for reporting in On/Off cluster */
#define ZB_ZCL_ON_OFF_REPORT_ATTR_COUNT 1

/*! @internal Struct for invoke User App & continue after */
typedef struct zb_zll_on_off_user_app_schedule_e
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t new_value;
  zb_bool_t is_run_timer;
} zb_zll_on_off_user_app_schedule_t;


/*! @}
    @endcond */ /* On/Off cluster internals */

/*! @} */ /* ZCL On/Off cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_on_off_init_server();
zb_void_t zb_zcl_on_off_init_client();
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SERVER_ROLE_INIT zb_zcl_on_off_init_server
#define ZB_ZCL_CLUSTER_ID_ON_OFF_CLIENT_ROLE_INIT zb_zcl_on_off_init_client

#endif /* ! defined ZB_ZCL_ON_OFF_H */
