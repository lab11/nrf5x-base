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
 * PURPOSE: NWK NIB database
*/

#ifndef ZB_NWK_NIB_H
#define ZB_NWK_NIB_H 1

/**
 *  \addtogroup nwk_ib
 *    @{
 *      @par NWK NIB
 *      Some NIB fields are indeed PIB fields. Use macros to access it.
 */


/*!  NWK NIB Attributes */
typedef enum zb_nib_attribute_e
{
  /**
     A sequence number used to identify outgoing frames.
    */
  ZB_NIB_ATTRIBUTE_SEQUENCE_NUMBER                   = 0X81,
  /**
     The maximum time duration in OctetDurations allowed for the parent and
     all child devices to retransmit a broadcast message
     (passive acknowledgment timeout).
    */
  ZB_NIB_ATTRIBUTE_PASSIVE_ASK_TIMEOUT               = 0X82,
  /**
     The maximum number of retries allowed after a broadcast transmission failure.
    */
  ZB_NIB_ATTRIBUTE_MAX_BROADCAST_RETRIES             = 0X83,
  /**
     The number of children a device is allowed to have on its current network.
     Note that when nwkAddrAlloc has a value of 0x02, indicating stochastic
     addressing, the value of this attribute is implementation-dependent.
    */
  ZB_NIB_ATTRIBUTE_MAX_CHILDREN                      = 0X84,
  /**
     The depth a device can have.
    */
  ZB_NIB_ATTRIBUTE_MAX_DEPTH                         = 0X85,
  /** The number of routers any one device is allowed to have as children.
      This value is determined by the ZigBee coordinator for all devices
      in the network. If nwkAddrAlloc is 0x02 this value not used.
    */
  ZB_NIB_ATTRIBUTE_MAX_ROUTERS                       = 0X86,
  /**
     The current set of neighbor table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_NEIGHBOR_TABLE                    = 0X87,
  /**
     Time duration in OctetDurations that a broadcast message needs to encompass
     the entire network. This is a calculated quantity based on other NIB
     attributes.
    */
  ZB_NIB_ATTRIBUTE_BROADCAST_DELIVERY_TIME           = 0X88,
  /**
     If this is set to 0, the NWK layer shall calculate link cost from all
     neighbor nodes using the LQI values reported by the MAC layer; other-wise,
     it shall report a constant value.
    */
  ZB_NIB_ATTRIBUTE_REPORT_CONSTANT_COST              = 0X89,
  /**
     Reserved.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_DISCOVERY_RETRIES_PERMITTED = 0X8A,
  /**
     The current set of routing table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_TABLE                       = 0X8B,
  /**
     The current route symmetry setting:
        - TRUE means that routes are considered to be comprised of symmetric links.
       Backward and forward routes are created during one-route discovery
       and they are identical.
        - FALSE indicates that routes are not consider to be comprised of symmetric
       links. Only the forward route is stored during route discovery.
    */
  ZB_NIB_ATTRIBUTE_SYM_LINK                          = 0X8E,
  /**
     This field shall contain the device capability information established at
     network joining time.
    */
  ZB_NIB_ATTRIBUTE_CAPABILITY_INFORMATION            = 0X8F,
  /**
     A value that determines the method used to assign addresses:
      - 0x00 = use distributed address allocation
      - 0x01 = reserved
      - 0x02 = use stochastic address allocation
    */
  ZB_NIB_ATTRIBUTE_ADDR_ALLOC                        = 0X90,
  /**
     A flag that determines whether the NWK layer should assume the ability to
     use hierarchical routing:
      - TRUE = assume the ability to use hierarchical routing.
      - FALSE = never use hierarchical routing.
    */
  ZB_NIB_ATTRIBUTE_USE_TREE_ROUTING                  = 0X91,
  /**
     The address of the designated network channel manager function.
    */
  ZB_NIB_ATTRIBUTE_MANAGER_ADDR                      = 0X92,
  /**
     The maximum number of hops in a source route.
    */
  ZB_NIB_ATTRIBUTE_MAX_SOURCE_ROUTE                  = 0X93,
  /**
     The value identifying a snapshot of the network settings with which this
     node is operating with.
    */
  ZB_NIB_ATTRIBUTE_UPDATE_ID                         = 0X94,
  /**
     The maximum time (in superframe periods) that a transaction is stored by a
     coordinator and indicated in its beacon. This attribute reflects the value
     of the MAC PIB attribute macTransaction-PersistenceTime and any changes
     made by the higher layer will be reflected in the MAC PIB attribute value
     as well.
    */
  ZB_NIB_ATTRIBUTE_TRANSACTION_PERSISTENCE_TIME      = 0X95,
  /**
     The 16-bit address that the device uses to communicate with the PAN.
     This attribute reflects the value of the MAC PIB attribute macShortAddress
     and any changes made by the higher layer will be reflected in the MAC PIB
     attribute value as well.
    */
  ZB_NIB_ATTRIBUTE_NETWORK_ADDRESS                   = 0X96,
  /**
     The identifier of the ZigBee stack profile in use for this device.
    */
  ZB_NIB_ATTRIBUTE_STACK_PROFILE                     = 0X97,
  /**
     The current set of broadcast transaction table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_BROADCAST_TRANSACTION_TABLE       = 0X98,
  /**
     The set of group identifiers, in the range 0x0000 - 0xffff, for groups of
     which this device is a member.
    */
  ZB_NIB_ATTRIBUTE_GROUP_ID_TABLE                    = 0X99,
  /**
     The Extended PAN Identifier for the PAN of which the device is a member.
     The value 0x0000000000000000 means the Extended PAN Identifier is unknown.
    */
  ZB_NIB_ATTRIBUTE_EXTENDED_PANID                    = 0X9A,
  /**
     A flag determining the layer where multicast messaging occurs.
      - TRUE = multicast occurs at the network layer.
      - FALSE= multicast oc-curs at the APS layer and using the APS header.
    */
  ZB_NIB_ATTRIBUTE_USE_MULTICAST                     = 0X9B,
  /**
     The route record table.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_RECORD_TABLE                = 0X9C,
  /**
     A flag determining if this device is a concentrator.
      - TRUE = Device is a concentrator.
      - FALSE = Device is not a concentrator.
    */
  ZB_NIB_ATTRIBUTE_IS_CONCENTRATOR                   = 0X9D,
  /**
     The hop count radius for concentrator route dis-coveries.
    */
  ZB_NIB_ATTRIBUTE_CONCENTRATOR_RADIUS               = 0X9E,
  /**
     The time in seconds between concentrator route discoveries.
     If set to 0x0000, the discoveries are done at start up and by
     the next higher layer only.
    */
  ZB_NIB_ATTRIBUTE_CONCENTRATOR_DESCOVERY_TIME       = 0X9F,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_SECURITY_LEVEL                    = 0XA0,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_SECURITY_MATERIAL_SET             = 0XA1,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_ACTIVE_KEY_SEQ_NUMBER             = 0XA2,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_ALL_FRESH                         = 0XA3,
  /**
     Not used.
    */
  ZB_NIB_ATTRIBUTE_SECURE_ALL_FRAMES                 = 0XA5,
  /**
     The time in seconds between link status command frames.
    */
  ZB_NIB_ATTRIBUTE_LINK_STATUS_PERIOD                = 0XA6,
  /**
     The number of missed link status command frames before resetting the
     link costs to zero.
    */
  ZB_NIB_ATTRIBUTE_ROUTER_AGE_LIMIT                  = 0XA7,
  /**
     A flag that determines whether the NWK layer should detect and correct
     conflicting addresses:
     - TRUE = assume ad-dresses are unique.
     - FALSE = addresses may not be unique.
  */
  ZB_NIB_ATTRIBUTE_UNIQUE_ADDR                       = 0XA8,
  /**
     The current set of 64-bit IEEE to 16-bit network address map.
    */
  ZB_NIB_ATTRIBUTE_ADDRESS_MAP                       = 0XA9,
  /**
     A flag that determines if a time stamp indication is provided on incoming
     and outgoing packets.
      - TRUE= time indication provided.
      - FALSE = no time indication provided.
    */
  ZB_NIB_ATTRIBUTE_TIME_STAMP                        = 0X8C,
  /**
     This NIB attribute should, at all times, have the same value as macPANId.
    */
  ZB_NIB_ATTRIBUTE_PAN_ID                            = 0X80,
  /**
     A count of unicast transmissions made by the NWK layer on this device.
     Each time the NWK layer transmits a unicast frame, by invoking the
     MCPS-DATA.request primitive of the MAC sub-layer, it shall increment this
     counter. When either the NHL performs an NLME-SET.request on this attribute
     or if the value of nwkTxTotal rolls over past 0xffff the NWK layer shall
     reset to 0x00 each Transmit Failure field contained in the neighbor table.
    */
  ZB_NIB_ATTRIBUTE_TX_TOTAL                          = 0X8D,
  /**
     This policy determines whether or not a remote NWK leave request command
     frame received by the local device is accepted.
    */
  ZB_NIB_ATTRIBUTE_LEAVE_REQ_ALLOWED                 = 0xAA
}
zb_nib_attribute_t;

/** @brief return NWK sequence number used to identify outgoing frames */
#define ZB_NIB_SEQUENCE_NUMBER() ZB_NIB().sequence_number
/** @brief increment NWK sequence number used to identify outgoing frames */
#define ZB_NIB_SEQUENCE_NUMBER_INC() (ZB_NIB().sequence_number++)

/** @brief Maximum network depth a device can have */
#define ZB_NIB_MAX_DEPTH() ZB_NIB().max_depth
/** @brief Device network depth */
#define ZB_NIB_DEPTH() ZB_NIB().depth

#if defined ZB_ED_ROLE

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() 0
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() 0
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() 1
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() 0
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() 1

#elif (defined ZB_ROUTER_ROLE && defined ZB_LITE_ROUTER_ONLY_ROLE && defined ZB_LITE_NO_JOIN_ZR_AS_ZED)

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() 0
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() 1
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() 0
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() 1
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() 0

#elif (defined ZB_COORDINATOR_ROLE && defined ZB_LITE_COORDINATOR_ONLY_ROLE)

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() 1
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() 0
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() 0
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() 1
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() 1

#else

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() (ZB_NIB().device_type == ZB_NWK_DEVICE_TYPE_COORDINATOR)
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() (ZB_NIB().device_type == ZB_NWK_DEVICE_TYPE_ROUTER)
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() (ZB_NIB().device_type == ZB_NWK_DEVICE_TYPE_ED)
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() (ZB_IS_DEVICE_ZC() || ZB_IS_DEVICE_ZR())
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() (!ZB_IS_DEVICE_ZR())

#endif  /* roles select */

/**
   Check that we at ZC working in Switch mode (routing between 2.4 and sub-ghz).

   Currently Switch is not implemented, so always return 0.
 */
#define ZB_SUBGHZ_SWITCH_MODE() 0

#define ZB_SET_SUBGHZ_SWITCH_MODE(mode)

/** @brief logical device type, see @ref zb_nwk_device_type_e */
#define ZB_NIB_DEVICE_TYPE() ZB_NIB().device_type


#ifdef ZB_PRO_STACK
/** @brief number of source routes in device source routing table */
#define ZB_NIB_SRCRT_CNT() ZB_NIB().nwk_src_route_cnt
#endif

/** @brief Device extended PAN ID */
#define ZB_NIB_EXT_PAN_ID() ZB_NIB().extended_pan_id
/** @brief The value of ZB_NIB_ATTRIBUTE_UPDATE_ID attribute */
#define ZB_NIB_UPDATE_ID() ZB_NIB().update_id

#ifndef ZB_LITE_ALWAYS_SECURE
/** @brief Device security level; by default is 5 */
#define ZB_NIB_SECURITY_LEVEL() ZB_NIB().security_level
/** @brief Set Device security level to value 'v' */
#define ZB_SET_NIB_SECURITY_LEVEL(v) ZB_NIB().security_level = (v)
#else
/** @brief Device security level; always is 5 */
#define ZB_NIB_SECURITY_LEVEL() 5
/** @brief \deprecated unsupported */
#define ZB_SET_NIB_SECURITY_LEVEL(v)
#endif  /* ZB_LITE_ALWAYS_SECURE */

/* use tree routing? */
#ifdef ZB_NWK_TREE_ROUTING
/** @brief returns USE_TREE_ROUTING attribute value */
#define ZB_NIB_GET_USE_TREE_ROUTING() ZB_NIB().use_tree_routing
/** @brief Set USE_TREE_ROUTING attribute value to 'v' */
#define ZB_NIB_SET_USE_TREE_ROUTING(v) (ZB_NIB().use_tree_routing = (v))
#else
/** @brief Use tree routing - unsupported */
#define ZB_NIB_GET_USE_TREE_ROUTING() 0
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_USE_TREE_ROUTING(v)
#endif

/* use nwk multicast? */
#ifndef ZB_NO_NWK_MULTICAST
/** @brief returns ZB_NIB_GET_USE_MULTICAST attribute value */
#define ZB_NIB_GET_USE_MULTICAST() ZB_NIB().nwk_use_multicast
/** @brief Set ZB_NIB_SET_USE_MULTICAST attribute value to 'v' */
#define ZB_NIB_SET_USE_MULTICAST( v ) (ZB_NIB().nwk_use_multicast = ( v ))
#else
/** @brief Use multicast - unsupported */
#define ZB_NIB_GET_USE_MULTICAST() 0
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_USE_MULTICAST( v )
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
/** @brief LINK_STATUS_PERIOD attribute */
#define ZB_NIB_GET_LINK_STATUS_PERIOD() ZB_NIB().link_status_period
/** @brief ROUTER_AGE_LIMIT attribute */
#define ZB_NIB_GET_ROUTER_AGE_LIMIT()   ZB_NIB().router_age_limit
#else
/** @brief LINK_STATUS_PERIOD attribute */
#define ZB_NIB_GET_LINK_STATUS_PERIOD() ZB_NWK_LINK_STATUS_PERIOD
/** @brief ROUTER_AGE_LIMIT attribute */
#define ZB_NIB_GET_ROUTER_AGE_LIMIT()   ZB_NWK_ROUTER_AGE_LIMIT
#endif



/* is leave request allowed? */
#ifdef ZB_PRO_STACK
/** Is leave request allowed */
/** @brief LEAVE_REQ_ALLOWED attribute */
#define ZB_NIB_GET_LEAVE_REQ_ALLOWED() ZB_NIB().leave_req_allowed
/** @brief Set LEAVE_REQ_ALLOWED attribute value to 'v' */
#define ZB_NIB_SET_LEAVE_REQ_ALLOWED( v ) (ZB_NIB().leave_req_allowed = ( v ))
#else
/** @brief LEAVE_REQ_ALLOWED - always true */
#define ZB_NIB_GET_LEAVE_REQ_ALLOWED() ZB_TRUE
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_LEAVE_REQ_ALLOWED( v )
#endif

#ifdef ZB_PRO_STACK
/** @brief LEAVE_REQ_WITHOUT_REJOIN_ALLOWED attribute */
#define ZB_NIB_GET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED() ZB_NIB().leave_req_without_rejoin_allowed
/** @brief Set LEAVE_REQ_WITHOUT_REJOIN_ALLOWED attribute value to 'v' */
#define ZB_NIB_SET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED( v ) (ZB_NIB().leave_req_without_rejoin_allowed = ( v ))
#else
/** @brief LEAVE_REQ_WITHOUT_REJOIN_ALLOWED - always true */
#define ZB_NIB_GET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED() ZB_TRUE
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED( v )
#endif


/** @brief SECURITY_MATERIAL attribute */
#define ZB_NIB_SECURITY_MATERIAL() ZB_NIB().secur_material_set
/** @brief NWK_MANAGER_ADDR attribute */
#define ZB_NIB_NWK_MANAGER_ADDR() ZB_NIB().nwk_manager_addr

/** @brief total number of transmissions made by NWK */
#define ZB_NIB_NWK_TX_TOTAL() (ZB_NIB().tx_stat.tx_total + 0)
/** @brief number of transmissions failed by NWK */
#define ZB_NIB_NWK_TX_FAIL()  (ZB_NIB().tx_stat.tx_fail + 0)

/**
   NWK route discovery
*/
typedef struct zb_nwk_route_discovery_s /* do not pack for IAR */
{
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise   */
  zb_bitfield_t expiration_time:7; /*!< Countdown timer indicating when route
                                    * discovery expires. ZB_NWK_ROUTE_DISCOVERY_EXPIRY 10 */
  zb_uint8_t request_id; /*!< Sequence number for a route request */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t source_addr; /*!< 16-bit network address of the route
                            * requests initiator */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t sender_addr; /*!< 16-bit network address of the device that
                            * has sent the most recent lowest cost route
                            * request */
  zb_uint16_t dest_addr; /*!< 16-bit network destination address of this
                          * request */
  zb_uint8_t forward_cost; /*!< Path cost from the source of the route request
                            * to the current device */
  zb_uint8_t residual_cost; /*!< Path cost from the current to the destination
                             * device */
} ZB_PACKED_STRUCT
zb_nwk_route_discovery_t;

/**
   NWK pending list element
*/
typedef struct zb_nwk_pend_s    /* do not pack for IAR */
{
  zb_uint16_t dest_addr; /*!< 16-bit network destination address of this
                          * request */
  zb_uint8_t  param; /*!< buffer waiting for route discovery */
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise */
  zb_bitfield_t expiry:5; /*!< expiration time. ZB_NWK_PENDING_ENTRY_EXPIRY 20,
                           * 5 bits i */
  zb_bitfield_t waiting_buf:1; /*!< if pending buffer waits new buffer to
                                 * start route discovery */
  zb_bitfield_t reserved:1;

  zb_uint8_t    handle;
} ZB_PACKED_STRUCT
zb_nwk_pend_t;

/**
   Part of the secured material stored in array.

   Other secured material components stored elsewhere:
   OutgoingFrameCounter - NIB
   IncomingFrameCounterSet - neighbor table
 */
typedef struct zb_secur_material_set_t
{
  zb_uint8_t          key[ZB_CCM_KEY_SIZE];  /*!< NWK security key */
  zb_uint8_t          key_seq_number;        /*!< NWK security key sequence number */
} zb_secur_material_set_t;

/**
   TX counters window size.
 */
/* ZB_TX_STAT_WINDOW_SIZE must be bigger than ZB_TX_TOTAL_THRESHOLD, else  of
 * transmitted packets never reach ZB_TX_TOTAL_THRESHOLD in case of continuous
 * failed transfers. */
#define ZB_TX_STAT_WINDOW_SIZE (ZB_TX_TOTAL_THRESHOLD + 1)

/**
   Data structure for TX statistics
 */
typedef struct zb_tx_stat_window_s
{
  zb_uint8_t tx_ok_cnts[ZB_TX_STAT_WINDOW_SIZE];
  zb_uint8_t tx_ok_used;
  zb_uint8_t tx_ok_i;
  zb_uint16_t tx_total;
  zb_uint16_t tx_fail;
} zb_tx_stat_window_t;

#if defined ZB_PRO_STACK && !defined ZB_LITE_NO_SOURCE_ROUTING
/**
  Route Record Table Entry Format, Table 3.45
*/
typedef struct zb_nwk_rrec_s    /* do not pack for IAR */
{
  zb_uint8_t  used;                         /*!< Is record used */
  zb_uint8_t  count;                        /*!< Count hops */
  zb_uint16_t addr;                         /*!< Destination address */
  zb_uint16_t path[ZB_NWK_MAX_PATH_LENGTH]; /*!< Full path */
} ZB_PACKED_STRUCT zb_nwk_rrec_t;

#endif


/**
   This is NWK NIB residental in memory.
   It is not clear yet when it will be save to nvram and when read.
*/
typedef struct zb_nib_s
{
  zb_uint16_t    passive_ack_timeout;    /*!< Maximum time duration allowed for the parent and all child to retransmit a broadcast message */
  zb_uint8_t     sequence_number;        /*!< A sequence number used to identify outgoing frames */
  zb_uint8_t     max_broadcast_retries;  /*!< The maximum number of retries allowed after a broadcast transmission failure. */
  zb_ext_pan_id_t  extended_pan_id;      /*!< The extended PAN identifier for the PAN of which the device is a member */
  zb_uint8_t     device_type;            /*!< Current device role, @see zb_nwk_device_type_t */
  zb_uint8_t     update_id;              /*!< nwkUpdateId - The value identifying a snapshot of the network settings with which this node is operating with. */
#if defined ZB_NWK_MESH_ROUTING && defined ZB_ROUTER_ROLE
#ifndef ZB_CONFIGURABLE_MEM
  zb_nwk_routing_t routing_table[ZB_NWK_ROUTING_TABLE_SIZE]; /*!< Routing table */
#else
  zb_nwk_routing_t *routing_table;
#endif
  zb_nwk_pend_t pending_table[ZB_NWK_PENDING_TABLE_SIZE];    /*!< store pending buffers while route discovery is in progress */
  zb_nwk_route_discovery_t route_disc_table[ZB_NWK_ROUTE_DISCOVERY_TABLE_SIZE]; /*!< Route discovery table */
#ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY
  zb_uint16_t aps_rreq_addr;                                 /*!< APS layer call us to find path to this address */
#endif
  zb_uint8_t routing_table_cnt;                              /*!< Routing table used elements */
  zb_uint8_t route_disc_table_cnt;                           /*!< Discovery table used elements */

  zb_uint8_t rreq_id;                                        /*!< ID, increments each new route discovery procedure */
  zb_uint8_t pending_table_cnt;                              /*!< number of used elements inside pending buffer */
#endif /* ZB_NWK_MESH_ROUTING && ZB_ROUTER_ROLE */

  zb_uint8_t     max_depth;              /*!< The depth a device can have */

#ifdef ZB_ROUTER_ROLE
  zb_uint8_t     max_children;           /*!< The number of children a device is allowed to have */
  zb_uint8_t     router_child_num;       /*!< Number of child devices with router capability */
  zb_uint8_t     ed_child_num;           /*!< Number of child ed devices */
#endif

#if defined ZB_NWK_DISTRIBUTED_ADDRESS_ASSIGN && defined ZB_ROUTER_ROLE
  zb_uint8_t     max_routers;            /*!< The number of routers any one device is allowed to have as children. */
  zb_uint16_t    cskip;                  /*!< Cskip value - size of the address sub-block beeing distributed */
#endif
  zb_uint8_t     depth;                  /*!< current node depth */


  zb_secur_material_set_t secur_material_set[ZB_SECUR_N_SECUR_MATERIAL]; /*!< Set of network security
                                                                            material descriptors
                                                                            capable of maintaining
                                                                            an active and alternate
                                                                            network key.  */
  zb_uint8_t              active_key_seq_number; /*!< The sequence number of
                                                   the active network key in
                                                   nwkSecurityMaterialSet.  */
  zb_uint32_t             outgoing_frame_counter; /*!< OutgoingFrameCounter
                                                   * stored here (not in the
                                                   * secured material).
                                                   * Rationale: will never use
                                                   * "old" key - why store more
                                                   * then 1 counter?
                                                   */
  zb_uint32_t             prev_outgoing_frame_counter;


  zb_uint16_t nwk_manager_addr; /*!< The address of the designated
                                 * network channel manager function. */

#ifdef ZB_PRO_STACK
#ifndef ZB_LITE_NO_SOURCE_ROUTING
  zb_nwk_rrec_t nwk_src_route_tbl[ZB_NWK_MAX_SRC_ROUTES];
  zb_uint8_t nwk_src_route_cnt;
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
  zb_uint8_t link_status_period;  /*!< Table 3.44 NIB Attributes - nwkLinkStatusPeriod */
  zb_uint8_t router_age_limit;    /*!< Table 3.44 NIB Attributes - nwkRouterAgeLimit */
#endif
  zb_bitfield_t leave_req_allowed:1; /*!< See: docs-11-5378-12-0csg-zigbee-pro-errata-for-r20 NIB entry nwkLeaveRequestAllowed */
  zb_bitfield_t leave_req_without_rejoin_allowed:1; /* DA: parameter set to ignore leave requests w/o rejoin. Reason: R21 core stack spec. */
                                                       /* DA: please note, that I set default value to 1, can't find it in current version of spec */
#ifndef ZB_NO_NWK_MULTICAST
  zb_bitfield_t nwk_use_multicast:1;     /*!< Multicast determination flag */
#endif
#endif  /* ZB_PRO_STACK */

  zb_bitfield_t disable_rejoin:1; /*!< Forbid rejoin - for Rejoin request set Rejoin response with error status */
#ifdef ZB_NWK_TREE_ROUTING
  zb_bitfield_t use_tree_routing:1;      /*!< if device is able to use tree routing */
#endif
  zb_bitfield_t addr_alloc:1;            /*!< Address assign method @see zb_nwk_address_alloc_method_t */

  zb_bitfield_t uniq_addr:1;             /*!< Table 3.44 NIB Attributes - nwkUniqueAddr */


#if defined (ZB_PRO_STACK)
  zb_bitfield_t reserve:1;
#else
  zb_bitfield_t reserve:4;
#endif

  zb_bitfield_t           security_level:3; /*!< The security level for
                                            outgoing and incoming
                                            NWK frames; the
                                            allowable security level
                                            identifiers are presented
                                            in Table 4.38.
                                            For ZB 2007 (Standard security only)
                                            only values 0 and 5 are possible.
                                            Or, seems, only value 5 is possible?
                                            */
  /* all_fresh is always 0 for Standard security */
  zb_bitfield_t           active_secur_material_i:2; /*!< index in
                                                      * secur_material_set for
                                                      * keys with
                                                      * key_seq_number == active_key_seq_number  */

  zb_bitfield_t           reserved:2;

  zb_bitfield_t nwk_report_constant_cost:1; /*!< If this is set to 0, the NWK
                                             * layer shall calculate link cost
                                             * from all neighbor nodes using the
                                             * LQI values reported by the MAC
                                             * layer; otherwise, it shall report
                                             * a constant value. */

  zb_tx_stat_window_t tx_stat;  /*!< TX/TX fail counters  */
  zb_uint8_t nwk_keepalive_modes;
  zb_uint8_t nwk_parent_information;
  zb_uint8_t nwk_ed_timeout_default;
/**
  The period for sending the keepalive to the router parent shall be determined by the
  * manufacturer of the device and is not specified by this standard. It is
  * recommended that the period allows the end device to send 3 keepalive
  * messages during the Device Timeout period. This will help insure that a
  * single missed keepalive message will not age out the end device on the router
  * parent.
 */
  zb_time_t nwk_ed_keepalive_timeout;
} zb_nib_t;

/** @} */

#endif /* ZB_NWK_NIB_H */
