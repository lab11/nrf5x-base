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
 * PURPOSE: ZCL Basic Cluster definitions - extension for HA IAS devices
*/
#if ! defined ZB_ZCL_BASIC_HA_ADDS_H
#define ZB_ZCL_BASIC_HA_ADDS_H

#include "zcl/zb_zcl_basic.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_BASIC_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
  * @{
  *   @details
  *   Extension for HA devices - DeviceEnable attribute shall be read-only and set to 1.
  */

/** @internal @name Basic cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#undef ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID

/** @cond internals_doc */
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID(data_ptr) \
{                                                                            \
  ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                                       \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                              \
  (zb_voidp_t) data_ptr                                                      \
}

/** @brief Declare attribute list for Basic cluster
    @param attr_list - attribute list name
    @param zcl_version - pointer to variable to store zcl version  attribute value
    @param power_source - pointer to variable to store power source attribute value
*/
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_HA_ADDS(attr_list, zcl_version, power_source) \
    zb_bool_t device_enable_##attr_list = ZB_TRUE;                             \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))      \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))    \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                  \
                    &(device_enable_##attr_list))                              \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @endcond */

 /**
 *  @brief Declare attribute list for Basic cluster with HA additions.
 *  @param attr_list [IN] - attribute list name.
 *  @param zcl_version [IN] - pointer to variable storing ZCL version  attribute value.
 *  @param app_version [IN] - pointer to the variable storing application version.
 *  @param stack_version [IN] - pointer to the variable storing stack version.
 *  @param hardware_version [IN] - pointer to the variable storing hardware version.
 *  @param manufacturer_name [IN] - pointer to the variable storing manufacturer name.
 *  @param model_id [IN] - pointer to the variable storing model identifier.
 *  @param date_code [IN] - pointer to the variable storing date code.
 *  @param power_source [IN] - pointer to variable storing power source attribute value.
 *  @param location_id [IN] - pointer to variable storing location description attribute value.
 *  @param ph_env [IN] - pointer to variable storing physical environment attribute value.
 */
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_HA_ADDS_FULL(                              \
  attr_list,                                                                        \
  zcl_version,                                                                      \
  app_version,                                                                      \
  stack_version,                                                                    \
  hardware_version,                                                                 \
  manufacturer_name,                                                                \
  model_id,                                                                         \
  date_code,                                                                        \
  power_source,                                                                     \
  location_id,                                                                      \
  ph_env)                                                                           \
    zb_bool_t device_enable_##attr_list = ZB_TRUE;                                  \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                     \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID, (app_version))   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID, (stack_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_HW_VERSION_ID, (hardware_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (manufacturer_name))\
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (model_id))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DATE_CODE_ID, (date_code))               \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_LOCATION_DESCRIPTION_ID, (location_id))  \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_PHYSICAL_ENVIRONMENT_ID, (ph_env))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DEVICE_ENABLED_ID,                       \
                    &(device_enable_##attr_list))                                   \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Basic cluster internals */

/*! @} */ /* ZCL Basic cluster definitions */

zb_uint8_t zb_zcl_get_cmd_list_basic(zb_bool_t is_client_generated, zb_uint8_t **cmd_list);

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_BASIC_HA_ADDS_H */
