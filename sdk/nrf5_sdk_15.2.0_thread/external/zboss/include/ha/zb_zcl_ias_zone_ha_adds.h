/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
PURPOSE: IAS Zone cluster defintions - extension for HA
*/

#if ! defined ZB_ZCL_IAS_ZONE_HA_ADDS_H
#define ZB_ZCL_IAS_ZONE_HA_ADDS_H

#include "zcl/zb_zcl_ias_zone.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_IAS_ZONE_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
 *  @{
 *    @details
 *    HA extensions for IAS Zone cluster
 */

/* Cluster ZB_ZCL_CLUSTER_ID_IAS_ZONE_HA_ADDS */

/*! @name IAS Zone cluster attributes IAS Zone HA extension
    @{
*/

/*! @brief IAS Zone cluster - extension for HA attribute identifiers
    @see HA spec, IAS Zone Cluster 10.7.4.2.2.1
*/
enum zb_zcl_ias_zone_attr_ha_e
{
  /*! @brief ZoneID attribute, HA spec 10.7.4.2.2 */
  ZB_ZCL_ATTR_IAS_ZONE_ZONEID_HA_ID = 0x0011,
  /*! @brief NumberOfZoneSensitivityLevelsSupported attribute, Errata to 05-3520-29 Spec, 1.1.23.1.1.1 */
  ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID = 0x0012,
  /*! @brief CurrentZoneSensitivityLevel attribute, Errata to 05-3520-29 Spec, 1.1.23.1.1.2 */
  ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID = 0x0013,

/* TODO: move this attribute to IAS Zone ZCL implementation */
  /*! @brief Struct with pointers on User App callbacks */
  ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID = 0xfffe,
};

/** @brief IAS Zone ZoneID attribute default value */
#define ZB_ZCL_ATTR_IAS_ZONE_ID_HA_ID_DEF_VALUE            0xff

/** @brief IAS Zone NumberOfZoneSensitivityLevelsSupported attribute min value */
#define ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_MIN_VALUE 0x02

/* IAS Zone NumberOfZoneSensitivityLevelsSupported attribute max value is 0xff */

/** @brief IAS Zone NumberOfZoneSensitivityLevels attribute default value */
#define ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_DEFAULT_VALUE \
   ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_MIN_VALUE

/** @brief IAS Zone CurrentZoneSensitivityLevel attribute default value */
#define ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_DEFAULT_VALUE 0


/*! @brief IAS Zone ZoneStatus extension for HA attribute flags
    @see HA spec 10.7.4.2.1.1
*/
enum zb_zcl_ias_zone_zonestatus_ha_e
{
  /** Test */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_TEST            = 1 << 8,
  /** Battery Defect */
  ZB_ZCL_IAS_ZONE_ZONE_STATUS_BATTERY_DEFECT  = 1 << 9,
};

/*! @internal @name IAS Zone cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_ZONEID_HA_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_ZONEID_HA_ID,                    \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID,                      \
  ZB_ZCL_ATTR_TYPE_U8,                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID,                      \
  ZB_ZCL_ATTR_TYPE_U8,                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                         \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID,                      \
  ZB_ZCL_ATTR_TYPE_NULL,                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                         \
  (zb_voidp_t) data_ptr                                 \
}

/** @internal @brief Declare attribute list for IAS Zone cluster - server side
    @param attr_list - attribure list name
    @param zone_state - pointer to variable to store ZoneState attribute
    @param zone_type - pointer to variable to store ZoneType attribute
    @param zone_status - pointer to variable to store ZoneStatus attribute
    @param ias_cie_address - pointer to variable to store IAS-CIE address attribute
    @param zone_id - pointer to variable to store Zone ID attribute
    @param number_of_zone_sens_levels_supported - pointer to variable to store
    NumberOfZoneSensitivityLevelsSupported attribute
    @param current_zone_sens_level - pointer to variable to store CurrentZoneSensitivityLevel attribute
    @param cie_short_addr - custom attribute to store CIE short address
    @param cie_ep - custom attribute to store CIE Endpoint number
*/
/* FIXME: declare custom attributes internally */
#define ZB_ZCL_DECLARE_IAS_ZONE_ATTRIB_LIST_HA(                                                                  \
  attr_list, zone_state, zone_type, zone_status, number_of_zone_sens_levels_supported, current_zone_sens_level,  \
  ias_cie_address, zone_id, cie_short_addr, cie_ep)                                                              \
  zb_uint16_t last_change_##attr_list;                                                                           \
  zb_zcl_ias_zone_int_ctx_t int_ctx_##attr_list;                                                                 \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID, (zone_state))                                          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONETYPE_ID, (zone_type))                                            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID, (zone_status))                                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID, (ias_cie_address))                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_ZONEID_HA_ID, (zone_id))                                             \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID,                      \
                       (number_of_zone_sens_levels_supported))                                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID,                                   \
                       (current_zone_sens_level))                                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID, &(int_ctx_##attr_list))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR, (cie_short_addr))                                      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_CIE_EP, (cie_ep))                                                      \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* IAS Zone cluster internals */

/*! @} */ /* IAS Zone cluster attributes */

/*! @name IAS Zone cluster commands - extension for HA
    @{
*/

enum zb_zcl_ias_zone_cmd_ha_e
{
  /**< "Initiate Normal Operation Mode" command, Errata to 05-3520-29 Spec, 1.1.23.2.1 */
  ZB_ZCL_CMD_IAS_ZONE_INITIATE_NORMAL_OPERATION_MODE_ID        = 0x01,
  /**< "Initiate Test Mode" command, Errata to 05-3520-29 Spec, 1.1.23.2.2 */
  ZB_ZCL_CMD_IAS_ZONE_INITIATE_TEST_MODE_ID        = 0x02,
};

/*! @brief Structure representsation of Initiate Test Mode - Errata to 05-3520-29 Spec, 1.1.23.2.2.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_zone_init_test_mode_ha_s
{
  /** Test mode duration */
  zb_uint8_t test_mode_duration;
  /** Current Zone Sensitivity Level */
  zb_uint8_t current_zone_sens_level;
} ZB_PACKED_STRUCT zb_zcl_ias_zone_init_test_mode_ha_t;

/** @brief Macro for getting "Initiate Test Mode" command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_zone_init_test_mode_ha_t.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ZONE_GET_INITIATE_TEST_MODE_REQ(data_ptr, buffer, status) \
{                                                                            \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_zone_init_test_mode_ha_t))   \
  {                                                                          \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                   \
  }                                                                          \
  else                                                                       \
  {                                                                          \
    zb_zcl_ias_zone_init_test_mode_ha_t *src_ptr =                           \
         (zb_zcl_ias_zone_init_test_mode_ha_t*)ZB_BUF_BEGIN((buffer));       \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                  \
    (data_ptr)->test_mode_duration = src_ptr->test_mode_duration;            \
    (data_ptr)->current_zone_sens_level = src_ptr->current_zone_sens_level;  \
  }                                                                          \
}

/*! @brief Initiate Test Mode command - HA extension, see HA errata 1.1.23.2.2
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
    @param test_mode_duration - Test Mode duration
    @param current_zone_sens_level - CurrentZoneSensitivityLevel
*/
#define ZB_ZCL_IAS_ZONE_SEND_INITIATE_TEST_MODE_REQ(                                            \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb,                                       \
    test_mode_duration, current_zone_sens_level)                                                \
{                                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                             \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE)      \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                                \
      ZB_ZCL_CMD_IAS_ZONE_INITIATE_TEST_MODE_ID);                                               \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (test_mode_duration));                                           \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (current_zone_sens_level));                                      \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                           \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,                                         \
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, cb);                                                          \
}

/*! @brief Initiate Normal Operation Mode command - HA extension, see HA errata 1.1.23.2.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_IAS_ZONE_SEND_INITIATE_NORMAL_OPERATION_MODE_REQ(                                \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb)                                       \
{                                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                             \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE)      \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                                \
      ZB_ZCL_CMD_IAS_ZONE_INITIATE_NORMAL_OPERATION_MODE_ID);                                   \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                           \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                    \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,                                         \
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, cb);                                                          \
}

/******************************* Zone Status Change Notification ******************************/

/*! @brief Structure representsation of Zone Status Change Notification - HA extension
 * see HA spec 10.7.4.3.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_zone_status_change_not_ha_s
{
  /** Zone Status, see @ref zb_zcl_ias_zone_zonestatus_e*/
  zb_uint16_t zone_status;
  /** Extended Status */
  zb_uint8_t extended_status;

  /** Zone ID*/
  zb_uint8_t zone_id;
  /** Delay */
  zb_uint16_t delay;
} ZB_PACKED_STRUCT zb_zcl_ias_zone_status_change_not_ha_t;


/*! @brief Zone Status Change Notification command - HA extension, see HA spec 10.7.4.3.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param cb - callback for getting command send status
    @param zone_status - Zone Status, see @ref zb_zcl_ias_zone_zonestatus_e
    @param extended_status - Extended Status
    @param zone_id - Zone ID
    @param delay - Delay
*/
#define ZB_ZCL_IAS_ZONE_SEND_STATUS_CHANGE_NOTIFICATION_HA_REQ(             \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, cb,                   \
    zone_status, extended_status, zone_id, delay )                          \
{                                                                           \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                            \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);                 \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),                \
      ZB_ZCL_CMD_IAS_ZONE_ZONE_STATUS_CHANGE_NOT_ID);                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (zone_status));                         \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (extended_status));                          \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (zone_id));                                  \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (delay));                               \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                       \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,                     \
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, cb);                                      \
}

/** @brief Macro for getting Zone Status Change Notification command - HA extension, see HA spec 10.7.4.3.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_zone_status_change_not_ha_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_ZONE_GET_STATUS_CHANGE_NOTIFICATION_HA_REQ(data_ptr, buffer, status) \
{                                                                             \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_zone_status_change_not_ha_t)) \
  {                                                                           \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                    \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    zb_zcl_ias_zone_status_change_not_ha_t *src_ptr =                         \
         (zb_zcl_ias_zone_status_change_not_ha_t*)ZB_BUF_BEGIN((buffer));     \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                   \
    ZB_HTOLE16(&((data_ptr)->zone_status), &(src_ptr->zone_status));          \
    (data_ptr)->extended_status = src_ptr->extended_status;                   \
    (data_ptr)->zone_id = src_ptr->zone_id;                                   \
    ZB_HTOLE16(&((data_ptr)->delay), &(src_ptr->delay));                      \
  }                                                                           \
}

/*! @} */ /* IAS Zone cluster commands - HA extension */

/*! @name User App for queue Status Change Notification command
    @{
*/

/*! @} */ /* User App for queue Status Change Notification command */

/*! @} */ /* ZCL IAS Zone cluster definitions - HA extension */

zb_uint8_t zb_zcl_get_cmd_list_ias_zone(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_IAS_ZONE_HA_ADDS_H */
