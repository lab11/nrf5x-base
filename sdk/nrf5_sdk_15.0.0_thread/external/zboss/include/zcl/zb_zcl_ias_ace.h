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
 * PURPOSE: IAS Ace cluster defintions
*/

#if ! defined ZB_ZCL_IAS_ACE_H
#define ZB_ZCL_IAS_ACE_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_IAS_ACE
 *  @{
 */


/* Cluster ZB_ZCL_CLUSTER_ID_IAS_ACE */

/*! @name IAS Ace cluster attributes
    @{
*/

/*! @brief Structure representsation of IAS ACE Zone Table
 * see ZCL spec 8.3.2.3, table 8.11 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_zone_table_s
{
  /** Zone ID */
  zb_uint8_t zone_id;
  /** Zone Type, see @ref zb_zcl_ias_zone_zonetype_e */
  zb_uint16_t zone_type;
  /** Zone Address */
  zb_ieee_addr_t zone_address;
} ZB_PACKED_STRUCT zb_zcl_ias_ace_zone_table_t;

/** @brief IAS ACE ZoneType attribute maximum value */
#define ZB_ZCL_IAS_ACE_ZONE_TYPE_MAX_VALUE    0xfffe

/** @brief IAS ACE ZoneType attribute maximum value */
#define ZB_ZCL_IAS_ACE_ZONE_TABLE_LENGTH      256

/*! @brief IAS Ace cluster custom attribute identifiers
*/
enum zb_zcl_ias_ace_attr_e
{
  /*! @brief Ace table length, ZCL spec 8.3.2.3 */
  ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_LENGTH_ID = 0xfffe,
  /*! @brief Ace table ZCL spec 8.3.2.3 */
  ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_ID = 0xffff,

};

/*! @cond internals_doc
    @name IAS Ace cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_LENGTH_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_LENGTH_ID,             \
  ZB_ZCL_ATTR_TYPE_U16,                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_ID,                    \
  ZB_ZCL_ATTR_TYPE_NULL,                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

/** @internal @brief Declare attribute list for IAS Ace cluster - server side
    @param attr_list - attribure list name
    @param length - (zb_uint16_t*) pointer to variable to store Length of IAS ACE Zone Table attribute,
        see ZCL spec 8.3.2.3, table 8.11
    @param table - pointer to variable to store IAS ACE Zone Table attribute,
    see ZCL spec 8.3.2.3, table 8.11
*/
#define ZB_ZCL_DECLARE_IAS_ACE_ATTRIB_LIST(attr_list, length, table)        \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_LENGTH_ID, (length))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_ID, (table))          \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


/*! @internal Number of attributes mandatory for reporting in IAS Ace cluster */
#define ZB_ZCL_IAS_ACE_REPORT_ATTR_COUNT 0

/*! @}
 *  @endcond*/ /* IAS Ace cluster internals */

/*! @} */ /* IAS Ace cluster attributes */

/*! @name IAS Ace cluster commands
    @{
*/

/*! @brief IAS Ace cluster command identifiers
    @see ZCL spec, IAS Ace Cluster, 8.3.2.4
*/
enum zb_zcl_ias_ace_cmd_e
{
  ZB_ZCL_CMD_IAS_ACE_ARM_ID             = 0x00,  /**< Arm command, ZCL spec 8.3.2.4.1 */
  ZB_ZCL_CMD_IAS_ACE_BYPASS_ID          = 0x01,  /**< Bypass command, ZCL spec 8.3.2.4.2 */
  ZB_ZCL_CMD_IAS_ACE_EMERGENCY_ID       = 0x02,  /**< Emergency command, ZCL spec 8.3.2.4.3 */
  ZB_ZCL_CMD_IAS_ACE_FIRE_ID            = 0x03,  /**< Fire command, ZCL spec 8.3.2.4.3 */
  ZB_ZCL_CMD_IAS_ACE_PANIC_ID           = 0x04,  /**< Panic command, ZCL spec 8.3.2.4.3 */
  ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_ID = 0x05,  /**< Get Zone ID Map command, ZCL spec 8.3.2.4.4 */
  ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_ID   = 0x06,  /**< Get Zone Information command, ZCL spec 8.3.2.4.5 */
};

/*! @brief IAS Ace cluster responce command identifiers
    @see ZCL spec, IAS Ace Cluster, 8.3.2.5
*/
enum zb_zcl_ias_ace_resp_cmd_e
{
  ZB_ZCL_CMD_IAS_ACE_ARM_RESP_ID              = 0x00, /**< Arm Response command, ZCL spec 8.3.2.5.1 */
  ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_RESP_ID  = 0x01, /**< Get Zone ID Map Response command, ZCL spec 8.3.2.5.2 */
  ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_RESP_ID    = 0x02, /**< Get Zone Information Response command, ZCL spec 8.3.2.5.3 */
};


/******************************* Arm Command ******************************/

/** @brief Values of the Arm Mode
 * see ZCL spec 8.3.2.4.1.2
*/
enum zb_zcl_ias_ace_arm_mode_e
{
  /** Disarm */
  ZB_ZCL_IAS_ACE_ARM_MODE_DISARM  = 0x00,
  /** Arm Day/Home Zones Only */
  ZB_ZCL_IAS_ACE_ARM_MODE_DAY     = 0x01,
  /** Arm Night/Sleep Zones Only */
  ZB_ZCL_IAS_ACE_ARM_MODE_NIGHT   = 0x02,
  /** Arm All Zones */
  ZB_ZCL_IAS_ACE_ARM_MODE_ALL     = 0x03,
};

/*! @brief Structure representsation of Arm command, ZCL spec 8.3.2.4.1.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_arm_s
{
  /** Arm Mode, see @ref zb_zcl_ias_ace_arm_mode_e*/
  zb_uint8_t arm_mode;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_arm_t;


/*! @brief Arm command, ZCL spec 8.3.2.4.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param arm_mode - Arm Mode, see @ref zb_zcl_ias_ace_arm_mode_e
*/
#define ZB_ZCL_IAS_ACE_SEND_ARM_REQ(                              \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, \
    arm_mode )                                                    \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_ARM_ID);                                 \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (arm_mode));                       \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/** @brief Macro for getting Arm command, see ZCL spec 8.3.2.4.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_arm_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_ARM_REQ(data_ptr, buffer, status)  \
{                                                             \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_arm_t))   \
  {                                                           \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                    \
  }                                                           \
  else                                                        \
  {                                                           \
    zb_zcl_ias_ace_arm_t *src_ptr =                           \
         (zb_zcl_ias_ace_arm_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                   \
    (data_ptr)->arm_mode = src_ptr->arm_mode;                 \
  }                                                           \
}

/******************************* Bypass Command ******************************/

/*! @brief Structure representsation of Bypass command, ZCL spec 8.3.2.4.2.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_bypass_s
{
  /** Number of Zones */
  zb_uint8_t length;
  /** Zone ID array, see @ref zb_zcl_ias_ace_zone_table_s */
  zb_uint8_t zone_id[256];

} ZB_PACKED_STRUCT zb_zcl_ias_ace_bypass_t;


/*! @brief Start Bypass command, ZCL spec 8.3.2.4.1
    @param buffer - to put packet to
    @param length - Number of Zones
    @param def_resp - enable/disable default response
    @param ptr - [out] (zb_uint8_t*) current position for @ref ZB_ZCL_IAS_ACE_SEND_BYPASS_REQ_ADD
    and @ref ZB_ZCL_IAS_ACE_SEND_BYPASS_REQ_END
*/
#define ZB_ZCL_IAS_ACE_SEND_BYPASS_REQ_START(buffer, def_resp, length, ptr) \
{                                                                 \
  (ptr) = ZB_ZCL_START_PACKET_REQ(buffer)                            \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_BYPASS_ID);                              \
  ZB_ZCL_PACKET_PUT_DATA8((ptr), (length));                       \
}

/*! @brief Add Zone Id to Bypass command, ZCL spec 8.3.2.4.1
    @param ptr - [in/out] (zb_uint8_t*) current position
    @param zone_id - Zone ID, see @ref zb_zcl_ias_ace_zone_table_s
*/
#define ZB_ZCL_IAS_ACE_SEND_BYPASS_REQ_ADD(ptr, zone_id)          \
{                                                                 \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                        \
}

/*! @brief End form Bypass command and send it, ZCL spec 8.3.2.4.1
    @param ptr - (zb_uint8_t*) current position
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ACE_SEND_BYPASS_REQ_END(                       \
    ptr, buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb)    \
{                                                                 \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/** @brief Macro for getting Bypass command, see ZCL spec 8.3.2.4.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_bypass_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_BYPASS_REQ(data_ptr, buffer, status)       \
{                                                                     \
  zb_zcl_ias_ace_bypass_t *src_ptr =                                  \
       (zb_zcl_ias_ace_bypass_t*)ZB_BUF_BEGIN((buffer));              \
  if (ZB_BUF_LEN((buffer)) != src_ptr->length+1 /*1-sizeof(length)*/) \
  {                                                                   \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                            \
  }                                                                   \
  else                                                                \
  {                                                                   \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                           \
    ZB_MEMCPY((data_ptr), src_ptr, src_ptr->length+1);                \
  }                                                                   \
}

/******************************* Emergency Command ******************************/

/*! @brief Emergency command, ZCL spec 8.3.2.4.3
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ACE_SEND_EMERGENCY_REQ(                        \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb) \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_EMERGENCY_ID);                           \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/******************************* Fire Command ******************************/

/*! @brief Fire command, ZCL spec 8.3.2.4.3
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ACE_SEND_FIRE_REQ(                             \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb) \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_FIRE_ID);                                \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/******************************* Panic Command ******************************/

/*! @brief Panic command, ZCL spec 8.3.2.4.3
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ACE_SEND_PANIC_REQ(                            \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb) \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_PANIC_ID);                               \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/******************************* Get Zone ID Map command ******************************/

/*! @brief Get Zone ID Map command, ZCL spec 8.3.2.4.4
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ACE_SEND_GET_ZONE_ID_MAP_REQ(                  \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb) \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_ID);                     \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/******************************* Get Zone Information command ******************************/

/*! @brief Structure representsation of Get Zone Information command, ZCL spec 8.3.2.4.5 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_get_zone_info_s
{
  /** Zone ID, see @ref zb_zcl_ias_ace_zone_table_s */
  zb_uint8_t zone_id;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_get_zone_info_t;


/*! @brief Get Zone Information command, ZCL spec 8.3.2.4.5
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param zone_id - Zone ID, see @ref zb_zcl_ias_ace_zone_table_s
*/
#define ZB_ZCL_IAS_ACE_SEND_GET_ZONE_INFO_REQ(                    \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, \
    zone_id)                                                      \
{                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),      \
      ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_ID);                       \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                        \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, cb);                             \
}

/** @brief Macro for getting Get Zone Information command, ZCL spec 8.3.2.4.5
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_get_zone_info_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_REQ(data_ptr, buffer, status)  \
{                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_get_zone_info_t))   \
  {                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                              \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    zb_zcl_ias_ace_get_zone_info_t *src_ptr =                           \
         (zb_zcl_ias_ace_get_zone_info_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                             \
    (data_ptr)->zone_id = src_ptr->zone_id;                             \
  }                                                                     \
}

/******************************* Arm Response command ******************************/

/*! @brief Values of the Arm Notification
 * see ZCL spec 8.3.2.5.1.2
*/
enum zb_zcl_ias_ace_arm_not_e
{
  /*! All Zones Disarmed */
  ZB_ZCL_IAS_ACE_ARM_NOT_DISARM  = 0x00,
  /*! Only Day/Home Zones Armed */
  ZB_ZCL_IAS_ACE_ARM_NOT_DAY     = 0x01,
  /*! Only Night/Sleep Zones Armed */
  ZB_ZCL_IAS_ACE_ARM_NOT_NIGHT   = 0x02,
  /*! All Zones Armed */
  ZB_ZCL_IAS_ACE_ARM_NOT_ALL     = 0x03,
};

/*! @brief Structure representation of Arm Response command, ZCL spec 8.3.2.5.1  */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_arm_resp_s
{
  /** Arm Notification, see @ref zb_zcl_ias_ace_arm_not_e */
  zb_uint8_t arm_notification;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_arm_resp_t;


/*! @brief Arm Response command, ZCL spec 8.3.2.5.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq - sequence
    @param arm_notification - Arm Notification, see @ref zb_zcl_ias_ace_arm_not_e
*/
#define ZB_ZCL_IAS_ACE_SEND_ARM_RESP(                           \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, seq,      \
    arm_notification)                                           \
{                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq,                     \
      ZB_ZCL_CMD_IAS_ACE_ARM_RESP_ID);                          \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (arm_notification));             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,         \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, NULL);                         \
}

/** @brief Macro for getting Arm Response command, ZCL spec 8.3.2.5.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_arm_resp_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_ARM_RESP(data_ptr, buffer, status)       \
{                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_arm_resp_t))    \
  {                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                          \
  }                                                                 \
  else                                                              \
  {                                                                 \
    zb_zcl_ias_ace_arm_resp_t *src_ptr =                            \
         (zb_zcl_ias_ace_arm_resp_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                         \
    (data_ptr)->arm_notification = src_ptr->arm_notification;       \
  }                                                                 \
}

/******************************* Get Zone ID Map Response command ******************************/

/*! @brief Length of array of Bitmap of Zone ID Map, ZCL spec 8.3.2.5.2.1  */
#define ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_LENGTH   16

/*! @brief Structure representsation of Get Zone ID Map Response command, ZCL spec 8.3.2.5.2.1  */
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_get_zone_id_map_resp_s
{
  /** Zone ID Map */
  zb_uint16_t zone_id_map[ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_LENGTH];

} ZB_PACKED_STRUCT zb_zcl_ias_ace_get_zone_id_map_resp_t;


/*! @brief Get Zone ID Map Response command, ZCL spec 8.3.2.5.2
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq - sequence
    @param zone_id_map - Zone ID Map, array[16] of zb_uint16_t
*/
#define ZB_ZCL_IAS_ACE_SEND_GET_ZONE_ID_MAP_RESP(               \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, seq,      \
    zone_id_map)                                                \
{                                                               \
  zb_uint8_t i;                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq,                     \
      ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_RESP_ID);              \
  for(i=0; i<ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_LENGTH; i++)        \
  {                                                             \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (zone_id_map)[i]);        \
  }                                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,         \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, NULL);                         \
}

/** @brief Macro for getting Get Zone ID Map Response command, ZCL spec 8.3.2.5.2
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_get_zone_id_map_resp_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_GET_ZONE_ID_MAP_RESP(data_ptr, buffer, status)       \
{                                                                               \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_get_zone_id_map_resp_t))    \
  {                                                                             \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                      \
  }                                                                             \
  else                                                                          \
  {                                                                             \
    zb_uint8_t i;                                                               \
    zb_zcl_ias_ace_get_zone_id_map_resp_t *src_ptr =                            \
         (zb_zcl_ias_ace_get_zone_id_map_resp_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                     \
    for(i=0; i<ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_LENGTH; i++)                      \
    {                                                                           \
      ZB_HTOLE16(&((data_ptr)->zone_id_map[i]), &(src_ptr->zone_id_map[i]));    \
    }                                                                           \
  }                                                                             \
}

/******************* Get Zone Information Response command ******************************/

/*! @brief Zone Type is empty, ZCL spec 8.3.2.5.3.1  */
#define ZB_ZCL_IAS_ACE_GET_ZONE_INFO_TYPE_NONE   0xffff

/*! @brief Structure representsation of Get Zone Information Response command, ZCL spec 8.3.2.5.3.1
 * see @ ref zb_zcl_ias_ace_zone_table_s*/
typedef ZB_PACKED_PRE struct zb_zcl_ias_ace_get_zone_info_resp_s
{
  /** Zone ID */
  zb_uint8_t zone_id;
  /** Zone Type, see @ref zb_zcl_ias_zone_zonetype_e */
  zb_uint16_t zone_type;
  /** Zone Address */
  zb_ieee_addr_t address;

} ZB_PACKED_STRUCT zb_zcl_ias_ace_get_zone_info_resp_t;


/*! @brief Get Zone Information Response command, ZCL spec 8.3.2.5.3
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq - sequence
    @param zone_id - Zone ID
    @param zone_type - Zone Type, see @ref zb_zcl_ias_zone_zonetype_e
    @param address - Zone Address
*/
#define ZB_ZCL_IAS_ACE_SEND_GET_ZONE_INFO_RESP(                 \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, seq,      \
    zone_id, zone_type, address)                                \
{                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq,                     \
      ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_RESP_ID);                \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                      \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (zone_type));               \
  ZB_ZCL_PACKET_PUT_DATA_IEEE(ptr, (address));                  \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,         \
      ZB_ZCL_CLUSTER_ID_IAS_ACE, NULL);                         \
}

/** @brief Macro for getting Get Zone Information Response command, ZCL spec 8.3.2.5.3
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_ace_get_zone_info_resp_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_RESP(data_ptr, buffer, status)         \
{                                                                               \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_ace_get_zone_info_resp_t))      \
  {                                                                             \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                      \
  }                                                                             \
  else                                                                          \
  {                                                                             \
    zb_zcl_ias_ace_get_zone_info_resp_t *src_ptr =                              \
         (zb_zcl_ias_ace_get_zone_info_resp_t*)ZB_BUF_BEGIN((buffer));          \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                     \
    (data_ptr)->zone_id = src_ptr->zone_id;                                     \
    ZB_HTOLE16(&((data_ptr)->zone_type), &(src_ptr->zone_type));                \
    ZB_IEEE_ADDR_COPY((data_ptr)->address, (src_ptr)->address);                 \
  }                                                                             \
}

/*! @} */ /* IAS Ace cluster commands */

/*! @} */ /* ZCL IAS Ace cluster definitions */

/** @brief Declare run Arm command for User Application
*/
typedef struct zb_zcl_ias_ace_arm_value_param_s
{
  zb_uint8_t arm_mode;         /*!< Arm Mode */
} zb_zcl_ias_ace_arm_value_param_t;

/** @brief Declare run User Applicatipon for non description IAS ACE command
*/
typedef struct zb_zcl_ias_ace_value_param_s
{
  zb_uint8_t command_id;        /*!< command id */
  zb_uint8_t zone_id_length;    /*!< Length ZoneId array for Bypass command or 0 */
  zb_uint8_t *zone_ids;         /*!< ZoneId array for Bypass command or NULL */
} zb_zcl_ias_ace_value_param_t;

#define ZB_ZCL_IAS_ACE_RUN_BYPASS_USER_APP(buffer, length, zone_id_array, ep) \
{                                                                                       \
  if (ZCL_CTX().device_cb)                                                              \
  {                                                                                     \
    zb_zcl_device_callback_param_t *user_app_data =                                     \
        ZB_GET_BUF_PARAM(buf, zb_zcl_device_callback_param_t);                          \
    user_app_data->cb_param.ace_value_param.command_id = ZB_ZCL_CMD_IAS_ACE_BYPASS_ID;  \
    user_app_data->cb_param.ace_value_param.zone_id_length = length;                    \
    user_app_data->cb_param.ace_value_param.zone_ids = zone_id_array;                   \
    user_app_data->device_cb_id = ZB_ZCL_IAS_ACE_VALUE_CB_ID;                           \
    user_app_data->endpoint = (ep);                                       \
    user_app_data->status = RET_OK;                                                     \
    (ZCL_CTX().device_cb)(param);                                                       \
  }                                                                                     \
}

#define ZB_ZCL_IAS_ACE_RUN_USER_APP(buffer, commandID, ep)             \
{                                                                   \
  if (ZCL_CTX().device_cb)                                          \
  {                                                                 \
    zb_zcl_device_callback_param_t *user_app_data =                 \
        ZB_GET_BUF_PARAM(buf, zb_zcl_device_callback_param_t);      \
    user_app_data->cb_param.ace_value_param.command_id = commandID; \
    user_app_data->cb_param.ace_value_param.zone_id_length = 0;     \
    user_app_data->cb_param.ace_value_param.zone_ids = NULL;        \
    user_app_data->device_cb_id = ZB_ZCL_IAS_ACE_VALUE_CB_ID;       \
    user_app_data->endpoint = (ep);                                   \
    user_app_data->status = RET_OK;                                 \
    (ZCL_CTX().device_cb)(param);                                   \
  }                                                                 \
}

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_ias_ace_init_server();
zb_void_t zb_zcl_ias_ace_init_client();
#define ZB_ZCL_CLUSTER_ID_IAS_ACE_SERVER_ROLE_INIT zb_zcl_ias_ace_init_server
#define ZB_ZCL_CLUSTER_ID_IAS_ACE_CLIENT_ROLE_INIT zb_zcl_ias_ace_init_client

#endif /* ! defined ZB_ZCL_IAS_ACE_H */
