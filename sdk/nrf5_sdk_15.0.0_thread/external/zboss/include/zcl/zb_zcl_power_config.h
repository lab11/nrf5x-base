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
 * PURPOSE: Power Configuration cluster defintions
*/

#if ! defined ZB_ZCL_POWER_CONFIG_H
#define ZB_ZCL_POWER_CONFIG_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_POWER_CONFIG
 *  @{
 */

/**
 * @brief Hook on Write attribute
 * @param endpoint - endpoint number
 * @param attr_id - ID of attribute being written
 * @param new_value - pointer to new value of attribute
 */
zb_void_t zb_zcl_power_config_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value);

/* Cluster ZB_ZCL_CLUSTER_ID_POWER_CONFIG */

/*! @name Power Configuration cluster attributes
    @{
*/

/*! @brief Power Configuration cluster attribute identifiers
    @see ZCL spec, Power Configuration Cluster 3.3.2.2
*/
enum zb_zcl_power_config_attr_e
{
  /*! @brief BatteryVoltage attribute, ZCL spec 3.3.2.2.3.1 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID = 0x0020,

  /*! @brief BatterySize attribute, ZCL spec 3.3.2.2.4.2 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID = 0x0031,
  /*! @brief BatteryQuantity attribute, ZCL spec 3.3.2.2.4.4 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID = 0x0033,
  /*! @brief BatteryRatedVoltage attribute, ZCL spec 3.3.2.2.4.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID = 0x0034,
  /*! @brief BatteryAlarmMask attribute, ZCL spec 3.3.2.2.4.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID = 0x0035,
  /*! @brief BatteryVoltageMinThreshold attribute, ZCL spec 3.3.2.2.4.7 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID = 0x0036,

  /* Custom attribures */
  ZB_ZCL_ATTR_POWER_CONFIG_CUSTOM_BATTERY_VOLTAGE_SHARP_ID = 0x8020
};

/** @brief BatteryVoltage attribute invalid value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_INVALID         0xff

/** @brief BatterySize attribute default value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_DEF_VALUE          0xff

/** @brief BatteryAlarmMask attribute default value */
#define ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_DEF_VALUE    0x00

/**
 * @brief Power Configuration BatterySize value
 * see ZCL spec 3.3.2.2.4.2
 */
enum zb_zcl_power_config_battery_size_e
{
  /** @brief BatterySize - no battery*/
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_NO_BATTERY = 0,
  /** @brief BatterySize - built in */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_BUILT_IN   = 1,
  /** @brief BatterySize - other */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_OTHER      = 2,
  /** @brief BatterySize - AA */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_AA         = 3,
  /** @brief BatterySize - AAA */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_AAA        = 4,
  /** @brief BatterySize - C */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_C          = 5,
  /** @brief BatterySize - D */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_D          = 6,
  /** @brief BatterySize - unknown */
    ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_UNKNOWN    = 0xff,
};

/**
 * @brief Power Configuration BatteryAlarmMask value
 * see ZCL spec 3.3.2.2.4.7
 */
enum zb_zcl_power_config_battery_alarm_mask_e
{
  /** @brief BatteryAlarmMask - Battery voltage too low */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_VOLTAGE_LOW = 0,
};

/*! @internal @name Power Configuration cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID,                  \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID,             \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID,         \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

/* DA: HA12 change */
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID,       \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                             \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

/** @internal @brief Declare attribute list for Power Configuration cluster - server side
    @param attr_list - attribure list name
    @param voltage - pointer to variable to store BatteryVoltage attribute
    @param size - pointer to variable to store BatterySize attribute
    @param quantity - pointer to variable to store BatteryQuantity attribute
    @param rated_voltage - pointer to variable to store BatteryRatedVoltage attribute
    @param alarm_mask - pointer to variable to store BatteryAlarmMask attribute
    @param voltage_min_threshold - pointer to variable to store BatteryVoltageMinThreshold attribute
*/
#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST(attr_list,                                  \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold)              \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID, (voltage))              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID, (size))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID, (quantity))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID, (rated_voltage))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID, (alarm_mask ))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID, (voltage_min_threshold))  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


/*! @internal Number of attributes mandatory for reporting in Power Configuration cluster */
#define ZB_ZCL_POWER_CONFIG_REPORT_ATTR_COUNT 1

/*! @internal Alarm Code for Power Configuration cluster */
#define ZB_ZCL_POWER_CONFIG_ALARM_CODE    0x10

/*! @} */ /* Power Configuration cluster internals */

/*! @} */ /* Power Configuration cluster attributes */

/*! @name Power Configuration cluster commands
    @{
*/

/*! @} */ /* Power Configuration cluster commands */

/*! @} */ /* ZCL Power Configuration cluster definitions */

zb_void_t zb_zcl_power_config_init_server();
zb_void_t zb_zcl_power_config_init_client();
#define ZB_ZCL_CLUSTER_ID_POWER_CONFIG_SERVER_ROLE_INIT zb_zcl_power_config_init_server
#define ZB_ZCL_CLUSTER_ID_POWER_CONFIG_CLIENT_ROLE_INIT zb_zcl_power_config_init_client

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ! defined ZB_ZCL_POWER_CONFIG_H */
