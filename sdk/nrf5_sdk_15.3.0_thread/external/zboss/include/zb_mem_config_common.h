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
 PURPOSE: Common rules for ZBOSS application-sude memory configuring

That file is to be included from zb_mem_config_xxxx.h after basic
selectors defined.

Do not include that file directly into the application source!
*/
#ifndef ZB_MEM_CONFIG_COMMON_H
#define ZB_MEM_CONFIG_COMMON_H 1

/*
  That file is useful only for ZBOSS buld with memory configurable by the user without ZBOSS lib recompile.
 */
#ifdef ZB_CONFIGURABLE_MEM

#ifdef ZB_ED_ROLE
/* If ZBOSS library is compiled for ZED only, force ZED config role. */
#ifndef ZB_CONFIG_ROLE_ZED
#define ZB_CONFIG_ROLE_ZED
#endif
#ifdef ZB_CONFIG_ROLE_ZC
#undef ZB_CONFIG_ROLE_ZC
#endif
#ifdef ZB_CONFIG_ROLE_ZR
#undef ZB_CONFIG_ROLE_ZR
#endif
#endif

/*
  Rules of deriving parameters from generic settings.

## device role ZC/ZR/ZED
### Parameters depending on it:
ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE: set 4 for ZR / OVERALL_NETWORK_SIZE for ZC

## Overall Zigbee network size:
OVERALL_NETWORK_SIZE 16 to 200

### Parameters depending on it:
- ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE: set = OVERALL_NETWORK_SIZE
- ZB_IEEE_ADDR_TABLE_SIZE - set = OVERALL_NETWORK_SIZE + reserve for redirected entries
- ZB_NEIGHBOR_TABLE_SIZE - set = ZB_IEEE_ADDR_TABLE_SIZE

## Total network traffic
HIGH_TRAFFIC / MODERATE_TRAFFIC / LIGHT_TRAFFIC

### Parameters depending on it:
- ZB_IOBUF_POOL_SIZE: 18 LITE / 32 MODERATE / 48 HIGH
- ZB_NWK_ROUTING_TABLE_SIZE ZB_NEIGHBOR_TABLE_SIZE if MODERATE or HIGH, 8 if LITE
- ZB_MAC_PENDING_QUEUE_SIZE (ZB_IOBUF_POOL_SIZE / 4)
- ZB_APS_BIND_TRANS_TABLE_SIZE function of ZB_IOBUF_POOL_SIZE

## Complexity of application relationships
APPLICATION_SIMPLE / APPLICATION_MODERATE / APPLICATION_COMPLEX

### Parameters depending on it:
- ZB_IOBUF_POOL_SIZE: same constants as for net traffic; use bigger one if have a choice
- ZB_N_APS_RETRANS_ENTRIES: derived from ZB_IOBUF_POOL_SIZE
- ZB_APS_DUPS_TABLE_SIZE: 8 SIMPLE / 16 MODERATE / 32 COMPLEX
- ZB_SCHEDULER_Q_SIZE 20 SIMPLE / 32 MODERATE / 48 COMPLEX
 */

/*
  Total network size: set at upperl level, just a verification here.
 */
#ifndef ZB_CONFIG_OVERALL_NETWORK_SIZE
#error Define ZB_CONFIG_OVERALL_NETWORK_SIZE!
#elif ZB_CONFIG_OVERALL_NETWORK_SIZE < 2 || ZB_CONFIG_OVERALL_NETWORK_SIZE > 200
#error ZB_CONFIG_OVERALL_NETWORK_SIZE must be between 2 and 200!
#else
/* Derive constands from the network size */

#endif  /* ZB_CONFIG_OVERALL_NETWORK_SIZE */

 /* Reserve in address translation table for aliases: 1/4, at least
  * 4. Table size must be < 255, so let's limit net size to 200 */
#define ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(n) ((n + 15)/16 * 4)

/*
  Device role
*/
#ifdef ZB_CONFIG_ROLE_ZC

#if defined ZB_CONFIG_ROLE_ZR || defined ZB_CONFIG_ROLE_ZED
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

/* ZC need to store one TCLK per device in the overall network */
#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE
/* Address table for entire network + reserve */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE + ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(ZB_CONFIG_OVERALL_NETWORK_SIZE))
/* Let's have enough space to have the entire network in neighbors - Star topology */
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE

#elif defined ZB_CONFIG_ROLE_ZR

#if defined ZB_CONFIG_ROLE_ZC || defined ZB_CONFIG_ROLE_ZED
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

/* Only own keys need to be stored */
#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE 4
/* The same as ZC: let's be able to work in Star. */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE (ZB_CONFIG_OVERALL_NETWORK_SIZE + ZB_IEEE_ADDR_TABLE_SIZE_RESERVE(ZB_CONFIG_OVERALL_NETWORK_SIZE))
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE

#elif defined ZB_CONFIG_ROLE_ZED

#if defined ZB_CONFIG_ROLE_ZC || defined ZB_CONFIG_ROLE_ZR
#error Only one ZB_CONFIG_ROLE_xxx can be defined!
#endif

#define ZB_CONFIG_N_APS_KEY_PAIR_ARR_MAX_SIZE 2
/* Set it here big enough; may decrease it later */
#define ZB_CONFIG_IEEE_ADDR_TABLE_SIZE ZB_CONFIG_OVERALL_NETWORK_SIZE

/* ZED needs neighbor table at join time only. More devices and nets around - biger nbt required. Let's use some euristics. */
#if ZB_CONFIG_OVERALL_NETWORK_SIZE < 8
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 8
#elif ZB_CONFIG_OVERALL_NETWORK_SIZE < 32
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 16
#else
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 32
#endif

#else

#error Define exactly one ZB_CONFIG_ROLE_xxx!

#endif  /* ZB_CONFIG_ROLE_ZC */


/*
  Total network traffic (including NWK routing)
 */
#ifdef ZB_CONFIG_HIGH_TRAFFIC

#if defined ZB_CONFIG_MODERATE_TRAFFIC || defined ZB_CONFIG_LIGHT_TRAFFIC
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

/* More NWK traffic we route or send/recv from our app - more packet buffers required. */
#define ZB_CONFIG_IOBUF_POOL_SIZE 48
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE ZB_CONFIG_NEIGHBOR_TABLE_SIZE
#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE (ZB_CONFIG_IOBUF_POOL_SIZE / 4)
#define ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE ((ZB_CONFIG_IOBUF_POOL_SIZE + 15)/16 * 4) /* 1/4, at least 4 */
#define ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE ((ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE + 7) / 8)
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 32
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 32

#elif defined ZB_CONFIG_MODERATE_TRAFFIC

#if defined ZB_CONFIG_HIGH_TRAFFIC || defined ZB_CONFIG_LIGHT_TRAFFIC
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

#define ZB_CONFIG_IOBUF_POOL_SIZE 32
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE ZB_CONFIG_NEIGHBOR_TABLE_SIZE
#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE (ZB_CONFIG_IOBUF_POOL_SIZE / 4)
#define ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE ((ZB_CONFIG_IOBUF_POOL_SIZE + 15)/16 * 4) /* 1/4, at least 4 */
#define ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE ((ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE + 7) / 8)
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 24
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 24

#elif defined ZB_CONFIG_LIGHT_TRAFFIC

#if defined ZB_CONFIG_HIGH_TRAFFIC || defined ZB_CONFIG_MODERATE_TRAFFIC
#error Only one ZB_CONFIG_xxx_TRAFFIC can be defined!
#endif

#define ZB_CONFIG_IOBUF_POOL_SIZE 20
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE 8
#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE (ZB_CONFIG_IOBUF_POOL_SIZE / 4)
#define ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE ((ZB_CONFIG_IOBUF_POOL_SIZE + 15)/16 * 4) /* 1/4, at least 4 */
#define ZB_CONFIG_SINGLE_TRANS_INDEX_SIZE ((ZB_CONFIG_APS_BIND_TRANS_TABLE_SIZE + 7) / 8)
#define ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE 16
#define ZB_CONFIG_APS_DST_BINDING_TABLE_SIZE 16

#else

#error Define exactly one ZB_CONFIG_xxx_TRAFFIC!

#endif  /* ZB_CONFIG_HIGH_TRAFFIC */

/* check that 5 bits of src_table_index is enough */
ZB_ASSERT_COMPILE_DECL(ZB_CONFIG_APS_SRC_BINDING_TABLE_SIZE <= (1<<5));

/*
  Complexity of the application interconnection to other Zigbee devices.
 */
#ifdef ZB_CONFIG_APPLICATION_COMPLEX

#if defined ZB_CONFIG_APPLICATION_MODERATE || defined ZB_CONFIG_APPLICATION_SIMPLE
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#define ZB_CONFIG_SCHEDULER_Q_SIZE 48

/* Increase pool and neighbor table sizes of complex application is supposed. */

#if ZB_CONFIG_IOBUF_POOL_SIZE < 32
#undef ZB_CONFIG_IOBUF_POOL_SIZE
#define ZB_CONFIG_IOBUF_POOL_SIZE 32
#endif

#if ZB_CONFIG_NEIGHBOR_TABLE_SIZE < 16
#undef ZB_CONFIG_NEIGHBOR_TABLE_SIZE
#define ZB_CONFIG_NEIGHBOR_TABLE_SIZE 16
#endif

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 32
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES ((ZB_CONFIG_IOBUF_POOL_SIZE + 8)/9 * 3) /* 1/3, at least 3 */

#elif defined ZB_CONFIG_APPLICATION_MODERATE

#if defined ZB_CONFIG_APPLICATION_COMPLEX || defined ZB_CONFIG_APPLICATION_SIMPLE
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#ifdef ZB_CONFIG_HIGH_TRAFFIC
#define ZB_CONFIG_SCHEDULER_Q_SIZE 48
#else
#define ZB_CONFIG_SCHEDULER_Q_SIZE 32
#endif

#if ZB_CONFIG_IOBUF_POOL_SIZE < 24
#undef ZB_CONFIG_IOBUF_POOL_SIZE
#define ZB_CONFIG_IOBUF_POOL_SIZE 24
#endif

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 16
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES ((ZB_CONFIG_IOBUF_POOL_SIZE + 8)/9 * 3) /* 1/3, at least 3 */

#elif defined ZB_CONFIG_APPLICATION_SIMPLE

#if defined ZB_CONFIG_APPLICATION_MODERATE || defined ZB_CONFIG_APPLICATION_COMPLEX
#error Only one ZB_CONFIG_APPLICATION_xxx can be defined!
#endif

#define ZB_CONFIG_SCHEDULER_Q_SIZE 20

#define ZB_CONFIG_APS_DUPS_TABLE_SIZE 8
#define ZB_CONFIG_N_APS_RETRANS_ENTRIES 6

#else

#error Define exactly one ZB_CONFIG_APPLICATION_xxx!

#endif  /* ZB_CONFIG_APPLICATION_COMPLEX */

#ifdef ZB_CONFIG_ROLE_ZED
/* That parameters will not be used in ZED, but just in case - let
 * compiler fail if routing parameter used by mistake. */
#undef ZB_CONFIG_NWK_ROUTING_TABLE_SIZE
#undef ZB_CONFIG_MAC_PENDING_QUEUE_SIZE
#define ZB_CONFIG_NWK_ROUTING_TABLE_SIZE 0
#define ZB_CONFIG_MAC_PENDING_QUEUE_SIZE 0
#endif

#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
/* Child heighbor hash table size equals to size of addr table + 33% aligned to 32 */
#define ZB_CONFIG_CHILD_HASH_TABLE_SIZE (((ZB_CONFIG_IEEE_ADDR_TABLE_SIZE + ZB_CONFIG_IEEE_ADDR_TABLE_SIZE / 3) + 31) / 32 * 32)

/* Pending bitmap size. Each bit corresponds to "child_hash_table" */
#define ZB_CONFIG_PENDING_BITMAP_SIZE ((ZB_CONFIG_CHILD_HASH_TABLE_SIZE + 31) / 32)
#endif

#endif  /* ZB_CONFIGURABLE_MEM */

#endif /* ZB_MEM_CONFIG_COMMON_H */
