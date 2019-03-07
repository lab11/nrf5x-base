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
PURPOSE: Configuration file: configuration- and platform-specific definitions,
constants etc.
*/

#ifndef ZB_CONFIG_H
#define ZB_CONFIG_H 1

/**
   Load vendor specific configurations
*/

/* \defgroup buildconfig Build Configurations
   @{ */
#include "zb_vendor.h"
/**
   Load commin ZBOSS definitions
*/
#include "zb_config_common.h"

#ifdef ZB_ED_ROLE
#define ZB_USE_SLEEP
#endif

/*
 * ZC/ZR - GPPB role library     (default) (implementation of Basic Proxy is mandatory for ZigBee 3.0 ZR)
 * ZC/ZR - GPCB role library               (Options file shall use ZB_ENABLE_ZGP_COMBO define)
 * ZC/ZR - GP Commissioning Tool           (Options file shall use ZGP_COMMISSIONING_TOOL define)
 * ZED   - Target role library   (default) (Options file shall use ZB_ENABLE_ZGP_TARGET define)
 * ZED   - Target+ role library            (Options file shall use ZB_ENABLE_ZGP_TARGET_PLUS define)
 *
 * To enable advanced features (not covered by the curent version of GP specification) please
 * define ZB_ENABLE_ZGP_ADVANCED, so in this case the GPPB/GPCB/GPCT basic libs will be re-compiled
 * to GPP/GPC/GPCT advanced libs
 *
 * GP infrastructure devices:
 *
 *   A.3.2.1 GP Target device ability:
 *    - ability to receive any GP frame in tunneled mode
 *
 *   A.3.2.2 GP Target+ device ability:
 *    - ability to receive any GP frame both in direct mode and in tunneled mode
 *
 *   A.3.2.3 GP Proxy device ability:
 *    - ability to receive any GP frame in direct mode when the proxy is in the radio range of the
 *      GPD
 *    - ability to send to the registered sink devices a GP Notification command with the received
 *      GP frame
 *    - ability to maintain a Proxy Table at commissioning time to register sink devices which are
 *      asking for GP frame forwarding service
 *    - ability to update the Proxy Table based on the observed GP traffic in order to enable GP
 *      device mobility in the network
 *    - ability to drop scheduled tunneling of GP frame, based on received GP commands related to
 *      the same GP frame
 *
 *   A.3.2.4 GP Combo device
 *    - ability to receive any GP frame both in direct mode and in tunneled mode
 *
 * For migration old sink dataset use ZB_ENABLE_ZGP_MIGRATE_OLD_SINK_DATASET define
 */

#ifdef ZB_ZGPD_ROLE
#define ZB_ENABLE_ZGP_DIRECT
#define ZB_ENABLE_ZGP_SECUR
#elif defined ZB_ENABLE_ZGP && !defined ZB_ENABLE_ZGP_TARGET && defined ZB_ED_ROLE
#undef ZB_ENABLE_ZGP
#endif

#if defined ZB_ENABLE_ZGP && !defined ZB_ZGPD_ROLE

/*
#define ZB_ENABLE_ZGP_ADVANCED
*/

#define ZB_ENABLE_ZGP_CLUSTER
#define ZB_ENABLE_ZGP_SECUR
#define ZB_USEALIAS

#ifndef ZB_ED_ROLE
/* ZED can only be implemented as GP Target/Target+ */
#define ZB_ENABLE_ZGP_PROXY
#endif

#ifndef ZB_ENABLE_ZGP_TARGET
#define ZB_ENABLE_ZGP_DIRECT
#define ZB_ENABLE_ZGP_TX_QUEUE
#endif

/** Maximum payload length in translation table entry */
#ifndef ZB_ZGP_TRANSL_CMD_PLD_MAX_SIZE
#define ZB_ZGP_TRANSL_CMD_PLD_MAX_SIZE  3
#endif

#if (defined ZB_ENABLE_ZGP_COMBO || defined ZB_ENABLE_ZGP_TARGET || defined ZB_ENABLE_ZGP_TARGET_PLUS || defined ZGP_COMMISSIONING_TOOL)
#define ZB_ENABLE_ZGP_SINK

/** @brief Max number of command identifiers in one
 *         functionality matching table entry */
#define ZB_ZGP_MATCH_TBL_MAX_CMDS_FOR_MATCH 5

/** @brief Max number of cluster identifiers in one
 *         functionality matching table entry */
#define ZB_ZGP_TBL_MAX_CLUSTERS 5
#endif

#ifndef ZB_ENABLE_ZGP_SINK
#undef ZB_ENABLE_ZGP_MIGRATE_OLD_SINK_DATASET
#endif

/** Sink table size */
#ifndef ZB_ZGP_SINK_TBL_SIZE
#define ZB_ZGP_SINK_TBL_SIZE 32
#endif /* ZB_ZGP_SINK_TBL_SIZE */

/* Obsolete define to support old sink table migration
 * Looks like for correct migration needs to upgrade old system to zgp_dataset_info_ver_6_0_s using
 * the same ZB_ZGP_SINK_TBL_SIZE number */
/* Anyway we don't need do migration for translaion table so let's keep this define as is. */
#define ZB_ZGP_TRANSL_TBL_SIZE 4*ZB_ZGP_SINK_TBL_SIZE

/* 5.1.2.3.2 test specification - The default value for DUT-GPP being a Basic Combo pr a Basic Proxy
 * is "ZigBeeAlliance09", i.e. {0x5A 0x69 0x67 0x42 0x65 0x65 0x41 0x6C 0x6C 0x69 0x61 0x6E
 * 0x63 0x65 0x30 0x39}. */
#define ZB_ZGP_DEFAULT_LINK_KEY "ZigBeeAlliance09"

/* 5.1.2.3.2 test specification - The default value for DUT-GPP being a Basic Proxy or Basic Combo
 * is 0b000. */
#define ZB_ZGP_DEFAULT_SHARED_SECURITY_KEY_TYPE ZB_ZGP_SEC_KEY_TYPE_NO_KEY

/* A.3.3.2.5 The default value is 180 seconds. */
#ifndef ZB_ZGP_DEFAULT_COMMISSIONING_WINDOW
#define ZB_ZGP_DEFAULT_COMMISSIONING_WINDOW 180
#endif

/* in bytes */
#define ZB_ZGP_MIN_PROXY_TABLE_ENTRY_SIZE 7

#ifndef ZB_ZGP_PROXY_COMMISSIONING_DEFAULT_COMMUNICATION_MODE
#define ZB_ZGP_PROXY_COMMISSIONING_DEFAULT_COMMUNICATION_MODE ZGP_PROXY_COMM_MODE_UNICAST
#endif

#define ZB_ZGP_DEFAULT_COMMISSIONING_EXIT_MODE ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT

#define ZB_ZGP_DEFAULT_SEC_LEVEL_PROTECTION_WITH_GP_LINK_KEY 0
#define ZB_ZGP_DEFAULT_SEC_LEVEL_INVOLVE_TC 0
#define ZB_ZGP_MAX_TEMP_MASTER_COUNT 0x03
#define ZB_ZGP_DMAX_FOR_ACCUMULATE_TEMP_MASTER_INFO 100
#define ZB_ZGP_MIN_SINK_TABLE_ENTRY_SIZE 8
#ifndef MAX_ZGP_CLUSTER_GPDF_PAYLOAD_SIZE
#define MAX_ZGP_CLUSTER_GPDF_PAYLOAD_SIZE 65 /* (64 for APP_000 + 1 byte header size) */
#endif
#define ZB_GP_DMIN_U_MS 5
#define ZB_GP_DMIN_D_MS 32

#ifndef ZB_ZGP_DEFAULT_COMMUNICATION_MODE
#define ZB_ZGP_DEFAULT_COMMUNICATION_MODE ZGP_COMMUNICATION_MODE_LIGHTWEIGHT_UNICAST
#endif

#define ZB_ZGP_DEFAULT_SECURITY_LEVEL ZB_ZGP_SEC_LEVEL_NO_SECURITY

#ifdef ZB_ENABLE_ZGP_PROXY
#ifndef ZB_ZGP_PROXY_TBL_SIZE
#define ZB_ZGP_PROXY_TBL_SIZE 16
#endif  /* ZB_ZGP_PROXY_TBL_SIZE */
#endif  /* ZB_ENABLE_ZGP_PROXY */

#endif  /* !ZB_ZGPD_ROLE */

/**
   # of frames in GPFS (repeated frames with same mac seq #)

   For debug purposes set it to 1
*/
#define ZB_ZGPD_GPFS_SIZE 4

#define ZB_ZGP_TX_QUEUE_SIZE 10
#ifdef ZB_ZGP_IMMED_TX
/** Number of zb_zgp_tx_pinfo_t in zb_zgp_tx_packet_info_q_t. */
#define ZB_ZGP_TX_PACKET_INFO_COUNT ZB_ZGP_TX_QUEUE_SIZE + (ZB_IOBUF_POOL_SIZE / 4)
/** Value with which immed_tx_frame_counter is incremented upon a reboot. */
#define ZB_GP_IMMED_TX_FRAME_COUNTER_UPDATE_INTERVAL 1000
#define ZGP_INCLUDE_DST_LONG_ADDR
#else
#define ZB_ZGP_TX_PACKET_INFO_COUNT ZB_ZGP_TX_QUEUE_SIZE
#endif  /* ZB_ZGP_IMMED_TX */
/** Maximum payload length in outgoing ZGP frames */
#ifndef ZB_ZGP_TX_CMD_PLD_MAX_SIZE
#define ZB_ZGP_TX_CMD_PLD_MAX_SIZE  64 /* Maximum payload size of APP_0000 packet */
#endif

#define ZB_ZGP_LED_COMMISSIONING_PARAMS ZB_LED_ARG_CREATE(0, ZB_LED_BLINK_HALF_SEC)

/* some preconditions and dependencies */
#ifdef ZB_BDB_TOUCHLINK
#define ZB_ENABLE_ZLL
#endif

/*
  Features to be specified in vendor config:

  - ZB_ENABLE_HA
  - ZB_ENABLE_ZLL
  - ENABLE_ZGP_TARGET_PLUS

  HA specific:
  -ZB_ALL_DEVICE_SUPPORT

*/

/* BDB uses HA clusters */
#if defined ZB_BDB_MODE && !defined ZB_ENABLE_HA
#define ZB_ENABLE_HA
#endif

/**
   Size of table used for long addresses compression: 3 bytes of manufacturer id.

   ZBOSS implements long address compression: 3 bytes of manufacturer
   id are stored in the separate table; reference to manufacturer
   entry is stored in the long address giving 2 bytes economy.

   That is an absolute limit of # of manufactorers known to the device.

   Note that all that machinery will not work if instead of legal
   maufacturer ids (or illegal, but fixed ids) use random values.
 */
#ifndef ZB_DEV_MANUFACTORER_TABLE_SIZE
#define ZB_DEV_MANUFACTORER_TABLE_SIZE 32
#endif

/**
   ZB_CONFIG_xxxxxx must be defined in build options.
*/

#ifdef ZB_CONFIG_SGW_LINUX_MACSPLIT
#define ZB_PLATFORM_LINUX
/* Linux on Geniatech board doesn't support CLOEXEC option */
#define UNIX
#define ZB_LITTLE_ENDIAN
#define ZB_MACSPLIT
#define ZB_MACSPLIT_HOST
#define ZB_ALIEN_MAC
#define MAC_AUTO_DELAY_IN_MAC_GP_SEND
#define ZB_SOFT_SECURITY
#define ZB_TRANSPORT_LINUX_UART
#endif /* ZB_CONFIG_SGW_LINUX_MACSPLIT */

/* }@ */

/* Allow optional MAC features (e.g., Orhpan Scan client) if our MAC is in
 * use */
#if defined ZB_DSR_MAC
/* #define ZB_OPTIONAL_MAC_FEATURES */
#endif

/***************************ARM Cortex platforms*****************************/

#ifndef ZB_DEVICE_RESET_BUTTON
  #define ZB_DEVICE_RESET_BUTTON 0
#endif

#define ZB_PLATFORM_NRF52840
#define ZB_IAR
#define ZB_LITTLE_ENDIAN
#define ZB_NEED_ALIGN
#define ZB_NO_SYSTEST_SUPPORT
#define ZB_USE_NVRAM
#define ZB_SOFT_SECURITY        /* temporary; switch to hw later */
#define ZB_HW_ZB_AES128
#define USE_ASYNC_UART
#define ZB_INTERRUPT_SAFE_CALLBACKS

#define ZB_IOBUF_POOL_SIZE  40
#define ZB_SCHEDULER_Q_SIZE 30
#define ZB_NRF_52840
#define ZB_ENABLE_HA
#define ZB_BDB_MODE
#define ZB_DISTRIBUTED_SECURITY_ON

/* trace */
#ifdef ZB_TRACE_LEVEL
#define ZB_TRACE_OVER_USART
#define ZB_TRACE_TO_PORT
#define ZB_BINARY_TRACE
#define ZB_TRACE_FROM_INTR
#define ZB_TRAFFIC_DUMP_ON
#define ZB_TRAF_DUMP_V2
#ifndef ZB_TRACE_OVER_JTAG
#define ZB_HAVE_SERIAL
#endif  /* not over jtag */
#endif  /* if trace */

/* our MAC */
#define ZB_USE_NORDIC_RADIO_DRIVER
#define ZB_OSIF_CONFIGURABLE_TX_POWER
#define ZB_MANUAL_ACK
#define ZB_MAC_MANUAL_DUPS
#define ZB_MANUAL_ACK_TIMEOUT
#define ZB_AUTO_ACK_TX
#define MAC_ACK_PARSE_BY_INTR
#define ZB_MAC_AUTO_ACK_RECV
#define ZB_MAC_SINGLE_PACKET_IN_FIFO
#define ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#define ZB_MAC_SOFTWARE_PB_MATCHING
/* #define ZB_MAC_STICKY_PENDING_BIT */
//#define MAC_AUTO_DELAY_IN_MAC_GP_SEND
#define MAC_AUTO_GPDF_RETX
#define ZB_MAC_RX_QUEUE_CAP 5
#ifndef ZB_NVRAM_BUF_SIZE
#define ZB_NVRAM_BUF_SIZE 256
#endif
#define ZB_NO_NVRAM_VER_MIGRATION

/* #define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   (0xffff << 11) */ /* 0000.0111 1111.1111 1111.1000 0000.0000*/
/* C51 doesn't like long shifts, it just cut last two bytes. (11-26) */
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000*/

/* @} */

/***********************************************************************/
/*************************** MAC SECTION********************************/
/***********************************************************************/

/* ZB_DEBUG_ENLARGE_TIMEOUT is used as multiplier for debug build */
#define ZB_DEBUG_ENLARGE_TIMEOUT 1

#ifndef ZB_MEMORY_COMPACT
/**
   If defined, switch ON visibility device limit functionality (drop packets from invisible devices at MAC level).
*/
#define ZB_LIMIT_VISIBILITY
#endif /*ZB_MEMORY_COMPACT*/

#define MAC_DEVICE_TABLE_SIZE 4

#if defined ZB_ENABLE_INTER_PAN_EXCHANGE && ! defined ZB_MAC_EXT_DATA_REQ
#define ZB_MAC_EXT_DATA_REQ
#endif /* defined ZB_ENABLE_INTER_PAN_EXCHANGE && ! defined ZB_MAC_EXT_DATA_REQ */

/***********************************************************************/
/****************************General stack options**********************/
/***********************************************************************/

#define ZB_VERSION "0.2.0.1"

/*! \addtogroup ZB_CONFIG */
/*! @{ */

/** Ids of stack profiles, network selection, 2007, and Pro respectively */
#define STACK_NETWORK_SELECT  0x00
#define STACK_2007            0x01
#define STACK_PRO             0x02

/**
   This define switches PRO version of or off.
*/
#define ZB_PRO_STACK

/**
   Define to let us work properly with Ember stack
*/
// #define ZB_EMBER_GOLDEN_UNIT

/**
   If defined, 2007 stack profile is implemented
*/
//#define ZB_STACK_PROFILE_2007

/**
 *    If defined, NVRAM is used
 *    Create *.nvram file
 *
 *    To be used near always to prevent flash damage (flash can do ~1000 rewrites only)
 *
 *    For certification test need comment.
 *    Else change Assignment to Rejoin for second and last runs
 *
 */
//#define NVRAM_NOT_AVAILABLE

#ifndef NVRAM_NOT_AVAILABLE
#define ZB_USE_NVRAM
#endif

/**
   Use 32 bit timer
*/
#define ZB_TIMER_32

/**
   ZigBee channel selection.
   Default value of ZB_AIB().aps_channel_mask
   Channels to be used for discovery and join.
   If here is single channel, not need to do energy scan.

   Note: ZLL, HA, ZLL tests can redefine this mask by other constants
*/

/* ZB_DEFAULT_APS_CHANNEL_MASK can be redefined in zb_vendor.h */
#ifndef ZB_DEFAULT_APS_CHANNEL_MASK
//#define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<11))
/* Change that place if you want to set channel for internal tests */
//#define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<11))
/* #define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<16)) */
#define ZB_DEFAULT_APS_CHANNEL_MASK (ZB_TRANSCEIVER_ALL_CHANNELS_MASK)
#endif  /* ZB_DEFAULT_APS_CHANNEL_MASK */

/**
   If defined, do not compile some features, even Mandatory, to fir into 64k ROM at 2410/64k device in Keil debug build.
*/
//#define ZB_LIMITED_FEATURES

/**
   PRO stack specific details
*/
#ifndef ZB_PRO_STACK
/**
   NWK: If defined, use distributed address assing for tree and for mesh routing (ZigBee 2007).
*/
#define ZB_NWK_DISTRIBUTED_ADDRESS_ASSIGN
#else /*ZB_PRO_STACK*/
/**
   NWK: If defined, use stochastic address assing (ZigBee PRO).
*/
#define ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN
/* Source routing path length, also called nwkMaxSourceRoute */
#define ZB_NWK_MAX_PATH_LENGTH  3
/* Source route table capacity */
#define ZB_NWK_MAX_SRC_ROUTES   3

#endif /*ZB_PRO_STACK*/

/****************************ZigBee Roles*******************************/

#if !defined ZB_ED_ROLE && !defined ZB_ZGPD_ROLE
/**
   If defined, ZC functionality is compiled
   Implies ZR role as well
*/
#define ZB_COORDINATOR_ROLE
#endif /*!defined ZB_ED_ROLE && !defined ZB_ZGPD_ROLE*/

#if defined ZB_COORDINATOR_ROLE && !defined ZB_ROUTER_ROLE
/* coordinator supposes router */
/**
   If defined, ZR role compiled.
*/
#define ZB_ROUTER_ROLE
#endif /*defined ZB_COORDINATOR_ROLE && !defined ZB_ROUTER_ROLE*/

#if defined ZB_COORDINATOR_ROLE
/**
   NWK: size of the neighbor table
*/
#define ZB_NEIGHBOR_TABLE_SIZE 32

#endif

#if defined ZB_ROUTER_ROLE
/**
   NWK: size of the neighbor table + ext neighbor table
*/
#define ZB_NEIGHBOR_TABLE_SIZE 32

#endif

#if defined ZB_ED_ROLE
/**
   End device idle time-out
*/
#define ZB_TIME_ED_IDLE ZB_MILLISECONDS_TO_BEACON_INTERVAL(7500)
/* Only parent is meangful for ED */
/**
   NWK: size of the neighbor table +  ext neighbor table
*/
#define ZB_NEIGHBOR_TABLE_SIZE 32

#endif
#if defined ZB_ZGPD_ROLE
/* zcl_common.zgpd.o won't be built without any ZB_NEIGHBOR_TABLE_SIZE definition. */
#define ZB_NEIGHBOR_TABLE_SIZE 1
#endif

/**
   Scheduler callbacks queue size. Ususlly not need to change it.
*/
#ifndef ZB_SCHEDULER_Q_SIZE
#ifdef ZB_ED_ROLE
#define ZB_SCHEDULER_Q_SIZE 16
#else
/* Note: is set ZB_SCHEDULER_Q_SIZE > 32, must change alarms bitmask in the scheduler */
#define ZB_SCHEDULER_Q_SIZE 32
#endif
#endif /*ZB_SCHEDULER_Q_SIZE*/

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE

/**
   NWK: If defined, enable routing functionality
*/
#define ZB_NWK_ROUTING

/**
   Number of secure materials to store
*/
#define N_SECUR_MATERIAL 3

/**
   If not defined, implement tree routing
*/
#ifndef ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN
#define ZB_NWK_TREE_ROUTING
#endif /*ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN*/
/**
   NWK: if defined, implement mesh routing
*/
#define ZB_NWK_MESH_ROUTING

#else

#define N_SECUR_MATERIAL 1

#endif  /* coordinator or router */

/****************************APS layer options**************************/
#ifndef ZB_MAX_EP_NUMBER
#define ZB_MAX_EP_NUMBER 5 /* max supported EP number, increase if needed */
#endif
/* APS binding */

#ifndef ZB_MEMORY_COMPACT
/**
   APS: SRC binding tble size
*/
#ifndef ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_SRC_BINDING_TABLE_SIZE 32
#endif

/**
   APS: DST binding table size
*/
#ifndef ZB_APS_DST_BINDING_TABLE_SIZE
#define ZB_APS_DST_BINDING_TABLE_SIZE 32
#endif

/**
   APS: man number of groups in the system
*/
#ifndef ZB_APS_GROUP_TABLE_SIZE
#define ZB_APS_GROUP_TABLE_SIZE       16
#endif

/**
   APS: max number of endpoints per group table entry
*/
#ifndef ZB_APS_ENDPOINTS_IN_GROUP_TABLE
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 8
#endif

/**
   APS: size of queue to be used to pass
   incoming group addresses packets up
*/
#define ZB_APS_GROUP_UP_Q_SIZE 8

#else  /* ZB_MEMORY_COMPACT */

#ifndef ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_SRC_BINDING_TABLE_SIZE 8
#endif

/**
   APS: DST binding table size
*/
#ifndef ZB_APS_DST_BINDING_TABLE_SIZE
#define ZB_APS_DST_BINDING_TABLE_SIZE 8
#endif

/**
   APS: man number of groups in the system
*/
#ifndef ZB_APS_GROUP_TABLE_SIZE
#define ZB_APS_GROUP_TABLE_SIZE       4
#endif

/**
   APS: max number of endpoints per group table entry
*/
#ifndef ZB_APS_ENDPOINTS_IN_GROUP_TABLE
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 4
#endif

/**
   APS: size of queue to be used to pass
   incoming group addresses packets up
*/
#define ZB_APS_GROUP_UP_Q_SIZE 4

#endif  /* ZB_MEMORY_COMPACT */

/**
 APS: maximum number of tables with key-pair information
*/
#ifndef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
#ifdef ZB_ED_ROLE
#define ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE            5
#else
/* [EE] 05/25/2016 CR:MINOR Set it to some big value - say, 128. This is total #
 * of devices in the network, not neighbor table size */
#define ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE            ZB_NEIGHBOR_TABLE_SIZE
#endif
#endif

/**
   If the joining device does not receive any of
   the keys within apsSecurityTimeOutPeriod, of receiving the NLMEJOIN.
   confirm primitive, it shall reset

   i.e. 1.7 seconds on 2.4 GHz.
*/
#ifdef ZB_PRO_STACK
#define ZB_APS_SECURITY_TIME_OUT_PERIOD ZB_MILLISECONDS_TO_BEACON_INTERVAL(1700)
#else
/* i.e. 700 milliseconds on 2.4 GHz */
#define ZB_APS_SECURITY_TIME_OUT_PERIOD ZB_MILLISECONDS_TO_BEACON_INTERVAL(700)
#endif

/**
 APS: Get APS ACK wait time for the device depending on its receiver on when idle
*/
/* WARNING: Generally, it is not by spec. There are some nwk and aps routines that should
 * correllate to this interval, for these we will use ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY. */
#ifndef ZB_ED_RX_OFF_WHEN_IDLE
#define ZB_N_APS_ACK_WAIT_DURATION(_rx_on_when_idle)  \
  ((_rx_on_when_idle) ?                                                 \
   ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY : ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY)
#else
#define ZB_N_APS_ACK_WAIT_DURATION(_rx_on_when_idle)  \
  (ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY)
#endif

#ifndef ZB_APS_DUPS_TABLE_SIZE
#define ZB_APS_DUPS_TABLE_SIZE 32
#endif

/****************************NWK layer options**************************/

/*
  If defined, NWK and upper layers can access PIB directly.

  Usually, can be defined when using our MAC.
  Undef it to separate MAC and upper layers IB spaces for debug purposes.

  #define ZB_DIRECT_PIB_ACCESS

*/
/**
   NWK: size of the long-short address translation table
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_IEEE_ADDR_TABLE_SIZE 101
#else
#define ZB_IEEE_ADDR_TABLE_SIZE 32
#endif

#ifndef ZB_PANID_TABLE_SIZE
/**
   NWK: size of the long-short panid translation table

   Must be <= (packet buffer size - sizeof(*discovery_confirm)) / sizeof(*network_descriptor)

   That value limits number of PANs visible for device during active scan.
*/
#define ZB_PANID_TABLE_SIZE 16
#endif

/**
   If defined, add jitter to broadcast transmits etc.

   Note: it adds zb_random() and, so. breaks some tests which supposes to know
   next generated address by setting random generator start.
*/
#define ZB_NWK_USE_SEND_JITTER

/* Some defaults for ZDO startup */

#ifndef ZB_TRACE_LEVEL
/**
   NWK: default energy/active scan duration
*/
#define ZB_DEFAULT_SCAN_DURATION 3
#else
#define ZB_DEFAULT_SCAN_DURATION 5
#endif /*ZB_TRACE_LEVEL*/

/**
   Number of pending Mgmt_Leave requests allowed
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_ZDO_PENDING_LEAVE_SIZE 4
#else
#define ZB_ZDO_PENDING_LEAVE_SIZE 2
#endif /*ZB_MEMORY_COMPACT*/

#ifndef ZB_LITE_COORDINATOR_ONLY_ROLE
/* Enables PAN blacklisting */
#define ZB_NWK_BLACKLIST
#ifndef ZB_NWK_BLACKLIST_SIZE
#define ZB_NWK_BLACKLIST_SIZE 16
#endif
#endif

/**
   NWK Mesh route stuff: routing table size
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_NWK_ROUTING_TABLE_SIZE ZB_NEIGHBOR_TABLE_SIZE
#else
#define ZB_NWK_ROUTING_TABLE_SIZE 5
#endif

/**
    Neighbour path cost calculation method.
    By default, calculate neighbour path cost from LQI value as describe in ZB
    spec. But for sime special cases (e.g. LCGW)) we need to calculate path cost
    from RSSI value.
 */
#ifndef ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED
#define ZB_NWK_NEIGHBOUR_PATH_COST_LQI_BASED
#endif /* ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED */

/***********************************************************************/
/***************************ZBOSS FEATURES SECTION**********************/
/***********************************************************************/

/***************************STACK FEATURES**********************/

#if defined ZB_EMBER_GOLDEN_UNIT
#define ZB_STACK_PROFILE        STACK_NETWORK_SELECT
#define ZB_PROTOCOL_VERSION     0x02
#elif defined ZB_PRO_STACK
#define ZB_STACK_PROFILE        STACK_PRO
#else
#define ZB_STACK_PROFILE        STACK_2007
#endif

/**
   Protocol version selection: see table 1.1(pro-specification)
*/
#if ZB_STACK_PROFILE == STACK_PRO || ZB_STACK_PROFILE == STACK_2007 || defined ZB_EMBER_GOLDEN_UNIT
#define ZB_PROTOCOL_VERSION 0x02
#else
#define ZB_PROTOCOL_VERSION 0x01
#endif

/**
   Size of queue for wait for free packet buffer
*/
#ifndef ZB_BUF_Q_SIZE
/* Note: to prevent deadlocks ZB_BUF_Q_SIZE must be < ZB_IOBUF_POOL_SIZE/2 */
#define ZB_BUF_Q_SIZE 8
#endif /*ZB_BUF_Q_SIZE*/

/**
   Size, in bytes, of the packet buffer

   Be sure keep it multiple of 4 to exclude alignment problems at ARM
*/
#define ZB_IO_BUF_SIZE 148

/**
   Number of packet buffers. More buffers - more memory. Less buffers - risk to be blocked due to buffer absence.
*/
#ifndef ZB_IOBUF_POOL_SIZE
#if defined ZB_MEMORY_COMPACT && defined ZB_ED_ROLE
#define ZB_IOBUF_POOL_SIZE 20
#else
#define ZB_IOBUF_POOL_SIZE 26
#endif
#endif

/*
  If enabled, real int24 and uint48 types will work. Else, int24 and uint48 is mapped to
  int32 and uint32.
  See include/zb_uint48.h.
*/
#define ZB_UINT24_48_SUPPORT

#define ZB_NWK_ROUTING_FAILURE_ATTEMTS_NUM 1  /*Should expire in this number of
                                               * attemptes if failure * */

/*Calculate routing expiry step interval based on timer initiation value and number of attempts*/
#define ZB_NWK_ROUTING_FAILURE_EXPIRY_STEP (ZB_NWK_ROUTING_TABLE_EXPIRY / ZB_NWK_ROUTING_FAILURE_ATTEMTS_NUM)

/**
   Number of times device failes to send packet to the parent before rejoin
*/
#define ZB_ZDO_PARENT_LINK_FAILURE_CNT 12
#define ZB_ZDO_CHECK_FAILS_NWK_UPDATE_NOTIFY_LIMIT 4
#define ZB_ZDO_CHECK_FAILS_NWK_UPDATE_NOTIFY_TIMEOUT (60 * 60 * ZB_TIME_ONE_SECOND)
#define ZB_ZDO_CHECK_FAILS_CLEAR_TIMEOUT (30 * ZB_TIME_ONE_SECOND)

#define ZB_PREDEFINED_ROUTER_ADDR 0x3344
#define ZB_PREDEFINED_ED_ADDR     0x3344

#ifdef ZB_LIMIT_VISIBILITY
/**
   Maximum number of addresses in the visibility limit arrays
*/
#define ZB_N_VIZIBLE_ADDRESSES 6
#endif

#if defined ZB_LITTLE_ENDIAN && defined ZB_BIG_ENDIAN
#error  Enable only 1 profile support: ZB_LITTLE_ENDIAN or ZB_BIG_ENDIAN
#endif /* defined ZB_LITTLE_ENDIAN && defined ZB_BIG_ENDIAN */

/* DA: network status with OUT_OF_MEMORY custom value */
#if !defined ZB_CHECK_OOM_STATUS && !defined xZB_CHECK_OOM_STATUS && !defined ZB_MACSPLIT_DEVICE
#define ZB_CHECK_OOM_STATUS
#endif
//#define ZB_SEND_OOM_STATUS

/***************************HA and ZLL FEATURES**********************/

#if defined ZB_ENABLE_HA || defined ZB_ENABLE_ZLL
#define ZB_ENABLE_ZCL
#endif /* defined ZB_ENABLE_HA || ZB_ENABLE_ZLL */

/* Enable only 1 profile support: ZLL or HA */

#if defined ZB_ENABLE_ZLL || defined DOXYGEN
/** @brief Controls inter-PAN exchange feature */
#define ZB_ENABLE_INTER_PAN_EXCHANGE
/* only ZLL uses AES128 decoding */
#define ZB_NEED_AES128_DEC
#define ZB_ZLL_ENABLE_COMMISSIONING_CLIENT
#define ZB_ZLL_ENABLE_COMMISSIONING_SERVER
#ifdef ZB_ZLL_ENABLE_COMMISSIONING_CLIENT
/* Client device should be address assignment capable */
#define ZB_ZLL_ADDR_ASSIGN_CAPABLE
#endif
#define ZB_ZLL_PRIMARY_CHANNELS {11, 15, 20, 25}
#endif /* defined ZB_ENABLE_ZLL || defined DOXYGEN */

/*************************Serial and UDP trace**********************/
#if defined ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG || defined ZB_NET_TRACE
/* binary trace: optimize traffic size. need special win_com_dump */
#define ZB_BINARY_TRACE
/* #define ZB_TRAFFIC_DUMP_ON */
/* #define ZB_TRAF_DUMP_V2 */
#endif /*ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG || defined ZB_NET_TRACE*/

#if ! (defined ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG) && defined ZB_NET_TRACE
#define ZB_FAKE_SERIAL_TRACE
#endif /*! (defined ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG) && defined ZB_NET_TRACE*/

#define ZB_MEMTRACE_BUF_SIZE 4080
/* Tune trace portion to fit HTTPS body into single Ethernet frame. Align to 3
 * because base64 packs 3 bytes into 4 chars. */
#define ZB_NET_TRACE_PORTION 1020

/**
   If defined, transport calls traffic dump by itself, generic MAC not need to do it
*/
//#define ZB_TRANSPORT_OWN_TRAFFIC_DUMP_ON

//#define ZB_DUMP_TRAF

/**
   Name for trace off switch file

   If file of this name exists in the current directory, swicth off both trace
   and traffic dump.
*/
#define ZB_TRACE_SWITCH_OFF_FILE_NAME "trace_off"

/************************Hardware watchdog******************/

/* Check watchdods once per 20 sec. Rational: H/W WD timer is set for 30 sec,
   we need to kick it even there is nothing to do */
#ifndef ZB_WATCHDOG_SCHED_QUANT
#define ZB_WATCHDOG_SCHED_QUANT (20 * ZB_TIME_ONE_SECOND)
#endif

#define ZB_OOM_THRESHOLD (120 * ZB_TIME_ONE_SECOND)
#define ZB_SEND_OOM_DELAY (5 * ZB_TIME_ONE_SECOND)

/* membuffers debug: */
/* extra buffers trace and debug */
/* WARNING: Disabled by default. It costs too many ROM size! */
#if ZB_TRACE_LEVEL && ZB_TRACE_MASK
/* #define ZB_DEBUG_BUFFERS */
#endif

/* Assert when already freed memory buffer is accessed. Useful for debugging */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_BUF_SHIELD
#endif /*ZB_DEBUG_BUFFERS*/

/* extended membuffers debug*/
/*
#define ZB_DEBUG_BUFFERS_EXT
#define ZB_DEBUG_BUFFERS_EXT_USAGES_COUNT 1
*/

#if(defined(ZB_DEBUG_BUFFERS_EXT) && !defined(ZB_DEBUG_BUFFERS))
#error "turn on ZB_DEBUG_BUFFERS."
#endif

/************************Special modes for testing*******************/

/**
   This define turns on/off test profile
*/
#ifdef DEBUG
#if !defined ZB_LIMITED_FEATURES && !defined ZB_TEST_PROFILE && !defined xZB_TEST_PROFILE
/**
   Compile Test Profile feature
*/
#define ZB_TEST_PROFILE
#endif /*ZB_LIMITED_FEATURES*/
#endif /*DEBUG*/

/* Testing mode for some pro certification tests */
#ifdef ZB_PRO_STACK

#define ZB_PRO_TESTING_MODE
#ifdef ZB_PRO_TESTING_MODE

/* See: certification test TP_PRO_BV-11 */
#ifdef DEBUG
#if !defined ZB_LIMIT_VISIBILITY && !defined ZB_MEMORY_COMPACT
#define ZB_LIMIT_VISIBILITY
#endif /*!defined ZB_LIMIT_VISIBILITY && !defined ZB_MEMORY_COMPACT*/
#endif /*DEBUG*/

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
/* See: certification test 13.66_Commissioning_the_short_address */
#define ZB_PRO_ADDRESS_ASSIGNMENT_CB
#endif /*defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE*/

/* Compatibility tests (with Ember ZigBee platform ISA3)*/

/* Ember + ZBOSS stack with ZLL Profile*/
#ifdef ZB_ENABLE_ZLL
#define ZLL_TEST_WITH_EMBER
#endif /*ZB_ENABLE_ZLL*/

/* Ember + ZBOSS Core Pro stack */
//#define ZB_EMBER_TESTS

/* Send messages with empty payload without APS-security */
#define ZB_SEND_EMPTY_DATA_WITHOUT_APS_ENCRYPTION

#endif /* ZB_PRO_TESTING_MODE */

#endif /* ZB_PRO_STACK */

/**
   +  This define is for APS retransmissions test, do not use it for the normal work
*/
//#define APS_RETRANSMIT_TEST

/* Definitions for 802.15.4 certification hacks */

//#define MAC_CERT_TEST_HACKS

/**
   This define turns on/off channel error mode (set errors while data sending)
*/
//#define ZB_CHANNEL_ERROR_TEST

//#define TP_PRO_BV_31

//#define TP_PRO_BV_38

/* Certification defines */

/**
   Disabling of processing of bind_req and end_device_bind_req for certification purposes
*/
#if 0
#define ZB_DISABLE_BIND_REQ
#endif

/**
   Disabling of processing of end device bind requests
*/
#if 0
#define ZB_DISABLE_ED_BIND_REQ
#endif

/*PRO STACK COMPATIBILITY, it's better to turn it on only when needed */
/* #AT disable compatible mode with 2007 stack version */
#if 0
#define ZB_PRO_COMPATIBLE
#endif

/**
   Default value for mac frame version subfield
*/
#define MAC_FRAME_VERSION MAC_FRAME_IEEE_802_15_4_2003

/**
   Block sleepy end devices (devices with rx-on-when-idle == false) from receiving broadcasts (drop it at MAC level)
*/
#define ZB_BLOCK_BROADCASTS_SLEEPY_ED

/* Specially for test with Daintree: do not ask ACK for key transport:
   Daintree wants to encrypt ACK by its predefined key before it receive key from us */
#define ZB_DISABLE_APS_ACK_FOR_TRANSPORT_KEY

#ifdef ZB_ZGPD_ROLE
#define ZB_MAC_ONLY_STACK
#undef ZB_CHECK_OOM_STATUS
#undef APS_FRAME_SECURITY
#undef ZB_ENABLE_ZCL
#undef ZB_ENABLE_ZLL
#undef ZB_ENABLE_HA
#undef ZB_USE_NVRAM
/* Temporarily disabled for ZGPD MAC library build. */
/* #undef ZB_DIRECT_MODE_WITH_ZGPD */
#define ZB_ENABLE_ZGPD_ATTR_REPORTING
/*
 * The OperationalChannel sub-field can take the following values: 0b0000: channel 11,
 * 0b0001: channel 12, , 0b1111: channel 26.
 * 26-11+1 = 12. Let's use 3 series of 4 channels each. Note that channels
 * diapason must be multiple of series. */
#define ZB_ZGPD_CH_SERIES       3

#define ZB_GPD_COMMISSIONING_RETRY_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(500)
#endif /* ZB_ZGPD_ROLE */

#ifdef ZB_ENABLE_ZGP_DIRECT
/* Old GP GPD sensors (until May, 2014) use
 * 5 ms offset. New sensors use 20 ms offset.*/
//#define ZB_GPD_RX_OFFSET_MS 5
#define ZB_GPD_RX_OFFSET_MS 20
/* gpTxOffset should be calibrated for each HW/SW variant.
 * The reason is that ZB_GPD_TX_OFFSET_US is used from ZGP stub during Green Power frame transmitting,
 * and the actual TX time is not known at this level due to delays of MAC layer or transceiver.
 */
#define ZB_GPD_TX_OFFSET_US                                   \
  (ZB_MILLISECONDS_TO_USEC(ZB_GPD_RX_OFFSET_MS))
/**
   gpdMinRxWindow
   The gpdMinRxWindow is minimal duration of the reception window of an Rx-capable GPD.
   It has the value of 0.576 ms that corresponds to the Channel Configuration GPDF of 18B.
   GPD vendors shall implement reception window duration that corresponds to the actual GPD frame
   size to be received by this GPD, which may never be shorter than
   gpdMinRxWindow.

   A.1.6.3.2 gpdMinRxWindow
*/
#define ZB_GPD_MIN_RX_WINDOW_MS 50
#endif  /* ZB_ENABLE_ZGP_DIRECT */

/************************NVRAM SUPPORT*******************/
/**
   Storing NWK security counter in NVRAM
*/
#if defined ZB_USE_NVRAM
#define ZB_STORE_COUNTERS
#define ZB_LAZY_COUNTER_INTERVAL 0x400
#endif /*ZB_USE_NVRAM*/

/**
   Migration datasets when page overflow in NVRAM
*/
#define ZB_NVRAM_DATASET_MIGRATION

/* Save address map in the NVRAM */
#define ZB_STORE_ADDR_MAP

/* Save neighbor table in the NVRAM */
#define ZB_STORE_NEIGHBOR_TBL

#define ZB_NVRAM_DATASET_OWN_VERSIONING

/*! @} */

/* Check for old GP definitions and produce a error */
#if defined ENABLE_ZGP_TARGET_PLUS || defined ZB_ENABLE_ZGP_EP || defined ZB_DIRECT_MODE_WITH_ZGPD
#error Old ZGP define detected!
#endif

/* Do not allow ZR to join as ZED by default. */
#define ZB_LITE_NO_JOIN_ZR_AS_ZED

/* Since R21 NWK multicast should be turned off by default */
#define ZB_LITE_NO_NWK_MULTICAST

/* ZBOSS Lite defines */

/*
  If defined maximum possible features cut
 */

#ifdef ZB_ZBOSS_LITE_MAX
#ifndef ZB_LITE_NO_SOURCE_ROUTING
/**
   Disable source routing
 */
#define ZB_LITE_NO_SOURCE_ROUTING
#endif

#ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY
/**
   Disable route discovery API (but keep implicit route discovery)
 */
#define ZB_LITE_NO_NLME_ROUTE_DISCOVERY
#endif

#ifndef ZB_NO_NWK_MULTICAST
/**
   Disable NWK multicast. Use APS grops and NWK broadcast instead.
 */
#define ZB_NO_NWK_MULTICAST
#endif

#ifdef ZB_LITE_NO_PANID_CONFLICT_DETECTION
/**
   Disable pan id conflict detection
 */
#define ZB_LITE_NO_PANID_CONFLICT_DETECTION
#endif

#ifdef ZB_LITE_NO_LINK_COST
/**
   Disable nwk link cost based on lqi

   See 3.6.3.1 Routing Cost
 */
///not sure this is good idea #define ZB_LITE_NO_LINK_COST
#endif

#ifndef ZB_LITE_NO_JOIN_ZR_AS_ZED
/**
   Do not attempt to join ZR as ZED if join as ZR failed.
   Do not join as ZED if joinint net is prior PRO.
 */
#define ZB_LITE_NO_JOIN_ZR_AS_ZED
#endif

#ifndef ZB_LITE_NO_ORPHAN_SCAN
//#define ZB_LITE_NO_ORPHAN_SCAN
#endif

#ifndef ZB_LITE_NO_APS_DATA_ENCRYPTION
/**
   Do not encrypt data at APS level
 */
#define ZB_LITE_NO_APS_DATA_ENCRYPTION
#endif

#ifndef ZB_LITE_NO_STORE_APS_COUNTERS
/**
  Do not store APS security frame counters in nvram
 */
#define ZB_LITE_NO_STORE_APS_COUNTERS
#endif

/**
   Do not check for APS security counters inconsistence at unsecure
 */
#ifndef ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
#define ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
#endif

#ifdef ZB_DISTRIBUTED_SECURITY_ON
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif

#ifdef ZB_SECURITY_INSTALLCODES
#undef ZB_SECURITY_INSTALLCODES
#endif

#ifdef ZB_CERTIFICATION_HACKS
#undef ZB_CERTIFICATION_HACKS
#endif

/**
   Do not send switch key unicast
 */
#ifndef ZB_LITE_NO_UNICAST_SWITCH_KEY
#define ZB_LITE_NO_UNICAST_SWITCH_KEY
#endif

/**
   Treate all APS keys as Unique.
(Unique vs Global is brain-damaging flag to define encryption/non wncryption at
APS of some APS commands, like Update Device. It introduced in r20 probably for
compatibility with some mammoth shit */
#ifndef ZB_LITE_NO_GLOBAL_VS_UNIQUE_KEYS
//#define ZB_LITE_NO_GLOBAL_VS_UNIQUE_KEYS
#endif

/**
   Never require unique keys exchange on join
 */
#ifndef ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
#define ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
#endif

#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
/**
   Exclude ZDO Syetrm Server Discovery
 */
#define ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
#endif

#ifndef ZB_LITE_NO_ZDO_MGMT_RTG
#define ZB_LITE_NO_ZDO_MGMT_RTG
#endif

#ifndef ZB_LITE_NO_FULL_FUNCLIONAL_MGMT_NWK_UPDATE
#define ZB_LITE_NO_FULL_FUNCLIONAL_MGMT_NWK_UPDATE
#endif

/**
   Do not try to inject LEAVE on data request from ZED which we already timed out.

   That define removes stupid r21 feature which requires from us to always set
   pending bit in any our real devices (just because we are a) not so fast to check
   _absence_ of device in the neighbor table and replay with MAC ACK and b) want
   to use auto-ack feature of radio which support it - like TI devices).
 */
#ifndef ZB_LITE_NO_LEAVE_INJECTION
#define ZB_LITE_NO_LEAVE_INJECTION
#endif
#ifdef ZB_LITE_NO_LEAVE_INJECTION
#ifdef ZB_MAC_STICKY_PENDING_BIT
#undef ZB_MAC_STICKY_PENDING_BIT
#endif
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
#define ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
#endif

#ifdef ZB_LITE_COORDINATOR_ONLY_ROLE
#ifdef ZB_DISTRIBUTED_SECURITY_ON
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif
#endif

#ifndef ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/**
   Disable mgmt leave with requires sendint leave to third device.
 */
#define ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/* Need to store only one entry - about leave myself */
#ifdef ZB_ZDO_PENDING_LEAVE_SIZE
#undef ZB_ZDO_PENDING_LEAVE_SIZE
#define ZB_ZDO_PENDING_LEAVE_SIZE 1
#endif
#endif

#ifndef ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF
/**
   Don't pass from APS up packet which is sent to myself - either directly or
   via group.
 */
#define ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF
#endif

#ifndef ZB_LITE_BDB_ONLY_COMMISSIONING
/**
   Allow only BDB commissioning and disallow old ZBOSS start sequence
 */
#define ZB_LITE_BDB_ONLY_COMMISSIONING
#endif
#if defined ZB_LITE_BDB_ONLY_COMMISSIONING && !defined ZB_BDB_MODE
#define ZB_BDB_MODE
#endif

#ifndef ZB_LITE_LIMIT_PIB_ACCESS
/**
   Limit access to PIB to values really used by the stack
 */
#define ZB_LITE_LIMIT_PIB_ACCESS
#endif

#ifndef ZB_LITE_ALWAYS_SECURE
/**
   No ability to switch off security
 */
#define ZB_LITE_ALWAYS_SECURE
#endif

#ifndef ZB_LITE_DONT_STORE_RSSI
/**
   Do not store rssi in the neigbor
 */
//#define ZB_LITE_DONT_STORE_RSSI
//#define ZB_LITE_DONT_STORE_RSSI
#endif

/**
   Disable end device bind
 */
#ifndef ZB_LITE_NO_END_DEVICE_BIND
#define ZB_LITE_NO_END_DEVICE_BIND
#endif

#ifdef ZB_TEST_PROFILE
#undef ZB_TEST_PROFILE
#endif

#endif  /* ZB_ZBOSS_LITE_MAX */

#ifdef ZB_LITE_FOR_GW
#define ZB_LITE_COORDINATOR_ONLY_ROLE
#define ZB_LITE_ALWAYS_SECURE
#define ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
#define ZB_LITE_NO_SOURCE_ROUTING
#define ZB_LITE_NO_NLME_ROUTE_DISCOVERY
//#define ZB_NO_NWK_MULTICAST
#define ZB_LITE_NO_JOIN_ZR_AS_ZED
#define ZB_LITE_NO_ORPHAN_SCAN
#define ZB_LITE_NO_STORE_APS_COUNTERS
//#define ZB_LITE_NO_FULL_FUNCLIONAL_MGMT_NWK_UPDATE
#define ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
#define ZB_LITE_NO_ZDO_MGMT_RTG
#define ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF

#ifdef ZB_SECURITY_INSTALLCODES
#undef ZB_SECURITY_INSTALLCODES
#endif

#ifdef ZB_CERTIFICATION_HACKS
#undef ZB_CERTIFICATION_HACKS
#endif

#ifdef ZB_TEST_PROFILE
#undef ZB_TEST_PROFILE
#endif

#ifndef ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/**
   Disable mgmt leave with requires sendint leave to third device.
 */
#define ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/* Need to store only one entry - about leave myself */
#ifdef ZB_ZDO_PENDING_LEAVE_SIZE
#undef ZB_ZDO_PENDING_LEAVE_SIZE
#define ZB_ZDO_PENDING_LEAVE_SIZE 1
#endif
#endif

#ifndef ZB_LITE_LIMIT_PIB_ACCESS
/**
   Limit access to PIB to values really used by the stack
 */
#define ZB_LITE_LIMIT_PIB_ACCESS
#endif

/**
   Disable end device bind
 */
#ifndef ZB_LITE_NO_END_DEVICE_BIND
#define ZB_LITE_NO_END_DEVICE_BIND
#endif

#endif  /* ZB_LITE_FOR_GW */

#ifdef ZB_LITE_COORDINATOR_ONLY_ROLE
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif

#if defined ZB_TRAFFIC_DUMP_OFF && defined ZB_TRAFFIC_DUMP_ON
#undef ZB_TRAFFIC_DUMP_ON
#endif

#ifndef ZB_NO_NWK_MULTICAST
/**
   Disable NWK multicast. Deprecated in R21.
   Use APS grops and NWK broadcast instead.
 */
#define ZB_NO_NWK_MULTICAST
#endif

#ifdef ZB_CONFIGURABLE_MEM

/*
  Huge feature if configuring RAM usage without ZBOSS library recompiling.
  Instead of normal static declarations use pointers.
  At init time set pointers to be used instead of static arrays used normally.
  Keep possibility to compile in a normal way; also keep code mergeable.
  Undefine here array size constants and define it to be variables. See zb_g_context.h for details.
*/
#ifdef ZB_IOBUF_POOL_SIZE
#undef ZB_IOBUF_POOL_SIZE
#endif
#ifdef ZB_SCHEDULER_Q_SIZE
#undef ZB_SCHEDULER_Q_SIZE
#endif
#ifdef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
#undef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
#endif
#ifdef ZB_IEEE_ADDR_TABLE_SIZE
#undef ZB_IEEE_ADDR_TABLE_SIZE
#endif
#ifdef ZB_NEIGHBOR_TABLE_SIZE
#undef ZB_NEIGHBOR_TABLE_SIZE
#endif
#ifdef ZB_NWK_ROUTING_TABLE_SIZE
#undef ZB_NWK_ROUTING_TABLE_SIZE
#endif
#ifdef ZB_APS_DUPS_TABLE_SIZE
#undef ZB_APS_DUPS_TABLE_SIZE
#endif

#endif  /* ZB_CONFIGURABLE_MEMORY */

/* pending bit source matching intended for ZB_ROUTER_ROLE only */
#ifndef ZB_ROUTER_ROLE
#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#undef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#endif
#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
#undef ZB_MAC_SOFTWARE_PB_MATCHING
#endif
#endif

#endif /* ZB_CONFIG_H */
