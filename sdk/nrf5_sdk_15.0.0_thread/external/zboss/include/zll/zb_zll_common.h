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
 * PURPOSE: ZLL common definitions
*/

#if ! defined ZB_ZLL_COMMON_H
#define ZB_ZLL_COMMON_H

#include "zb_config.h"
#include "zll/zb_zll_config.h"
#include <zboss_api_core.h> /* int types, zb_buf_t */
#include "zcl/zb_zcl_common.h"

/************************* Include ZLL device definition files *************************/
#include "zll/zb_zll_on_off_light.h"
#include "zll/zb_zll_on_off_plugin_unit.h"
#include "zll/zb_zll_dimmable_light.h"
#include "zll/zb_zll_dimmable_plugin_unit.h"
#include "zll/zb_zll_color_light.h"
#include "zll/zb_zll_extended_color_light.h"
#include "zll/zb_zll_color_temperature_light.h"

#include "zll/zb_zll_color_controller.h"
#include "zll/zb_zll_color_scene_controller.h"
#include "zll/zb_zll_non_color_controller.h"
#include "zll/zb_zll_non_color_scene_controller.h"
#include "zll/zb_zll_control_bridge.h"
#include "zll/zb_zll_on_off_sensor.h"

/******************** Include ZLL additions to ZCL definition files ********************/
#include "zll/zb_zcl_basic_zll_adds.h"
#include "zll/zb_zcl_color_control_zll_adds.h"
#include "zll/zb_zcl_groups_zll_adds.h"
#include "zll/zb_zcl_level_control_zll_adds.h"
#include "zll/zb_zcl_on_off_zll_adds.h"
#include "zll/zb_zcl_scenes_zll_adds.h"

#include "zll/zb_zll_commissioning.h"

/*!
    @defgroup ZB_ZLL_CLUSTERS_ADDS ZLL Extension clusters
    @ingroup ZB_ZLL

    @defgroup ZB_ZLL_COMMON ZLL Common constants, structures and functions
    @ingroup ZB_ZLL
    @addtogroup ZB_ZLL_COMMON
    ZLL Common constants, structures and functions
    @{
*/

#ifdef DEBUG
/* If defined, samples included to zll source files will be compiled - need for development only */
#define ZB_COMPILE_ZLL_SAMPLE
#endif


/** @brief Identifiers for clusters specific to ZLL profile. */
enum zb_zll_cluster_id_e
{
  /** ZLL.Commissioning cluster identifier. */
  ZB_ZLL_CLUSTER_ID_COMMISSIONING = 0x1000
};/* enum zb_zll_cluster_id_e */

/** @brief ZLL.aplcInterPANTransIdLifetime constant value.
  * @see ZLL spec, subclause 8.1.8.
  */
#define ZB_ZLL_APLC_INTRP_TRANSID_LIFETIME ( ZB_MILLISECONDS_TO_BEACON_INTERVAL(8000))


//#if defined ZB_NS_BUILD

// Added 1 sec - transport linux pipes may wait 1 sec
//#define ZB_ZLL_APLC_SCAN_TIME_BASE_DURATION ( ZB_MILLISECONDS_TO_BEACON_INTERVAL(250+1000))

//#else
/** @brief ZLL.aplcScanTimeBaseDuration constant value.
  * @see ZLL spec, subclause 8.1.8.
  */
#define ZB_ZLL_APLC_SCAN_TIME_BASE_DURATION ( ZB_MILLISECONDS_TO_BEACON_INTERVAL(250))

//#endif

/** @brief ZLL.aplcRxWindowDuration constant value.
  * @see ZLL spec, subclause 8.1.8.
  */
#define ZB_ZLL_APLC_RX_WINDOW_DURATION (ZB_MILLISECONDS_TO_BEACON_INTERVAL(5000))

/**
 *  @brief ZLL.aplcMinStartupDelayTime.
 *  @see ZLL spec, subclause 8.1.8.
 */
#define ZB_ZLL_APLC_MIN_STARTUP_DELAY_TIME  (ZB_MILLISECONDS_TO_BEACON_INTERVAL(2000))

/** @brief Minimum RSSI value to process ScanRequest packets. */
#define ZB_ZLL_RSSI_THRESHOLD 47

/** @brief ZLL device identifiers. */
enum zb_zll_device_e
{
  ZB_ZLL_ON_OFF_LIGHT_DEVICE_ID = 0x0000,               /**< On/Off light. */
  ZB_ZLL_ON_OFF_PLUGIN_UNIT_DEVICE_ID = 0x0010,         /**< On/Off light plugin. */
  ZB_ZLL_DIMMABLE_LIGHT_DEVICE_ID = 0x0100,             /**< Dimmable light. */
  ZB_ZLL_DIMMABLE_PLUGIN_UNIT_DEVICE_ID = 0x0110,       /**< Dimmable light plugin. */
  ZB_ZLL_COLOR_LIGHT_DEVICE_ID = 0x0200,                /**< Color light. */
  ZB_ZLL_EXTENDED_COLOR_LIGHT_DEVICE_ID = 0x0210,       /**< Extended color light. */
  ZB_ZLL_COLOR_TEMPERATURE_LIGHT_DEVICE_ID = 0x0220,    /**< Color temperature light. */
  ZB_ZLL_COLOR_CONTROLLER_DEVICE_ID = 0x0800,           /**< Color controller. */
  ZB_ZLL_COLOR_SCENE_CONTROLLER_DEVICE_ID = 0x0810,     /**< Color scene controller. */
  ZB_ZLL_NON_COLOR_CONTROLLER_DEVICE_ID = 0x0820,       /**< Non-color controller. */
  ZB_ZLL_NON_COLOR_SCENE_CONTROLLER_DEVICE_ID = 0x0830, /**< Non-color scene controller. */
  ZB_ZLL_CONTROL_BRIDGE_DEVICE_ID = 0x0840,             /**< Control bridge. */
  ZB_ZLL_ON_OFF_SENSOR_DEVICE_ID = 0x0850               /**< On/Off sensor. */
};

/** No attributes for reporting in ZLL profile. */
#define ZB_ZLL_REPORT_ATTR_COUNT  0

/**
 *  @brief Initializes ZLL device.
 */
zb_void_t zll_init(zb_void_t);

/* 12/11/12 CR:MEDIUM:FIXED Describe how result is returned to user App
 * zb_zdo_startup_complete() - ? what parameters should be analysed? */
/**
 *  @brief Starts ZLL device.
 *  @returns Startup sequence schedule status (see @ref zb_ret_t):
 *  @li RET_OK on success;
 *  @li RET_OVERFLOW on callback ring buffer overflow.
 *
 *  On process start device invoke one or more times user fuction zb_zdo_startup_complete
 *  with zb_zll_transaction_task_status_t as buffer:
 *  @code
 *  zb_buf_t* buffer = ZB_BUF_FROM_REF(param);
 *  zb_zll_transaction_task_status_t* task_status = ZB_GET_BUF_PARAM(buffer, zb_zll_transaction_task_status_t);
 *  TRACE_MSG(TRACE_ZLL3, "> zb_zdo_startup_complete %hd status %hd", (FMT__H_H, param, task_status->task));
 *  @endcode
 *  task_status->task - see @ref zb_zll_transaction_task_e
 *  task_status->status - see @ref zb_zll_task_status_e
 */
zb_ret_t zb_zll_dev_start(zb_void_t);

/**
 *  @brief Finishes startup sequence.
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" to put task status notification to.
 */
zb_void_t zll_continue_start(zb_uint8_t param);

/**
 *  @brief Changes channel.
 *  @param buf_idx [IN] - reference to the @ref zb_buf_t "buffer" to put request to.
 *  @param channel [IN] - channel number to set.
 *  @param callback [IN] - @ref zb_callback_t "callback" to be scheduled on operation finish.
 */
#define ZLL_SET_CHANNEL(buf_idx, channel, callback)                                 \
{                                                                                   \
  nwk_txstat_clear();                                                               \
  ZB_PIBCACHE_CURRENT_CHANNEL() = (channel);                                        \
  zb_nwk_pib_set((buf_idx), ZB_PHY_PIB_CURRENT_CHANNEL, &(channel), 1, (callback)); \
}


/**
 *  @brief Finishes NLME-DIRECT-JOIN.confirm handling for start new network procedure.
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" containing operation status
 *  infotmation.
 */
zb_void_t zll_direct_join_confirm(zb_uint8_t param);


/**
 *  @brief Main loop of process commissioning command
 *  @param param [IN] - reference to the @ref zb_buf_t "buffer" containing parameters of
 *  commissioning command @ref zb_zcl_parsed_hdr_s
 */
zb_void_t zll_process_device_command(zb_uint8_t param);

/**
 * @}
 */

#endif /* ! defined ZB_ZLL_COMMON_H */
