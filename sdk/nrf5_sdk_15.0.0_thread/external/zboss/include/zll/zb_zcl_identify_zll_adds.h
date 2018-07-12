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
 * PURPOSE: ZLL additions to ZCL Identify cluster
*/

#if ! defined ZB_ZCL_IDENTIFY_ZLL_ADDS_H
#define ZB_ZCL_IDENTIFY_ZLL_ADDS_H

#include "zcl/zb_zcl_identify.h"

/**
 *  @defgroup ZB_ZLL_IDENTIFY_ADDS ZLL additions to Identify cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_IDENTIFY_ADDS
 *  ZLL additions to Identify cluster.
 *  @{
 *    @details
 *    Extended Identify cluster for Light Link (see spec 6.3.1).
 */

/*! @brief Identify cluster extended command identifiers
    @see ZLL specification, subclause 6.3.1.2
*/
enum zb_zcl_identify_cmd_zll_e
{
  ZB_ZCL_IDENTIFY_TRIGGER_EFFECT_ID = 0x40,           /**< "Trigger effect" command identifier. */
};

/** Effect identifier enum
 * @see ZLL spec 6.3.1.2.1.1 */
enum zb_zcl_identify_trigger_effect_e
{
  /**< Effect identifier field value: Light is turned on/off once */
  ZB_ZCL_IDENTIFY_EFFECT_ID_BLINK       = 0x00,
  /**< Effect identifier field value: Light turned on/off over 1 second and
   * repeated 15 times */
  ZB_ZCL_IDENTIFY_EFFECT_ID_BREATHE     = 0x01,
  /**< Effect identifier field value: Colored light turns green for 1 second;
   * non-colored light flashes twice */
  ZB_ZCL_IDENTIFY_EFFECT_ID_OKAY        = 0x02,
  /**< Effect identifier field value: Colored light turns orange for 8 seconds; non-colored
   * light switches to maximum brightness for 0.5s and then minimum brightness for 7.5s */
  ZB_ZCL_IDENTIFY_EFFECT_ID_CHANNEL_CHANGE  = 0xb,
  /**< Effect identifier field value: Complete the current effect sequence before terminating.
   * E.g., if in the middle of a breathe effect (as above), first complete the current 1s
   * breathe effect and then terminate the effect*/
  ZB_ZCL_IDENTIFY_EFFECT_ID_FINISH_EFFECT  = 0xfe,
  /**< Effect identifier field value: Terminate the effect as soon as possible */
  ZB_ZCL_IDENTIFY_EFFECT_ID_STOP  = 0xff,
};

/** Effect identifier enum
 * @see ZLL spec 6.3.1.2.1.2 */
enum zb_zcl_identify_trigger_variant_e
{
  /**< Effect variant field value: Default */
  ZB_ZCL_IDENTIFY_EFFECT_ID_VARIANT_DEFAULT       = 0x00,
};

/*! @brief Structured representsation of Trigger effect command payload */
typedef ZB_PACKED_PRE struct zb_zcl_identify_effect_req_s
{
  zb_uint8_t effect_id;         /*!< Effect identifiy, see @ref zb_zcl_identify_trigger_effect_e */
  zb_uint8_t effect_variant;    /*!< Effect variant, see @ref zb_zcl_identify_trigger_variant_e  */
} ZB_PACKED_STRUCT zb_zcl_identify_effect_req_t;

/*! @brief Send Trigger effect command
    @param buffer to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - flag "Default response required"
    @param cb - callback for getting command send status
    @param effect_id - effect identifiy, see @ref zb_zcl_identify_trigger_effect_e
    @param effect_var - effect variant, see @ref zb_zcl_identify_trigger_variant_e
*/
#define ZB_ZCL_IDENTIFY_SEND_TRIGGER_VARIANT_REQ(                                                   \
    buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, dis_default_resp, cb, effect_id, effect_var)  \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp);                               \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_IDENTIFY_TRIGGER_EFFECT_ID);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, effect_id);                                                          \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, effect_var);                                                         \
  ZB_ZCL_FINISH_PACKET(buffer, ptr);                                                                \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
      buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_IDENTIFY, cb);            \
}

/** @brief Parses Trigger effect command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_identify_effect_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_IDENTIFY_GET_TRIGGER_VARIANT_REQ(data_ptr, buffer, status)    \
{                                                                            \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_identify_effect_req_t))          \
  {                                                                          \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                   \
  }                                                                          \
  else                                                                       \
  {                                                                          \
    zb_zcl_identify_effect_req_t *src_ptr =                                  \
                      (zb_zcl_identify_effect_req_t*)ZB_BUF_BEGIN((buffer)); \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                  \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_identify_effect_req_t));    \
  }                                                                          \
}

/**
 *  @name Inform User App about ZLL Identify cluster command and change attributes.
 *  Internal structures and define-procedure for inform User App about ZLL Identify
 *  cluster command and change attributes.
 *  @internal
 *  @{
 */

/** @brief Declare change Identify attribute for User Application
*/
typedef struct zb_zcl_identify_effect_value_param_s
{
  zb_uint8_t effect_id;         /*!< Effect identifiy */
  zb_uint8_t effect_variant;    /*!< Effect variant */
} zb_zcl_identify_effect_value_param_t;

/*! Struct for invoke User App & continue after */
typedef struct zb_zll_identify_effect_user_app_schedule_e
{
  zb_zcl_parsed_hdr_t cmd_info;         /**< Parameters for continue command,
                                              see @ref zb_zcl_parsed_hdr_s */
  zb_zcl_identify_effect_value_param_t param; /**< User App command parameters,
                                        see @ref zb_zcl_identify_effect_value_param_s */
} zb_zll_identify_effect_user_app_schedule_t;

#define ZB_ZCL_IDENTIFY_EFFECT_SCHEDULE_USER_APP(buffer, pcmd_info, effectId, effectVar)    \
{                                                                                           \
  zb_zll_identify_effect_user_app_schedule_t* data =                                        \
          ZB_GET_BUF_PARAM((buffer), zb_zll_identify_effect_user_app_schedule_t);           \
  ZB_MEMMOVE(&(data->cmd_info), (pcmd_info), sizeof(zb_zcl_parsed_hdr_t));                  \
  data->param.effect_id = (effectId);                                                       \
  data->param.effect_variant = (effectVar);                                                 \
  ZB_SCHEDULE_CALLBACK(zb_zcl_identify_effect_invoke_user_app, ZB_REF_FROM_BUF((buffer)));  \
}

/** @} */

/** @} */ /* ZLL additions to Identify cluster. */

#endif /* ! defined ZB_ZCL_IDENTIFY_ZLL_ADDS_H */
