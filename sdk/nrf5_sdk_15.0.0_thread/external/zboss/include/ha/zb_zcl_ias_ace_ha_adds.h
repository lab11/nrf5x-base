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
 * PURPOSE: IAS Ace cluster defintions - HA extension
*/

#if ! defined ZB_ZCL_IAS_ACE_HA_ADDS_H
#define ZB_ZCL_IAS_ACE_HA_ADDS_H

#include "zcl/zb_zcl_ias_ace.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_IAS_ACE_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
 *  @{
 *    @details
 *    HA extensions for IAS ACE cluster
 */

/* Cluster ZB_ZCL_CLUSTER_ID_IAS_ACE - HA extension */

/*! @name IAS Ace cluster commands
    @{
*/

/*! @brief IAS Ace cluster responce command identifiers - HA extension
    @see HA spec, 10.8.4.4, 10.8.4.5
*/
enum zb_zcl_ias_ace_resp_cmd_ha_e
{
  // todo command index ?
  ZB_ZCL_CMD_IAS_ACE_ZONE_STATUS_CHANGED_ID   = 0x40, /**< Zone Status Changed command, HA spec, 10.8.4.4 */
  ZB_ZCL_CMD_IAS_ACE_PANEL_STATUS_CHANGED_ID  = 0x01, /**< Panel Status Changed command, HA spec, 10.8.4.5 */
};


/******************************* Arm Command ******************************/

/*! @brief Structure representsation of Arm command - HA extended, HA spec 10.8.4.3.1.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_arm_ha_s
{
  /** Arm Mode, see @ref zb_zcl_ias_ace_arm_mode_e*/
  zb_uint8_t arm_mode;
  /** Arm/Disarm Code */
  zb_char_t arm_code[8];
  /** Zone ID */
  zb_uint8_t zone_id;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_arm_ha_t;


/*! @brief Arm command - HA extended, HA spec 10.8.4.3.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param arm_mode - Arm Mode, see @ref zb_zcl_ias_ace_arm_mode_e
    @param arm_code - Arm/Disarm Code
    @param zone_id - Zone ID
*/
#define ZB_ZCL_IAS_ACE_SEND_ARM_HA_REQ(                           \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, \
    arm_mode, arm_code, zone_id )                                 \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_ARM_ID);                                 \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (arm_mode));                       \
  ZB_ZCL_PACKET_PUT_DATA_N(ptr, (arm_code), 8);                   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                        \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/** @brief Macro for getting Arm command - HA extended, HA spec 10.8.4.3.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_arm_ha_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_ARM_HA_REQ(data_ptr, buffer, status)   \
{                                                                 \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_arm_ha_t))    \
  {                                                               \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                        \
  }                                                               \
  else                                                            \
  {                                                               \
    zb_zcl_ias_ace_arm_ha_t *src_ptr =                            \
         (zb_zcl_ias_ace_arm_ha_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                       \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_ias_ace_arm_ha_t)); \
  }                                                               \
}

/******************************* Zone Status Changed command ******************************/

/*! @brief Structure representsation of Zone Status Changed command, HA spec, 10.8.4.4 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_zone_status_changed_s
{
  /** Zone ID, see HA spec 10.8.4.4.0.2 */
  zb_uint8_t zone_id;
  /** Zone Status, see HA spec 10.8.4.4.0.3 */
  zb_uint16_t zone_status;
  /** Zone Label, see HA spec 10.8.4.4.0.4 */
  zb_char_t zone_label[16];

} ZB_PACKED_STRUCT zb_zcl_ias_ace_zone_status_changed_t;


/*! @brief Zone Status Changed command, HA spec, 10.8.4.4
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
    @param zone_id - Zone ID
    @param zone_status - Zone Status
    @param zone_label - Zone Label
*/
#define ZB_ZCL_IAS_ACE_SEND_ZONE_STATUS_CHANGED_RESP(           \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb,       \
    zone_id, zone_status, zone_label)                           \
{                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),    \
      ZB_ZCL_CMD_IAS_ACE_ZONE_STATUS_CHANGED_ID);               \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                      \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (zone_status));             \
  ZB_ZCL_PACKET_PUT_DATA_N(ptr, (zone_label), 16);              \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,         \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                           \
}

/** @brief Macro for getting Zone Status Changed command, HA spec, 10.8.4.4
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_zone_status_changed_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_ZONE_STATUS_CHANGED_RESP(data_ptr, buffer, status)         \
{                                                                                 \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_zone_status_changed_t))       \
  {                                                                               \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                        \
  }                                                                               \
  else                                                                            \
  {                                                                               \
    zb_zcl_ias_ace_zone_status_changed_t *src_ptr =                               \
         (zb_zcl_ias_ace_zone_status_changed_t*)ZB_BUF_BEGIN((buffer));           \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                       \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_ias_ace_zone_status_changed_t)); \
    ZB_HTOLE16(&((data_ptr)->zone_status), &(src_ptr->zone_status));              \
  }                                                                               \
}

/******************************* Panel Status Changed command ******************************/

/** @brief Panel Status, see HA spec 10.8.4.5.0.2  */
enum zb_zcl_ias_ace_panel_status_e
{
  /** Panel disarmed (all zones disarmed) and ready to arm */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_DISARMED    = 0x00,
  /** Armed stay */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_ARMED_STAY  = 0x01,
  /** Armed night */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_ARMED_NIGHT = 0x02,
  /** Armed away */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_ARMED_AWAY  = 0x03,
  /** Exit delay */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_EXIT_DELAY  = 0x04,
  /** Entry delay */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_ENTRY_DELAY = 0x05,
  /** Not ready to arm */
  ZB_ZCL_IAS_ACE_PANEL_STATUS_NOT_READY   = 0x06,
};

/*! @brief Structure representsation of Panel Status Changed command, HA spec, 10.8.4.5 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_panel_status_changed_s
{
  /** Panel Status, see HA spec 10.8.4.5.0.2, see @ref zb_zcl_ias_ace_panel_status_e */
  zb_uint8_t panel_status;
  /** Seconds Remaining, see HA spec 10.8.4.5.0.3 */
  zb_uint8_t seconds_remaining;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_panel_status_changed_t;


/*! @brief Panel Status Changed command, HA spec, 10.8.4.5
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
    @param panel_status - Panel Status, see HA spec 10.8.4.5.0.2, see @ref zb_zcl_ias_ace_panel_status_e
    @param seconds_remaining - Seconds Remaining, see HA spec 10.8.4.5.0.3
*/
#define ZB_ZCL_IAS_ACE_SEND_PANEL_STATUS_CHANGED_RESP(          \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb,       \
    panel_status, seconds_remaining)                            \
{                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),    \
      ZB_ZCL_CMD_IAS_ACE_PANEL_STATUS_CHANGED_ID);              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (panel_status));                 \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (seconds_remaining));            \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,         \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                           \
}

/** @brief Macro for getting Panel Status Changed command, HA spec, 10.8.4.4
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_panel_status_changed_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_PANEL_STATUS_CHANGED_RESP(data_ptr, buffer, status)          \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_panel_status_changed_t))        \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_ias_ace_panel_status_changed_t *src_ptr =                                \
         (zb_zcl_ias_ace_panel_status_changed_t*)ZB_BUF_BEGIN((buffer));            \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_ias_ace_panel_status_changed_t));  \
  }                                                                                 \
}


/*! @} */ /* IAS Ace cluster commands */

/*! @} */ /* ZCL IAS Ace cluster definitions */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_IAS_ACE_HA_ADDS_H */
