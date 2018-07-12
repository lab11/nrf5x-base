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
 * PURPOSE: Diagnostics cluster defintions
*/

#if ! defined ZB_ZCL_DIAGNOSTICS_H
#define ZB_ZCL_DIAGNOSTICS_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_DIAGNOSTICS
 *  @{
 *    @details
 *    ZCL Diagnostics cluster definitions
 */


/* Cluster ZB_ZCL_CLUSTER_ID_DIAGNOSTICS */

/*! @name Diagnostics cluster attributes
    @{
*/

/*! @brief Structure saved all diagnostic attribute */
typedef struct zb_zcl_diagnostics_data_s
{
  /*! @brief numberOfResets, Zigbee Diagnostic Cluster spec 1.2.2.1.1 */
  zb_uint16_t numberOfResets;
  /*! @brief MacRxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macRxBcast;
  /*! @brief MacTxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macTxBcast;
  /*! @brief MacTxUcast, HA spec 9.3.2.2.2 */
  zb_uint16_t macTxUcast;
  /*! @brief APSTxBcast, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxBcast;
  /*! @brief APSTxUcastSuccess, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxUcastSuccess;
  /*! @brief APSTxUcastzb_zcl_diagnostics_data_t diagnostics_data;Fail, HA spec 9.3.2.2.2 */
  zb_uint16_t apsTxUcastFail;
  /*! @brief JoinIndication, Zigbee Diagnostic Cluster spec 1.2.2.2.17 */
  zb_uint16_t joinIndication;
  /*! @brief AverageMACRetryPerAPSMessageSent, HA spec 9.3.2.2.2.27 */
  zb_uint16_t averageMACRetryPerAPSMessageSent;
  /*! @brief JoinIndication, Zigbee Diagnostic Cluster spec 1.2.2.2.24  */
  zb_uint16_t packetBufferAllocateFeatures;
  /*! @brief LastMessageLQI, HA spec 9.3.2.2.2. */
  zb_uint8_t lastMessageLQI;
  /*! @brief LastMessageRSSI, HA spec 9.3.2.2.2. */
  zb_uint8_t lastMessageRSSI;
} zb_zcl_diagnostics_data_t;




extern zb_zcl_diagnostics_data_t diagnostics_data;


/*! @brief Diagnostics cluster attribute identifiers
    @see HA spec, Diagnostics Cluster 9.3.2.2.2
*/
enum zb_zcl_diagnostics_attr_e
{
  /*! @brief NumberOfResets, Zigbee Diagnostic Cluster spec 1.2.2.1.1 */
  ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID              = 0x0000,
  /*! @brief MacRxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID               = 0x0100,
  /*! @brief MacTxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID               = 0x0101,
  /*! @brief MacTxUcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID               = 0x0103,
  /*! @brief APSTxBcast, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID               = 0x0107,
  /*! @brief APSTxUcastSuccess, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID       = 0x0109,
  /*! @brief APSTxUcastFail, HA spec 9.3.2.2.2 */
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID          = 0x010b,
  /*! @brief JoinIndication, HA spec 1.2.2.2.17 */
  ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID               = 0x0110,
  /*! @brief AverageMACRetryPerAPSMessageSent, HA spec 9.3.2.2.2.27 */
  ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID  = 0x011b,
  /*! @brief PacketBufferAllocateFeatures, Zigbee Diagnostic Cluster spec 1.2.2.2.24 */
  ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FEATURES_ID = 0x0117,
  /*! @brief LastMessageLQI, HA spec 9.3.2.2.2. */
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID                   = 0x011c,
  /*! @brief LastMessageRSSI, HA spec 9.3.2.2.2. */
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID                  = 0x011d,
};

/*! @internal @name Diagnostics cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID,                  \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FEATURES_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FEATURES_ID,   \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID,                   \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U32,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID,                      \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID,              \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID,                 \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID,         \
  ZB_ZCL_ATTR_TYPE_U16,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID,                          \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID,                         \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

/** @internal @brief Declare attribute list for Diagnostics cluster - server side
    @param attr_list - attribure list name
*/
#define ZB_ZCL_DECLARE_DIAGNOSTICS_ATTRIB_LIST(attr_list)                                         \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_NUMBER_OF_RESETS_ID,                               \
                       &(diagnostics_data.numberOfResets))                                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_RX_BCAST_ID, &(diagnostics_data.macRxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_BCAST_ID, &(diagnostics_data.macTxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_MAC_TX_UCAST_ID, &(diagnostics_data.macTxUcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_BCAST_ID, &(diagnostics_data.apsTxBcast))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_SUCCESS_ID,                           \
                                        &(diagnostics_data.apsTxUcastSuccess))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_APS_TX_UCAST_FAIL_ID,                              \
                                        &(diagnostics_data.apsTxUcastFail))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_PACKET_BUFFER_ALLOCATE_FEATURES_ID,                \
                                        &(diagnostics_data.packetBufferAllocateFeatures))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_JOIN_INDICATION_ID,                                \
                                        &(diagnostics_data.joinIndication))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_AVERAGE_MAC_RETRY_PER_APS_ID,                      \
                                        &(diagnostics_data.packetBufferAllocateFeatures))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_LQI_ID, &(diagnostics_data.lastMessageLQI))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DIAGNOSTICS_LAST_RSSI_ID, &(diagnostics_data.lastMessageRSSI)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Diagnostics cluster internals */

/*! @} */ /* Diagnostics cluster attributes */

/*! @name Diagnostics cluster commands
    @{
*/

/*! @brief Diagnostics cluster command identifiers
    @see Diagnostics cluster spec
*/
enum zb_zcl_diagnostics_cmd_e
{
  ZB_ZCL_CMD_DIAGNOSTICS_CLEAR_ID  = 0x00,  /**< Clear diagnostic attributes */
};

/******************************* Clear diagnostic attributes ******************************/

/*! @brief Structure representsation of Clear diagnostic command */
typedef ZB_PACKED_PRE struct zb_zcl_diagnostics_clear_s
{
  /** Number of attributes */
  zb_uint8_t length;
  /** Attribute ID array */
  zb_uint16_t attributes[1];  // non fixed size

} ZB_PACKED_STRUCT zb_zcl_diagnostics_clear_t;


/*! @brief Start Clear diagnostic command
    @param buffer - to put packet to
    @param length - Attribute ID number
    @param def_resp - enable/disable default response
    @param ptr - [out] (zb_uint8_t*) current position for @ref ZB_ZCL_DIAGNOSTICS_CLEAR_REQ_ADD
    and @ref ZB_ZCL_DIAGNOSTICS_CLEAR_REQ_END
*/
#define ZB_ZCL_DIAGNOSTICS_CLEAR_REQ_START(buffer, def_resp, length, ptr) \
{                                                                         \
  (ptr) = ZB_ZCL_START_PACKET_REQ(buffer)                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),              \
      ZB_ZCL_CMD_DIAGNOSTICS_CLEAR_ID);                                   \
  ZB_ZCL_PACKET_PUT_DATA8((ptr), (length));                               \
}

/*! @brief Add Clear diagnostic command
    @param ptr - [in/out] (zb_uint8_t*) current position
    @param attr_id - Attribute ID
*/
#define ZB_ZCL_DIAGNOSTICS_CLEAR_REQ_ADD(ptr, attr_id)  \
{                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (attr_id));         \
}

/*! @brief Clear diagnostic command
    @param ptr - (zb_uint8_t*) current position
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_DIAGNOSTICS_CLEAR_REQ_END(                         \
    ptr, buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb)    \
{                                                                 \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                      \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,           \
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS, cb);                         \
}

/** @brief Macro for getting Clear diagnostic command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_diagnostics_clear_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_DIAGNOSTICS_GET_CLEAR_REQ(data_ptr, buffer, status)        \
{                                                                         \
  zb_zcl_diagnostics_clear_t *src_ptr =                                   \
       (zb_zcl_diagnostics_clear_t*)ZB_BUF_BEGIN((buffer));               \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_uint16_t)*src_ptr->length+sizeof(zb_uint8_t)) \
  {                                                                       \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                \
  }                                                                       \
  else                                                                    \
  {                                                                       \
    zb_uint16_t value, i;                                                 \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                               \
    (data_ptr) = src_ptr;                                                 \
    for(i=0; i<src_ptr->length; i++)                                      \
    {                                                                     \
      value = src_ptr->attributes[i];                                     \
      ZB_HTOLE16(&(src_ptr->attributes[i]), &value);                      \
    }                                                                     \
  }                                                                       \
}

/*! @} */ /* Diagnostics cluster commands */

/*! @brief Increment diagnostic attribute
    @param attr_id - attribute Id, see @ref zb_zcl_diagnostics_attr_e
*/
zb_void_t zb_zcl_diagnostics_inc(zb_uint16_t attr_id, zb_uint8_t value);

#if defined ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS && defined ZB_ENABLE_ZCL
#define ZB_ZCL_DIAGNOSTICS_INC(attr_id)           zb_zcl_diagnostics_inc((attr_id), 0)
#define ZB_ZCL_DIAGNOSTICS_SAVE(attr_id, value)   zb_zcl_diagnostics_inc((attr_id), (value))
#else /* defined ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS && defined ZB_ENABLE_ZCL */
#define ZB_ZCL_DIAGNOSTICS_INC(attr_id)
#define ZB_ZCL_DIAGNOSTICS_SAVE(attr_id, value)
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS && defined ZB_ENABLE_ZCL */

/*! @} */ /* ZCL Diagnostics cluster definitions */

#if defined ZB_ENABLE_HA
zb_uint8_t zb_zcl_get_cmd_list_diagnostics(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);
#endif /* defined ZB_ENABLE_HA */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_diagnostics_init_server();
zb_void_t zb_zcl_diagnostics_init_client();
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_SERVER_ROLE_INIT zb_zcl_diagnostics_init_server
#define ZB_ZCL_CLUSTER_ID_DIAGNOSTICS_CLIENT_ROLE_INIT zb_zcl_diagnostics_init_client

#endif /* ! defined ZB_ZCL_DIAGNOSTICS_H */
