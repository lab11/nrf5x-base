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
PURPOSE: Zigbee cluster library types and macros common for all clusters
*/

#if ! defined ZB_ZCL_COMMON_H
#define ZB_ZCL_COMMON_H

#include <zboss_api_core.h> /* int types, zb_buf_t */

/** @cond DOXYGEN_ZCL_SECTION */

#ifdef DEBUG
/* If defined, samples included to zcl source files will be comiled - need for development only */
#define ZB_COMPILE_ZCL_SAMPLE
#endif


/** @addtogroup ZB_ZCL_COMMON
 *  @{
 */

/**
 *  @defgroup zb_zcl_common_types ZCL common types
 *  @{
 *    @details
 *    The section contains enumerations defining cluster identifiers and inter-device operation
 *    statuses, and a callback type usesd to notify application on attribute value change (see @ref
 *    write_attr_cmd "Write attributes" command description).
 */

/*************************** ZCL cluster definitions ***********************************/

/*! ZCL payload maximum size; is used to calculate number of
 *  available bytes in variable-size commands */
/* TODO: current max size is defined for the worst case (nwk security
 * enabled, long address is specified). Possibly - recalculate this
 * max size in run-time taking into account current parameters */
/* TODO: check this value - calculate it correctly!!! */
/*
  Let's count:
- MAC hdr (w/o panid complession, short src/dest) - 11b + MAC FCS 2b + len 1b = 14b MAC
- NWK HDR 8 + 8*2 (addresses). Note: not use source routing, else have more fields.
- NWK security 18b
Total 56 (40 without ieee addresses).
So, rest is 128-56 = 72b
So, rest is 128-40 = 88b
 */
#define ZB_ZCL_MAX_PAYLOAD_SIZE 48
#define ZB_ZCL_HI_MAX_PAYLOAD_SIZE 72
#define ZB_ZCL_HI_WO_IEEE_MAX_PAYLOAD_SIZE 88

struct zb_zcl_attr_s; /* Forward declaration */

typedef zb_void_t (*zb_zcl_cluster_init_t)(void);
typedef zb_bool_t (*zb_zcl_cluster_handler_t)(zb_uint8_t param);
typedef zb_bool_t (*zb_zcl_cluster_check_value_t)(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
typedef zb_void_t (*zb_zcl_cluster_write_attr_hook_t)(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value);

/** @brief ZCL cluster description. */
typedef ZB_PACKED_PRE struct zb_zcl_cluster_desc_s
{
  zb_uint16_t cluster_id; /**< ZCL 16 bit cluster id. @see zb_zcl_cluster_id_e */
  zb_uint16_t attr_count;  /**< Attributes number supported by the cluster */
  struct zb_zcl_attr_s *attr_desc_list; /**< List of cluster attributes,
         declared using ZB_ZCL_DECLARE_<CLUSTER_NAME>_ATTRIB_LIST() macro */
  zb_uint8_t role_mask;   /**< Cluster roile, refer to zb_zcl_cluster_role_e */
  zb_uint16_t manuf_code; /**< Manufacturer code for cluster and its attributes */
  zb_zcl_cluster_init_t cluster_init;
} ZB_PACKED_STRUCT
zb_zcl_cluster_desc_t;

zb_ret_t zb_zcl_add_cluster_handlers(zb_uint16_t cluster_id, zb_uint8_t cluster_role,
  zb_zcl_cluster_check_value_t cluster_check_value,
  zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook,
  zb_zcl_cluster_handler_t cluster_handler);
zb_zcl_cluster_handler_t zb_zcl_get_cluster_handler(zb_uint16_t cluster_id, zb_uint8_t cluster_role);
zb_zcl_cluster_check_value_t zb_zcl_get_cluster_check_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role);
zb_zcl_cluster_write_attr_hook_t zb_zcl_get_cluster_write_attr_hook(zb_uint16_t cluster_id, zb_uint8_t cluster_role);

/** @brief Cluster roles in message exchange */
enum zb_zcl_cluster_role_e
{
  /*! Server cluster role */
  ZB_ZCL_CLUSTER_SERVER_ROLE = 0x01,
  /*! Client cluster role */
  ZB_ZCL_CLUSTER_CLIENT_ROLE = 0x02,
  /*! Internal value - used for searching any role (on the moment when cluster role is not known
   * yet). */
  ZB_ZCL_CLUSTER_ANY_ROLE    = 0x03
};

/** @brief ZCL cluster identifiers
  * @see ZCL spec, subclause 2.2.2.
  */
typedef enum zb_zcl_cluster_id_e
{
  ZB_ZCL_CLUSTER_ID_BASIC = 0x0000,                 /**< Basic cluster identifier. */
  ZB_ZCL_CLUSTER_ID_POWER_CONFIG = 0x0001,          /**< Power configuration cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DEVICE_TEMP_CONFIG = 0x0002,    /**< Device temperature configuration cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_IDENTIFY = 0x0003,              /**< Identify cluster identifier. */
  ZB_ZCL_CLUSTER_ID_GROUPS = 0x0004,                /**< Groups cluster identifier. */
  ZB_ZCL_CLUSTER_ID_SCENES = 0x0005,                /**< Scenes cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ON_OFF = 0x0006,                /**< On/Off cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG = 0x0007,  /**< On/Off switch configuration cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL = 0x0008,         /**< Level control cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ALARMS = 0x0009,                /**< Alarms cluster identifier. */
  ZB_ZCL_CLUSTER_ID_TIME = 0x000a,                  /**< Time cluster identifier. */
  ZB_ZCL_CLUSTER_ID_RSSI_LOCATION = 0x000b,         /**< RSSI location cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ANALOG_INPUT = 0x000c,          /**< Analog input (basic) cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT = 0x000d,         /**< Analog output (basic) cluster identifier.
                                                      */
  ZB_ZCL_CLUSTER_ID_ANALOG_VALUE = 0x000e,          /**< Analog value (basic) cluster identifier. */
  ZB_ZCL_CLUSTER_ID_BINARY_INPUT = 0x000f,          /**< Binary input (basic) cluster identifier. */
  ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT = 0x0010,         /**< Binary output (basic) cluster identifier.
                                                      */
  ZB_ZCL_CLUSTER_ID_BINARY_VALUE = 0x0011,          /**< Binary value (basic) cluster identifier. */
  ZB_ZCL_CLUSTER_ID_MULTI_INPUT = 0x0012,           /**< Multistate input (basic) cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_MULTI_OUTPUT = 0x0013,          /**< Multistate output (basic) cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_MULTI_VALUE = 0x0014,           /**< Multistate value (basic) cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_COMMISSIONING = 0x0015,          /**< Commissioning cluster identifier. */

  ZB_ZCL_CLUSTER_ID_OTA_UPGRADE = 0x0019,          /**< Over The Air cluster identifier. */

  /* General clusters */
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL = 0x0020,          /**< Poll control cluster identifier. */
  ZB_ZCL_CLUSTER_ID_GREEN_POWER  = 0x0021,          /**< Green Power cluster identifier. */

  ZB_ZCL_CLUSTER_ID_KEEP_ALIVE = 0x0025,            /**< Keep Alive cluster identifier. */

  /* Closures clusters */
  ZB_ZCL_CLUSTER_ID_SHADE_CONFIG = 0x0100,          /**< Shade configuration cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DOOR_LOCK = 0x0101,             /**< Door lock cluster identifier. */
  ZB_ZCL_CLUSTER_ID_WINDOW_COVERING = 0x0102,       /**< Window covering cluster identifier. */
  /* HVAC clusters */
  ZB_ZCL_CLUSTER_ID_PUMP_CONFIG_CONTROL = 0x200,    /**< Pump configuration and control cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_THERMOSTAT = 0x201,             /**< Thermostat cluster identifier. */
  ZB_ZCL_CLUSTER_ID_FAN_CONTROL = 0x202,            /**< Fan control cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DEHUMID_CONTROL = 0x203,        /**< Dehumidification control cluster
                                                      identifier. */
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_UI_CONFIG = 0x204,   /**< Thermostat user interface configuration
                                                      cluster identifier. */

  /* Lighting clusters */
  ZB_ZCL_CLUSTER_ID_COLOR_CONTROL = 0x0300,         /**< Color control cluster identifier. */
  ZB_ZCL_CLUSTER_ID_BALLAST_CONFIG = 0x0301,        /**< Ballast configuration cluster identifier. */

  /* Measurement and Sensing */
  ZB_ZCL_CLUSTER_ID_ILLUMINANCE_MEASUREMENT = 0x0400,   /**< Illuminance measurement */
  ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT = 0x0402,          /**< Temperature measurement */
  ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT = 0x0403,      /**< Pressure measurement */
  ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT = 0x0405,  /**< Relative humidity measurement */
  ZB_ZCL_CLUSTER_ID_OCCUPANCY_SENSING = 0x0406,  /**< Occupancy sensing */

  /* IAS clusters */
  ZB_ZCL_CLUSTER_ID_IAS_ZONE = 0x0500,              /**< IAS Zone cluster identifier */
  ZB_ZCL_CLUSTER_ID_IAS_ACE = 0x0501,               /**< IAS ACE cluster identifier */
  ZB_ZCL_CLUSTER_ID_IAS_WD = 0x0502,                /**< IAS WD cluster identifier */

  /* Smart Energy */
  ZB_ZCL_CLUSTER_ID_PRICE = 0x0700,        	       	 /**< Price cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DRLC = 0x0701,                   /**< Demand-Response cluster identifier. */
  ZB_ZCL_CLUSTER_ID_METERING = 0x0702,               /**< Metering cluster identifier. */
  ZB_ZCL_CLUSTER_ID_MESSAGING = 0x0703,              /**< Messaging cluster identifier. */
  ZB_ZCL_CLUSTER_ID_TUNNELING = 0x0704,			 /**< Tunneling cluster identifier. */
  ZB_ZCL_CLUSTER_ID_PREPAYMENT = 0x0705,			 /**< Prepayment cluster identifier. */
  ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT = 0x0706,      /**< Energy Management cluster identifier. */
  ZB_ZCL_CLUSTER_ID_CALENDAR = 0x0707,               /**< Calendar cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DEVICE_MANAGEMENT = 0x0708,      /**< Device Management cluster identifier. */
  ZB_ZCL_CLUSTER_ID_EVENTS = 0x0709,                 /**< Events cluster identifier. */
  ZB_ZCL_CLUSTER_ID_MDU_PAIRING = 0x070A,            /**< MDU Pairing cluster identifier. */
  ZB_ZCL_CLUSTER_ID_SUB_GHZ = 0x070B,                /**< Sub-GHz cluster identifier. */
  ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT = 0x0800,      /**< Key Establishment cluster identifier. */


  /* Home Automation */
  ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS = 0x0b02, /**< Appliance events
                                                               and alerts cluster identifier */
  /* HA spec, rev. 29, ver. 1.2 - Electricity Measurement, not sure that its Electrical Measurement */
  ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT = 0x0b04, /**< Electrical Measurement cluster identifier. */
  ZB_ZCL_CLUSTER_ID_DIAGNOSTICS = 0x0b05,           /**< Home Automation Diagnostics */

  /** @cond touchlink */
  ZB_ZCL_CLUSTER_ID_TOUCHLINK_COMMISSIONING = 0x1000,  /**< Touchlink Commissioning cluster identifier */
  /** @endcond */ /* touchlink */

  /************** Manufacturer specific clusters ****************/

  ZB_ZCL_CLUSTER_ID_TUNNEL      = 0xfc00,    /**< Manufacturer specific Tunnel cluster */

  ZB_ZCL_CLUSTER_ID_CUSTOM_ATTR = 0xffee,    /**< Custom Attributes */

  ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION = 0x0B01,  /**< Meter Identification cluster identifier */
  /* TODO: define other clusters IDs */
  /* Measurements and sensing clusters */
  /* Security and safety clusters */
} zb_zcl_cluster_id_t;


/** @brief Check whether cluster is manufacturer specific. */
#define ZB_ZCL_IS_CLUSTER_MANUF_SPEC(_cluster_id) ((_cluster_id) > 0xfbff)

/** @brief Internal invalid manufacturer code value. */
#define ZB_ZCL_MANUF_CODE_INVALID 0x0000


/** @brief Parse frame operation status. */
typedef enum zb_zcl_parse_status_e
{
  ZB_ZCL_PARSE_STATUS_SUCCESS,  /**< Frame parsed successfully. */
  ZB_ZCL_PARSE_STATUS_FAILURE   /**< Frame parse operation failed. */
} zb_zcl_parse_status_t;

/** @brief Attribute Reporting Status. */
typedef enum zb_zcl_attribute_reporting_status_e
{
  ZB_ZCL_AR_STATUS_PENDING	=0x00,  	/**< Pending. */
  ZB_ZCL_AR_STATUS_COMPLETE =0x01 	 	/**< Attribute Reporting Complete. */
} zb_zcl_attribute_reporting_status_t;

/** @brief ZCL status values
    @see ZCL spec, subclause 2.5.3
    @note The enumeration does not contain statuses not used in ZB HA profile
*/
typedef enum zb_zcl_status_e
{
  ZB_ZCL_STATUS_SUCCESS                  = 0x00, /*!< ZCL Success */
  ZB_ZCL_STATUS_FAIL                     = 0x01, /*!< ZCL Fail */
  ZB_ZCL_STATUS_NOT_AUTHORIZED           = 0x7E, /*!< Server is not authorized to upgrade the client */
  ZB_ZCL_STATUS_MALFORMED_CMD            = 0x80, /*!< Malformed command */
  ZB_ZCL_STATUS_UNSUP_CLUST_CMD          = 0x81, /*!< Unsupported cluster command */
  ZB_ZCL_STATUS_UNSUP_GEN_CMD            = 0x82, /*!< Unsupported general command */
  ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD    = 0x83, /*!< Unsupported manuf-specific clust command */
  ZB_ZCL_STATUS_UNSUP_MANUF_GEN_CMD      = 0x84, /*!< Unsupported manuf-specific general command */
  ZB_ZCL_STATUS_INVALID_FIELD            = 0x85, /*!< Invalid field */
  ZB_ZCL_STATUS_UNSUP_ATTRIB             = 0x86, /*!< Unsupported attribute */
  ZB_ZCL_STATUS_INVALID_VALUE            = 0x87, /*!< Invalid value */
  ZB_ZCL_STATUS_READ_ONLY                = 0x88, /*!< Read only */
  ZB_ZCL_STATUS_INSUFF_SPACE             = 0x89, /*!< Insufficient space */
  ZB_ZCL_STATUS_DUPE_EXISTS              = 0x8a, /*!< Duplicate exists */
  ZB_ZCL_STATUS_NOT_FOUND                = 0x8b, /*!< Not found */
  ZB_ZCL_STATUS_UNREPORTABLE_ATTRIB      = 0x8c, /*!< Unreportable attribute */
  ZB_ZCL_STATUS_INVALID_TYPE             = 0x8d, /*!< Invalid type */
  ZB_ZCL_STATUS_WRITE_ONLY               = 0x8f, /*!< Write only */
  ZB_ZCL_STATUS_INCONSISTENT             = 0x92, /*!< Supplied values are inconsistent */
  ZB_ZCL_STATUS_ACTION_DENIED            = 0x93, /*!< The credentials presented by the device sending the
                                                       command are not sufficient to perform this action. */
  ZB_ZCL_STATUS_TIMEOUT                  = 0x94, /*!< Timeout */
  ZB_ZCL_STATUS_ABORT                    = 0x95, /*!< Abort */

  ZB_ZCL_STATUS_INVALID_IMAGE            = 0x96, /*!< Invalid OTA upgrade image */
  ZB_ZCL_STATUS_WAIT_FOR_DATA            = 0x97, /*!< Server does not have data block available yet */
  ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE       = 0x98, /*!< No OTA upgrade image available for a particular client. */
  ZB_ZCL_STATUS_REQUIRE_MORE_IMAGE       = 0x99, /*!< The client still requires more OTA upgrade image files
                                                       in order to successfully upgrade. */
  ZB_ZCL_STATUS_NOTIFICATION_PENDING     = 0x9A, /*!< The command has been received and is being processed.*/
  ZB_ZCL_STATUS_HW_FAIL                  = 0xc0, /*!< Hardware failure */
  ZB_ZCL_STATUS_SW_FAIL                  = 0xc1, /*!< Software failure */
  ZB_ZCL_STATUS_CALIB_ERR                = 0xc2, /*!< Calibration error */
  ZB_ZCL_STATUS_UNSUP_CLUST              = 0xc3, /*!< Cluster is not found on the target endpoint */
} zb_zcl_status_t;

/** @brief ZCL global attibute: cluster version returned by default. 
    Used if the GLOBAL_CLUSTER_REVISION attribute is undefined for the cluster/role.
*/
#define ZB_ZCL_GLOBAL_CLUSTER_VERSION_DEFAULT 1

#if defined ZB_ENABLE_HA

/** @brief Discovery manufacture specific command
    @param direction - direction 0 (client to server) or 1 (server to client)
    @param ep - endpoint number
    @param cluster_id - cluster ID
    @param manuf_id - manufacture ID
    @param recv_cmd_type - need list of received (ZB_TRUE) or generated (ZB_FALSE) command
    @param value - (out) pointer to list command ID

    @return number of value elements
*/
typedef zb_uint8_t (*zb_zcl_disc_manuf_cmd_cb_t)(
    zb_uint8_t direction, zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint16_t manuf_id, zb_bool_t recv_cmd_type, zb_uint8_t **value);

#endif /* ZB_ENABLE_HA */

/********************************** ZCL attributes definitions ************************************/

/** @brief Informs application that some attribute value is being updated by Write attribute command
    @param ep - endpoint number
    @param cluster_id - cluster ID
    @param attr_id - attribute ID of the attribute being updated
    @param value - pointer to new data
*/
typedef void (*zb_zcl_modify_attr_value_cb_t)(
    zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint16_t attr_id, zb_uint8_t *value);

/*! @defgroup zb_zcl_common_attrs General attributes' description
    Types and macros shared among all attributes' definitions.
    @{
    @par Example
    Use ZB_ZCL_SET_ATTRIBUTE
    @snippet scenes_dut_563.c ZB_ZCL_SET_ATTRIBUTE
    @par
    For more information see 5.6.3_scenes and other HA samples
*/

/** @brief Sets correctly ZCL char string value */
#define ZB_ZCL_SET_STRING_VAL(str, val, len)                                \
  (ZB_MEMCPY((zb_uint8_t*)(str) + 1, (val), (len)), *(zb_uint8_t*)(str) = (len))

/** @brief Get ZCL string length */
#define ZB_ZCL_GET_STRING_LENGTH(str) ((str)[0])

#define ZB_ZCL_SET_STRING_LENGTH(str, _newlen) (((str))[0] = _newlen)

/** @brief Returns size of a string constant */
#define ZB_ZCL_STRING_CONST_SIZE(str) (zb_uint8_t)(sizeof(str) - 1)

/** Get pointer to first char in the string. */
#define ZB_ZCL_GET_STRING_BEGIN(_str) (&((_str)[1]))

/** Get pointer to (begin + length) char in the string. */
#define ZB_ZCL_GET_STRING_END(_str) (ZB_ZCL_GET_STRING_BEGIN(_str) + ZB_ZCL_GET_STRING_LENGTH(_str))

/** Get pointer to char in string at specified pos. */
#define ZB_ZCL_GET_STRING_REF_AT(_str, _pos) (ZB_ZCL_GET_STRING_BEGIN(_str) + _pos)

/** Get char by specified pos. */
#define ZB_ZCL_GET_STRING_CHAR_AT(_str, _pos) *ZB_ZCL_GET_STRING_REF_AT(_str, _pos)

/** Turn ZCL Octet string into string with null-term. */
#define ZB_ZCL_STRING_TO_C_STRING(_str)  \
{                                        \
  zb_uint8_t _len = _str[0];             \
  ZB_MEMMOVE(_str, _str + 1, _len);      \
  _str[_len] = 0;                        \
}

/** Set size of ZCL Octet String into 0 .*/
#define ZB_ZCL_STRING_CLEAR(_str) (_str[0] = 0)

/** Add some data to ZCL Octet String. */
static ZB_INLINE zb_uint16_t zb_zcl_string_append_data(zb_uint8_t *zcl_str,
                                                    zb_uint8_t zcl_str_max_size,
                                                    const zb_uint8_t *c_str,
                                                    zb_uint8_t c_str_len)
{
  zb_uint16_t newlen = ZB_ZCL_GET_STRING_LENGTH(zcl_str) + c_str_len;

  if (newlen < (zb_uint16_t) zcl_str_max_size)
  {
    ZB_MEMCPY(ZB_ZCL_GET_STRING_END(zcl_str), c_str, c_str_len);
    ZB_ZCL_SET_STRING_LENGTH(zcl_str, newlen);
  }
  else
  {
    newlen = 0;
  }

  return newlen;
}

/** Add signle byte to ZCL Octet String. */
static ZB_INLINE zb_uint16_t zb_zcl_string_append_byte(zb_uint8_t *zcl_str,
                                                    zb_uint8_t zcl_str_max_size,
                                                    zb_uint8_t value)
{
  zb_uint16_t newlen = ZB_ZCL_GET_STRING_LENGTH(zcl_str) + 1;

  if (newlen < (zb_uint16_t) zcl_str_max_size)
  {
    *ZB_ZCL_GET_STRING_END(zcl_str) = value;
    ZB_ZCL_SET_STRING_LENGTH(zcl_str, newlen);
  }
  else
  {
    newlen = 0;
  }

  return newlen;
}

/** Append C-string to ZCL string. */
#define ZB_ZCL_STRING_APPEND_C_STR(_zcl_str, _zcl_str_max_size, _c_str) \
  zb_zcl_string_append_data(_zcl_str, _zcl_str_max_size, (const zb_uint8_t *) _c_str, strlen(_c_str))

/** Append C-string to ZCL static string. */
#define ZB_ZCL_STATIC_STRING_APPEND_C_STR(_zcl_s_str, _c_str) \
  ZB_ZCL_STRING_APPEND_C_STR((zb_uint8_t *) _zcl_s_str, sizeof(_zcl_s_str), _c_str)

/** Append char to ZCL static string. */
#define ZB_ZCL_STATIC_STRING_APPEND_CHAR(_zcl_s_str, _char) \
  zb_zcl_string_append_byte(_zcl_s_str, sizeof(_zcl_s_str), (zb_uint8_t) _char)


/** @brief ZCL attribute data type values
    @note The enumeration is not full, and does not contain ZCL types not used in HA profile.
    @see ZCL spec., subclause 2.5.2, Table 2-10. Data Types
*/
typedef enum zb_zcl_attr_type_e
{
  ZB_ZCL_ATTR_TYPE_NULL           = 0x00, /*!< Null data type */
  ZB_ZCL_ATTR_TYPE_8BIT           = 0x08, /*!< 8-bit value data type */
  ZB_ZCL_ATTR_TYPE_16BIT          = 0x09, /*!< 16-bit value data type */
  ZB_ZCL_ATTR_TYPE_24BIT          = 0x0a, /*!< 24-bit value data type */
  ZB_ZCL_ATTR_TYPE_32BIT          = 0x0b, /*!< 32-bit value data type */
  ZB_ZCL_ATTR_TYPE_40BIT          = 0x0c, /*!< 40-bit value data type */
  ZB_ZCL_ATTR_TYPE_48BIT          = 0x0d, /*!< 48-bit value data type */
  ZB_ZCL_ATTR_TYPE_56BIT          = 0x0e, /*!< 56-bit value data type */
  ZB_ZCL_ATTR_TYPE_64BIT          = 0x0f, /*!< 64-bit value data type */
  ZB_ZCL_ATTR_TYPE_BOOL           = 0x10, /*!< Boolean data type */
  ZB_ZCL_ATTR_TYPE_8BITMAP        = 0x18, /*!< 8-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_16BITMAP       = 0x19, /*!< 16-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_24BITMAP       = 0x1a, /*!< 24-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_32BITMAP       = 0x1b, /*!< 32-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_40BITMAP       = 0x1c, /*!< 40-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_48BITMAP       = 0x1d, /*!< 48-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_56BITMAP       = 0x1e, /*!< 56-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_64BITMAP       = 0x1f, /*!< 64-bit bitmap data type */
  ZB_ZCL_ATTR_TYPE_U8             = 0x20, /*!< Unsigned 8-bit value data type */
  ZB_ZCL_ATTR_TYPE_U16            = 0x21, /*!< Unsigned 16-bit value data type */
  ZB_ZCL_ATTR_TYPE_U24            = 0x22, /*!< Unsigned 24-bit value data type */
  ZB_ZCL_ATTR_TYPE_U32            = 0x23, /*!< Unsigned 32-bit value data type */
  ZB_ZCL_ATTR_TYPE_U40            = 0x24, /*!< Signed 40-bit value data type */
  ZB_ZCL_ATTR_TYPE_U48            = 0x25, /*!< Unsigned 48-bit value data type */
  ZB_ZCL_ATTR_TYPE_U56            = 0x26, /*!< Unsigned 56-bit value data type */
  ZB_ZCL_ATTR_TYPE_U64            = 0x27, /*!< Unsigned 64-bit value data type */
  ZB_ZCL_ATTR_TYPE_S8             = 0x28, /*!< Signed 8-bit value data type */
  ZB_ZCL_ATTR_TYPE_S16            = 0x29, /*!< Signed 16-bit value data type */
  ZB_ZCL_ATTR_TYPE_S24            = 0x2a, /*!< Signed 24-bit value data type */
  ZB_ZCL_ATTR_TYPE_S32            = 0x2b, /*!< Signed 32-bit value data type */
  ZB_ZCL_ATTR_TYPE_S48            = 0x2d, /*!< Signed 48-bit value data type */
  ZB_ZCL_ATTR_TYPE_S56            = 0x2e, /*!< Signed 56-bit value data type */
  ZB_ZCL_ATTR_TYPE_S64            = 0x2f, /*!< Signed 64-bit value data type */
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM      = 0x30, /*!< 8-bit enumeration (U8 discrete) data type */
  ZB_ZCL_ATTR_TYPE_16BIT_ENUM     = 0x31, /*!< 16-bit enumeration (U16 discrete) data type */
  ZB_ZCL_ATTR_TYPE_OCTET_STRING   = 0x41, /*!< Octet string data type, */
  ZB_ZCL_ATTR_TYPE_CHAR_STRING    = 0x42, /*!< Charactery string (array) data type */
  ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING = 0x43, /*!< Long octet string */
  ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING  = 0x44, /*!< Long character string */
  ZB_ZCL_ATTR_TYPE_ARRAY          = 0x48, /*!< Array data type */
  ZB_ZCL_ATTR_TYPE_IEEE_ADDR      = 0xf0, /*!< IEEE address (U64) type */
  ZB_ZCL_ATTR_TYPE_128_BIT_KEY    = 0xf1, /*!< 128-bit security key */
  ZB_ZCL_ATTR_TYPE_UTC_TIME       = 0xe2, /*!< UTC Time type */

  ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY = 0x4a, /*!< Custom array of 32 elems data type (now is
                                                  * equal to ZB_ZCL_ATTR_TYPE_ARRAY) */

  ZB_ZCL_ATTR_TYPE_INVALID        = 0xff  /*!< Invalid data type */
} zb_zcl_attr_type_t;

/*! @brief ZCL attribute access values */
typedef enum zb_zcl_attr_access_e
{
  ZB_ZCL_ATTR_ACCESS_READ_ONLY      = 0x01,
  ZB_ZCL_ATTR_ACCESS_WRITE_ONLY     = 0x02,  /*!< Attribute is read/write */
  ZB_ZCL_ATTR_ACCESS_READ_WRITE     = ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_WRITE_ONLY,  /*!< Attribute is read/write */
  ZB_ZCL_ATTR_ACCESS_REPORTING      = 0x04,  /*!< Attribute is allowed for reporting */
  ZB_ZCL_ATTR_ACCESS_WRITE_OPTIONAL = ZB_ZCL_ATTR_ACCESS_READ_ONLY,  /*!< Attribute is read/write */
  ZB_ZCL_ATTR_ACCESS_SINGLETON      = 0x08,  /*!< Attribute is singleton */
  ZB_ZCL_ATTR_ACCESS_SCENE          = 0x10,  /*!< Attribute is accessed through scene */
  /* Use free bit in access attribute field to save RAM */
  ZB_ZCL_ATTR_MANUF_SPEC            = 0x20,  /*!< Attribute is manufacturer specific */
  ZB_ZCL_ATTR_ACCESS_INTERNAL       = 0x40,  /*!< ZBOSS Internal accsess only Attribute */
} zb_zcl_attr_access_t;

#define ZB_ZCL_ATTR_SET_WITH_ATTR_ID(_set, _id) ((_set << 8) | (_id & 0xFF))

/** @cond internals_doc */

/**
 *  @brief Sets attribute access type to WRITABLE, if it is optionally permitted.
 *  @param attr_desc - pointer to attribute descriptor
 *  @hideinitializer
 */
#define ZB_ZCL_SET_ATTR_WRITABLE(attr_desc)                  \
{                                                            \
  if (attr_desc->access & ZB_ZCL_ATTR_ACCESS_WRITE_OPTIONAL) \
  {                                                          \
    attr_desc->access |= ZB_ZCL_ATTR_ACCESS_WRITE_ONLY;      \
  }                                                          \
}

/** @endcond */ /* internals_doc */

/*! @brief ZCL attribute definiton structure */
typedef ZB_PACKED_PRE  struct zb_zcl_attr_s
{
  zb_uint16_t id;     /*!< Attribute id */
  zb_uint8_t type;    /*!< Attribute type see @ref zb_zcl_attr_type_t */
  zb_uint8_t access;  /*!< Attribute access options according to @ref zb_zcl_attr_access_t */
  zb_voidp_t data_p;  /*!< Pointer to data */
} ZB_PACKED_STRUCT
zb_zcl_attr_t;


/*! @brief ZCL 6.0: Global attributes that are included to each cluster */
enum zb_zcl_attr_global_e
{
  ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID = 0xfffd,           /*!< Cluster revision */
  ZB_ZCL_ATTR_GLOBAL_ATTRIBUTE_REPORTING_STATUS_ID = 0xfffe, /*!< Attribute reporting status */
};

/** @brief ZCL 6.0: Default value for cluster revision global attribute,
 *  see @ref zb_zcl_attr_global_e
 */
#define ZB_ZCL_CLUSTER_REVISION_DEFAULT 1

#define ZB_ZCL_ATTR_SET_WITH_ATTR_ID(_set, _id) ((_set << 8) | (_id & 0xFF))

/** @cond internals_doc */

/*! @internal @brief Set attribute description.
    @param attr_id - attribute identifier (defined individually for any particular cluster).
    @param data_ptr - pointer to attribute's value storage.

    Creates attribute description value (@ref zb_zcl_attr_s) initialized with provided pointer to
    attribute's data.
*/
#define ZB_ZCL_SET_ATTR_DESC(attr_id, data_ptr) ZB_SET_ATTR_DESCR_WITH_##attr_id(data_ptr),


/*! @internal @brief Set manufacturer specififc attribute description.
    @param attr_id - attribute identifier (defined individually for any particular cluster).
    @param attr_type - attribute data type @see zb_zcl_attr_type_e
    @param attr_access - attribute access bitbask @see zb_zcl_attr_access_e
    @param data_ptr - pointer to attribute's value storage.

    Creates attribute description value (@ref zb_zcl_attr_s) initialized with provided pointer to
    attribute's data.
*/
#define ZB_ZCL_SET_MANUF_SPEC_ATTR_DESC(attr_id, attr_type, attr_access, data_ptr) \
  {                                                                                \
    attr_id,                                                                       \
    attr_type,                                                                     \
    attr_access | ZB_ZCL_ATTR_MANUF_SPEC,                                          \
    (zb_voidp_t) data_ptr                                                          \
  },


/*! @internal @brief Start declaration of attributes list */
#define ZB_ZCL_START_DECLARE_ATTRIB_LIST(attrs_desc_name) \
  zb_uint16_t cluster_revision_##attrs_desc_name = ZB_ZCL_CLUSTER_REVISION_DEFAULT; \
  zb_zcl_attr_t attrs_desc_name [] = {                                              \
  {                                                                                 \
    ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID,                                         \
    ZB_ZCL_ATTR_TYPE_U16,                                                           \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                   \
    (zb_voidp_t) &(cluster_revision_##attrs_desc_name)                              \
  },

/*! @internal @brief End declaration of attributes list */
#define ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST \
  {                                       \
    ZB_ZCL_NULL_ID,                       \
    0,                                    \
    0,                                    \
    NULL                                  \
  }                                       \
}

#define ZB_ZCL_SET_ATTR_DESC_M(id, data_ptr, type, attr)                \
  {                                                                     \
    (id), (type), (attr), (data_ptr)                                    \
      },

/*! @internal @brief Check, if attribute is reportable */
#define ZB_ZCL_IS_ATTR_REPORTABLE(attr_desc) ((attr_desc)->access & ZB_ZCL_ATTR_ACCESS_REPORTING)

/*! @internal @brief Check, if attribute is singleton */
#define ZB_ZCL_IS_ATTR_SINGLETON(attr_desc) ((attr_desc)->access & ZB_ZCL_ATTR_ACCESS_SINGLETON)

/*! @internal @brief Check, if attribute is accessed through scene */
#define ZB_ZCL_IS_ATTR_SCENE_ACCESSED(attr_desc) ((attr_desc)->access & ZB_ZCL_ATTR_ACCESS_SCENE)

/*! @internal @brief Check, if attribute is manufacturer specific */
#define ZB_ZCL_IS_ATTR_MANUF_SPEC(attr_desc) ((attr_desc)->access & ZB_ZCL_ATTR_MANUF_SPEC)

/*! Mark attribute ID to distinguish if it is common/manufacturer
    specific/internal attribute:
    - set bit 15 (the highest) for internal attributes
    - set bit 14 for manufacturer specific attributes
*/
/*! @internal @brief Mark attribute Id as custom/manufacture specific attribute */
#define ZB_ZCL_ATTR_INTERNAL_ID(_attr_id) ((_attr_id) | (1 << 15))

/*! @internal @brief Check if attribute Id is custom/manufacture specific */
#define ZB_ZCL_ATTR_CHECK_INTERNAL_ID(_attr_id) ((_attr_id) & (1 << 15))


/*! @internal @brief Mark attribute Id as custom/manufacture specific attribute */
#define ZB_ZCL_ATTR_CUSTOM_ID(_attr_id) ((_attr_id) | (1 << 14))

/*! @internal @brief Check if attribute Id is custom/manufacture specific */
#define ZB_ZCL_ATTR_CHECK_CUSTOM_ID(_attr_id) ((_attr_id) & (1 << 14))


/*! @internal @brief Start number for custom attributes */
#define ZB_ZCL_ATTR_CUSTOM_START_NUMBER    0xfff0


/*! @internal @brief Check if attribute Id is custom/manufacture specific */
#define ZB_ZCL_ATTR_CHECK_INTERNAL_ID(_attr_id) ((_attr_id) & (1 << 15))


/*! @internal @brief Mark attribute Id as custom/manufacture specific attribute */
#define ZB_ZCL_ATTR_CUSTOM_ID(_attr_id) ((_attr_id) | (1 << 14))

/*! @internal @brief Check if attribute Id is custom/manufacture specific */
#define ZB_ZCL_ATTR_CHECK_CUSTOM_ID(_attr_id) ((_attr_id) & (1 << 14))

/*! @internal @brief Check boolean type attribute value */
#define ZB_ZCL_CHECK_BOOL_VALUE(x) ((x) == ZB_FALSE || (x) == ZB_TRUE)


/** @endcond */ /* internals_doc */

/**
 *  @brief Sets attribute value, perform all needed checks before and after setting new value,
 *  including read-only check and marking for reporting.
 *  @param ep - end point number.
 *  @param cluster_id - cluster identifier.
 *  @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 *  @param attr_id - attribute identifier.
 *  @param value_ptr - pointer to new attribute value.
 *  @param check_access - boolean flag to specify if to perform access check or not.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_ATTRIBUTE(ep, cluster_id, cluster_role, attr_id, value_ptr, check_access) \
  ZVUNUSED(zb_zcl_set_attr_val(ep, cluster_id, cluster_role, attr_id, value_ptr, check_access))


/**
 *  @brief Set attribute 8-bit value without any check. Use this macro
 *  on your own risk, if and only if you are absolutely shure that no check for Access
 *  rights, Reporting and attribute type is needed.
 *  @param attr_desc - pointer to an attribute description structure @ref zb_zcl_attr_s.
 *  @param value - 8 bit value to be set.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, value)    \
  (*(zb_uint8_t*)attr_desc->data_p = (zb_uint8_t)(value))

/**
 *  @brief Set attribute 16-bit value without any check. Use this macro
 *  on your own risk, if and only if you are absolutely shure that no check for Access
 *  rights, Reporting and attribute type is needed.
 *  @param attr_desc - pointer to an attribute description structure @ref zb_zcl_attr_s.
 *  @param value - 16 bit value to be set.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, value)  \
  (*(zb_uint16_t*)attr_desc->data_p = (zb_uint16_t)(value))

/**
 *  @brief Set attribute 32-bit value without any check. Use this macro
 *  on your own risk, if and only if you are absolutely shure that no check for Access
 *  rights, Reporting and attribute type is needed.
 *  @param attr_desc - pointer to an attribute description structure @ref zb_zcl_attr_s.
 *  @param value - 32 bit value to be set.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, value)  \
  (*(zb_uint32_t*)attr_desc->data_p = (zb_uint32_t)(value))


/*! Get 8-bit unsigned attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc)           \
  (*(zb_uint8_t*)attr_desc->data_p)

/*! Get 8-bit signed attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_S8(attr_desc)           \
  (*(zb_int8_t*)attr_desc->data_p)

/*! Get 16-bit unsigned attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc)          \
  (*(zb_uint16_t*)attr_desc->data_p)

/*! Get 16-bit signed attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc)          \
  (*(zb_int16_t*)attr_desc->data_p)

/*! Get 32-bit unsigned attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc)          \
  (*(zb_uint32_t*)attr_desc->data_p)

/*! Get 32-bit unsigned attribute value (without any check) */
#define ZB_ZCL_GET_ATTRIBUTE_VAL_S32(attr_desc)          \
  (*(zb_int32_t*)attr_desc->data_p)


/*! @} */ /* General attributes' description */


/************************************** ZCL frame definitions ************************************/

/*! @defgroup zb_zcl_common_frame General frame description
    Types and macros required for ZCL frame manipulations
    @{

*/

/** @cond internals_doc */

/*! @brief ZCL header size */
#define ZB_ZCL_GET_HEADER_SIZE(frm_ctl)    \
  (   (ZB_ZCL_GET_MANUF_SPECIFIC(frm_ctl)) \
   ? sizeof(zb_zcl_frame_hdr_full_t)       \
   : sizeof(zb_zcl_frame_hdr_short_t))

/** @internal @brief Cuts ZCL header form a buffer. */
#define ZB_ZCL_CUT_HEADER(buf)                                                   \
  ZB_BUF_CUT_LEFT2(buf, ZB_ZCL_GET_HEADER_SIZE(*(zb_uint8_t*)ZB_BUF_BEGIN(buf)))

/** @endcond */ /* internals_doc */

/*! @brief ZCL frame type

    Defines Permitted values for Frame Type subfield of Frame Control field in ZCL frame.
    @see ZCL spec, subclause 2.3.1.1.1.
*/
typedef enum zb_zcl_frame_type_e
{
  ZB_ZCL_FRAME_TYPE_COMMON            = 0x00, /*!< Command acts across the entire profile  */
  ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC  = 0x01  /*!< Command is specific to a cluster */
} zb_zcl_frame_type_t;

/*! @brief Values for Manufacturer-specific subfield of FCF in ZCL frame.
    @see ZCL spec, subclause 2.3.1.1.2.
*/
typedef enum zb_zcl_manufacturer_specific_e
{
  /** @brief Standard profile command

      Manufacturer code field shall @b not be included into ZCL frame header.
    */
  ZB_ZCL_NOT_MANUFACTURER_SPECIFIC    = 0x00,
  /** @brief Command refers to manufacturer-specific profile extension

      Manufacturer code field shall be included into ZCL frame header.
    */
  ZB_ZCL_MANUFACTURER_SPECIFIC        = 0x01
} zb_zcl_manufacturer_specific_t;

#define ZB_ZCL_MANUFACTURER_WILDCARD_ID         0xffff

/*! @brief Values for Disable default response subfield of FCF in ZCL frame.
    @see ZCL spec, subclause 2.3.1.1.3.
*/
typedef enum zb_zcl_disable_default_response_e
{
  /** @brief Enable default response */
  ZB_ZCL_ENABLE_DEFAULT_RESPONSE    = 0x00,
  /** @brief Disble default response */
  ZB_ZCL_DISABLE_DEFAULT_RESPONSE    = 0x01,
} zb_zcl_disable_default_response_t;


/*! @brief ZCL frame direction
    @see ZCL spec, subclause 2.3.1.1.3.
*/
typedef enum zb_zcl_frame_direction_e
{
  /*! Command sent from client to a server */
  ZB_ZCL_FRAME_DIRECTION_TO_SRV = 0x00,
  /*! Command sent from server to a client */
  ZB_ZCL_FRAME_DIRECTION_TO_CLI = 0x01
} zb_zcl_frame_direction_t;

/*! @brief Reverts ZCL direction value */
#define ZB_ZCL_REVERT_DIRECTION(_d) \
  (((zb_zcl_frame_direction_t)(_d) == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ? ZB_ZCL_FRAME_DIRECTION_TO_CLI : ZB_ZCL_FRAME_DIRECTION_TO_SRV)

/*! @brief ZCL frame control field
    @see ZCL spec, subclause 2.3.1.1
*/
typedef ZB_PACKED_PRE struct zb_zcl_frame_ctrl_s
{
  zb_bitfield_t frame_type        :2;     /*!< Frame type @ref zb_zcl_frame_type_t */
  zb_bitfield_t manufacturer      :1;     /*!< Manufacturer specific frame @ref
                                          zb_zcl_manufacturer_specific_t*/
  zb_bitfield_t direction         :1;     /*!< Direction @ref zb_zcl_frame_direction_t*/
  zb_bitfield_t disable_def_resp  :1;     /*!< Disable default response */
  zb_bitfield_t reserved          :3;
} ZB_PACKED_STRUCT
zb_zcl_frame_ctrl_t;

/*! @brief ZCL frame header with manufacturer code
    @see ZCL spec, subclause 2.3.1
*/
typedef ZB_PACKED_PRE  struct zb_zcl_frame_hdr_full_s
{
  zb_zcl_frame_ctrl_t frame_ctrl;         /*!< Frame control filed see @ref zb_zcl_frame_ctrl_s */
  zb_uint16_t         manufacturer_code;  /*!< Manufacturer Code */
  zb_uint8_t          seq_number;         /*!< Transaction Sequence Number */
  zb_uint8_t          command_id;         /*!< Command Identifier Field */
} ZB_PACKED_STRUCT
zb_zcl_frame_hdr_full_t;

/*! @brief ZCL frame header without manufacturer code
    @see ZCL spec, subclause 2.3.1
*/
typedef ZB_PACKED_PRE  struct zb_zcl_frame_hdr_short_s
{
  zb_zcl_frame_ctrl_t frame_ctrl;   /*!< Frame control field see @ref zb_zcl_frame_ctrl_s */
  zb_uint8_t          seq_number;   /*!< Transaction Sequence Number */
  zb_uint8_t          command_id;   /*!< Command Identifier Field */
} ZB_PACKED_STRUCT
zb_zcl_frame_hdr_short_t;

/**
 * @brief Possible address types of devices from which ZCL message is received.
 */
enum zb_zcl_addr_type_e
{
  ZB_ZCL_ADDR_TYPE_SHORT,
  ZB_ZCL_ADDR_TYPE_IEEE_GPD,
  ZB_ZCL_ADDR_TYPE_SRC_ID_GPD,
  ZB_ZCL_ADDR_TYPE_IEEE,
};

/**
 * @brief Type to represent source address of ZCL message
 *
 * It is needed if ZGP sink is enabled and therefore messages can
 * be received from ZGPD devices which are identified by 32-bit
 * source Id.
 */
typedef ZB_PACKED_PRE struct zb_zcl_addr_s
{
  zb_uint8_t addr_type;  /**< address type (see @ref zb_zcl_addr_type_e) */
  ZB_PACKED_PRE union zb_zcl_addr_u
  {
    zb_uint16_t    short_addr; /**< Zigbee short address */
    zb_uint32_t    src_id;     /**< Source ID of ZGPD */
    zb_ieee_addr_t ieee_addr;  /**< Full IEEE-address of ZGPD */
  } u;
} ZB_PACKED_STRUCT
zb_zcl_addr_t;

/*! Parsed ZCL header */
typedef ZB_PACKED_PRE  struct zb_zcl_parsed_hdr_s
{
  /* Values from APS data indication */
  ZB_PACKED_PRE union
  {
    ZB_PACKED_PRE struct
    {
      //zb_uint16_t src_addr;
      zb_zcl_addr_t source;
      zb_uint16_t dst_addr;
      zb_uint8_t  src_endpoint;
      zb_uint8_t  dst_endpoint;
      zb_uint8_t  fc;
#if (defined ZB_ENABLE_SE)
      zb_uint8_t aps_key_source:1;
      zb_uint8_t aps_key_attrs:2;
      zb_uint8_t reserved:5;
#endif
    } ZB_PACKED_STRUCT common_data;
#if defined ZB_ENABLE_ZLL
    ZB_PACKED_PRE struct
    {
/* 07/26/2017 EE CR:MINOR Kill that data structure. Utilize src_addr_ext. Kill upper one nesting level. */
      zb_ieee_addr_t src_addr;  /**< Sender device extended address. */
    } ZB_PACKED_STRUCT intrp_data;
#endif /* defined ZB_ENABLE_ZLL */
  } addr_data;
#if defined ZB_ENABLE_ZLL
  zb_uint8_t rssi;          /**< RSSI of the packet. */
#endif /* defined ZB_ENABLE_ZLL */
  zb_uint16_t cluster_id;
  zb_uint16_t profile_id;

  /* ZCL specific fields */
  zb_uint8_t  cmd_id;                   /**< Command identifier. */
  zb_uint8_t  cmd_direction;            /**< Command direction identifier. */
  zb_uint8_t  seq_number;               /**< ZCL transaction sequence number. */
  zb_uint8_t  is_common_command;        /**< "Common command" flag. */
  zb_uint8_t  disable_default_response; /**< "Disable default response" flag. */
  zb_uint8_t  is_manuf_specific;        /**< "Has manufacturer-specific data" flag. */
  zb_uint16_t manuf_specific;           /**< Manufacturer-specific data. */
//#ifdef APS_FRAGMENTATION
  zb_uint8_t  next_buf_ref;             /**< Pointer to next buf if fragmentation */
//#endif
} ZB_PACKED_STRUCT
zb_zcl_parsed_hdr_t;


/*! Attribute address info structure */
typedef struct zb_zcl_attr_addr_info_s
{
  zb_uint8_t src_ep;          /**< Source endpoint */
  zb_uint8_t dst_ep;          /**< Destination endpoint */
  zb_uint16_t dst_short_addr; /**< Destination short address */

  zb_uint16_t profile_id;     /**< Profile id */
  zb_uint16_t cluster_id;     /**< Cluster id */
  zb_uint8_t cluster_role;    /**< Cluster role */
  zb_uint16_t attr_id;        /**< Attribute id */
}
zb_zcl_attr_addr_info_t;


/** @brief Extract common addresing data from parsed ZCL structure.
  * @param header - pointer to the parsed ZCL header structure (see @ref zb_zcl_parsed_hdr_s).
  */
#define ZB_ZCL_PARSED_HDR_SHORT_DATA(header) ((header)->addr_data.common_data)

/**
 *  @brief Get ZCL frame type @ref zb_zcl_frame_type_e.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_FRAME_TYPE(v)     ((v) & 0x3)

/**
 *  @brief Check is ZCL frame manufacturer specific @ref zb_zcl_manufacturer_specific_e.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_MANUF_SPECIFIC(v) ((v >> 2) & 0x1)

/**
 *  @brief Get ZCL frame direction @ref zb_zcl_frame_direction_e.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_DIRECTION(v)      ((v >> 3) & 0x1)

/**
 *  @brief Get ZCL disable default response field @ref zb_zcl_disable_default_response_e.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_DISABLE_DEFAULT_RESPONSE(v) ((v >> 4) & 0x1)

/**
 *  @brief Set ZCL frame type @ref zb_zcl_frame_type_e.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_FRAME_TYPE(v, val)     ((v) |= (val))

/**
 * @brief  Set ZCL frame manufacturer specific @ref zb_zcl_manufacturer_specific_e.
 * @hideinitializer
 */
#define ZB_ZCL_SET_MANUF_SPECIFIC(v, val) ((v) |= (val << 2))

/**
 *  @brief Set ZCL frame direction @ref zb_zcl_frame_direction_e.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DIRECTION(v, val)      ((v) |= (val << 3))

/**
 *  @brief Set ZCL disable default response field @ref zb_zcl_disable_default_response_e.
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DISABLE_DEFAULT_RESPONSE(v, val) ((v) |= (val << 4))

/*! @brief Construct ZCL command header in the buffer
    @param zbbuf pointer to the the buffer
    @param frame_ctl Frame Control field for ZCL packet
    @param manuf_code manufacturer code, not used manufacturer-specific flag is not set
    @param cmd_id ZCL command id
    @param tsn pointer to variable (may be NULL) for TSN value of ZCL packet (generated by the function)

    @return pointer to the buf after ZCL header (where command payload can be placed if needed)
 */
void *zb_zcl_start_command_header(zb_buf_t *zbbuf, zb_uint8_t frame_ctl, zb_uint16_t manuf_code, zb_uint8_t cmd_id, zb_uint8_t* tsn);

/** @cond internals_doc */

/** @internal @brief Construct ZCL header frame control value */
#define ZB_ZCL_CONSTRUCT_FRAME_CONTROL(                                 \
  frame_type, manuf_specific, direction, disable_default_resp)          \
  ((  (frame_type)                                                      \
      | ((manuf_specific) << 2)                                         \
      | ((direction) << 3)                                              \
      | ((disable_default_resp) << 4)))

#define ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(                             \
    frame_ctl, frame_type, manuf_specific, direction, disable_default_resp) \
  ((frame_ctl) = ZB_ZCL_CONSTRUCT_FRAME_CONTROL((frame_type),           \
                                                (manuf_specific),       \
                                                (direction),            \
                                                (disable_default_resp)))

/* NOTE: We suppose all general requests/responses currently are not manufacturer specific */
/** @internal @brief Construct ZCL header frame control value for General command request */
#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_REQ_FRAME_CONTROL(buf_ptr, def_resp) \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                                   \
    ZB_ZCL_FRAME_TYPE_COMMON,                                                 \
    ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,                                         \
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,                                            \
    (def_resp)),                                                              \
   buf_ptr++)

#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_REQ_FRAME_CONTROL_A(buf_ptr,   \
                  direction, is_manufacturer_specific, def_resp)        \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                             \
    ZB_ZCL_FRAME_TYPE_COMMON,                                           \
    (is_manufacturer_specific),                                         \
    (direction),                                                        \
    (def_resp)),                                                        \
   buf_ptr++)

/*! @internal Construct ZCL header frame control value for General
 *  command request, specifying extra parameters */
#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_REQ_FRAME_CONTROL_EXT(buf_ptr, \
                _is_manuf_specific, direction, def_resp)                \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                             \
    ZB_ZCL_FRAME_TYPE_COMMON,                                           \
    (_is_manuf_specific),                                               \
    (direction),                                                        \
    (def_resp)),                                                        \
   buf_ptr++)


/* To do not change existing code too much: 3 consequent macro create single function call. */
#define ZB_ZCL_START_PACKET_REQ(zbbuf)  zb_zcl_start_command_header((zbbuf),
#define ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(buf_ptr, def_resp) \
  ZB_ZCL_CONSTRUCT_FRAME_CONTROL(ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC, ZB_ZCL_NOT_MANUFACTURER_SPECIFIC, \
                                 ZB_ZCL_FRAME_DIRECTION_TO_SRV, (def_resp)), \
    0,                          /* No manuf_code */
#define ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(data_ptr, tsn, cmd_id) (cmd_id), NULL)

/* TODO: Implement via zb_zcl_start_command_header() */
#define ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL_O(buf_ptr, def_resp) \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                                    \
    ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC,                                        \
    ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,                                          \
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,                                             \
    def_resp),                                                                 \
   buf_ptr++)

#define ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL_A(buf_ptr,  \
                  direction, is_manufacturer_specific, def_resp)        \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                             \
    ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC,                                 \
    (is_manufacturer_specific),                                         \
    (direction),                                                        \
    (def_resp)),                                                        \
   buf_ptr++)

#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_A(buf_ptr,  \
                direction, is_manufacturer_specific)                    \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                             \
    ZB_ZCL_FRAME_TYPE_COMMON,                                           \
    (is_manufacturer_specific),                                         \
    (direction),                                                        \
    ZB_ZCL_DISABLE_DEFAULT_RESPONSE),                                   \
     buf_ptr++)

#define ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RESP_FRAME_CONTROL_A(buf_ptr, \
                direction, is_manufacturer_specific)                    \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                             \
    ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC,                                 \
    (is_manufacturer_specific),                                         \
    (direction),                                                        \
    ZB_ZCL_DISABLE_DEFAULT_RESPONSE),                                   \
     buf_ptr++)
/*! @internal Construct ZCL header frame control valueZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL for General command response */
#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL(buf_ptr) \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                          \
    ZB_ZCL_FRAME_TYPE_COMMON,                                        \
    ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,                                \
    ZB_ZCL_FRAME_DIRECTION_TO_CLI,                                   \
    ZB_ZCL_DISABLE_DEFAULT_RESPONSE),                                \
     buf_ptr++)


/*! @internal Construct ZCL header frame control value for General
 *  command response, specifying extra parameters */
#define ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_EXT(_buf_ptr, _is_manuf_specific, _direction) \
  (ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(                                   \
    *(_buf_ptr),                                                     \
    ZB_ZCL_FRAME_TYPE_COMMON,                                        \
    (_is_manuf_specific),                                            \
    (_direction),                                                    \
    ZB_ZCL_DISABLE_DEFAULT_RESPONSE),                                \
   (_buf_ptr)++)

/** @endcond */ /* internals_doc */

/*! @brief Construct ZCL header frame control value for cluster-specific command response
 *
 *  @snippet zcl_groups.c ZB_ZCL_CONSTRUCT_FRAME_HEADER
 * */
#define ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(buf_ptr) \
  ( ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*buf_ptr,                         \
        ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC,                          \
        ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,                            \
        ZB_ZCL_FRAME_DIRECTION_TO_CLI,                               \
        ZB_ZCL_DISABLE_DEFAULT_RESPONSE),                            \
    buf_ptr++)

/**
 *  @brief Construct ZCL header
 *  @note To add Manufacturer specific field, use ZB_ZCL_CONSTRUCT_COMMAND_HEADER_M(), macro.
 *  @hideinitializer
 */
#define ZB_ZCL_CONSTRUCT_COMMAND_HEADER(data_ptr, tsn, cmd_id)                        \
  (ZB_ZCL_PACKET_PUT_DATA8(data_ptr, tsn), ZB_ZCL_PACKET_PUT_DATA8(data_ptr, cmd_id))

/*! @brief Construct ZCL header, Manufacturer specific value is conditionally supported */
#define ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(_data_ptr, _tsn, _is_manuf_spec, _manuf_specific, _cmd_id) \
  {                                                                     \
    if (_is_manuf_spec)                                                 \
    {                                                                   \
      ZB_ZCL_PACKET_PUT_DATA16_VAL((_data_ptr), (_manuf_specific));     \
    }                                                                   \
    ZB_ZCL_PACKET_PUT_DATA8((_data_ptr), (_tsn));                       \
    ZB_ZCL_PACKET_PUT_DATA8((_data_ptr), (_cmd_id));                    \
  }

/** @cond internals_doc */

/*! @internal @brief Caclulate ZCL frame header size */
#define ZB_ZCL_FRAME_HDR_GET_SIZE(p) ( ZB_ZCL_GET_MANUF_SPECIFIC(p) ?    \
                                       2*sizeof(zb_uint8_t)+sizeof(zb_uint16_t) : \
                                       2*sizeof(zb_uint8_t) )

/*! @internal @brief Get ZCL frame manufacturer code from the packet */
#define ZB_ZCL_FRAME_HDR_GET_MANUFACTURER_CODE(p)                       \
    ( *(zb_uint16_t *)((zb_uint8_t *)p + sizeof(zb_zcl_frame_ctrl_t)) )

/*! @internal Get ZCL frame sequence number from the packet. */
#define ZB_ZCL_FRAME_HDR_GET_SEQ_NUM(p)                            \
    (   *((zb_uint8_t *)p                                          \
     +  sizeof(zb_zcl_frame_ctrl_t)                                \
     + (ZB_ZCL_GET_MANUF_SPECIFIC(p) ? sizeof(zb_uint16_t) : 0)) )

/*! @internal Get ZCL frame command identifier from the packet. */
#define ZB_ZCL_FRAME_HDR_GET_COMMAND_ID(p)                                              \
    (   *((zb_uint8_t *)p                                                               \
     + sizeof(zb_zcl_frame_ctrl_t)                                                      \
     + (ZB_ZCL_GET_MANUF_SPECIFIC(p) ? sizeof(zb_uint16_t) : 0) + sizeof(zb_uint8_t)) )

/** @endcond */ /* internals_doc */

/**
 *  @brief Return next sequence number for ZCL frame.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_SEQ_NUM() (ZCL_CTX().seq_number++)

/*! @brief Copy parsed ZCL header from buffer
    @param buf - pointer to @ref zb_buf_t structure containing parsed ZCL header as parameter
    @param dst - pointer to the memory area to copy header to
*/
#define ZB_ZCL_COPY_PARSED_HEADER(buf, dst)         \
  (ZB_MEMCPY(                                       \
      (dst),                                        \
      ZB_GET_BUF_PARAM((buf), zb_zcl_parsed_hdr_t), \
      sizeof(zb_zcl_parsed_hdr_t)))


/*! @brief General macro for sending a command with empty payload
    @param buffer to put command payload and headers to
    @param addr address to send to
    @param dst_addr_mode destination address mode
    @param dst_ep destination endpoint
    @param ep source endpoint
    @param prof_id profile the command belongs to
    @param dis_default_resp "Disable default response" flag
    @param cluster_id cluster identifier
    @param command_id command identifier
    @param cb command send status callback

    @code
      #define ZB_ZCL_DOOR_LOCK_SEND_LOCK_DOOR_REQ(buffer,           \
                                                  dst_addr,         \
                                                  dst_addr_mode,    \
                                                  dst_ep,           \
                                                  ep,               \
                                                  dis_default_resp, \
                                                  cb)               \
        ZB_ZCL_SEND_CMD(  buffer,                                   \
                          dst_addr,                                 \
                          dst_addr_mode,                            \
                          dst_ep,                                   \
                          ep,                                       \
                          ZB_AF_HA_PROFILE_ID,                      \
                          dis_default_resp,                         \
                          ZB_ZCL_CLUSTER_ID_DOOR_LOCK,              \
                          ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR,           \
                          cb)
    @endcode
*/
#define ZB_ZCL_SEND_CMD(                                                                       \
    buffer,                                                                                    \
    addr,                                                                                      \
    dst_addr_mode,                                                                             \
    dst_ep,                                                                                    \
    ep,                                                                                        \
    prof_id,                                                                                   \
    dis_default_resp,                                                                          \
    cluster_id,                                                                                \
    command_id, cb)                                                                            \
{                                                                                              \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                            \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, dis_default_resp)                   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), command_id);                  \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                            \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, cluster_id, cb); \
}

/*! @} */ /* General frame description */


/******************************* ZCL frame basic manipulations *******************************/

/*! @defgroup zb_zcl_common_pckt_fill Fill ZCL packet low-level API
    Types and macros shared fill Fill ZCL packet.
    @{
      @par Example
      @snippet configure_report_req.c ZB_ZCL_PACKET
      @par

    For more information see any HA sample
*/

/**
 *  @brief Initializes zb_buf_t buffer and returns pointer to the beginning of array.
 *  @hideinitializer
 */
#define ZB_ZCL_START_PACKET(zbbuf)                    \
  (ZB_BUF_REUSE(zbbuf), ZB_BUF_GET_ARRAY_PTR(zbbuf));

/**
 *  @brief Return number of bytes written to the buffer.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_BYTES_WRITTEN(zbbuf, ptr) \
  ((ptr) - ZB_BUF_GET_ARRAY_PTR(zbbuf))

/**
 *  @brief Return number of bytes available in a packet.
 *  @hideinitializer
 */
#define ZB_ZCL_GET_BYTES_AVAILABLE(zbbuf, ptr)              \
  (ZB_ZCL_MAX_PAYLOAD_SIZE - ZB_ZCL_GET_BYTES_WRITTEN(zbbuf, ptr))

/**
 *  @brief Put 8 bit value to packet.
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA8(ptr, val) (*(ptr) = (val), (ptr)++)

/*!
  Put 16 bit value to packet
  @param ptr - pointer to the place to put value to
  @param val - pointer to the value to put into packet
*/
#define ZB_ZCL_PACKET_PUT_DATA16(ptr, val)       \
 {                                              \
   ZB_HTOLE16((ptr), (val));                    \
   (ptr) += 2;                                  \
}

/*!
  Put 16 bit value to packet
  @param ptr - pointer to the place to put value to
  @param val - value to put into packet
*/
#define ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, val) ZB_PUT_NEXT_HTOLE16(ptr, val)

#define ZB_ZCL_PACKET_PUT_2DATA16_VAL(ptr, val1, val2) (ptr) = zb_put_next_2_htole16((ptr),(val1),(val2))

#define ZB_ZCL_PACKET_PUT_2DATA32_VAL(ptr, val1, val2) (ptr) = zb_put_next_2_htole32((ptr),(val1),(val2))

/**
 *  @brief Put 32 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - pointer to the value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA32_VAL(ptr, val)  ZB_PUT_NEXT_HTOLE32((ptr),(val))

/**
 *  @brief Put 32 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - pointer to the value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA32(ptr, val)       \
   (ZB_HTOLE32(ptr, val), (ptr) += 4)

/**
 *  @brief Put 48 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA48_VAL(ptr, val) \
    (PUT_DATA48_VAL(ptr, val))

/**
 *  @brief Put 48 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - pointer to the value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA48(ptr, val)       \
   (PUT_DATA48(ptr, val))

/**
 *  @brief Put 24 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA24_VAL(ptr, val) \
    (PUT_DATA24_VAL(ptr, val))

/**
 *  @brief Put 24 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - pointer to the value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA24(ptr, val)       \
    (PUT_DATA24(ptr, val))



/**
 *  @brief Put 64 bit value to packet.
 *  @param ptr - pointer to the place to put value to
 *  @param val - pointer to the value to put into packet
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA64(ptr, val) (ZB_HTOLE64(ptr, val), (ptr) += 8)

/** @brief Get 8-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented).
  */
#define ZB_ZCL_PACKET_GET_DATA8(dst_ptr, src_ptr) \
{                                                 \
  *(dst_ptr) = *((src_ptr)++);                    \
}

#define ZB_ZCL_PACKET_PUT_2DATA16_VAL(ptr, val1, val2) (ptr) = zb_put_next_2_htole16((ptr),(val1),(val2))

#define ZB_ZCL_PACKET_PUT_2DATA32_VAL(ptr, val1, val2) (ptr) = zb_put_next_2_htole32((ptr),(val1),(val2))

/**
 *  @brief Put N byte data to packet.
 *  @param ptr - pointer to the place to put value to.
 *  @param val - pointer to the value to put into packet.
 *  @param n - number of bytes to be copied.
 *  @hideinitializer
 */
#define ZB_ZCL_PACKET_PUT_DATA_N(ptr, val, n) (ZB_MEMCPY(ptr, val, n), (ptr) += (n))

/** @brief Get 16-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA16(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH16((dst_ptr), (src_ptr));                \
  (src_ptr) += sizeof(zb_uint16_t);                \
}

/** @brief Get 24-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA24(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH24(dst_ptr, src_ptr);                    \
  (src_ptr) += ZB_24BIT_SIZE;                \
}


/** @brief Get 48-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA48(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH48(dst_ptr, src_ptr);                    \
  (src_ptr) += ZB_48BIT_SIZE;                \
}


/** @brief Get 32-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA32(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH32((dst_ptr), (src_ptr));                \
  (src_ptr) += sizeof(zb_uint32_t);                \
}

/** @brief Get 64-bit value from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA64(dst_ptr, src_ptr) \
{                                                  \
  ZB_LETOH64((dst_ptr), (src_ptr));                \
  (src_ptr) += 8;                                  \
}

/** @brief Get IEEE address from packet.
  * @param dst_ptr - pointer to the memory to store value (pinter value preserved).
  * @param src_ptr - pointer to the memory to get value from (will be incremented by value size).
  */
#define ZB_ZCL_PACKET_GET_DATA_IEEE(dst_ptr, src_ptr) \
{                                                     \
  ZB_IEEE_ADDR_COPY((dst_ptr), (src_ptr));            \
  (src_ptr) += sizeof(zb_ieee_addr_t);                \
}

/** @brief Put IEEE address into packet.
  * @param dst_ptr - pointer to the memory to store value (will be incremented by value size).
  * @param src_ptr - pointer to the memory to get value from (pinter value preserved).
  */
#define ZB_ZCL_PACKET_PUT_DATA_IEEE(dst_ptr, src_ptr) \
{                                                     \
  ZB_IEEE_ADDR_COPY((dst_ptr), (src_ptr));            \
  (dst_ptr) += sizeof(zb_ieee_addr_t);                \
}

/*!
  Put N byte data to packet
*/
#define ZB_ZCL_PACKET_PUT_DATA_N(ptr, val, n)     \
  (ZB_MEMCPY(ptr, val, n), (ptr) += (n))

/** Put ZCL Octet String to packet */
#define ZB_ZCL_PACKET_PUT_STRING(_ptr, _val)                          \
{                                                                     \
 ZB_ZCL_PACKET_PUT_DATA8 (_ptr, ZB_ZCL_GET_STRING_LENGTH(_val));      \
 ZB_ZCL_PACKET_PUT_DATA_N(_ptr, ZB_ZCL_GET_STRING_BEGIN(_val),        \
                          ZB_ZCL_GET_STRING_LENGTH(_val));            \
}

/** Get ZCL String from packet.
 * NOTE: If string is invalid (cannot be stored in buffer)
 * this macro sets data to NULL.
 *
 * @param _zcl_str - Destination buffer.
 * @param data - Pointer to incomming data.
 * @param _zcl_str_max_size - Size of destination buffer.
 */
#define ZB_ZCL_PACKET_GET_STRING(_zcl_str, data, _zcl_str_max_size)         \
{                                                                           \
  ZB_ZCL_PACKET_GET_DATA8(&_zcl_str[0], data);                              \
  if (_zcl_str_max_size > ZB_ZCL_GET_STRING_LENGTH(_zcl_str))               \
  {                                                                         \
    ZB_ZCL_PACKET_GET_DATA_N(ZB_ZCL_GET_STRING_BEGIN(_zcl_str),             \
                             data,                                          \
                             ZB_ZCL_GET_STRING_LENGTH(_zcl_str));           \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    data = NULL;                                                            \
  }                                                                         \
}

/** Get ZCL String from packet and put it into array with fixed size. */
#define ZB_ZCL_PACKET_GET_STATIC_STRING(_zcl_str, data) \
  ZB_ZCL_PACKET_GET_STRING(_zcl_str, data, sizeof(_zcl_str))


/** Put ZCL Octet String to packet */
#define ZB_ZCL_PACKET_PUT_STRING(_ptr, _val)                          \
{                                                                     \
 ZB_ZCL_PACKET_PUT_DATA8 (_ptr, ZB_ZCL_GET_STRING_LENGTH(_val));      \
 ZB_ZCL_PACKET_PUT_DATA_N(_ptr, ZB_ZCL_GET_STRING_BEGIN(_val),        \
                          ZB_ZCL_GET_STRING_LENGTH(_val));            \
}

/** Get ZCL String from packet.
 * NOTE: If string is invalid (cannot be stored in buffer)
 * this macro sets data to NULL.
 *
 * @param _zcl_str - Destination buffer.
 * @param data - Pointer to incomming data.
 * @param _zcl_str_max_size - Size of destination buffer.
 */
#define ZB_ZCL_PACKET_GET_STRING(_zcl_str, data, _zcl_str_max_size)         \
{                                                                           \
  ZB_ZCL_PACKET_GET_DATA8(&_zcl_str[0], data);                              \
  if (_zcl_str_max_size > ZB_ZCL_GET_STRING_LENGTH(_zcl_str))               \
  {                                                                         \
    ZB_ZCL_PACKET_GET_DATA_N(ZB_ZCL_GET_STRING_BEGIN(_zcl_str),             \
                             data,                                          \
                             ZB_ZCL_GET_STRING_LENGTH(_zcl_str));           \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    data = NULL;                                                            \
  }                                                                         \
}

/** Get ZCL String from packet and put it into array with fixed size. */
#define ZB_ZCL_PACKET_GET_STATIC_STRING(_zcl_str, data) \
  ZB_ZCL_PACKET_GET_STRING(_zcl_str, data, sizeof(_zcl_str))


/** @brief Get N-byte value from packet.
  * @param dst_ptr - pointer to the memory to store value (will be incremented by value size).
  * @param src_ptr - pointer to the memory to get value from (pinter value preserved).
  * @param n - data length.
  */
#define ZB_ZCL_PACKET_GET_DATA_N(dst_ptr, src_ptr, n) \
{                                                     \
  ZB_MEMCPY((dst_ptr), (src_ptr), (n));              \
  (src_ptr) += (n);                                  \
}

/*! @brief ZCL finish and send packet
    @param buffer pointer to the buffer where ZCL cmd is stored
    @param ptr pointer to the end of ZCL cmd in buffer
    @param dst_addr destination address (see zb_addr_u)
    @param dst_addr_mode destination address mode (only @ref ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT and
    @ref ZB_APS_ADDR_MODE_16_ENDP_PRESENT are supported)
    @param dst_ep destination endpoint
    @param ep source endpoint
    @param prof_id profile the command belongs to
    @param cluster_id cluster identifier
    @param cb command send status callback
 */
zb_ret_t zb_zcl_finish_and_send_packet(zb_buf_t *buffer, zb_uint8_t *ptr,
                                   zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode,
                                   zb_uint8_t dst_ep, zb_uint8_t ep,
                                   zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                   zb_callback_t cb);
zb_ret_t zb_zcl_finish_and_send_packet_frag(zb_buf_t *buffer, zb_uint8_t *ptr,
                                    zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode,
                                    zb_uint8_t dst_ep, zb_uint8_t ep,
                                    zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                    zb_callback_t cb, zb_bool_t frag);

/*!
  Finish ZCL packet filling. Used bytes number are marked in a buffer.
  @param zbbuf - pointer to zb_buf_t buffer.
  @param ptr - pointer to the byte after the last written byte in the buffer.
  @attention After ZB_ZCL_FINISH_PACKET() call, pointer ptr is set to the beginning of the buffer
  data.
 */
/* Glue ZB_ZCL_FINISH_PACKET and ZB_ZCL_SEND_COMMAND_SHORT into one call */
#define ZB_ZCL_FINISH_PACKET(zbbuf, ptr) (void) zb_zcl_finish_and_send_packet((zbbuf), (ptr),

#define ZB_ZCL_SEND_COMMAND_SHORT(                                      \
  buffer, addr, dst_addr_mode, dst_ep, ep, prof_id, cluster_id, cb)     \
  (zb_addr_u *)(&(addr)), dst_addr_mode, dst_ep, ep, prof_id, cluster_id, cb)

/* TODO: Implement via zb_zcl_finish_and_send_packet() */
#define ZB_ZCL_FINISH_PACKET_O(zbbuf, ptr)                              \
  ZB_BUF_ALLOC_LEFT(zbbuf, ZB_ZCL_GET_BYTES_WRITTEN(zbbuf, ptr), ptr);

/*! @}*/ /* Fill ZCL packet */

/* align func */
#ifdef ZB_NEED_ALIGN

zb_uint16_t zb_zcl_attr_get16(zb_uint8_t *value);
zb_int16_t zb_zcl_attr_gets16(zb_uint8_t *value);
zb_uint32_t zb_zcl_attr_get32(zb_uint8_t *value);
zb_int32_t zb_zcl_attr_gets32(zb_uint8_t *value);
#define ZB_ZCL_ATTR_GET16(value) zb_zcl_attr_get16(value)
#define ZB_ZCL_ATTR_GETS16(value) zb_zcl_attr_gets16(value)
#define ZB_ZCL_ATTR_GET32(value) zb_zcl_attr_get32(value)
#define ZB_ZCL_ATTR_GETS32(value) zb_zcl_attr_gets32(value)

zb_int24_t zb_zcl_attr_get24(zb_uint8_t *value);
zb_uint48_t zb_zcl_attr_get48(zb_uint8_t *value);
#define ZB_ZCL_ATTR_GET24(value) zb_zcl_attr_get24(value)
#define ZB_ZCL_ATTR_GET48(value) zb_zcl_attr_get48(value)

#else

#define ZB_ZCL_ATTR_GET16(value) (*((zb_uint16_t *)value))
#define ZB_ZCL_ATTR_GETS16(value) (*((zb_int16_t *)value))
#define ZB_ZCL_ATTR_GET32(value) (*((zb_uint32_t *)value))
#define ZB_ZCL_ATTR_GETS32(value) (*((zb_int32_t *)value))

#define ZB_ZCL_ATTR_GET24(value) (*((zb_int24_t *)value))
#define ZB_ZCL_ATTR_GET48(value) (*((zb_uint48_t *)value))

#endif

/***************************** ZCL general functionality definitions ******************************/

/** @cond internals_doc */
/** @internal @brief Calculates array size */
#define ZB_ZCL_ARRAY_SIZE(ar, type) (sizeof(ar)/sizeof(type))

/** @internal @brief Calculates byte array size (add 2 bytes for full length). */
#define ZB_ZCL_ARRAY_GET_SIZE(ar, val) ZB_LETOH16(ar, val)
#define ZB_ZCL_ARRAY_SET_SIZE(ar, val) ZB_HTOLE16_VAL(ar, val)

/** @internal @brief Calculates 32-byte array size (add 2 bytes for full length). */
/* [EE] 04/09/2015 CR:MAJOR never cast byte* to int* */
#define ZB_BYTE_32ARRAY_GET_SIZE(ar, val) { ZB_ZCL_ARRAY_GET_SIZE(ar, val); *(zb_uint16_t*)(ar) *= 4; }
#define ZB_BYTE_32ARRAY_SET_SIZE(ar, val) { ZB_ZCL_ARRAY_SET_SIZE(ar, val); *(zb_uint16_t*)(ar) /= 4; }

#define ZB_ZCL_NULL_EP_ID (zb_uint8_t)(-1)
#define ZB_ZCL_NULL_ID (zb_uint16_t)(-1)
#define ZB_ZCL_NULL_STRING (zb_uint8_t)(0)

/** @internal @brief Maximum size of Character String (with Length octet) */
#define ZB_ZCL_MAX_STRING_SIZE 0xFF
#define ZB_ZCL_INVALID_STRING_VALUE 0xFF
#define ZB_ZCL_INVALID_ARRAY_VALUE 0xFFFF

/** @endcond */

/*! @} */ /* ZCL common types and definitions */

#ifdef DOXYGEN
/** @brief Fix data endian, if needed, and store it in the same buffer
    @param data_ptr - pointer to data value
    @param data_type - ZCL data type
 */
#define ZB_ZCL_FIX_ENDIAN(data_ptr, data_type)
#endif

/** @cond internals_doc */
#if ! defined ZB_LITTLE_ENDIAN

/** @internal @brief Fix data endian, if needed, and store it in the same buffer
    @param data_ptr - pointer to data value
    @param data_type - ZCL data type
 */
#define ZB_ZCL_FIX_ENDIAN(data_ptr, data_type) zb_zcl_fix_endian(data_ptr, data_type)

void zb_zcl_fix_endian(zb_uint8_t *data_ptr, zb_uint8_t data_type);

#define ZB_ZCL_HTOLE16_INPLACE(data_ptr)      \
  {                                           \
    zb_uint16_t tmp_place;                    \
    ZB_HTOLE16(&tmp_place, (data_ptr));       \
    ZB_ASSIGN_UINT16((data_ptr), &tmp_place); \
  }

#else

#define ZB_ZCL_FIX_ENDIAN(data_ptr, data_type) (ZVUNUSED((data_ptr)), ZVUNUSED((data_type)))

#define ZB_ZCL_HTOLE16_INPLACE(data_ptr) (ZVUNUSED((data_ptr)))

#endif /* ZB_LITTLE_ENDIAN */

#define ZB_ZCL_TRAN_TABLE_HASH_PARAM(param) ZB_1INT_HASH_FUNC(param) % ZCL_TRAN_TABLE_SIZE

/** @internal @brief ZCL hash entity, stores callback functions for commands specified by
 * buffer index */
typedef struct zcl_cb_hash_ent_s
{
  zb_callback_t func; /*!< callback pointer */
  zb_uint8_t    buf;  /*!< buffer index */
}
zcl_cb_hash_ent_t;

/** @endcond */

/** Command send status structure */
typedef struct zb_zcl_command_send_status_s
{
  zb_uint8_t status;          /*!< command send status */
  zb_uint8_t  dst_endpoint;   /*!< Destination endpoint */
  zb_zcl_addr_t dst_addr;     /*!< Destination address */
  zb_uint8_t  src_endpoint;   /*!< Source endpoint */
  /* add more fields here if needed */
}
zb_zcl_command_send_status_t;

/** @brief Set default value attribute callback
    @param ep - endpoint number
*/
typedef zb_void_t (*zb_zcl_set_default_value_attr_cb_t)(zb_uint8_t ep);

/** @brief Check attribute value callback
    @param cluster_id - Cluster ID
    @param cluster_role - cluster role (@ref zb_zcl_cluster_role_e)
    @param endpoint - endpoint number
    @param attr_id - attribute ID
    @param value - new attribute value

    @return RET_OK - if new value is valid, RET_ERROR - if new value is wrong or
            RET_IGNORE - if use default Check attribute functions
*/
typedef zb_ret_t (*zb_zcl_app_check_attr_value_cb_t)
    (zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *value);

/* ZCL common functions */

/*! @brief Get registered attribute description by cluster descriptor
    @param cluster_desc - cluster descriptor
    @param attr_id - attribute identifier
    @returns Pointer to attribute description, or NULL if no attribute description found.
*/
zb_zcl_attr_t* zb_zcl_get_attr_desc(zb_zcl_cluster_desc_t *cluster_desc, zb_uint16_t attr_id);

/** @brief Check registered attribute is writable one
    @param attr_desc - attribute descriptor
    @param endpoint - endpoint
    @param cluster_id - cluster identifier
    @param cluster_role - cluster role (@ref zb_zcl_cluster_role_e)
    @param new_value - new value for an attribute
    @param check_access - check READ_ONLY access or not
    @returns ZB_ZCL_STATUS_SUCCESS on success, error status otherwise
*/
zb_uint8_t zb_zcl_check_attribute_writable(
  zb_zcl_attr_t* attr_desc,
  zb_uint8_t endpoint,
  zb_uint16_t cluster_id,
  zb_uint8_t cluster_role,
  zb_uint8_t *new_value,
  zb_bool_t check_access);

/*!
  Hook on Write Attribute command
  @param endpoint - endpoint
  @param cluster_id - cluster ID
  @param cluster_role - cluster role (@ref zb_zcl_cluster_role_e)
  @param attr_id - attribute ID
  @param new_value - pointer to newly assigned value
*/
zb_void_t zb_zcl_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint8_t *new_value);

/**
 * @brief Get size of value of given attribute type
 *
 * @param attr_type - attribute type in question (see @ref zb_zcl_attr_type_t)
 * @param attr_value - pointer to value in question (needed for variable sized types)
 *
 * @return size in bytes or 0xFF if type is invalid
 */
zb_uint8_t zb_zcl_get_attribute_size(zb_uint8_t attr_type, zb_uint8_t *attr_value);

/**
 * @brief Get size of analog data type
 * @param attr_type - attribute type in question (see @ref zb_zcl_attr_type_t)
 *
 * @return size in bytes of 0xFF in type is not analog or invalid
 */
zb_uint8_t zb_zcl_get_analog_attribute_size(zb_uint8_t attr_type);

/**
 * @brief Check whether type of ZCL attrbiute is analog
 * @param attr_type - attribute type in question (see @ref zb_zcl_attr_type_t)
 *
 * @return ZB_TRUE if type is analog, ZB_FALSE otherwith
 */
zb_bool_t zb_zcl_is_analog_data_type(zb_uint8_t attr_type);

/**
 * @brief Check if attribute value is valid or not
 *
 * @param cluster_id - cluster ID
 * @param cluster_role - cluster role (@ref zb_zcl_cluster_role_e)
 * @param endpoint - endpoint
 * @param attr_id - attribute ID
 * @param value - pointer to attribute data
 *
 * @return ZB_TRUE if data value is valid, ZB_FALSE otherwise
 */
zb_bool_t zb_zcl_check_attr_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *value);

zb_bool_t zb_zcl_is_target_endpoint(zb_af_endpoint_desc_t *ep_desc, zb_uint16_t profile_id);

zb_uint8_t zb_zcl_get_next_target_endpoint(
  zb_uint8_t start_from_ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t profile_id);

/**
 * @brief Search endpoint for cluster with given id
 *
 * @param ep_desc - application endpoint description
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_e)
 * @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 * @return cluster descriptor or NULL if not present
 */
zb_zcl_cluster_desc_t* get_cluster_desc(zb_af_endpoint_desc_t* ep_desc, zb_uint16_t cluster_id, zb_uint8_t cluster_role);

/**
 * @brief Find endpoint which has cluster with given ID
 *
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_e)
 * @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 * @return endpoint number or 0 if not found
 */
zb_uint8_t get_endpoint_by_cluster(zb_uint16_t cluster_id, zb_uint16_t cluster_role);

zb_uint16_t get_profile_id_by_endpoint(zb_uint8_t endpoint_id);

/**
 * @brief Find endpoint which has cluster with given ID and role
 *
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_e)
 * @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 *
 * @return endpoint number or 0 if not found
 */
zb_af_endpoint_desc_t *get_endpoint_by_cluster_with_role(
    zb_uint16_t cluster_id,
    zb_uint8_t cluster_role);

/**
 * Find attribute descriptor by given endpoint number, cluster ID and attribute ID
 *
 * @param ep - endpoint number (must exist)
 * @param cluster_id - cluster ID (must exist on given ep, see @ref zb_zcl_cluster_id_t)
 * @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 * @param attr_id - attribute ID
 *
 * @return attribute descriptor
 */
zb_zcl_attr_t* zb_zcl_get_attr_desc_a(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id);

/**
 * Put attribute value to command packet, fix endian if needed.
 *
 * @param cmd_ptr - pointer to destination
 * @param attr_type - type of attribute being written (see @ref zb_zcl_attr_type_t)
 * @param attr_value - pointer to value to be put into packet
 *
 * @return command pointer adjusted to point at the tail of written value
*/
zb_uint8_t* zb_zcl_put_value_to_packet(
    zb_uint8_t *cmd_ptr, zb_uint8_t attr_type, zb_uint8_t *attr_value);


/**
 * @brief Sets attribute value, perform all needed checks before and after setting new value
 *
 * @param ep - endpoint number
 * @param cluster_id - cluster ID (see @ref zb_zcl_cluster_id_t)
 * @param cluster_role - role (see @ref zb_zcl_cluster_role_e)
 * @param attr_id - ID of the attribute being set
 * @param value - pointer to new value
 * @param check_access - whether read-only check should be performed
 *
 * @return ZB_ZCL_STATUS_SUCCESS if value was written \n
 *         ZB_ZCL_STATUS_READ_ONLY if attribute is read only and check_access is false
 *         ZB_ZCL_STATUS_UNSUP_ATTRIB if attribute is not supported by cluster
 *         ZB_ZCL_STATUS_INVALID_VALUE if current value of attribute is invalid
 * @note given endpoint with given cluster ID should exist
 */
zb_zcl_status_t zb_zcl_set_attr_val(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role,
                             zb_uint16_t attr_id, zb_uint8_t *value, zb_bool_t check_access);

/**
 * @brief Register callback that should be called when APS acknowledge is received for ZCL command
 *
 * @param param - pointer to buffer with ZCL command
 * @param cb - pointer to callback function
 */
zb_ret_t zb_zcl_register_cb(zb_uint8_t param, zb_callback_t cb);

/**
 * @brief Call callback on receiving APS acknowledge for ZCL command
 *
 * @param param - buffer with ZCL command
 *
 * @return RET_OK if callback was found and called
 *         RET_NOT_FOUND if no callback was registered
 */
zb_ret_t zb_zcl_ack_callback(zb_uint8_t param);

/**
 * Check whether ZCL command should be handled by stack
 *
 * @param param - reference to buffer with parsed ZCL command
 *
 * @return ZB_ZCL_STATUS_SUCCESS if command should be handled
 *         ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD if manufacturer specific command is not supported
 *         ZB_ZCL_STATUS_UNSUP_CLUST_CMD if general command is not supported
 *         ZB_ZCL_STATUS_UNSUP_CLUST if cluster with role specified in ZCL header is not supported
 */
zb_uint8_t zb_zcl_check_accept_command(zb_uint8_t param);

/** @brief Common continue command

    Usualy use after invoke User App. If invoke User App finished with error (status!=RET_OK),
    send error responce, else if not disabled default response send it or free buffer

    @param buffer - buffer for send response
    @param pcmd_info - pointer of cmd_info (see @ref zb_zcl_parsed_hdr_s)
    @param status - status of operation (see @ref zb_zcl_status_t)
*/
#define ZB_ZCL_PROCESS_COMMAND_FINISH(buffer, pcmd_info, status)        \
{                                                                       \
  if (ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*(pcmd_info), status))          \
  {                                                                     \
    ZB_ZCL_SEND_DEFAULT_RESP(                                           \
      (buffer),                                                         \
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).source.u.short_addr,    \
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                                 \
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).src_endpoint,           \
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).dst_endpoint,           \
      (pcmd_info)->profile_id,                                          \
      (pcmd_info)->cluster_id,                                          \
      (pcmd_info)->seq_number,                                          \
      (pcmd_info)->cmd_id,                                              \
      (status));                                                        \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    TRACE_MSG( TRACE_ZCL3, "Default response disabled", (FMT__0));      \
    zb_free_buf((buffer));                                              \
  }                                                                     \
}


/** @brief Declare for change attribute for User Application
 *
 * Note size of values - twice of maximum size of possible attribute
*/
typedef struct zb_zcl_set_attr_value_param_s
{
  zb_uint16_t cluster_id;   /**< Cluster ID*/
  zb_uint16_t attr_id;      /**< Attribute ID*/
  /*
   * Constant size of values - for correct alloc place in buffer tail, as
   * "ZB_GET_BUF_PARAM(buffer, zb_zcl_device_callback_param_t)".
   * Current size = maximum of fixed size of any attribute.
   * If will need to translate attribute with variable size then need
   * set value as size and pointer of attribule.
   * */
  /* CR:MEDIUM:FIXED why value size is equal to sizeof(zb_ieee_addr_t) ?
   * need to discuss */
  union
  {
    zb_uint8_t data8;           /**< 8bit data */
    zb_uint16_t data16;         /**< 16bit data */
    zb_uint24_t data24;         /**< 24bit data */
    zb_uint32_t data32;         /**< 32bit data */
    zb_uint48_t data48;         /**< 48bit data */
    zb_ieee_addr_t data_ieee;   /**< 64bit data or IEEE address */
    struct
    {
      zb_uint8_t size;          /**< Size of data (byte) */
      zb_uint8_t *p_data;       /**< Pointer of data */
    } data_variable;            /**< Variable size data - string or byte array */
  }
  values;
} zb_zcl_set_attr_value_param_t;

/** @cond internals_doc */
/** @internal @brief Inform User Application about change attribute
 *  @param buffer - buffer for transmite data
 *  @param clusterId - cluster ID
 *  @param attrDesc - attribute description
 *  @param p_value - pointer of new value attribute
 *  @param result - [out] result user callback
*/
#define ZB_ZCL_INVOKE_USER_APP_SET_ATTR_WITH_RESULT(                           \
  buffer, ep, clusterId, attrDesc, p_value, result)                            \
{                                                                              \
  (result) = RET_OK;                                                           \
                                                                               \
  if (ZCL_CTX().device_cb)                                                     \
  {                                                                            \
    zb_zcl_device_callback_param_t *data = NULL;                               \
    zb_uint8_t size =                                                          \
      zb_zcl_get_attribute_size((attrDesc)->type, (attrDesc)->data_p);         \
                                                                               \
    switch ((zb_zcl_attr_type_t)(attrDesc)->type)                              \
    {                                                                          \
      case ZB_ZCL_ATTR_TYPE_OCTET_STRING:                                      \
      case ZB_ZCL_ATTR_TYPE_CHAR_STRING:                                       \
      {                                                                        \
        zb_uint8_t *ptr =                                                      \
          (zb_uint8_t*)ZB_GET_BUF_TAIL(                                        \
            (buffer), sizeof(zb_zcl_device_callback_param_t) + size);          \
                                                                               \
        ZB_MEMCPY(ptr, (p_value), size);                                       \
        data = (zb_zcl_device_callback_param_t*)(void*)(ptr + size);           \
        data->cb_param.set_attr_value_param.values.data_variable.size = size;  \
        data->cb_param.set_attr_value_param.values.data_variable.p_data = ptr; \
      }                                                                        \
      break;                                                                   \
                                                                               \
      default:                                                                 \
        data = ZB_GET_BUF_PARAM((buffer), zb_zcl_device_callback_param_t);     \
        ZB_MEMCPY(&(data->cb_param.set_attr_value_param.values),               \
                  (p_value),                                                   \
                  size);                                                       \
      break;                                                                   \
    }                                                                          \
                                                                               \
    if (data)                                                                  \
    {                                                                          \
      data->device_cb_id = ZB_ZCL_SET_ATTR_VALUE_CB_ID;                        \
      data->endpoint = (ep);                                                   \
      data->attr_type = (zb_zcl_attr_access_t) (attrDesc)->type;               \
      data->cb_param.set_attr_value_param.cluster_id = (clusterId);            \
      data->cb_param.set_attr_value_param.attr_id = (attrDesc)->id;            \
      (ZCL_CTX().device_cb)(ZB_REF_FROM_BUF((buffer)));                        \
      (result) = data->status;                                                 \
    }                                                                          \
  }                                                                            \
}

/** @internal @brief Inform User Application about change attribute
 *  @param buffer - buffer for transmite data
 *  @param clusterId - cluster ID
 *  @param attrDesc - attribute description
 *  @param p_value - pointer of new value attribute
 */
#define ZB_ZCL_INVOKE_USER_APP_SET_ATTR(                                       \
  buffer, ep, clusterId, attrDesc, p_value)                                    \
{                                                                              \
  if (ZCL_CTX().device_cb)                                                     \
  {                                                                            \
    zb_zcl_device_callback_param_t *data = NULL;                               \
    zb_uint8_t size =                                                          \
      zb_zcl_get_attribute_size((attrDesc)->type, (attrDesc)->data_p);         \
                                                                               \
    switch ((zb_zcl_attr_type_t)(attrDesc)->type)                              \
    {                                                                          \
      case ZB_ZCL_ATTR_TYPE_OCTET_STRING:                                      \
      case ZB_ZCL_ATTR_TYPE_CHAR_STRING:                                       \
      {                                                                        \
        zb_uint8_t *ptr =                                                      \
          (zb_uint8_t*)ZB_GET_BUF_TAIL(                                        \
            (buffer), sizeof(zb_zcl_device_callback_param_t) + size);          \
                                                                               \
        ZB_MEMCPY(ptr, (p_value), size);                                       \
        data = (zb_zcl_device_callback_param_t*)(void*)(ptr + size);           \
        data->cb_param.set_attr_value_param.values.data_variable.size = size;  \
        data->cb_param.set_attr_value_param.values.data_variable.p_data = ptr; \
      }                                                                        \
      break;                                                                   \
                                                                               \
      default:                                                                 \
        data = ZB_GET_BUF_PARAM((buffer), zb_zcl_device_callback_param_t);     \
        ZB_MEMCPY(&(data->cb_param.set_attr_value_param.values),               \
                  (p_value),                                                   \
                  size);                                                       \
      break;                                                                   \
    }                                                                          \
                                                                               \
    if (data)                                                                  \
    {                                                                          \
      data->device_cb_id = ZB_ZCL_SET_ATTR_VALUE_CB_ID;                        \
      data->endpoint = (ep);                                                   \
      data->attr_type = (zb_zcl_attr_access_t) (attrDesc)->type;               \
      data->cb_param.set_attr_value_param.cluster_id = (clusterId) ;           \
      data->cb_param.set_attr_value_param.attr_id = (attrDesc)->id;            \
      (ZCL_CTX().device_cb)(ZB_REF_FROM_BUF((buffer)));                        \
    }                                                                          \
  }                                                                            \
}

/** @endcond */

/**
 * @}
 */

zb_void_t zb_zcl_schedule_status_abort(zb_buf_t* buffer, zb_addr_u *addr,
                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                  zb_uint8_t ep, zb_callback_t cb);

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ! defined ZB_ZCL_COMMON_H */
