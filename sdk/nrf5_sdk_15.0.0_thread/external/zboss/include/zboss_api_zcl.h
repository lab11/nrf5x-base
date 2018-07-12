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
 * PURPOSE: ZBOSS Zigbee cluster library API header
*/
#ifndef ZBOSS_API_ZCL_H
#define ZBOSS_API_ZCL_H 1

#include "zb_types.h"

#if defined ZB_ENABLE_ZLL
#include "zll/zb_zll_device_config.h"
#endif

#include "zcl/zb_zcl_config.h"

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"
#include "zcl/zb_zcl_reporting.h"
#include "zcl/zb_zcl_scenes.h"
#include "zcl/zb_zcl_basic.h"
#include "zcl/zb_zcl_alarms.h"
#include "zcl/zb_zcl_power_config.h"
#include "zcl/zb_zcl_identify.h"
#include "zcl/zb_zcl_on_off_switch_conf.h"
#include "zcl/zb_zcl_on_off.h"
#include "zcl/zb_zcl_groups.h"
#include "zcl/zb_zcl_door_lock.h"
#include "zcl/zb_zcl_binary_input.h"
#include "zcl/zb_zcl_level_control.h"
#include "zcl/zb_zcl_window_covering.h"
#include "zcl/zb_zcl_color_control.h"
#include "zcl/zb_zcl_shade_config.h"
#include "zcl/zb_zcl_cvc_commands.h"

#include "zcl/zb_zcl_temp_measurement.h"
#include "zcl/zb_zcl_rel_humidity_measurement.h"
#include "zcl/zb_zcl_illuminance_measurement.h"

#include "zcl/zb_zcl_ias_ace.h"
#include "zcl/zb_zcl_ias_wd.h"
#include "zcl/zb_zcl_ias_zone.h"
#include "zcl/zb_zcl_diagnostics.h"

#include "zcl/zb_zcl_custom_attributes.h"

#include "zcl/zb_zcl_time.h"
#include "zcl/zb_zcl_dehumidification_control.h"
#include "zcl/zb_zcl_el_measurement.h"
#include "zcl/zb_zcl_fan_control.h"
#include "zcl/zb_zcl_diagnostics.h"
#include "zcl/zb_zcl_en50523_appliance_events_and_alerts.h"
#include "zcl/zb_zcl_metering.h"
#include "zcl/zb_zcl_thermostat.h"
#include "zcl/zb_zcl_thermostat_ui_config.h"
#include "zcl/zb_zcl_occupancy_sensing.h"
#include "zcl/zb_zcl_meter_identification.h"


#if defined ZB_ENABLE_ZLL
#include "zll/zb_zcl_on_off_zll_adds.h"
#endif

#if defined ZB_ENABLE_HA
#include "zcl/zb_zcl_poll_control.h"
#include "zcl/zb_zcl_ota_upgrade.h"

#include "ha/zb_zcl_basic_ha_adds.h"
#include "ha/zb_zcl_ias_zone_ha_adds.h"
#include "ha/zb_zcl_ias_ace_ha_adds.h"
#include "ha/zb_zcl_ias_wd_ha_adds.h"
#include "ha/zb_zcl_power_config_ha_adds.h"
#include "ha/zb_zcl_thermostat_ha_adds.h"
#include "ha/zb_zcl_identify_ha_adds.h"
#endif

#include "zcl/zb_zcl_price.h"
#include "zcl/zb_zcl_calendar.h"
#include "zcl/zb_zcl_drlc.h"
#include "zcl/zb_zcl_messaging.h"
#include "zcl/zb_zcl_tunnel.h"
#include "zcl/zb_zcl_tunneling.h"
#include "zcl/zb_zcl_events.h"
#include "zcl/zb_zcl_subghz.h"
#include "zcl/zb_zcl_prepayment.h"
#include "zcl/zb_zcl_energy_mgmt.h"
#include "zcl/zb_zcl_mdu_pairing.h"
#include "zcl/zb_zcl_device_management.h"

#if defined ZB_HA_SUPPORT_EZ_MODE
#include "ha/zb_ha_ez_mode_comissioning.h"
#endif

#if defined ZB_BDB_MODE
//#include "zb_bdb_comissioning.h"
#endif

#if defined ZB_ENABLE_ZGP_CLUSTER
//#include "zgp/zgp_internal.h"
#endif

#include "zcl/zb_zcl_grpw_freq_agility.h"

/**
 *  \addtogroup ZB_ZCL_INITIALIZATION
 *  @{
 *    @details
 *    Most of initialization is being handled automatically during device start. However,
 *    application may set additional callbacks for event handling. Particularly:

 *    @li @ref ZB_ZCL_SET_MODIFY_ATTR_VALUE_CB macro sets callback for attribute change handling.
 *    This event could arise, for example, as a result of @ref write_attr_cmd "Write attributes"
 *    command execution.
 *    @li @ref ZB_ZCL_SET_REPORT_ATTR_CB and @ref ZB_ZCL_SET_NO_REPORTING_CB macros set
 *    callbacks for handling "report attribute" and "no reporting" callbacks respectively. For more
 *    information see @ref ZB_ZCL_REPORTING "Attribute reporting" description.
 *
 *    Sample use ZB_ZCL_SET_REPORT_ATTR_CB
 *    @snippet reporting_th.c ZB_ZCL_SET_REPORT_ATTR_CB_func
 *    @snippet reporting_th.c ZB_ZCL_SET_REPORT_ATTR_CB
 *
 *    Sample use ZB_ZCL_SET_NO_REPORTING_CB
 *    @snippet reporting_srv_th.c ZB_ZCL_SET_NO_REPORTING_CB_func
 *    @snippet reporting_srv_th.c ZB_ZCL_SET_NO_REPORTING_CB
 *
 */

/** @brief ZCL version. */
#define ZB_ZCL_VERSION       0x01

/**
 *  @brief Set modify attribute value callback.
 *  The macro sets a callback being called on external attribute change. The callback will be
 *  provided with all data necessary for correct value change handling.
 *  @param func_ptr - a callback to set (of type @ref zb_zcl_modify_attr_value_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_MODIFY_ATTR_VALUE_CB(func_ptr) ZCL_CTX().modify_attr_value_cb = (func_ptr)

/**
 *  @brief Set report attribute callback.
 *  The macro sets a callback being called on receive attribute report. The callback will
 *  be provided with all data necessary for correct attribute handling.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_report_attr_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_REPORT_ATTR_CB(func_ptr) ZCL_CTX().report_attr_cb = (func_ptr)

/**
 *  @brief Set no reporting callback.
 *  The macro sets a callback being called on remote attribute reporting timeout. The callback will
 *  be provided with all data necessary for correct attribute report timeout handling.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_no_reporting_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_NO_REPORTING_CB(func_ptr) ZCL_CTX().no_reporting_cb = (func_ptr)

/**
 *  @brief Set "Set default value all attribute" callback.
 *  The macro sets a callback being called for need to set attribute to default value.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_set_default_value_attr_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_SET_DEFAULT_VALUE_CB(func_ptr) ZCL_CTX().set_default_attr_cb = (func_ptr)

#if defined ZB_ENABLE_HA
/**
 *  @brief Set "Check attribute value" callback.
 *  The macro sets a callback being called for need to check attribute value.
 *  @param func_ptr - callback to set (of type @ref zb_zcl_check_attr_value_cb_t).
 *  @hideinitializer
 */
#define ZB_ZCL_CHECK_ATTR_VALUE_CB(func_ptr) ZCL_CTX().zb_zcl_check_attr_value_cb = (func_ptr)

/**
    @brief Set "Discovery manufacture command" callback.
    The macro sets a callback that will be called to get a list of manufacture specific commands.
    @param func_ptr - callback to set (of type @ref zb_zcl_disc_manuf_cmd_cb_t).
    @hideinitializer

    Sample use Discovery manufacture command callback

    @par Example
    Example custom callback

    zb_uint8_t list_my_manufacture_command_generate[] = {
      MY_COMMAND_GENERATE_1,
      MY_COMMAND_GENERATE_2,
      MY_COMMAND_GENERATE_3,
      MY_COMMAND_GENERATE_4,
    };

    zb_uint8_t list_my_manufacture_command_receive[] = {
      MY_COMMAND_RECEIVE_1,
      MY_COMMAND_RECEIVE_2,
      MY_COMMAND_RECEIVE_3,
    };

    zb_uint8_t my_disc_manuf_cmd_cb(
        zb_uint8_t direction, zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint16_t manuf_id, zb_bool_t recv_cmd_type, zb_uint8_t **value)
    {
       zb_uint8_t ret;

       if(manuf_id!=MY_MANUFACTURE_ID && manuf_id!=ZB_ZCL_MANUFACTURER_WILDCARD_ID)
         return 0;

      if (!recv_cmd_type)
      {
        value = &list_my_manufacture_command_generate;
        ret = sizeof(list_my_manufacture_command_generate);
      }
      else
      {
        value = &list_my_manufacture_command_receive;
        ret = sizeof(list_my_manufacture_command_receive);
      }
      return ret;
    }
    @par

    @par Example
    Example register callback

    ZB_ZCL_SET_DISC_MANUFACTURE_COMMAND_CB(&my_disc_manuf_cmd_cb);
    @par
 */
#define ZB_ZCL_SET_DISC_MANUFACTURE_COMMAND_CB(func_ptr) ZCL_CTX().disc_manuf_cmd_cb = (func_ptr)

#endif /* ZB_ENABLE_HA */

/**
 *  @brief Get current time in units.
 *  Uint = 1/10 sec
 */
#define ZB_GET_TIME_IN_UNITS()    ZB_BEACON_INTERVAL_TO_TIME_UNITS(ZB_TIMER_GET())

/**
 *  @brief Convert beacon interval to units
 *  Uint = 1/10 sec
 */
#define ZB_BEACON_INTERVAL_TO_TIME_UNITS(beacons)    ZB_TIME_BEACON_INTERVAL_TO_MSEC(beacons) / 100

/**
 *  @brief Convert units to beacon interval.
 *  Uint = 1/10 sec
 */
#define ZB_TIME_UNITS_TO_BEACON_INTERVAL(uints)  ZB_MILLISECONDS_TO_BEACON_INTERVAL((uints)*100)

/**
 * Types of device user application callbacks
 * @see zb_zcl_device_callback_param_t
 */
typedef enum zb_zcl_device_callback_id_e
{
  /** Inform user about attribute value modification */
  ZB_ZCL_SET_ATTR_VALUE_CB_ID = 0,
  /** Inform user about received attribute report */
  ZB_ZCL_REPORT_ATTR_CB_ID,
  /** Inform user that report was not received during  defined time interval */
  ZB_ZCL_NO_REPORTING_CB_ID,
  /** Inform user about change On/Off effect @see ZLL spec 6.6.1.4.4.2 */
  ZB_ZCL_ON_OFF_WITH_EFFECT_VALUE_CB_ID,
  /** */
  /* TODO: describe this callback */
  ZB_ZCL_SHADE_SET_VALUE_CB_ID,
  /** */
  /* TODO: describe this callback */
  ZB_ZCL_SHADE_GET_VALUE_CB_ID,
  /** Inform user about call identify effect command @see ZLL spec 6.3.1.2.1 */
  ZB_ZCL_IDENTIFY_EFFECT_CB_ID,
  /** Inform user about ZCL Level Contol cluster attributes value modification */
  ZB_ZCL_LEVEL_CONTROL_SET_VALUE_CB_ID,
  /** Inform user about call Start warning command @see ZCL spec 8.4.2.3.1 */
  ZB_ZCL_IAS_WD_START_WARNING_VALUE_CB_ID,
  /** Inform user about call Squawk command @see ZCL spec 8.4.2.3.2 */
  ZB_ZCL_IAS_WD_SQUAWK_VALUE_CB_ID,
  /** Inform user about call Start warning HA command @see HA spec 10.9.4.3.1 */
  ZB_ZCL_IAS_WD_START_WARNING_HA_VALUE_CB_ID,
  /** Inform user about call Arm ZCL command @see ZCL spec 8.3.2.4.1.2 */
  ZB_ZCL_IAS_ACE_ARM_VALUE_CB_ID,
  /** Inform user about call non description IAS ACE commands  */
  ZB_ZCL_IAS_ACE_VALUE_CB_ID,
  /** Inform user about OTA Upgrade commands  */
  ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID,
  /** Inform user about Basic Reset to Factory Defaults commands  */
  ZB_ZCL_BASIC_RESET_CB_ID,
  /** Inform user about call Thermostat command @see HA spec 10.1.3.3 */
  ZB_ZCL_THERMOSTAT_VALUE_CB_ID,
  /** Inform user about Check-In command from polled device */
  ZB_ZCL_POLL_CONTROL_CHECK_IN_CLI_CB_ID,
  /** Inform user about EN50523 Alert notification command @see ZCL 6.0 spec 15.4.2.4.2 */
  ZB_ZCL_EN50523_ALERT_CB_ID,
  /** Inform user about Query next image request */
  ZB_ZCL_OTA_UPGRADE_SRV_QUERY_IMAGE_CB_ID,
  /** Inform user about finished OTA upgrade */
  ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_END_CB_ID,
  /** Ask user about payload for PublishPrice request. */
  ZB_ZCL_PRICE_GET_CURRENT_PRICE_CB_ID,
  /**@b Server. Notify user about GetScheduledPrices request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_out param_in @ref zb_zcl_price_get_scheduled_prices_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - price data is available.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   *
   */
  ZB_ZCL_PRICE_GET_SCHEDULED_PRICES_CB_ID,
  /** @b Server. Notify user about GetTierLabels request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_out param_in @ref zb_zcl_price_get_tier_labels_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - price data is available. In this case application must send reply using
   * provided buffer or free buffer. Stack doesn't free buffer in this case.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   *
   */
  ZB_ZCL_PRICE_GET_TIER_LABELS_CB_ID,
  /** @b Client. Inform user about PublishPrice request.
   *
   * User's application callback is initialized by RET_OK status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_publish_price_payload_t
   * @param[out] param_out @ref zb_zcl_price_ack_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_PRICE_CLI_CMD_PRICE_ACK "PriceAcknowledgement" command.
   * @return RET_ERROR - command is handled with errors
   *
   */
  ZB_ZCL_PRICE_PUBLISH_PRICE_CB_ID,
  /** @b Client. Inform user about PublishTierLabels request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_publish_tier_labels_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors
   *
   */
  ZB_ZCL_PRICE_PUBLISH_TIER_LABELS_CB_ID,
  /** @b Server. Inform user about PriceAck command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_price_ack_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors.
   *
   */
  ZB_ZCL_PRICE_PRICE_ACK_CB_ID,
  /** Inform user about change of state for an active Load Control event */
  ZB_ZCL_DRLC_REPORT_EVENT_STATUS_CB_ID,
  /** Ask user about currently active or scheduled Load Control Events */
  ZB_ZCL_DRLC_GET_SCHEDULED_EVENTS_CB_ID,
  /** Inform user about received Load Control Event  */
  ZB_ZCL_DRLC_LOAD_CONTROL_EVENT_CB_ID,
  /** Inform user about Load Control Event cancellation  */
  ZB_ZCL_DRLC_CANCEL_LOAD_CONTROL_EVENT_CB_ID,
  /** Inform user about all Load Control Events cancellation  */
  ZB_ZCL_DRLC_CANCEL_ALL_LOAD_CONTROL_EVENTS_CB_ID,
  /** Inform user about received Message Confirmation request */
  ZB_ZCL_MESSAGING_MSG_CONFIRMATION_CB_ID,
  /** Ask user about payload for Display Message request */
  ZB_ZCL_MESSAGING_GET_LAST_MSG_CB_ID,
  /** Inform user about received Cancel Message request */
  ZB_ZCL_MESSAGING_CANCEL_MSG_CB_ID,
  /** Inform user about received Display Message request */
  ZB_ZCL_MESSAGING_DISPLAY_MSG_CB_ID,
  /** Inform user about request to setup a tunnel association with the server */
  ZB_ZCL_TUNNELING_REQUEST_TUNNEL_CB_ID,
  /** Inform user about response to a RequestTunnel command */
  ZB_ZCL_TUNNELING_REQUEST_TUNNEL_RESPONSE_CB_ID,
  /** Infrom user about received TransferData request */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_CLI_CB_ID,
  /** Infrom user about received TransferData request */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_SRV_CB_ID,
  /** Infrom user about received TransferDataError request */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_ERROR_CLI_CB_ID,
  /** Infrom user about received TransferDataError request */
  ZB_ZCL_TUNNELING_TRANSFER_DATA_ERROR_SRV_CB_ID,
  /** Inform user about closing the tunnel with the server */
  ZB_ZCL_TUNNELING_CLOSE_TUNNEL_CB_ID,
  /** Ask user about payload for PublishCalendar request  */
  ZB_ZCL_CALENDAR_GET_CALENDAR_CB_ID,
  /** Ask user about payload for PublishDayProfiles request  */
  ZB_ZCL_CALENDAR_GET_DAY_PROFILES_CB_ID,
  /** Ask user about payload for PublishWeekProfiles request  */
  ZB_ZCL_CALENDAR_GET_WEEK_PROFILES_CB_ID,
  /** Ask user about payload for PublishSeasons request  */
  ZB_ZCL_CALENDAR_GET_SEASONS_CB_ID,
  /** Ask user about payload for PublishSpecialDays request  */
  ZB_ZCL_CALENDAR_GET_SPECIAL_DAYS_CB_ID,
  /** Ask user about payload for CancelCalendar request  */
  ZB_ZCL_CALENDAR_GET_CALENDAR_CANCELLATION_CB_ID,
  /** Inform user about PublishCalendar request */
  ZB_ZCL_CALENDAR_PUBLISH_CALENDAR_CB_ID,
  /** Inform user about PublishDayProfile request  */
  ZB_ZCL_CALENDAR_PUBLISH_DAY_PROFILE_CB_ID,
  /** Inform user about PublishWeekProfile request  */
  ZB_ZCL_CALENDAR_PUBLISH_WEEK_PROFILE_CB_ID,
  /** Inform user about PublishSeasons request  */
  ZB_ZCL_CALENDAR_PUBLISH_SEASONS_CB_ID,
  /** Inform user about PublishSpecialDays request  */
  ZB_ZCL_CALENDAR_PUBLISH_SPECIAL_DAYS_CB_ID,
  /** Inform user about CancelCalendar request  */
  ZB_ZCL_CALENDAR_CANCEL_CALENDAR_CB_ID,
  /** Inform user about ReportEventStatus request  */
  ZB_ZCL_ENERGY_MANAGEMENT_REPORT_EVENT_STATUS_CB_ID,
  /** Inform user about ManageEvent request  */
  ZB_ZCL_ENERGY_MANAGEMENT_MANAGE_EVENT_CB_ID,
  /** Inform user about MDU Paring Request. You may provide MDU list as reply  */
  ZB_ZCL_MDU_PAIRING_REQUEST_CB_ID,
  /** Inform user about MDU Pairing Response. List of MDU Paired devices passed */
  ZB_ZCL_MDU_PAIRING_RESPONSE_CB_ID,
  /** @b Server. Inform user about GetProfile request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_profile_payload_t
   * @param[out] param_out @ref zb_zcl_metering_get_profile_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_GET_PROFILE_RESPONSE "GetProfileResponse" command.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_PROFILE_CB_ID,
  /** @b Server. Inform user about RequestFastPollMode request.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_request_fast_poll_mode_payload_t
   * @param[out] param_out @ref zb_zcl_metering_request_fast_poll_mode_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_REQUEST_FAST_POLL_MODE_RESPONSE "RequestFastPollModeResponse" command.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_CB_ID,
  /** @b Server. Inform user about GetSnapshot request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_snapshot_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.  In this case application must send reply using
   *                  provided buffer or free buffer. Stack doesn't free buffer in this case.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SNAPSHOT_CB_ID,
  /** @b Server. Inform user about GetSampledData request.
   *
   * User's application callback is initialized by RET_NOT_FOUND status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_sampled_data_payload_t
   * @param[out] param_out @ref zb_zcl_metering_get_sampled_data_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command.
   *                  Send @ref ZB_ZCL_METERING_SRV_CMD_GET_SAMPLED_DATA_RESPONSE "GetSampledData" command.
   * @return RET_NOT_FOUND - price data isn't available. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SAMPLED_DATA_CB_ID,
  /** @b Client. Inform user about response to GetProfileResponse command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_profile_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_PROFILE_RESPONSE_CB_ID,
  /** @b Client. Inform user about response to RequestFastPollMode command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_request_fast_poll_mode_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_RESPONSE_CB_ID,
  /** @b Client. Inform user about response to GetSnapshot command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_publish_snapshot_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_PUBLISH_SNAPSHOT_CB_ID,
  /** @b Client. Inform user about response to GetSampledData command.
   *
   * User's application callback is initialized by RET_ERROR status of device
   * callback parameters.
   * @param[in] param_in @ref zb_zcl_metering_get_sampled_data_response_payload_t
   *
   * One of the following statuses must be returned:
   * @return RET_OK - successfully handle command. Default Response will be send if requested.
   * @return RET_ERROR - command is handled with errors. Default Response will be send if requested.
   *
   */
  ZB_ZCL_METERING_GET_SAMPLED_DATA_RESPONSE_CB_ID
} zb_zcl_device_callback_id_t;

/** @cond internals_doc */
/** Generic in/out params for cluster commands handlers. */
typedef struct zb_zcl_device_cmd_generic_param_s
{
  /** Input packet header metadata.
   * Represents packet header information for incomming packets.
   */
  const zb_zcl_parsed_hdr_t *in_cmd_info;

  /** Input packet payload. */
  const zb_void_t *in;

  /** Output packet payload.
   * User application should fill the payload with specific data.
   * ZCL can fill some of fields of payload with default/calculated values.
   */
  zb_void_t *out;
} zb_zcl_device_cmd_generic_param_t;
/** @endcond */ /* internals_doc */

/**
 *  @brief Data for Device user application callback.
 *  This structure has receive to User application callback (see @ref zb_callback_t).
 *  @hideinitializer
 * */
typedef struct zb_zcl_device_callback_param_s
{
  /** Type of device callback (see @ref zb_zcl_device_callback_id_e) */
  zb_zcl_device_callback_id_t device_cb_id;
  zb_uint8_t endpoint;
  /** Return status (see @ref zb_ret_t) */
  zb_ret_t status;
  /** Callback custom data */
  union
  {
    zb_zcl_set_attr_value_param_t  set_attr_value_param;
#if defined ZB_ENABLE_ZLL
    /* Off with effect command, On/Off cluster */
    zb_zcl_on_off_set_effect_value_param_t  on_off_set_effect_value_param;
#endif /* ZB_ENABLE_ZLL */
    /* */
    zb_zcl_identify_effect_value_param_t  identify_effect_value_param;
   zb_zcl_shade_set_value_param_t  shade_set_value_param;
   zb_zcl_shade_get_value_param_t  shade_get_value_param;
   //
   zb_zcl_level_control_set_value_param_t  level_control_set_value_param;
   //
   zb_zcl_ias_wd_start_warning_value_param_t  start_warning_value_param;
#if defined ZB_ENABLE_HA
   //
   zb_zcl_ias_wd_start_warning_ha_value_param_t start_warning_ha_value_param;
   //
   zb_zcl_thermostat_value_param_t thermostat_value_param;
#endif /* defined ZB_ENABLE_HA */
   //
   zb_zcl_ias_wd_squawk_value_param_t  squawk_value_param;
   //
   zb_zcl_ias_ace_arm_value_param_t  arm_value_param;
   //
   zb_zcl_ias_ace_value_param_t  ace_value_param;
#if defined ZB_ENABLE_HA
#if defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN
   //
   zb_zcl_ota_upgrade_value_param_t  ota_value_param;
#endif /*defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN */
   zb_zcl_en50523_alert_param_t             en50523_alert_param;
   zb_zcl_poll_control_check_in_cli_param_t checkin_cli_param;
#if defined ZB_HA_ENABLE_OTA_UPGRADE_SERVER
   zb_zcl_ota_upgrade_srv_query_img_param_t   ota_upgrade_srv_query_img_param;
   zb_zcl_ota_upgrade_srv_upgrade_end_param_t ota_upgrade_srv_upgrade_end_param;
#endif
#endif /* defined ZB_ENABLE_HA */
    zb_zcl_device_cmd_generic_param_t gnr;
  }
  cb_param;
} zb_zcl_device_callback_param_t;


/** @cond internals_doc */
/** The way of accessing to @ref zb_zcl_device_callback_param_t from buf ref. */
#define ZB_ZCL_DEVICE_CMD_PARAM(_param) \
    (ZB_GET_BUF_PARAM((ZB_BUF_FROM_REF(_param)), zb_zcl_device_callback_param_t))

/** Set INPUT device callback parameter with specific value. */
#define ZB_ZCL_DEVICE_CMD_PARAM_IN_SET(_param, _pvalue) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in = _pvalue)

/** Set OUTPUT device callback parameter with specific value. */
#define ZB_ZCL_DEVICE_CMD_PARAM_OUT_SET(_param, _pvalue) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.out = _pvalue)

/** Init all fields of device callback params. */
#define ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(_param, _cb_id, _status, _cmd_info, _in, _out) \
  (ZB_BZERO(ZB_ZCL_DEVICE_CMD_PARAM(_param), sizeof(*ZB_ZCL_DEVICE_CMD_PARAM(_param))), \
   (ZB_ZCL_DEVICE_CMD_PARAM_CB_ID(_param) = _cb_id, \
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(_param) = _status, \
     (ZB_ZCL_DEVICE_CMD_PARAM_CMD_INFO(_param) = _cmd_info, \
      (ZB_ZCL_DEVICE_CMD_PARAM_IN_SET(_param, _in), \
       (ZB_ZCL_DEVICE_CMD_PARAM_OUT_SET(_param, _out)))))))
/** @endcond */ /* internals_doc */

/** Get INPUT device callback parameter from buffer reference.
 * @param _param - Reference to buffer.
 * @param _ptype - Target data type.
 * @return - Pointer to @ref _ptype data.
 * @see @ref zb_zcl_device_callback_param_t.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_IN_GET(_param, _ptype) \
  ((const _ptype *) ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in))

/** Get OUTPUT device callback parameter from buffer reference.
 * @param _param - Reference to buffer.
 * @param _type - Target data type
 * @return - Pointer to @ref _ptype data.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_OUT_GET(_param, _ptype) \
  ((_ptype *) ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.out))

/** Get access to @ref zb_zcl_parsed_hdr_t data from device callback parameter.
 * @param _param - Reference to buffer.
 * @return - Pointer to @ref zb_zcl_parsed_hdr_t structure.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_CMD_INFO(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->cb_param.gnr.in_cmd_info)

/** Get access to @ref device_cb_id of device callback parameters.
 * @param _param - Reference to buffer.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_CB_ID(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->device_cb_id)

/** Get access to @ref status of device callback parameters.
 * @param _param - Reference to buffer.
 * @return - @ref zb_ret_t value of current status.
 */
#define ZB_ZCL_DEVICE_CMD_PARAM_STATUS(_param) \
  ((ZB_ZCL_DEVICE_CMD_PARAM(_param))->status)

/** @cond internals_doc */
/** @brief ZCL default handler.
  *
  * This function handles ZCL commands which was not processed by application.
  *
  * @param param - reference to the buffer containing ZCL packet.
  * @return @ref ZB_TRUE if packet was processed; @ref ZB_FALSE - otherwise
  */
zb_uint8_t zb_zcl_handle(zb_uint8_t param);
/**
   Get ZCL TSN stored in ZCL frame
   @param buffer - pointer to a buffer with ZCL frame
   @return TSN value
*/
zb_uint8_t zb_zcl_get_tsn_from_packet(zb_buf_t *buffer);

zb_void_t zb_zcl_adjust_reporting_timer(zb_uint8_t param);

zb_void_t zb_zcl_update_reporting_info(zb_zcl_reporting_info_t *rep_info);

zb_ret_t zb_zcl_put_default_reporting_info(zb_zcl_reporting_info_t* default_rep_info_ptr);

zb_void_t zb_zcl_remove_default_reporting_info(zb_uint16_t cluster_id, zb_uint8_t cluster_role);

zb_void_t zb_zcl_mark_report_not_sent(zb_zcl_reporting_info_t *rep_info);

/**
   Register device context.
   @param _device_ctx - pointer to device context

   Use @ref ZB_AF_REGISTER_DEVICE_CTX.
 */
void zb_zcl_register_device_ctx(zb_af_device_ctx_t *device_ctx);
zb_bool_t zb_zcl_check_cluster_list();
/** @endcond */ /* internals_doc */
/** @} */ /* ZB_ZCL_INITIALIZATION */


#ifdef ZB_BDB_MODE
/*! \addtogroup zboss_bdb_api */
/*! @{ */


/** @cond internals_doc */
/**
  @brief BDB error codes
  */
enum zb_bdb_error_codes_e
{
  ZB_BDB_STATUS_SUCCESS = 0,
  /*
    Smart guys from Zigbee Alliance forgotten to define error code values, so what
    can I do here? See Table 5  Values of the bdbCommissioningStatus attribute.

  SUCCESS
The commissioning sub-procedure was successful.
IN_PROGRESS
One of the commissioning sub-procedures has started but is not yet complete.
NOT_AA_CAPABLE
The initiator is not address assignment capable during touchlink.
NO_NETWORK
A network has not been found during network steering or touchlink.
TARGET_FAILURE
A node has not joined a network when requested during touchlink.
FORMATION_FAILURE
A network could not be formed during network formation.
NO_IDENTIFY_QUERY_-RESPONSE
No response to an identify query command has been received during finding & binding.
BINDING_TABLE_FULL
A binding table entry could not be created due to insufficient space in the binding table during finding & binding.
NO_SCAN_RESPONSE
No response to a scan request inter-PAN command has been received during touchlink.
NOT_PERMITTED
A touchlink (steal) attempt was made when a node is already connected to a centralized security network.
TCLK_EX_FAILURE
The Trust Center link key exchange procedure has failed attempting to join a centralized security network.
  */
  ZB_BDB_STATUS_IN_PROGRESS,
  ZB_BDB_STATUS_NOT_AA_CAPABLE,
  ZB_BDB_STATUS_NO_NETWORK,
  ZB_BDB_STATUS_TARGET_FAILURE,
  ZB_BDB_STATUS_FORMATION_FAILURE,
  ZB_BDB_STATUS_NO_IDENTIFY_QUERY_RESPONSE,
  ZB_BDB_STATUS_BINDING_TABLE_FULL,
  ZB_BDB_STATUS_NO_SCAN_RESPONSE,
  ZB_BDB_STATUS_NOT_PERMITTED,
  ZB_BDB_STATUS_TCLK_EX_FAILURE
};
/** @endcond */ /* internals_doc */


/** @brief BDB commissioning mode mask bits */
typedef enum zb_bdb_commissioning_mode_mask_e
{
  /* Used internally */
  ZB_BDB_INITIALIZATION = 0,
  /** Touchlink: 0 = Do not attempt Touchlink commissioning;
                  1 = Attempt Touchlink commissioning
   */
  ZB_BDB_TOUCHLINK_COMMISSIONING = 1,
  /** Network steering: 0 = Do not attempt network steering;
                         1 = Attempt network steering
   */
  ZB_BDB_NETWORK_STEERING = 2,
  /** Network formation: 0 = Do not attempt to form a network;
                          1 = Attempt to form a network, according to device type2
   */
  ZB_BDB_NETWORK_FORMATION = 4,
  /** Finding & binding: 0 = Do not attempt finding & binding;
                          1 = Attempt finding & binding
    @note actually this mode does not call Finding & Binding procedure. Use
          @ref zb_bdb_finding_binding_target or @ref zb_bdb_finding_binding_initiator.
   */
  ZB_BDB_FINDING_N_BINDING = 8,
  /* Used internally */
  ZB_BDB_LAST_COMMISSIONING_STEP = 0x10,
  /* Used internally */
  ZB_BDB_COMMISSIONING_STOP = 0x20,
  /* Used internally */
  ZB_BDB_TOUCHLINK_TARGET = 0x40,
} zb_bdb_commissioning_mode_mask_t;

/**
   @brief Start top level commissioning procedure with specified mode mask.

   @param mode_mask - commissioning modes, see @ref zb_bdb_commissioning_mode_mask_e

   @b Example:
   @code
   zb_zdo_app_signal_hdr_t *sg_p = NULL;
   zb_zdo_app_signal_type_type_t sig = zb_get_app_signal(param, &sg_p);

   switch(sig)
     {
       case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
         TRACE_MSG(TRACE_APP1, "Device STARTED OK", (FMT__0));
         zb_bdb_set_legacy_device_support(1);
         bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
         break;

       case ZB_BDB_SIGNAL_STEERING:
         TRACE_MSG(TRACE_APP1, "Successfull steering", (FMT__0));
         break;
     }
   @endcode
  */
zb_bool_t bdb_start_top_level_commissioning(zb_uint8_t mode_mask);

/**
   @brief Starts EZ-Mode Finding and binding mechanism at the target's endpoint.

   Put device into identifying mode; default duration is 3 minutes.

   @param endpoint - target endpoint
   @return RET_OK if procedure was successfully started

   @note: endpoint should be registered on target

   @b Example
   @snippet fb_dap_tc_01a_dutzc.c zb_bdb_finding_binding_target_example
  */
zb_ret_t zb_bdb_finding_binding_target(zb_uint8_t endpoint);


/**
 * List of EZ-Mode binding callback states
 */
typedef enum zb_bdb_comm_binding_cb_state_e
{
  /** Previously user applied bind finished successfully */
  ZB_BDB_COMM_BIND_SUCCESS = 0,
  /** Previously user applied bind failed */
  ZB_BDB_COMM_BIND_FAIL = 1,
  /** Ask user whether to perform binding */
  ZB_BDB_COMM_BIND_ASK_USER = 2,
} zb_bdb_comm_binding_cb_state_t;

/**
 *  @brief BDB finding & binding callback template.
 *
 *  Function is used both to interact with user application, get decision
 *  if new binding is needed or not, and to report the binding result
 *
 *  @param status - status of the binding (ask user, success or fail) @see zb_bdb_comm_binding_cb_state_t
 *  @param addr - extended address of a device to bind
 *  @param ep - endpoint of a device to bind
 *  @param cluster - cluster id to bind
 *  @return bool - agree or disagree
 *
 * @b Example:
 * @snippet fb_ini_tc_06a_dut_ci.c zb_bdb_finding_binding_initiator
 */
typedef zb_bool_t (ZB_CODE * zb_bdb_comm_binding_callback_t)(
  zb_int16_t status, zb_ieee_addr_t addr, zb_uint8_t ep, zb_uint16_t cluster);

/**
 *  @brief Start BDB finding & binding procedure on initiator.
 *
 *  Summary: Finding and binding as initiator zb_bdb_finding_binding_initiator()
 *  returns RET_OK if procedure was started successfully, error code otherwise. To report procedure
 *  status, user callback is called. It may be called several times with Success status and only once
 *  with error status. If any error appears, finding and binding stops.
 *
 *  @param endpoint - initiator endpoint
 *  @param user_binding_cb - user callback, see @ref zb_bdb_comm_binding_callback_t
 *  @return bool - RET_OK or error code
 *
 * @b Example:
 * @snippet fb_ini_tc_06a_dut_ci.c zb_bdb_finding_binding_initiator
 */
zb_ret_t zb_bdb_finding_binding_initiator(zb_uint8_t endpoint, zb_bdb_comm_binding_callback_t user_binding_cb);

/** Cancel previously started Finding & Binding procedure on target */
void zb_bdb_finding_binding_target_cancel();

/**
   Set the primary channel set for the BDB energy scan. Beacon request
   will be send on these channels
   @param channel_mask - Channel mask.
*/
zb_void_t zb_set_bdb_primary_channel_set(zb_uint32_t channel_mask);
/**
   Get the primary channel set for the BDB energy scan.
   @return channel_mask - Channel mask.
*/
zb_uint32_t zb_get_bdb_primary_channel_set();

/**
   Set the secondary channel set for the BDB energy scan. Beacon request
   will be send on these channels if no network found after energy
   scan on the primary channels.
   @param channel_mask - Channel mask.
*/
zb_void_t zb_set_bdb_secondary_channel_set(zb_uint32_t channel_mask);

/**
   Get the secondary channel set for the BDB energy scan.
   @return channel_mask - Channel mask.
*/
zb_uint32_t zb_get_bdb_secondary_channel_set();

/**
 Enable Zigbee PRO complaint commissioning support
 @param state - 1 to enable trust center require key exchange; 0 -
 disable.
*/
zb_void_t zb_bdb_set_legacy_device_support(zb_uint8_t state);

/**
   Set commissioning mode.
   @param  commissioning_mode - bitfield with the bdbCommissioningMode
   attribute. Set 1 to the corresponding bit to enable, 0 to disable:
   Bit number                   Description
       0              Enables/disables Touchlink commissioning
       1              Attempt network steering
       2              Attempt to form a network
       3              Attempt finding & binding
   @see zb_bdb_commissioning_mode_mask_t
 */
zb_void_t zb_set_bdb_commissioning_mode(zb_uint8_t commissioning_mode);


/**
 * Maximum endpoints of the "respondent" that can be served
 */
#define ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN 4

/**
 * Identify query responses queue size
 */
#define BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER 4

/**
 * List of EZ-Mode commissioning states
 */
typedef enum zb_bdb_comm_state_e
{
  /** EZ-Mode isn't invoked */
  ZB_BDB_COMM_IDLE                  = 0,
  /** EZ-Mode factory reset in progress */
  ZB_BDB_COMM_FACTORY_RESET         = 1,
  /** EZ-Mode network steering in progress (scanning or forming network) */
  ZB_BDB_COMM_NWK_STEERING          = 2,
  /**  EZ-Mode network steering in progress (scanning or forming network finished) */
  ZB_BDB_COMM_NWK_STEERING_JOINED   = 3,
  /** EZ-Mode finding and binding in progress (on initiator) */
  ZB_BDB_COMM_FINDING_AND_BINDING   = 4,
  /** EZ-Mode finding and binding in progress (on target) */
  ZB_BDB_COMM_FINDING_AND_BINDING_TARGET   = 5,
}
  zb_bdb_comm_state_t;

/** @cond internals_doc */
typedef ZB_PACKED_PRE struct zb_bdb_comm_respondent_info_s
{
  /** Address reference in Translation Table for "respondent" */
  zb_address_ieee_ref_t addr_ref;
  /** Respondent's active endpoints list */
  zb_uint8_t ep_list[ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN];
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;
  /** Count of the currently checked endpoints */
  zb_uint8_t eps_checked;
} ZB_PACKED_STRUCT zb_bdb_comm_respondent_info_t;

/**
 * BDB commissioning context
 */
typedef ZB_PACKED_PRE struct zb_bdb_comm_ctx_s
{
  /** State of commissioning */
  zb_bdb_comm_state_t state;
  zb_ret_t status;
  /** Callback function invoked when factory fresh or network steering operations finish */
  zb_callback_t user_cb;
  /** Callback function invoked when binding operation start */
  zb_bdb_comm_binding_callback_t finding_binding_progress_cb;
/* [AV] After having separated binding user callbacks from f&b complete callback
    the last one can be stored in the user_cb field. */
  /** Duration of PermitJoining and IdentifyTime */
  zb_uint16_t duration;
/*Data array to store info from Identity_Query_Resp */
  zb_bdb_comm_respondent_info_t respondent[BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER];
  zb_uint8_t respondent_number;
  /** Endpoint which participate in finding and binding*/
  zb_uint8_t ep;
  /** Endpoint descriptor*/
  zb_af_endpoint_desc_t *ep_desc;
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;
  /** Signals that at least one endpoint was bound during finding and binding;
    * it is used to invoke user callback if no endpoint was binded
    */
  zb_bool_t was_bound;
  /*EZ mode in progress flag. Sets for EZScanTimeout*/
  zb_bool_t ez_scan_inprogress;
  /** Reference to the buffer holding simple descriptor response */
  zb_uint8_t simple_desc_buf_ref;
} ZB_PACKED_STRUCT zb_bdb_comm_ctx_t;
/** @endcond */ /* internals_doc */

/*! @} */ /* zboss_bdb_api */
#endif /*#ifdef ZB_BDB_MODE*/


/** \addtogroup ZB_ZCL_INITIALIZATION
 *  @{
 */

typedef struct zcl_cluster_handlers_s
{
  zb_uint16_t cluster_id;
  zb_uint8_t cluster_role;
  zb_zcl_cluster_handler_t cluster_handler;
  zb_zcl_cluster_check_value_t cluster_check_value;
  zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook;
} zcl_cluster_handlers_t;

#define ZB_ZCL_CLUSTER_HANDLERS_TABLE_SIZE 20

/** @cond internals_doc */
typedef struct zb_zcl_globals_s
{
  /** @internal ZCL sequence number used when send zcl frames */
  zb_uint8_t seq_number;
  /** @internal Buffer to store runtime data during ZCL commands processing */

//TODO: can be used for SHADE PROCESSING?
  zb_buf_t *runtime_buf;

  /** @internal Store device context information */
  zb_af_device_ctx_t *device_ctx;
  /** @internal Array for internal device ctx and user device ctx. To be used
   * by ZGP. */
  zb_af_device_ctx_t *device_ctx_arr[2];
  /** @internal ZCL transaction hash */
  zcl_cb_hash_ent_t zcl_cb[ZCL_TRAN_TABLE_SIZE];
  /** @internal User defined callback, used to inform user about attribute value  modification */
  zb_zcl_modify_attr_value_cb_t modify_attr_value_cb;
  /** @internal User defined callback, is used to inform user about received attribute report */
  zb_zcl_report_attr_cb_t report_attr_cb;
  /** @internal User defined callback, is used to inform user that report was not received during
    * defined time interval */
  zb_zcl_no_reporting_cb_t no_reporting_cb;
  /** @internal User defined API callback, is used to connection with User Application */
  zb_callback_t device_cb;
  zb_callback_t checkin_cb;
  /** @internal Stores context information for reporting */
  zb_zcl_reporting_ctx_t reporting_ctx;
  /** @internal Set default value all attributes */
  zb_zcl_set_default_value_attr_cb_t set_default_attr_cb;
  /** @internal Check attribute value */
  zb_zcl_zb_zcl_check_attr_value_cb_t zb_zcl_check_attr_value_cb;
#ifdef IAS_OBSOLETE
#if !defined ZB_IAS_ZONE_WITHOUT_HOOK && defined ZB_ENABLE_HA
  /** @internal Parameters for processing IAS Zone Notify when HOOK procedure */
  zb_zcl_ias_zone_set_status_param_t *ias_zone_notify_param;
#endif /* !defined ZB_IAS_ZONE_WITHOUT_HOOK */
#endif  /* IAS_OBSOLETE */
#ifdef ZB_HA_ENABLE_OTA_UPGRADE_SERVER
  zb_zcl_ota_upgrade_next_data_ind_t ota_upgrade_next_data_ind_cb;
#endif /* ZB_HA_ENABLE_OTA_UPGRADE_SERVER */
#ifdef ZB_HA_ENABLE_OTA_UPGRADE_CLIENT
  zb_zcl_ota_upgrade_cli_ctx_t ota_cli;
#endif
#if defined ZB_HA_SUPPORT_EZ_MODE
  /** @internal HA EZ-Mode commissioning context */
  zb_ha_ez_mode_ctx_t ha_ez_mode_ctx;
#endif /*ZB_HA_SUPPORT_EZ_MODE*/
#if defined ZB_BDB_MODE
  /** @internal BDB commissioning context */
  zb_bdb_comm_ctx_t bdb_comm_ctx;
#endif /*ZB_BDB_MODE*/

#ifdef ZB_ZCL_SUPPORT_CLUSTER_TUNNEL
  zb_zcl_tunnel_context_t tunnel_ctx;
#endif /* ZB_ZCL_SUPPORT_CLUSTER_TUNNEL */
#if defined ZB_ENABLE_HA
  zb_zcl_disc_manuf_cmd_cb_t disc_manuf_cmd_cb;
#endif /* ZB_ENABLE_HA */

  zcl_cluster_handlers_t zcl_handlers[ZB_ZCL_CLUSTER_HANDLERS_TABLE_SIZE];
  zb_uint8_t zcl_handlers_cnt;
} zb_zcl_globals_t;
/** @endcond */ /* internals_doc */

/**
   Returns ZCL context.
   @return pointer ZCL context
  */
zb_zcl_globals_t *zb_zcl_get_ctx();
/** Returns pointer to ZCL context */
#define ZCL_CTX() (*zb_zcl_get_ctx())

/*! @} */ /* ZB_ZCL_INITIALIZATION */

#define ZB_ZCL_MAX_EP_NUMBER 10

typedef zb_uint8_t *(*zb_zcl_put_payload_cb_t)(zb_uint8_t *, const zb_void_t *);

zb_void_t zb_zcl_send_cmd(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_zcl_frame_direction_t direction,
  zb_uint8_t src_ep,
  const zb_void_t *payload,
  zb_uint8_t payload_size,
  zb_zcl_put_payload_cb_t put_payload,
  zb_zcl_cluster_id_t cluster_id,
  zb_zcl_disable_default_response_t def_resp,
  zb_uint16_t cmd_id,
  zb_callback_t cb
);

zb_bool_t zb_zcl_send_default_handler(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info, zb_zcl_status_t status);

#endif /* ZBOSS_API_ZCL_H */
