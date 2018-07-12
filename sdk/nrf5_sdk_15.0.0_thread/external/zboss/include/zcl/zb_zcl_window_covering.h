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
 * PURPOSE: Window Covering cluster defintions
*/

#ifndef ZB_ZCL_WINDOW_COVERING_H
#define ZB_ZCL_WINDOW_COVERING_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_WINDOW_COVERING
 *  @{
 *    @details
 *    All commands in the cluster have request form only, and could be responded with Default
 *    Response command, if not disabled explicitly.
 *
 *    Most of the cluster commands have no payload, so could be sent with a simple macro calls, and
 *    identified on receipt by their identifiers. The exceptions are: Go To Lift Percentage and Go
 *    To Tilt Percentage commands. These two should be parsed on receipt with @ref
 *    ZB_ZCL_WINDOW_COVERING_GET_GO_TO_LIFT_PERCENTAGE_REQ(), or
 *    ZB_ZCL_WINDOW_COVERING_GET_GO_TO_TILT_PERCENTAGE_REQ() macro respectively. As usually, parse
 *    macros take a packet buffer as input, and provide an in-buffer pointer to the structure of
 *    type zb_zcl_go_to_lift_percentage_req_t (respectively, @ref
 *    zb_zcl_go_to_tilt_percentage_req_s).
 *
 *    Sample use ZB_ZCL_WINDOW_COVERING_GET_GO_TO_LIFT_PERCENTAGE_REQ
 *    @code
      zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param)
      {
        zb_buf_t *zcl_cmd_buf = (zb_buf_t *)ZB_BUF_FROM_REF(param);
        zb_zcl_parsed_hdr_t *cmd_info = ZB_GET_BUF_PARAM(zcl_cmd_buf, zb_zcl_parsed_hdr_t);
        zb_zcl_go_to_lift_percentage_req_t *lift_p;
        zb_bool_t cmd_processed = ZB_FALSE;

        switch (cmd_info->cluster_id)
        {
          case ZB_ZCL_CLUSTER_ID_WINDOW_COVERING:
          {
            if ( !cmd_info->is_common_command )
            {
              switch (cmd_info->cmd_id)
              {
              case ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE:
                  ZB_ZCL_WINDOW_COVERING_GET_GO_TO_LIFT_PERCENTAGE_REQ(zcl_cmd_buf, lift_p);
                  if (lift_p)
                  {
                    cmd_processed = zb_zcl_check_attr_value(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
                                                endpoint,
                                                ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID,
                                                &(*lift_p).percentage_lift_value);
                  }
                  break;
                  ...
              }
              ...
            }
            break;
          }
        }
        return cmd_processed;
      }
      @endcode

      For more information see window_covering sample.
  */

/** @internal Set bits from conf_var variable to type_value */
#define ZB_ZCL_SET_BIT(conf_var, type_value)                        \
{                                                                   \
  (conf_var) = (conf_var) | (type_value);                           \
}

/** @internal Get type_value bits from conf_var variable */
#define ZB_ZCL_GET_BIT(conf_var, type_value) ((conf_var) & (type_value))

/** @internal Clear type_value bits in conf_var variable */
#define ZB_ZCL_CLR_BIT(conf_var, type_value)                            \
{                                                                       \
  (conf_var) = (conf_var) & ~(type_value);                              \
}

/** @name Window Covering cluster attributes
    @{
*/

/** @brief Window Covering cluster information attribute set identifiers
    @see ZCL spec, subclause 9.4.2.1.1 - 9.4.2.1.4
*/
enum zb_zcl_window_covering_info_attr_e
{
  /** @brief Window Covering Type attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID                = 0x0000,
  /** @brief PhysicalClosedLimit Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_PHYSICAL_CLOSED_LIMIT_LIFT_ID          = 0x0001,
  /** @brief PhysicalClosedLimit Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_PHY_CLOSED_LIMIT_TILT_ID               = 0x0002,
  /** @brief CurrentPosition Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID               = 0x0003,
  /** @brief CurrentPosition Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID               = 0x0004,
  /** @brief Number of Actuations Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_NUMBER_OF_ACTUATIONS_LIFT_ID           = 0x0005,
  /** @brief Number of Actuations Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_NUMBER_OF_ACTUATIONS_TILT_ID           = 0x0006,
  /** @brief Config/Status attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID                       = 0x0007,
  /** @brief Current Position Lift Percentage attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID    = 0x0008,
  /** @brief Current Position Tilt Percentage attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_ID    = 0x0009
};

/** @brief Permissible values for Type attribute
  * @see ZCL spec, subclause 9.4.2.1.2
  */
enum zb_zcl_window_covering_window_covering_type_e
{
  /** Rollershade value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE                       = 0x00,
  /** Rollershade - 2 Motor value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_2_MOTOR               = 0x01,
  /** Rollershade - Exterior value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_EXTERIOR              = 0x02,
  /** Rollershade - Exterior - 2 Motor value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_EXTERIOR_2_MOTOR      = 0x03,
  /** Drapery value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_DRAPERY                           = 0x04,
  /** Awning value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_AWNING                            = 0x05,
  /** Shutter value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_SHUTTER                           = 0x06,
  /** Tilt Blind - Tilt Only value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_TILT_BLIND_TILT_ONLY              = 0x07,
  /** Tilt Blind - Lift and Tilt value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_TILT_BLIND_LIFT_AND_TILT          = 0x08,
  /** Projector screen value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_PROJECTOR_SCREEN                  = 0x09
};

/** @brief Permissible values for Config/Status attribute
    @see ZCL spec, subclause 9.4.2.1.2.7*/
enum zb_zcl_window_covering_config_status_e
{
  /** Operational value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_OPERATIONAL                   = 0x01,
  /** Online value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_ONLINE                        = 0x02,
  /** Open/Up Commands have been reversed value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_REVERSE_COMMANDS              = 0x04,
  /** Lift control is Closed Loop value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_LIFT_CONTROL_IS_CLOSED_LOOP   = 0x08,
  /** Tilt control is Closed Loop value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_TILT_CONTROL_IS_CLOSED_LOOP   = 0x10,
  /** Lift Encoder Controlled value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_LIFT_ENCODER_CONTROLLED       = 0x20,
  /** Tilt Encoder Controlled value */
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_TILT_ENCODER_CONTROLLED       = 0x40
};

/** @internal Set bits from conf_var variable to type_value */
#define ZB_ZCL_SET_BIT(conf_var, type_value)                        \
{                                                                   \
  (conf_var) = (conf_var) | (type_value);                           \
}

/** @internal Get type_value bits from conf_var variable */
#define ZB_ZCL_GET_BIT(conf_var, type_value) ((conf_var) & (type_value))

/** @internal Clear type_value bits in conf_var variable */
#define ZB_ZCL_CLR_BIT(conf_var, type_value)                            \
{                                                                       \
  (conf_var) = (conf_var) & ~(type_value);                              \
}

/** @brief Sets bits of Config/Status parameter
    @param type_value - bit to set
    @param conf_var - config variable
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_SET(conf_var, type_value) \
  ZB_ZCL_SET_BIT(conf_var, type_value)

/** @brief Gets bit value of Config/Status parameter
    @param type_value - bit value to check
    @param conf_var - config variable
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_GET_CONFIG_STATUS_BIT_VAL(conf_var, type_value) \
  ZB_ZCL_GET_BIT(conf_var, type_value)


/** @brief Clear bit of Config/Status parameter
    @param type_value - bit to clear
    @param conf_var - config variable
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_CLEAR(conf_var, type_value) \
  ZB_ZCL_CLR_BIT(conf_var, type_value)


/** @brief Window covering cluster settings attribute set identifiers
    @see ZCL spec, subclause 9.4.2.1.5
*/
enum zb_zcl_window_covering_settings_attr_e
{
  /** @brief InstalledOpenLimit -  Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID            = 0x0010,
  /** @brief InstalledClosedLimit - Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID          = 0x0011,
  /** @brief InstalledOpenLimit - Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_ID            = 0x0012,
  /** @brief InstalledClosedLimit - Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_ID          = 0x0013,
  /** @brief Velocity - Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_VELOCITY_ID                             = 0x0014,
  /** @brief Acceleration Time - Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_ACCELERATION_TIME_ID                    = 0x0015,
  /** @brief Deceleration Time - Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_DECELERATION_TIME_ID                    = 0x0016,
  /** @brief Mode attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID                                 = 0x0017,
  /** @brief Intermediate Setpoints - Lift attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INTERMEDIATE_SETPOINTS_LIFT_ID          = 0x0018,
  /** @brief Intermediate Setpoints - Tilt attribute */
  ZB_ZCL_ATTR_WINDOW_COVERING_INTERMEDIATE_SETPOINTS_TILT_ID          = 0x0019
};

/** @brief Permissible values for Mode attribute
    @see ZCL spec, subclause 9.4.2.1.5.7.1*/
enum zb_zcl_window_covering_mode_e
{
  /** Reversed motor direction value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_REVERSED_MOTOR_DIRECTION               = 0x01,
  /** Run in calibration mode value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_RUN_IN_CALIBRATION_MODE                = 0x02,
  /** Motor is running in maintenance mode value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_MOTOR_IS_RUNNING_IN_MAINTENANCE_MODE   = 0x04,
  /** LEDs will display feedback value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_LEDS_WILL_DISPLAY_FEEDBACK             = 0x08
};

/** @brief Sets bytes of Mode parameter
    @param type_value - type which need to set
    @param mode_var - mode variable
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_MODE_SET(mode_var, type_value) \
  ZB_ZCL_SET_BIT(mode_var, type_value)

/** @brief Gets bits of Mode parameter
    @param type_value - type which need to set
    @param mode_var - mode variable
 *  @hideinitializer
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_MODE_GET(mode_var, type_value)                                \
  ZB_ZCL_GET_BIT(mode_var, type_value)

/** @brief Clear bytes of Mode parameter
    @param type_value - type which need to set
    @param mode_var - mode variable
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_MODE_CLEAR(mode_var, type_value)                              \
  ZB_ZCL_CLR_BIT(mode_var, type_value)

/** @brief Window covering type attribute default value */
#define ZB_ZCL_WINDOW_COVERING_WINDOW_COVERING_TYPE_DEFAULT_VALUE 0x00

/** @brief Config/status attribute default value */
#define ZB_ZCL_WINDOW_COVERING_CONFIG_STATUS_DEFAULT_VALUE                                        \
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_OPERATIONAL                                                  \
  | ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_ONLINE

/** @brief Current position lift percentage attribute default value */
#define ZB_ZCL_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_DEFAULT_VALUE 0x00

/** @brief Current position tilt percentage attribute default value */
#define ZB_ZCL_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_DEFAULT_VALUE 0x00

/** @brief Installed open limit lift attribute default value */
#define ZB_ZCL_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_DEFAULT_VALUE 0x0000

/** @brief Installed closed limit lift attribute default value */
#define ZB_ZCL_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_DEFAULT_VALUE 0xffff

/** @brief Installed open limit tilt attribute default value */
#define ZB_ZCL_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_DEFAULT_VALUE 0x0000

/** @brief Installed closed limit tilt attribute default value */
#define ZB_ZCL_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_DEFAULT_VALUE 0xffff

/** @brief Mode attribute default value */
#define ZB_ZCL_WINDOW_COVERING_MODE_DEFAULT_VALUE           \
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_REVERSED_MOTOR_DIRECTION

/** @} */ /* Window Covering cluster attributes */

/**
 *  @name Attribute value manipulation API
 *  @{
 */

/**
 *  @brief Sets bits of Config/Status parameter.
 *  @param type_value - bit to set.
 *  @param conf_var - config variable.
 *  @hideinitializer
 */
#define ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_SET(conf_var, type_value) \
  ZB_ZCL_SET_BIT(conf_var, type_value)

/** @brief Gets bit value of Config/Status parameter
    @param type_value - bit value to check
    @param conf_var - config variable
 *  @hideinitializer
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_GET_CONFIG_STATUS_BIT_VAL(conf_var, type_value) \
  ZB_ZCL_GET_BIT(conf_var, type_value)

/** @brief Clear bit of Config/Status parameter
    @param type_value - bit to clear
    @param conf_var - config variable
 *  @hideinitializer
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_CLEAR(conf_var, type_value) \
  ZB_ZCL_CLR_BIT(conf_var, type_value)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID(data_ptr)      \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID,                                            \
  ZB_ZCL_ATTR_TYPE_8BIT_ENUM,                                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

/** @brief Sets bits of Mode parameter
    @param type_value - type which need to set
    @param mode_var - mode variable
 *  @hideinitializer
*/
#define ZB_ZCL_ATTR_WINDOW_COVERING_MODE_SET(mode_var, type_value) \
  ZB_ZCL_SET_BIT(mode_var, type_value)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID(data_ptr)             \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID,                                                   \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID(data_ptr)     \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID(data_ptr)     \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID(data_ptr) \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID(        \
  data_ptr)                                                                                       \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                                     \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_ID(data_ptr) \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_ID(        \
  data_ptr)                                                                                       \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                                     \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID(data_ptr)                      \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID,                                                            \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                  \
  (zb_voidp_t) data_ptr                                                                           \
}

/** Declare attribute list for Window Covering cluster
    @param attr_list - attribure list name
    @param window_covering_type - pointer to variable to store Window Covering Type attribute value
    @param config_status - pointer to variable to store Config/Status attribute value
    @param current_position_lift_percentage  - pointer to variable to store Current Position Lift
      attribute value
    @param current_position_tilt_percentage - pointer to variable to store Current Position Tilt
      attribute value
    @param installed_open_limit_lift - pointer to variable to store Installed Open Limit Lift
    attribute value
    @param installed_closed_limit_lift - pointer to variable to store Installed Closed Limit Lift
    attribute value
    @param installed_open_limit_tilt - pointer to variable to store Installed Open Limit Tilt
    attribute value
    @param installed_closed_limit_tilt - pointer to variable to store Installed Closed Limit Tilt
    attribute value
    @param mode - pointer to variable to store Mode attribute value
*/
#define ZB_ZCL_DECLARE_WINDOW_COVERING_CLUSTER_ATTRIB_LIST(attr_list, window_covering_type,       \
   config_status, current_position_lift_percentage, current_position_tilt_percentage,             \
   installed_open_limit_lift, installed_closed_limit_lift, installed_open_limit_tilt,             \
   installed_closed_limit_tilt, mode)                                                             \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID,                       \
    (window_covering_type))                                                                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID,                              \
    (config_status))                                                                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID,                      \
    (current_position_lift_percentage))                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID,                      \
    (current_position_tilt_percentage))                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                  \
    (installed_open_limit_lift))                                                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                \
    (installed_closed_limit_lift))                                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_ID,                  \
    (installed_open_limit_tilt))                                                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_ID,                \
    (installed_closed_limit_tilt))                                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID, (mode))                               \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** Number of attributes mandatory for reporting in window covering cluster */
#define ZB_ZCL_WINDOW_COVERING_REPORT_ATTR_COUNT 0

/** @} */ /* Attribute value manipulation API */

/** @name Window Covering cluster commands
    @{
*/

/** @brief Window Covering cluster command identifiers
    @see ZCL spec, subclause 9.4.2.2
*/
enum zb_zcl_window_covering_cmd_e
{
  /** Up/Open command */
  ZB_ZCL_CMD_WINDOW_COVERING_UP_OPEN               = 0x00,
  /** Down/Close command */
  ZB_ZCL_CMD_WINDOW_COVERING_DOWN_CLOSE            = 0x01,
  /** Stop command */
  ZB_ZCL_CMD_WINDOW_COVERING_STOP                  = 0x02,
  /** Go to Lift Setpoint command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_SETPOINT   = 0x03,
  /** Go to Lift Value command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_VALUE      = 0x04,
  /** Go to Lift Percentage command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE = 0x05,
  /** Go to Tils Setpoint command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_TILT_SETPOINT   = 0x06,
  /** Go to Tilt Value command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_TILT_VALUE      = 0x07,
  /** Go to Tilt Percentage command */
  ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE = 0x08,
  /** Program Setpoint command */
  ZB_ZCL_CMD_WINDOW_COVERING_PROGRAM_SETPOINT      = 0x09
};

/** Mandatory commands defines */

/** @brief Structured representsation of GO_TO_LIFT_PERCENTAGE command payload
    @see ZCL spec, subclause 9.4.2.2.5.2.1*/
typedef ZB_PACKED_PRE struct zb_zcl_go_to_lift_percentage_req_s
{
  /** Percentage Lift Value */
  zb_uint8_t       percentage_lift_value;
} ZB_PACKED_STRUCT zb_zcl_go_to_lift_percentage_req_t;

/** @brief Structured representsation of GO_TO_TILT_PERCENTAGE command payload
    @see ZCL spec, subclause 9.4.2.2.5.5.1*/
typedef ZB_PACKED_PRE struct zb_zcl_go_to_tilt_percentage_req_s
{
  /** Percentage Tilt Value */
  zb_uint8_t       percentage_tilt_value;
} ZB_PACKED_STRUCT zb_zcl_go_to_tilt_percentage_req_t;

/** @brief Send Up/Open command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_WINDOW_COVERING_SEND_UP_OPEN_REQ(                                                  \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb)                                 \
{                                                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                                      \
    ZB_ZCL_CMD_WINDOW_COVERING_UP_OPEN);                                                          \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep,                              \
    prfl_id, ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, cb);                                              \
}

/** @brief Send Down/Close command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_WINDOW_COVERING_SEND_DOWN_CLOSE_REQ(                                               \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb)                                 \
{                                                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                                      \
    ZB_ZCL_CMD_WINDOW_COVERING_DOWN_CLOSE);                                                       \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep,                              \
    prfl_id, ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, cb);                                              \
}

/** @brief Send Stop command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
*/
#define ZB_ZCL_WINDOW_COVERING_SEND_STOP_REQ(                                                     \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb)                                 \
{                                                                                                 \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                  \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                                      \
    ZB_ZCL_CMD_WINDOW_COVERING_STOP);                                                             \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep,                              \
    prfl_id, ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, cb);                                              \
}

/** @brief Send Go to Lift Percentage command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param percentage_lift_value - Percentage Lift value
*/
#define ZB_ZCL_WINDOW_COVERING_SEND_GO_TO_LIFT_PERCENTAGE_REQ(                           \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, percentage_lift_value) \
{                                                                                        \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                         \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                    \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                             \
    ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE);                                   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, percentage_lift_value);                                   \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                     \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep,                     \
    prfl_id, ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, cb);                                     \
}

/** @brief Parses Get Go to Lift Percentage command and fills data request
    structure. If request contains invalid data, status parameter is ZB_FALSE
    @param data_buf - pointer to zb_buf_t buffer containing command request data
    @param lift_percentage_req - pointer to a variable to save command request to
    @param status - return ZB_TRUE if request contains valid data, else ZB_FALSE
    @note data_buf buffer should contain command request payload without ZCL header.
 */
#define ZB_ZCL_WINDOW_COVERING_GET_GO_TO_LIFT_PERCENTAGE_REQ(           \
  data_buf, lift_percentage_req, status)                                \
{                                                                       \
  zb_zcl_go_to_lift_percentage_req_t *lift_percentage_req_ptr;          \
  (lift_percentage_req_ptr) = ZB_BUF_LEN(data_buf) ==                   \
    sizeof(zb_zcl_go_to_lift_percentage_req_t) ?                        \
    (zb_zcl_go_to_lift_percentage_req_t*)ZB_BUF_BEGIN(data_buf) : NULL; \
  if (lift_percentage_req_ptr)                                          \
  {                                                                     \
    (lift_percentage_req).percentage_lift_value =                       \
      lift_percentage_req_ptr->percentage_lift_value;                   \
    status = ZB_TRUE;                                                   \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    status = ZB_FALSE;                                                  \
  }                                                                     \
}


/** @brief Send Go to Tilt Percentage command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param percentage_tilt_value - Percentage Tilt value
*/
#define ZB_ZCL_WINDOW_COVERING_SEND_GO_TO_TILT_PERCENTAGE_REQ(                           \
  buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, def_resp, cb, percentage_tilt_value) \
{                                                                                        \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                         \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)                    \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(),                             \
    ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE);                                   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, percentage_tilt_value);                                   \
  ZB_ZCL_FINISH_PACKET(buffer, ptr)                                                     \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer, addr, dst_addr_mode, dst_ep, ep,                     \
    prfl_id, ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, cb);                                     \
}

/** @brief Parses Get Go to Tilt Percentage command and fills to data request
    structure. If request contains invalid data, -1 is returned as Percentage Tilt Value
    @param data_buf - pointer to zb_buf_t buffer containing command request data
    @param tilt_percentage_req - variable to save command request
    @param status - return ZB_TRUE if request contains valid data, else ZB_FALSE
    @note data_buf buffer should contain command request payload without ZCL header.
*/
#define ZB_ZCL_WINDOW_COVERING_GET_GO_TO_TILT_PERCENTAGE_REQ(           \
  data_buf, tilt_percentage_req, status)                                \
{                                                                       \
  zb_zcl_go_to_tilt_percentage_req_t *tilt_percentage_req_ptr;          \
  (tilt_percentage_req_ptr) = ZB_BUF_LEN(data_buf) ==                   \
    sizeof(zb_zcl_go_to_tilt_percentage_req_t) ?                        \
    (zb_zcl_go_to_tilt_percentage_req_t*)ZB_BUF_BEGIN(data_buf) : NULL; \
  if (tilt_percentage_req_ptr)                                          \
  {                                                                     \
    (tilt_percentage_req).percentage_tilt_value =                       \
      tilt_percentage_req_ptr->percentage_tilt_value;                   \
    status = ZB_TRUE;                                                   \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    status = ZB_FALSE;                                                  \
  }                                                                     \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID(data_ptr)             \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID,                                                   \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID(data_ptr)     \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID(data_ptr)     \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID,                                           \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID(data_ptr) \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID(        \
  data_ptr)                                                                                       \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                                     \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_ID(data_ptr) \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_ID(        \
  data_ptr)                                                                                       \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                                     \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID(data_ptr)                      \
{                                                                                                 \
  ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID,                                                            \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE,                                                                  \
  (zb_voidp_t) data_ptr                                                                           \
}

/** @internal @brief Declare attribute list for Window Covering cluster
    @param attr_list - attribure list name
    @param window_covering_type - pointer to variable to store Window Covering Type attribute value
    @param config_status - pointer to variable to store Config/Status attribute value
    @param current_position_lift_percentage - pointer to variable to store Current Position Lift attribute
    value
    @param current_position_tilt_percentage - pointer to variable to store Current Position Tilt attribute
    value
    @param installed_open_limit_lift - pointer to variable to store Installed Open Limit Lift
    attribute value
    @param installed_closed_limit_lift - pointer to variable to store Installed Closed Limit Lift
    attribute value
    @param installed_open_limit_tilt - pointer to variable to store Installed Open Limit Tilt
    attribute value
    @param installed_closed_limit_tilt - pointer to variable to store Installed Closed Limit Tilt
    attribute value
    @param mode - pointer to variable to store Mode attribute value
*/
#define ZB_ZCL_DECLARE_WINDOW_COVERING_CLUSTER_ATTRIB_LIST(attr_list, window_covering_type,       \
   config_status, current_position_lift_percentage, current_position_tilt_percentage,             \
   installed_open_limit_lift, installed_closed_limit_lift, installed_open_limit_tilt,             \
   installed_closed_limit_tilt, mode)                                                             \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID,                       \
    (window_covering_type))                                                                       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID,                              \
    (config_status))                                                                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_ID,                      \
    (current_position_lift_percentage))                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_ID,                      \
    (current_position_tilt_percentage))                                                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_LIFT_ID,                  \
    (installed_open_limit_lift))                                                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_LIFT_ID,                \
    (installed_closed_limit_lift))                                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_OPEN_LIMIT_TILT_ID,                  \
    (installed_open_limit_tilt))                                                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_INSTALLED_CLOSED_LIMIT_TILT_ID,                \
    (installed_closed_limit_tilt))                                                                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID, (mode))                               \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @internal Number of attributes mandatory for reporting in window covering cluster */
#define ZB_ZCL_WINDOW_COVERING_REPORT_ATTR_COUNT 0

/** @} */ /* Window Covering cluster internals */

/** @} */ /* ZCL Window Covering cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_window_covering_init_server();
zb_void_t zb_zcl_window_covering_init_client();
#define ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_SERVER_ROLE_INIT zb_zcl_window_covering_init_server
#define ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_CLIENT_ROLE_INIT zb_zcl_window_covering_init_client

#endif /* ZB_ZCL_WINDOW_COVERING_H */
