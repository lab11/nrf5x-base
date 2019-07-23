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
 PURPOSE: Vendor configuration for Nordic nRF52840
*/

/*

 NOTE: This file configures behavior and some size constants when
 compiling ZBOSS libraries. If some definition changed here, ZBOSS
 libraries must be recompiled. Changing this file contents without
 recompiling ZBOSS libraries causes memory structures inconsistence.

 DO NOT CHANGE THAT FILE INCLUDED INTO BINARY SDK BUILD!
*/
#ifndef ZB_VENDOR_CFG_NRF52840_NORDIC_SDK_H
#define ZB_VENDOR_CFG_NRF52840_NORDIC_SDK_H 1

/**
   NRF52840 radio from Nordic
*/

#define ZB_CONFIG_NRF52840

#define ZB_PLATFORM_NRF52840

#define ZB_BDB_MODE
#define ZB_DISTRIBUTED_SECURITY_ON

#define ZB_NO_SYSTEST_SUPPORT

#define ZB_ALL_DEVICE_SUPPORT

#define ZB_REJOIN_BACKOFF

#define ZB_ENABLE_ZGP

//#define ZB_CHECK_OOM_STATUS
//#define ZB_SEND_OOM_STATUS

#define ZB_PROMISCUOUS_MODE
#define ZB_USE_OSIF_OTA_ROUTINES
#define ZB_PRODUCTION_CONFIG
#define ZB_SECURITY_INSTALLCODES

#define ZB_BDB_TOUCHLINK

#define ZB_DONT_NEED_TRACE_FILE_ID

/* Use NRF_LOG subsystem for ZBOSS trace.
   If disabled, ZBOSS will init and use UART for binary trace. */
#define ZB_NRF_TRACE

/* Use NRF fstorage for NVRAM operations. */
#define ZB_NRF_USE_FSTORAGE

/* Let device select flash memory address based on configuration. */
#define ZB_NVRAM_FLASH_AUTO_ADDRESS

/* Compile-time memory configuration: hard-coded parameters.

   DO NOT CHANGE IT WHEN COMPILING APPLICATIONS USING PRE-COMPILED ZBOSS LIBRARIES!
 */
#define ZB_PANID_TABLE_SIZE 32
#define ZB_DEV_MANUFACTORER_TABLE_SIZE 32
#define ZB_BUF_Q_SIZE 8
#define ZDO_TRAN_TABLE_SIZE 16
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 8
#define ZB_NWK_BTR_TABLE_SIZE 16
#define ZB_NWK_BRR_TABLE_SIZE 16
#define ZB_MAX_EP_NUMBER 6
#ifndef ZB_ED_ROLE
#define ZB_APS_SRC_BINDING_TABLE_SIZE 16
#define ZB_APS_DST_BINDING_TABLE_SIZE 32
#else
#define ZB_APS_SRC_BINDING_TABLE_SIZE 8
#define ZB_APS_DST_BINDING_TABLE_SIZE 16
#endif  /* ZB_ED_ROLE */
#ifndef ZB_ED_ROLE
#define ZB_APS_GROUP_TABLE_SIZE 16
#else
#define ZB_APS_GROUP_TABLE_SIZE 8
#endif
#define ZB_ZGP_SINK_TBL_SIZE 24
#define ZB_ZGP_PROXY_TBL_SIZE 4


#define ZB_CONFIGURABLE_MEM

#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

/* Default memory storage configuration - to be used if user does not include any of zb_mem_config_xxx.h */

#ifndef ZB_ED_ROLE
/* Note that by dafault ZC/ZR ZBOSS library uses ZC memory
 * configuration which is biffer than for ZR (more space for TCLK) */
#define ZB_CONFIG_ROLE_ZC
#else
/* If this is ZED-only ZBOSS library, let's use ZED-specific memory configuration. */
#define ZB_CONFIG_ROLE_ZED
#endif

#define ZB_CONFIG_OVERALL_NETWORK_SIZE 128

#define ZB_CONFIG_HIGH_TRAFFIC

#define ZB_CONFIG_APPLICATION_COMPLEX

#endif  /*  ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

#define ZB_ENABLE_SE_CLUSTERS

#endif /* ZB_VENDOR_CFG_NRF52840_NORDIC_SDK_H */
