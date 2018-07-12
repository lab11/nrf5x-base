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
 * PURPOSE: Time cluster defintions
*/

#if ! defined ZB_ZCL_CUSTOM_ATTR_H
#define ZB_ZCL_CUSTOM_ATTR_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"


/** @cond (DOXYGEN_HA_SECTION && internals_doc) */

/** @addtogroup zcl_api
 * @{
 */

/* Cluster ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR */

/*! @name Custom Attributes cluster attributes
    @{
*/

/*! @brief Custom Attributes attribute identifiers
    @see ZCL spec, subclause 3.12.2.2
*/
enum zb_zcl_custom_attr_attr_e
{
  ZB_ZCL_ATTR_CUSTOM_ATTR_U8_ID           = 0x0001,
  ZB_ZCL_ATTR_CUSTOM_ATTR_U16_ID          = 0x0002,
  ZB_ZCL_ATTR_CUSTOM_ATTR_U32_ID          = 0x0003,
  ZB_ZCL_ATTR_CUSTOM_ATTR_IEEE_ID         = 0x0004,
  ZB_ZCL_ATTR_CUSTOM_ATTR_CHAR_STRING_ID  = 0x0005,
  ZB_ZCL_ATTR_CUSTOM_ATTR_UTC_TIME_ID     = 0x0006,
  ZB_ZCL_ATTR_CUSTOM_ATTR_BYTE_ARRAY_ID   = 0x0007,
  ZB_ZCL_ATTR_CUSTOM_ATTR_U8_READ_ONLY_ID = 0x0008,
  ZB_ZCL_ATTR_CUSTOM_ATTR_BOOL_ID = 0x0009
};

#define ZB_ZCL_CUSTOM_ATTR_U8_DEFAULT_VALUE 0x00
#define ZB_ZCL_CUSTOM_ATTR_U16_DEFAULT_VALUE 0x0000
#define ZB_ZCL_CUSTOM_ATTR_U32_DEFAULT_VALUE 0x00000000

/*! @} */ /* Custom Attributes cluster attributes */

/*! @name Custom Attributes commands
    @{
*/

/*! @} */ /* Custom Attributes commands */

/*! @internal @name Custom Attributes internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_U8_ID(data_ptr)           \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_U8_ID,                                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_U8_READ_ONLY_ID(data_ptr) \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_U8_READ_ONLY_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_BOOL_ID(data_ptr)         \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_BOOL_ID,                                                 \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_U16_ID(data_ptr)          \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_U16_ID,                                                \
  ZB_ZCL_ATTR_TYPE_U16,                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_U32_ID(data_ptr)          \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_U32_ID,                                                \
  ZB_ZCL_ATTR_TYPE_U32,                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                                                 \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_IEEE_ID(data_ptr)         \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_IEEE_ID,                                               \
  ZB_ZCL_ATTR_TYPE_IEEE_ADDR,                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                                                 \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_CHAR_STRING_ID(data_ptr)  \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_CHAR_STRING_ID,                                        \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_UTC_TIME_ID(data_ptr)     \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_UTC_TIME_ID,                                           \
  ZB_ZCL_ATTR_TYPE_UTC_TIME,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CUSTOM_ATTR_BYTE_ARRAY_ID(data_ptr)   \
{                                                                                \
  ZB_ZCL_ATTR_CUSTOM_ATTR_BYTE_ARRAY_ID,                                         \
  ZB_ZCL_ATTR_TYPE_BYTE_ARRAY,                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
  (zb_voidp_t) data_ptr                                                          \
}

/** @internal @brief Declare attribute list for Custom Attributes cluster
*/
#define ZB_ZCL_DECLARE_CUSTOM_ATTR_CLUSTER_ATTRIB_LIST(attr_list, u8_attr, u8ro_attr, u16_attr, u32_attr,\
                                                       ieee_attr, char_string_attr, utc_time_attr, byte_array_attr, bool_attr) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_U8_ID, (u8_attr))                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_U16_ID, (u16_attr))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_U32_ID, (u32_attr))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_IEEE_ID, (ieee_attr))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_CHAR_STRING_ID, (char_string_attr))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_UTC_TIME_ID, (utc_time_attr))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_BYTE_ARRAY_ID, (byte_array_attr))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_U8_READ_ONLY_ID, (u8ro_attr))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CUSTOM_ATTR_BOOL_ID, (bool_attr))           \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Custom Attributes cluster */
#define ZB_ZCL_CUSTOM_ATTR_REPORT_ATTR_COUNT 8

/*! @} */ /* Custom Attributes cluster internals */

/*! @} */ /* ZCL Custom Attributes cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_custom_attr_init_server();
zb_void_t zb_zcl_custom_attr_init_client();
#define ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR_SERVER_ROLE_INIT zb_zcl_custom_attr_init_server
#define ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR_CLIENT_ROLE_INIT zb_zcl_custom_attr_init_client

#endif /* ! defined ZB_ZCL_CUSTOM_ATTR_H */
