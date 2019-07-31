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
PURPOSE: Power Configuration cluster defintions - HA extension
*/

#if ! defined ZB_ZCL_POWER_CONFIG_HA_ADDS_H
#define ZB_ZCL_POWER_CONFIG_HA_ADDS_H

#include "zcl/zb_zcl_power_config.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_POWER_CONFIG_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
 *  @{
 *    @details
 *    HA extensions for Power Configuration cluster
 */

/* Cluster ZB_ZCL_CLUSTER_ID_POWER_CONFIG - HA extension */

/*! @name Power Configuration cluster attributes - HA extension
    @{
*/

/*! @brief Power Configuration cluster - extension for HA attribute identifiers
    @see HA spec, Power Configuration Cluster 10.10.2, 10.10.3
 */
enum zb_zcl_power_config_attr_ha_e
{
  /*! @brief BatteryPercentageRemaining attribute, HA spec 10.10.2.1 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_REMAINING_HA_ID              = 0x0021,

  /*! @brief BatteryVoltageThreshold1 attribute, HA spec 10.10.3.3 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID        = 0x0037,
  /*! @brief BatteryVoltageThreshold2 attribute, HA spec 10.10.3.3 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID        = 0x0038,
  /*! @brief BatteryVoltageThreshold3 attribute, HA spec 10.10.3.3 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID        = 0x0039,

  /*! @brief BatteryPercentageMinThreshold attribute, HA spec 10.10.3.4 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID  = 0x003a,

  /*! @brief BatteryPercentageThreshold1 attribute, HA spec 10.10.3.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID     = 0x003b,
  /*! @brief BatteryPercentageThreshold2 attribute, HA spec 10.10.3.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID     = 0x003c,
  /*! @brief BatteryPercentageThreshold3 attribute, HA spec 10.10.3.5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID     = 0x003d,

  /*! @brief BatteryAlarmState attribute, HA spec 10.10.3.6 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID               = 0x003e,


  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID = 0x0040, /* attr set 4 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_REMAINING_HA_ID             = 0x0041,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_SIZE_ID = 0x0051, /* attr set 5 */
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_QUANTITY_ID = 0x0053,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_RATED_VOLTAGE_ID = 0x0054,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_MASK_ID = 0x0055,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_MIN_THRESHOLD_ID = 0x0056,

  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD1_ID        = 0x0057,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD2_ID        = 0x0058,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD3_ID        = 0x0059,

  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_MIN_THRESHOLD_ID  = 0x005a,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD1_ID     = 0x005b,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD2_ID     = 0x005c,
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD3_ID     = 0x005d,

  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_STATE_ID               = 0x005e
};

/** @brief Power Configuration BatteryPercentageRemaining attribute unknown value */
#define ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_REMAINING_UNKNOWN            0xff

/*! @brief Power Configuration BatteryAlarmState attribute flags
    @see HA spec 10.10.3.1
*/
enum zb_zcl_power_config_battery_alarm_mask_ha_e
{
  /** @brief BatteryAlarmMask - Battery voltage too low */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_VOLTAGE_LOW_HA = 0,

  /** @brief BatteryAlarmMask - Alarm1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM1 = 1,
  /** @brief BatteryAlarmMask - Alarm2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM2 = 2,
  /** @brief BatteryAlarmMask - Alarm3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_MASK_ALARM3 = 3,
};

/*! @brief Power Configuration Alarm Code for Battery Alarm
    @see HA spec 10.10.3.2
*/
enum zb_zcl_power_config_battery_alarm_code_ha_e
{
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_MIN_THRESHOLD = 0x10,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE1   = 0x11,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE2   = 0x12,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE1_VOLTAGE3   = 0x13,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_MIN_THRESHOLD = 0x20,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE1   = 0x21,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE2   = 0x22,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE2_VOLTAGE3   = 0x23,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_MIN_THRESHOLD = 0x30,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE1   = 0x31,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE2   = 0x32,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODE_SOURCE3_VOLTAGE3   = 0x33,
};

/*! @brief Power Configuration Alarm Code for Battery Alarm
    @see HA spec 10.10.3.6
*/
enum zb_zcl_power_config_battery_alarm_state_ha_e
{
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD = 1 << 0,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1   = 1 << 1,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2   = 1 << 2,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 1 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3   = 1 << 3,

  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_MIN_THRESHOLD = 1 << 10,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE1   = 1 << 11,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE2   = 1 << 12,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 2 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE2_VOLTAGE3   = 1 << 13,

    /* use 1l to exclude overflow at 16-bit CPU: force 32-bit enum */
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold or
   * BatteryPercentageMinThreshold reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_MIN_THRESHOLD = 1l << 20,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold1 or
   * BatteryPercentageMinThreshold1 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE1   = 1l << 21,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold2 or
   * BatteryPercentageMinThreshold2 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE2   = 1l << 22,
  /** @brief BatteryAlarmCode - BatteryVoltageMinThreshold3 or
   * BatteryPercentageMinThreshold3 reached for Battery Source 3 */
    ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE3_VOLTAGE3   = 1l << 23
};


/*! @internal @name Power Configuration cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_REMAINING_HA_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_REMAINING_HA_ID,             \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID,              \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(bat_num,                                 \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state)                                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_ID, (voltage)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_SIZE_ID, (size))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_QUANTITY_ID, (quantity))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_RATED_VOLTAGE_ID, (rated_voltage))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_ALARM_MASK_ID, (alarm_mask ))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_MIN_THRESHOLD_ID, (voltage_min_threshold))  \
  /* ext attribute */                                                                                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_REMAINING_HA_ID, (remaining))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD1_ID, (threshold1))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD2_ID, (threshold2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_VOLTAGE_THRESHOLD3_ID, (threshold3))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_MIN_THRESHOLD_ID, (min_threshold))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD1_ID, (percent_threshold1)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD2_ID, (percent_threshold2)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_PERCENTAGE_THRESHOLD3_ID, (percent_threshold3)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY##bat_num##_ALARM_STATE_ID, (alarm_state))                  \


/** @internal @brief Declare attribute list for Power Configuration cluster - server side
    @param attr_list - attribure list name
    @param voltage - pointer to variable to store BatteryVoltage attribute
    @param size - pointer to variable to store BatterySize attribute
    @param quantity - pointer to variable to store BatteryQuantity attribute
    @param rated_voltage - pointer to variable to store BatteryRatedVoltage attribute
    @param alarm_mask - pointer to variable to store BatteryAlarmMask attribute
    @param voltage_min_threshold - pointer to variable to store BatteryVoltageMinThreshold attribute

    @param remaining - pointer to variable to store BatteryPercentageRemaining attribute
    @param threshold1 - pointer to variable to store BatteryVoltageThreshold1 attribute
    @param threshold2 - pointer to variable to store BatteryVoltageThreshold2 attribute
    @param threshold3 - pointer to variable to store BatteryVoltageThreshold3 attribute
    @param min_threshold - pointer to variable to store BatteryPercentageMinThreshold attribute
    @param percent_threshold1 - pointer to variable to store BatteryPercentageThreshold1 attribute
    @param percent_threshold2 - pointer to variable to store BatteryPercentageThreshold2 attribute
    @param percent_threshold3 - pointer to variable to store BatteryPercentageThreshold3 attribute
    @param alarm_state - pointer to variable to store BatteryAlarmState attribute
*/
#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST_HA(attr_list,                               \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state)                                    \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                               \
  ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(,                                              \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state)                                    \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/********* Battery pack 2 definitions **************/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID,                  \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_SIZE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_SIZE_ID,             \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_QUANTITY_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_QUANTITY_ID,         \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_RATED_VOLTAGE_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_RATED_VOLTAGE_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_MASK_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_MASK_ID,       \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_MIN_THRESHOLD_ID(data_ptr) \
{                                                       \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_MIN_THRESHOLD_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                        \
  (zb_voidp_t) data_ptr                                 \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_REMAINING_HA_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_REMAINING_HA_ID,             \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD1_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD1_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD2_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD2_ID,      \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD3_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD3_ID,       \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_MIN_THRESHOLD_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_MIN_THRESHOLD_ID, \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD1_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD1_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD2_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD2_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD3_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_THRESHOLD3_ID,    \
  ZB_ZCL_ATTR_TYPE_U8,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_STATE_ID(data_ptr) \
{                                                               \
  ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_STATE_ID,              \
  ZB_ZCL_ATTR_TYPE_U32,                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,  \
  (zb_voidp_t) data_ptr                                         \
}

#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST_HA_2_BAT_FULL(attr_list,     \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state, \
    /**** battery pack 2 ****/ \
    voltage_b2, size_b2, quantity_b2, rated_voltage_b2, alarm_mask_b2, voltage_min_threshold_b2, \
    remaining_b2, threshold1_b2, threshold2_b2, threshold3_b2, min_threshold_b2, percent_threshold1_b2, \
    percent_threshold2_b2, percent_threshold3_b2, alarm_state_b2)                           \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                               \
  ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(,                                              \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state)                                    \
  /*** batterry pack 2 ***/                                                                 \
  ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(2,                                             \
    voltage_b2, size_b2, quantity_b2, rated_voltage_b2, alarm_mask_b2, voltage_min_threshold_b2,        \
    remaining_b2, threshold1_b2, threshold2_b2, threshold3_b2, min_threshold_b2, percent_threshold1_b2, \
    percent_threshold2_b2, percent_threshold3_b2, alarm_state_b2)                                       \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


/** @internal @brief Declare attribute list for Power Configuration cluster (mains + 2 batterys) - server side
    @param attr_list - attribure list name

    @param voltage_m - pointer to variable to store MainsVoltage attribute
    @param frequency_m - pointer to variable to store MainsFrequency attribute
    @param alarm_mask_m - pointer to variable to store BatteryAlarmMask attribute
    @param voltage_min_threshold_m - pointer to variable to store MainsVoltageMinThreshold attribute
    @param voltage_max_threshold_m - pointer to variable to store MainsVoltageMaxThreshold attribute
    @param dwell_trip_point_m - pointer to variable to store MainsVoltageDwellTripPoint attribute

    @param voltage - pointer to variable to store Battery1 BatteryVoltage attribute
    @param size - pointer to variable to store Battery1 BatterySize attribute
    @param quantity - pointer to variable to store Battery1 BatteryQuantity attribute
    @param rated_voltage - pointer to variable to store Battery1 BatteryRatedVoltage attribute
    @param alarm_mask - pointer to variable to store Battery1 BatteryAlarmMask attribute
    @param voltage_min_threshold - pointer to variable to store Battery1 BatteryVoltageMinThreshold attribute

    @param remaining - pointer to variable to store Battery1 BatteryPercentageRemaining attribute
    @param threshold1 - pointer to variable to store Battery1 BatteryVoltageThreshold1 attribute
    @param threshold2 - pointer to variable to store Battery1 BatteryVoltageThreshold2 attribute
    @param threshold3 - pointer to variable to store Battery1 BatteryVoltageThreshold3 attribute
    @param min_threshold - pointer to variable to store Battery1 BatteryPercentageMinThreshold attribute
    @param percent_threshold1 - pointer to variable to store Battery1 BatteryPercentageThreshold1 attribute
    @param percent_threshold2 - pointer to variable to store Battery1 BatteryPercentageThreshold2 attribute
    @param percent_threshold3 - pointer to variable to store Battery1 BatteryPercentageThreshold3 attribute
    @param alarm_state - pointer to variable to store Battery1 BatteryAlarmState attribute

    @param voltage_b2 - pointer to variable to store Battery2 BatteryVoltage attribute
    @param size_b2 - pointer to variable to store Battery2 BatterySize attribute
    @param quantity_b2 - pointer to variable to store Battery2 BatteryQuantity attribute
    @param rated_voltage_b2 - pointer to variable to store Battery2 BatteryRatedVoltage attribute
    @param alarm_mask_b2 - pointer to variable to store Battery2 BatteryAlarmMask attribute
    @param voltage_min_threshold_b2 - pointer to variable to store Battery2 BatteryVoltageMinThreshold attribute

    @param remaining_b2 - pointer to variable to store Battery2 BatteryPercentageRemaining attribute
    @param threshold1_b2 - pointer to variable to store Battery2 BatteryVoltageThreshold1 attribute
    @param threshold2_b2 - pointer to variable to store Battery2 BatteryVoltageThreshold2 attribute
    @param threshold3_b2 - pointer to variable to store Battery2 BatteryVoltageThreshold3 attribute
    @param min_threshold_b2 - pointer to variable to store Battery2 BatteryPercentageMinThreshold attribute
    @param percent_threshold1_b2 - pointer to variable to store Battery2 BatteryPercentageThreshold1 attribute
    @param percent_threshold2_b2 - pointer to variable to store Battery2 BatteryPercentageThreshold2 attribute
    @param percent_threshold3_b2 - pointer to variable to store Battery2 BatteryPercentageThreshold3 attribute
    @param alarm_state_b2 - pointer to variable to store Battery2 BatteryAlarmState attribute
*/
#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST_HA_2_BAT_MAINS_FULL(attr_list,              \
    /**** mains ****/ \
    voltage_m, frequency_m, alarm_mask_m,                                                   \
    voltage_min_threshold_m, voltage_max_threshold_m,                                       \
    dwell_trip_point_m,                                                                     \
    /**** battery pack ****/ \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state, \
    /**** battery pack 2 ****/ \
    voltage_b2, size_b2, quantity_b2, rated_voltage_b2, alarm_mask_b2, voltage_min_threshold_b2, \
    remaining_b2, threshold1_b2, threshold2_b2, threshold3_b2, min_threshold_b2, percent_threshold1_b2, \
    percent_threshold2_b2, percent_threshold3_b2, alarm_state_b2)                           \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                               \
  /**** mains ****/                                                                         \
  ZB_ZCL_POWER_CONFIG_MAINS_ATTRIB_LIST(voltage_m, frequency_m, alarm_mask_m,               \
    voltage_min_threshold_m, voltage_max_threshold_m, dwell_trip_point_m)                   \
  /**** battery pack ****/                                                                  \
  ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(,                                              \
    voltage, size, quantity, rated_voltage, alarm_mask, voltage_min_threshold,              \
    remaining, threshold1, threshold2, threshold3, min_threshold, percent_threshold1,       \
    percent_threshold2, percent_threshold3, alarm_state)                                    \
  /*** batterry pack 2 ***/ \
  ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIB_LIST_HA(2,                                             \
    voltage_b2, size_b2, quantity_b2, rated_voltage_b2, alarm_mask_b2, voltage_min_threshold_b2,        \
    remaining_b2, threshold1_b2, threshold2_b2, threshold3_b2, min_threshold_b2, percent_threshold1_b2, \
    percent_threshold2_b2, percent_threshold3_b2, alarm_state_b2)                                       \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


#define ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST_HA_2_BAT_NO_ACCUM(attr_list, \
    voltage, size, quantity, alarm_mask, voltage_min_threshold,              \
    threshold1, threshold2, threshold3, alarm_state,                         \
    /**** battery pack 2 ****/ \
    voltage_b2, size_b2, quantity_b2, alarm_mask_b2, voltage_min_threshold_b2, \
    threshold1_b2, threshold2_b2, threshold3_b2, alarm_state_b2)                           \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID, (voltage))              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID, (size))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID, (quantity))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID, (alarm_mask ))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID, (voltage_min_threshold))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID, (threshold1))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID, (threshold2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID, (threshold3))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID, (alarm_state))                  \
  /*** batterry pack 2 ***/ \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID, (voltage_b2))              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_SIZE_ID, (size_b2))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_QUANTITY_ID, (quantity_b2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_MASK_ID, (alarm_mask_b2 ))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_MIN_THRESHOLD_ID, (voltage_min_threshold_b2))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD1_ID, (threshold1_b2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD2_ID, (threshold2_b2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_THRESHOLD3_ID, (threshold3_b2))            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_ALARM_STATE_ID, (alarm_state_b2))                  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Power
 *  Configuration cluster for 1 battery pack */
#undef ZB_ZCL_POWER_CONFIG_REPORT_ATTR_COUNT
#define ZB_ZCL_POWER_CONFIG_REPORT_ATTR_COUNT 2 /* Voltage + Alarm State */
/*! @internal Number of attributes mandatory for reporting in Power
 *  Configuration cluster for 2 Battery packs included */
#define ZB_ZCL_POWER_CONFIG_BAT_PACK_2_REPORT_ATTR_COUNT 4 /* Voltage + Voltage 2 + Alarm State +
                                                            * Alarm State 2 */


/*! @} */ /* Power Configuration cluster internals */

/*! @} */ /* Power Configuration cluster attributes */

/*! @name Power Configuration cluster commands
    @{
*/

/*! @} */ /* Power Configuration cluster commands */

/*! @} */ /* ZCL Power Configuration cluster definitions */

zb_uint8_t zb_zcl_get_cmd_list_power_config(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_POWER_CONFIG_HA_ADDS_H */
