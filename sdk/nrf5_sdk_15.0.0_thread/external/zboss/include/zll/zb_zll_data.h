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
 * PURPOSE: ZLL data types
*/

#if ! defined ZB_ZLL_DATA_H
#define ZB_ZLL_DATA_H

#include "zll/zb_zll_common.h"
#include "zll/zb_zll_nwk_features.h"

/*!
    @addtogroup ZB_ZLL_COMMON
    @{
*/

/** @brief Access ZLL global context */
#define ZLL_CTX() (ZG->zll)

/** ZLL transaction states */
enum zb_zll_transaction_task_e
{
  ZB_ZLL_NO_TASK,                       /**< No specific task. */
  ZB_ZLL_DEVICE_START_TASK,             /**< Device startup. */
  ZB_ZLL_DEVICE_DISCOVERY_TASK,         /**< Device info gathering (device discovery routine). */
  ZB_ZLL_IDENTIFY_TASK,                 /**< Identify process requested/started. Initiator only */
  ZB_ZLL_TRANSACTION_NWK_START_TASK,    /**< Start network. Initiator only */
  ZB_ZLL_TRANSACTION_NWK_START_TASK_TGT,/**< Start network. Target only */
  ZB_ZLL_TRANSACTION_JOIN_ROUTER_TASK,  /**< Network join router process requested/started. */
  ZB_ZLL_TRANSACTION_JOIN_ROUTER_TASK_TGT, /**< Network join router process requested/started (target side) */
  ZB_ZLL_TRANSACTION_JOIN_ED_TASK,      /**< Network join end device requested/started. */
  ZB_ZLL_TRANSACTION_JOIN_ED_TASK_TGT, /**< Network join end device process requested/started (target side) */
  ZB_ZLL_TRANSACTION_RESET,            /**< Reset process requested  */
  ZB_ZLL_START_COMMISSIONING,           /**< Device start commissioning */
  ZB_ZLL_COMMISSIONED /**< Fake task id. It is used to inform user App on target side that device
                         commissioning is complete */
};

/*!
    @}

    @addtogroup ZB_ZLL_COMMON
    @internal
    @{
*/

/** @brief Access ZLL transaction context */
#define ZLL_TRAN_CTX() (ZLL_CTX().zll_tran_ctx)

/* TODO Provide normal battle-time implementation */
/** @brief Provides new response identifier. */
#define ZB_ZLL_GET_NEW_RESPONSE_ID() 0x01

/* TODO Provide normal battle-time implementation */
/** @brief Provides new inter-PAN transaction identifier. */
#define ZB_ZLL_GET_NEW_TRANS_ID() 0x01

/** @brief Get current inter-PAN ASDU handle value */
#define ZB_ZLL_GET_ASDU_HANDLE() (ZLL_TRAN_CTX.intrp_asdu_handle)

/** @brief Get new inter-PAN ASDU handle */
#define ZB_ZLL_GET_NEW_ASDU_HANDLE()  (++ZB_ZLL_GET_ASDU_HANDLE())

/** @brief Address range */
typedef struct zb_zll_addr_range_s
{
  zb_uint16_t addr_begin;
  zb_uint16_t addr_end;
}
zb_zll_addr_range_t;

/** @brief Group ID range */
typedef struct zb_zll_group_id_range_s
{
  zb_uint16_t group_id_begin;
  zb_uint16_t group_id_end;
}
zb_zll_group_id_range_t;

/** @brief Get address range size */
#define ZB_ZLL_ADDR_RANGE_SIZE(r) ((r).addr_end - (r).addr_begin)

/** @brief Get group ID range size */
#define ZB_ZLL_GROUP_RANGE_SIZE(r) ((r).group_id_end - (r).group_id_begin)

/** ZLL device data */
typedef struct zb_zll_device_info_s
{
  zb_uint8_t rssi_correction;
  zb_uint8_t zll_info;
  zb_uint16_t key_info;
  zb_uint8_t key_index;
  zb_zll_addr_range_t addr_range;
  zb_zll_group_id_range_t group_id_range;
  zb_uint8_t total_group_id_count; /* TODO: Not sure it is needed maybe calculate it using group ID
                                      count for EP? */
  /** @brief User-provided "report task result" callback. */
  zb_callback_t report_task_result;

  zb_uint8_t master_key[16];
  zb_uint8_t certification_key[16];
  zb_uint32_t transaction_id;
  zb_uint32_t response_id;
  zb_uint8_t encr_nwk_key[16];  /* FIXME: do we really need to store it? */

/* TODO Uncomment and remove a line after the comment
#if defined ZB_ZLL_ENABLE_COMMISSIONING_SERVER && defined ZB_ZLL_ENABLE_DEFAULT_IDENTIFY_PROCESSING
*/
  zb_uint16_t identify_duration;  /**< Identify duration period for ZLL commissioning server. */
  zb_callback_t identify_handler; /**< User-provided identify start/stop handler. */

  zb_uint8_t freqagility_channel;
  zb_uint8_t nwk_channel;       /* Logical channel number for start/join the network (as initiator
                                 * role) */
}
zb_zll_device_info_t;

/** @brief ZLL security key index values */
enum zb_zll_key_index_value_e
{
  ZB_ZLL_DEVELOPMENT_KEY_INDEX = 0,
  ZB_ZLL_MASTER_KEY_INDEX = 4,
  ZB_ZLL_CERTIFICATION_KEY_INDEX = 15
};

/** @brief ZLL security key index values */
enum zb_zll_key_value_e
{
  ZB_ZLL_DEVELOPMENT_KEY = 0x0001,
  ZB_ZLL_MASTER_KEY = 0x0010,
  ZB_ZLL_CERTIFICATION_KEY = 0x8000 //AT: fix (see: Table 67 Key encryption algorithms)
};

/** @brief Enumeration of touchlink actions, that should be approved
 * by end user application */
enum zb_zll_touchlink_action_e
{
  ZB_ZLL_ACTION_START_NEW_NETWORK,
  ZB_ZLL_ACTION_JOIN_ROUTER,
  ZB_ZLL_ACTION_JOIN_ED
};

/** @brief Access ZLL device information */
#define ZLL_DEVICE_INFO() (ZLL_CTX().zll_device_info)

/** Sets user identify start/stop handle callback. */
#define ZB_ZLL_REGISTER_IDENTIFYCALLBACK(callback) (ZLL_DEVICE_INFO().identify_handler = (callback))

/* ZLL network information */
#if OBSOLETE
/* Currently, network structure is not needed, all values are stored in NIB and PIB */
typedef ZB_PACKED_PRE struct zb_zll_network_ctx_s
{
  /* zb_uint8_t nwk_update_id; - ZB_NIB_UPDATE_ID() */
  /* zb_uint8_t logical_channel; - ZB_PIBCACHE_CURRENT_CHANNEL() */
  /* zb_ext_pan_id_t extended_pan_id; - ZB_NIB_EXT_PAN_ID() */
  /* zb_uint16_t short_pan_id; - ZB_PIBCACHE_PAN_ID()  */
  /*zb_uint8_t short_addr; -  ZB_PIBCACHE_NETWORK_ADDRESS() */
} ZB_PACKED_STRUCT zb_zll_network_ctx_t;
#endif

/** Sub-device information */
typedef struct zb_zll_sub_device_info_s
{
  zb_uint8_t ep_id;
  zb_uint16_t profile_id;
  zb_uint16_t device_id;
  zb_uint8_t version;
  zb_uint8_t group_id_count;
  zb_uint8_t dev_idx; /**< Index (in the device info table) of the device endpoint belongs to. */
} zb_zll_sub_device_info_t;

/** Discovered device information */
typedef struct zb_zll_ext_device_info_s
{
  /** @brief Our custom field to be able to address device information request. */
  zb_ieee_addr_t device_addr;
  zb_uint8_t rssi_correction;
  zb_uint8_t zb_info;
  zb_uint8_t zll_info;
  zb_uint16_t key_info;
  zb_uint32_t resp_id;
  zb_ext_pan_id_t ext_pan_id;
  zb_uint8_t nwk_update_id;
  zb_uint8_t channel_number;
  zb_uint16_t pan_id;
  zb_uint16_t nwk_addr;
  zb_uint8_t sub_device_count;
  zb_uint8_t total_group_id_count;
  zb_uint8_t ep_info_idx;
} zb_zll_ext_device_info_t;

/**
 *  @brief Callback type for checking requested action is allowed.
 *  @param action [IN] - action to check.
 *  @returns ZB_TRUE if action allowed, ZB_FALSE otherwise.
 */
typedef zb_uint8_t (*zb_zll_is_action_allowed_cb_t)(zb_uint8_t action);

/**
 *  @brief Structure to store data for start new network command.
 *  Stored in the context on the target side.
 */
typedef struct zb_zll_start_new_network_target_data_s
{
  zb_uint16_t seq_number;         /**< ZCL sequence number. */
  zb_uint16_t pan_id;             /**< Identifier of the PAN to be created. */
  zb_ext_pan_id_t ext_pan_id;     /**< Extended identifier of the PAN to be created. */
  zb_uint8_t channel;             /**< Channel to be used for PAN creation. */
  zb_uint16_t short_addr;         /**< New short addr */

  zb_ieee_addr_t target_ieee_addr;        /**< Target ieee addr */
} zb_zll_start_new_network_target_data_t;

typedef struct zb_zll_network_join_router_data_s
{
  zb_ieee_addr_t target_ieee_addr;        /**< Target ieee addr */
} zb_zll_network_join_router_data_t;

/**
 *  @brief Type of the ZLL command data storage.
 *  Stored in the context on the target side for to be used in asyncronous chains.
 */
typedef union zb_zll_command_target_data_u
{
  zb_zll_start_new_network_target_data_t start_new_nwk; /**< "Start new network" command data. */
  zb_zll_start_router_param_t start_router_param; /**< Parameters for start router command */
  zb_zll_rejon_nwk_ed_param_t rejoin_nwk_param; /**< Parameters for rejon nwk end device router
                                                 * command */
  zb_zll_network_join_router_data_t nwk_join_router; /**< Parameters for rejon nwk end device router command */
} zb_zll_command_target_data_t;

/**
 *  @brief Callback for case select/unselect device as Commissioning candidate.
 *  @param index - index of ZLL_TRAN_CTX().device_infos table
 *  @return zb_bool_t select/unselect device as Commissioning candidate
 */
typedef zb_bool_t (ZB_CODE * zb_zll_select_device_cb)(zb_uint8_t index);

#define ZB_ZLL_SET_SELECT_DEVICE_CB(cb)         ZLL_TRAN_CTX().select_device = (cb)


/** @brief ZLL transaction data context */
typedef struct zb_zll_transaction_ctx_s
{
  /** @brief Inter-PAN transaction identifier for the transaction under processing. */
  zb_uint32_t transaction_id;

  /** Specifies if identify request was sent during transaction */
  zb_bitfield_t identify_sent:1;
  /** "Send confirmed" flag. */
  zb_bitfield_t send_confirmed:1;
  /** "Scan timed out" flag. */
  zb_bitfield_t scan_timed_out:1;
  /** "Extended scan" flag. */
  zb_bitfield_t ext_scan:1;
  /** "Out of memory" flag. */
  zb_bitfield_t out_of_memory:1;

  /** Source device address received with the packet in the current transaction stage. */
  zb_ieee_addr_t src_addr;

  /**
   *  @brief Describe current transaction task. Possible tasks are defined in enumeration @ref
   *  zb_zll_transaction_task_e
   */
  zb_ushort_t transaction_task;

  zb_uint8_t n_device_infos;  /**< Number of device infos currently stored in the table. */
  zb_uint8_t current_dev_info_idx;  /**< Index for the device info record iterator. */
  /** Table of devices reported during the active transaction. */
  zb_zll_ext_device_info_t device_infos[ZB_ZLL_TRANS_CTX_DEV_INFO_MAX_NUMBER];

  /** Number of filled endpoint info records. */
  zb_uint8_t n_ep_infos;
  /** Sub-device (endpoint) info table. */
  zb_zll_sub_device_info_t ep_infos[ZB_ZLL_TRANS_CTX_EP_INFO_MAX_NUMBER];

  /** @brief User-provided "check action allowed" callback. */
  zb_zll_is_action_allowed_cb_t check_action_allowed;
  /** @internal @brief Packet send counter.
    *
    * Counts packets sent during particular transaction procedure.
    * @attention Should be neither referenced nor changed from the userspace.
    */
  zb_uint8_t pckt_cnt;
  /** Old address range for transaction rollback needs. */
  zb_zll_addr_range_t addr_range;
  /** Old group identifiers range for transaction rollback needs. */
  zb_zll_group_id_range_t group_id_range;
  /** Callback-chain data for target-side command handling. */
  zb_zll_command_target_data_t command_data;
  /** Callback for select device when search device by commissioning. */
  zb_zll_select_device_cb select_device;

} zb_zll_transaction_ctx_t;

/**
 *  @brief Looks up device information table in the transaction context and sets current device
 *  information record index according to lookup results.
 *  If appropriate device was not found, current device record index cwill be set to the number of
 *  the currently stored records.
 *  @param ieee_addr [IN] - @ref zb_ieee_addr_t "IEEE address" of the device to be found.
 *  @param status [OUT] - device lookup status. Takes value of RET_OK on success, and
 *  RET_ERROR if no device was found. See @ref zb_ret_t
 */
#define ZB_ZLL_FIND_DEVICE_INFO_BY_ADDR(ieee_addr, status)                               \
{                                                                                        \
  (status) = RET_ERROR;                                                                  \
  for ( ZLL_TRAN_CTX().current_dev_info_idx = 0;                                         \
        ZLL_TRAN_CTX().current_dev_info_idx < ZLL_TRAN_CTX().n_device_infos;             \
        ++ZLL_TRAN_CTX().current_dev_info_idx)                                           \
  {                                                                                      \
    if (ZB_IEEE_ADDR_CMP(                                                                \
          (ieee_addr),                                                                   \
          ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].device_addr)) \
    {                                                                                    \
      (status) = RET_OK;                                                                 \
      break;                                                                             \
    }                                                                                    \
  }                                                                                      \
}

/**
 *  @brief Reports number of group identifiers required by the current device in the transaction
 *  context.
 */
#define ZB_ZLL_TRANS_GET_CURRENT_DEV_GROUP_ID_RANGE_LEN()                                    \
  (   (ZLL_TRAN_CTX().current_dev_info_idx == ZLL_TRAN_CTX().n_device_infos)                 \
   ?  0                                                                                      \
   :  ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].total_group_id_count)

/* 01/26/2018 EE CR:MINOR You can simplify condition to return NULL using ||. */
#define ZB_ZLL_TRANS_GET_CURRENT_DEV_EP_INFO()                          \
  ( ( ZLL_TRAN_CTX().current_dev_info_idx > ZLL_TRAN_CTX().n_device_infos ) \
    ? NULL                                                              \
    : ( ( ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].ep_info_idx > ZLL_TRAN_CTX().n_ep_infos ) \
      ? NULL                                                            \
      : &(ZLL_TRAN_CTX().ep_infos[ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].ep_info_idx]) ) )

/**
 *  @brief Reports address assign capability of the current device selected in the transaction
 *  context.
 *  @returns @ref ZB_FALSE if device has no address assign capabilities, or no device selected
 *  either, and @ref ZB_TRUE otherwise.
 */
#define ZB_ZLL_TRANS_GET_CURRENT_DEV_ADDR_ASSIGN_CAPABILITY()                    \
  ((   ZLL_TRAN_CTX().device_infos[ZLL_TRAN_CTX().current_dev_info_idx].zll_info \
    &  ZB_ZLL_INFO_ADDR_ASSIGNMENT) != 0)

/**
 *  @brief Sets "report task result" callback.
 *  @param callback [IN] - @ref zb_callback_t "callback" to set.
 */
#define ZB_ZLL_REGISTER_COMMISSIONING_CB(callback)  \
  (ZLL_DEVICE_INFO().report_task_result = (callback))

/**
 *  @brief Sets "check action allowed" callback.
 *  @param callback [IN] - @ref zb_zll_is_action_allowed_cb_t "callback" to set.
 */
#define ZB_ZLL_REGISTER_ACTION_CHECK_CB(callback) (ZLL_TRAN_CTX().check_action_allowed = (callback))

/** Special transaction identifier value: no transaction. */
#define ZLL_NO_TRANSACTION_ID ((zb_uint32_t)0)

/** @brief Zigbee information device type subfield values */
enum zb_zll_zigbee_info_e
{
  ZB_ZLL_ZB_INFO_COORD_DEVICE_TYPE = 0x00,
  ZB_ZLL_ZB_INFO_ROUTER_DEVICE_TYPE = 0x01,
  ZB_ZLL_ZB_INFO_ED_DEVICE_TYPE = 0x02,
};

/** @brief Bit mask for ZigbeeInfo.DeviceType 2-bit subfield extraction. */
#define ZB_ZLL_ZB_INFO_RX_ON_WHEN_IDLE_MASK 0x4

/** @brief Bit mask for ZigbeeInfo.RxOnWhenIdle 1-bit subfield extraction. */
#define ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK 0x03

/* TODO Do we really need following 4 macros?
 * Yes, at last ZB_ZLL_ZB_INFO_GET_DEVICE_TYPE */
/** @brief Set ZigbeeInfo.DeviceType subfield.
  * @attention Assumes that dst was pre-zeroed.
  */
#define ZB_ZLL_ZB_INFO_SET_DEVICE_TYPE(dst, value) \
  ((dst) |= ((value) & ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK))

/** @brief Get ZigbeeInfo.DeviceType subfield.
  */
#define ZB_ZLL_ZB_INFO_GET_DEVICE_TYPE(zb_info) \
  ((zb_info)  & ZB_ZLL_ZB_INFO_DEVICE_TYPE_MASK)

/** @brief Set ZigbeeInfo.RxOnWhenIdle subfield.
  * @attention Assumes that dst was pre-zeroed.
  */
#define ZB_ZLL_ZB_INFO_SET_RX_ON_WHEN_IDLE(dst, value) \
    ((dst) |= ((value) & ZB_ZLL_ZB_INFO_RX_ON_WHEN_IDLE_MASK))

#define ZB_ZLL_ZB_INFO_GET_RX_ON_WHEN_IDLE(zb_info)              \
  (((zb_info) & ZB_ZLL_COMMISSIONING_SCAN_REQ_RX_ON_WHEN_IDLE_MASK) >> 2)

/** ZLL information bit values */
enum zb_zll_info_e
{
  ZB_ZLL_INFO_FACTORY_NEW = 0x01,
  ZB_ZLL_INFO_ADDR_ASSIGNMENT = 0x02,
  ZB_ZLL_INFO_TOUCHLINK_INITIATOR = 0x10,
  ZB_ZLL_INFO_TOUCHLINK_PRIOR = 0x20
};

/** @brief ZLL context data */
typedef struct zb_zll_ctx_s
{
  zb_zll_device_info_t zll_device_info;
  zb_zll_transaction_ctx_t zll_tran_ctx;
} zb_zll_ctx_t;

/**
 * @}
 */

#endif /* ! defined ZB_ZLL_DATA_H */
