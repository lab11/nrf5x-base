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
 PURPOSE: Common constants and definitions, mainly related to ZigBee protocol.
*/
#ifndef ZB_CONFIG_COMMON_H
#define ZB_CONFIG_COMMON_H 1

/*! \addtogroup ZB_CONFIG */
/*! @{ */

/* To compile MAC only build;
   TODO: Fix it. there should probably be another way to build without security
*/
#ifdef ZB_MACSPLIT_DEVICE
#define ZB_CCM_M 4
#define ZB_CCM_KEY_SIZE 16
#ifndef ZB_BUILD_DATE
#define ZB_BUILD_DATE "19700101"
#endif
#endif

/****************************Sequrity options**************************/

/****************************Sequtity keys***************************/

/* Always security on (both NWK and APS). Remove all special security configs */
/*
   If defined, security APS is compiled
   remove to Options file
 */
//#define APS_FRAME_SECURITY


/*
4.6.3.2.2.1 Standard Security Mode

A default global trust center link key must be supported by the device if no other
link key is specified by the application at the time of joining. This default link key
shall have a value of 5A 69 67 42 65 65 41 6C 6C 69 61 6E 63 65 30 39
(ZigBeeAlliance09).44
 */
#define ZB_STANDARD_TC_KEY {0x5A, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6C, 0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0x30, 0x39 };


/**
6.3.2 Distributed security global link key

 The distributed security global link key is used to join a distributed security
 network. This link key is provided to a company as a result of a successful
 certification of a product. For testing, this key SHALL have the value of:

 In ZLL specification this is ZLL Certification pre-installed link key - see
 8.7.2 Transfer ring the network key dur ing classical ZigBee commissioning.

 "During classical ZigBee commissioning where a non-ZLL device is being joined to a ZLL network
 without a trust center, a pre-installed link key is used to secure the transfer of the network key when
 authenticating." - means,
 */
#define ZB_DISTRIBUTED_GLOBAL_KEY {0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf};

/** @cond touchlink */

/**
6.3.4 Touchlink preconfigured link key

The touchlink preconfigured link key is used to join a network via
touchlink. This link key is provided to a company as a result of a
successful certification of a product. For testing, this key SHALL have the
value of:

In ZLL specification this is 8.7.5.1.2 Certification key (key index 15)

In BDB 8.7.1.2 Key index and encrypted network key fields.
"This value SHALL be set to 0x04 during certification testing or 0x0f at all
other times."
In ZLL Table 67 Key encryption algorithms: 4 is Master key, 0xf is Certification
key. They use same algorithm.

 */
#define ZB_TOUCHLINK_PRECONFIGURED_KEY {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf};

/** @endcond */ /* touchlink */

/* BDB does not mention using of Development ZLL key, so skip it. */

/**
 SECUR: if defined, implement Standard security

Note: High Security is obsolet and will never be implemented.
*/
#define ZB_STANDARD_SECURITY

/**
 SECUR: If defined, generate random keys at Trust Center at start of pre-configured jey is not set.
*/
#define ZB_TC_GENERATES_KEYS
/**
 SECUR: If defined, trust Center is at ZC (currently - always)
*/
#define ZB_TC_AT_ZC

/**
 SECUR: CCM key size. Hard-coded
*/
#define ZB_CCM_KEY_SIZE 16

/* NLS5 - All devices shall maintain at least 2 NWK keys with the frame
   counters consistent with the security mode of the network (Standard or High).*/
#define ZB_SECUR_N_SECUR_MATERIAL 3


/* parameters for security level 5 - the only security level supported */
/**
 SECUR: security level. Now fixed to be 5
*/
#define ZB_SECURITY_LEVEL 5

/**
 SECUR: CCM L parameter. Fixed to 2 for security level 5
*/
#define ZB_CCM_L 2

/**
 SECUR: CCM nonce length. Now fixed.
*/
#define ZB_CCM_NONCE_LEN  (15 - ZB_CCM_L)

/**
 SECUR: CCM M parameter. Fixed to 4 for security level 5
*/
#define ZB_CCM_M 4

/**
 Value of nwk packets counter which triggered nwk key switch
*/
#define ZB_SECUR_NWK_COUNTER_LIMIT (((zb_uint32_t)~0) - 128)

//#define ZB_DEFAULT_SECURE_ALL_FRAMES 0
/**
 Default value for nib.secure_all_frames
*/
#define ZB_DEFAULT_SECURE_ALL_FRAMES 1

/**
   If defined, include long address into APS security header
 */
#define ZB_APS_SECUR_APS_USE_EXT_NONCE

/**
  This macro defines total number of NWK frame unsecure attempts before NWK decides that
  frame can not be processed. Made as Workaround for Onsemi platform, used for all.
  If frame was not unsecured, cut 1 byte of buffer end and try to unsecure frame again.
 */
#define ZB_NWK_SECUR_UNSECURE_FRAME_ATTEMPTS 4


/****************************APS layer options**************************/



/* APS */

/**
   APS: dup check timeout.

   APS dup checks resolution is 1 sec, so dup timeout is 21 sec. This
   interval guaranties total APS retransmission interval (1 original + 2 retransmits).
*/

#define ZB_APS_DUP_INITIAL_CLOCK 21

#define ZB_APS_DUP_CHECK_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)

#define ZB_APS_DUPS_TABLE_SIZE 32


/**
   Maximum MAC packet waiting time to expire
 */

  #define ZB_MAC_INDIRECT_POLL_EXPIRE_TMO ZB_MILLISECONDS_TO_BEACON_INTERVAL(7680)



/**
   APS retransmissions
 */

/**
 APS: max number of packets waiting for APS ACK

 Derive it from the pool size. Keep some free buffers in the pool, remember some
 buffers are reserved at start (I see 4 out buffers resetved).
*/
#ifndef ZB_CONFIGURABLE_MEM
  #define ZB_N_APS_RETRANS_ENTRIES   ((ZB_IOBUF_POOL_SIZE / 3) > 0 ? (ZB_IOBUF_POOL_SIZE / 3) : (ZB_IOBUF_POOL_SIZE / 2))
#endif

/**
 APS maximum of apscMaxFrameRetries times
   see ZB spec 2.2.7.1
*/
  #define ZB_N_APS_MAX_FRAME_RETRIES 3


/**
 APS: APS ACK wait time from Sleepy devices. After this timeout resend APS packet
      see ZB spec 2.2.7.1
*/
  #define ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY (10*ZB_TIME_ONE_SECOND)

/**
 APS: The base amount of delay before each broadcast parent announce is sent.
 */
#define ZB_APS_PARENT_ANNOUNCE_BASE_TIMER  (10 * ZB_TIME_ONE_SECOND)

/**
The max amount of jitter that is added to the apsParentAnnounceBaseTimer before each broadcast parent announce is sent.
*/
#define ZB_APS_PARENT_ANNOUNCE_JITTER_MAX (10 * ZB_TIME_ONE_SECOND)


/**
 APS: APS ACK wait time from Non Sleepy devices. After this timeout resend APS packet
      see ZB spec 2.2.7.1
*/
  /* Some devices send APS_ACK to AF and ZDO commands after sending appropriate response or
   * DefaultResponse. For example, ZCL On/Off command can be done within 5-7 seconds,
   * so 2 seconds for wail duration is insufficiently. */
  #define ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY (3*ZB_TIME_ONE_SECOND)

/**
 APS: maximum number of tables with information from a binding table to be sent to the devices
*/
  #define ZB_N_APS_BINDTRANS_ARR_MAX_SIZE            5

/**
 APS: maximum number of elements in array with index from dst array in bind table
*/
  #define ZB_N_APS_BINDTRANS_DST_INDEX_ARR_MAX_SIZE  ZB_APS_DST_BINDING_TABLE_SIZE

/**
 NWK radius to be used when sending APS command
*/

/* DA: disable aps acknowledgement request bit in APS commands. */
/* Origin of this tweak is R21 version of core stack */
/* Also it's kind of "everybody knows" issue */
#define ZB_APS_DISABLE_ACK_IN_COMMANDS

#define ZB_APS_SECUR_APS_USE_EXT_NONCE


/* EE: wrong place for that constant in zb_aps.h (this is not public APS API!) and wrong
 * calculation!
 * BTW that constant used at 1 place only.
 * ZB_ZDO_MAX_PAYLOAD_SIZE in zb_zdo.h is hard-coded!
 * And, zb_zdo.h is also bad place for it.
 *
 * Let's count supposing no APS security.
 * MAC header - 10 with fcf (suppose panid compression)
 * NWK header - 24 unicast, but 16 broadcast
 * NWK ZB security header + MIC - cf 1 + fcnt 4 + ext src 8 + key seq 1 mic 4 = 18
 * APS hdr 8b
 * Totally for broadcast - 127 - (11+16+18+8) = 74
 * For unicast 74-8 = 66

Fixed that size for the certification test TP/R21/BV-24.
We actually fit 5 parent annce records into 127 bytes only for broadcast tx,
with panid compression.

At the worst case our NWK can skip long address at tx: 8 bytes of reserve.

 */
#define ZB_APS_MAX_BROADCAST_PAYLOAD_SIZE 74
/**
   Maximum proadcast aps payload size supposing NWK encryption only, no long
   address in NWK header - means, really possible maximum.
 */
#define ZB_APS_MAX_MAX_BROADCAST_PAYLOAD_SIZE (74 + 8)

/* ZB packet length must not exceed 127 bytes
 *
 * Old calculation by Maxim:
 *
 * MAC header (total: 11 bytes)
 * - FCF: 2 bytes
 * - Sequence number: 1 bytes
 * - PAN: 2 bytes
 * - Destination address: 2 bytes
 * - Source address: 2 bytes
 *
 * NWK header (total: 24 bytes)
 * - FCF: 2 bytes
 * - Destination short address: 2 bytes
 * - Source short address: 2 bytes
 * - Radius: 1 byte
 * - Sequence number: 1 byte
 * - Destination long address: 8 bytes
 * - Source long address: 8 bytes
 *
 * ZB security header (total: 31 bytes)
 *
 * APS header (total: 8 bytes)
 * - FCF: 1 byte
 * - Destination endpoint: 1 byte
 * - Cluster id: 2 bytes
 * - Profile id: 2 bytes
 * - Source endpoint: 1 byte
 * - Counter: 1 byte
 *
 * APS payload (total: variable lenght)
 *
 * MAC footer (total: 2 bytes)
 * - FCS: 2 bytes
 *
 * Total length w/o APS payload: 127-(11+24+31+8+2)= 51 bytes,
 * consequently APS payload max size is 51 bytes.
 */
/**
 * max broadcast nwk payload size
 */
/*
 * MAC (short source, short destination): MAX_MAC_OVERHEAD_SHORT_ADDRS bytes
 * NWK (broadcast, no destination ieee): 16 bytes header + 18 bytes security
 * Total length: 127-(11+16+18) = 82 bytes  */
 /* nwk hdr include source ieee address */
#define ZB_NWK_MAX_BROADCAST_HDR_SIZE 16
#define ZB_NWK_MAX_SECURITY_HDR_SIZE 18
#define ZB_NWK_MAX_BROADCAST_PAYLOAD_SIZE (MAX_PHY_FRM_SIZE - (MAX_MAC_OVERHEAD_SHORT_ADDRS + \
  ZB_NWK_MAX_BROADCAST_HDR_SIZE + ZB_NWK_MAX_SECURITY_HDR_SIZE))
/**
 * max unicast aps payload size, supposing no APS encryption
 */
#define ZB_APS_MAX_PAYLOAD_SIZE (ZB_APS_MAX_BROADCAST_PAYLOAD_SIZE - 8)

/* ZDO header adds 1 byte of sequence number to APS payload */
#define ZB_ZDO_MAX_PAYLOAD_SIZE (ZB_APS_MAX_PAYLOAD_SIZE-1)

#define ZB_NWKC_MAC_FRAME_OVERHEAD 0xB
#define ZB_NWKC_MIN_HEADER_OVERHEAD 0x8
/* max nsdulength = aMaxPHYFrameSize -(nwkcMACFrameOverhead + nwkcMinHeaderOverhead) (D.4 aMaxMACFrameSize) */
#define ZB_NSDU_MAX_LEN (MAX_PHY_FRM_SIZE - (ZB_NWKC_MAC_FRAME_OVERHEAD + ZB_NWKC_MIN_HEADER_OVERHEAD))

#define ZB_APSC_MIN_HEADER_OVERHEAD 0x0C
/* max asdu length 256*(NsduLength - apscMinHeaderOverhead); currently fragmentation
 * is not supported  */
#define ZB_ASDU_MAX_LEN (ZB_NSDU_MAX_LEN - ZB_APSC_MIN_HEADER_OVERHEAD)

#define ZB_APS_HEADER_MAX_LEN                                           \
  (2 +                          /* fc + aps counter */                  \
   /* Packet either has dest and src endpoints (1+1 byte) if not group  \
    * addressing or Group address elsewhere - so 2 anyway */            \
   2 +                                                                  \
   /* cluster id, profile id */                                         \
   4 +                                                                  \
                                                                        \
   /* TODO: handle fragmentation and Extended header. Now suppose no Extended header */ \
   0)

#define ZB_APS_PAYLOAD_MAX_LEN ZB_ASDU_MAX_LEN - ZB_APS_HEADER_MAX_LEN


/****************************NWK layer options**************************/

/**
 NWK: Max number of children per node
*/
#define ZB_NWK_MAX_CHILDREN 4

/**
 NWK: Max number of routers per node
*/
#define ZB_NWK_MAX_ROUTERS  4

/**
 NWK: max network depth for tree routing
*/
#define ZB_NWK_TREE_ROUTING_DEPTH    5

/**
 NWK: network depth in stochastic addressing mode (NLF90)
*/
#define ZB_NWK_STOCH_DEPTH    15


/**
   NWK Mesh route stuff: route discovery table size
*/
#define ZB_NWK_ROUTE_DISCOVERY_TABLE_SIZE 5

/* nwkcRouteDiscoveryTime == 0x2710 ms == 10 sec. Expiry function called once
 * per second */
#define ZB_NWK_ROUTE_DISCOVERY_EXPIRY 10

#define ZB_NWK_EXPIRY_ROUTE_DISCOVERY (ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY / ZB_NWK_ROUTE_DISCOVERY_EXPIRY)

/*#define ZB_NWK_RREQ_TABLE_SIZE 5*/

/**
   nwkcInitialRREQRetries

The number of times the first broadcast
transmission of a route request command
frame is retried.

 */
#define ZB_NWK_INITIAL_RREQ_RETRIES 3
/**
   nwkcRREQRetries

The number of times the broadcast
transmission of a route request command
frame is retried on relay by an intermediate
ZigBee router or ZigBee coordinator.
 */
#define ZB_NWK_RREQ_RETRIES 2


#ifndef ZB_NWK_PENDING_TABLE_SIZE
#define ZB_NWK_PENDING_TABLE_SIZE 5
#endif

/* Pending entry expiry when route request is in progress - lets wait one ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY */
#define ZB_NWK_PENDING_ENTRY_EXPIRY 2
#define ZB_NWK_STATIC_PATH_COST 7

/**
   Delay to wait before sending packet after receiving route replay.

   Used only if ZB_CERT_HACKS().delay_pending_tx_on_rresp is 1.
   Useful for test TP_PRO_BV-04.
   Normally ZBOSS sends NWK immediately after receiving any route.
 */
#define ZB_NWK_SEND_AFTER_RRESP_RECV_DELAY_MS 1000

#ifndef ZB_NWK_BTR_TABLE_SIZE
#define ZB_NWK_BTR_TABLE_SIZE 16
#endif

#ifndef ZB_NWK_BRR_TABLE_SIZE
#define ZB_NWK_BRR_TABLE_SIZE 8
#endif

#define ZB_NWK_MAX_BROADCAST_JITTER_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(0x40)
#define ZB_NWK_RREQ_RETRY_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(0xFE)

#define ZB_NWK_RREQ_RETRY_JITTER_MIN ZB_MILLISECONDS_TO_BEACON_INTERVAL(2)
#define ZB_NWK_RREQ_RETRY_JITTER_MAX ZB_MILLISECONDS_TO_BEACON_INTERVAL(128)

#define ZB_NWK_EXPIRY_PENDING (ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY / 2) /* 5 seconds */

/* According to 2013 PICS, it should be 9 sec. 
   According to spec, it should be converted from broadcastDeliveryTime */
#define ZB_NWK_EXPIRY_BROADCAST 9 /* 9 seconds */

/* Check if it is really should be so long. According to r21 spec, 3.6.3.2 - it is out of
 * the scope of this spec. */
#define ZB_NWK_ROUTING_TABLE_EXPIRY 60 /* Should correspond to ( 5
                                        * minutes = 300 seconds / ZB_NWK_EXPIRY_PENDING )  */

/**
 * The maximum number of retries allowed after a broadcast transmission failure.

See PICS NDF4.
*/
#define ZB_NWK_MAX_BROADCAST_RETRIES 0x02

/**
 * The maximum time duration in milliseconds allowed for the parent and all
 * child devices to retransmit a broadcast message.

 See PICS NDF4.
*/
/*0x2710 is too long, ZB_NWK_BROADCAST_DELIVERY_TIME becoms huge */
#define ZB_NWK_PASSIVE_ACK_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(500)

/**
 NWK Broadcast delivery time See 3.5.2.1
nwkBroadcastDeliveryTime = 2*nwkMaxDepth* ((0.05+(nwkcMaxBroadcastJitter/2))+ nwkPassiveAckTimeout*nwkBroadcastRetries/1000
nwkcMaxBroadcastJitter and nwkPassiveAckTimeout are measured in OctetDurations (!).

In PICS NDF4 for PRO:

nwkBroadcastDeliveryTime = 0x44AA27 Octet
durations (9 seconds on
2.4 GHz)
nwkPassiveAckTimeout =
0x3D098 Octet
Durations9 (500 ms on
2.4 GHz) maximum
nwkMaxBroadcastRetries
= 2
 */
#define ZB_NWK_BROADCAST_DELIVERY_TIME() (9 * ZB_TIME_ONE_SECOND)


/**
 * nwkLinkStatusPeriod: period for link status commands, in seconds
 */
#define ZB_NWK_LINK_STATUS_PERIOD 0xf

/**
 * nwkRouterAgeLimit: The number of missed link status command framesbefore
 * resetting the link costs to zero.
*/
#define ZB_NWK_ROUTER_AGE_LIMIT 3

/**
 Maximum number of rejoin requests in progress
*/
#define ZB_NWK_REJOIN_REQUEST_TABLE_SIZE 3

#define ZB_NWK_REJOIN_TIMEOUT (ZB_TIME_ONE_SECOND * 5)

/*
  The rejoining sleeping end-device shall not poll before the response timeout (500ms), such that
  the parent has enough time to process the rejoin request and put a response frame in its indirect
  queue (same as for association response). Depending on the implementation, once a rejoining device
  has performed an active scan and selected a new parent router, it sends a rejoin request (and at
  that time considers this router to be its new parent, otherwise it cannot accept the rejoin
  response), and polls for a response 500ms later.
 */
#define ZB_NWK_REJOIN_FIRST_POLL_DELAY (ZB_TIME_ONE_SECOND / 2)

#define ZB_NWK_REJOIN_POLL_ATTEMPTS 3
//#define ZB_NWK_REJOIN_POLL_ATTEMPTS 6 //TEST

/**
acceptable energy level on channel
*/
#define ZB_NWK_CHANNEL_ACCEPT_LEVEL 0x60


/**
   Default duration to permit joining (currently infinite)
*/
#define ZB_DEFAULT_PRMIT_JOINING_DURATION 0xff

/**
   Default value of nib.max_children - max number of children which can join to this device
*/
#define ZB_DEFAULT_MAX_CHILDREN 32

/* If there was error in NWK security processing of incoming packet from parent,
 * then device tries to rejoin. This define disables this behaviour */
/* NOTE: We need this feature to be enabled for BMS. TBD: move this macro declaration to vendor specific header */
//#define ZB_DISABLE_REJOIN_AFTER_SEC_FAIL

/* By default device is rejoined if network key switch failed */
/*  #define ZB_DISABLE_REJOIN_AFTER_KEY_SWITCH_FAIL */

/*
*  The default value of zb_tc_rejoin flag. Allows ZED to rejoin using Trust Center
*  rejoin procedure
*/
#define ZB_TC_REJOIN_ENABLE

 /*
   Define policy of ignoring assoc. permit and corresponding flags during rejoin
 */
#define  ZB_REJOIN_IGNORES_FLAGS

/**
Workaroud for secure rejoin
*/
#define xZB_NO_KEY_AFTER_REJOIN

//#define ZB_MULTILEAVE_HACK

#define ZB_LEAVE_USE_APP_CALLBACK


/********************ZDO layer options*********************************/
  /**
   ZDO Indirect poll timer
 */
#define ZB_ZDO_INDIRECT_POLL_TIMER (5*ZB_TIME_ONE_SECOND) /* ZB_TIME_ONE_SECOND*10 */

/**
   ZDO Max parent threshold retry
 */
#define ZB_ZDO_MAX_PARENT_THRESHOLD_RETRY 10

/**
 Min scan duration for mgmt_nwk_update_req
*/
#define ZB_ZDO_MIN_SCAN_DURATION 0
/**
 Max scan duration for mgmt_nwk_update_req
*/
#define ZB_ZDO_MAX_SCAN_DURATION 5
/**
 Special value of the scan duration for mgmt_nwk_update_req: change active channel (by number)
 Table 2.88    Fields of the Mgmt_NWK_Update_req Command
 If ScanDuration has a value of 0xfe this is a request  for channel change.
*/
#define ZB_ZDO_NEW_ACTIVE_CHANNEL 0xFE
/**
 Special value of the scan duration for mgmt_nwk_update_req: change channels mask
*/
#define ZB_ZDO_NEW_CHANNEL_MASK   0xFF

/**
 15 minutes timeout. KLUDGE: it is 2 bytes value, 15 minutes is
 nearly maximum value that can be stored,
*/
#define ZB_ZDO_CHANNEL_CHECK_TIMEOUT (ZB_TIME_ONE_SECOND * 60 * 2)

/**
 A countdown timer (in minutes) indicating the time to the next
 permitted frequency agility channel change
*/
#define ZB_ZDO_APS_CHANEL_TIMER (1 * 60)

/**
 15 minutes timer to measure large timeouts
*/
#define ZB_ZDO_15_MIN_TIMEOUT (ZB_TIME_ONE_SECOND * 60 * 15)

/**
 1 minute timer to measure large timeouts
*/
#define ZB_ZDO_1_MIN_TIMEOUT (ZB_TIME_ONE_SECOND * 60)

/* Default values: see HA spec 9.6.4.2 Attribute Settings and Battery Life Considerations */

/**
   Default fast poll timeout
 */
#define ZB_PIM_DEFAULT_FAST_POLL_TIMEOUT (ZB_TIME_ONE_SECOND * 10)

/**
   Default fast poll interval
 */
#define ZB_PIM_DEFAULT_FAST_POLL_INTERVAL ZB_QUARTERECONDS_TO_BEACON_INTERVAL(2)


/**
   Default long poll interval
 */
#define ZB_PIM_DEFAULT_LONG_POLL_INTERVAL (ZB_TIME_ONE_SECOND * 5)

/**
   Default turbo poll interval (TODO: tune it)
 */
#define ZB_PIM_DEFAULT_TURBO_POLL_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(100)

/**
   Minimal possible turbo poll interval
 */
#define ZB_PIM_DEFAULT_MIN_TURBO_POLL_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(100)
//#define ZB_PIM_DEFAULT_MIN_TURBO_POLL_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(60)

/**
   Maximal possible turbo poll interval
 */
#define ZB_PIM_DEFAULT_MAX_TURBO_POLL_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(3000)

/**
   Timeout for turbo poll
 */
#define ZB_PIM_TURBO_POLL_PACKETS_TIMEOUT (ZB_TIME_ONE_SECOND * 15)

/**
   Timeout for turbo poll
 */
#define ZB_PIM_TURBO_POLL_LEAVE_TIMEOUT (ZB_TIME_ONE_SECOND / 3)

/**
   Timeout for poll buffer allocation retry
 */
#define ZB_PIM_POLL_ALLOC_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(500)

/**
   Integer value representing the
   time duration (in milliseconds)
*/
#define ZB_ZDO_NWK_TIME_BTWN_SCANS 30

/**
 Timeout value in seconds
 employed in End Device
 Binding */
#define ZB_ZDO_ENDDEV_BIND_TIMEOUT 30


#ifndef ZDO_TRAN_TABLE_SIZE
/**
 ZDO: transactions table size
*/
#define ZDO_TRAN_TABLE_SIZE 16
#endif

/* Number of packets with "no ack" status before removing unreachable device from nbt. */
#define ZB_ZDO_NEIGHBOR_FAILURE_PKT_THRESHOLD 5
/* Timeout before removing unreachable device from nbt. */
#define ZB_ZDO_NEIGHBOR_UNREACHABLE_TIMEOUT 60 /* 60 seconds */

/**
   Integer value representing the number of scan attempts to make before the NWK layer decides
   which ZigBee coordinator or router to associate with
*/
#define ZB_ZDO_NWK_SCAN_ATTEMPTS 1

/***********************************************************************/
/*************************** MAC SECCTION*******************************/
/***********************************************************************/

/**
RX-to-TX or TX-to-RX maximum turnaround time
(in symbol periods) (see 6.9.1 and 6.9.2)
 */
#define ZB_MAC_A_TURNAROUND_TIME_SYM 12


/* Note: all packet tx timings are for 2.4 GHz radio. */

/**


6.3.1 Preamble field, octets
Table 19Preamble field length

24002483.5 MHz O-QPSK 4 octets 8 symbols
 */
#define ZB_MAC_PEAMBLE_LEN 4


/**
   PHY hdr len, octets


6.3 PPDU format
This subclause specifies the
 */
#define ZB_MAC_PHR_LEN 1


/**

The SFD is a field indicating the end of the SHR and the start of the packet data.


6.3.2 SFD field
 */
#define ZB_MAC_SFD_LEN 1

/**
   MAC frame preamble length, octets

6.3 PPDU format
Each PPDU packet consists of the following basic components:
 A synchronization header (SHR), which allows a receiving device to synchronize and lock onto the
bit stream
 A PHY header (PHR), which contains frame length information
 A variable length payload, which carries the MAC sublayer frame
 */
#define ZB_MAC_FRAME_PPDU_HDR_LEN (ZB_MAC_PEAMBLE_LEN + ZB_MAC_SFD_LEN + ZB_MAC_PHR_LEN)


 /* See D.10.1.1 PPDU Format for European Sub-GHz FSK */
#define ZB_MAC_SUBG_PEAMBLE_LEN 8
#define ZB_MAC_SUBG_SFD_LEN     2
#define ZB_MAC_SUBG_PHR_LEN     2
#define ZB_MAC_SUBG_FRAME_PPDU_HDR_LEN (ZB_MAC_SUBG_PEAMBLE_LEN + ZB_MAC_SUBG_SFD_LEN + ZB_MAC_SUBG_PHR_LEN)
#define ZB_MAC_SUBG_FRAME_PPDU_FOOTER_LEN 2
#define ZB_MAC_SUBG_SYMBOL_DURATION_USEC 10
#define ZB_MAC_SUBG_OCTET_TO_SYMBOL(octets) ((octets) * 8)
#define ZB_MAC_SUBG_TURNAROUND_TIME_SYM 100 /* aRxTxTurnAround - see Table D-23 LBT MAC Sublayer Constants - Implementation */

/**
   Define how many symbols used to transmit given number of octets

6.5.2.2 Bit-to-symbol mapping
All binary data contained in the PPDU shall be encoded using the modulation and spreading functions
shown in Figure 18. This subclause describes how binary information is mapped into data symbols.
The 4 LSBs (b0, b1, b2, b3) of each octet shall map into one data symbol, and the 4 MSBs (b4, b5, b6, b7) of
each octet shall map into the next data symbol. Each octet of the PPDU is processed through the modulation
and spreading functions (see Figure 18) sequentially, beginning with the Preamble field and ending with the
last octet of the PSDU.
 */
#define ZB_MAC_OCTET_TO_SYMBOL(octets) ((octets) * 2)

/**
The minimum value of the
backoff exponent (BE) in the
CSMA-CA algorithm. See
7.5.1.4 for a detailed
explanation of the backoff
exponent.
 */
#define ZB_MAC_MIN_BE 3

/**
The maximum value of the
backoff exponent, BE, in the
CSMA-CA algorithm. See
7.5.1.4 for a detailed
explanation of the backoff
exponent.
 */
#define ZB_MAC_MAX_BE 5


/**
The maximum number of
backoffs the CSMA-CA
algorithm will attempt before
declaring a channel access
failure.
 */
#define ZB_MAC_MAX_CSMA_BACKOFFS 4

/**
The number of symbols forming the basic time period
used by the CSMA-CA algorithm.

See 7.4.1 MAC constants
*/
#define ZB_MAC_A_UNIT_BACKOFF_PERIOD 20

/**
6.9.9 Clear channel assessment (CCA)
The CCA detection time shall be equal to 8 symbol periods.
 */
#define ZB_MAC_CCA_PERIOD 8

/* CCA is performed 2 times in slotted CSMA-CA*/
#define ZB_MAC_NUM_CCA 2

#define ZB_TX_TOTAL_THRESHOLD 20 /* Annex E, total transmissions attempted */
#define ZB_CHANNEL_BUSY_ED_VALUE 0x60  /* TODO: check it */
#define ZB_CHANNEL_FREE_ED_VALUE 0x60  /* TODO: check it */
#define ZB_FAILS_PERCENTAGE   4  /* use it as divider, 25% */

#define ZB_MAC_QUEUE_SIZE 5

/**
The maximum time, in
multiples of
aBaseSuperframeDuration, a
device shall wait for a
response command frame to
be available following a
request command frame.

   MAC: max time to wait for a response command frame, range 2-64
   Default is 32, 64 set for better compatibility
*/
#ifndef ZB_NSNG
#define ZB_MAC_RESPONSE_WAIT_TIME 32
#else
/* Too fast for nsng causing retransmits. TODO: check why can't it work with
 * normal timeouts.  */
#define ZB_MAC_RESPONSE_WAIT_TIME (5 * 4)
#endif

/* Make all MAC PIB attributes configurable */
#define ZB_CONFIGURABLE_MAC_PIB
/**
 MAC: max time to wait for indirect data.

 See 7.4.2 MAC PIB attributes in 802.15.4.

 Note: this time must not be too big.

 As an option: maybe, always send empty data packet as GP does?
*/
/* ZigBee IEEE 802.15.4 Test Spec (ZibBee Doc. 14-0332-01), TP/154/MAC/DATA-04 test:
   macMaxFrameTotalWaitTime for 2.4 GHz 802.15.4-2003 ZigBee/PRO/RF4CE device is
   1220 symbols (or 20 msec) - minimal WaitTime to pass the test.

   Previous value was 500 msec, looks like it is too big. Big WaitTime values bring additional
   battery drain for ED devices.
 */
#define ZB_MAX_FRAME_TOTAL_WAIT_TIME ZB_MILLISECONDS_TO_BEACON_INTERVAL(40)

/**
   MAC: The maximum number of retries allowed after a transmission failure 0-7
*/
#define ZB_MAC_MAX_FRAME_RETRIES 3

/**
   Define it to set MAC address at start (currently - all platforms)
 */
#define ZB_SET_MAC_ADDRESS

/**
  Replay to the beacon request after jitter (meaningful for DSR MAC only).
 */
#ifndef ZB_SEND_BEACON_IMMEDIATELY
#define ZB_SEND_BEACON_AFTER_RANDOM_DELAY
#endif /* ZB_SEND_BEACON_IMMEDIATELY */

/**
  Define it for external MAC
 */
#define ZB_MAC_EXT_DATA_REQ

/* Was 3, seems like it is too small when there are >7-10 beacons.
   Also it seems like we need some bottom line for this - it should not be 1 etc. */
#ifndef ZB_MAC_HANDLE_BEACON_REQ_TMO
#define ZB_MAC_HANDLE_BEACON_REQ_TMO 8
#endif /* ZB_MAC_HANDLE_BEACON_REQ_TMO */


/* Base Device */

/**
   bdbTrustCenterNodeJoinTimeout

   The bdbTrustCenterNodeJoinTimeout attribute specifies a timeout in seconds
   for the Trust Center to remove the Trust Center link key of the newly joined
   node that did not successfully establish a new link key. This attribute
   is used by ZigBee coordinator nodes.
 */
#define ZB_DEFAULT_BDB_TRUST_CENTER_NODE_JOIN_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(0xf * 1000)

#define ZB_BDBC_TCLINK_KEY_EXCHANGE_TIMEOUT ZB_MILLISECONDS_TO_BEACON_INTERVAL(5000)

#define ZB_BDBC_MIN_COMMISSIONING_TIME_S 180

#define ZB_DEFAULT_BDB_TCLINK_KEY_EXCHANGE_ATTEMPTS_MAX 3

#define ZB_STACK_SPEC_VERSION 21


/** @cond touchlink */
/* Table 2 Constants used by nodes supporting touchlink */
#define ZB_BDBC_TL_INTER_PANTRANS_ID_LIFETIME ZB_MILLISECONDS_TO_BEACON_INTERVAL(8000)
#define ZB_BDBC_TL_MIN_STARTUP_DELAY_TIME     ZB_MILLISECONDS_TO_BEACON_INTERVAL(2000)
/* used for a non-extended touchlink scan */
#define ZB_BDBC_TL_PRIMARY_CHANNEL_SET        0x02108800
#define ZB_BDBC_TL_RX_WINDOW_DURATION         ZB_MILLISECONDS_TO_BEACON_INTERVAL(5000)
#define ZB_BDBC_TL_SCAN_TIME_BASE_DURATION    ZB_MILLISECONDS_TO_BEACON_INTERVAL(250)
/* used for an extended touchlink scan after the bdbcTLPrimaryChannelSet
 * channels have been scanned. */
#define ZB_BDBC_TL_SECONDARY_CHANNEL_SET      (0x07fff800 ^ ZB_BDBC_TL_PRIMARY_CHANNEL_SET)
/** @endcond */ /* touchlink */


/* Green Power */


/**
 * @brief zgpDuplicateTimeout value
 *
 * The time the EPP of the ZGPS and ZGPP keeps the information on the received
 * GPDF with random sequence number, in order to filter out duplicates.
 *
 * The default value of 2 seconds can be modified by the application profile
 *
 * @see ZGP spec, A.3.6.1.2.1 */
#define ZB_ZGP_DUPLICATE_TIMEOUT    (1 * ZB_TIME_ONE_SECOND)

/*
  The OperationalChannel sub-field can take the following values: 0b0000: channel 11, 0b0001: channel
  12, , 0b1111: channel 26.
*/
#define ZB_ZGPD_FIRST_CH        11
#define ZB_ZGPD_LAST_CH         26

/* ZGP spec, A.3.9.1:
 * If no Channel Request is received on channel TransmitChannel for 5sec,
 * the TempMaster removes the Channel Configuration GPDF from its
 * gpTxQueue and returns to the operational channel in commissioning mode
 */
#define ZB_ZGP_CHANNEL_REQ_ON_TX_CH_TIMEOUT (5 * ZB_TIME_ONE_SECOND)

/**
 * If commissioning on the last stage, then it determines time to wait
 * until commissioning is complete. If commissioning is not completed
 * during this time, then commissioning cancel procedure is started.
 */
#define ZB_ZGP_TIMEOUT_BEFORE_FORCE_CANCEL (3 * ZB_TIME_ONE_SECOND)

#define ZB_ZGPD_MANUF_ID_UNSPEC     0xFFFF
#define ZB_ZGPD_DEF_MANUFACTURER_ID 0x10d0


#define ZB_ZGP_MAX_LW_UNICAST_ADDR_PER_GPD 2

#define ZB_ZGP_MAX_SINK_GROUP_PER_GPD 2

#define ZB_ZGP_MAX_PAIRED_ENDPOINTS 2

#define ZB_ZGP_MAX_PAIRED_CONF_GPD_COMMANDS 2

#define ZB_ZGP_MAX_PAIRED_CONF_CLUSTERS 2

/*! @} */


/** @cond internals_doc */
/* No need to set manufacturer code in node descriptor directly.
 * zb_set_node_descriptor should select the right one depending on defined platform and/or radio
 * For other codes see docs-05-3874-26-0csg-database-of-manufacturer-codes-for-use-in-the-node-descriptor.pdf */
#define ZB_MANUFACTURER_CODE 0
#define ZB_MANUFACTURER_CODE_GREENPEAK      0x10D0
#define ZB_MANUFACTURER_CODE_BOSCH          0x1133
#define ZB_MANUFACTURER_CODE_DSR            0x1234
/** @endcond */ /* internals_doc */

#define ZB_DEFAULT_MANUFACTURER_CODE ZB_MANUFACTURER_CODE_DSR

#endif /* ZB_CONFIG_COMMON_H */
