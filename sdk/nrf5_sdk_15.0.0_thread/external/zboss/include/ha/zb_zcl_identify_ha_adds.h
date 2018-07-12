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
 * PURPOSE: Identify cluster defintions - extension for HA
*/

#if ! defined ZB_ZCL_IDENTIFY_HA_ADDS_H
#define ZB_ZCL_IDENTIFY_HA_ADDS_H

#include "zcl/zb_zcl_identify.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_IDENTIFY_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
 *  @{
 *    @details
 *    HA extensions for Identify cluster
 */

/* Cluster ZB_ZCL_CLUSTER_ID_IDENTIFY_HA_ADDS */

/*! @name Identify cluster attributes Identify HA extension
    @{
*/

/*! @brief Identify cluster - extension for HA attribute identifiers
    @see HA spec, Identify Cluster 8.3.11
*/
enum zb_zcl_identify_attr_ha_e
{
  /*! @brief Commission State attribute, HA spec 8.3.11.1 */
  ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_HA_ID = 0x0001,

  /*! @brief Struct with pointers on User App callbacks
   * for EZ-Mode Invoke and Update Commission State commands
   * see @ref zb_zcl_identify_callbacks_t, HA Spec 8.3.12 */
  ZB_ZCL_ATTR_IDENTIFY_CALLBACKS_HA_ID = 0xfffe,
};

/** @brief Identify Commission State attribute default value */
#define ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_HA_ID_DEF_VALUE            0x00


/*! @brief Identify Commission State extension for HA attribute flags
    @see HA spec 8.3.11.1
*/
enum zb_zcl_identify_commission_state_ha_e
{
  /** Network State */
  ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_NETWORK         = 1 << 0,
  /** Operational State */
  ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_OPERATIONAL     = 1 << 1,
};

/** Callback for EZ-Mode Invoke command, see HA Spec 8.3.12.1 */
typedef zb_uint8_t (*zb_zcl_identify_ez_mode_invoke_cb)(zb_uint8_t action);
/** Callback for EZ-Mode Invoke command, see HA Spec 8.3.12.2 */
typedef zb_uint8_t (*zb_zcl_identify_update_commission_state_cb)(zb_uint8_t action, zb_uint8_t commission_state_mask);

/*! @brief callback specific to Identify HA extension */
typedef struct zb_zcl_identify_callbacks_s
{
  /** Callback for EZ-Mode Invoke command, see HA Spec 8.3.12.1 */
  zb_zcl_identify_ez_mode_invoke_cb ez_mode_invoke_cb;
  /** Callback for EZ-Mode Invoke command, see HA Spec 8.3.12.2 */
  zb_zcl_identify_update_commission_state_cb update_commission_state_cb;

} zb_zcl_identify_callbacks_t;


/*! @brief Set User App callbacks for Identify cluster request command,
 * @param endpoint - endpoint for which callbacks are to be registered
 * @param ez_mode_invoke_cb - see @ref zb_zcl_identify_ez_mode_invoke_cb
 * @param update_commission_state_cb - see @ref zb_zcl_identify_update_commission_state_cb
*/
zb_void_t zb_zcl_identify_register(
    zb_uint8_t endpoint,
    zb_zcl_identify_ez_mode_invoke_cb ez_mode_invoke_cb,
    zb_zcl_identify_update_commission_state_cb update_commission_state_cb);

/*! @cond internals_doc
    @name Identify cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_HA_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_HA_ID,          \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IDENTIFY_CALLBACKS_HA_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IDENTIFY_CALLBACKS_HA_ID,                 \
  ZB_ZCL_ATTR_TYPE_NULL,                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

/** @internal @brief Declare attribute list for Identify cluster
  @param attr_list - attribute list name
  @param identify_time - pointer to variable to store Identify Time attribute value
  @param commission_state - pointer to variable to store Commission State attribute value
*/
#define ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST_HA(attr_list, identify_time, commission_state)   \
  zb_zcl_identify_callbacks_t callbacks_##attr_list = { NULL, NULL };                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID, (identify_time))          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IDENTIFY_COMMISSION_STATE_HA_ID, (commission_state)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IDENTIFY_CALLBACKS_HA_ID, &(callbacks_##attr_list))  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @}
 *  @endcond */ /* Identify cluster internals */

/*! @} */

/*! @name Identify cluster commands - extension for HA
    @{
*/

/*! @brief Identify cluster request command identifiers - HA extension
    @see HA spec, 8.3.12
*/
enum zb_zcl_identify_req_cmd_ha_e
{
  ZB_ZCL_CMD_IDENTIFY_EZ_MODE_INVOKE_ID           = 0x02, /**< EZ-Mode Invoke command, HA spec, 8.3.12.1 */
  ZB_ZCL_CMD_IDENTIFY_UPDATE_COMMISSION_STATE_ID  = 0x03, /**< Update Commission State command, HA spec, 8.3.12.2 */
};

/******************************* EZ-Mode Invoke ******************************/

/*! @brief EZ-Mode Invoke command Action attribute flags
    @see HA spec 8.3.12.1
*/
enum zb_zcl_identify_ez_mode_invoke_action_ha_e
{
  /** Factory Fresh - Clear all bindings, group table entries and the
      ComissionState attribute and revert to Factory */
  ZB_ZCL_ATTR_IDENTIFY_EZ_MODE_INVOKE_FACTORY_FRESH             = 1 << 0,
  /** Network Steering - Invoke the EZ-Mode Network Steering process  */
  ZB_ZCL_ATTR_IDENTIFY_EZ_MODE_INVOKE_NETWORK_STEERING          = 1 << 1,
  /** Finding and Binding - Invoke the EZ-Mode Finding and Binding
      process if Network Steering is successful    */
  ZB_ZCL_ATTR_IDENTIFY_EZ_MODE_INVOKE_FINDING_BINDING           = 1 << 2,
};

/*! @brief Structure representation of EZ-Mode Invoke - HA extension
 * see HA spec 8.3.12.1 */
typedef ZB_PACKED_PRE struct zb_zcl_identify_ez_mode_invoke_ha_s
{
  /** Action, see @ref zb_zcl_identify_ez_mode_invoke_action_ha_e */
  zb_uint8_t action;
} ZB_PACKED_STRUCT zb_zcl_identify_ez_mode_invoke_ha_t;

/*! @brief EZ-Mode Invoke command - HA extension, see HA spec 8.3.12.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - default response
    @param cb - callback for getting command send status
    @param action - Action, see @ref zb_zcl_identify_ez_mode_invoke_action_ha_e
*/
#define ZB_ZCL_IDENTIFY_SEND_EZ_MODE_INVOKE_HA_REQ(                             \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, action)     \
{                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));         \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),                    \
                  ZB_ZCL_CMD_IDENTIFY_EZ_MODE_INVOKE_ID);                       \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (action));                                       \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                          \
  ZB_ZCL_SEND_COMMAND_SHORTbuffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,    \
      ZB_ZCL_CLUSTER_ID_IDENTIFY, cb);                                          \
}

/** @brief Macro for getting EZ-Mode Invoke command - HA extension, see HA spec 8.3.12.1
  * @attention Assumes that ZCL header already cut
  * @param data_ptr - pointer to a variable of type @ref zb_zcl_identify_ez_mode_invoke_ha_t
  * @param buffer - buffer containing the packet (by pointer)
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t)
  */
#define ZB_ZCL_IDENTIFY_GET_EZ_MODE_INVOKE_HA_REQ(data_ptr, buffer, status) \
{                                                                           \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_identify_ez_mode_invoke_ha_t))  \
  {                                                                         \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                  \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    zb_zcl_identify_ez_mode_invoke_ha_t *src_ptr =                          \
         (zb_zcl_identify_ez_mode_invoke_ha_t*)ZB_BUF_BEGIN((buffer));      \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                 \
    (data_ptr)->action = src_ptr->action;                                   \
  }                                                                         \
}

/******************************* Update Commission State ******************************/

/*! @brief Update Commission State command Action attribute flags
    @see HA spec 8.3.12.1.2 */
enum zb_zcl_identify_update_commission_state_action_ha_e
{
  /** Do nothing */
  ZB_ZCL_ATTR_IDENTIFY_UPDATE_COMMISSION_STATE_NULL                 = 1 << 0,
  /** For each bit set in the CommissionStateMask, set the
      same bit in the CommissionState attribute*/
  ZB_ZCL_ATTR_IDENTIFY_UPDATE_COMMISSION_STATE_SET                 = 1 << 1,
  /** For each bit set in the CommissionStateMask, clear the
same bit in the CommissionState attribute */
  ZB_ZCL_ATTR_IDENTIFY_UPDATE_COMMISSION_STATE_CLEAR                 = 1 << 2,
};

/*! @brief Structure representation of Update Commission State - HA extension
 * see HA spec 8.3.12.2 */
typedef ZB_PACKED_PRE struct zb_zcl_identify_update_commission_state_ha_s
{
  /** Action, see @ref zb_zcl_identify_update_commission_state_action_ha_e */
  zb_uint8_t action;
  /** Commission State Mask attribute, see @ref zb_zcl_identify_commission_state_ha_e */
  zb_uint8_t commission_state_mask;
} ZB_PACKED_STRUCT zb_zcl_identify_update_commission_state_ha_t;

/*! @brief Update Commission State command - HA extension, see HA spec 8.3.12.2
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - default response
    @param cb - callback for getting command send status
    @param action - Action, see @ref zb_zcl_identify_update_commission_state_action_ha_e
    @param commission_state_mask - Commission State Mask, see @ref zb_zcl_identify_commission_state_ha_e
*/
#define ZB_ZCL_IDENTIFY_SEND_UPDATE_COMMISSION_STATE_HA_REQ(                    \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb,             \
    action, commission_state_mask)                                              \
{                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));         \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),                    \
      ZB_ZCL_CMD_IDENTIFY_UPDATE_COMMISSION_STATE_ID);                          \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (action));                                       \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (commission_state_mask));                        \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                          \
  ZB_ZCL_SEND_COMMAND_SHORTbuffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,    \
      ZB_ZCL_CLUSTER_ID_IDENTIFY, cb);                                          \
}

/** @brief Macro for getting Update Commission State command - HA extension, see HA spec 8.3.12.2
  * @attention Assumes that ZCL header already cut
  * @param data_ptr - pointer to a variable of type @ref zb_zcl_identify_update_commission_state_ha_s
  * @param buffer containing the packet (by pointer)
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t)
  */
#define ZB_ZCL_IDENTIFY_GET_UPDATE_COMMISSION_STATE_HA_REQ(data_ptr, buffer, status)    \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_identify_update_commission_state_ha_t))     \
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_identify_update_commission_state_ha_t *src_ptr =                             \
         (zb_zcl_identify_update_commission_state_ha_t*)ZB_BUF_BEGIN((buffer));         \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    (data_ptr)->action = src_ptr->action;                                               \
    (data_ptr)->commission_state_mask = src_ptr->commission_state_mask;                 \
  }                                                                                     \
}


/*! @} */ /* Identify cluster commands - HA extension */

/*! @name User App for queue Status Change Notification command
    @{
*/

#ifdef IAS_OBSOLETE
//XXXXXXXXXXX
typedef struct zb_zcl_identify_queue_interface_s
{
  zb_callback_t process_result;

} zb_zcl_identify_queue_interface_t;


/** @brief Registry callbacks for use queue Zone Status Change Noticication events

    @param process_result - callback pointer on process send result command, see @ref zb_zcl_identify_queue_result_cb_t
*/
zb_void_t zb_zcl_identify_queue_registry(
    zb_uint8_t endpoint,
    zb_callback_t process_result);

/* Set Zone Status - all bits
 * for User App
 * @param param - buffer handler
*/
zb_void_t zb_zcl_identify_set_status(zb_uint8_t param);


/** @brief Declare set Zone Status attribute
*/
typedef struct zb_zcl_identify_set_status_param_s
{
  zb_uint8_t endpoint;        /*!< endpoint */
  zb_uint16_t value;          /*!< new value of zone_zone_status, see @ref zb_zcl_identify_zonestatus_e */
  zb_uint16_t old_value;      /*!< previos value of zone_zone_status, see @ref zb_zcl_identify_zonestatus_e */
  zb_uint16_t delay;          /*!< Delay */
  zb_uint8_t param;           /*!< buffer reference */

} zb_zcl_identify_set_status_param_t;

/*! @brief Set Zone Status bits custom server command
    @param buffer - to put packet to
    @param ep - sending endpoint
    @param new_val - new value of zone_zone_status
    @param old_val - old value of zone_zone_status
    @param ts - event timestamp
*/
#define ZB_ZCL_IDENTIFY_SET_STATUS(buffer, ep, new_val, old_val, d)             \
{                                                                               \
  zb_zcl_identify_set_status_param_t* cmd_data =                                \
          ZB_GET_BUF_PARAM((buffer), zb_zcl_identify_set_status_param_t);       \
  cmd_data->endpoint = (ep);                                                    \
  cmd_data->value = (new_val);                                                  \
  cmd_data->old_value = (old_val);                                              \
  cmd_data->param = ZB_REF_FROM_BUF((buffer));                                  \
  cmd_data->delay = (d);                                                        \
  ZB_SCHEDULE_CALLBACK(zb_zcl_identify_set_status, ZB_REF_FROM_BUF((buffer)));  \
}

//XXXXXXXXXXXX

#endif /* IAS_OBSOLETE */

/*! @} */ /* User App for queue Status Change Notification command */

/*! @} */ /* ZCL Identify cluster definitions - HA extension */

zb_uint8_t zb_zcl_get_cmd_list_identify(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_IDENTIFY_HA_ADDS_H */
