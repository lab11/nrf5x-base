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
 * PURPOSE: Alarms cluster defintions
*/

#if ! defined ZB_ZCL_ALARMS_H
#define ZB_ZCL_ALARMS_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_ALARMS
 * @{
 */

/* Cluster ZB_ZCL_CLUSTER_ID_ALARMS */

/*! @name Alarms cluster attributes
    @{
*/

/*! @brief Alarms cluster attribute identifiers
    @see ZCL spec, subclause 3.11.2.2
*/
enum zb_zcl_alarms_attr_e
{
  /*! @brief AlarmCount attribute */
  ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID = 0x0000,
  ZB_ZCL_ATTR_ALARMS_ALARM_TABLE_ID = 0xfffe
};

/** @brief Default value for AlarmCount attribute */
#define ZB_ZCL_ALARMS_ALARM_COUNT_DEFAULT_VALUE 0


/*! @brief Format of Alarm table
    @see ZCL spec, subclause 3.11.2.2 */
typedef struct zb_zcl_alarms_alarm_table_s
{
  /*! @brief Alarm Code field */
  zb_uint8_t alarm_code;
  /*! @brief Cluster ID field */
  zb_uint16_t cluster_id;
  /*! @brief TimeStamp field */
  zb_uint32_t timestamp;
}  zb_zcl_alarms_alarm_table_t;

/*! @name Color Control cluster internals
    Internal structures for Color Control cluster
    @internal
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID(data_ptr)    \
{                                                                             \
  ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U16,                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                               \
  (zb_voidp_t) data_ptr                                                       \
}


/*!
  Declare mandatory attribute list for Alarms cluster
  @param attr_list - attribure list name

#define ZB_ZCL_DECLARE_ALARMS_ATTRIB_LIST(attr_list)                          \
    zb_uint16_t alarm_conunt_data_ctx_##attr_list = 0;                        \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID,                     \
                       (&(alarm_conunt_data_ctx_##attr_list)))                \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

*/

/*! @} */ /* internal */

/*! @} */ /* Alarms cluster attributes */

/*! @name Alarms cluster commands
    @{
*/

/*! @brief Alarms cluster command identifiers
    @see ZCL spec, subclause 3.11.2.4
*/
enum zb_zcl_alarms_cmd_e
{
  ZB_ZCL_CMD_ALARMS_RESET_ALARM_ID         = 0x00, /**< "Reset alarm" command. */
  ZB_ZCL_CMD_ALARMS_RESET_ALL_ALARMS_ID    = 0x01, /**< "Reset all alarms" command. */
  ZB_ZCL_CMD_ALARMS_GET_ALARM_ID           = 0x02, /**< "Get alarms" command. */
  ZB_ZCL_CMD_ALARMS_RESET_ALARM_LOG_ID     = 0x03  /**< "Reset alarm log" command. */
};

/*! @brief Structured representsation of Reset alarm command payload */
typedef ZB_PACKED_PRE struct zb_zcl_alarms_reset_alarm_req_s
{
  /** Alarm Code field */
  zb_uint8_t alarm_code;
  /** Cluster ID field */
  zb_uint16_t cluster_id;
} ZB_PACKED_STRUCT zb_zcl_alarms_reset_alarm_req_t;

/** @brief Reset alarm payload length macro */
#define ZB_ZCL_ALARMS_RESET_ALARM_REQ_PAYLOAD_LEN \
  sizeof(zb_zcl_alarms_reset_alarm_req_t)

/*! @brief Send Reset alarm command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param cb - callback to call to report send status
    @param alarm_code - Alarm Code field
    @param cluster_id - Cluster ID field
*/
#define ZB_ZCL_ALARMS_SEND_RESET_ALARM_REQ(                                                       \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb, alarm_code, cluster_id) \
{                                                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)                     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_ALARMS_RESET_ALARM_ID);   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (alarm_code));                                                     \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (cluster_id));                                                \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ALARMS, cb);            \
}

/** @brief Parses Reset alarm command and fills in data request
    structure. If request contains invlid data, status will set to ZB_FALSE.
    @param data_buf - pointer to zb_buf_t buffer containing command request data
    @param reset_alarm_req - variable to save command request
    @note data_buf buffer should contain command request payload without ZCL header.
 */
#define ZB_ZCL_ALARMS_GET_RESET_ALARM_REQ(data_buf, reset_alarm_req, status)       \
{                                                                                  \
  zb_zcl_alarms_reset_alarm_req_t *reset_alarm_req_ptr;                            \
  (reset_alarm_req_ptr) = ZB_BUF_LEN(data_buf) >=                                  \
    ZB_ZCL_ALARMS_RESET_ALARM_REQ_PAYLOAD_LEN ?                                    \
    (zb_zcl_alarms_reset_alarm_req_t*)ZB_BUF_BEGIN(data_buf) : NULL;               \
  if (reset_alarm_req_ptr)                                                         \
  {                                                                                \
    reset_alarm_req.alarm_code = reset_alarm_req_ptr->alarm_code;                  \
    ZB_HTOLE16(&(reset_alarm_req).cluster_id, &(reset_alarm_req_ptr->cluster_id)); \
    status = ZB_TRUE;                                                              \
  }                                                                                \
  else                                                                             \
  {                                                                                \
    status = ZB_FALSE;                                                             \
  }                                                                                \
}

/*! @brief Send Reset all alarms command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param cb - callback to call to report send status
*/
#define ZB_ZCL_ALARMS_SEND_RESET_ALL_ALARMS_REQ(                                                     \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb)                            \
{                                                                                                    \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                     \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)                        \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_ALARMS_RESET_ALL_ALARMS_ID); \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                                 \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                         \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ALARMS, cb);               \
}

/*! @brief Send Get alarm command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param cb - callback to call to report send status
*/
#define ZB_ZCL_ALARMS_SEND_GET_ALARM_REQ(                                                     \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb)                     \
{                                                                                             \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                              \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)                 \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_ALARMS_GET_ALARM_ID); \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                          \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                  \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ALARMS, cb);        \
}

/*! @brief Send Reset alarm log command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param cb - callback to call to report send status
*/
#define ZB_ZCL_ALARMS_SEND_RESET_ALARM_LOG_REQ(                                                     \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, command_id, cb)               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)                       \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_ALARMS_RESET_ALARM_LOG_ID); \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                                \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ALARMS, cb);              \
}

/********************************** Responses *******************/

/** @brief Alarms cluster response command identifiers
    @see ZCL spec, subclause 3.6.2.3
*/
enum zb_zcl_alarms_cmd_resp_e
{
  ZB_ZCL_CMD_ALARMS_ALARM_ID               = 0x00,  /**< Alarm command identifier. */
  ZB_ZCL_CMD_ALARMS_GET_ALARM_RES_ID       = 0x01   /**< Get alarm response command identifier. */
};

/*! @brief Structured representsation of Alarm command payload */
typedef zb_zcl_alarms_reset_alarm_req_t zb_zcl_alarms_alarm_res_t;

typedef ZB_PACKED_PRE struct zb_zcl_alarm_get_alarm_hdr_res_s
{
  zb_uint8_t status;         /**< Status field */
} ZB_PACKED_STRUCT zb_zcl_alarm_get_alarm_hdr_res_t;

/** @brief Get alarm response command structure */
typedef ZB_PACKED_PRE struct zb_zcl_alarm_get_alarm_res_s
{
  zb_uint8_t status;         /**< Status field */
  zb_uint8_t alarm_code;     /**< Alarm code field */
  zb_uint16_t cluster_id;    /**< Cluster ID field */
  zb_uint32_t timestamp;     /**< TimeStamp field */
} ZB_PACKED_STRUCT zb_zcl_alarm_get_alarm_res_t;

/** @internal Get alarm response size */
#define ZB_ZCL_ALARM_GET_ALARM_RES_SIZE \
  sizeof(zb_zcl_alarm_get_alarm_res_t)

/** @internal Get alarm response header size */
#define ZB_ZCL_ALARM_GET_ALARM_HDR_RES_SIZE \
  sizeof(zb_zcl_alarm_get_alarm_hdr_res_t)

/*! @brief Send Alarm command (to client)
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param cb - callback to call to report send status
    @param alarm_code - Alarm Code
    @param cluster_id - Cluster Id
*/
#define ZB_ZCL_ALARMS_SEND_ALARM_RES(                                                               \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, cb, alarm_code, cluster_id)                     \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);                                         \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_ALARMS_ALARM_ID);           \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (alarm_code));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (cluster_id));                                                  \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                                \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_ALARMS, cb);              \
}

/** @brief Parses Reset alarm command and fills in data request
    structure. If request contains invalid data, status will set to ZB_FALSE.
  * @param data_ptr - pointer to a variable of type @ref zb_zcl_alarms_alarm_res_t.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
 */
#define ZB_ZCL_ALARMS_GET_ALARM_RES(data_ptr, buffer, status)           \
{                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_alarms_alarm_res_t))        \
  {                                                                     \
    (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                             \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    zb_zcl_alarms_alarm_res_t *src_ptr =                                \
             (zb_zcl_alarms_alarm_res_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                             \
    (data_ptr)->alarm_code = src_ptr->alarm_code;                       \
    ZB_HTOLE16(&((data_ptr)->cluster_id), &(src_ptr->cluster_id));      \
  }                                                                     \
}


/** @brief Parses Get alarm response command and returns response data
    structure or status = ZB_FALSE if request contains invlid data.
    @param data_buf - pointer to zb_buf_t buffer containing command response data
    @param get_alarm_res - command response record
    @note data_buf buffer should contain response command payload without ZCL header
 */
#define ZB_ZCL_ALARMS_GET_GET_ALARM_RES(data_buf, get_alarm_res, status)         \
{                                                                                \
  zb_zcl_alarm_get_alarm_res_t *get_alarm_res_ptr;                               \
  if (ZB_BUF_LEN(data_buf) >= ZB_ZCL_ALARM_GET_ALARM_HDR_RES_SIZE)               \
  {                                                                              \
    (get_alarm_res_ptr) = ZB_BUF_BEGIN(data_buf);                                \
    get_alarm_res.status = get_alarm_res_ptr->status;                            \
    if (ZB_BUF_LEN(data_buf) >= ZB_ZCL_ALARM_GET_ALARM_RES_SIZE)                 \
    {                                                                            \
      get_alarm_res.alarms_code = get_alarm_res_ptr->alarm_code;                 \
      ZB_HTOLE16(&(get_alarm_res).cluster_id, &(get_alarm_res_ptr->cluster_id)); \
      ZB_HTOLE32(&(get_alarm_res).timestamp, &(get_alarm_res_ptr->timestamp));   \
    }                                                                            \
    status = ZB_TRUE;                                                            \
  }                                                                              \
  else                                                                           \
  {                                                                              \
    status = ZB_FALSE;                                                           \
  }                                                                              \
}

/*! @} */ /* Alarms cluster commands */

/*! @internal @name Alarms cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID(data_ptr) \
{                                                                          \
  ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                            \
  (zb_voidp_t) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ALARMS_ALARM_TABLE_ID(data_ptr) \
{                                                                          \
  ZB_ZCL_ATTR_ALARMS_ALARM_TABLE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_NULL,                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                            \
  (zb_voidp_t) data_ptr                                                    \
}


/** @internal @brief Declare attribute list for Alarms cluster
    @param attr_list - attribure list name
    @param alarm_count - pointer to variable to store AlarmCount attribute value
    @param max_alarm_count - Maximum AlarmCount value (from profile)
*/
#define ZB_ZCL_DECLARE_ALARMS_ATTRIB_LIST(attr_list, alarm_count)                                     \
  zb_zcl_alarms_alarm_table_t alarm_table_data_ctx## _attr_list[ZB_ZCL_ALARMS_ALARM_COUNT_MAX_VALUE]; \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ALARMS_ALARM_COUNT_ID, (alarm_count))                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ALARMS_ALARM_TABLE_ID,                                             \
                       (&(alarm_table_data_ctx## _attr_list)))                                        \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Alarms cluster */
#define ZB_ZCL_ALARMS_REPORT_ATTR_COUNT 0


/*! @} */ /* Alarms cluster internals */

/*! @} */ /* ZCL Alarms cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_alarms_init_server();
zb_void_t zb_zcl_alarms_init_client();

#define ZB_ZCL_CLUSTER_ID_ALARMS_SERVER_ROLE_INIT zb_zcl_alarms_init_server
#define ZB_ZCL_CLUSTER_ID_ALARMS_CLIENT_ROLE_INIT zb_zcl_alarms_init_client

#endif /* ! defined ZB_ZCL_ALARMS_H */
