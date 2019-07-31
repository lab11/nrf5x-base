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
 * PURPOSE: common definitions for ZGP profile
*/

#ifndef ZBOSS_API_ZGP_H
#define ZBOSS_API_ZGP_H 1

#include "zb_address.h"

/** @cond internals_doc */
extern zb_uint8_t g_zgp_enabled;
/** @endcond */ /* internals_doc */

/**
   Switch off ZGP at runtime
 */
#define ZB_ZGP_DISABLE() (g_zgp_enabled = 0)
/**
   Switch on ZGP at runtime
 */
#define ZB_ZGP_ENABLE() (g_zgp_enabled = 1)

/********************************************************************/
/********* Type declarations specific to ZGP profile ****************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/

#define ZB_ZGP_MAX_GPDF_CMD_PAYLOAD_APP_ID_0000 59
#define ZB_ZGP_MAX_GPDF_CMD_PAYLOAD_APP_ID_0010 54

/** @brief ZGPD application ID */
typedef enum zb_zgp_app_id_e
{
  ZB_ZGP_APP_ID_0000    = 0x00, /**< ApplicationID value 0b000 - usage of the SrcID */
  ZB_ZGP_APP_ID_0010    = 0x02, /**< ApplicationID value 0b010 - usage of the GPD IEEE address.*/
  ZB_ZGP_APP_ID_INVALID = 0x07, /**< Invalid ApplicationID */
}
zb_zgp_app_id_t;

/**
 * @brief ZGPD address
 *
 * ZGPD is identified in network whether by SrcId or its IEEE address. */
typedef ZB_PACKED_PRE union zb_zgpd_addr_u
{
  zb_uint32_t     src_id;    /**< ZGPD SrcId @see ZGP spec, A.1.4.1.4 */
  zb_ieee_addr_t  ieee_addr; /**< ZGPD IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_addr_t;

enum zb_zgp_endpoint_e
{
  ZB_ZGP_COMMUNICATION_ENDPOINT,
  ZB_ZGP_ALL_ENDPOINTS = 255
};

/**
 * @brief ZGPD identification info (ZGPD ID)
 *
 * ZGPD is identified by SrcId if ApplicationID is @ref ZB_ZGP_APP_ID_0000.
 * Otherwise, ZGPD is identified by its IEEE address. */
typedef ZB_PACKED_PRE struct zb_zgpd_id_s
{
  zb_uint8_t         app_id;  /**< One of the @ref zb_zgp_app_id_t values */
  zb_uint8_t         endpoint;/**< Identifier of the GPD endpoint, which jointly with the GPD IEEE address identifies a unique logical GPD device.*/
  zb_zgpd_addr_t     addr;    /**< ZGPD SrcId or IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_id_t;

#define SIZE_BY_APP_ID(app_id) (((app_id) == ZB_ZGP_APP_ID_0000) ? 4 : 8)
#define ZGPD_ID_SIZE(zgpd_id) SIZE_BY_APP_ID((zgpd_id)->app_id)

#define ZB_ZGPD_IDS_ARE_EQUAL(id1, id2) \
  (((id1)->app_id == (id2)->app_id) && \
   (((id1)->app_id == ZB_ZGP_APP_ID_0000) ? \
   ((id1)->addr.src_id == (id2)->addr.src_id) : \
    (!ZB_MEMCMP(&(id1)->addr.ieee_addr, &(id2)->addr.ieee_addr, sizeof(zb_ieee_addr_t)) &&\
     ((id1)->endpoint == (id2)->endpoint))))

/* From ZGP spec, A. 1.4.1.4:
 * ZGPD ID value of 0x00000000 indicates unspecified.
 * ... 0xffffffff indicates all.
 */
#define ZB_ZGP_SRC_ID_UNSPECIFIED 0x00000000
#define ZB_ZGP_SRC_ID_ALL         0xffffffff

#define ZB_INIT_ZGPD_ID(zgpd_id) \
{ \
  (zgpd_id)->app_id = ZB_ZGP_APP_ID_0000; \
  (zgpd_id)->addr.src_id = ZB_ZGP_SRC_ID_UNSPECIFIED; \
}

#define ZB_ZGPD_IS_SPECIFIED(zgpd_id) \
  (((zgpd_id)->app_id != ZB_ZGP_APP_ID_0000) \
  || ((zgpd_id)->addr.src_id != ZB_ZGP_SRC_ID_UNSPECIFIED))

#define ZB_MAKE_ZGPD_ID(zgpd_id, s_app_id, s_endpoint, s_addr)\
  {\
    (zgpd_id).app_id = (s_app_id);\
    (zgpd_id).endpoint = (s_endpoint);\
    (zgpd_id).addr = (s_addr);\
  }

/**
 * @brief ZGP security level type
 * @see ZGP spec, A.1.5.3.2 */
enum zb_zgp_security_level_e
{
  ZB_ZGP_SEC_LEVEL_NO_SECURITY           = 0x00,  /**< No security */
  ZB_ZGP_SEC_LEVEL_REDUCED               = 0x01,  /**< 1LSB of frame counter and short (2B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_NO_ENC           = 0x02,  /**< Full (4B) frame counter and full (4B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_WITH_ENC         = 0x03,  /**< Encryption & full (4B) frame counter and
                                                       full (4B) MIC */
};

/**
The gpSharedSecurityKeyType attribute can take the following values from Table 48:
Table 48 - Values of gpSecurityKeyType

See also Table 12 - Mapping between the gpSecurityKeyType and the SecurityKey
sub-field of the Extended NWK Frame Control field
*/
enum zb_zgp_security_key_type_e
{
  ZB_ZGP_SEC_KEY_TYPE_NO_KEY             = 0x00,  /**< No key */
  ZB_ZGP_SEC_KEY_TYPE_NWK                = 0x01,  /**< Zigbee NWK key */
  ZB_ZGP_SEC_KEY_TYPE_GROUP              = 0x02,  /**< ZGPD group key */
  ZB_ZGP_SEC_KEY_TYPE_GROUP_NWK_DERIVED  = 0x03,  /**< NWK-key derived ZGPD group key */
  ZB_ZGP_SEC_KEY_TYPE_ZGPD_INDIVIDUAL    = 0x04,  /**< (Individual) out-of-the-box ZGPD key */
  ZB_ZGP_SEC_KEY_TYPE_DERIVED_INDIVIDUAL = 0x07,  /**< Derived individual ZGPD key */
};
/**
 * @brief Security key sub-field value of Extended NWK frame control field
 *        derived from ZGPD key type (see @ref zb_zgp_security_key_type_e)
 *
 * ZGP spec, A.1.4.1.3
 * The Security Key sub-field, if set to 0b1, indicates an individual key
 * (KeyType 0b100 or 0b111). If set to 0b0, it indicates a shared key
 * (KeyType 0b011, 0b01017 or 0b001) or no key.
 */
#define ZGP_KEY_TYPE_IS_INDIVIDUAL(kt)                                  \
  ((kt) == ZB_ZGP_SEC_KEY_TYPE_ZGPD_INDIVIDUAL                          \
   || (kt) == ZB_ZGP_SEC_KEY_TYPE_DERIVED_INDIVIDUAL)

/********************************************************************/
/******** ZGP Device Type declarations and configuration ************/
/********************************************************************/

/**
 * @brief Possible ZGPD device identifiers
 * @see ZGP spec, A.4.3 */
typedef enum zb_zgpd_dev_id_e
{
  ZB_ZGP_SIMPLE_GEN_1_STATE_SWITCH_DEV_ID = 0x00, /**< Simple Generic 1-state ZGP switch */
  ZB_ZGP_ON_OFF_SWITCH_DEV_ID             = 0x02, /**< ZGP On/Off switch */
  ZB_ZGP_LEVEL_CONTROL_SWITCH_DEV_ID      = 0x03, /**< ZGP Level Control Switch */
#define ZB_ZGP_LVL_CTRL_SWITCH_DEV_ID ZB_ZGP_LEVEL_CONTROL_SWITCH_DEV_ID
  ZB_ZGP_TEMPERATURE_SENSOR_DEV_ID        = 0x30, /**< ZGP temperature sensor */
  ZB_ZGP_ENVIRONMENT_SENSOR_DEV_ID        = 0x33, /**< ZGP Temperature + Humidity sensor */
  ZB_ZGP_MANUF_SPECIFIC_DEV_ID            = 0xfe, /**< Manufactures-specific; 2
                                                   * more fields in the
                                                   * Commissioning frame. See 4.1
                                                   * Manufacturer Specific Green
                                                   * Power Device Type Use case
                                                   * in Green Power Device
                                                   * Manufacturer Specific Device & Command
                                                   * Definition Proposal
                                                  */
  ZB_ZGP_UNDEFINED_DEV_ID                 = 0xff, /**< Undefined device type */
}
zb_zgpd_dev_id_t;

/**
 * @brief Manufacturer-specific device identifiers for GreanPeak
 * @see ZGP spec, A.4.3 */
typedef enum zb_zgpd_manuf_specific_dev_id_e
{
  /* TODO: ZB_ZGP_MS_DOOR_SENSOR_DEV_ID is deprecated. Remove it when there will be
   * no testing devices using this type. GreanPeak door sensors use
   * ZB_ZGP_MS_DOOR_WINDOW_SENSOR_DEV_ID now. */
  ZB_ZGP_MS_DOOR_SENSOR_DEV_ID            = 0x00, /**< IAS Zone - Door Sensor*/
  ZB_ZGP_MS_DOOR_WINDOW_SENSOR_DEV_ID     = 0x01, /**< IAS Zone - Door/Window Sensor*/
  ZB_ZGP_MS_LEAKAGE_SENSOR_DEV_ID         = 0x02, /**< IAS Zone - Leakage Sensor */
  ZB_ZGP_MS_HUMIDITY_SENSOR_DEV_ID        = 0x03, /**< Relative Humidity Sensor */
  ZB_ZGP_MS_MOTION_SENSOR_DEV_ID          = 0x05, /**< IAS Zone - Motion sensor */
  ZB_ZGP_MS_MOVEMENT_SENSOR_DEV_ID        = 0x07, /**< IAS Zone - Movement sensor */
  ZB_ZGP_MS_SMART_PLUG_DEV_ID             = 0X08, /**< GreenPeak Greenpower smart plug */
  ZB_ZGP_MS_KEY_FOB_DEV_ID                = 0x0A, /**< GreenPeak Greenpower IAS Zone - Key fob */
  ZB_ZGP_MS_SMOKE_DETECTOR_DEV_ID         = 0x0b, /**< IAS Zone - Smoke Detector */
  ZB_ZGP_MS_PARKING_SENSOR_DEV_ID         = 0xc0, /**< Occupancy - Parking sensor */
}
zb_zgpd_manuf_specific_dev_id_t;

/**
   @}
*/

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/


/********************************************************************/
/*********************** Sink definitions ***************************/
/********************************************************************/

/**
 * @brief Mapping of ZGPD command ID to Zigbee ZCL command ID
 */
typedef struct zgp_to_zb_cmd_mapping_s
{
  zb_uint8_t zgp_cmd_id;  /**< ZGPD command ID */
  zb_uint8_t zb_cmd_id;   /**< Zigbee ZCL command ID */
}
zgp_to_zb_cmd_mapping_t;

typedef struct zgps_cluster_rec_s
{
  zb_uint16_t cluster_id;   /** Cluster ID to which specified ZGPD commands are translated
                                (see @ref zb_zcl_cluster_id_t) */
/**
 * Options field of cluster table entry
 *
 * [0-1]        role mask          client/server/both
 *
 */
  zb_uint8_t  options;
  zb_uint8_t  cmd_ids[ZB_ZGP_MATCH_TBL_MAX_CMDS_FOR_MATCH]; /**< Supported commands by sink */
}
zgps_dev_cluster_rec_t;

#define GET_CLUSTER_ROLE(cluster) \
  (cluster->options & 0x03)

typedef ZB_PACKED_PRE union zgps_device_id_u
{
  zb_uint8_t  zgpd_dev_id;
  zb_uint16_t	zgpd_manuf_model;
}
zgps_device_id_t;

typedef ZB_PACKED_PRE struct zgps_dev_match_rec_s
{
  zb_uint16_t		clusters[ZB_ZGP_TBL_MAX_CLUSTERS];
  zb_uint16_t           manuf_id;
  zgps_device_id_t 	dev_id;
}
ZB_PACKED_STRUCT zgps_dev_match_rec_t;

#define IS_STANDART_ZGPS_DEVICE(dev_match_rec) \
  (dev_match_rec->manuf_id == ZB_ZGPD_MANUF_ID_UNSPEC)

/**
 * @brief Necessary information for filling translation table for any ZGPD
 *        during commissioning
 *
 * Includes matching table and command ID mappings. Given some ZGPD device ID it is
 * possible to fill translation table entry with help of this structure.
 */
typedef struct zb_zgps_match_info_s
{
  const zb_uint8_t                       match_tbl_size;
  const ZB_CODE zgps_dev_match_rec_t    *match_tbl;
  const zb_uint8_t                       cmd_mappings_count;
  const ZB_CODE zgp_to_zb_cmd_mapping_t *cmd_mapping;
  const zb_uint8_t                       clusters_tbl_size;
  const ZB_CODE zgps_dev_cluster_rec_t  *clusters_tbl;
}
zb_zgps_match_info_t;

enum zb_zgp_data_handle_e
{
  ZB_ZGP_HANDLE_DEFAULT_HANDLE,
  ZB_ZGP_HANDLE_REMOVE_CHANNEL_CONFIG,
  ZB_ZGP_HANDLE_REMOVE_COMMISSIONING_REPLY,
  ZB_ZGP_HANDLE_ADD_COMMISSIONING_REPLY,
  ZB_ZGP_HANDLE_ADD_CHANNEL_CONFIG,
  ZB_ZGP_HANDLE_REMOVE_AFTER_FAILED_COMM,
  ZB_ZGP_HANDLE_REMOVE_BY_USER_REQ,

/**
 * The first handle that can be used by application for gp-data.req primitives.
 * Application may use any greater or equal handle value to match request with
 * confirmation.
 *
 * @see zb_zgps_send_data().
 */
  ZB_ZGP_HANDLE_APP_DATA,
};

#define ZB_GP_TX_QUEUE_ENTRY_LIFETIME_NONE ZB_MIN_TIME_VAL
#define ZB_GP_TX_QUEUE_ENTRY_LIFETIME_INF  ZB_MAX_TIME_VAL

#define ZB_GP_DATA_REQ_USE_GP_TX_QUEUE   0x01
#define ZB_GP_DATA_REQ_USE_CSMA_CA_BIT   0x02
#define ZB_GP_DATA_REQ_USE_MAC_ACK_BIT   0x04
#define ZB_GP_DATA_REQ_MAINT_FRAME_TYPE  0x08

#define ZB_GP_DATA_REQ_FRAME_TYPE(tx_opt) \
  ((tx_opt >> 3) & 0x03)

#define ZB_GP_DATA_REQ_ACTION_REMOVE_GPDF ZB_FALSE
#define ZB_GP_DATA_REQ_ACTION_ADD_GPDF    ZB_TRUE

#define ZB_CGP_DATA_REQ_USE_CSMA_CA_BIT 0x01
#define ZB_CGP_DATA_REQ_USE_MAC_ACK_BIT 0X02

/*! @} */
/*! @endcond */

/********************************************************************/
/*********************** Proxy definitions **************************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/

/** Values of gpsCommunicationMode attribute
 *  Table 27
*/
typedef enum zgp_communication_mode_e
{
  ZGP_COMMUNICATION_MODE_FULL_UNICAST = 0,
  /*Groupcast - one of the communication modes used for tunneling GPD commands between the
  proxies and sinks. In ZigBee terms, it is the APS level multicast, with NWK level broadcast to the
  RxOnWhenIdle=TRUE (0xfffd) broadcast address.*/
  ZGP_COMMUNICATION_MODE_GROUPCAST_DERIVED = 1,
  ZGP_COMMUNICATION_MODE_GROUPCAST_PRECOMMISSIONED = 2,
  ZGP_COMMUNICATION_MODE_LIGHTWEIGHT_UNICAST = 3
} zgp_communication_mode_t;

/* A.3.3.2.4 gpsCommissioningExitMode attribute */
typedef enum zgp_commissioningp_exit_mode_e
{
  ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION  = (1<<0),
  ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS                  = (1<<1),
  ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT = (1<<2),
  ZGP_COMMISSIONING_EXIT_MODE_ON_CWE_OR_PS                        = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS),
  ZGP_COMMISSIONING_EXIT_MODE_ON_CWE_OR_PCM                       = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT),
  ZGP_COMMISSIONING_EXIT_MODE_ALL                                 = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT)
} zgp_commissioningp_exit_mode_t;

/** @brief possible types of GP device */
typedef enum zgp_gp_device_e {
  ZGP_DEVICE_PROXY       = 0x0060,
  ZGP_DEVICE_PROXY_BASIC = 0x0061,
  ZGP_DEVICE_TARGET_PLUS = 0x0062,
  ZGP_DEVICE_TARGET      = 0x0063,
  ZGP_DEVICE_COMMISSIONING_TOOL  = 0x0064,
  ZGP_DEVICE_COMBO       = 0x0065,
  ZGP_DEVICE_COMBO_BASIC = 0x0066
} zgp_gp_device_t;

typedef ZB_PACKED_PRE struct zb_zgp_gp_proxy_info_s
{
  zb_uint16_t short_addr;
  zb_uint8_t  link;
}
ZB_PACKED_STRUCT zb_zgp_gp_proxy_info_t;

#define ZGP_PROXY_COMM_MODE_BROADCAST 0
#define ZGP_PROXY_COMM_MODE_UNICAST   1

/*! @} */

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/* Structure stores GreenPower security and commissioning parameters */
typedef struct zb_zgp_cluster_s
{
  zb_uint8_t  gp_shared_security_key_type; /**< @see zb_zgp_shared_security_key_type_t */
  zb_uint8_t  gp_shared_security_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t  gp_link_key[ZB_CCM_KEY_SIZE];
  zb_uint8_t  gps_communication_mode; /**< @see zgp_communication_mode_t  */
  zb_uint8_t  gps_commissioning_exit_mode; /**< @see zgp_commissioningp_exit_mode_t  */
  zb_uint8_t  gps_security_level;
  zb_uint16_t gps_commissioning_window;
} zb_zgp_cluster_t;

#define IS_EQUAL_ZGPD_ID_FOR_TABLE_ENTRY(ent, id)  \
  (ZGP_TBL_GET_APP_ID((ent)) == (id)->app_id\
   && (((id)->app_id == ZB_ZGP_APP_ID_0000 && ((ent)->zgpd_id.src_id == (id)->addr.src_id || (ent)->zgpd_id.src_id == ZB_ZGP_SRC_ID_ALL)) || \
       ((id)->app_id == ZB_ZGP_APP_ID_0010 && (!ZB_MEMCMP(&(id)->addr, &(ent)->zgpd_id, sizeof(zb_ieee_addr_t)) || ZB_IEEE_ADDR_IS_UNKNOWN((ent)->zgpd_id.ieee_addr)) && \
        ((id)->endpoint == (ent)->endpoint || (id)->endpoint == ZB_ZGP_ALL_ENDPOINTS || (ent)->endpoint == ZB_ZGP_ALL_ENDPOINTS || \
         (id)->endpoint == ZB_ZGP_COMMUNICATION_ENDPOINT))))

typedef ZB_PACKED_PRE struct zgp_pair_group_list_s
{
  zb_uint16_t sink_group;
  zb_uint16_t alias;
}
ZB_PACKED_STRUCT zgp_pair_group_list_t;

typedef struct zgp_tbl_ent_s
{
  zb_zgpd_addr_t   zgpd_id;             /**< ID of the paired ZGPD */
  zb_uint16_t      options;             /**< The options for the reception from ZGPD */
  zb_uint16_t      zgpd_assigned_alias; /**< The commissioned 16-bit ID to be used
                                          as alias for ZGPD */

  zb_uint32_t      security_counter; /**< The incoming security frame counter for ZGPD */
  zb_uint8_t       zgpd_key[ZB_CCM_KEY_SIZE]; /**< Security key for the GPD */
  zb_uint8_t       endpoint;
  zb_uint8_t       sec_options;               /**< Security options */
  zb_uint8_t       groupcast_radius;    /**< To limit the range of the groupcast */

  zb_uint8_t       is_sink;
/*
For Proxy: see A.3.4.2.2.2 Proxy Table entry format

Options
GPD ID
Endpoint
GPD Assigned Alias
Security Options
GPD security frame counter
GPD key
Lightweight sink address list (ieee + short). Limit to 2? 4?
Sink group list (may limit to 2 groups)
Groupcast radius
Search Counter
Extended Options
*/

/*
Sink table format - see A.3.3.2.2.2	Sink Table entry format
Options
GPD ID
Endpoint
GPD Assigned Alias
Security Options
GPD security frame counter
GPD key
DeviceID
Group list
Groupcast radius

 */

  union
  {
    struct zgp_proxy_tbl_ent_s
    {
      zb_uint16_t ext_options;
      struct zgp_lwsink_addr_list_s
      {
        zb_address_ieee_ref_t addr_ref;
      } lwsaddr[ZB_ZGP_MAX_LW_UNICAST_ADDR_PER_GPD];
      zgp_pair_group_list_t sgrp[ZB_ZGP_MAX_SINK_GROUP_PER_GPD];
    } proxy;
    struct zgp_sink_tbl_ent_s
    {
      zb_uint8_t       device_id;           /**< ZGPD Device ID @see zb_zgpd_dev_id_t */
      zgp_pair_group_list_t sgrp[ZB_ZGP_MAX_SINK_GROUP_PER_GPD];
      zb_uint8_t match_dev_tbl_idx;
      /**
       * Extension to the table (field is not presented in specification).
       *
       * Dest IEEE address to use in GPT to GPD packets even if GPD is identified by SrcID
       */
      zb_ieee_addr_t   ieee_addr;
    } sink;
  } u;
} zgp_tbl_ent_t;

typedef zgp_tbl_ent_t zb_zgp_sink_tbl_ent_t;
typedef zgp_tbl_ent_t zb_zgp_proxy_tbl_ent_t;

typedef struct zb_zgp_tbl_array_s
{
  zb_uint32_t back_sec_counter;
  zb_uint32_t security_counter;
  zb_uint32_t nvram_offset;
  zb_uint8_t  lqi;
  zb_int8_t   rssi;

  /* bit 0 - EntryValid flag
   * bit 1 - FirstToForward flag
   * bit 2 - HasAllUnicastRoutes flag
   * bits 3-7 reserved
   */
  zb_uint8_t runtime_options;
  zb_uint8_t search_counter;
} zb_zgp_tbl_array_t;

typedef struct zb_zgp_tbl_s
{
  zgp_tbl_ent_t cached;
  /* pack 8 4-bit entries to every array element */
  zb_uint32_t security_counter_timeouts[((ZB_ZGP_PROXY_TBL_SIZE > ZB_ZGP_SINK_TBL_SIZE) ? ZB_ZGP_PROXY_TBL_SIZE : ZB_ZGP_SINK_TBL_SIZE + 7) / 8];
  zb_uint_t cached_i;
  zb_uint_t entry_size;
  zb_uint_t tbl_size;
  zb_uint8_t nvram_dataset;     /*<! @see zb_nvram_dataset_types_t  */
  zb_uint8_t nvram_page;

  zb_zgp_tbl_array_t array[1];
} zb_zgp_tbl_t;


typedef struct zb_zgp_proxy_table_s
{
  zb_zgp_tbl_t base;

  zb_zgp_tbl_array_t array[ZB_ZGP_PROXY_TBL_SIZE - 1];
} zb_zgp_proxy_table_t;


typedef struct zb_zgp_sink_table_s
{
  zb_zgp_tbl_t base;

  zb_zgp_tbl_array_t array[ZB_ZGP_SINK_TBL_SIZE - 1];
} zb_zgp_sink_table_t;

/*! @}
 *  @endcond*/

/********************************************************************/
/******************** Commissioning definitions *********************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/


/**
 * @brief Possible commissioning result
 */
typedef enum zb_zgp_comm_status_e
{
  /** Commissioning with some device completed successfully */
  ZB_ZGP_COMMISSIONING_COMPLETED,
  /** Commissioning failed. The reason can be:
   * - parameters of connection can't be negotiated
   * - ZGPD device behaves incorrectly
   * - Maximum number of connected ZGPDs is reached */
  ZB_ZGP_COMMISSIONING_FAILED,
  /** Commissioning failed, because of timeout */
  ZB_ZGP_COMMISSIONING_TIMED_OUT,
  /** No functionality match with commissioning device is found.
   *  Maybe matching table is not provided by user application */
  ZB_ZGP_COMMISSIONING_NO_MATCH_ERROR,
  /** Commissioning failed, because some internal error occured in stack.
   * This type of error is recoverable, so next commissioning attempt can
   * be successful */
  ZB_ZGP_COMMISSIONING_INTERNAL_ERROR,
  /** Commissioning failed, because some critical error has occured.
   * Normal functioning of ZGP subsystem is not possible
   * (e.g. physical operational channel can't be changed) */
  ZB_ZGP_COMMISSIONING_CRITICAL_ERROR,
  /** User cancelled commissioning by calling zb_zgps_stop_commissioning() */
  ZB_ZGP_COMMISSIONING_CANCELLED_BY_USER,
  /** ZGPD sent Decommissioning command */
  ZB_ZGP_ZGPD_DECOMMISSIONED,
}
zb_zgp_comm_status_t;

/**
 * @brief Commissioning callback type
 *
 * Note: this is legacy API. Use ZB_ZGP_SIGNAL_COMMISSIONING signal passed to
 * zboss_signal_handler instead!
 *
 * Commissioning callback notifies user about commissioning complete. So, ZGP
 * device is in the operational mode at the time of calling this callback.
 * Result variable carries the status of commissioning: whether some device
 * successfully commissioned or not.
 */
typedef void (ZB_CODE * zb_zgp_comm_completed_cb_t)(
    zb_zgpd_id_t *zgpd_id,
    zb_zgp_comm_status_t result);

/**
 * @brief Commissioning request callback type
 *
 * Commissioning request callback notifies application about commissioning
 * attempt from ZGPD. Callback is called when ZGPD sends commissioning frame.
 * Callback provides to user ZGPD ID and ZGPD Device ID. Based on these
 * parameters user application should decide whether commissioning procedure
 * should be continued or not.
 *
 * Commissioning request callback should be set during ZGP initialization using
 * @ref ZB_ZGP_REGISTER_COMM_REQ_CB macro. If callback is not registered, then
 * ZBOSS accepts all incoming commissioning attempts.
 *
 * @param zgpd_id         [in]  ZGPD ID
 * @param device_id       [in]  ZGPD device ID
 * @param manuf_id        [in]  Manufacturer ID (meaningful if device_id = 0xFE or 0xFF)
 * @param manuf_model_id  [in]  Manufacturer model ID (meaningful if device_id = 0xFE or 0xFF)
 * @param ieee_addr       [in]  ZGPD long IEEE address if available, otherwise filled with zeroes
 *
 * @see zb_zgps_accept_commissioning
 * @see ZB_IS_64BIT_ADDR_ZERO
 */
typedef void (ZB_CODE * zb_zgp_comm_req_cb_t)(
    zb_zgpd_id_t    *zgpd_id,
    zb_uint8_t       device_id,
    zb_uint16_t      manuf_id,
    zb_uint16_t      manuf_model_id,
    zb_ieee_addr_t   ieee_addr);

#ifdef ZB_ENABLE_ZGP_CLUSTER
#define ZB_ZGP_UNSEL_TEMP_MASTER_IDX   0xFF
#define ZB_ZGP_TEMP_MASTER_EMPTY_ENTRY 0xFFFF
#endif

/*! @} */

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/


/**
 * @brief Information about commissioning process with a ZGPD
 */
typedef struct zb_zgps_dev_comm_data_s
{
  zb_uint8_t     state;         /**< Current commissioning state \ref zb_zgp_comm_state_t */
  zb_zgpd_id_t   zgpd_id;       /**< ZGPD ID */
  zb_uint8_t     oper_channel;  /**< ZGP physical operational channel */
  zb_uint8_t     temp_master_tx_chnl;  /**< Temp channel during commissioning */
  zb_uint8_t     basic_flag;    /**< If Channel configuration was send by Basic Combo, this bit will be set to 1 */
  zb_uint8_t     result;        /**< Commissioning result @ref zb_zgp_comm_status_t */
  zb_uint8_t     comm_req_buf;  /**< Reference to buffer with Commissioning GPDF from ZGPD */
  /** Used for duplicate filtering during early stages of commissioning, when device is not
   * added to the sink table yet. Two frame types are filtered this way:
   * - Channel request frames (filtered by MAC sequence number). Also it's impossible to filter
   *   duplicates using sink table if Channel request is sent as Maintenance frame without concrete
   *   ZGPD ID.
   * - Commissioning frames (both secured and unsecured).
   */
  zb_uint32_t    comm_dup_counter;
  /* indicate for dup_counter check function that comm_dup_counter field have legal and valid value */
  zb_uint8_t     any_packet_received;

  /* A.3.3.5.3 The Options as shown in Figure 53 without action field */
  /*
    0-2:  exit mode
      3:  channel present
      4:  unicast communication
  */
  zb_uint8_t     proxy_comm_mode_options;
  zb_uint16_t    sink_addr;

  zb_uint8_t     gpdf_options;
  zb_uint8_t     gpdf_ext_options;
  zb_uint32_t    gpdf_security_frame_counter;

#if defined ZB_ENABLE_ZGP_CLUSTER && defined ZB_ZGP_SINK
  zb_uint8_t     selected_temp_master_idx;
  zb_zgp_gp_proxy_info_t temp_master_list[ZB_ZGP_MAX_TEMP_MASTER_COUNT];
  zb_uint8_t     need_send_dev_annce;
#endif
}
zb_zgps_dev_comm_data_t;

/********************************************************************/
/********************* TX Queue definitions *************************/
/********************************************************************/

#define USED_MASK(count) ((count)/8 + 1)

#ifndef ZB_ZGP_IMMED_TX
ZB_ASSERT_COMPILE_DECL(ZB_ZGP_TX_QUEUE_SIZE <= ZB_ZGP_TX_PACKET_INFO_COUNT);
#endif  /* ZB_ZGP_IMMED_TX */

#define TX_PACKET_INFO_QUEUE_USED_MASK USED_MASK(ZB_ZGP_TX_PACKET_INFO_COUNT)

typedef struct zb_zgp_tx_pinfo_s
{
  zb_uint8_t                    handle;
  zb_zgpd_id_t                  zgpd_id;
  zb_uint8_t                    buf_ref;   /* Reference to buffer, which is used for GPDF */
}
zb_zgp_tx_pinfo_t;

typedef struct zb_zgp_tx_q_ent_s
{
  zb_uint8_t    tx_options;
  zb_uint8_t    cmd_id;
  zb_uint8_t    payload_len;
  zb_uint8_t    pld[ZB_ZGP_TX_CMD_PLD_MAX_SIZE]; /**< Payload */
  zb_bitfield_t is_expired:1;
  zb_bitfield_t sent:1;
  zb_bitfield_t reserved:6;
}
zb_zgp_tx_q_ent_t;

typedef struct zb_zgp_tx_q_s
{
  zb_zgp_tx_q_ent_t queue[ZB_ZGP_TX_QUEUE_SIZE];
}
zb_zgp_tx_q_t;

typedef struct zb_zgp_tx_packet_info_q_s
{
  zb_zgp_tx_pinfo_t queue[ZB_ZGP_TX_PACKET_INFO_COUNT];
  zb_uint8_t        used_mask[TX_PACKET_INFO_QUEUE_USED_MASK];
}
zb_zgp_tx_packet_info_q_t;

/********************************************************************/
/********* Runtime context declarations for ZGP profile *************/
/********************************************************************/


/**
 * @brief Application confirm callback type
 *
 * Application confirm callback notifies application about data
 * request attempt to ZGPD. Callback is called when data frame is sent
 * to ZGPD. Callback provides to command ID and its status. Based on these
 * parameters user application should decide further actions.
 *
 * Application confirm callback should be set during ZGP initialization using
 * @ref ZB_ZGP_REGISTER_APP_CFM_CB macro.
 *
 * @param cmd_id          [in]  Command ID
 * @param status          [in]  Confimation status
 * @param data_ptr        [in]  Pointer to data payload
 * @param zgpd_id         [in]  ZGPD ID
 * @param handle          [in]  ZGP handle
 */
typedef void (ZB_CODE * zb_zgp_app_cfm_cb_t)(
  zb_uint8_t    cmd_id,
  zb_int16_t    status,
  zb_uint8_t   *data_ptr,
  zb_zgpd_id_t *zgpd_id,
  zb_uint8_t    handle);

/*! @} */
/*! @endcond */


/**
   @addtogroup zgp_sink
   @{
*/

  /**
   * @brief Application callback, indication of the attempted commissioning in GPS operational mode.
   *
   * Application commissioning indication callback notifies application about commissioning
   * attempt from ZGPD. Callback is called when commissioning frame or notification is received
   * from ZGPD. Callback provides to zgpd_id and full incoming packet's data. Based on these
   * parameters user application should decide further actions.
   *
   * Application commissioning indication callback should be set during ZGP initialization using
   * @ref ZB_ZGP_REGISTER_APP_CIC_CB macro.
   *
   * @param zgpd_id         [in]  ZGPD ID
   * @param param           [in]  buffer index, containing GPDF
   */
typedef void (ZB_CODE * zb_zgp_app_comm_ind_cb_t)(
  zb_zgpd_id_t *zgpd_id,
  zb_uint8_t    param);


/*! @} */

/**
   @cond internal
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Application search ZGP manufactured specific device callback
 *
 * Callback is called when sink table entry data is restored from old NVRAM version.
 *
 * Application search ZGP manufactured specific device callback
 * should be set during ZGP initialization using
 * @ref ZB_ZGP_REGISTER_APP_SEARCH_ZGP_DEVICE_CB macro.
 *
 * @param zgpd_id         [in]  ZGPD ID
 * @param manuf_model_id  [out]  Manufacturer model ID
 * @return RET_OK on success, RET_NOT_FOUND otherwise
 */
typedef zb_ret_t (ZB_CODE * zb_zgp_app_search_zgp_device_cb_t)(
                  zb_zgpd_id_t *zgpd_id,
                  zb_uint16_t  *manuf_model_id);


/** @brief global ZGP context */
typedef struct zb_zgp_ctx_s
{
  zb_uint8_t                device_role; /**< current device role. @see zgp_gp_device_t  */
  zb_uint8_t                bidir_commissioning_disable; /**< if 1, disable
                                                          * bidirectional commissioning  */
  zb_zgp_tx_q_t             tx_queue;     /**< ZGP Tx queue */
  zb_zgp_tx_packet_info_q_t tx_packet_info_queue;
  zb_uint8_t                delayed_data_ind_buf; /**< When we've received GPDF with Maintenance frame type (0b01) */
  /** and if we found an outgoing packet in our GP TX Queue, then we need to send the outgoing packet which we found,
    * subsequently we need to wait "confirm event of send", subsequently we need to process the incoming GPDF. */

  zb_uint8_t                tbl_alarm_running;
  zb_zgps_dev_comm_data_t   comm_data;  /**< Info about currently ongoing commissioning process */

#ifdef ZB_ZGP_SINK
  zb_uint8_t                sink_mode; /**< Current mode of Sink side. One of the @ref zb_zgp_mode_t */
  /** Functionality matching information. Should be set by user application.
    * It is used during commissioning to fill translation table entries based
    * on ZGPD Device ID received in Commissioning command. */
  const zb_zgps_match_info_t *match_info;
  zb_zgp_comm_completed_cb_t comm_cb;   /**< Commissioning callback on commissioning complete event */
  zb_zgp_comm_req_cb_t      comm_req_cb;   /**< Commissioning callback when ZGPD sent commissioning frame */
  zb_zgp_app_cfm_cb_t       app_cfm_cb;    /**< Application callback
                                             * when ZGPD sent application data */
  zb_zgp_app_comm_ind_cb_t  app_comm_op_cb;    /**< Application callback when receive commisioning in operational mode */

#ifdef ZB_ZGP_IMMED_TX
  zb_uint32_t               immed_tx_frame_counter;
#endif  /* ZB_ZGP_IMMED_TX */

  /**< Application callback when migrate NVRAM */
  zb_zgp_app_search_zgp_device_cb_t app_search_zgp_device_cb;

  /* new Sink table, unified with Proxy table */
  zb_zgp_sink_table_t       sink_table;

  /* b0 - if 1 - start comm/stop comm by gp_sink_commissioning_mode request
   * b1 - if 1 - unicast proxy commissioning mode, otherwise - broadcast mode
   * b7-b15 - gp_sink_commissioning_mode options */
  zb_uint16_t               sink_comm_mode_opt;
#endif  /* ZB_ZGP_SINK */
  zb_uint8_t                proxy_mode; /**< Current mode of Proxy side. One of the @ref
                                         * zb_zgp_mode_t */
  zb_zgp_cluster_t          cluster;
#ifdef ZB_ENABLE_ZGP_PROXY
  zb_zgp_proxy_table_t      proxy_table;
#endif  /* ZB_ENABLE_ZGP_PROXY */
}
zb_zgp_ctx_t;

zb_zgp_ctx_t *zb_zgp_ctx();
/** @brief Macro to access global ZGP context */
#define ZGP_CTX() (*zb_zgp_ctx())

/*! @} */
/*! @endcond */

/**
   @addtogroup zgp_sink
   @{
*/

/**
 * @ingroup zgp_sink
 * @brief Set matching information that is used to fill ZGP command - ZCL
 * cluster translation table.
 * @param [in]  info  Matching information of type @ref zb_zgps_match_info_t
 */
#define ZB_ZGP_SET_MATCH_INFO(info) \
{ \
  ZGP_CTX().match_info = info; \
}

/**
 * @ingroup zgp_sink
 * @brief Register commissioning callback
 *
 * Note: this is legacy API. Use ZB_ZGP_SIGNAL_COMMISSIONING signal passed to
 * zboss_signal_handler instead!
 *
 * @param cb [in]  Commissioning callback (@ref zb_zgp_comm_completed_cb_t)
 */
#define ZB_ZGP_REGISTER_COMM_COMPLETED_CB(cb) \
{ \
  ZGP_CTX().comm_cb = cb; \
}

/**
 * @ingroup zgp_sink
 * @brief Register commissioning request callback
 *
 * @param cb [in]  Commissioning request callback (@ref zb_zgp_comm_req_cb_t)
 *
 * @snippet tests/zgp/gppb/test_gps_decommissioning/dut_gps.c accept_comm
 */
#define ZB_ZGP_REGISTER_COMM_REQ_CB(cb) \
{ \
  ZGP_CTX().comm_req_cb = cb; \
}

/**
 * @ingroup zgp_sink
 * @brief Register application commissioning indication callback
 *
 * @param cb [in]  Application commissioning indication callback (@ref zb_zgp_app_comm_ind_cb_t)
 */

#define ZB_ZGP_REGISTER_APP_CIC_CB(cb) \
{ \
  ZGP_CTX().app_comm_op_cb = cb; \
}

/*! @} */
/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Register application confirm callback
 *
 * @param cb [in]  Application confirm callback (@ref zb_zgp_app_cfm_cb_t)
 */

#define ZB_ZGP_REGISTER_APP_CFM_CB(cb) \
{ \
  ZGP_CTX().app_cfm_cb = cb; \
}


/**
 * @brief Register application search ZGP manufactured specific device callback
 *
 * @param cb [in]  Application search ZGP manufactured specific device callback (@ref zb_zgp_app_search_zgp_device_cb_t)
 */

#define ZB_ZGP_REGISTER_APP_SEARCH_ZGP_DEVICE_CB(cb) \
{ \
  ZGP_CTX().app_search_zgp_device_cb = cb; \
}

/**
 * @ingroup zgp_sink
 * @brief Register application search ZGP manufactured specific device callback
 *
 * @param cb [in]  Application search ZGP manufactured specific device callback (@ref zb_zgp_app_search_zgp_device_cb_t)
 */

#define ZB_ZGP_REGISTER_APP_SEARCH_ZGP_MANUF_SPEC_DEVICE_CB(cb) \
{ \
  ZGP_CTX().app_search_zgp_device_cb = cb; \
}


/*! @} */
/*! @endcond */

/**
   @addtogroup zgp_common
   @{
*/


/********************************************************************/
/********************* GPDF command IDs *****************************/
/********************************************************************/

/**
 * @brief Command identifiers sent from or to ZGPD */
enum zb_zgpd_cmd_id_e
{
  ZB_GPDF_CMD_RECALL_SCENE0                        = 0x10,
  ZB_GPDF_CMD_RECALL_SCENE1                        = 0x11,
  ZB_GPDF_CMD_RECALL_SCENE2                        = 0x12,
  ZB_GPDF_CMD_RECALL_SCENE3                        = 0x13,
  ZB_GPDF_CMD_RECALL_SCENE4                        = 0x14,
  ZB_GPDF_CMD_RECALL_SCENE5                        = 0x15,
  ZB_GPDF_CMD_RECALL_SCENE6                        = 0x16,
  ZB_GPDF_CMD_RECALL_SCENE7                        = 0x17,
  /* Note: recall scene 8-11 are got from LCGW. But GPPB specification
   * does not define recall scenes 8-11. Only 8 scenes are
   * supported. See Table 49 Payloadless GPDF commands 6236 sent by
   * GPD */
  ZB_GPDF_CMD_RECALL_SCENE8                        = 0x18,
  ZB_GPDF_CMD_RECALL_SCENE9                        = 0x19,
  ZB_GPDF_CMD_RECALL_SCENE10                       = 0x1A,
  ZB_GPDF_CMD_RECALL_SCENE11                       = 0x1B,
#define ZB_GPDF_CMD_STORE_SCENE0                       ZB_GPDF_CMD_RECALL_SCENE8
#define ZB_GPDF_CMD_STORE_SCENE1                       ZB_GPDF_CMD_RECALL_SCENE9
#define ZB_GPDF_CMD_STORE_SCENE2                       ZB_GPDF_CMD_RECALL_SCENE10
#define ZB_GPDF_CMD_STORE_SCENE3                       ZB_GPDF_CMD_RECALL_SCENE11
  ZB_GPDF_CMD_STORE_SCENE4                        = 0x1C,
  ZB_GPDF_CMD_STORE_SCENE5                        = 0x1D,
  ZB_GPDF_CMD_STORE_SCENE6                        = 0x1E,
  ZB_GPDF_CMD_STORE_SCENE7                        = 0x1F,
  /* on/off */
  ZB_GPDF_CMD_OFF                                  = 0x20,
  ZB_GPDF_CMD_ON                                   = 0x21,
  ZB_GPDF_CMD_TOGGLE                               = 0x22,
  /* level control */
  ZB_GPDF_CMD_MOVE_UP                              = 0x30,
  ZB_GPDF_CMD_MOVE_DOWN                            = 0x31,
  ZB_GPDF_CMD_STEP_UP                              = 0x32,
  ZB_GPDF_CMD_STEP_DOWN                            = 0x33,
  ZB_GPDF_CMD_LC_STOP                              = 0x34,
#define ZB_GPDF_CMD_LVL_CTRL_STOP ZB_GPDF_CMD_LC_STOP
  ZB_GPDF_CMD_MOVE_UP_W_ONOFF                      = 0x35,
#define ZB_GPDF_CMD_MOVE_UP_WITH_ON_OFF ZB_GPDF_CMD_MOVE_UP_W_ONOFF
  ZB_GPDF_CMD_MOVE_DOWN_W_ONOFF                    = 0x36,
#define ZB_GPDF_CMD_MOVE_DOWN_WITH_ON_OFF ZB_GPDF_CMD_MOVE_DOWN_W_ONOFF
  ZB_GPDF_CMD_STEP_UP_W_ONOFF                      = 0x37,
#define ZB_GPDF_CMD_MOVE_STEP_ON ZB_GPDF_CMD_STEP_UP_W_ONOFF
  ZB_GPDF_CMD_STEP_DOWN_W_ONOFF                    = 0x38,
#define ZB_GPDF_CMD_MOVE_STEP_OFF ZB_GPDF_CMD_STEP_DOWN_W_ONOFF
  /* Color Control */
  ZB_GPDF_CMD_MOVE_HUE_STOP                        = 0x40,
  ZB_GPDF_CMD_MOVE_HUE_UP                          = 0x41,
  ZB_GPDF_CMD_MOVE_HUE_DOWN                        = 0x42,
  ZB_GPDF_CMD_STEP_HUE_UP                          = 0x43,
  ZB_GPDF_CMD_STEP_HUE_DOWN                        = 0x44,
  ZB_GPDF_CMD_MOVE_SATURATION_STOP                 = 0x45,
  ZB_GPDF_CMD_MOVE_SATURATION_UP                   = 0x46,
  ZB_GPDF_CMD_MOVE_SATURATION_DOWN                 = 0x47,
  ZB_GPDF_CMD_STEP_SATURATION_UP                   = 0x48,
  ZB_GPDF_CMD_STEP_SATURATION_DOWN                 = 0x49,
  ZB_GPDF_CMD_MOVE_COLOR                           = 0x4A,
  ZB_GPDF_CMD_STEP_COLOR                           = 0x4B,
  /* Simple Generic Switch */
  ZB_GPDF_CMD_PRESS_1_OF_1                         = 0x60,
  ZB_GPDF_CMD_RELEASE_1_OF_1                       = 0x61,
  ZB_GPDF_CMD_PRESS_1_OF_2                         = 0x62,
  ZB_GPDF_CMD_RELEASE_1_OF_2                       = 0X63,

  ZB_GPDF_CMD_ATTR_REPORT                          = 0xA0,
  ZB_GPDF_CMD_MANUF_SPEC_ATTR_REPORT               = 0xA1,
  ZB_GPDF_CMD_MULTI_CLUSTER_ATTR_REPORT            = 0xA2,
  ZB_GPDF_CMD_MANUF_SPEC_MULTI_CLUSTER_ATTR_REPORT = 0xA3,
  ZB_GPDF_CMD_REQUEST_ATTRIBUTES                   = 0xA4,
  ZB_GPDF_CMD_READ_ATTR_RESP                       = 0xA5,

  ZB_GPDF_CMD_ZCL_TUNNELING_FROM_ZGPD              = 0xA6,

  /* commissioning from ZGPD */
  ZB_GPDF_CMD_COMMISSIONING                        = 0xE0,
  ZB_GPDF_CMD_DECOMMISSIONING                      = 0xE1,
  ZB_GPDF_CMD_SUCCESS                              = 0xE2,
  ZB_GPDF_CMD_CHANNEL_REQUEST                      = 0xE3,

  /* GPDF commands sent to GPD */
  ZB_GPDF_CMD_COMMISSIONING_REPLY                  = 0xF0,
  ZB_GPDF_CMD_WRITE_ATTRIBUTES                     = 0xF1,
  ZB_GPDF_CMD_READ_ATTRIBUTES                      = 0xF2,
  ZB_GPDF_CMD_CHANNEL_CONFIGURATION                = 0xF3,

  ZB_GPDF_CMD_ZCL_TUNNELING_TO_ZGPD                = 0xF6,
};

/*! @} */

/********************************************************************/
/**************** Macros for sending GPDF packets *******************/
/********************************************************************/

/**
   @cond internal
   @addtogroup zgp_internal
   @{
*/


/**
 * @brief Start constructing GPDF packet
 *
 * @param [in] zbbuf Buffer for GPDF packet
 */
#define ZB_START_GPDF_PACKET(zbbuf)                    \
   (ZB_BUF_REUSE(zbbuf), ZB_BUF_GET_ARRAY_PTR(zbbuf))

/**
 * @brief Put 1 byte into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 1-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT8(ptr, val) \
  ( *(ptr) = (val), (ptr)++ )

/**
 * @brief Put 2 bytes into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 2-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT16(ptr, val) \
  ( ZB_HTOLE16((ptr), (val)), (ptr) += 2 )


/**
 * @brief Put 4 bytes into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 4-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT32(ptr, val) \
  ( ZB_HTOLE32((ptr), (val)), (ptr) += 4 )


/**
 * @brief Number of bytes written to GPDF packet
 *
 * @param zbbuf [in] Buffer with GPDF
 * @param ptr   [in] Pointer to the GPDF tail
 */
#define ZB_GPDF_GET_BYTES_WRITTEN(zbbuf, ptr) \
  ((ptr) - ZB_BUF_GET_ARRAY_PTR(zbbuf))

/**
 * @brief Finish constructing GPDF frame
 *
 * @param zbbuf [in] Buffer with GPDF
 * @param ptr   [in] Pointer to the GPDF tail
 */
#define ZB_FINISH_GPDF_PACKET(zbbuf, ptr)                              \
  ZB_BUF_ALLOC_LEFT(zbbuf, ZB_GPDF_GET_BYTES_WRITTEN(zbbuf, ptr), ptr)


/**
 * @brief RX channel in the next attempt parameter of ZGPD Channel request command
 * @see ZGP spec, A.4.2.1.4
 */
#define ZB_GPDF_CHANNEL_REQ_NEXT_RX_CHANNEL(par) \
  ((par) & 0x0F)

/**
 * @brief ZGPD Commissioning command parameters
 * @see ZGP spec, A.4.2.1.1
 */
typedef struct zb_gpdf_comm_params_s
{
  zb_uint8_t     zgpd_device_id;            /**< ZGPD Device ID */
  zb_uint8_t     options;                   /**< Options */
  zb_uint8_t     ext_options;               /**< Extended options */
  zb_uint8_t     ms_extensions;             /**< MS extensions */
  zb_uint16_t    manuf_model_id;            /**< Manufacturer model ID */
  zb_uint16_t    manuf_id;                  /**< Manufacturer ID */
  /* TODO: Add fields "Number of GP commands", "GP command ID list",
   * "Number of cluster reports", "ClusterReportN" */
}
zb_gpdf_comm_params_t;

/**
 * @brief ZGPD Commissioning reply parameters
 * @see ZGP spec, A.4.2.1.1
 */
typedef struct zb_gpdf_comm_reply_s
{
  zb_uint8_t     options;                       /**< Options */
  zb_uint16_t    pan_id;                        /**< Pan ID if requested */
  zb_uint8_t     security_key[ZB_CCM_KEY_SIZE]; /**< ZGPD key */
  zb_uint8_t     key_mic[ZB_CCM_M];             /**< ZGPD key MIC */
  zb_uint32_t    frame_counter;                 /**< ZGPD key encryption counter */
}
zb_gpdf_comm_reply_t;

#define ZB_GPDF_COMM_REPLY_PAN_ID_PRESENT(options) ((options) & 0x01)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_PRESENT(options) (((options) >> 1) & 0x01)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_ENCRYPTED(options) (((options) >> 2) & 0x01)
#define ZB_GPDF_COMM_REPLY_SEC_LEVEL(options) (((options) >> 3) & 0x03)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_TYPE(options) (((options) >> 5) & 0x07)

/**
 * @brief Construct options field of commissioning command from given values
 * @see ZGP spec, A.4.2.1.1.2
 */
#define ZB_GPDF_COMM_OPT_FLD(sn_cap, rx_cap, ms_ext, pan_id_req, \
    sec_key_req, fixed_loc, ext_opt) \
  (   (sn_cap) \
   | ((rx_cap)      << 1) \
   | ((ms_ext)      << 2) \
   | ((pan_id_req)  << 4) \
   | ((sec_key_req) << 5) \
   | ((fixed_loc)   << 6) \
   | ((ext_opt)     << 7) )

/**
 * @brief Value of Extended Options bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_EXT_OPT_PRESENT(options) \
  ((options) >> 7)

/**
 * @brief Value of GP security Key request bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_KEY_REQ(options) \
  (((options) >> 5) & 0x01)

/**
 * @brief Value of PAN ID request bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_PAN_ID_REQ(options) \
  (((options) >> 4) & 0x01)

/**
 * @brief Value of MAC sequence number capability bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEQ_NUM_CAPS(options) \
  ((options) & 0x01)

/**
 * @brief Value of RxOnCapability bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_RX_CAPABILITY(options) \
  (((options) >> 1) & 0x01)

/**
 * @brief Value of Fixed location bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_FIX_LOC(options) \
  (((options) >> 6) & 0x01)

/**
 * @brief Value of GPD MS extensions present bit in
 * Options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_MS_EXT_PRESENT(options) \
  (((options) >> 2) & 0x01)

/**
 * @brief Construct Extended Options field of commissioning command
 * @see ZGP spec, A.4.2.1.1.3
 */
#define ZB_GPDF_COMM_EXT_OPT_FLD(sec_cap, key_type, key_present, \
    key_enc, out_counter) \
  (   (sec_cap) \
   | ((key_type)    << 2) \
   | ((key_present) << 5) \
   | ((key_enc)     << 6) \
   | ((out_counter) << 7))

/**
 * @brief Value of GPD Key present bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_KEY_PRESENT(ext_options) \
  (((ext_options) >> 5) & 0x01)

/**
 * @brief Value of GPD Key encryption bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_KEY_ENCRYPTED(ext_options) \
  (((ext_options) >> 6) & 0x01)

/**
 * @brief Value of GPD outgoing counter present bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_OUT_COUNTER_PRESENT(ext_options) \
  ((ext_options) >> 7)

/**
 * @brief Value of SecurityLevel capabilities bits in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_LEVEL_CAPS(ext_options) \
  ((ext_options) & 0x03)

/**
 * @brief Value of SecurityKey type bits in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_KEY_TYPE(ext_options) \
  ((ext_options >> 2) & 0x07)

/**
 * @brief Construct MS extensions field of commissioning command
 * @see docs-13-0146-04-batt-green-powermanufacturer-specific-device-type-command.docx
 */
#define ZB_GPDF_COMM_MS_EXT_FLD(_manuf_id, _manuf_model_id, _gp_cmd_list, _clstr_reports) \
  (   (_manuf_id) \
   | ((_manuf_model_id) << 1) \
   | ((_gp_cmd_list)    << 2) \
   | ((_clstr_reports)  << 3))

/**
 * @brief Value of "ManufacturerID present" bit in
 * MS extensions field of commissioning command
 */
#define ZB_GPDF_COMM_MSEXT_MANUF_ID_PRESENT(ms_ext) \
  ((ms_ext) & 0x01)

/**
 * @brief Value of "Manufacturer ModelID present" bit in
 * MS extensions field of commissioning command
 */
#define ZB_GPDF_COMM_MSEXT_MODEL_ID_PRESENT(ms_ext) \
  ((ms_ext >> 1) & 0x01)

/**
 * @brief Construct Options field of commissioning reply command
 * @see ZGP spec, A.4.2.1.2.1
 */
#define ZB_GPDF_COMM_REPLY_OPT_FLD(pan_id_present, key_present, \
    key_enc, sec_level, key_type) \
  (   (pan_id_present) \
   | ((key_present) << 1) \
   | ((key_enc)     << 2) \
   | ((sec_level)   << 3) \
   | ((key_type)    << 5))

/** @} */
/*! @endcond */


/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Send commissioning GPDF with ZGPD securityLevel capabilities set to 0
 */
#define ZB_SEND_COMMISSIONING_GPDF_WITHOUT_SEC(buf, device_id, sn_cap, rx_cap) \
{ \
  zb_uint8_t* ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_BUF_ALLOC_LEFT(buf, 3, ptr); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_CMD_COMMISSIONING); \
  ZB_GPDF_PUT_UINT8(ptr, device_id); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_COMM_OPT_FLD(sn_cap, rx_cap, 0, 0, 0, 0, 0)); \
  ZB_SEND_GPDF_CMD(ZB_REF_FROM_BUF(buf)); \
}

/**
 * @brief Attribute report field of attribute reporting command
 * @see ZGP spec, A.4.2.3.1
 */
typedef struct zb_gpdf_attr_report_fld_s
{
  zb_uint16_t attr_id;   /**< Attribute ID specific to cluster */
  zb_uint8_t attr_type;  /**< Attribute type (see @ref zb_zcl_attr_type_t) */
  zb_voidp_t data_p;     /**< Attribute data */
}
zb_gpdf_attr_report_fld_t;

/**
 * @brief Start constructing ZGPD attribute reporting command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param cluster_id [in]  Cluster ID of attributes being reported
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_INIT(buf, cluster_id, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_CMD_ATTR_REPORT); \
  ZB_GPDF_PUT_UINT16(ptr, &cluster_id); \
}

/**
 * @brief Put attribute report field into attribute reporting command
 *
 * Macro should be called only after attribute reporting command is
 * initialized with @ref ZB_ZGPD_ATTR_REPORTING_CMD_INIT
 * @param ptr   [in,out]  Pointer to the tail of attribute reporting command
 * @param attr  [in]      Attribute report field (see @zb_gpdf_attr_report_fld_t)
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_NEXT(ptr, attr) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &attr.attr_id); \
  ZB_GPDF_PUT_UINT8(ptr, attr.attr_type); \
  ptr = zb_zcl_put_value_to_packet(ptr, attr.attr_type, (zb_uint8_t *)attr.data_p); \
}

/**
 * @brief Finish constructing ZGPD attribute reporting command
 *
 * @param buf        [in] Buffer for GPDF command
 * @param ptr        [in] Pointer to the tail of GPDF
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_FINISH(buf, ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(buf, ptr);	\
}

/**
 * @brief Value of multi-record bit of options field
 *        in ZGPD Request attributes or Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_IS_MULTI_RECORD(opts) \
  (opts & 0x01)

/**
 * @brief Value of multi-record bit of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_WRITE_ATTR_IS_MULTI_RECORD \
  ZB_GPDF_REQUEST_ATTR_IS_MULTI_RECORD

/**
 * @brief Value of "manufacturer field present" bit of options field
 *        in ZGPD Request attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_MANUF_FIELD_PRESENT(opts) \
  ((opts >> 1) & 0x01)

/**
 * @brief Value of "manufacturer field present" bit of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_WRITE_ATTR_MANUF_FIELD_PRESENT \
  ZB_GPDF_REQUEST_ATTR_MANUF_FIELD_PRESENT

/**
 * @brief Construct value of options field
 *        in ZGPD Request attributes or Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_OPTIONS_FLD(multi_record, manuf_present) \
  ((multi_record) | (manuf_present << 1))

/**
 * @brief Construct value of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.3)
 */
#define ZB_GPDF_WRITE_ATTR_OPTIONS_FLD \
  ZB_GPDF_REQUEST_ATTR_OPTIONS_FLD

/**
 * @brief Parse one Cluster Record request field of
 * ZGPD Request attributes command
 * @param rec         pointer to record to parse of type zb_uint8_t*
 * @param cluster_id  Cluster ID (out)
 * @param attr_count  Attribute count (out)
 * @param attrs  Pointer to attributes list (out)
 */
#define ZB_GPDF_REQUEST_ATTR_PARSE_RECORD(rec, cluster_id, attr_count, attrs) \
{ \
  ZB_LETOH16((cluster_id), (rec)); \
  *(attr_count) = (rec)[2]/sizeof(zb_uint16_t); \
  *(attrs) = (zb_uint16_t *)&(rec)[3]; \
}

/**
 * @brief Start constructing ZGP write attribute command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param cluster_id [in]  Cluster ID of attributes being reported
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_INIT(buf, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_WRITE_ATTR_OPTIONS_FLD(0,0)); \
}

/**
 * @brief Start constructing ZGP write attribute command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param manuf_id   [in]  Manufacturer ID
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT(buf, manuf_id, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_WRITE_ATTR_OPTIONS_FLD(0,1)); \
  ZB_GPDF_PUT_UINT16(ptr, &(manuf_id)); \
}

/**
 * @brief Start cluster record field in write attributes command
 *
 * Macro should be called only after write attributes command is
 * initialized with @ref ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT or
 * @ref ZB_ZGPD_WRITE_ATTR_CMD_INIT
 * @param ptr             [in,out]  Pointer to the tail of write attributes command
 * @param cluster_id      [in]      Cluster ID
 * @param record_list_len [in]      Length in bytes of following Write Attribute Records list
 */
#define ZB_ZGPD_WRITE_ATTR_NEW_CLUSTER_RECORD(ptr, cluster_id, record_list_len) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &cluster_id); \
  ZB_GPDF_PUT_UINT8(ptr, (record_list_len)); \
}

/**
 * @brief Put write attribute record into write attributes command
 *
 * Macro should be called only after new cluster record is
 * initialized with @ref ZB_ZGPD_WRITE_ATTR_NEXT_CLUSTER_RECORD
 * @param ptr       [in,out]  Pointer to the tail of write attributes command
 * @param attr_id   [in]      Attribute ID
 * @param attr_type [in]      Attribute type
 * @param value     [in]      Pointer to attribute value
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_NEXT_ATTR(ptr, attr_id, attr_type, value) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &attr_id); \
  ZB_GPDF_PUT_UINT8(ptr, attr_type); \
  ptr = zb_zcl_put_value_to_packet(ptr, attr_type, (zb_uint8_t *)(value)); \
}

/**
 * @brief Finish constructing ZGPD write attributes command
 *
 * @param buf        [in] Buffer for GPDF command
 * @param ptr        [in] Pointer to the tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_FINISH(buf, ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(buf, ptr);	\
}

/* [AV] tmm it'll be better to name macros like this once
   and don't rename them like #def macro_write_init(...) macro_read_init(...),
   because there is no difference
*/
#define ZB_ZGPD_CLUSTER_CMD_INIT(_buf, _ptr) \
  ZB_ZGPD_WRITE_ATTR_CMD_INIT(_buf, _ptr)
#define ZB_ZGPD_CLUSTER_CMD_MANUF_INIT(_buf, _manuf_id, _ptr)	\
  ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT(_buf, _manuf_id, _ptr)
#define ZB_ZGPD_NEW_CLUSTER_RECORD(_ptr, _cluster_id, _record_list_len) \
  ZB_ZGPD_WRITE_ATTR_NEW_CLUSTER_RECORD(_ptr, _cluster_id, _record_list_len)
#define ZB_ZGPD_READ_ATTR_CMD_NEXT_ATTR(_ptr, _attr_id) \
{ \
  ZB_GPDF_PUT_UINT16(_ptr, &_attr_id); \
}
#define ZB_ZGPD_CLUSTER_CMD_FINISH(_buf, _ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(buf, ptr); \
}

typedef struct zb_zgps_send_cmd_params_s
{
  zb_uint8_t     cmd_id;
  zb_zgpd_id_t   zgpd_id;
  zb_ieee_addr_t ieee_addr;
  zb_time_t      lifetime;
  zb_uint8_t     tx_options;
  zb_uint8_t     handle;
}
zb_zgps_send_cmd_params_t;

/**
 * @brief Send provided packet to ZGPD
 *
 * Buffer data is command payload to send.
 * Other parameters are in the buffer tail (see @ref zb_zgps_send_cmd_params_t).
 *
 * @param param[in, out]    Reference to buffer.
 *
 * @note maximum length of data payload is @ref ZB_ZGP_TX_CMD_PLD_MAX_SIZE
 *
 * @note zb_gp_data_cfm is called from:
 *  - gp_data_req_send_cnf       to notify about status of adding data to tx_packet_info_queue;
 *  - notify_about_expired_entry to notify about expired entry;
 *  - zb_cgp_data_cfm            to notify about status from MAC layer.
 *
 * @note Status of confirm (ZGP TX queue is used) can be:
 *      ZB_ZGP_STATUS_ENTRY_REPLACED
 *      ZB_ZGP_STATUS_ENTRY_ADDED
 *      ZB_ZGP_STATUS_ENTRY_EXPIRED
 *      ZB_ZGP_STATUS_ENTRY_REMOVED
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *
 * @note Status of confirm (ZGP TX queue is not used) can be:
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *      MAC_NO_ACK
 *
 */
void zb_zgps_send_data(zb_uint8_t param);

/*! @}
 *  @endcond */

/**
   @addtogroup zgp_sink
   @{
*/

/**
 * @brief Put ZGPS into commissioning mode
 *
 * It is safe to call this function when device is already in
 * commissioning mode. In this case function does nothing.
 *
 * @param timeout [in]  Maximum commissioning time in beacon intervals. \n
 *                      0 means no timeout. \n
 *                      If timeout occurs, then result of commissioning is
 *                      @ref ZB_ZGP_COMMISSIONING_TIMED_OUT
 *
 * @snippet tests/zgp/gppb/simple_combo/zc_combo.c start_comm
 */
void zb_zgps_start_commissioning(zb_time_t timeout);

/**
 * @brief Enable or disable bidirectional commissioning
 *
 * @param enable [in]  If ZB_TRUE, enable bidirectional commissioning, else -
 *                      disable and drop bidirectional frames.
 */
void zb_zgps_switch_bidir_commissioning(zb_bool_t enable);

/**
 * @brief Switch ZGPS back to operational mode from commissioning
 *
 * After commissioning is cancelled, user is notified with
 * @ref zb_zgp_comm_completed_cb_t with ZB_ZGP_COMMISSIONING_CANCELLED_BY_USER
 * status.
 *
 * @snippet tests/zgp/gppb/simple_combo/zc_combo.c stop_comm
 */
void zb_zgps_stop_commissioning();

/**
 * @brief Accept/reject ZGPD commissioning attempt
 *
 * This function should be called as an answer to commissioning request made by
 * stack via @ref zb_zgp_comm_req_cb_t. Also, it can be called from @ref
 * zb_zgp_comm_req_cb_t callback as well as outside it.
 *
 * @param[in] accept - If ZB_TRUE, then stack will continue ongoing commissioning
 *                     process with ZGPD \n
 *                     Otherwise ongoing commissioning process will be
 *                     terminated
 * @snippet tests/zgp/gppb/test_gps_decommissioning/dut_gps.c accept_comm
 */
void zb_zgps_accept_commissioning(zb_bool_t accept);

/**
 * @brief Remove all the information about ZGPD from stack
 *
 * In ZGP there is no way to say ZGPD to leave the network.
 * ZGPD can leave network by itself using "Decommissioning" command,
 * but ZGPS can miss this command if it was not in commissioning state.
 *
 * This function removes all the information related to specified ZGPD
 * from stack.
 *
 * @param buf_ref  reference to the free buffer
 * @param zgpd_id  identifier of ZGPD to be removed
 *
 * @note It is safe to free or overwrite memory pointed by zgpd_id
 *       after call
 */
void zb_zgps_delete_zgpd(zb_uint8_t buf_ref, zb_zgpd_id_t *zgpd_id);

/**
 * @brief Remove all the information about ALL ZGPD from stack
 *
 */
void zb_zgps_delete_all_zgpd(void);

/**
   Get LQI and RSSI last time received from that ZGPD.
 */
void zb_zgps_get_diag_data(zb_zgpd_id_t *zgpd_id, zb_uint8_t *lqi, zb_int8_t *rssi);

/*! @} */

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/* TODO: move all internals out of that .h file */

void zb_zgp_init();

void zb_gp_mcps_data_indication(zb_uint8_t param);
void zb_cgp_data_cfm(zb_uint8_t param);

#ifdef ZB_ENABLE_ZGP_CLUSTER
void zgp_init_by_scheduler(zb_uint8_t param);
#endif

#ifdef ZGP_CLUSTER_TEST
typedef zb_uint8_t (*zgp_cluster_app_zcl_cmd_handler_t)(zb_uint8_t buf_ref);
zb_void_t zgp_cluster_set_app_zcl_cmd_handler(zgp_cluster_app_zcl_cmd_handler_t handler);
#endif

zb_bool_t zb_zgp_will_skip_all_packets();

/*! @} */
/*! @endcond */


/**
   @addtogroup zgp_sink
   @{
*/

#if defined ZB_ENABLE_ZGP_CLUSTER || defined DOXYGEN
/**
   Fill security level constant to be passed to zb_zgps_set_security_level()

   Described in A.3.3.2.6 gpsSecurityLevel attribute.

   @param sec_lvl  @ref zb_zgp_security_level_e Minimal GPD Security Level sub-field contains the minimum gpdSecurityLevel this sink accepts
   @param with_link_key 1 bit - Protection with the gpLinkKey sub-field, indicates if
   the GPDs attempting the pairing are required to support protecting the
   over-the-air exchange of the GPD Key
   @param involve_tc always zero for the current GPPB specification

   @snippet tests/zgp/gppb/simple_combo/zc_combo.c set_secur_level
 */
#define ZB_ZGP_FILL_GPS_SECURITY_LEVEL(sec_lvl, with_link_key, involve_tc)\
  (((sec_lvl) & 3) | ((!!(with_link_key)) << 2) | ((!!(involve_tc)) << 3))
#else
#define ZB_ZGP_FILL_GPS_SECURITY_LEVEL(sec_lvl)\
  ((sec_lvl) & 3)
#endif


/**
   Set gpsSecurityLevel GP cluster attribute of gpcb

   Described in A.3.3.2.6 gpsSecurityLevel attribute.

   @param level Security level to set

   @snippet tests/zgp/gppb/simple_combo/zc_combo.c set_secur_level
  */
void zb_zgps_set_security_level(zb_uint_t level);


/**
   Set gpsCommunicationMode GP cluster attribute of gpcb

   Described in A.3.3.2.3 gpsCommunicationMode attribute

   @param mode @ref zgp_communication_mode_t communication mode

   @snippet tests/zgp/gppb/simple_combo/zc_combo.c set_comm_mode
  */
void zb_zgps_set_communication_mode(zgp_communication_mode_t mode);

/**
   Select GP infrastructure device role at runtime.

   This function can be used if ZBOSS compiled with both GPPB and GBCP roles
   support.
   N/A if ZBOSS compiled with GPPB support only.

   @param device_role - device role (see @ref zgp_gp_device_t).
   ZGP_DEVICE_COMBO_BASIC or ZGP_DEVICE_PROXY_BASIC are allowed.

   @snippet tests/zgp/gppb/simple_combo/zc_combo.c set_dev_role
  */
zb_void_t zb_set_zgp_device_role(zb_uint16_t device_role);


/**
   Set ZBOSS to skip all incloming GPDF.

   To be used for testing only.
   Use that function with ZB_TRUE parameter to prevent Combo device from
   receiving GPDFS thus always working thru Proxy device.

   @param skip if ZB_TRUE, skip incoming GP frames

   @snippet tests/zgp/gppb/simple_combo/zc_combo.c set_skip_gpdf
  */
void zb_zgp_set_skip_gpfd(zb_bool_t skip);

/**
   Check that ZBOSS skips all incloming GPDFS

   @return ZB_TRUE if incoming GPDFS are skipped.
*/
zb_bool_t zb_zgp_get_skip_gpfd();


/*! @} */

#endif /* ZBOSS_API_ZGP_H */
