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
 * PURPOSE: EN50523 Appliance events and alerts cluster defintions
*/

#if ! defined ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_H
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_H

/** @cond DOXYGEN_ZCL_SECTION */

/*! @brief EN50523 Appliance events and alerts cluster
    command received identifiers
    @see ZCL 6.0 spec, subclause 15.4.2.3
*/
enum zb_zcl_en50523_appl_ev_and_alerts_recv_cmd_e
{
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_ID = 0x00, /**< "Get alerts" command. */
};

/*! @brief EN50523 Appliance events and alerts cluster
    commands generated identifiers
    @see ZCL 6.0 spec, subclause 15.4.2.4
*/
enum zb_zcl_en50523_appl_ev_and_alerts_send_cmd_e
{
  /*! "Get alerts response" command. */
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_RESP_ID     = 0x00,
  /*! "Alerts notification" command. */
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERTS_NOTIFICATION_ID = 0x01,
  /*! "Event notification" command. */
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_EVENT_NOTIFICATION_ID  = 0x02,
};

#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_NUM(_alerts_cnt)  \
  (  (_alerts_cnt) & 0x0F         )
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_TYPE(_alerts_cnt) \
  ( ((_alerts_cnt) & 0xF0) >> 4   )
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERT_NOT_ID(_alert)         \
  (  (_alert) & 0x000000FF)
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERT_NOT_CATEGORY(_alert)   \
  ( ((_alert) & 0x00000F00) >> 8  )
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERT_NOT_PRESENCE(_alert)   \
  ( ((_alert) & 0x00003000) >> 12 )

enum zb_zcl_en50523_appl_ev_and_alerts_alerts_type_e
{
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERTS_TYPE_UNSTRUCTURED = 0x00,
};

enum zb_zcl_en50523_appl_ev_and_alerts_alert_category_e
{
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERT_CATEGORY_WARNING = 0x01,
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERT_CATEGORY_DANGER  = 0x02,
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERT_CATEGORY_FAILURE = 0x03,
};

enum zb_zcl_en50523_appl_ev_and_alerts_alert_presence_e
{
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERT_RECOVERY = 0x00,
  ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERT_PRESENCE = 0x01,
};

/* Calculated basing on mask in ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_NUM(...) */
#define ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_MAX_ALERTS_NUM  15

typedef ZB_PACKED_PRE struct zb_zcl_en50523_parsed_alert_s
{
  zb_uint8_t id;
  zb_uint8_t category;
  zb_uint8_t presence;
}
  ZB_PACKED_STRUCT zb_zcl_en50523_parsed_alert_t;

typedef ZB_PACKED_PRE struct zb_zcl_en50523_alert_param_s
{
  zb_uint16_t short_addr;
  zb_uint8_t  endpoint;
  zb_zcl_en50523_parsed_alert_t *alert;
}
 ZB_PACKED_STRUCT zb_zcl_en50523_alert_param_t;

zb_bool_t zb_zcl_process_en50523_appliance_events_and_alerts(zb_uint8_t param);

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_en50523_appliance_events_and_alerts_init_server();
zb_void_t zb_zcl_en50523_appliance_events_and_alerts_init_client();
#define ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS_SERVER_ROLE_INIT zb_zcl_en50523_appliance_events_init_server
#define ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_ROLE_INIT zb_zcl_en50523_appliance_events_init_client

#endif  /* ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_H */
