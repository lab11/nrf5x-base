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
 * PURPOSE: HA EZ-Mode commissioning definitions
*/

#ifndef ZB_HA_EZ_MODE_COMISSIONING_H
#define ZB_HA_EZ_MODE_COMISSIONING_H 1

/**
 *  @cond include_ha_ez_mode
 *  \addtogroup ha_ez_mode_comissioning
 *  @{
 *    @details
 *
 */

/**
 * Minimum time for PermitJoining and identifying (second)
 */
#define ZB_HA_EZ_MODE_TIME           ((zb_time_t)(180))
#define ZB_HA_EZ_MODE_QUERY_TIME        4

/**
 * Maximum endpoints of the "respondent" that
 * EZ-Mode can serve
 */
#define ZB_HA_EZ_MODE_ACTIVE_ENDP_LIST_LEN 4

/**
 * Channels that should be scanned before
 * others during EZ-Mode Nwk steering
 * 0000.0011 0001.1000 1100.1000 0000.0000
 */
#define ZB_HA_PREFERRED_CHANNEL_MASK  0x0318C800

/**
 * List of EZ-Mode commissioning states
 */
typedef enum zb_ha_ez_mode_state_e
{
  /** EZ-Mode isn't invoked */
  ZB_HA_EZ_MODE_IDLE                  = 0,
  /** EZ-Mode factory reset in progress */
  ZB_HA_EZ_MODE_FACTORY_RESET         = 1,
  /** EZ-Mode network steering in progress (scanning or forming network) */
  ZB_HA_EZ_MODE_NWK_STEERING          = 2,
  /**  EZ-Mode network steering in progress (scanning or forming network finished) */
  ZB_HA_EZ_MODE_NWK_STEERING_JOINED   = 3,
  /** EZ-Mode finding and binding in progress */
  ZB_HA_EZ_MODE_FINDING_AND_BINDING   = 4,
  ZB_HA_EZ_MODE_FINDING_AND_BINDING_TARGET   = 5,
}
  zb_ha_ez_mode_state_t;

/**
 * List of EZ-Mode binding callback states
 */
typedef enum zb_ha_ez_mode_binding_cb_state_e
{
  /** Previously user applied bind finished successfully */
  ZB_HA_EZ_MODE_BIND_SUCCESS = 0,
  /** Previously user applied bind failed */
  ZB_HA_EZ_MODE_BIND_FAIL = 1,
  /** Ask user whether to perform binding */
  ZB_HA_EZ_MODE_BIND_ASK_USER = 2,
}
  zb_ha_ez_mode_binding_cb_state_t;

/**
 *  EZ-Mode finding & binding binding in progress callback function typedef.
 *  Function is used both to interact with user application, get decision
 *  if new binding is needed or not, and to report the binding result
 *  @param status - status of the binding (ask user, success or fail)
 *  @param addr - extended address of a device to bind
 *  @param ep - endpoint of a device to bind
 *  @param cluster - cluster id to bind
 *  @return bool - agree or disagree
 */
typedef zb_bool_t (ZB_CODE * zb_ha_ez_mode_binding_callback_t)(
  zb_int16_t status, zb_ieee_addr_t addr, zb_uint8_t ep, zb_uint16_t cluster);

/* [AV] no longer required; see description below in zb_ha_ez_mode_ctx_s */


/**
 * EZ-Mode commissioning context
 */
typedef ZB_PACKED_PRE struct zb_ha_ez_mode_ctx_s
{
  /** State of EZ-Mode */
  zb_ha_ez_mode_state_t state;
  /** Callback function invoked when factory fresh or network steering operations finish */
  zb_callback_t user_cb;
  /** Callback function invoked when binding operation start */
  zb_ha_ez_mode_binding_callback_t finding_binding_progress_cb;
/* [AV] After having separated binding user callbacks from f&b complete callback
    the last one can be stored in the user_cb field. */
  /** Duration of PermitJoining and IdentifyTime */
  zb_uint16_t duration;
  /** Short address of "respondent" */
  zb_uint16_t respondent_addr;
  /** Extended address of "respondent" */
  zb_ieee_addr_t respondent_ext_addr;
  /** Endpoint which participate in finding and binding*/
  zb_uint8_t ep;
  /** Endpoint descriptor*/
  zb_af_endpoint_desc_t *ep_desc;
  /** Respondent's active endpoints list */
  zb_uint8_t ep_list[ZB_HA_EZ_MODE_ACTIVE_ENDP_LIST_LEN];
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;
  /** Count of the currently checked endpoints */
  zb_uint8_t eps_checked;
  /** Signals that at least one endpoint was bound during finding and binding;
    * it is used to invoke user callback if no endpoint was binded
    */
  zb_bool_t was_bound;
  /*EZ mode in progress flag. Sets for EZScanTimeout*/
  zb_bool_t ez_scan_inprogress;
  /** Reference to the buffer holding simple descriptor response */
  zb_uint8_t simple_desc_buf_ref;
} ZB_PACKED_STRUCT zb_ha_ez_mode_ctx_t;

/** @brief Invoke EZ-Mode factory reset operation.
  *
  * This function is called by user to perform factory reset:
  * leave network, clean binding and group table entries;
  * note that, if error occurs, user_cb won't be called.
  *
  * @param user_cb - callback function to be called after operation finishes
  * @return RET_OK, if invoking factory reset completes OK, or error code
  */
zb_ret_t zb_ha_ez_mode_factory_reset(zb_callback_t user_cb);

/** @brief Invoke EZ-Mode network steering operation.
  *
  * This function is called by user to perform network steering:
  * set PermitJoining if device is already in network or
  * scan and join / form network;
  * note that, if error occurs, user_cb won't be called
  *
  * @param permit_duration - time (in seconds) when join is permitted
  * @param user_cb - callback function to be called after operation finishes
  * @return RET_OK, if invoking network steering completes OK, or error code
  */
zb_ret_t zb_ha_ez_mode_nwk_steering(zb_uint16_t permit_duration, zb_callback_t user_cb);

/** @brief Invoke EZ-Mode finding and binding operation.
  *
  * This function is called by user to perform finding and binding operation
  * on initiator endpoint: search for identifying devices and bind to target;
  * note that, if error occurs, user_cb won't be called
  *
  * @param endpoint - initiator endpoint
  * @param user_binding_cb - callback function to be called before operation finisher
  * @param user_cb - callback function to be called after operation finishes
  * @return RET_OK, if invoking finding & binding completes OK, or error code
  */
zb_ret_t zb_ha_ez_mode_finding_binding_initiator(zb_uint8_t endpoint,
    zb_ha_ez_mode_binding_callback_t user_binding_cb, zb_callback_t user_cb);

/** @brief Invoke EZ-Mode finding and binding operation.
  *
  * This function is called by user to perform finding and binding operation
  * on target endpoint: makes endpoint start identifying.
  *
  * @param endpoint - target endpoint
  * @param identify_time - time (in seconds) when endpoint is identifying
  * @return RET_OK on success or other error code on fail @see zb_ha_ez_mode_error_code_t
  */
zb_ret_t zb_ha_ez_mode_finding_binding_target(zb_uint8_t endpoint, zb_uint16_t identify_time);

/** @brief Cancel ongoing finding and binding procedure on target
  *
  * Function is used to force stopping of finding and binding procedure on
  * target before provided identify_time to @ref zb_ha_ez_mode_finding_binding_target
  * is passed.
  *
  * @param endpoint   target endpoint
  */
void zb_ha_ez_mode_finding_binding_target_cancel(zb_uint8_t endpoint);

/*
  Clears device binding table only
  return - unbind status as buf->u.hdr.status
  NOTE! not release input buffer (param)
*/
zb_void_t zb_ha_ez_mode_unbind(zb_uint8_t param);

/** @brief EZ-Mode cancel.
  *
  * This is a callback called after EZScanTime if no open networks found
  *
  */
void zb_ha_ez_mode_timeout(zb_uint8_t param);

/** @brief EZ-Mode cancel.
  *
  * This function is called by user to stop EZ-Mode commissioning.
  *
  */
void zb_ha_ez_mode_cancel();

/* [AV] see zb_ha_ez_mode_commissioning.c */
/** @brief Check whether EZ-Mode steering is in progress
  *
  * Use this function to check if EZ-Mode network steering is in progress.
  *
  */
zb_bool_t zb_ha_ez_mode_steering_in_progress();

/**  @}
*  @endcond
*/

void zb_ha_ez_mode_init_ctx();
void zb_ha_ez_mode_nlme_permit_joining_conf(zb_uint8_t param);
zb_bool_t zb_ha_ez_mode_dev_start(zb_uint8_t param);
void zb_ha_ez_mode_process_identify_query_res(zb_uint8_t param);

#endif /* !defined ZB_HA_EZ_MODE_COMISSIONING_H */
