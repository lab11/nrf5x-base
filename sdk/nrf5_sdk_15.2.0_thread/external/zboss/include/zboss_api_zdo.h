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
 PURPOSE: Public ZDO layer API
*/
#ifndef ZB_ZBOSS_API_ZDO_H
#define ZB_ZBOSS_API_ZDO_H 1

#include "zboss_api_zgp.h"

/*! \addtogroup zdo_base */
/*! @{ */

/**
   ZDP status values
   (2.4.5 ZDP Enumeration Description)
 */
typedef enum zb_zdp_status_e
{
  /** The requested operation or transmission was completed successfully */
  ZB_ZDP_STATUS_SUCCESS         = 0x00,
  /** The supplied request type was invalid. */
  ZB_ZDP_STATUS_INV_REQUESTTYPE = 0x80,
  /** The requested device did not exist on a device following a child descriptor request to
    * a parent.
    */
  ZB_ZDP_STATUS_DEVICE_NOT_FOUND = 0x81,
  /** The supplied endpoint was equal to 0x00 or between 0xf1 and 0xff. */
  ZB_ZDP_STATUS_INVALID_EP = 0x82,
  /** The requested endpoint is not described by a simple descriptor. */
  ZB_ZDP_STATUS_NOT_ACTIVE = 0x83,
  /** The requested optional feature is not supported on the target device. */
  ZB_ZDP_STATUS_NOT_SUPPORTED = 0x84,
  /** A timeout has occurred with the requested operation. */
  ZB_ZDP_STATUS_TIMEOUT = 0x85,
  /** The end device bind request was unsuccessful due to a failure to match any suitable clusters.
    */
  ZB_ZDP_STATUS_NO_MATCH = 0x86,
  /** The unbind request was unsuccessful due to the coordinator or source device not having an
    * entry in its binding table to unbind.
    */
  ZB_ZDP_STATUS_NO_ENTRY = 0x88,
  /** A child descriptor was not available following a discovery request to a parent. */
  ZB_ZDP_STATUS_NO_DESCRIPTOR = 0x89,
  /** The device does not have storage space to support the requested operation. */
  ZB_ZDP_STATUS_INSUFFICIENT_SPACE = 0x8a,
  /** The device is not in the proper state to support the requested operation. */
  ZB_ZDP_STATUS_NOT_PERMITTED = 0x8b,
  /** The device does not have table space to support the operation. */
  ZB_ZDP_STATUS_TABLE_FULL = 0x8c,
  /** The permissions configuration table on the target indicates that the request is not
    * authorized from this device.
    */
  ZB_ZDP_STATUS_NOT_AUTHORIZED = 0x8d,
  /** Custom internal statuses. */
  ZB_ZDP_STATUS_TIMEOUT_BY_STACK = 0xff,
}
zb_zdp_status_t;

/** @} */

/*! \addtogroup zb_comm_signals */
/*! @{ */
/**
   Signals passed to zboss_signal_handler()
 */
typedef enum zb_zdo_app_signal_type_e
{
  ZB_ZDO_SIGNAL_DEFAULT_START,  /*!< Default signal, for pre-R21 ZBOSS API
                                 * compatibility.
                                 *
                                 * The signal has Code+statusonly. */
  ZB_ZDO_SIGNAL_SKIP_STARTUP,   /*!< Notifies the application that ZBOSS framework (scheduler,
                                 * buffer pool, etc.) started, but no join/rejoin/formation/BDB
                                 * initialization is done yet.
                                 *
                                 * That signal is generated if application called
                                 * zb_zdo_start_no_autostart() instead of
                                 * zboss_start() before
                                 * zboss_main_loop(). Useful, for instance,
                                 * when some HW (like, sensor connected via UART
                                 * etc) must be initialized before
                                 * ZigBee commissioning, but having ZBOSS
                                 * scheduler running.
                                 *
                                 * The signal has Code+statusonly.
                                 *
                                 * @snippet sp_device.c signal_skip_startup
                                 * */
  ZB_ZDO_SIGNAL_DEVICE_ANNCE,   /*!< Inform App about receiving device_annce.
                                 *
                                 * The even has additional data of
                                 * zb_zdo_signal_device_annce_params_t type.
                                 *
                                 * @snippet simple_gw.c signal_device_annce
                                 */
  ZB_ZDO_SIGNAL_LEAVE,          /*!< The device itself has left the network.
                                 *
                                 * The evemnt has an additional data of type
                                 * zb_zdo_signal_leave_params_t.
                                 *
                                 * @snippet application/light_sample/light_control/light_control.c signal_leave
                                 */
  ZB_ZDO_SIGNAL_ERROR,          /*!< some mess in the buffer  */
  ZB_BDB_SIGNAL_DEVICE_FIRST_START, /*!< Device started and commissioned first
                                     * time after NVRAM erase.
                                     *
                                     * The signal has Code + Status only.
                                     * @snippet application/onoff_server/on_off_switch_zed.c signal_first
                                     */
  ZB_BDB_SIGNAL_DEVICE_REBOOT,      /*!< BDB initialization completed after
                                     * device reboot, use NVRAM contents during
                                     * initialization. Device joined/rejoined
                                     * and started.
                                     *
                                     * The signal has Code + Status only.
                                     * @snippet application/onoff_server/on_off_switch_zed.c signal_reboot
                                     */
  ZB_BDB_SIGNAL_TOUCHLINK_NWK_STARTED, /*!<Inform the Touchlink initiator that the new
                                     * Zigbee network created.
                                     * @snippet light_controller_zed.c signal_touchlink_nwk_started
                                     */
  ZB_BDB_SIGNAL_TOUCHLINK_NWK_JOINED_ROUTER, /*!<Inform the Touchlink initiator that the new
                                     * router joined the network.
                                     * @snippet light_controller_zed.c signal_touchlink_nwk_joined_router
                                     */
  ZB_BDB_SIGNAL_TOUCHLINK,          /*!< Touchlink commissioning done.  */
  ZB_BDB_SIGNAL_STEERING,           /*!< BDB steering completed.  */
  ZB_BDB_SIGNAL_FORMATION,          /*!< BDB formation completed.  */
  ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED,     /*!< BDB finding and binding target completed.  */
  ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED,  /*!< BDB finding and binding initiator completed.  */
  ZB_BDB_SIGNAL_TOUCHLINK_TARGET,   /*!< Touchlink Target started.  */
  ZB_BDB_SIGNAL_TOUCHLINK_NWK,      /*!< Touchlink Target Network
                                     * started.  */
  ZB_NWK_SIGNAL_DEVICE_ASSOCIATED,  /*!< New device associated to ZigBee
                                     * Coordinator or router. Note: that signal
                                     * exists due to historical reasons. Its
                                     * usage is not recommented. Base your
                                     * logic on ZB_ZDO_SIGNAL_DEVICE_ANNCE
                                     * instead. */
  ZB_ZDO_SIGNAL_LEAVE_INDICATION,   /*!< NWK leave indication from the child
                                     * received
                                     *
                                     * Has additional data of type
                                     * zb_zdo_signal_leave_indication_params_t.
                                     *
                                     * @snippet simple_gw.c signal_leave_ind
                                     **/

#if defined DOXYGEN && defined ZB_DOCUMENT_ZGP
/**
   @addtogroup zgp_sink
   @{
*/
#endif
  /**
     ZGP commissioning complete signal

     Called when GPCB is leaving commissioning mode.
     @see zb_zgp_signal_commissioning_params_t.

     @snippet zc_combo.c gcomm_complete
   */
  ZB_ZGP_SIGNAL_COMMISSIONING,
#if defined DOXYGEN && defined ZB_DOCUMENT_ZGP
/*! @} */
#endif

  ZB_COMMON_SIGNAL_CAN_SLEEP,       /*!< Notifies application that MCU can go to sleep.
                                     *
                                     * Passed up with parameter @ref zb_zdo_signal_can_sleep_params_t
                                     **/
  ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY, /*!< Notifies application that application specific part of
                                          * production configuration can be applied
                                          *
                                          * Passed up with application specific data from
                                          * production configuration.
                                          */
  /* ****************** */
  ZB_ZDO_APP_SIGNAL_COUNT
} zb_zdo_app_signal_type_t;
/* Legacy API: zb_zdo_app_signal_type_t == zb_zdo_app_signal_t */
typedef zb_zdo_app_signal_type_t zb_zdo_app_signal_t;

/**
   Obtains pointer to parameters passed with application signal.

   @param sg_p - pointer to application signal
   @param type - parameter type
   @return pointer to signal parameters

@b Example
@code
  zb_zdo_signal_device_annce_params_t *dev_annce_params = ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, zb_zdo_signal_device_annce_params_t);
  simple_gw_dev_annce_cb(dev_annce_params->device_short_addr);
@endcode
  */
#define ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, type) ((type *)(((zb_uint8_t *)sg_p) + sizeof(zb_zdo_app_signal_hdr_t)))

/**
   @brief Leave parameters

   Stack passes this parameter to application when device itself lefts network.
  */
typedef struct zb_zdo_signal_leave_params_s
{
  /**
    Type of leave: ZB_NWK_LEAVE_TYPE_RESET or ZB_NWK_LEAVE_TYPE_REJOIN.
    @see zb_nwk_leave_type_t
    */
  zb_uint8_t leave_type;
} zb_zdo_signal_leave_params_t;

/**
   @brief Leave Indication parameters

   Stack passes this parameter to application upon receipt leave.indication
   primitive.
  */
typedef struct zb_zdo_signal_leave_indication_params_s
{
  zb_ieee_addr_t device_addr; /*!< Long address of device requested to leave or leaving device*/
  zb_uint8_t rejoin;          /*!< 1 if this was leave with rejoin; 0 - otherwise */
} zb_zdo_signal_leave_indication_params_t;

#if defined DOXYGEN && defined ZB_DOCUMENT_ZGP
/**
   @addtogroup zgp_sink
   @{
*/
#endif

/**
   @brief ZGP commissioning parameters

   Stack passes this parameter to application to notify about GPD commissioning
   result.
  */
typedef struct zb_zgp_signal_commissioning_params_s
{
  zb_zgpd_id_t zgpd_id;       /*!< Pointer to GPD ID */
  zb_zgp_comm_status_t result; /*!< commissining result*/
} zb_zgp_signal_commissioning_params_t;
#if defined DOXYGEN && defined ZB_DOCUMENT_ZGP
/*! @} */
#endif

/**
   @brief Association parameters

   Stack passes this parameter to application when some device associates to us.
  */
typedef struct zb_nwk_signal_device_associated_params_s
{
  zb_ieee_addr_t device_addr; /*!< address of associated device */
} zb_nwk_signal_device_associated_params_t;

/**
   @brief Association parameters

   Stack passes this parameter to application when some device joins/rejoins to
   network.
  */
typedef struct zb_zdo_signal_device_annce_params_s
{
  zb_uint16_t device_short_addr; /*!< address of device that recently joined to network */
} zb_zdo_signal_device_annce_params_t;

typedef struct zb_bdb_signal_touchlink_nwk_started_params_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_started_params_t;

typedef struct zb_bdb_signal_touchlink_nwk_joined_router_s
{
  zb_ieee_addr_t device_ieee_addr; /*!< address of device that started the network */
  zb_uint8_t endpoint;
  zb_uint16_t profile_id;
} zb_bdb_signal_touchlink_nwk_joined_router_t;

/**
   @brief Sleep signal parameters.

   Stack passes this parameter to application when device ready to sleep.
  */
typedef struct zb_zdo_signal_can_sleep_params_s
{
  zb_uint32_t sleep_tmo; /*!< sleep duration in millisecionds */
} zb_zdo_signal_can_sleep_params_t;

/** Application signal header */
typedef struct zb_zdo_app_signal_hdr_s
{
  /* 32 bit to have data section aligned well */
  zb_uint32_t sig_type;            /** Application signal type, @see zb_zdo_app_signal_type_t */
} zb_zdo_app_signal_hdr_t;
/* Legacy API: zb_zdo_app_signal_hdr_t == zb_zdo_app_event_t */
typedef zb_zdo_app_signal_hdr_t zb_zdo_app_event_t;

/** @} */
/*! \addtogroup zdo_base */
/*! @{ */

#define ZB_ZDO_INVALID_TSN 0xFF

typedef ZB_PACKED_PRE struct zb_zdo_callback_info_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_callback_info_t;

typedef ZB_PACKED_PRE struct zb_zdo_default_resp_s
{
  zb_uint8_t tsn;
  zb_uint8_t status;
} ZB_PACKED_STRUCT zb_zdo_default_resp_t;

/** @} */
/*! \addtogroup zb_comm_signals */
/*! @{ */


/**
 * If any parameters are needed to be passed to the zboss_signal_handler, it
 * will be better to inherit zb_zdo_app_signal_t and introduce the structure
 * something like this:
 *
 * typedef struct zb_zdo_app_my_custom_signal_s
 * {
 *   zb_zdo_app_signal_t super;
 *   zb_my_custom_data_type_t my_custom_data;
 * }
 *
 */


/**
   Unpack application signal buffer in zboss_signal_handler()

   @param param - parameter of zboss_signal_handler()
   @param sg_p - pointer to the extended signal info, if application wants it. Can be NULL.

   @return application signal
 */
zb_zdo_app_signal_type_t zb_get_app_signal(zb_uint8_t param, zb_zdo_app_signal_hdr_t **sg_p);
/** @cond internals_doc */
/* Legacy API: zb_get_app_event(param, sg_p) == zb_get_app_signal(param, sg_p) */
#define zb_get_app_event(param, sg_p) zb_get_app_signal(param, sg_p)
/** @endcond */ /* internals_doc */

/**
   Get status from application signal
   @param param - buffer reference
   @return status, see @ref zb_ret_t
  */
#define ZB_GET_APP_SIGNAL_STATUS(param) (ZB_BUF_FROM_REF(param)->u.hdr.status)

/** @} */
/*! \addtogroup zdo_base */
/*! @{ */


/**
   Obtains last known LQI and RSSI values from device with specified short address

   @param short_address - address of device
   @param lqi [in] - pointer to @ref zb_uint8_t variable to store lqi value
   @param rssi [in] - pointer to @ref zb_uint8_t variable to store rssi value

@b Example:
@code
  zb_uint8_t lqi, rssi;

  zb_zdo_get_diag_data(0x0000, &lqi, &rssi);
@endcode
 */
zb_void_t zb_zdo_get_diag_data(zb_uint16_t short_address, zb_uint8_t *lqi, zb_int8_t *rssi);

/*! @} */


/*! \addtogroup zdo_addr */
/*! @{ */


/**
   2.4.3.1, 2.4.4.1
*/
#define ZB_ZDO_SINGLE_DEVICE_RESP   0 /*!< Single device response */
#define ZB_ZDO_EXTENDED_DEVICE_RESP 1 /*!< Extended response */

/** @brief NWK_addr_req command primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_req_s
{
  zb_ieee_addr_t   ieee_addr;    /*!< The IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_nwk_addr_req_t;

/** @brief Parameters for nwk_addr_req command */
typedef struct zb_zdo_nwk_addr_req_param_s
{
  zb_uint16_t      dst_addr;     /*!< Destinitions address */
  zb_ieee_addr_t   ieee_addr;    /*!< The IEEE address to be matched by the
                                   Remote Device  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
}
zb_zdo_nwk_addr_req_param_t;

/** @brief NWK_addr_req response frame. */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_head_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_head_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext_t;

/** @brief NWK_addr_req response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_nwk_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_nwk_addr_resp_ext2_t;

/** @brief Sends NWK_addr_req primitive.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param cb    - user's function to call when got response from the
                  remote. \ref zb_zdo_nwk_addr_resp_head_s passed to cb as parameter.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

@b Example:
@code
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t tsn;
  zb_zdo_nwk_addr_req_param_t *req_param = ZB_GET_BUF_PARAM(buf, zb_zdo_nwk_addr_req_param_t);

  req_param->dst_addr = 0;
  zb_address_ieee_by_ref(req_param->ieee_addr, short_addr);
  req_param->request_type = ZB_ZDO_SINGLE_DEVICE_RESP;
  req_param->start_index = 0;
  tsn = zb_zdo_nwk_addr_req(param, zb_get_peer_short_addr_cb);

  if (tsn == ZB_ZDO_INVALID_TSN)
  {
    TRACE_MSG(TRACE_ZDO2, "request failed", (FMT__0));
  }
}

void zb_get_peer_short_addr_cb(zb_uint8_t param)
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_nwk_addr_resp_head_t *resp;
  zb_ieee_addr_t ieee_addr;
  zb_uint16_t nwk_addr;
  zb_address_ieee_ref_t addr_ref;

  TRACE_MSG(TRACE_ZDO2, "zb_get_peer_short_addr_cb param %hd", (FMT__H, param));

  resp = (zb_zdo_nwk_addr_resp_head_t*)ZB_BUF_BEGIN(buf);
  TRACE_MSG(TRACE_ZDO2, "resp status %hd, nwk addr %d", (FMT__H_D, resp->status, resp->nwk_addr));
  ZB_DUMP_IEEE_ADDR(resp->ieee_addr);

  // additionally check tsn if needed
  //if (resp->tsn == my_saved_tsn)

  if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    ZB_LETOH64(ieee_addr, resp->ieee_addr);
    ZB_LETOH16(&nwk_addr, &resp->nwk_addr);
    zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
  }
  zb_free_buf(buf);
}
@endcode

See tp_zdo_bv-31 sample
*/
zb_uint8_t zb_zdo_nwk_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/**
   @brief Broadcast NWK_addr_req primitive.
   Used internally in stack.

   @param param - index of buffer with primitive parameters - \ref zb_zdo_nwk_addr_req_param_s
   @param ieee_addr - The IEEE address to be matched by the Remote Device
*/
zb_uint8_t zb_zdo_initiate_nwk_addr_req(zb_uint8_t param, zb_ieee_addr_t ieee_addr);
/* Used internally in stack. */
void zb_zdo_initiate_nwk_addr_req_2param(zb_uint8_t param, zb_uint16_t user_param);
/** @endcond */ /* internals_doc */

/** @brief Parameters of IEEE_addr_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       request_type; /*!< Request type for this command:
                                   0x00  Single device response
                                   0x01  Extended response  */
  zb_uint8_t       start_index; /*!< If the Request type for this command is
                                  Extended response, the StartIndex
                                  provides the starting index for the
                                  requested elements of the associated
                                  devices list  */
} ZB_PACKED_STRUCT zb_zdo_ieee_addr_req_t;

/** brief ZDO IEEE address response frame */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the NWK_addr_req command. */
  zb_ieee_addr_t ieee_addr_remote_dev; /*!< 64-bit address for the Remote Device. */
  zb_uint16_t nwk_addr_remote_dev; /*!< 16-bit address for the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_t;

/** brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext_s
{
  zb_uint8_t num_assoc_dev; /*!< Count of the ED List. */
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext_t;

/** brief ZDO IEEE address response frame tail */
typedef ZB_PACKED_PRE struct zb_zdo_ieee_addr_resp_ext2_s
{
  zb_uint8_t start_index; /*!< Starting index to begin reporting for the ED List.*/
}
ZB_PACKED_STRUCT
zb_zdo_ieee_addr_resp_ext2_t;

/** @brief IEEE_addr_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_ieee_addr_req_s.
  * Parameters mut be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @code
  * {
  *   zb_zdo_ieee_addr_req_t *req = NULL;
  *   zb_uint8_t tsn;
  *
  *   ZB_BUF_INITIAL_ALLOC(buf, sizeof(zb_zdo_ieee_addr_req_t), req);
  *
  *   req->nwk_addr = ind->src_addr;
  *   req->request_type = ZB_ZDO_SINGLE_DEV_RESPONSE;
  *   req->start_index = 0;
  *   tsn = zb_zdo_ieee_addr_req(ZB_REF_FROM_BUF(buf), ieee_addr_callback);
  *
  *   if (tsn == ZB_ZDO_INVALID_TSN)
  *   {
  *     TRACE_MSG(TRACE_ZDO2, "request failed", (FMT__0));
  *   }
  * }
  *
  * void ieee_addr_callback(zb_uint8_t param)
  * {
  *   zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  *   zb_zdo_nwk_addr_resp_head_t *resp;
  *   zb_ieee_addr_t ieee_addr;
  *   zb_uint16_t nwk_addr;
  *   zb_address_ieee_ref_t addr_ref;
  *
  *   TRACE_MSG(TRACE_ZDO2, "zb_get_peer_short_addr_cb param %hd", (FMT__H, param));
  *
  *   resp = (zb_zdo_nwk_addr_resp_head_t*)ZB_BUF_BEGIN(buf);
  *   TRACE_MSG(
  *       TRACE_ZDO2, "resp status %hd, nwk addr %d", (FMT__H_D, resp->status, resp->nwk_addr));
  *   ZB_DUMP_IEEE_ADDR(resp->ieee_addr);
  *   if (resp->status == ZB_ZDP_STATUS_SUCCESS)
  *   {
  *     ZB_LETOH64(ieee_addr, resp->ieee_addr);
  *     ZB_LETOH16(&nwk_addr, &resp->nwk_addr);
  *     zb_address_update(ieee_addr, nwk_addr, ZB_TRUE, &addr_ref);
  *   }
      zb_free_buf(buf);
  * }
  *
  * @endcode
  *
  * See tp_zdo_bv-31 sample
  */
zb_uint8_t zb_zdo_ieee_addr_req(zb_uint8_t param, zb_callback_t cb);

/** @cond internals_doc */
/* Used internally in stack. */
zb_uint8_t zb_zdo_initiate_ieee_addr_req(zb_uint8_t param, zb_uint16_t nwk_addr);
/** @endcond */ /* internals_doc */

/** @} */

/**
   @addtogroup zdo_disc
   @{
*/

/** @brief Parameters of Node_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_node_desc_req_t;


/** @brief Header of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @see zb_zdp_status_t */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
} ZB_PACKED_STRUCT
zb_zdo_desc_resp_hdr_t;

/** @brief Parameters of Node_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_node_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_desc_t node_desc; /*!< Node Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_node_desc_resp_t;

/** @brief Header of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_hdr_s
{
  zb_uint8_t      tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @see zb_zdp_status_t */
  zb_uint16_t     nwk_addr; /*!< NWK address for the request  */
  zb_uint8_t      length;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_hdr_t;

#ifdef ZB_FIXED_OPTIONAL_DESC_RESPONSES
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_complex_desc_resp_hdr_t;
typedef zb_zdo_simple_desc_resp_hdr_t     zb_zdo_user_desc_resp_hdr_t;
typedef zb_zdo_desc_resp_hdr_t            zb_zdo_user_desc_conf_hdr_t;
#endif

/** @brief Parameters of simple_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_resp_s
{
  zb_zdo_simple_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_simple_desc_1_1_t simple_desc; /*!< Simple Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_simple_desc_resp_t;


typedef ZB_PACKED_PRE struct zb_zdo_child_info_s
{
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
}ZB_PACKED_STRUCT
zb_zdo_child_info_t;

/** Jitter used for sending Parent Annce */
#define ZB_PARENT_ANNCE_JITTER() (ZB_APS_PARENT_ANNOUNCE_BASE_TIMER + ZB_RANDOM_JTR(ZB_APS_PARENT_ANNOUNCE_JITTER_MAX))


/** @brief Header of parent_annce primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_s
{
  zb_uint8_t      tsn;               /*!< ZDO sequence number */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_t;


/** @brief Header of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_hdr_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t      status;   /*!< The status of the Desc_req command. @see zb_zdp_status_t */
  zb_uint8_t      num_of_children;   /*!< Length of the simple descriptor */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_hdr_t;

/** @brief Parameters of parent_annce_rsp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_parent_annce_rsp_s
{
  zb_zdo_parent_annce_rsp_hdr_t hdr;  /*!< header for response */
} ZB_PACKED_STRUCT
zb_zdo_parent_annce_rsp_t;

/** @brief Parameters of Power_desc_resp primitive.  */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_resp_s
{
  zb_zdo_desc_resp_hdr_t hdr;  /*!< header for response */
  zb_af_node_power_desc_t power_desc; /*!< Power Descriptor */
} ZB_PACKED_STRUCT
zb_zdo_power_desc_resp_t;

/** @brief Node_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_node_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet tp_zdo_bv_09_zed1.c zb_zdo_node_desc_req
  *
  * See tp_zdo_bv_09 sample
  */
zb_uint8_t zb_zdo_node_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Power_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_power_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_power_desc_req_t;

/** @brief Power_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_power_desc_req_s.
  * Parameters must be put into buffer as data (allocated).
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet tp_zdo_bv_09_zed1.c zb_zdo_power_desc_req
  *
  * See tp_zdo_bv_09 sample
  */
zb_uint8_t zb_zdo_power_desc_req(zb_uint8_t param, zb_callback_t cb);



/** @brief Parameters of Simple_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_simple_desc_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint8_t       endpoint;    /*!< The endpoint on the destination  */
} ZB_PACKED_STRUCT zb_zdo_simple_desc_req_t;

/** @brief Simple_desc_req primitive.
  *
  * @param param - index of buffer with primitive parameters \ref zb_zdo_simple_desc_req_s.
  * @param cb    - user's function to call when got response from the remote.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * @b Example:
  * @snippet tp_zdo_bv_09_zed1.c zb_zdo_simple_desc_req
  *
  * See tp_zdo_bv_09 sample
  */
zb_uint8_t zb_zdo_simple_desc_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters of Active_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_active_ep_req_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
} ZB_PACKED_STRUCT zb_zdo_active_ep_req_t;


/** @brief Active EP response */
typedef ZB_PACKED_PRE struct zb_zdo_ep_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Active_EP_req command. */
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t ep_count;  /*!< The count of active endpoints on the Remote Device. */
}
ZB_PACKED_STRUCT
zb_zdo_ep_resp_t;

/** @brief Active_desc_req primitive.

   @param param - index of buffer with primitive parameters \ref zb_zdo_active_ep_req_s. Parameters must be
   put into buffer as data (allocated).
   @param cb    - user's function to call when got response from the remote.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
  * @snippet tp_zdo_bv_09_zed1.c zb_zdo_active_ep_req
  *
  * See tp_zdo_bv_09 sample
*/
zb_uint8_t zb_zdo_active_ep_req(zb_uint8_t param, zb_callback_t cb);

/** @brief Parameters of match_desc_req primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_param_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint16_t     addr_of_interest; /*!< NWK address of interest */

  zb_uint16_t      profile_id;  /*!< Profile ID to be matched at the
                                  destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                      provided for matching within the
                                      InClusterList.  */
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
  zb_uint16_t      cluster_list[1]; /*!< variable size: [num_in_clusters] +  [num_out_clusters]
                                         List of Input ClusterIDs to be used
                                         for matching; the InClusterList is
                                         the desired list to be matched by
                                         the Remote Device (the elements
                                         of the InClusterList are the
                                         supported output clusters of the
                                         Local Device).
                                         List of Output ClusterIDs to be
                                         used for matching; the
                                         OutClusterList is the desired list to
                                         be matched by the Remote Device
                                         (the elements of the
                                         OutClusterList are the supported
                                         input clusters of the Local
                                         Device). */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_param_t;

/** @brief Match_desc_req head */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_head_s
{
  zb_uint16_t      nwk_addr;    /*!< NWK address that is used for IEEE
                                  address mapping.  */
  zb_uint16_t      profile_id;  /*!< Profile ID to be matched at the
                                  destination.  */
  zb_uint8_t       num_in_clusters; /*!< The number of Input Clusters
                                      provided for matching within the
                                      InClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_head_t;

/** @brief Match_desc_req tail */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_req_tail_s
{
  zb_uint8_t       num_out_clusters; /*!< The number of Output Clusters
                                       provided for matching within
                                       OutClusterList.  */
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_req_tail_t;

/** @brief Match_Desc_rsp response structure
  * @see ZB spec, subclause 2.4.4.1.7
  */
typedef ZB_PACKED_PRE struct zb_zdo_match_desc_resp_s
{
  zb_uint8_t tsn;       /*!< ZDP transaction sequence number */
  zb_uint8_t status;    /*!< The status of the Match_Desc_req command.*/
  zb_uint16_t nwk_addr; /*!< NWK address for the request. */
  zb_uint8_t match_len; /*!< The count of endpoints on the Remote Device that match the
                          request criteria.*/
}
ZB_PACKED_STRUCT
zb_zdo_match_desc_resp_t;


/** @brief Match_desc_req primitive.

   @param param - index of buffer with primitive parameters \ref zb_zdo_match_desc_param_s.
   @param cb    - user's function to call when got response from the remote. If command is \n
                  broadcast, then user's function will be called as many times as number of \n
                  responses received plus one more time with status \ref ZB_ZDP_STATUS_TIMEOUT
                  to indicate that no more responses will be received.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
   @snippet tp_zdo_bv_09_zed1.c zb_zdo_match_desc_req

   See tp_zdo_bv_09 sample
*/
zb_uint8_t zb_zdo_match_desc_req(zb_uint8_t param, zb_callback_t cb);


#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
/** @brief Request parameters for System_Server_Discovery_req.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_req_s
{
  zb_uint16_t server_mask; /*!< Server mask for device discovery */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_req_t;

/** @brief Parameters for System_Server_Discovery_req call.
  * @see ZB spec, subclause 2.4.3.1.13.
  */
typedef zb_zdo_system_server_discovery_req_t zb_zdo_system_server_discovery_param_t;


/** @brief Response parameters for System_Server_Discovery_rsp.
  * @see ZB spec, subclause 2.4.4.1.10.
  */
typedef ZB_PACKED_PRE struct zb_zdo_system_server_discovery_resp_s
{
  zb_uint8_t tsn;          /*!< ZDP transaction sequence number */
  zb_uint8_t status;       /*!< Status of the operation */
  zb_uint16_t server_mask; /*!< Mask of the supported features */
}
ZB_PACKED_STRUCT
zb_zdo_system_server_discovery_resp_t;


/** @brief Performs System_Server_Discovery_req
   @param param - index of buffer with request parameters
   \ref zb_zdo_system_server_discovery_param_t
   @param cb - user's function to call when got response from the remote. \ref
    zb_zdo_system_server_discovery_resp_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
  @snippet tr_pro_bv_36_zr.c zb_zdo_system_server_discovery_req

See tp_pro_bv_36 sample

 */
zb_uint8_t zb_zdo_system_server_discovery_req(zb_uint8_t param, zb_callback_t cb);
#endif  /* #ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY */

/*! @} */

/*! \addtogroup zdo_mgmt */
/*! @{ */


/** @brief Header of parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_hdr_s
{
  zb_uint32_t scan_channels;   /*!< Channels bitmask */
  zb_uint8_t scan_duration;    /*!< A value used to calculate the
                               * length of time to spend scanning
                               * each channel. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_hdr_t;

/** @brief Parameters for Mgmt_NWK_Update_req */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_req_s
{
  zb_zdo_mgmt_nwk_update_req_hdr_t hdr; /*!< Request header */
  zb_uint8_t scan_count;       /*!< This field represents the number
                                * of energy scans to be conducted and reported */
  zb_uint8_t update_id;     /*!< This value is set by the Network
                               * Channel Manager prior to sending
                               * the message. This field shall only
                               * be present of the ScanDuration is 0xfe or 0xff */
  zb_uint16_t manager_addr; /*!< This field shall be present only
                               * if the ScanDuration is set to 0xff,
                               * and, where present, indicates the
                               * NWK address for the device with the
                               * Network Manager bit set in its Node Descriptor. */
  zb_uint16_t dst_addr;     /*!< Destinition address */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_req_t;


/** @brief Header parameters for mgmt_nwk_update_notify */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_nwk_update_notify_hdr_s
{
  zb_uint8_t tsn;      /*!< ZDP transaction sequence number */
  zb_uint8_t status;     /*!< The status of the Mgmt_NWK_Update_notify command. */
  zb_uint32_t scanned_channels;  /*!< List of channels scanned by the request */
  zb_uint16_t total_transmissions;  /*!< Count of the total transmissions reported by the device */
  zb_uint16_t transmission_failures;  /*!< Sum of the total transmission failures reported by the
                                            device */
  zb_uint8_t scanned_channels_list_count;  /*!< The list shall contain the number of records
                                            * contained in the EnergyValues parameter. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_nwk_update_notify_hdr_t;

/** @brief Parameters for mgmt_nwk_update_notify */
typedef struct zb_zdo_mgmt_nwk_update_notify_param_s
{
  zb_zdo_mgmt_nwk_update_notify_hdr_t hdr;             /*!< Fixed parameters set */
  zb_uint8_t energy_values[ZB_MAC_SUPPORTED_CHANNELS]; /*!< ed scan values */
  zb_uint16_t dst_addr;  /*!< destinition address */
  zb_uint8_t tsn;        /*!< tsn value */
}
zb_zdo_mgmt_nwk_update_notify_param_t;

/** @brief Performs Mgmt_NWK_Update_req request

   @param param - index of buffer with call parameters. Parameters mut be
   put into buffer as parameters. \ref zb_zdo_mgmt_nwk_update_req_s
   @param cb    - user's function to call when got response from the remote.
   \ref zb_zdo_mgmt_nwk_update_notify_hdr_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_uint8_t tsn;
  zb_zdo_mgmt_nwk_update_req_t *req;

  req = ZB_GET_BUF_PARAM(buf, zb_zdo_mgmt_nwk_update_req_t);

  req->hdr.scan_channels = ZB_MAC_ALL_CHANNELS_MASK;
  req->hdr.scan_duration = TEST_SCAN_DURATION;
  req->scan_count = TEST_SCAN_COUNT;
  req->update_id = ZB_NIB_UPDATE_ID();

  req->dst_addr = 0;

  tsn = zb_zdo_mgmt_nwk_update_req(param, mgmt_nwk_update_ok_cb);
}


void mgmt_nwk_update_ok_cb(zb_uint8_t param)
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t *zdp_cmd = ZB_BUF_BEGIN(buf);
  zb_zdo_mgmt_nwk_update_notify_hdr_t *notify_resp = (zb_zdo_mgmt_nwk_update_notify_hdr_t *)zdp_cmd;

  TRACE_MSG(TRACE_APS3,
            "notify_resp status %hd, scanned_channels %x %x, total_transmissions %hd, "
            "transmission_failures %hd, scanned_channels_list_count %hd, buf len %hd",
            (FMT__H_D_D_H_H_H_H, notify_resp->status, (zb_uint16_t)notify_resp->scanned_channels,
             *((zb_uint16_t*)&notify_resp->scanned_channels + 1),
             notify_resp->total_transmissions, notify_resp->transmission_failures,
             notify_resp->scanned_channels_list_count, ZB_BUF_LEN(buf)));

  if (notify_resp->status == ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_APS3, "mgmt_nwk_update_notify received, Ok", (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "mgmt_nwk_update_notify received, ERROR incorrect status %x",
              (FMT__D, notify_resp->status));
  }

  zb_free_buf(buf);
}
@endcode

See TP_PRO_BV-37 sample
 */
zb_uint8_t zb_zdo_mgmt_nwk_update_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Parameters for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef struct zb_zdo_mgmt_lqi_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_lqi_param_t;

/** @brief Request for Mgmt_Lqi_req.
  * @see ZB spec, subclause 2.4.3.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Neighbor Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_req_t;

/** @brief Response for Mgmt_Lqi_rsp.
  * @see ZB spec, subclause 2.4.4.3.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_lqi_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the Mgmt_Lqi_req command.*/
  zb_uint8_t neighbor_table_entries; /*!< Total number of Neighbor
                                      * Table entries within the Remote Device */
  zb_uint8_t start_index; /*!< Starting index within the Neighbor
                           * Table to begin reporting for the NeighborTableList.*/
  zb_uint8_t neighbor_table_list_count; /*!< Number of Neighbor Table
                                         * entries included within NeighborTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_lqi_resp_t;


/* bits 0 - 1, mask 0x3 */
/**
   Set device type of neighbor table record to type 'type': bits 0 - 1, mask 0x3;

   @param var - neighbor table record type_flags
   @param type - Zigbee device type value

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_DEVICE_TYPE(var, type) ( var &= ~3, var |= type )
/**
   Get device type of neighbor table record.

   @param var - neighbor table record type_flags

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_DEVICE_TYPE(var) ( var & 3 )

/* bits 2 - 3, mask 0xC */
/**
   Set RxOnWhenIdle attribute of neighbor table record to type 'type':
   bits 2 - 3, mask 0xC;

   @param var - neighbor table record type_flags
   @param type - RxOnWhenIdle value

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RX_ON_WHEN_IDLE(var, type) ( var &= ~0xC, var |= (type << 2) )
/**
   Get RxOnWhenIdle of neighbor table record.

   @param var - neighbor table record type_flags

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RX_ON_WHEN_IDLE(var) ( (var & 0xC) >> 2 )

/* bits 4 - 6, mask 0x70 */
/**
   Set relationship attribute of neighbor table record to type 'type':
   bits 4 - 6, mask 0x70;

   @param var - neighbor table record type_flags
   @param type - Zigbee relationship value

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_SET_RELATIONSHIP(var, type) ( var &= ~0x70, var |= (type << 4) )
/**
   Get relationship of neighbor table record.

   @param var - neighbor table record type_flags

   @see @ref zb_zdo_neighbor_table_record_s
  */
#define ZB_ZDO_RECORD_GET_RELATIONSHIP(var) ( (var & 0x70) >> 4 )

/** @brief NeighborTableList Record Format for Mgmt_Lqi_resp */
typedef ZB_PACKED_PRE struct zb_zdo_neighbor_table_record_s
{
  zb_ext_pan_id_t ext_pan_id;   /*!< The 64-bit extended PAN
                                 * identifier of the neighboring device.*/
  zb_ieee_addr_t  ext_addr;     /*!< 64-bit IEEE address that is
                                 * unique to every device.*/
  zb_uint16_t     network_addr; /*!< The 16-bit network address of the
                                 * neighboring device */
  zb_uint8_t      type_flags;   /*!< device type, rx_on_when_idle,
                                 * relationship */
  zb_uint8_t      permit_join;  /*!< An indication of whether the
                                 * neighbor device is accepting join requests*/
  zb_uint8_t      depth;        /*!< The tree depth of the neighbor device. */
  zb_uint8_t      lqi;          /*!< The estimated link quality for RF
                                 * transmissions from this device */
}
ZB_PACKED_STRUCT
zb_zdo_neighbor_table_record_t;


/** @brief Sends 2.4.3.3.2 Mgmt_Lqi_req
   @param param - index of buffer with Lqi request parameters. \ref zb_zdo_mgmt_lqi_param_s
   @param cb    - user's function to call when got response from the remote.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)
   @ref zb_zdo_mgmt_lqi_resp_s, \ref zb_zdo_neighbor_table_record_s

   @b Example:
@code
{
  zb_uint8_t tsn;
  zb_zdo_mgmt_lqi_param_t *req_param;

  req_param = ZB_GET_BUF_PARAM(buf, zb_zdo_mgmt_lqi_param_t);
  req_param->start_index = 0;
  req_param->dst_addr = 0; //coord short addr

  tsn = zb_zdo_mgmt_lqi_req(ZB_REF_FROM_BUF(buf), get_lqi_cb);
}


void get_lqi_cb(zb_uint8_t param)
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_uint8_t *zdp_cmd = ZB_BUF_BEGIN(buf);
  zb_zdo_mgmt_lqi_resp_t *resp = (zb_zdo_mgmt_lqi_resp_t*)(zdp_cmd);
  zb_zdo_neighbor_table_record_t *record = (zb_zdo_neighbor_table_record_t*)(resp + 1);
  zb_uint_t i;

  TRACE_MSG(TRACE_APS1, "get_lqi_cb status %hd, neighbor_table_entries %hd, start_index %hd, neighbor_table_list_count %d",
            (FMT__H_H_H_H, resp->status, resp->neighbor_table_entries, resp->start_index, resp->neighbor_table_list_count));

  for (i = 0; i < resp->neighbor_table_list_count; i++)
  {
    TRACE_MSG(TRACE_APS1, "#%hd: long addr " TRACE_FORMAT_64 " pan id " TRACE_FORMAT_64,
              (FMT__H_A_A, i, TRACE_ARG_64(record->ext_addr), TRACE_ARG_64(record->ext_pan_id)));

    TRACE_MSG(TRACE_APS1,
      "#%hd: network_addr %d, dev_type %hd, rx_on_wen_idle %hd, relationship %hd, permit_join %hd, depth %hd, lqi %hd",
      (FMT_H_D_H_H_H_H_H_H, i, record->network_addr,
       ZB_ZDO_RECORD_GET_DEVICE_TYPE(record->type_flags),
       ZB_ZDO_RECORD_GET_RX_ON_WHEN_IDLE(record->type_flags),
       ZB_ZDO_RECORD_GET_RELATIONSHIP(record->type_flags),
       record->permit_join, record->depth, record->lqi));

    record++;
  }
}

@endcode

See zdpo_lqi sample
*/
zb_uint8_t zb_zdo_mgmt_lqi_req(zb_uint8_t param, zb_callback_t cb);

/** @brief RoutingTableList Record Format for mgmt_rtg_resp */
typedef ZB_PACKED_PRE struct zb_zdo_routing_table_record_s
{
  zb_uint16_t     dest_addr; /*!< The 16-bit network address of the
                                 * destination device */
  zb_uint8_t      flags;        /*!< Routing flags */
  zb_uint16_t     next_hop_addr; /*!< The 16-bit network address of the
                                 * next-hop device */
}
ZB_PACKED_STRUCT
zb_zdo_routing_table_record_t;

/** @} */
/** @addtogroup zdo_bind
    @{
*/

/** @brief Parameters for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef struct zb_zdo_mgmt_bind_param_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
  zb_uint16_t dst_addr;   /*!< destination address */
}
zb_zdo_mgmt_bind_param_t;

/** @brief Request for Mgmt_Bind_req.
  * @see ZB spec, subclause 2.4.3.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_req_s
{
  zb_uint8_t start_index; /*!< Starting Index for the requested elements
                           * of the Binding Table */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_req_t;

/** @brief Response for Mgmt_Bind_rsp.
  * @see ZB spec, subclause 2.4.4.3.4.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /*!< The status of the Mgmt_Rtg_req command.*/
  zb_uint8_t binding_table_entries; /*!< Total number of Binding Table
                                     * entries within the Remote Device*/
  zb_uint8_t start_index; /*!< Starting index within the Binding
                           * Table to begin reporting for the BindingTableList.*/
  zb_uint8_t binding_table_list_count; /*!< Number of Binding Table
                                         * entries included within BindingTableList*/
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_bind_resp_t;


/** @brief BindingTableList Record Format for mgmt_bind_resp. See ZB
 * spec, Table 2.131 */
typedef ZB_PACKED_PRE struct zb_zdo_binding_table_record_s
{
  zb_ieee_addr_t src_address; /*!< The source IEEE address for the binding entry. */
  zb_uint8_t src_endp;  /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;  /*!< The identifier of the cluster on the
                             source device that is bound to the
                             destination device. */
  zb_uint8_t dst_addr_mode;  /*!< The addressing mode for the
                               destination address used in this
                               command. This field can take one of
                               the non-reserved values from the
                               following list:
                               0x00 = reserved
                               0x01 = 16-bit group address for
                               DstAddress and DstEndp not present
                               0x02 = reserved
                               0x03 = 64-bit extended address for
                               DstAddress and DstEndp present
                               0x04 . 0xff = reserved*/
  zb_addr_u dst_address;  /*!< The destination address for the
                                 * binding entry.16 or 64 bit. As specified by the
                                 * dst_addr_mode field.*/
  zb_uint8_t dst_endp;  /*!< This field shall be present only if the
                          DstAddrMode field has a value of
                          0x03 and, if present, shall be the
                          destination endpoint for the binding
                          entry. */
}
ZB_PACKED_STRUCT
zb_zdo_binding_table_record_t;

/** @brief Sends Mgmt_Bind_req request.
  * @param param reference to the buffer to put request data to.
  * @param cb callback to be called on operation finish.
  * @return        - ZDP transaction sequence number or 0xFF if operation cannot be
  *                performed now (nor enough memory, resources, etc.)
  *
  * See zdo_binding sample
  */
zb_uint8_t zb_zdo_mgmt_bind_req(zb_uint8_t param, zb_callback_t cb);

/**
   @brief Sends 2.4.4.3.4 Mgmt_Bind_rsp
   @param param - index of buffer with Mgmt_Bind request
 */
void zdo_mgmt_bind_resp(zb_uint8_t param);




/** @brief Parameters for Bind_req API call
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef struct zb_zdo_bind_req_param_s
{
  zb_ieee_addr_t src_address; /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;  /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;  /*!< The identifier of the cluster on the
                             source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;  /*!< The addressing mode for the
                               destination address used in this
                               command. This field can take one of
                               the non-reserved values from the
                               following list:
                               0x00 = reserved
                               0x01 = 16-bit group address for
                               DstAddress and DstEndp not present
                               0x02 = reserved
                               0x03 = 64-bit extended address for
                               DstAddress and DstEndp present
                               0x04 . 0xff = reserved*/
  zb_addr_u dst_address;  /*!< The destination address for the
                                 * binding entry. */
  zb_uint8_t dst_endp;  /*!< This field shall be present only if the
                          DstAddrMode field has a value of
                          0x03 and, if present, shall be the
                          destination endpoint for the binding
                          entry. */
  zb_uint16_t req_dst_addr; /*!< Destinition address of the request */
}
zb_zdo_bind_req_param_t;


/** @brief Bind_req request head send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_head_s
{
  zb_ieee_addr_t src_address; /*!< The IEEE address for the source. */
  zb_uint8_t src_endp;  /*!< The source endpoint for the binding entry. */
  zb_uint16_t cluster_id;  /*!< The identifier of the cluster on the
                             source device that is bound to the destination. */
  zb_uint8_t dst_addr_mode;  /*!< The addressing mode for the
                               destination address used in this
                               command. This field can take one of
                               the non-reserved values from the
                               following list:
                               0x00 = reserved
                               0x01 = 16-bit group address for
                               DstAddress and DstEndp not present
                               0x02 = reserved
                               0x03 = 64-bit extended address for
                               DstAddress and DstEndp present
                               0x04 . 0xff = reserved*/
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_head_t;

/** @brief Bind_req request tail 1st variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_1_s
{
  zb_uint16_t dst_addr; /*!< The destination address for the
                         * binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_1_t;

/** @brief Bind_req request tail 2nd variant send to the remote.
  * @see ZB spec, subclause 2.4.3.2.2.
  */
typedef ZB_PACKED_PRE struct zb_zdo_bind_req_tail_2_s
{
  zb_ieee_addr_t dst_addr; /*!< The destination address for the
                            * binding entry. */
  zb_uint8_t dst_endp;  /*!< The destination address for the
                         * binding entry. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_req_tail_2_t;

/** @brief Response by BibdReq. */
typedef ZB_PACKED_PRE struct zb_zdo_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_bind_resp_t;


/** @brief Bind_req request.

   @param param - index of buffer with request. \ref zb_apsme_binding_req_s
   @param cb    - user's function to call when got response from the
   remote. \ref zb_zdo_bind_resp_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_apsme_binding_req_t *req;

  req = ZB_GET_BUF_PARAM(buf, zb_apsme_binding_req_t);
  ZB_MEMCPY(&req->src_addr, &g_ieee_addr, sizeof(zb_ieee_addr_t));
  req->src_endpoint = i;
  req->clusterid = 1;
  req->addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
  ZB_MEMCPY(&req->dst_addr.addr_long, &g_ieee_addr_d, sizeof(zb_ieee_addr_t));
  req->dst_endpoint = 240;

  zb_zdo_bind_req(ZB_REF_FROM_BUF(buf), zb_bind_callback);
  ret = buf->u.hdr.status;
  if (ret == RET_TABLE_FULL)
  {
    TRACE_MSG(TRACE_ERROR, "TABLE FULL %d", (FMT__D, ret));
  }
}

void zb_bind_callback(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_buf_t *buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
  zb_uint8_t *aps_body = NULL;

  aps_body = ZB_BUF_BEGIN(buf);
  ZB_MEMCPY(&ret, aps_body, sizeof(ret));

  TRACE_MSG(TRACE_INFO1, "zb_bind_callback %hd", (FMT__H, ret));
  if (ret == RET_OK)
  {
    // bind ok
  }
}
@endcode

See tp_zdo_bv-12 sample
 */
zb_uint8_t zb_zdo_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Unbind_req request.

   @param param - index of buffer with request. @ref zb_zdo_bind_req_param_s
   @param cb    - user's function to call when got response from the
   remote. @ref zb_zdo_bind_resp_s
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @b Example:
@code
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_bind_req_param_t *bind_param;

  TRACE_MSG(TRACE_COMMON1, "unbind_device_1", (FMT__0));

  zb_buf_initial_alloc(buf, 0);
  bind_param = ZB_GET_BUF_PARAM(buf, zb_zdo_bind_req_param_t);
  ZB_MEMCPY(bind_param->src_address, g_ieee_addr_ed1, sizeof(zb_ieee_addr_t));
  bind_param->src_endp = TEST_ED1_EP;
  bind_param->cluster_id = TP_BUFFER_TEST_REQUEST_CLID;
  bind_param->dst_addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
  ZB_MEMCPY(bind_param->dst_address.addr_long, g_ieee_addr_ed2, sizeof(zb_ieee_addr_t));
  bind_param->dst_endp = TEST_ED2_EP;
  bind_param->req_dst_addr = zb_address_short_by_ieee(g_ieee_addr_ed1);
  TRACE_MSG(TRACE_COMMON1, "dst addr %d", (FMT__D, bind_param->req_dst_addr));

  zb_zdo_unbind_req(param, unbind_device1_cb);
}


void unbind_device1_cb(zb_uint8_t param)
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_bind_resp_t *bind_resp = (zb_zdo_bind_resp_t*)ZB_BUF_BEGIN(buf);

  TRACE_MSG(TRACE_COMMON1, "unbind_device1_cb resp status %hd", (FMT__H, bind_resp->status));
  if (bind_resp->status != ZB_ZDP_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_COMMON1, "Error bind device 1. Test status failed", (FMT__0));
  }
  zb_free_buf(buf);

}
@endcode

  See tp_aps_bv-23-i, tp_zdo_bv-12 samples
 */
zb_uint8_t zb_zdo_unbind_req(zb_uint8_t param, zb_callback_t cb);

/**
 * @brief Checks existance of  bind table entries with selected endpoint and cluster ID.
 *
 * @param src_end - source endpoint
 * @param cluster_id - source cluster ID
 *
 * @return ZB_TRUE if binding is found on given endpoint, ZB_FALSE otherwise
 */
zb_bool_t zb_zdo_find_bind_src(zb_uint8_t src_end, zb_uint16_t cluster_id);

/** @} */

/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Request for Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.5.

   Problem in the specification:
   in 2.4.3.3.5  Mgmt_Leave_req only one DeviceAddress exists.
   But, in such case it is impossible to satisfy 2.4.3.3.5.1:
   "The Mgmt_Leave_req is generated from a Local Device requesting that a Remote
   Device leave the network or to request that another device leave the network."
   Also, in the PRO TC document, 14.2TP/NWK/BV-04 ZR-ZDO-APL RX Join/Leave is
   following note:
   "gZC sends Mgmt_Leave.request with DevAddr=all zero, DstAddr=ZR"
 */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_param_s
{
  zb_ieee_addr_t device_address;   /*!< 64 bit IEEE address */
  zb_uint16_t    dst_addr;          /*!< destinition address. Not defined in
                                     * the spac - let's it be short address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1; /*!< Remove children */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_param_t;

/** Request for Mgmt_Leave_req.
  * @see ZB spec, subclause 2.4.3.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_req_s
{
  zb_ieee_addr_t device_address;   /*!< 64 bit IEEE address */
  zb_bitfield_t reserved:6;        /*!< Reserve */
  zb_bitfield_t remove_children:1; /*!< Remove children */
  zb_bitfield_t rejoin:1;          /*!< Rejoin */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_req_t;

/** @brief Response for Mgmt_Leave_rsp.
  * @see ZB spec, subclause 2.4.4.3.5.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_leave_res_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_leave_res_t;

/** @brief Sends Mgmt_Leave_req
    @see ZB spec, subclause 2.4.3.3.2.

   @param param - index of buffer with Lqi request parameters. @ref zb_zdo_mgmt_leave_param_s
   @param cb    - user's function to call when got response from the remote.
   @return        - transaction sequence number of request or 0xFF if operation not be
                  performed right now (in case if not exist free slot for registering callback)

   @b Example:
@code
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  zb_zdo_mgmt_leave_param_t *req = NULL;
  zb_ret_t ret = RET_OK;
  zb_uint8_t tsn;

  TRACE_MSG(TRACE_ERROR, "zb_leave_req", (FMT__0));

  req = ZB_GET_BUF_PARAM(buf, zb_zdo_mgmt_leave_param_t);

  ZB_MEMSET(req->device_address, 0, sizeof(zb_ieee_addr_t));
  req->remove_children = ZB_FALSE;
  req->rejoin = ZB_FALSE;
  req->dst_addr = 1;
  tsn = zdo_mgmt_leave_req(param, leave_callback);
}

void leave_callback(zb_uint8_t param)
{
  zb_zdo_mgmt_leave_res_t *resp = (zb_zdo_mgmt_leave_res_t *)ZB_BUF_BEGIN(ZB_BUF_FROM_REF(param));

  TRACE_MSG(TRACE_ERROR, "LEAVE CALLBACK status %hd", (FMT__H, resp->status));
}
@endcode

See nwk_leave sample
*/
zb_uint8_t zdo_mgmt_leave_req(zb_uint8_t param, zb_callback_t cb);

/** @} */
/** @addtogroup zdo_bind
    @{
*/


/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_head_s
{
  zb_uint16_t binding_target;   /*!< The address of the target for the
                                 * binding. This can be either the
                                 * primary binding cache device or the
                                 * short address of the local device. */
  zb_ieee_addr_t src_ieee_addr; /*!< The IEEE address of the device generating the request */
  zb_uint8_t src_endp;          /*!< The endpoint on the device generating the request */
  zb_uint16_t profile_id;       /*!< ProfileID which is to be matched
                                 * between two End_Device_Bind_req
                                 * received at the ZigBee Coordinator */
  zb_uint8_t num_in_cluster;    /*!< The number of Input Clusters
                                 * provided for end device binding
                                 * within the InClusterList. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_head_t;

/** @brief End_Device_Bind_req command head.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_req_tail_s
{
  zb_uint8_t num_out_cluster;   /*!< The number of Output Clusters
                                 * provided for matching within OutClusterList */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_req_tail_t;

/** @brief Parameters for End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_end_device_bind_req_param_s
{
  zb_uint16_t dst_addr;         /*!< Destinition address */
  zb_zdo_end_device_bind_req_head_t head_param; /*!< Parameters for command head */
  zb_zdo_end_device_bind_req_tail_t tail_param; /*!< Parameters for command tail */
  zb_uint16_t cluster_list[1];  /*!< List of Input and Output
                                 * ClusterIDs to be used for matching */
} ZB_PACKED_STRUCT
zb_end_device_bind_req_param_t;

/**
   sends 2.4.3.2.1 End_Device_Bind_req command
   @param param - index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return        - ZDP transaction sequence number or 0xFF if operation cannot be
                performed now (nor enough memory, resources, etc.)
*/
zb_uint8_t zb_end_device_bind_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Response from End_Device_Bind_req.
  * @see ZB spec, subclause 2.4.3.2.1.
  */
typedef ZB_PACKED_PRE struct zb_zdo_end_device_bind_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_end_device_bind_resp_t;

/** @} */
/** @addtogroup zdo_mgmt
    @{
*/

/** @brief Parameters for Mgmt_Permit_Joining_req.
  * @see ZB spec, subclause 2.4.3.3.7.
  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_s
{
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the
                                  * Trust Center authentication policy as described in the
                                  * sub-clauses below; According to
                                  * r21, should be always set to 0x01.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_t;

/** @brief Parameters for zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_req_param_s
{
  zb_uint16_t dest_addr;        /**< Destination address */
  zb_uint8_t permit_duration;   /**< The length of time in seconds.  0x00 and 0xff indicate that
                                  * permission is disabled or enabled
                                  */
  zb_uint8_t tc_significance;   /**< If this is set to 0x01 and the remote device is the Trust
                                  * Center, the command affects the Trust Center authentication
                                  * policy as described in the sub-clauses below; If this is set to
                                  * 0x00, there is no effect on the Trust
                                  * Center.
                                  * Ignored for r21.
                                  */
} ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_req_param_t;

/**
   sends 2.4~/Work/nordic/nordic_r21/devel/build/Nordic_nRF52840/code_drop.3.3.7 Mgmt_Permit_Joining_req
   @param param - index of buffer with request
   @param cb    - user's function to call when got response from the remote.
   @return        ZDP transaction sequence number or 0xFF if operation cannot be
                  performed now (nor enough memory, resources, etc.)

   @snippet zdo_join_duration_zr.c zb_zdo_mgmt_permit_joining_req

   See zdo_permit_joing sample
 */
zb_uint8_t zb_zdo_mgmt_permit_joining_req(zb_uint8_t param, zb_callback_t cb);


/** @brief Response from zb_zdo_mgmt_permit_joining_req.  */
typedef ZB_PACKED_PRE struct zb_zdo_mgmt_permit_joining_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_mgmt_permit_joining_resp_t;

/** @brief Not Supported Response */
typedef ZB_PACKED_PRE struct zb_zdo_not_supported_resp_s
{
  zb_uint8_t tsn; /*!< ZDP transaction sequence number */
  zb_uint8_t status; /**< Operation status. */
}
ZB_PACKED_STRUCT
zb_zdo_not_supported_resp_t;

/** @} */
/** @addtogroup zdo_groups
    @{
*/


/** @brief ZDO interface for ADD-GROUP.request.

   Note that zb_apsme_add_group_request does not call confirm callback.

   @param param - (in/out) buffer with parameters
      - in - \ref zb_apsme_add_group_req_s
      - out - \ref zb_apsme_add_group_conf_s

   @b Example
   @snippet tp_pro_bv-46_zr.c tp_pro_bv-46_zr

   See tp_pro_bv-46 sample
 */
void zb_zdo_add_group_req(zb_uint8_t param);


/** @brief ZDO interface for REMOVE-GROUP.request
  * @param param - (in/out) buffer with parameters
   @snippet tp_pro_bv-46_zed.c tp_pro_bv-46_zed

   See tp_pro_bv-46 sample
  */
void zb_zdo_remove_group_req(zb_uint8_t param);

/** @brief ZDO interface for REMOVE-ALL-GROUPS.request
  * @param param - (in/out) buffer with parameters
  *
  * @snippet aps_group_management/aps_group_zc.c add_remove_all_groups
  *
  * See aps_group_management sample
  */
void zb_zdo_remove_all_groups_req(zb_uint8_t param);

/** @brief ZDO interface for ZCL Get Group Membership Command
  * @param param - (in/out) buffer with parameters
  *
  * @snippet aps_group_zc.c zb_zdo_get_group_membership_req
  *
  * See aps_group_management sample
  */
void zb_zdo_get_group_membership_req(zb_uint8_t param);

/** @} */
/** @addtogroup zdo_mgmt
    @{ */

/**
   Parameters of Device_annce primitive.

   To be put into buffer as data (means - after space alloc).
 */
typedef ZB_PACKED_PRE struct zb_zdo_device_annce_s
{
  zb_uint8_t       tsn;         /*!< TSN of command */
  zb_uint16_t      nwk_addr;    /*!< NWK address for the Local Device  */
  zb_ieee_addr_t   ieee_addr;   /*!< IEEE address for the Local Device  */
  zb_uint8_t       capability; /*!< Capability of the local device */
} ZB_PACKED_STRUCT zb_zdo_device_annce_t;


/**
   Arguments of the NLME-LEAVE.indication routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_leave_indication_s
{
  zb_ieee_addr_t device_address; /**< 64 bit IEEE address of the device to remove, zero fill if device
                                  * itself */
  zb_uint8_t rejoin; /**< Join after leave */
} ZB_PACKED_STRUCT
zb_nlme_leave_indication_t;

/*! @} */

#ifndef ZB_LITE_COORDINATOR_ONLY_ROLE
void zb_zdo_pim_turbo_poll_continuous_leave(zb_uint8_t param);
void zb_zdo_pim_set_turbo_poll_max(zb_time_t turbo_poll_max_ms);
void zb_zdo_pim_permit_turbo_poll(zb_uint8_t permit);
void zb_zdo_pim_start_turbo_poll_continuous(zb_time_t turbo_poll_timeout_ms);
#else
#define zb_zdo_pim_turbo_poll_continuous_leave(param)
#define zb_zdo_pim_set_turbo_poll_max(turbo_poll_max_ms)
#define zb_zdo_pim_permit_turbo_poll(permit)
#define zb_zdo_pim_start_turbo_poll_continuous(turbo_poll_timeout_ms)
#endif

void zb_zdo_update_long_poll_int(zb_uint8_t param);

#ifdef ZB_REJOIN_BACKOFF

/** Start rejoin backoff procedure.
  If the method is to be directly called from application, it is expected that MAC layer and PIB cache
  are properly initialized. For starting device call zb_zdo_dev_start_cont instead - if device is commisioned it will
  reinit network settings and trigger rejoin from inside the stack.

  @param insecure_rejoin - specify if insecure rejoin is allowed
  @return RET_OK on success, RET_ALREADY_EXISTS if rejoin backoff is running already
*/
zb_ret_t zb_zdo_rejoin_backoff_start(zb_bool_t insecure_rejoin);

/** Run next rejoin backoff iteration */
zb_void_t zb_zdo_rejoin_backoff_continue(zb_uint8_t param);

/** Force rejoin backoff iteration start */
zb_bool_t zb_zdo_rejoin_backoff_force(void);

/** Return rejoin backoff status: running (ZB_TRUE) or not running (ZB_FALSE) */
zb_bool_t zb_zdo_rejoin_backoff_is_running(void);

/** Clear rejoin backoff context, cancel scheduled function */
zb_void_t zb_zdo_rejoin_backoff_cancel(void);

#endif /* ZB_REJOIN_BACKOFF */

#if defined ZB_ENABLE_ZLL
/**
 *  @brief Set Touchlink Master key
 *  @param param [IN] - Touchlink Master key value
 */
zb_void_t zb_zdo_touchlink_set_master_key(zb_uint8_t *key);

/**
 *  @brief Set Touchlink NWK channel
 *  @param param [IN] - Touchlink NWK channel value
 */
zb_void_t zb_zdo_touchlink_set_nwk_channel(zb_uint8_t channel);

/**
 *  @brief Set Touchlink RSSI threshold (used for Touchlink commissioning procedure)
 *  @param param [IN] - Touchlink RSSI threshold value
 */
zb_void_t zb_zdo_touchlink_set_rssi_threshold(zb_uint8_t rssi_threshold);

/**
 *  @brief Get Touchlink RSSI threshold
 *  @return Current Touchlink RSSI threshold value
 */
zb_uint8_t zb_zdo_touchlink_get_rssi_threshold(zb_void_t);

/**
 *  @brief Set Touchlink RSSI correction
 *  @param param [IN] - Touchlink RSSI correction value
 */
zb_void_t zb_zdo_touchlink_set_rssi_correction(zb_uint8_t rssi_correction);

/**
 *  @brief Get Touchlink RSSI correction
 *  @return Current Touchlink RSSI correction value
 */
zb_uint8_t zb_zdo_touchlink_get_rssi_correction(zb_void_t);
#endif /* ZB_ENABLE_ZLL */

#endif /*#ifndef ZB_ZBOSS_API_ZDO_H*/
