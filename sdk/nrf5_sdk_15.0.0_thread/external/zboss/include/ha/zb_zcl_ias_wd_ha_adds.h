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
 * PURPOSE: IAS WD cluster defintions - HA extended
*/

#if ! defined ZB_ZCL_IAS_WD_HA_ADDS_H
#define ZB_ZCL_IAS_WD_HA_ADDS_H

#include "zcl/zb_zcl_ias_wd.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_IAS_WD_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
 *  @{
 *    @details
 *    HA extensions for IAS WD cluster
 */


/* Cluster ZB_ZCL_CLUSTER_ID_IAS_WD - HA extended */

/*! @name IAS WD cluster commands - HA extended
    @{
*/

/******************************* Start warning command ******************************/

/*! @brief Structure representsation Start warning command - HA extended. HA spec 10.9.4.1 */
typedef ZB_PACKED_PRE struct zb_zcl_ias_wd_start_warning_ha_s
{
  /** Status - enum flags Warning mode and Strobe, see ZCL spec 8.4.2.3.1 */
  zb_uint8_t status;
  /** Warning duration*/
  zb_uint16_t duration;
  /** Strobe duty cycle */
  zb_uint8_t strobe_duty_cycle;
  /** Strobe level */
  zb_uint8_t strobe_level;

} ZB_PACKED_STRUCT zb_zcl_ias_wd_start_warning_ha_t;

/*! @brief Get Strobe from payload Start warning command */
#define ZB_ZCL_IAS_WD_GET_SIREN_LEVEL(status)        (((status) >>6) & 0x03)

/*! @brief Make Status field Start warning command from Warning Mode,  Strobe and Siren Level */
#define ZB_ZCL_IAS_WD_MAKE_START_WARNING_STATUS_HA(warning_mode, strobe, siren_level)  \
  ((siren_level) << 6 | (strobe) << 4 | (warning_mode))

/*! @brief Start warning command - HA extended. HA spec 10.9.4.1
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param status - Status, see @ref zb_zcl_ias_wd_start_warning_s
    @param duration - Warning duration
    @param strobe_duty_cycle - Strobe duty cycle
    @param strobe_level - Strobe level
*/
#define ZB_ZCL_IAS_WD_SEND_START_WARNING_HA_REQ(              \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, \
    status, duration, strobe_duty_cycle, strobe_level)        \
{                                                             \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);              \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp));     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),  \
      ZB_ZCL_CMD_IAS_WD_START_WARNING_ID);                    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                     \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (duration));              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (strobe_duty_cycle));          \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (strobe_level));               \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                        \
  ZB_ZCL_SEND_COMMAND_SHORT(                                  \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id,       \
      ZB_ZCL_CLUSTER_ID_IAS_WD, cb);                          \
}

/** @brief Macro for getting Start warning command - HA extended. HA spec 10.9.4.1
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_ias_wd_start_warning_ha_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IAS_WD_GET_START_WARNING_HA_REQ(data_ptr, buffer, status)      \
{                                                                             \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_ias_wd_start_warning_ha_t))       \
  {                                                                           \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                    \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    zb_zcl_ias_wd_start_warning_ha_t *src_ptr =                               \
         (zb_zcl_ias_wd_start_warning_ha_t*)ZB_BUF_BEGIN((buffer));           \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                   \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_ias_wd_start_warning_ha_t)); \
    ZB_HTOLE16(&((data_ptr)->duration), &(src_ptr->duration));                \
  }                                                                           \
}

/*! @} */ /* IAS WD cluster commands */

/*! @} */ /* ZCL IAS WD cluster definitions */


/** @brief Declare run Start Warning command for User Application
*/
typedef struct zb_zcl_ias_wd_start_warning_ha_value_param_s
{
  zb_uint8_t warning_mode;        /*!< Warning Mode */
  zb_uint8_t strobe;              /*!< Strobe */
  zb_uint8_t siren_level;         /*!< Siren level */
  zb_uint16_t duration;           /*!< Duration */
  zb_uint8_t strobe_duty_cycle;   /*!< Strobe duty cycle */
  zb_uint8_t strobe_level;        /*!< Strobe level */
} zb_zcl_ias_wd_start_warning_ha_value_param_t;

/*! Struct for invoke User App & continue after */
typedef struct zb_zcl_ias_wd_start_warning_ha_user_app_schedule_e
{
  zb_zcl_parsed_hdr_t cmd_info;         /**< Parameters for continue command,
                                              see @ref zb_zcl_parsed_hdr_s */
  zb_zcl_ias_wd_start_warning_ha_value_param_t param; /**< User App command parameters,
                                        see @ref zb_zcl_ias_wd_start_warning_value_param_s */
} zb_zcl_ias_wd_start_warning_ha_user_app_schedule_t;

#define ZB_ZCL_IAS_WD_START_WARNING_HA_SCHEDULE_USER_APP(buffer, pcmd_info,                     \
    warningMode, strobe_, siren_level_, duration_, strobe_duty_cycle_, strobe_level_)           \
{                                                                                               \
  zb_zcl_ias_wd_start_warning_ha_user_app_schedule_t* user_data =                               \
          ZB_GET_BUF_PARAM((buffer), zb_zcl_ias_wd_start_warning_ha_user_app_schedule_t);       \
  ZB_MEMMOVE(&(user_data->cmd_info), (pcmd_info), sizeof(zb_zcl_parsed_hdr_t));                 \
  user_data->param.warning_mode = (warningMode);                                                \
  user_data->param.strobe = (strobe_);                                                          \
  user_data->param.siren_level = (siren_level_);                                                \
  user_data->param.duration = (duration_);                                                      \
  user_data->param.strobe_duty_cycle = (strobe_duty_cycle_);                                    \
  user_data->param.strobe_level = (strobe_level_);                                              \
  ZB_SCHEDULE_CALLBACK(zb_zcl_ias_wd_start_warning_ha_invoke_user_app, ZB_REF_FROM_BUF((buffer))); \
}

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_IAS_WD_HA_ADDS_H */
