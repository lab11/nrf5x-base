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

#if ! defined ZB_ZCL_THERMOSTAT_HA_ADDS_H
#define ZB_ZCL_THERMOSTAT_HA_ADDS_H

#include "zcl/zb_zcl_thermostat.h"

/** @cond DOXYGEN_HA_SECTION */

/** @addtogroup ZB_ZCL_THERMOSTAT_HA_ADDS
 *  @ingroup ZB_HA_CLUSTERS_ADDS
  * @{
  *   @details
  *   HA extensions for ZCL thermostat.
  */

/*! @name Thermostat cluster attributes
    @{
*/

/*! @brief Thermostat cluster attribute identifiers
    @see ZCL HA spec, subclause 10.1.3.2.3
*/
enum zb_zcl_thermostat_attr_ha_e
{
  /*! @brief Start of Week attribute */
  ZB_ZCL_ATTR_THERMOSTAT_START_OF_WEEK_ID                 = 0x0020,
};

/*! @brief Values for System Mode attribute,
    @see HA ZCL spec, subclause 10.1.3.2.2.2 */
enum zb_zcl_thermostat_system_mode_ha_e
{
  /*! Dry value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_DRY               = 0x08,
  /*! Sleep value */
  ZB_ZCL_THERMOSTAT_SYSTEM_MODE_SLEEP             = 0x09,
};

/*! @brief Values for Start of Week attribute,
    @see ZCL spec, subclause 6.4.2.2.1 */
enum zb_zcl_thermostat_start_of_week_e
{
  /*! Sunday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_SUNDAY         = 0x00,
  /*! Monday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_MONDAY         = 0x01,
  /*! Tuesday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_TUESDAY        = 0x02,
  /*! Wednesday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_WEDNESDAY      = 0x03,
  /*! Thursday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_THURSDAY       = 0x04,
  /*! Friday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_FRIDAY         = 0x05,
  /*! Saturday value */
  ZB_ZCL_THERMOSTAT_START_OF_WEEK_SATURDAY       = 0x06,
};

/* Default value for Start Of Week attribute */
#define ZB_ZCL_THERMOSTAT_START_OF_WEEK_DEFAULT_VALUE ZB_ZCL_THERMOSTAT_START_OF_WEEK_SUNDAY

/*! @} */ /* Thermostat cluster attributes */

/*! @name Thermostat cluster commands
    @{
*/

/*! @brief Thermostat cluster command identifiers
    @see ZCL spec, subclause 10.1.3.3
*/
enum zb_zcl_thermostat_cmd_ha_e
{
  /** Set Weekly Schedule command */
  ZB_ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE   = 0x01,
  /** Get Weekly Schedule command */
  ZB_ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE   = 0x02,
  /** Clear Weekly Schedule command */
  ZB_ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE = 0x03,
  /** Get Relay Status Log command */
  ZB_ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG  = 0x04
};

/*! @brief Thermostat cluster command identifiers
    @see ZCL spec, subclause 6.3.2.3
*/
enum zb_zcl_thermostat_cmd_resp_ha_e
{
  /** Get Weekly Schedule response command */
  ZB_ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE_RESP  = 0x00,
  /** Get Relay Status Log response command */
  ZB_ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG_RESP = 0x01
};

/*! @} */ /* Thermostat cluster commands */

/*! @cond internals_doc
    @name Thermostat cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_THERMOSTAT_START_OF_WEEK_ID(data_ptr)                 \
{                                                                                                \
  ZB_ZCL_ATTR_THERMOSTAT_START_OF_WEEK_ID,                                                       \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                 \
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
    @param start_of_week - pointer to variable to store Start of Week attribute value
*/
#define ZB_ZCL_DECLARE_THERMOSTAT_ATTRIB_LIST_HA(attr_list,                                             \
      local_temperature, local_temperature_calibration, occupied_cooling_setpoint,                      \
      occupied_heating_setpoint, control_seq_of_operation, system_mode, start_of_week)                  \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID, (local_temperature))                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID, (local_temperature_calibration))                        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID, (occupied_cooling_setpoint))        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID, (occupied_heating_setpoint))        \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID, (control_seq_of_operation))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID, (system_mode))                            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_THERMOSTAT_START_OF_WEEK_ID, (start_of_week))                        \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/***************************** commands *****************************/

/* Add/Get Weekly Schedule record for Set/Get Weekly Schedule commands */

/** @internal Structure for Weekly Schedule record
 */
typedef ZB_PACKED_PRE struct zb_zcl_thermostat_weekly_schedule_point_pair_s
{
  zb_uint16_t transition_time;
  zb_uint16_t heat_set_point;
  zb_uint16_t cool_set_point;
} ZB_PACKED_STRUCT zb_zcl_thermostat_weekly_schedule_point_pair_t;

#define ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_POINT_PAIR_FULL_SIZE \
  sizeof(zb_zcl_thermostat_weekly_schedule_point_pair_t)

#define ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_POINT_PAIR_ONE_SIZE \
  sizeof(zb_zcl_thermostat_weekly_schedule_point_pair_t) - sizeof(zb_uint16_t)

#define ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MAX_NUM_OF_TRANSITIONS 10

/*! @brief Values for Mode for Sequence field,
    @see ZCL spec, subclause 10.1.3.3.1.4 */
enum zb_zcl_thermostat_weekly_schedule_mode_for_seq_e
{
  /*! Heat value */
  ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_HEAT     = 0x01,
  /*! Cool value */
  ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_COOL     = 0x02,
  /*! Both (Heat and Cool) value */
  ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_BOTH     = 0x03,

  ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_RESERVED = 0x04
};

/** @brief Add attribute value to command payload
   @param cmd_ptr - pointer to a command data memory
   @param mode_for_seq - Mode For Sequence value (from header)
   @param transition_time - Transition Time value
   @param heat_set_point - Heat Set Point value
   @param cool_set_point - Cool Set Point value
*/
#define ZB_ZCL_THERMOSTAT_ADD_VALUE_WEEKLY_SCHEDULE(cmd_ptr,                    \
      mode_for_seq, transition_time, heat_set_point, cool_set_point)            \
{                                                                               \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_ptr, (transition_time));                     \
  if (mode_for_seq & ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_HEAT)       \
  {                                                                             \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_ptr, (heat_set_point));                    \
  }                                                                             \
  if (mode_for_seq & ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_COOL)       \
  {                                                                             \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_ptr, (cool_set_point));                    \
  }                                                                             \
}

/** @brief Parses Set Weekly Schedule request and returns next Set point pair record or NULL if there
  * is no more data.

   If request contains invalid data, NULL is returned.
   @param data_buf - pointer to zb_buf_t buffer containing write attribute request data
   @param mode_for_seq - Mode For Sequence value (from Set Weekly Schedule header)
   @param pair_req - out pointer to Set Point Pair record
 */
#define ZB_ZCL_THERMOSTAT_GET_NEXT_WEEKLY_SCHEDULE_POINT_PAIR_REQ(data_buf, mode_for_seq, pair_req, status) \
{                                                                                       \
  zb_zcl_thermostat_weekly_schedule_point_pair_t *pair_req_ptr;                         \
  zb_uint8_t req_size;                                                                  \
                                                                                        \
  if (mode_for_seq == ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_BOTH)              \
  {                                                                                     \
    req_size = ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_POINT_PAIR_FULL_SIZE;                  \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    req_size = ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_POINT_PAIR_ONE_SIZE;                   \
  }                                                                                     \
                                                                                        \
  pair_req_ptr = (ZB_BUF_LEN(data_buf) >= req_size) ?                                   \
      (zb_zcl_thermostat_weekly_schedule_point_pair_t*)ZB_BUF_BEGIN(data_buf) : NULL;   \
  if (pair_req_ptr)                                                                     \
  {                                                                                     \
    ZB_MEMCPY(&(pair_req), pair_req_ptr, req_size);                                     \
                                                                                        \
    ZB_ZCL_HTOLE16_INPLACE(&(pair_req.transition_time));                                \
    ZB_ZCL_HTOLE16_INPLACE(&(pair_req.heat_set_point));                                 \
    if (mode_for_seq == ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MODE_FOR_SEQ_COOL)            \
    {                                                                                   \
      (pair_req).cool_set_point = (pair_req).heat_set_point;                            \
    }                                                                                   \
                                                                                        \
    ZB_BUF_CUT_LEFT2((data_buf), req_size);                                             \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                             \
  }                                                                                     \
}

/* Set Weekly Schedule command */

typedef ZB_PACKED_PRE struct zb_zcl_thermostat_set_weekly_schedule_req_s
{
  zb_uint8_t num_of_transitions;
  zb_uint8_t day_of_week;
  zb_uint8_t mode_for_seq;
} ZB_PACKED_STRUCT zb_zcl_thermostat_set_weekly_schedule_req_t;

#define ZB_ZCL_THERMOSTAT_SET_WEEKLY_SCHEDULE_REQ_SIZE \
  sizeof(zb_zcl_thermostat_set_weekly_schedule_req_t)

/** @brief Initialize Set Weekly Schedule command
    @param buffer - buffer to store command data
    @param cmd_ptr - pointer to a command data memory
    @param def_resp - enable/disable default response
    @param num_of_transition - Number Of Transition value
    @param day_of_week - Day Of Week value
    @param mode_for_seq - Mode For Sequence value
*/
#define ZB_ZCL_THERMOSTAT_INIT_SET_WEEKLY_SCHEDULE_REQ(buffer,                  \
    cmd_ptr, def_resp, num_of_transition, day_of_week, mode_for_seq)            \
{                                                                               \
  cmd_ptr = ZB_ZCL_START_PACKET(buffer);                                        \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(cmd_ptr, def_resp);       \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(cmd_ptr, ZB_ZCL_GET_SEQ_NUM(),                \
      ZB_ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE);                               \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (num_of_transition));                        \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (day_of_week));                              \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (mode_for_seq));                             \
}

/** @brief Send Set Weekly Schedule command
    @param buffer - buffer to store command data
    @param cmd_ptr - pointer to a command data memory
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param profile_id - profile identifier
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_THERMOSTAT_SEND_SET_WEEKLY_SCHEDULE_REQ(                                 \
    buffer, cmd_ptr, addr, dst_addr_mode, dst_ep, ep, profile_id, cb)                   \
{                                                                                       \
  ZB_ZCL_FINISH_PACKET(buffer, cmd_ptr)                                                \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep, profile_id,        \
      ZB_ZCL_CLUSTER_ID_THERMOSTAT, cb);                                                \
}

/** @brief Parses Set Weekly Schedule request header

   If request contains invalid data, status is ZB_FALSE.
   @param data_buf - pointer to zb_buf_t buffer containing write attribute request data
   @param req - record of type @ref zb_zcl_thermostat_set_weekly_schedule_req_t
   @param status - result of parsing
 */
#define ZB_ZCL_THERMOSTAT_GET_SET_WEEKLY_SCHEDULE_REQ(data_buf, req, status)                    \
{                                                                                               \
  zb_zcl_thermostat_set_weekly_schedule_req_t *req_ptr =                                        \
        ZB_BUF_LEN(data_buf) >= ZB_ZCL_THERMOSTAT_SET_WEEKLY_SCHEDULE_REQ_SIZE ?                \
              (zb_zcl_thermostat_set_weekly_schedule_req_t*)ZB_BUF_BEGIN(data_buf) : NULL;      \
  if (req_ptr)                                                                                  \
  {                                                                                             \
    ZB_MEMCPY(&(req), req_ptr, sizeof(zb_zcl_thermostat_set_weekly_schedule_req_t));            \
    ZB_ASSERT(req.num_of_transitions <= ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MAX_NUM_OF_TRANSITIONS);\
    status = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                       \
                                                                                                \
    ZB_BUF_CUT_LEFT2((data_buf), sizeof(zb_zcl_thermostat_set_weekly_schedule_req_t));          \
  }                                                                                             \
  else                                                                                          \
  {                                                                                             \
    status = ZB_ZCL_PARSE_STATUS_FAILURE;                                                       \
  }                                                                                             \
}

/* Get Weekly Schedule command */

typedef ZB_PACKED_PRE struct zb_zcl_thermostat_get_weekly_schedule_req_s
{
  zb_uint8_t days_to_return;
  zb_uint8_t mode_to_return;
} ZB_PACKED_STRUCT zb_zcl_thermostat_get_weekly_schedule_req_t;

#define ZB_ZCL_THERMOSTAT_GET_WEEKLY_SCHEDULE_REQ_PAYLOAD_LEN \
  sizeof(zb_zcl_thermostat_get_weekly_schedule_req_t)

/*! @brief Send Get Weekly Schedule command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param days_to_return - Days To Return value
    @param mode_to_return - Mode To Return value
*/
#define ZB_ZCL_THERMOSTAT_SEND_GET_WEEKLY_SCHEDULE_REQ(                 \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb,       \
  days_to_return, mode_to_return)                                       \
{                                                                       \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                        \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp);   \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),            \
        ZB_ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE);                     \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (days_to_return));                       \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (mode_to_return));                       \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                    \
  ZB_ZCL_SEND_COMMAND_SHORT((buffer), (addr), (dst_addr_mode),          \
        (dst_ep), (ep), (prfl_id), ZB_ZCL_CLUSTER_ID_THERMOSTAT, (cb)); \
}

/*!
  @brief Parses Get Weekly Schedule command and fills data request
  structure.
  @param data_buf - pointer to zb_buf_t buffer containing command request data
  @param req - record of type @ref zb_zcl_thermostat_set_weekly_schedule_req_t
  @param status - success or not read parameters - ZB_ZCL_PARSE_STATUS_SUCCESS or
  ZB_ZCL_PARSE_STATUS_FAILURE
  @note data_buf buffer should contain command request payload without ZCL header.
*/
#define ZB_ZCL_THERMOSTAT_GET_GET_WEEKLY_SCHEDULE_REQ(data_buf, req, status)            \
{                                                                                       \
  zb_zcl_thermostat_get_weekly_schedule_req_t *req_ptr =                                \
        ZB_BUF_LEN(data_buf) >= ZB_ZCL_THERMOSTAT_GET_WEEKLY_SCHEDULE_REQ_PAYLOAD_LEN ? \
            (zb_zcl_thermostat_get_weekly_schedule_req_t*)ZB_BUF_BEGIN(data_buf) : NULL;\
  if (req_ptr)                                                                          \
  {                                                                                     \
    ZB_MEMCPY(&(req), req_ptr, sizeof(zb_zcl_thermostat_get_weekly_schedule_req_t));    \
    status = ZB_ZCL_PARSE_STATUS_SUCCESS;                                               \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    status = ZB_ZCL_PARSE_STATUS_FAILURE;                                               \
  }                                                                                     \
}

/* Get Weekly Schedule response command */

/** Structure for Get Weekly Schedule resp command
 */
typedef ZB_PACKED_PRE struct zb_zcl_thermostat_get_weekly_schedule_resp_s
{
  zb_uint8_t num_of_transitions;
  zb_uint8_t day_of_week;
  zb_uint8_t mode_for_seq;
} ZB_PACKED_STRUCT zb_zcl_thermostat_get_weekly_schedule_resp_t;

/** @brief Initialize Get Weekly Schedule resp command
    @param buffer - buffer to store command data
    @param cmd_ptr - pointer to a command data memory
    @param seq - sequency
    @param num_of_transitions - Number Of Transition value
    @param day_of_week - Day Of Week value
    @param mode_for_seq - Mode For Sequence value
*/
#define ZB_ZCL_THERMOSTAT_INIT_GET_WEEKLY_SCHEDULE_RESP(buffer, cmd_ptr, seq,   \
    num_of_transitions, day_of_week, mode_for_seq)                              \
{                                                                               \
  cmd_ptr = ZB_ZCL_START_PACKET(buffer);                                        \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(cmd_ptr);                 \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(cmd_ptr, (seq),                               \
        ZB_ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE_RESP);                        \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (num_of_transitions));                       \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (day_of_week));                              \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, (mode_for_seq));                             \
}

/** @brief Send Get Weekly Schedule resp command
    @param buffer - buffer to store command data
    @param cmd_ptr - pointer to a command data memory
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param profile_id - profile identifier
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_THERMOSTAT_SEND_GET_WEEKLY_SCHEDULE_RESP(                        \
    buffer, cmd_ptr, addr, dst_addr_mode, dst_ep, ep, profile_id, cb)           \
{                                                                               \
  ZB_ZCL_FINISH_PACKET(buffer, cmd_ptr)                                        \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep, profile_id,\
      ZB_ZCL_CLUSTER_ID_THERMOSTAT, cb);                                        \
}

/** @brief Parses Get Weekly Schedule response header

   If request contains invlid data, status is ZB_FALSE.
   @param data_buf - pointer to zb_buf_t buffer containing write attribute request data
   @param resp - record of type @ref zb_zcl_thermostat_get_weekly_schedule_resp_t
   @param status - result of parsing
 */
#define ZB_ZCL_THERMOSTAT_GET_GET_WEEKLY_SCHEDULE_RESP(data_buf, resp, status)          \
{                                                                                       \
  zb_zcl_thermostat_get_weekly_schedule_resp_t *resp_ptr =                              \
        ZB_BUF_LEN(data_buf) >= sizeof(zb_zcl_thermostat_get_weekly_schedule_resp_t) ?  \
              (zb_zcl_thermostat_get_weekly_schedule_resp_t*)ZB_BUF_BEGIN(data_buf) : NULL; \
  if (resp_ptr)                                                                         \
  {                                                                                     \
    ZB_MEMCPY(&(resp), resp_ptr, sizeof(zb_zcl_thermostat_get_weekly_schedule_resp_t)); \
    ZB_ASSERT(resp.num_of_transitions <=                                                \
              ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_MAX_NUM_OF_TRANSITIONS);                \
    status = ZB_ZCL_PARSE_STATUS_SUCCESS;                                               \
                                                                                        \
    ZB_BUF_CUT_LEFT2((data_buf), sizeof(zb_zcl_thermostat_get_weekly_schedule_resp_t)); \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    status = ZB_ZCL_PARSE_STATUS_FAILURE;                                               \
  }                                                                                     \
}

/* Clear Weekly Schedule command */
/*! @brief Clear Weekly Schedule command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_THERMOSTAT_SEND_CLEAR_WEEKLY_SCHEDULE_REQ(                       \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb)             \
{                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp);           \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),                    \
      ZB_ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE);                             \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                            \
  ZB_ZCL_SEND_COMMAND_SHORT((buffer), (addr), (dst_addr_mode), (dst_ep),        \
      (ep), (prfl_id), ZB_ZCL_CLUSTER_ID_THERMOSTAT, (cb));                     \
}

/* Get Relay Status Log command */
/*! @brief Send Get Relay Status Log req command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_THERMOSTAT_SEND_GET_RELAY_STATUS_LOG_REQ(                        \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb)             \
{                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp);           \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, ZB_ZCL_GET_SEQ_NUM(),                    \
      ZB_ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG);                              \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                            \
  ZB_ZCL_SEND_COMMAND_SHORT((buffer), (addr), (dst_addr_mode), (dst_ep),        \
      (ep), (prfl_id), ZB_ZCL_CLUSTER_ID_THERMOSTAT, (cb));                     \
}

/* Get Relay Status Log response */

typedef ZB_PACKED_PRE struct zb_zcl_thermostat_get_relay_status_log_resp_s
{
  zb_uint16_t time_of_day;
  zb_uint16_t relay_status;
  zb_int16_t local_temperature;
  zb_uint8_t humidity_in_percentage;
  zb_int16_t set_point;
  zb_uint16_t unread_entries;
} ZB_PACKED_STRUCT zb_zcl_thermostat_get_relay_status_log_resp_t;

#define ZB_ZCL_THERMOSTAT_GET_RELAY_STATUS_LOG_RESP_SIZE \
  sizeof(zb_zcl_thermostat_get_relay_status_log_resp_t)

/*! @brief Send Get Relay Status Log resp command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq - sequency
    @param cb - callback for getting command send status
    @param time_of_day - Time Of Day value
    @param relay_status - Relay Status value
    @param local_temperature - Local Temperature value
    @param humidity_in_percentage - Humidity In Percentage value
    @param set_point - Set Point value
    @param unread_entries - Unread Entries value
*/
#define ZB_ZCL_THERMOSTAT_SEND_GET_RELAY_STATUS_LOG_RESP(                                       \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, seq, cb, time_of_day, relay_status,         \
  local_temperature, humidity_in_percentage, set_point, unread_entries)                         \
{                                                                                               \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                                \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);                                     \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq, ZB_ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG_RESP);   \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (time_of_day));                                             \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (relay_status));                                            \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (local_temperature));                                       \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (humidity_in_percentage));                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (set_point));                                               \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (unread_entries));                                          \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                            \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                    \
    buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_THERMOSTAT, cb);        \
}

/*!
  @brief Parses Get Relay Status Log command and fills data request
  structure.
  @param data_buf - pointer to zb_buf_t buffer containing command request data
  @param resp - variable to save command request
  @param status - success or not read parameters - ZB_ZCL_PARSE_STATUS_SUCCESS or
  ZB_ZCL_PARSE_STATUS_FAILURE
    @note data_buf buffer should contain command request payload without ZCL header.
*/
#define ZB_ZCL_THERMOSTAT_GET_GET_RELAY_STATUS_LOG_RESP(data_buf, resp, status)         \
{                                                                                       \
  zb_zcl_thermostat_get_relay_status_log_resp_t *resp_ptr =                             \
     ZB_BUF_LEN(data_buf) >= ZB_ZCL_THERMOSTAT_GET_RELAY_STATUS_LOG_RESP_SIZE ?         \
     (zb_zcl_thermostat_get_relay_status_log_resp_t*)ZB_BUF_BEGIN(data_buf) : NULL;     \
  if (resp_ptr)                                                                         \
  {                                                                                     \
    ZB_MEMCPY(&(resp), resp_ptr, sizeof(zb_zcl_thermostat_get_relay_status_log_resp_t));\
    ZB_ZCL_HTOLE16_INPLACE(&(resp)->time_of_day);                                       \
    ZB_ZCL_HTOLE16_INPLACE(&(resp)->relay_status);                                      \
    /* FIXME: Can we use it with signed 16-bit int? */                                  \
    ZB_ZCL_HTOLE16_INPLACE(&(resp)->local_temperature);                                 \
    ZB_ZCL_HTOLE16_INPLACE(&(resp)->set_point);                                         \
    ZB_ZCL_HTOLE16_INPLACE(&(resp)->unread_entries);                                    \
    status = ZB_ZCL_PARSE_STATUS_SUCCESS;                                               \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    status = ZB_ZCL_PARSE_STATUS_FAILURE;                                               \
  }                                                                                     \
}

/*! @}
 *  @endcond */ /* Thermostat cluster internals */

/*! @cond internals_doc
    @name Thermostat cluster command internals
    Internal structures and defines for execute cluster command handlers.
    @{
*/

/** @brief Size of table with Weekly Schedule records */
#define ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE    20

/** @brief Flag days field "record is free" */
#define ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD            0

/** @brief Bit set "All days" */
#define ZB_ZCL_THERMOSTAT_WEEKLY_ALL                    0x7f

/** @brief Bit "Weekly vacation" */
#define ZB_ZCL_THERMOSTAT_WEEKLY_VACATION               (1<<7)

/** @brief Parameter - Thermostat will be schedule
 * without any specific record*/
#define ZB_ZCL_THERMOSTAT_PROCESS_NONE                  0xff

/** @brief Second per week */
#define ZB_SECOND_PER_WEEK                              (60*60*24*7)

/** @brief Second per day */
#define ZB_SECOND_PER_DAY                               (60*60*24)

/** @brief Second per minutes */
#define ZB_SECOND_PER_MINUTE                            60

/** @brief Days per week */
#define ZB_DAYS_PER_WEEK 7

/** @brief Time delta for using setpoint: skip setpoint if it expired
 * more then this value */
#define ZB_ZCL_THERMOSTAT_SETPOINT_TIME_DELTA   (10 * 60) /* 10 minutes */

/** @brief Subtract time in seconds round week */
#define ZB_WEEK_TIME_SUBTRACT(a, b) ((a) > (b)) ? ((a) - (b)) : ((a) + ZB_SECOND_PER_WEEK - (b))

#ifdef ZB_TIMER_32
  // 1 day (24h)
/** @brief Maximum period of Thermostat process loop */
  #define ZB_ZCL_THERMOSTAT_TIMER_MAX          (24*60*60)
#else
  // 5 min
/** @brief Maximum period of Thermostat process loop */
  #define ZB_ZCL_THERMOSTAT_TIMER_MAX          (5*60)
#endif


/** @brief Struct with one Weekly Schedule attribute */
typedef struct zb_zcl_thermostat_weekly_schedule_record_s
{
  zb_uint8_t  days;             /*< days, ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD (0) - free record */
  zb_uint32_t start_time;       /*< seconds after midnight */
  zb_uint8_t mode;              /*< Mode for Sequence */
  zb_uint16_t heat_setpoint;    /*< Heat Set Point */
  zb_uint16_t cool_setpoint;    /*< Cool Set Point */
}
zb_zcl_thermostat_weekly_schedule_record_t;

/** @brief Struct with Context Thermostat cluster */
typedef struct zb_zcl_thermostat_context_ha_s
{
  zb_zcl_thermostat_weekly_schedule_record_t schedule_table[ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE];
  zb_uint8_t setpoint_mode;
}
zb_zcl_thermostat_context_ha_t;

/*! @}
 *  @endcond */ /* Thermostat cluster command internals */

#if defined ZB_ENABLE_HA
zb_uint8_t zb_zcl_thermostat_get_current_day_of_week(zb_uint8_t start_of_week);
#endif

/*! @} */ /* ZB_ZCL_THERMOSTAT_HA_ADDS */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ! defined ZB_ZCL_THERMOSTAT_HA_ADDS_H */
