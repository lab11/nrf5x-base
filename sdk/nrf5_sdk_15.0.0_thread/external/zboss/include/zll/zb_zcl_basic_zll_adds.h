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
 * PURPOSE: ZLL additions to ZCL Basic cluster
*/

#if ! defined ZB_ZCL_BASIC_ZLL_ADDS_H
#define ZB_ZCL_BASIC_ZLL_ADDS_H

#include "zcl/zb_zcl_basic.h"

/**
 *  @defgroup ZB_ZLL_BASIC_ADDS ZLL additions to Basic cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_BASIC_ADDS
 *  ZLL additions to Basic cluster.
 *  @{
 *    @details
 *    Extended Basic cluster for Light Link (see spec 6.6.1).
 *
 *    @par Example
 *    Declare cluster:
 *    @snippet tp_cst_tc_06_zr.c BASIC_DECLARE
 *    Read attribute Vestion ID command sending:
 *    @snippet tp_cst_tc_06_zed.c SEND_VERSION_ID
 *    Process response:
 *    @snippet tp_cst_tc_06_zed.c GET_VERSION_ID
 *    @par
 *
 *    Other commands can be sent in the same way using appropriate macros.
 *
 *    For more information tp_cst_06 test
 */

/**
 *  @name Attributes added to Basic cluster.
 *  @{
 */

/**
 *  @brief "Manufacturer-specific reference to the version of the software" attribute identifier.
 *  @see ZLL specification, subclause 6.2.1.1.
 */
#define ZB_ZCL_ATTR_BASIC_SW_BUILD_ID_ZLL_ID  0x4000

/**
 *  @brief Default value for SWBuildId attribute.
 *  @see ZLL specification, subclause 6.2.1.1.
 *  @note Default value supposes that attribute will be stored as Pascal-style string (i. e.
 *  length-byte, then content).
 */
#define ZB_ZCL_BASIC_SW_BUILD_ID_DEVAULT_VALUE  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/** @} */ /* Attributes added to Basic cluster. */

/**
 *  @name ZLL Basic cluster additions internals.
 *  Internal structures for attribute representation in cluster additions.
 *  @internal
 *  @{
 */
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_BASIC_SW_BUILD_ID_ZLL_ID(data_ptr)   \
{                                                                               \
  ZB_ZCL_ATTR_BASIC_SW_BUILD_ID_ZLL_ID,                                         \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                 \
  (zb_voidp_t) data_ptr                                                         \
}

/**
 *  @brief Declare attribute list for Basic cluster with ZLL additions.
 *  @param attr_list [IN] - attribure list name.
 *  @param zcl_version [IN] - pointer to variable storing ZCL version  attribute value.
 *  @param app_version [IN] - pointer to the variable storing application version.
 *  @param stack_version [IN] - pointer to the variable storing stack version.
 *  @param hardware_version [IN] - pointer to the variable storing hardware version.
 *  @param manufacturer_name [IN] - pointer to the variable storing manufacturer name.
 *  @param model_id [IN] - pointer to the variable storing model identifier.
 *  @param date_code [IN] - pointer to the variable storing date code.
 *  @param power_source [IN] - pointer to variable storing power source attribute value.
 *  @param sw_build_id [IN] - pointer to the variable storing software version reference.
 */
#define ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_ZLL(                                       \
  attr_list,                                                                        \
  zcl_version,                                                                      \
  app_version,                                                                      \
  stack_version,                                                                    \
  hardware_version,                                                                 \
  manufacturer_name,                                                                \
  model_id,                                                                         \
  date_code,                                                                        \
  power_source,                                                                     \
  sw_build_id)                                                                      \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                     \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, (zcl_version))           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID, (app_version))   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_STACK_VERSION_ID, (stack_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_HW_VERSION_ID, (hardware_version))       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (manufacturer_name))\
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (model_id))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_DATE_CODE_ID, (date_code))               \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, (power_source))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_BASIC_SW_BUILD_ID_ZLL_ID, (sw_build_id))       \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @} */ /* Basic cluster additions internals. */

/** @} */ /* ZLL additions to Basic cluster. */

#endif /* ! defined ZB_ZCL_BASIC_ZLL_ADDS_H */
