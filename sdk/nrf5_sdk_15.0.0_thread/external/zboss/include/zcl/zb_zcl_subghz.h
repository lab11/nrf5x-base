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
 * PURPOSE: Sub-GHz cluster definitions
*/
#ifndef ZB_ZCL_SUBGHZ_H
#define ZB_ZCL_SUBGHZ_H

#if defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ || defined DOXYGEN

#include "zboss_api_core.h" /* ZBOSS core API types */
#include "zb_zcl_common.h"

/** @cond (DOXYGEN_ZCL_SECTION && DOXYGEN_SUBGIG_SECTION) */
/** @addtogroup ZB_ZCL_SUBGHZ
 *  @{
 *    @details This cluster provides attributes and commands specific to the use of Sub-GHz frequencies
 *      for a Smart Energy network.
 */

/** @defgroup ZB_ZCL_SUBGHZ_ATTRS Sub-GHz cluster attributes
 * @{
 */

/** @brief Sub-GHz cluster's server-side attributes
 *  @see SE spec, subclause 14.2.2
 */
typedef enum zb_zcl_subghz_srv_attr_e
{
  ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE      = 0x0000,     /**< This is a 32-bit channel mask that defines
                                                        * the sub-GHz channel that the Coordinator’s
                                                        * Network Manager intends to move to.
                                                        * @par Value format
                                                        * bits 0..26 - channel number
                                                        * bits 27..31 - channel page number
                                                        */

  ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK = 0x0001,     /**< This is a 32-bit channel mask that defines
                                                        * the channels that are to be scanned when
                                                        * forming, joining or re-joining a network.
                                                        * Page 28 defines the first 27 channels within
                                                        * the 863-876MHz frequency band.
                                                        * @par Value format
                                                        * bits 0..26 - channel numbers bitmask
                                                        * bits 27..31 - page number (28 == 0b11100)
                                                        */

  ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK = 0x0002,     /**< This is a 32-bit channel mask that defines
                                                        * the channels that are to be scanned when
                                                        * forming, joining or re-joining a network.
                                                        * Page 29 defines channels 27 to 34 and channel
                                                        * 62 of the 863-876MHz frequency band.
                                                        * @par Value format
                                                        * bits 0..8 - channel numbers bitmask
                                                        * bits 9..26 - unused (set to 0)
                                                        * bits 27..31 - page number (29 == 0b11101)
                                                        */

  ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK = 0x0003,     /**< This is a 32-bit channel mask that defines
                                                        * the channels that are to be scanned when
                                                        * forming, joining or re-joining a network.
                                                        * Page 30 defines channels 35 to 61 of the
                                                        * 863-876MHz frequency band.
                                                        * @par Value format
                                                        * bits 0..26 - channel numbers bitmask
                                                        * bits 27..31 - page number (30 == 0b11110)
                                                        */

  ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK = 0x0004      /**< This is a 32-bit channel mask that defines
                                                        * the channels that are to be scanned when
                                                        * forming, joining or re-joining a network.
                                                        * Page 31 defines the 27 channels within the
                                                        * 915-921MHz frequency band.
                                                        * @par Value format
                                                        * bits 0..26 - channel numbers bitmask
                                                        * bits 27..31 - page numbers (31 == 0b11111)
                                                        */
} zb_zcl_subghz_svr_attr_t;


#ifndef ZB_ED_ROLE

/** @cond internals_doc */

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE(data_ptr)          \
{                                                                                   \
  ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE,                                                \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK(data_ptr)     \
{                                                                                   \
  ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK,                                           \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK(data_ptr)     \
{                                                                                   \
  ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK,                                           \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK(data_ptr)     \
{                                                                                   \
  ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK,                                           \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK(data_ptr)     \
{                                                                                   \
  ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK,                                           \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

/** @def ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST(attr_list, channel_change)
 *  @brief Declares mandatory attributes list for Sub-GHz cluster server
 *  @param[in]  attr_list - attribute list name
 *  @param[in]  channel_change - pointer to variable which will store ChannelChange attribute value
 */
#define ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST_PARTIAL(attr_list, channel_change)              \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE, (channel_change))         \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST


/** @def ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST_FULL(attr_list, channel_change, page28_channel_mask, page29_channel_mask, page30_channel_mask, page31_channel_mask)
 *  @brief Declares mandatory and all optional attributes list for Sub-GHz cluster server
 *  @param[in]  attr_list - attribute list name
 *  @param[in]  channel_change - pointer to variable which will store ChannelChange attribute value
 *  @param[in]  page28_channel_mask - pointer to variable which will store Page28ChannelMask attribute value
 *  @param[in]  page29_channel_mask - pointer to variable which will store Page29ChannelMask attribute value
 *  @param[in]  page30_channel_mask - pointer to variable which will store Page30ChannelMask attribute value
 *  @param[in]  page31_channel_mask - pointer to variable which will store Page31ChannelMask attribute value
 */
#define ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST_FULL(attr_list, channel_change, page28_channel_mask, page29_channel_mask, page30_channel_mask, page31_channel_mask)        \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE, (channel_change))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK, (page28_channel_mask))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK, (page29_channel_mask))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK, (page30_channel_mask))         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK, (page31_channel_mask))         \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @endcond */ /* internals_doc */


/**
 *  @brief Sub GHz cluster attributes
 */
typedef struct zb_zcl_sub_ghz_attrs_s
{

  /** @copydoc ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE
   * @see ZB_ZCL_ATTR_SUBGHZ_CHANNEL_CHANGE
   */
  zb_uint32_t channel_change;

  /** @copydoc ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK
   * @see ZB_ZCL_ATTR_SUBGHZ_PAGE28_CHANNEL_MASK
   */
  zb_uint32_t page28_channel_mask;

  /** @copydoc ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK
   * @see ZB_ZCL_ATTR_SUBGHZ_PAGE29_CHANNEL_MASK
   */
  zb_uint32_t page29_channel_mask;

  /** @copydoc ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK
   * @see ZB_ZCL_ATTR_SUBGHZ_PAGE30_CHANNEL_MASK
   */
  zb_uint32_t page30_channel_mask;

  /** @copydoc ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK
   * @see ZB_ZCL_ATTR_SUBGHZ_PAGE31_CHANNEL_MASK
   */
  zb_uint32_t page31_channel_mask;
}
zb_zcl_sub_ghz_attrs_t;


/** Declare attribute list for Sub-GHz cluster
 * @param[in]  attr_list - attribute list variable name
 * @param[in]  attrs - pointer to @ref zb_zcl_sub_ghz_attrs_s structure
 */
#define ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST(attr_list, attrs) \
  ZB_ZCL_DECLARE_SUBGHZ_SRV_ATTR_LIST_FULL(attr_list, &attrs.channel_change, &attrs.page28_channel_mask, &attrs.page29_channel_mask, &attrs.page30_channel_mask, &attrs.page31_channel_mask)

#endif /* !ZB_ED_ROLE */

/** @} */ /* ZB_ZCL_SUBGHZ_ATTRS */

/** @cond (DOXYGEN_ZCL_SECTION && DOXYGEN_SUBGIG_SECTION) */
/** @defgroup ZB_ZCL_SUBGHZ_COMMANDS Sub-GHz cluster commands
 *  @{
 */

/** @brief Sub-GHz cluster's server-side commands IDs
 *  @see SE spec, subclause 14.2.3
 */
typedef enum zb_zcl_subghz_srv_cmd_e
{
  ZB_ZCL_SUBGHZ_SRV_CMD_SUSPEND_ZCL_MESSAGES = 0x00   /**< The @b SuspendZCLMessages command is sent to
                                                       * client device(s) by the server device when the
                                                       * server device has determined that the client
                                                       * device(s) shall suspend their ZCL communications
                                                       * to the server device for the period stated in
                                                       * the command. The command is also sent in response
                                                       * to a @b GetSuspendZCLMessagesStatus command.
                                                       * @note This command is sent automatically by SubGHz
                                                       *    cluster and is not available in the application.
                                                       */
} zb_zcl_subghz_srv_cmd_t;


/** @brief @ref ZB_ZCL_SUBGHZ_SRV_CMD_SUSPEND_ZCL_MESSAGES "SuspendZCLMessages" command payload
 *  @details An unsigned 8-bit integer indicating the period, in minutes, during which ZCL
 *    communications from the device shall be suspended. A value of zero shall indicate that ZCL
 *    communications are not currently suspended.
 *  @see SE spec, subclause 14.2.3.1
 */
typedef zb_uint8_t zb_zcl_subghz_suspend_zcl_msg_payload_t;


/** @brief Sub-GHz cluster's cliend-side commands IDs
 *  @see SE spec, subclause 14.3.3
 */
typedef enum zb_zcl_subghz_cli_cmd_e
{
  ZB_ZCL_SUBGHZ_CLI_CMD_GET_SUSPEND_ZCL_MESSAGES_STATUS = 0x00  /**< The @b GetSuspendZCLMessagesStatus
                                                                 * command allows a client device to
                                                                 * request the current status of its
                                                                 * ZCL communications with the server.
                                                                 * @note This command is Mandatory for BOMDs.
                                                                 */
} zb_zcl_subghz_cli_cmd_t;


/** @cond internals_doc */

#ifdef ZB_ED_ROLE

/** @fn zb_void_t zb_subghz_cli_get_suspend_zcl_messages_status(zb_uint8_t param, zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep, zb_uint8_t src_ep)
 *  @brief Sends @b GetSuspendZCLMessagesStatus client command
 *  @param[in]  param - reference to the buffer to be used for outgoing packet
 *  @param[in]  dst_addr - Coordinator's address (either short or long)
 *  @param[in]  dst_addr_mode - address mode (see @ref zb_aps_addr_mode_e)
 *  @param[in]  dst_ep - endpoint address of Sub-GHz cluster on Coordinator
 *  @param[in]  src_ep - endpoint address of Sub-GHz cluster on this device
 */
zb_void_t zb_subghz_cli_get_suspend_zcl_messages_status(zb_uint8_t param,
                                                        zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode,
                                                        zb_uint8_t dst_ep,
                                                        zb_uint8_t src_ep);

#else

/** @fn zb_void_t zb_subghz_srv_suspend_zcl_messages(zb_uint8_t param, zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep, zb_uint8_t src_ep, zb_zcl_subghz_suspend_zcl_msg_payload_t *payload)
 *  @brief Sends @b SuspendZCLMessages server command
 */
zb_void_t zb_subghz_srv_suspend_zcl_messages(zb_uint8_t param,
                                             zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode,
                                             zb_uint8_t dst_ep,
                                             zb_uint8_t src_ep,
                                             zb_zcl_subghz_suspend_zcl_msg_payload_t *payload);


zb_bool_t zb_subghz_srv_device_suspended(zb_uint16_t addr);

#endif /* ZB_ED_ROLE */


/** @fn zb_bool_t zb_zcl_process_subghz_specific_commands(zb_uint8_t param)
 *  @brief Internal handler for Sub-GHz specific commands
 *  @param[in]  param - reference to the buffer containing incoming command
 *  @returns ZB_TRUE, if command was processed by Sub-GHz cluster, ZB_FALSE otherwise
 */
zb_bool_t zb_zcl_process_subghz_specific_commands(zb_uint8_t param);


/** @fn zb_void_t zb_subghz_init()
 *  @brief Cluster context initialization
 */
zb_void_t zb_subghz_init();


/**
   Start (or continue) polling ZC by Get Suspend ZCL Message Status
 */
void zb_subghz_start_suspend_status_poll(zb_uint8_t param);

zb_bool_t zb_subghz_catch_ota_image_block_req(zb_uint8_t *paramp);

/** @endcond */ /* internals_doc */

/** @} */ /* ZB_ZCL_SUBGHZ_COMMANDS */
/** @endcond */ /* DOXYGEN_ZCL_SECTION && DOXYGEN_SUBGIG_SECTION */
/** @} */ /* ZB_ZCL_SUBGHZ */

/** @endcond */ /* DOXYGEN_ZCL_SECTION && DOXYGEN_SUBGIG_SECTION */

#endif /* ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ || defined DOXYGEN */

zb_void_t zb_zcl_subghz_init_server();
zb_void_t zb_zcl_subghz_init_client();
#define ZB_ZCL_CLUSTER_ID_SUB_GHZ_SERVER_ROLE_INIT zb_zcl_subghz_init_server
#define ZB_ZCL_CLUSTER_ID_SUB_GHZ_CLIENT_ROLE_INIT zb_zcl_subghz_init_client

#endif /* ZB_ZCL_SUBGHZ_H */
