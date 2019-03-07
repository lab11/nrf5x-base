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
 PURPOSE: Public APS layer API
*/
#ifndef ZB_ZBOSS_API_APS_H
#define ZB_ZBOSS_API_APS_H 1

/** @cond internals_doc */
/*! \addtogroup aps_ib */
/*! @{ */

/** @brief APS Information Base constants. */
typedef enum zb_aps_aib_attr_id_e
{
  ZB_APS_AIB_BINDING                   = 0xc1, /*!< The current set of binding table entries in the device
                                                 (see subclause 2.2.8.2.1). */
  ZB_APS_AIB_DESIGNATED_COORD          = 0xc2, /*!< TRUE if the device should become the ZigBee Coordinator on
                                                 startup, FALSE if otherwise. */
  ZB_APS_AIB_CHANNEL_MASK              = 0xc3, /*!< The mask of allowable channels for this device
                                                 to use for network operations. */
  ZB_APS_AIB_USE_EXT_PANID             = 0xc4, /*!< The 64-bit address of a network to form or to join. */
  ZB_APS_AIB_GROUP_TABLE               = 0xc5, /*!< The current set of group table entries (see Table 2.25). */
  ZB_APS_AIB_NONMEMBER_RADIUS          = 0xc6, /*!< The value to be used for the NonmemberRadius
                                                 parameter when using NWK layer multicast. */
  ZB_APS_AIB_PERMISSION_CONFIG         = 0xc7, /*!< The current set of permission configuration items. */
  ZB_APS_AIB_USE_INSECURE_JOIN         = 0xc8, /*!< A flag controlling the use of insecure join at startup. */
  ZB_APS_AIB_INTERFRAME_DELAY          = 0xc9, /*!< Fragmentation parameter - the standard delay, in
                                                 milliseconds, between sending two blocks of a
                                                 fragmented transmission (see subclause 2.2.8.4.5). */
  ZB_APS_AIB_LAST_CHANNEL_ENERGY       = 0xca, /*!< The energy measurement for the channel energy
                                                 scan performed on the previous channel just
                                                 before a channel change (in accordance with [B1]). */
  ZB_APS_AIB_LAST_CHANNEL_FAILURE_RATE = 0xcb, /*!< The latest percentage of transmission network
                                                 transmission failures for the previous channel just before
                                                 a channel change (in percentage of failed transmissions
                                                 to the total number of transmissions attempted) */
  ZB_APS_AIB_CHANNEL_TIMER             = 0xcc,  /*!< A countdown timer (in
                                                 * hours) indicating the time to
                                                 * the next permitted frequency
                                                 * agility channel change. A value of NULL
                                                 * indicates the channel has not
                                                 * been changed previously.
                                                */
  ZB_APS_MAX_WINDOW_SIZE               = 0xcd, /*!< A table with the active
                                                * endpoints and their respective
                                                * apsMaxWin-dowSize where
                                                * frag-mentation is used
                                                * (ac-tive endpoints not
                                                * sup-porting fragmentations
                                                * shall be omitted from the
                                                * list).
                                                */
  ZB_APS_PARENT_ANNOUNCE_TIMER         = 0xce  /*!< The value of the current
                                                * countdown timer before the
                                                * next Parent_annce is sent.
                                                */
} zb_aps_aib_attr_id_t;

/*! @} */
/** @endcond */

/*! \addtogroup aps_api */
/*! @{ */

/** @name Common constants.
  * @{
  */


/** @brief APS status constants. */
typedef enum zb_aps_status_e
{
  ZB_APS_STATUS_SUCCESS               = 0x00, /*!< A request has been executed
                                                * successfully. */
  ZB_APS_STATUS_ILLEGAL_REQUEST       = 0xa3, /*!< A parameter value was out of range. */
  ZB_APS_STATUS_INVALID_BINDING       = 0xa4, /*!< An APSME-UNBIND.request failed due to
                                               the requested binding link not existing in the
                                               binding table. */
  ZB_APS_STATUS_INVALID_GROUP         = 0xa5, /*!< An APSME-REMOVE-GROUP.request has
                                               been issued with a group identifier that does
                                               not appear in the group table. */
  ZB_APS_STATUS_INVALID_PARAMETER     = 0xa6, /*!< A parameter value was invalid or out of
                                               range.
                                               ZB_APS_STATUS_NO_ACK 0xa7 An APSDE-DATA.request requesting
                                               acknowledged transmission failed due to no
                                               acknowledgement being received. */
  ZB_APS_STATUS_NO_ACK                = 0xa7, /*!< An APSDE-DATA.request requesting
                                                acknowledged transmission failed due to no
                                                acknowledgement being received. */
  ZB_APS_STATUS_NO_BOUND_DEVICE       = 0xa8, /*!< An APSDE-DATA.request with a destination
                                                addressing mode set to 0x00 failed due to
                                                there being no devices bound to
                                                this device. */
  ZB_APS_STATUS_NO_SHORT_ADDRESS      = 0xa9, /*!< An APSDE-DATA.request with a destination
                                                addressing mode set to 0x03 failed due to no
                                                corresponding short address found in the
                                                address map table. */
  ZB_APS_STATUS_NOT_SUPPORTED         = 0xaa, /*!< An APSDE-DATA.request with a destination
                                                addressing mode set to 0x00 failed due to a
                                                binding table not being supported on the
                                                device. */
  ZB_APS_STATUS_SECURED_LINK_KEY      = 0xab, /*!< An ASDU was received that was secured
                                                using a link key. */
  ZB_APS_STATUS_SECURED_NWK_KEY       = 0xac, /*!< An ASDU was received that was secured
                                                using a network key. */
  ZB_APS_STATUS_SECURITY_FAIL         = 0xad, /*!< An APSDE-DATA.request requesting
                                                security has resulted in an error during the
                                                corresponding security
                                                processing. */
  ZB_APS_STATUS_TABLE_FULL            = 0xae, /*!< An APSME-BIND.request or APSME.ADD-
                                                GROUP.request issued when the binding or
                                                group tables, respectively, were
                                                full. */
  ZB_APS_STATUS_UNSECURED             = 0xaf, /*!< An ASDU was received without
                                               * any security */
  ZB_APS_STATUS_UNSUPPORTED_ATTRIBUTE = 0xb0  /*!< An APSME-GET.request or APSME-
                                                SET.request has been issued with an
                                                unknown attribute identifier. */
} zb_aps_status_t;

/** @brief APS addressing mode constants. */
typedef enum zb_aps_addr_mode_e
{
  ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT = 0, /*!< 0x00 = DstAddress and
                                                   * DstEndpoint not present  */
  ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT = 1, /*!< 0x01 = 16-bit group
                                                   * address for DstAddress;
                                                   * DstEndpoint not present */
  ZB_APS_ADDR_MODE_16_ENDP_PRESENT = 2, /*!< 0x02 = 16-bit address for DstAddress and
                                          DstEndpoint present  */
  ZB_APS_ADDR_MODE_64_ENDP_PRESENT = 3 /*!< 0x03 = 64-bit extended address for
                                         DstAddress and DstEndpoint present  */
} zb_aps_addr_mode_t;

/** @brief Minimum valid endpoint number. */
#define ZB_MIN_ENDPOINT_NUMBER 1

/** @brief Maximum valid endpoint number. */
#define ZB_MAX_ENDPOINT_NUMBER 240

/** @brief Broadcast endpoint number. */
#define ZB_APS_BROADCAST_ENDPOINT_NUMBER 255

/** @} */


/** @name APS data service structures and API.
  * @{
  */

/** @brief Parsed APS header.
 */
typedef ZB_PACKED_PRE struct zb_aps_hdr_s
{
  zb_uint8_t  fc;              /*!< Frame control. */
  zb_uint16_t src_addr;        /*!< Source address of packet originator. */
  zb_uint16_t dst_addr;        /*!< The destination address of the packet. */
  zb_uint16_t group_addr;      /*!< If send to group then group address destination else 0. */
  zb_uint8_t  dst_endpoint;    /*!< The destination endpoint of the packet receiver. */
  zb_uint8_t  src_endpoint;    /*!< The source endpoint from which that packet was send. */
  zb_uint16_t clusterid;       /*!< The identifier of the cluster on the source device. */
  zb_uint16_t profileid;        /*!< Profile Id */
  zb_uint8_t  aps_counter;      /*!< APS Counter for check APS dup command. */
  zb_uint16_t mac_src_addr;     /*!< Source address of device that transmit that packet. */
  zb_uint16_t mac_dst_addr;     /*!< Next hop address used for frame transmission. */
  zb_uint8_t tsn;               /*!< Transaction sequence number for ZDO/ZCL command. */
} ZB_PACKED_STRUCT zb_aps_hdr_t;


/**
    Parameters of the APSDE-DATA.indication primitive.

    Also used as a parameter in different APS calls.
 */
typedef zb_aps_hdr_t zb_apsde_data_indication_t;

/** @} */


/** @name APS management service data structures and API.
  * @{
  */

/** @brief APSME binding structure.
  *
  * This data structure passed to @ref zb_apsme_bind_request()
  * and to @ref zb_apsme_unbind_request()
  */
typedef struct zb_apsme_binding_req_s
{
  zb_ieee_addr_t  src_addr;       /*!< The source IEEE address for the binding entry. */
  zb_uint8_t      src_endpoint;   /*!< The source endpoint for the binding entry. */
  zb_uint16_t     clusterid;      /*!< The identifier of the cluster on the source
                                        device that is to be bound to the destination device.*/
  zb_uint8_t      addr_mode;      /*!< The type of destination address supplied by
                                       the DstAddr parameter - see @ref zb_aps_addr_mode_e  */
  zb_addr_u dst_addr;       /*!< The destination address for the binding entry. */
  zb_uint8_t      dst_endpoint;   /*!< This parameter will be present only if
                                       the DstAddrMode parameter has a value of
                                       0x03 and, if present, will be the
                                       destination endpoint for the binding entry.*/
} zb_apsme_binding_req_t;

/** @brief APSME-ADD-GROUP.request primitive parameters. */
typedef struct zb_apsme_add_group_req_s
{
  zb_uint16_t group_address;    /*!< The 16-bit address of the group being added.  */
  zb_uint8_t  endpoint;         /*!< The endpoint to which the given group is being added.  */
} zb_apsme_add_group_req_t;

/** @brief APSME-ADD-GROUP.confirm primitive parameters. */
typedef struct zb_apsme_add_group_conf_s
{
  zb_uint16_t group_address;    /*!< The 16-bit address of the group being added.  */
  zb_uint8_t  endpoint;         /*!< The endpoint to which the given group is being added.  */
  zb_uint8_t  status;           /*!< Request send status. */
} zb_apsme_add_group_conf_t;

/** @brief APSME-REMOVE-GROUP.request primitive parameters.  */
typedef struct zb_apsme_add_group_req_s zb_apsme_remove_group_req_t;

/** @brief APSME-REMOVE-GROUP.confirm primitive parameters.  */
typedef struct zb_apsme_add_group_conf_s zb_apsme_remove_group_conf_t;

/** @brief APSME-REMOVE-ALL-GROUPS.request primitive parameters.  */
typedef struct zb_apsme_remove_all_groups_req_s
{
  zb_uint8_t  endpoint;         /*!< The endpoint to which the given group is being removed. */
} zb_apsme_remove_all_groups_req_t;

/** @brief APSME-REMOVE-ALL-GROUPS.confirm primitive parameters.  */
typedef struct zb_apsme_remove_all_groups_conf_s
{
  zb_uint8_t  endpoint;         /*!< The endpoint which is to be removed from all groups. */
  zb_uint8_t  status;           /*!< The status of the request to remove all groups. */
} zb_apsme_remove_all_groups_conf_t;

zb_uint8_t zb_aps_is_endpoint_in_group(
    zb_uint16_t group_id,
    zb_uint8_t endpoint);

/** @cond internals_doc */
/** @brief APSME GET request structure. */
typedef struct zb_apsme_get_request_s
{
  zb_aps_aib_attr_id_t aib_attr;       /*!< The identifier of the AIB attribute to read. */
  zb_callback_t        confirm_cb;     /*!< User's callback to be called as APSME-GET.confirm  */
} zb_apsme_get_request_t;

/** @brief APSME GET confirm structure. */
typedef ZB_PACKED_PRE struct zb_apsme_get_confirm_s
{
  zb_aps_status_t status;         /*!< The results of the request to read an AIB attribute value. */
  zb_aps_aib_attr_id_t aib_attr;  /*!< The identifier of the AIB attribute that was read.*/
  zb_uint8_t aib_length;          /*!< The length, in octets, of the attribute value being returned.*/
  /* Various */                   /* The value of the AIB attribute that was read.*/
} ZB_PACKED_STRUCT zb_apsme_get_confirm_t;

/** @brief APSME SET request structure. */
typedef ZB_PACKED_PRE struct zb_apsme_set_request_s
{
  zb_aps_aib_attr_id_t aib_attr; /*!< The identifier of the AIB attribute to be written. */
  zb_uint8_t           aib_length; /*!< The length, in octets, of the attribute value being set. */
  zb_callback_t        confirm_cb; /*!< User's callback to be called as APSME-SET.confirm  */
  /* Various */                   /* The value of the AIB attribute that should be written. */
} ZB_PACKED_STRUCT zb_apsme_set_request_t;

/** @brief APSME SET confirm structure. */
typedef ZB_PACKED_PRE struct zb_apsme_set_confirm_s
{
  zb_aps_status_t   status;       /*!< The result of the request to write the AIB Attribute. */
  zb_aps_aib_attr_id_t aib_attr;  /*!< The identifier of the AIB attribute that was written. */
} ZB_PACKED_STRUCT zb_apsme_set_confirm_t;

/** @endcond */

/** @brief APSME-BIND.request primitive.
  * @param param - reference to the buffer containing request data (see @ref
  * zb_apsme_binding_req_t).
  *
  * @par Example
  * @snippet tp_aps_bv_19_i_zr1.c tp_aps_bv_19_i_zr1
  * @par
  *
  * See tp_aps_bv-19-i sample
  */
void zb_apsme_bind_request(zb_uint8_t param);

/** @brief APSME-UNBIND.request primitive.
  * @param param - reference to the buffer containing request data (see @ref
  * zb_apsme_binding_req_t).
  *
  * @par Example
  * @snippet aps_binding_test.c zb_apsme_unbind_request
  * @par
  *
  * See aps_binding_test sample
  */
void zb_apsme_unbind_request(zb_uint8_t param);

/** @brief Perform unbind all entries. This custom function and it is not described
 * in ZigBee specificatoin.
 * @param param - not used.
 */
void zb_apsme_unbind_all(zb_uint8_t param);

/** @brief APSME-ADD-GROUP.request primitive.
  *
  * @internal
  * Use macro @ref ZDO_REGISTER_CALLBACK to register APSME-ADD-GROUP.confirm callback.
  * @endinternal
  * @param param - buffer with parameter. See @ref zb_apsme_add_group_req_t.
  *
  * @par Example
  * @snippet tp_aps_bv_23_i_zr3.c zb_apsme_add_group_request
  * @par
  *
  * See tp_aps_bv-23-i sample
  */
void zb_apsme_add_group_request(zb_uint8_t param);

/** @brief APSME-REMOVE-GROUP.request primitive.
  *
  * @internal
  * Use macro @ref ZDO_REGISTER_CALLBACK to register APSME-REMOVE-GROUP.confirm callback.
  * @endinternal
  * @param param - buffer with parameter. See @ref zb_apsme_remove_group_req_t.
  *
  * @par Example
  * @snippet tp_aps_bv-17_zed.c zb_apsme_remove_group_request
  * @par
  *
  * See tp_aps_bv-17 sample
  */
void zb_apsme_remove_group_request(zb_uint8_t param);

/** @brief APSME-REMOVE-ALL-GROUPS.request primitive.
  *
  * @internal
  * Use macro @ref ZDO_REGISTER_CALLBACK to register APSME-REMOVE-ALL-GROUPS.confirm callback.
  * @endinternal
  * @param param - buffer with parameter. See @ref zb_apsme_remove_all_groups_req_t.
  *
  * @par Example
  * @snippet tp_aps_bv-18_zed.c zb_apsme_remove_all_groups_request
  * @par
  *
  * See tp_aps_bv-18 sample
  */
void zb_apsme_remove_all_groups_request(zb_uint8_t param);

/** @} */ /* APS management service data structures and API. */

/*! @} */ /* aps_api */


#endif /*#ifndef ZB_ZBOSS_API_APS_H*/
