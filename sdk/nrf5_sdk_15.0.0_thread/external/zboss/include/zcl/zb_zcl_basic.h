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
 * PURPOSE: ZCL Basic Cluster definitions
*/
#if ! defined ZB_ZCL_BASIC_H
#define ZB_ZCL_BASIC_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_BASIC
  * @{
  *   @details
  *   According to ZCL spec, clause 3.2, Basic cluster contains attributes only. These attributes
  *   could be queried with @ref ZB_ZCL_COMMANDS "general ZCL commands".
  */

/** @name Basic cluster attributes
  * @{
  */

/** @brief Basic cluster information attribute set identifiers
    @see ZCL spec, subclauses 3.2.2.2.1 to 3.2.2.2.9
*/
enum zb_zcl_basic_info_attr_e
{
  /*! ZCL version attribute */
  ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID          = 0x0000,
  /*! Application version attribute */
  ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID  = 0x0001,
  /*! Stack version attribute */
  ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID        = 0x0002,
  /*! Hardware version attribute */
  ZB_ZCL_ATTR_BASIC_HW_VERSION_ID           = 0x0003,
  /*! Manufacturer name attribute */
  ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID    = 0x0004,
  /*! Model identifier attribute */
  ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID     = 0x0005,
  /*! Date code attribute */
  ZB_ZCL_ATTR_BASIC_DATE_CODE_ID            = 0x0006,
  /*! Power source attribute */
  ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID         = 0x0007,
};

/** @brief Basic cluster settings attribute set identifiers
    @see ZCL spec, subclauses 3.2.2.2.10 to 3.2.2.2.15
*/
enum zb_zcl_basic_settings_attr_e
{
  /*! Location description attribute */
  ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID = 0x0010,
  /*! Physical environment attribute */
  ZB_ZCL_ATTR_BASIC_PHYSICAL_ENVIRONMENT_ID = 0x0011,
  /*! Device enabled attribute */
#ifndef ZB_DISABLE_DEVICE_ENABLED_ATTR
  ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID       = 0x0012,
#else
  ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID       = 0xF012,
#endif
  /*! Alarm mask attribute */
  ZB_ZCL_ATTR_BASIC_ALARM_MASK_ID           = 0x0013,
  /*! Disable local config attribute */
  ZB_ZCL_ATTR_BASIC_DISABLE_LOCAL_CONFIG_ID = 0x0014
};

/**
 *  @brief Permitted values for "Power source" attribute.
 *  @see ZCL spec, subclauses 3.2.2.2.9.
 */
enum zb_zcl_basic_power_source_e
{
  ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN                = 0x00,  /**< Power source unknown. */
  ZB_ZCL_BASIC_POWER_SOURCE_MAINS_SINGLE_PHASE     = 0x01,  /**< Single-phase mains. */
  ZB_ZCL_BASIC_POWER_SOURCE_MAINS_THREE_PHASE      = 0x02,  /**< 3-phase mains. */
  ZB_ZCL_BASIC_POWER_SOURCE_BATTERY                = 0x03,  /**< Battery source. */
  ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE              = 0x04,  /**< DC source. */
  ZB_ZCL_BASIC_POWER_SOURCE_EMERGENCY_MAINS_CONST  = 0x05,  /**< Emergency mains constantly
                                                              powered. */
  ZB_ZCL_BASIC_POWER_SOURCE_EMERGENCY_MAINS_TRANSF = 0x06   /**< Emergency mains and transfer
                                                              switch. */
};

/** @brief Basic permitted values for "Physical environment" attribute.
    @note most of values are profile-specific.
    @see ZCL spec, subclause 3.2.2.2.12
*/
enum zb_zcl_basic_physical_env_e
{
  /*! Environment unspecified */
  ZB_ZCL_BASIC_ENV_UNSPECIFIED = 0,
  /*! Environment unknown */
  ZB_ZCL_BASIC_ENV_UNKNOWN     = 0xff
};

/** @brief Permitted bits for "Alarm mask" attribute
    @see ZCL spec, subclause 3.2.2.2.14
*/
enum zb_zcl_basic_alarm_mask_e
{
  /*! General hardware fault */
  ZB_ZCL_BASIC_ALARM_MASK_HW_FAULT = 0,
  /*! General software fault */
  ZB_ZCL_BASIC_ALARM_MASK_SW_FAULT = 1
};

/** @brief Value structure for "Disable local config" attribute
    @see ZCL spec, subclause 3.2.2.2.15
*/
typedef struct zb_zcl_basic_disable_local_conf_s
{
  /*! Reset to factry defaults disabled */
  zb_bitfield_t reset_disabled  : 1;
  /*! Device configuration enabled */
  zb_bitfield_t config_enabled  : 1;
  /*! Reserved bits */
  zb_bitfield_t reserved        : 6;
} zb_zcl_basic_disable_local_conf_t;

/** @brief Default value for ZCL version attribute */
#define ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE ((zb_uint8_t)0x02)

/** @brief Default value for Application version attribute */
#define ZB_ZCL_BASIC_APPLICATION_VERSION_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for Stack version attribute */
#define ZB_ZCL_BASIC_STACK_VERSION_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for Hardware version attribute */
#define ZB_ZCL_BASIC_HW_VERSION_DEFAULT_VALUE ((zb_uint8_t)0x00)

/** @brief Default value for Manufacturer name attribute */
#define ZB_ZCL_BASIC_MANUFACTURER_NAME_DEFAULT_VALUE {0}

/** @brief Default value for Model identifier attribute */
#define ZB_ZCL_BASIC_MODEL_IDENTIFIER_DEFAULT_VALUE {0}

/** @brief Default value for Date code attribute */
#define ZB_ZCL_BASIC_DATE_CODE_DEFAULT_VALUE {0}

/** @brief Default value for Power source attribute */
#define ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE (ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN)

/** @brief Default value for location description attribute */
#define ZB_ZCL_BASIC_LOCATION_DESCRIPTION_DEFAULT_VALUE {0}

/** @brief Default value for Physical environment attribute */
#define ZB_ZCL_BASIC_PHYSICAL_ENVIRONMENT_DEFAULT_VALUE (ZB_ZCL_BASIC_ENV_UNSPECIFIED)

/** @brief Default value for Device enabled attribute */
#define ZB_ZCL_BASIC_DEVICE_ENABLED_DEFAULT_VALUE ((zb_uint8_t)0x01)

/** @brief Default value for Alarm mask attribute */
#define ZB_ZCL_BASIC_ALARM_MASK_DEFAULT_VALUE (ZB_ZCL_BASIC_ALARM_MASK_HW_FAULT)

/** @brief Default value for Disable local config attribute */
#define ZB_ZCL_BASIC_DISABLE_LOCAL_CONFIG_DEFAULT_VALUE ((zb_zcl_basic_disable_local_conf_t)0x00)


/** @cond internals_doc */

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID(data_ptr) \
{                                                                         \
  ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                           \
  (zb_voidp_t) data_ptr                                                   \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID(data_ptr) \
{                                                                                 \
  ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                            \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                   \
  (zb_voidp_t) data_ptr                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID(data_ptr) \
{                                                                           \
  ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                      \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                             \
  (zb_voidp_t) data_ptr                                                     \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_HW_VERSION_ID(data_ptr) \
{                                                                        \
  ZB_ZCL_ATTR_BASIC_HW_VERSION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                          \
  (zb_voidp_t) data_ptr                                                  \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID(data_ptr) \
{                                                                               \
  ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID,                                       \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                 \
  (zb_voidp_t) data_ptr                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID(data_ptr) \
{                                                                              \
  ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID,                                       \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                \
  (zb_voidp_t) data_ptr                                                        \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_DATE_CODE_ID(data_ptr) \
{                                                                       \
  ZB_ZCL_ATTR_BASIC_DATE_CODE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                         \
  (zb_voidp_t) data_ptr                                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID(data_ptr) \
{                                                                          \
  ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                            \
  (zb_voidp_t) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID(data_ptr) \
{                                                                                  \
  ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                   \
  (zb_voidp_t) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_PHYSICAL_ENVIRONMENT_ID(data_ptr) \
{                                                                                  \
  ZB_ZCL_ATTR_BASIC_PHYSICAL_ENVIRONMENT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                      \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                   \
  (zb_voidp_t) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID(data_ptr) \
{                                                                            \
  ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                                       \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                             \
  (zb_voidp_t) data_ptr                                                      \
}

/** @internal @brief Declare attribute list for Basic cluster
    @param attr_list - attribute list name
    @param zcl_version - pointer to variable to store zcl version  attribute value
    @param power_source - pointer to variable to store power source attribute value
*/
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(attr_list, zcl_version, power_source) \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))      \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))    \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


#if defined ZB_ZCL_SUPPORT_CLUSTER_SCENES
/*! Scenes fieldset length for Basic cluster */
#define ZB_ZCL_CLUSTER_ID_BASIC_SCENE_FIELD_SETS_LENGTH 0
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_SCENES */

/** @endcond */ /* internals_doc */


/**
 *  @brief Basic cluster attributes
 */
typedef struct zb_zcl_basic_attrs_s
{
  /** @copydoc ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID
   * @see ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID
   */
  zb_uint8_t zcl_version;

  /** @copydoc ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID
   * @see ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID
   */
  zb_uint8_t power_source;
} zb_zcl_basic_attrs_t;


/** @brief Declare attribute list for Basic cluster cluster
 *  @param[in]  attr_list - attribute list variable name
 *  @param[in]  attrs - pointer to @ref zb_zcl_basic_attrs_s structure
 */
#define ZB_ZCL_DECLARE_BASIC_ATTR_LIST(attr_list, attrs) \
  ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(attr_list, &attrs.zcl_version, &attrs.power_source)

/** @} */ /* Basic cluster attributes */

/*! @name Alarms cluster commands
    @{
*/

/*! @brief Basic cluster command identifiers
    @see ZCL spec, subclause 3.2.2.3
*/
enum zb_zcl_basic_cmd_e
{
  ZB_ZCL_CMD_BASIC_RESET_ID       = 0x00, /**< "Reset to Factory Defaults" command. */
};

////////////////////////////////////////

/*! @brief Send Reset to Factory Defaults command
    @param buffer to put data to
    @param addr - address of the device to send command to
    @param dst_addr_mode - addressing mode
    @param dst_ep destination endpoint
    @param ep - current endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - "Disable default response" flag
    @param cb - callback to call to report send status
*/
#define ZB_ZCL_BASIC_SEND_RESET_REQ(                                                      \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb)                 \
{                                                                                         \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                          \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_BASIC_RESET_ID);  \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                      \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                              \
      buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_BASIC, cb);     \
}


/** Call device callback in user application to reset application settings to defaults.
 * @param[in] buffer - pointer to buffer for callback
 * @param[in] ep - current endpoint
 * @param[out] result - callback status
 */
#define ZB_ZCL_RESET_TO_FACTORY_DEFAULTS_USER_APP(buffer, ep, result) \
{                                                                   \
  TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_RESET_TO_FACTORY_DEFAULTS_USER_APP", (FMT__0)); \
  if (ZCL_CTX().device_cb)                                          \
  {                                                                 \
    zb_zcl_device_callback_param_t *user_app_data =                 \
      ZB_GET_BUF_PARAM(buffer, zb_zcl_device_callback_param_t);     \
    user_app_data->device_cb_id = ZB_ZCL_BASIC_RESET_CB_ID;         \
    user_app_data->endpoint = (ep);                                   \
    user_app_data->status = RET_OK;                                 \
    (ZCL_CTX().device_cb)(param);                                   \
    result = user_app_data->status;                                 \
  }                                                                 \
}

/** @} */ /* Basic cluster commands */

/*! @} */ /* ZCL Basic cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_basic_init_server();
zb_void_t zb_zcl_basic_init_client();

#define ZB_ZCL_CLUSTER_ID_BASIC_SERVER_ROLE_INIT zb_zcl_basic_init_server
#define ZB_ZCL_CLUSTER_ID_BASIC_CLIENT_ROLE_INIT zb_zcl_basic_init_client

#endif /* ! defined ZB_ZCL_BASIC_H */
