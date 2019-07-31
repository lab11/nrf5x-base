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
 * PURPOSE: DOOR_LOCK cluster defintions
*/

#ifndef ZB_ZCL_DOOR_LOCK_H
#define ZB_ZCL_DOOR_LOCK_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_DOOR_LOCK
 *  @{
 *    @details
 *    Door Lock cluster defined in ZCL spec, clause 7.3.
 *
 *    The cluster contains 2 command sets:
 *    @li Lock Door request and response;
 *    @li Unlock Door request and response.
 *
 *    Both requests have no payload and could be scheduled for sending with corresponding macros.
 *    Both responses have simple payload, and their sending and parsing with corresponding macros
 *    is almost trivial.
 *
 *    @par Examples:
 *
 *    Send Door Lock command:
 *    @snippet door_lock_th_538.c Send Door Lock request
 *
 *    Send Door Lock response:
 *    @snippet ha_door_lock_sample.c Send Door Lock response
 *
 *    Parse Door Lock response:
 *    @snippet ha_door_lock_controller_sample.c Parse Door Lock response
 *
 *    Send Door Unlock command:
 *    @snippet door_lock_th_538.c Send Door Unlock request
 *
 *    Send Door Unlock response:
 *    @snippet ha_door_lock_sample.c Send Door Unlock response
 *
 *    Parse Door Unlock response:
 *    @snippet ha_door_lock_controller_sample.c Parse Unlock Door response
 *    @par
 *
 *    For more information see devices_closures, 538_door_lock samples
 */

/*! @name DoorLock cluster attributes
    @{
*/

/*! @brief Door Lock cluster attribute identifiers
    @see ZCL spec, subclause 7.3.2.2
*/
enum zb_zcl_door_lock_attr_e
{
  /*! @brief LockState attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID                = 0x0000,
  /*! @brief LockType attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_ID                 = 0x0001,
  /*! @brief ActuatorEnabled attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ID          = 0x0002,
  /*! @brief DoorState attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_ID                = 0x0003,
  /*! @brief DoorOpenEvents attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_OF_DOOR_OPEN_EVENTS_ID   = 0x0004,
  /*! @brief DoorClosedEvents attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_OF_DOOR_CLOSED_EVENTS_ID = 0x0005,
  /*! @brief OpenPeriod attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_OPEN_PERIOD_ID                    = 0x0006,
  /*! @brief NumberOfTotalUsersSupported attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_TOTAL_USERS_ID                = 0x0011,
  /*! @brief NumberOfPINUsersSupported attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_PIN_USERS_ID                  = 0x0012,
  /*! @brief NumberOfWeekDaySchedulesSupportedPerUser attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_WEEK_DAY_SCHEDULE_PER_USER_ID = 0x0014,
  /*! @brief NumberOfYearDaySchedulesSupportedPerUser attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_YEAR_DAY_SCHEDULE_PER_USER_ID = 0x0015,
  /*! @brief NumberOfHolidaySchedulesSupported attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_NUM_HOLIDAY_SCHEDULE_ID           = 0x0016,
  /*! @brief Max PIN code length attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_MAX_PIN_LEN_ID                    = 0x0017,
  /*! @brief Min PIN code length attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_MIN_PIN_LEN_ID                    = 0x0018,
  /*! @brief OperatingMode attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_OPERATING_MODE_ID                 = 0x0025,
  /*! @brief SupportedOperatingModes attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_OPERATION_MODES_SUPPORTED_ID      = 0x0026,
  /*! @brief EnableLocalProgramming attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_ENABLE_LOCAL_PROGRAMMING_ID       = 0x0028,
  /*! @brief Require PIN for RF operation attribute */
  ZB_ZCL_ATTR_DOOR_LOCK_REQUIRE_PIN_RF_ID                 = 0x0033,

};


/** @brief Values for LockState attribute
  * @see ZCL spec, subclause 7.3.2.2.1
  */
enum zb_zcl_door_lock_lock_state_e
{
  /*! "Not fully locked" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_NOT_FULLY_LOCKED  = 0x00,
  /*! "Locked" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_LOCKED            = 0x01,
  /*! "Unlocked" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNLOCKED          = 0x02,
  /*! "Not Defined" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNDEFINED         = 0xff
};

/** @brief Default value for Lock State attribute. */
#define ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_DEFAULT_VALUE ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNDEFINED

/** @brief Values for LockType attribute
  * @see ZCL spec, subclause 7.3.2.2.2
  */
enum zb_zcl_door_lock_lock_type_e
{
  /*! "Deadbolt" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_DEADBOLT     = 0x00,
  /*! "Magnetic" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_MAGNETIC     = 0x01,
  /*! "Other" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_OTHER        = 0x02,
  /*! "Not Defined" value */
  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_UNDEFINED    = 0xff
};

/** @brief Default value for Lock Type attribute. */
#define ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_DEFAULT_VALUE   ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_UNDEFINED

/** @brief Values for ActuatorEnabled attribute
  * @see ZCL spec, subclause 7.3.2.2.3
  */
enum zb_zcl_door_lock_actuator_enabled_e
{
  /*! "Disabled" value */
  ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_DISABLED  = 0x0b0,
  /*! "Enabled" value */
  ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ENABLED   = 0x0b1
};

/** @brief Default value for Actuator Enabled attribute. */
#define ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_DEFAULT_VALUE \
  ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ENABLED

/** @brief Values for DoorState attribute
  * @see ZCL specx, subclause 7.3.2.2.4
  */
enum zb_zcl_door_lock_door_state_e
{
  /*! "Open" value */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_OPEN              = 0x00,
  /*! "Closed" value */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_CLOSED            = 0x01,
  /*! "Error (jammed)" value */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_ERROR_JAMMED      = 0x02,
  /*! "Error (forced open)" value */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_ERROR_FORCED_OPEN = 0x03,
  /*! "Error (unspecified)" value */
  ZB_ZCL_ATTR_DOOR_LOCK_DOOR_STATE_ERROR_UNSPECIFIED = 0x04
};

/*! @} */ /* DoorLock cluster attributes */

/*! @name DoorLock cluster commands
    @{
*/

/*! @brief DoorLock cluster commands
    @see ZCL spec, subclause 7.3.2.3 Commands Received
*/
enum zb_zcl_door_lock_cmd_e
{
  ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR    = 0x00,
  ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR  = 0x01,
};

/*! @brief DoorLock cluster commands response
    @see ZCL spec, subclause 7.3.2.4 Commands Generated
*/
enum zb_zcl_door_lock_cmd_resp_e
{
  ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR_RES    = 0x00,
  ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR_RES                 = 0x01,
  ZB_ZCL_CMD_DOOR_LOCK_OPERATION_EVENT_NOTIFICATION_ID = 0x20,
};


enum zb_zcl_door_lock_operation_event_code_e
{
  ZB_ZCL_DOOR_LOCK_OPERATION_EVENT_CODE_UNKNOWN       = 0x00,
  ZB_ZCL_DOOR_LOCK_OPERATION_EVENT_CODE_LOCK          = 0x01,
  ZB_ZCL_DOOR_LOCK_OPERATION_EVENT_CODE_UNLOCK        = 0x02,
  ZB_ZCL_DOOR_LOCK_OPERATION_EVENT_CODE_MANUAL_LOCK   = 0x0D,
  ZB_ZCL_DOOR_LOCK_OPERATION_EVENT_CODE_MANUAL_UNLOCK = 0x0E,
};

/*! @brief General macro for sending a "Door Lock" cluster command
    @param buffer to put command payload and headers to
    @param dst_addr address to send to
    @param dst_addr_mode destination address mode
    @param dst_ep destination endpoint
    @param ep source endpoint
    @param dis_default_resp "Disable default response" flag
    @param cb command send status callback
*/
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

/*! @brief General macro for sending a "Door Unlock" cluster command
    @param buffer to put command payload and headers to
    @param dst_addr address to send to
    @param dst_addr_mode destination address mode
    @param dst_ep destination endpoint
    @param ep source endpoint
    @param dis_default_resp "Disable default response" flag
    @param cb command send status callback
*/
#define ZB_ZCL_DOOR_LOCK_SEND_UNLOCK_DOOR_REQ(buffer,             \
                                              dst_addr,           \
                                              dst_addr_mode,      \
                                              dst_ep,             \
                                              ep,                 \
                                              dis_default_resp,   \
                                              cb)                 \
  ZB_ZCL_SEND_CMD(buffer,                                         \
                  dst_addr,                                       \
                  dst_addr_mode,                                  \
                  dst_ep,                                         \
                  ep,                                             \
                  ZB_AF_HA_PROFILE_ID,                            \
                  dis_default_resp,                               \
                  ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                    \
                  ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR, cb)

/*! @brief Door Lock command send response macro
    @param buffer to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq_num - sequence number
    @param status - result of command execute
*/
#define ZB_ZCL_DOOR_LOCK_SEND_LOCK_DOOR_RES(buffer,                                  \
                                            addr,                                    \
                                            dst_addr_mode,                           \
                                            dst_ep,                                  \
                                            ep,                                      \
                                            prfl_id,                                 \
                                            seq_num,                                 \
                                            status)                                  \
{                                                                                    \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                     \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);                          \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq_num, ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR_RES); \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, status);                                              \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                 \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer,                                                  \
                            addr,                                                    \
                            dst_addr_mode,                                           \
                            dst_ep,                                                  \
                            ep,                                                      \
                            prfl_id,                                                 \
                            ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                             \
                            NULL);                                                   \
}

/*! @brief Door Unlock command send response macro
    @param buffer to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param seq_num - sequence number
    @param status - result of command execute
*/
#define ZB_ZCL_DOOR_LOCK_SEND_UNLOCK_DOOR_RES(buffer,                                  \
                                              addr,                                    \
                                              dst_addr_mode,                           \
                                              dst_ep,                                  \
                                              ep,                                      \
                                              prfl_id,                                 \
                                              seq_num,                                 \
                                              status)                                  \
{                                                                                      \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);                                       \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr);                            \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(ptr, seq_num, ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR_RES); \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, status);                                                \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                   \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer,                                                    \
                            addr,                                                      \
                            dst_addr_mode,                                             \
                            dst_ep,                                                    \
                            ep,                                                        \
                            prfl_id,                                                   \
                            ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                               \
                            NULL);                                                     \
}

/*! Door lock commands response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_door_lock_read_lock_door_res_payload_s
{
  zb_uint8_t status;      /*!< Command execution status */
} ZB_PACKED_STRUCT zb_zcl_door_lock_read_lock_door_res_payload_t;

/*! @brief Door Lock response structured reading
    @param buffer - pointer to the message buffer (of type @ref zb_buf_t) containing payload
    @return pointer to @ref zb_zcl_door_lock_read_lock_door_res_payload_s structure
    @attention returned pointer will point to the same data in the buffer thus being valid until
    buffer data will be overwritten.
*/
#define ZB_ZCL_DOOR_LOCK_READ_LOCK_DOOR_RES(buffer)                                   \
  (   (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_door_lock_read_lock_door_res_payload_t)) \
   ?  NULL                                                                            \
   : (zb_zcl_door_lock_read_lock_door_res_payload_t*)ZB_BUF_BEGIN((buffer)))

/*! Door unlock commands response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_door_lock_read_unlock_door_res_payload_s
{
  zb_uint8_t status;      /*!< Command execution status */
} ZB_PACKED_STRUCT zb_zcl_door_lock_read_unlock_door_res_payload_t;

/*! @brief Door unLock response structured reading
    @param buffer - pointer to the message buffer (of type @ref zb_buf_t)
    containing payload
    @return pointer to @ref zb_zcl_door_lock_read_unlock_door_res_payload_s structure
    @attention returned pointer will point to the same data in the buffer thus
    being valid until buffer data will be overwritten.
*/
#define ZB_ZCL_DOOR_LOCK_READ_UNLOCK_DOOR_RES(buffer)                                   \
  (   (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_door_lock_read_unlock_door_res_payload_t)) \
   ?  NULL                                                                              \
   : (zb_zcl_door_lock_read_unlock_door_res_payload_t*)ZB_BUF_BEGIN((buffer)))

/*! @} */ /* DoorLock cluster commands */

/*! @cond internals_doc
    @internal @name DoorLock cluster internals
    Internal structures for DoorLock cluster
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID(data_ptr)                 \
  {                                                                                          \
    ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID,                                                     \
    ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                              \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING | ZB_ZCL_ATTR_ACCESS_SCENE,  \
    (zb_voidp_t) data_ptr                                                                    \
  }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_ID(data_ptr) \
  {                                                               \
    ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_ID,                           \
    ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                   \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
    (zb_voidp_t) data_ptr                                         \
  }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ID(data_ptr) \
  {                                                               \
    ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ID,                    \
    ZB_ZCL_ATTR_TYPE_BOOL,                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                 \
    (zb_voidp_t) data_ptr                                         \
  }


/** @internal @brief Declare attribute list for DoorLock cluster
    @param attr_list - attribure list name
    @param lock_state
    @param lock_type
    @param actuator_enabled
*/
#define ZB_ZCL_DECLARE_DOOR_LOCK_CLUSTER_ATTRIB_LIST(attr_list,          \
                                                     lock_state,         \
                                                     lock_type,          \
                                                     actuator_enabled)   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID, (lock_state))             \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_ID, (lock_type))               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ID, (actuator_enabled)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST



/*! @internal @brief Number of attributes mandatory for reporting in Door Lock cluster */
#define ZB_ZCL_DOOR_LOCK_REPORT_ATTR_COUNT  1

/*! @}
    @endcond */ /* DoorLock cluster internals */

/*! @} */ /* ZCL DoorLock cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_door_lock_init_server();
zb_void_t zb_zcl_door_lock_init_client();
#define ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SERVER_ROLE_INIT zb_zcl_door_lock_init_server
#define ZB_ZCL_CLUSTER_ID_DOOR_LOCK_CLIENT_ROLE_INIT zb_zcl_door_lock_init_client

#endif /* ZB_ZCL_DOOR_LOCK_H */
