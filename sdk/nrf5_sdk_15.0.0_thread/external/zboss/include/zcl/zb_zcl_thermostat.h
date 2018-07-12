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
 * PURPOSE: Thermostat cluster defintions
*/

#if ! defined ZB_ZCL_THERMOSTAT_H
#define ZB_ZCL_THERMOSTAT_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"


/** @cond DOXYGEN_ZCL_SECTION */

/* Cluster ZB_ZCL_CLUSTER_ID_THERMOSTAT */

/** @addtogroup ZB_ZCL_THERMOSTAT
 *  @{
 */

/* <31.07.13>  Tokarev CR function prototype moved from C file into header for public access */
/**
 * Thermostat initialization function
 * initialize setpoint mode, shedule table
 */
zb_void_t zb_zcl_thermostat_init();
/* <31.07.13>  Tokarev CR function prototype moved from C file into header for public access */

/*! @name Thermostat cluster attributes
    @{
*/

/*! @brief Thermostat cluster attribute identifiers
    @see ZCL spec, subclause 6.3.2.2
*/
enum zb_zcl_thermostat_attr_e
{
  /*! @brief Local Temperature attribute */
  ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID             = 0x0000,
  /*! @brief Local Temperature Calibration */
  ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID = 0x0010,
  /*! @brief Occupied Cooling Setpoint attribute */
  ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID     = 0x0011,
  /*! @brief Occupied Heating Setpoint attribute */
  ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID     = 0x0012,
  /*! @brief Unoccupied Cooling Setpoint attribute */
  ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_COOLING_SETPOINT_ID   = 0x0013,
  /*! @brief Unoccupied Heating Setpoint attribute */
  ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID   = 0x0014,
  /*! @brief Control Sequence Of Operation attribute */
  ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID = 0x001b,
  /*! @brief System Mode attribute */
  ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID                   = 0x001c,
  /*! @brief AC Louver position attribute */
  ZB_ZCL_ATTR_THERMOSTAT_AC_LOUVER_POSITION_ID            = 0x0045,
};

/*! @brief Values for Control Sequence Of Operation attribute,
    @see ZCL spec, subclause 6.4.2.2.1 */
enum zb_zcl_thermostat_control_sequence_of_operation_e
{
  /*! Cooling Only value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_ONLY                            = 0x00,
  /*! Cooling With Reheat value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_WITH_REHEAT                     = 0x01,
  /*! Heating Only value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_ONLY                            = 0x02,
  /*! Heating With Reheat value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_WITH_REHEAT                     = 0x03,
  /*! Cooling and Heating 4-pipes value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_AND_HEATING_4_PIPES             = 0x04,
  /*! Cooling and Heating 4-pipes with Reheat value */
  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_AND_HEATING_4_PIPES_WITH_REHEAT = 0x05,

  ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_RESERVED                                = 0x06
};

/*! @brief Values for System Mode attribute,
    @see ZCL spec, subclause 6.4.2.2.1 */
enum zb_zcl_thermostat_system_mode_e
{
  /*! Off value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_OFF               = 0x00,
  /*! Auto value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_AUTO              = 0x01,
  /*! Cool value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_COOL              = 0x03,
  /*! Heat value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT              = 0x04,
  /*! Emergency Heating value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_EMERGENCY_HEATING = 0x05,
  /*! Precooling value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_PRECOOLING        = 0x06,
  /*! Fan Only value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_FAN_ONLY          = 0x07,
  /* 07/02/2015 CR [AV] */
  /*! Unknown system mode value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_RESERVED = 0X08
};


/*! @brief Values for AC Louver position attribute */
enum zb_zcl_thermostat_ac_louver_position_e
{
  ZB_ZCL_THERMOSTAT_LOUVER_FULLY_CLOSED        = 0x01,
  ZB_ZCL_THERMOSTAT_LOUVER_FULLY_OPENED        = 0x02,
  ZB_ZCL_THERMOSTAT_LOUVER_QUARTER_OPEN        = 0x03,
  ZB_ZCL_THERMOSTAT_LOUVER_HALF_OPEN           = 0x04,
  ZB_ZCL_THERMOSTAT_LOUVER_THREE_QUARTERS_OPEN = 0x05,
};

/** @brief Min value for Local Temperature attribute */
#define ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_MIN_VALUE 0x954d

/** @brief Max value for Local Temperature attribute */
#define ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_MAX_VALUE 0x7fff

/** @brief Invalid value for Local Temperature attribute */
#define ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_INVALID 0x8000

/** @brief Min value for Local Temperature Calibration attribute */
#define ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MIN_VALUE 0xe7

/** @brief Max value for Local Temperature Calibration attribute */
#define ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MAX_VALUE 0x19

/** @brief Default value for Occupied Cooling Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_DEFAULT_VALUE 0x0a28

/** @brief Min value for Occupied Cooling Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MIN_VALUE 0x954d

/** @brief Max value for Occupied Cooling Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MAX_VALUE 0x7fff

/** @brief Default value for Occupied Heating Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_DEFAULT_VALUE 0x07d0

/** @brief Min value for Occupied Heating Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MIN_VALUE 0x954d

/** @brief Max value for Occupied Heating Setpoint attribute */
#define ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MAX_VALUE 0x7fff

/** @brief Default value for Control Sequence Of Operation attribute */
#define ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_DEFAULT_VALUE 0x04

/** @brief Default value for System Mode attribute */
#define ZB_ZCL_THERMOSTAT_CONTROL_SYSTEM_MODE_DEFAULT_VALUE 0x01

/*! @} */ /* Thermostat cluster attributes */

/*! @name Thermostat cluster commands
    @{
*/

/*! @brief Thermostat cluster command identifiers
    @see ZCL spec, subclause 6.3.2.3
*/
enum zb_zcl_thermostat_cmd_e
{
  /** Setpoint Raise/Lower command */
  ZB_ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER  = 0x00,
};

/*! @} */ /* Thermostat cluster commands */

/*!
    @cond internals_doc
    @internal @name Thermostat cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID(data_ptr)             \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID,                                                   \
  ZB_ZCL_ATTR_TYPE_S16,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                                   \
  (zb_voidp_t) data_ptr                                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID(data_ptr) \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_S8,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                 \
  (zb_voidp_t) data_ptr                                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID(data_ptr)     \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_S16,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID(data_ptr)     \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_S16,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID(data_ptr) \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID,                                       \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                 \
  (zb_voidp_t) data_ptr                                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID(data_ptr)                   \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID,                                                         \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_ACCESS_SCENE,                                      \
  (zb_voidp_t) data_ptr                                                                          \
}

/** @internal @brief Declare attribute list for Thermostat cluster
    @param attr_list - attribute list name
    @param local_temperature - pointer to variable to store Local Temperature attribute value
    @param local_temperature_calibration - pointer to variable to store Local Temperature Calibration attribute value
    @param occupied_cooling_setpoint - pointer to variable to store Occupied Cooling Setpoint attribute value
    @param occupied_heating_setpoint - pointer to variable to store Occupied Heating Setpoint attribute value
    @param control_seq_of_operation - pointer to variable to store Control Sequence Of Operation attribute value
    @param system_mode - pointer to variable to store System Mode attribute value
*/
#define ZB_ZCL_DECLARE_THERMOSTAT_ATTRIB_LIST(attr_list,                                                \
      local_temperature, local_temperature_calibration, occupied_cooling_setpoint,                      \
      occupied_heating_setpoint, control_seq_of_operation, system_mode)                                 \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID, (local_temperature))                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID, (local_temperature_calibration))                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID, (occupied_cooling_setpoint))        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID, (occupied_heating_setpoint))        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID, (control_seq_of_operation))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID, (system_mode))                            \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Thermostat cluster */
#define ZB_ZCL_THERMOSTAT_REPORT_ATTR_COUNT 1

/***************************** commands *****************************/

/** @internal Structure for Setpoint Raise/Lower command
 */
typedef ZB_PACKED_PRE struct zb_zcl_thermostat_setpoint_raise_lower_req_s
{
  zb_uint8_t mode;
  zb_int8_t amount;
} ZB_PACKED_STRUCT zb_zcl_thermostat_setpoint_raise_lower_req_t;

/*! @brief Values for Mode field,
    @see ZCL spec, subclause 6.3.2.3.1.2 */
enum zb_zcl_thermostat_setpoint_raise_lower_mode_e
{
  /*! Heat value */
  ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_HEAT     = 0x00,
  /*! Cool value */
  ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_COOL     = 0x01,
  /*! Both (Heat and Cool) value */
  ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_BOTH     = 0x02,

  ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_RESERVED = 0x03
};

#define ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_REQ_PAYLOAD_LEN \
  sizeof(zb_zcl_thermostat_setpoint_raise_lower_req_t)

/*! @brief Send Setpoint Raise/Lower command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param mode - Mode value
    @param amount - Amount value
*/
#define ZB_ZCL_THERMOSTAT_SEND_SETPOINT_RAISE_LOWER_REQ(                                \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, mode, amount)         \
{                                                                                       \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                        \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER);                                    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (mode));                                                 \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (amount));                                               \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                    \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                            \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_THERMOSTAT, cb);\
}

/*!
  @brief Parses Get Setpoint Raise/Lower command and fills data request
  structure.
  @param data_buf - pointer to zb_buf_t buffer containing command request data
  @param setpoint_raise_lower_req - variable to save command request
  @param status - success or not read parameters - ZB_TRUE or ZB_FALSE
  @note data_buf buffer should contain command request payload without ZCL header.
*/
#define ZB_ZCL_THERMOSTAT_GET_SETPOINT_RAISE_LOWER_REQ(data_buf, req, status)           \
{                                                                                       \
  zb_zcl_thermostat_setpoint_raise_lower_req_t *req_ptr =                               \
     ZB_BUF_LEN(data_buf) >= ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_REQ_PAYLOAD_LEN ?   \
         (zb_zcl_thermostat_setpoint_raise_lower_req_t*)ZB_BUF_BEGIN(data_buf) : NULL;  \
  if (req_ptr)                                                                          \
  {                                                                                     \
    ZB_MEMCPY(&(req), req_ptr, sizeof(zb_zcl_thermostat_setpoint_raise_lower_req_t));   \
    status = ZB_ZCL_PARSE_STATUS_SUCCESS;                                               \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    status = ZB_ZCL_PARSE_STATUS_FAILURE;                                               \
  }                                                                                     \
}

/*! @}
    @endcond */ /* Thermostat cluster internals */

/*! @} */ /* ZCL HA Thermostat cluster definitions */

/** @brief Struct with Context Thermostat cluster */
typedef struct zb_zcl_thermostat_context_s
{
  zb_uint8_t setpoint_mode;
}
zb_zcl_thermostat_context_t;

/** @brief Declare run Thermostat command for User Application
*/
typedef ZB_PACKED_PRE struct zb_zcl_thermostat_value_param_s
{
  zb_uint8_t mode;              /*!< Mode for Sequence */
  zb_uint16_t heat_setpoint;    /*!< Heat Set Point */
  zb_uint16_t cool_setpoint;    /*!< Cool Set Point */
} zb_zcl_thermostat_value_param_t;

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_thermostat_init_server();
zb_void_t zb_zcl_thermostat_init_client();
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT_SERVER_ROLE_INIT zb_zcl_thermostat_init_server
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT_CLIENT_ROLE_INIT zb_zcl_thermostat_init_client

#endif /* ! defined ZB_ZCL_THERMOSTAT_H */
