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
 * PURPOSE: Color control cluster definitions
*/

#if !defined ZB_ZCL_COLOR_CONTROL_H
#define ZB_ZCL_COLOR_CONTROL_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"

/** @cond DOXYGEN_ZCL_SECTION */

/** @addtogroup ZB_ZCL_COLOR_CONTROL
    @{
*/

/* ZB_ZCL_CLUSTER_ID_COLOR_CONTROL = 0x0300 defined in zb_zcl_common.h */

/*! @name Color Control cluster attributes
    @{
*/

/** @brief Color Control cluster information attribute identifiers
    @see ZCL spec, subclause 5.2.2.2.3 & ZLL spec 6.8.1.1
*/

/**
 * @brief Color control attribute list
 */
enum zb_zcl_color_control_attr_e
{
  /*! @internal Current_HUE attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID = 0x0000,
  /*! @internal Current Saturation attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID = 0x0001,
  /*! @internal Remaining Time attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID = 0x0002,
  /*! @internal CurrentX attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID = 0x0003,
  /*! @internal CurrentY attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID = 0x0004,
  /*! @internal Color Temperature attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID = 0x0007,
  /*! @internal Color Mode attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID = 0x0008,
  /*! @internal Number Of Primaries attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID = 0x0010,
  /*! @internal Primary 1 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_X_ID = 0x0011,
  /*! @internal Primary 1 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_Y_ID = 0x0012,
  /*! @internal Primary 1 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_INTENSITY_ID = 0x0013,
  /*! @internal Primary 2 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_X_ID = 0x0015,
  /*! @internal Primary 2 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_Y_ID = 0x0016,
  /*! @internal Primary 2 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_INTENSITY_ID = 0x0017,
  /*! @internal Primary 3 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_X_ID = 0x0019,
  /*! @internal Primary 3 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_Y_ID = 0x001a,
  /*! @internal Primary 3 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_INTENSITY_ID = 0x001b,
  /*! @internal Primary 4 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_X_ID = 0x0020,
  /*! @internal Primary 4 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_Y_ID = 0x0021,
  /*! @internal Primary 4 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_INTENSITY_ID = 0x0022,
  /*! @internal Primary 5 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_X_ID = 0x0024,
  /*! @internal Primary 5 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_Y_ID = 0x0025,
  /*! @internal Primary 5 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_INTENSITY_ID = 0x0026,
  /*! @internal Primary 6 X attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_X_ID = 0x0028,
  /*! @internal Primary 6 Y attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_Y_ID = 0x0029,
  /*! @internal Primary 6 Intensity attribute */
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_INTENSITY_ID = 0x002a,
};

/** @brief Current Hue attribute minimum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MIN_VALUE          0

/** @brief Current Hue attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE          0xfe

/**
 * @brief Color control Hue value
 */
enum zb_zcl_color_control_hue_e
{
/** @brief Current Hue attribute red value */
  ZB_ZCL_COLOR_CONTROL_HUE_RED  = 0,

/** @brief Current Hue attribute green value */
  ZB_ZCL_COLOR_CONTROL_HUE_GREEN = 85 /*254 / 3*/,

/** @brief Current Hue attribute blue value */
  ZB_ZCL_COLOR_CONTROL_HUE_BLUE =  169/*2*254 / 3*/
};

/** @brief Current Saturation attribute minimum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MIN_VALUE   0

/** @brief Current Saturation attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE   0xfe

/** @brief Remaining Time attribute minimum value */
#define ZB_ZCL_COLOR_CONTROL_REMAINING_TIME_MIN_VALUE       0

/** @brief Remaining Time attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_REMAINING_TIME_MAX_VALUE       0xfffe



/** @brief Current X attribute default value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_X_DEF_VALUE            0x616b

/** @brief Current X attribute minimum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_X_MIN_VALUE            0

/** @brief Current X attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE            0xfeff

/** @brief Current Y attribute default value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_Y_DEF_VALUE            0x607d

/** @brief Current Y attribute minimum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MIN_VALUE            0

/** @brief Current Y attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE            0xfeff

/** @brief Current Hue attribute red value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_X_RED    (zb_uint16_t)(0.735484*ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE)

/** @brief Current Hue attribute green value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_X_GREEN  (zb_uint16_t)(0.0138702*ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE)

/** @brief Current Hue attribute blue value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_X_BLUE   (zb_uint16_t)(0.0995467*ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE)


/** @brief Current Hue attribute red value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_Y_RED   (zb_uint16_t)(0.264516*ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE)

/** @brief Current Hue attribute green value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_Y_GREEN (zb_uint16_t)(0.750186*ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE)

/** @brief Current Hue attribute blue value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_Y_BLUE  (zb_uint16_t)(0.11196*ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE)



/** @brief Color Temperature default value (4000K) */
#define ZB_ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_DEF_VALUE    0x00fa

/** @brief Color Temperature minimum value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_MIN_VALUE    0

/** @brief Color Temperature maximum value */
#define ZB_ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_MAX_VALUE    0xfeff

/*! @brief Color Mode attribute enumeration
    @see ZCL spec, subclause 5.2.2.2.1.9
*/
enum zb_zcl_color_control_color_mode_e
{
  /** CurrentHue and CurrentSaturation */
  ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION    = 0x00,
  /** CurrentX and CurrentY */
  ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y       = 0x01,
  /** ColorTemperature */
  ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE       = 0x02,
};

/** @brief Number Of Primaries attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_NUMBER_OF_PRIMARIES_MAX_VALUE    6

/** @brief Primary X attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_PRIMARY_X_MAX_VALUE            0xfeff

/** @brief Primary Y attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_PRIMARY_Y_MAX_VALUE            0xfeff

/** @brief Primary Intensity attribute maximum value */
#define ZB_ZCL_COLOR_CONTROL_PRIMARY_INTENSITY_MAX_VALUE    0xfe

/** @brief Primary Intensity attribute not used value */
#define ZB_ZCL_COLOR_CONTROL_PRIMARY_INTENSITY_NON_USED_VALUE    0xff

/*! @} */ /* Color Control cluster attributes */

/*! @name Color Control cluster internals
    Internal structures for Color Control cluster
    @internal
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                      \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID(data_ptr)    \
{                                                                                           \
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U8,                                                                      \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING | ZB_ZCL_ATTR_ACCESS_SCENE,   \
  (zb_voidp_t) data_ptr                                                                     \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID,                                      \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID(data_ptr)           \
{                                                                                         \
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,                                                 \
  ZB_ZCL_ATTR_TYPE_U16,                                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING | ZB_ZCL_ATTR_ACCESS_SCENE, \
  (zb_voidp_t) data_ptr                                                                   \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID(data_ptr)           \
{                                                                                         \
  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID,                                                 \
  ZB_ZCL_ATTR_TYPE_U16,                                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING | ZB_ZCL_ATTR_ACCESS_SCENE, \
  (zb_voidp_t) data_ptr                                                                   \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,                                   \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                      \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID(data_ptr)    \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID,                                          \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_X_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_X_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_Y_ID(data_ptr)   \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_Y_ID,                                         \
  ZB_ZCL_ATTR_TYPE_U16,                                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_INTENSITY_ID(data_ptr) \
{                                                                                   \
  ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_INTENSITY_ID,                                 \
  ZB_ZCL_ATTR_TYPE_U8,                                                              \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                     \
  (zb_voidp_t) data_ptr                                                             \
}

/** @internal Structure of Move variables for Move alarm
 */
typedef struct zb_zcl_color_control_move_loop_s
{
  /** Command header */
  zb_zcl_parsed_hdr_t cmd_info;

  /** @brief Time for last process move loop command */
  zb_time_t time_last;

  /** @brief attribute id */
  zb_uint16_t attr_id;
  /** @brief rate */
  zb_int16_t rate;
  /** @brief limit */
  zb_uint16_t limit;
  /** @brief time mod - different last time - process time*/
  zb_uint16_t time_mod;

  /** @brief attribute id2 */
  zb_uint16_t attr_id2;
  /** @brief rate2 */
  zb_int16_t rate2;
  /** @brief limit2 */
  zb_uint16_t limit2;
  /** @brief time mod - different last time - process time*/
  zb_uint16_t time_mod2;

} zb_zcl_color_control_move_loop_t;

#define ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(buf, cmd_info_,                     \
          attr_id_, rate_, limit_)                                              \
{                                                                               \
  zb_zcl_color_control_move_loop_t *loop_data =                                 \
          ZB_GET_BUF_PARAM((buf), zb_zcl_color_control_move_loop_t);            \
  ZB_MEMMOVE(&(loop_data->cmd_info), (cmd_info_), sizeof(zb_zcl_parsed_hdr_t));  \
  loop_data->time_last = ZB_TIMER_GET();                                        \
  loop_data->attr_id = (attr_id_);                                              \
  loop_data->rate = (rate_);                                                    \
  loop_data->limit = (limit_);                                                  \
  loop_data->time_mod = 0;                                                      \
  loop_data->attr_id2 = 0;                                                      \
  loop_data->time_mod2 = 0;                                                     \
}

/*! @internal
 *  @brief Structure for loop of move to command */
typedef struct zb_zcl_color_control_move_to_loop_s
{
  /** Command header */
  zb_zcl_parsed_hdr_t cmd_info;

  /** Attribute_ID */
  zb_uint16_t attr_id;
  /** Start value */
  zb_uint16_t start_value;
  /** Finish value */
  zb_uint16_t finish_value;
  /** Is increment direction */
  zb_bool_t incr_direction;

  /** Attribute_ID */
  zb_uint16_t attr_id2;
  /** Start value */
  zb_uint16_t start_value2;
  /** Finish value */
  zb_uint16_t finish_value2;

  /** Start time - beacon interval */
  zb_time_t start_time;
  /** transition time - time unit = 1/10sec */
  zb_time_t transition_time;
} zb_zcl_color_control_move_to_loop_t;

#define ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(buf, cmd_info_,                              \
            attr_id_, start_, finish_, incr_direction_, transition_time_)                   \
{                                                                                           \
  zb_zcl_color_control_move_to_loop_t *loop_data =                                          \
          ZB_GET_BUF_PARAM((buf), zb_zcl_color_control_move_to_loop_t);                     \
    ZB_MEMMOVE(&(loop_data->cmd_info), (cmd_info_), sizeof(zb_zcl_parsed_hdr_t));            \
    loop_data->attr_id = (attr_id_);                                                        \
    loop_data->start_value = start_;                                                        \
    loop_data->finish_value = finish_;                                                      \
    loop_data->attr_id2 = 0;                                                                \
    loop_data->incr_direction = (zb_bool_t)(incr_direction_);           \
    loop_data->start_time = ZB_TIMER_GET();                                                 \
    loop_data->transition_time = transition_time_;                                          \
}

/*! @internal
 *  @brief Structure for loop of step command */
typedef struct zb_zcl_color_control_step_loop_s
{
  /** Command header */
  zb_zcl_parsed_hdr_t cmd_info;

  /** Attribute_ID */
  zb_uint16_t attr_id;
  /** Rate value */
  zb_uint16_t rate;
  /** @brief limit */
  zb_uint16_t limit;

  /** last time - beacon interval */
  zb_time_t last_time;
  /** time bitween two steps - beacon interval */
  zb_time_t step_time;

  /** Attribute_ID second */
  zb_uint16_t attr_id2;
  /** Rate value second */
  zb_uint16_t rate2;
  /** @brief limit2 */
  zb_uint16_t limit2;

} zb_zcl_color_control_step_loop_t;

#define ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(buf, cmd_info_,  attr_id_, rate_,               \
                                                       transition_time, limit_)             \
{                                                                                           \
  zb_zcl_color_control_step_loop_t *loop_data =                                             \
          ZB_GET_BUF_PARAM((buf), zb_zcl_color_control_step_loop_t);                        \
    ZB_MEMMOVE(&(loop_data->cmd_info), (cmd_info_), sizeof(zb_zcl_parsed_hdr_t));            \
    loop_data->attr_id = (attr_id_);                                                        \
    loop_data->attr_id2 = 0;                                                                \
    loop_data->rate = (rate_);                                                              \
    loop_data->last_time = ZB_TIMER_GET();                                                  \
    loop_data->step_time = ZB_TIME_UNITS_TO_BEACON_INTERVAL(transition_time);               \
    loop_data->limit = (limit_);                                                            \
}

/*!
  Declare attribute list for Color Control cluster
  @param attr_list - attribure list name
  @param current_hue - pointer to variable to store current_hue attribute value
  @param current_saturation - pointer to variable to store current_saturation attribute value
  @param remaining_time - pointer to variable to store remaining_time attribute value
  @param current_X - pointer to variable to store current_X attribute value
  @param current_Y - pointer to variable to store current_Y attribute value
  @param color_temperature - pointer to variable to store color_temperature attribute value
  @param color_mode - pointer to variable to store color_mode attribute value
  @param number_primaries - pointer to variable to store number_primaries attribute value
  @param primary_1_X - pointer to variable to store primary_1_X attribute value
  @param primary_1_Y - pointer to variable to store primary_1_Y attribute value
  @param primary_1_intensity - pointer to variable to store primary_1_intensity attribute value
  @param primary_2_X - pointer to variable to store primary_2_X attribute value
  @param primary_2_Y - pointer to variable to store primary_2_Y attribute value
  @param primary_2_intensity - pointer to variable to store primary_2_intensity attribute value
  @param primary_3_X - pointer to variable to store primary_3_X attribute value
  @param primary_3_Y - pointer to variable to store primary_3_Y attribute value
  @param primary_3_intensity - pointer to variable to store primary_3_intensity attribute value
  @param primary_4_X - pointer to variable to store primary_4_X attribute value
  @param primary_4_Y - pointer to variable to store primary_4_Y attribute value
  @param primary_4_intensity - pointer to variable to store primary_4_intensity attribute value
  @param primary_5_X - pointer to variable to store primary_5_X attribute value
  @param primary_5_Y - pointer to variable to store primary_5_Y attribute value
  @param primary_5_intensity - pointer to variable to store primary_5_intensity attribute value
  @param primary_6_X - pointer to variable to store primary_6_X attribute value
  @param primary_6_Y - pointer to variable to store primary_6_Y attribute value
  @param primary_6_intensity - pointer to variable to store primary_6_intensity attribute value
*/
#define ZB_ZCL_DECLARE_COLOR_CONTROL_ATTRIB_LIST(attr_list, current_hue,                        \
    current_saturation, remaining_time, current_X, current_Y, color_temperature,                \
    color_mode, number_primaries, primary_1_X, primary_1_Y, primary_1_intensity,                \
    primary_2_X, primary_2_Y, primary_2_intensity, primary_3_X, primary_3_Y,                    \
    primary_3_intensity, primary_4_X, primary_4_Y, primary_4_intensity, primary_5_X,            \
    primary_5_Y, primary_5_intensity, primary_6_X, primary_6_Y, primary_6_intensity)            \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, (current_hue))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID, (current_saturation))   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID, (remaining_time))           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, (current_X))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID, (current_Y))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID, (color_temperature))     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID, (color_mode))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID, (number_primaries))    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_X_ID, (primary_1_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_Y_ID, (primary_1_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_1_INTENSITY_ID, (primary_1_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_X_ID, (primary_2_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_Y_ID, (primary_2_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_2_INTENSITY_ID, (primary_2_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_X_ID, (primary_3_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_Y_ID, (primary_3_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_3_INTENSITY_ID, (primary_3_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_X_ID, (primary_4_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_Y_ID, (primary_4_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_4_INTENSITY_ID, (primary_4_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_X_ID, (primary_5_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_Y_ID, (primary_5_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_5_INTENSITY_ID, (primary_5_intensity)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_X_ID, (primary_6_X))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_Y_ID, (primary_6_Y))                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_PRIMARY_6_INTENSITY_ID, (primary_6_intensity)) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*!
  Declare mandatory attribute list for Color Control cluster
  @param attr_list - attribure list name
  @param current_X - pointer to variable to store current_X attribute value
  @param current_Y - pointer to variable to store current_Y attribute value
*/
#define ZB_ZCL_DECLARE_COLOR_CONTROL_MANDATORY_ATTRIB_LIST(attr_list, current_X, current_Y)     \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, (current_X))                     \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID, (current_Y))                     \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! Number of attributes mandatory for reporting in Color Control cluster
 * Attribute: CurrentX, CurrentY, CurrentHue, CurrentSaturation, ColorTemperature
 *  */
#define ZB_ZCL_COLOR_CONTROL_REPORT_ATTR_COUNT 5

/*! @} */ /* Color Control cluster internals */

/*! @name Color Control cluster commands
    @{
*/

/************************** Color Control cluster command definitions ****************************/

/*! @brief Color control cluster command identifiers
    @see ZCL spec, subclause 5.2.2.3
*/
enum zb_zcl_color_control_cmd_e
{
  /** Move To Hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE               = 0x00,
  /** Move Hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE                  = 0x01,
  /** Step Hue command */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE                  = 0x02,
  /** Move To Saturation command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION        = 0x03,
  /** Move Saturation command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION           = 0x04,
  /** Step Saturation command */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION           = 0x05,
  /** Move To Hue and Saturation command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION    = 0x06,
  /** Move To Color command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR             = 0x07,
  /** Move Color command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR                = 0x08,
  /** Step Color command */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR                = 0x09,
  /** Move to color temperature command */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE = 0x0a
};

/* command parameters enumerates */

/** @brief Direction of Move Hue command.
 *  See specs 5.2.2.3.3.2, 5.2.2.3.6.2 */
enum zb_zcl_color_control_move_direction_e
{
  /** Stop */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_STOP    = 0x00,
  /** Up */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP      = 0x01,
  /** Down */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_DOWN     = 0x03
};

/** @brief Direction of Step command.
 *  See specs 5.2.2.3.4.2, 5.2.2.3.7.2 */
enum zb_zcl_color_control_step_mode_e
{
  /** Up */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP       = 0x01,
  /** Down */
  ZB_ZCL_CMD_COLOR_CONTROL_STEP_DOWN     = 0x03
};


/* command request structure */

/******************************* MOVE_TO_HUE ******************************/

/** @brief Direction of Move to Hue command.
 *  See spec 5.2.2.3.2.3 */
enum zb_zcl_color_control_move_to_hue_direction_e
{
  /** Shortest distance */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SHORTEST = 0x00,
  /** Longest distance */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_LONGEST  = 0x01,
  /** Up */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_UP       = 0x02,
  /** Down */
  ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_DOWN     = 0x03
};

/*! @brief Structure representsation of Move To Hue command payload
    @see ZCL spec, subclause 5.2.2.3.2 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_to_hue_req_s
{
  /** Hue */
  zb_uint8_t hue;
  /** Direction, see @ref zb_zcl_color_control_move_to_hue_direction_e */
  zb_uint8_t direction;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_to_hue_req_t;

/*! @brief Send Move to Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param hue - Hue value
    @param direction - Direction value, see @ref zb_zcl_color_control_move_to_hue_direction_e
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_REQ(buffer, addr, dst_addr_mode,                      \
  dst_ep, ep, prfl_id, def_resp, cb, hue, direction, transition_time)                               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE); \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));                                                              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move to Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_to_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_REQ(data_ptr, buffer, status)          \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_to_hue_req_t))       \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_move_to_hue_req_t *src_ptr =                               \
                   (zb_zcl_color_control_move_to_hue_req_t*)ZB_BUF_BEGIN((buffer)); \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_move_to_hue_req_t)); \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));        \
  }                                                                                 \
}

/******************************* MOVE_HUE ******************************/

/*! @brief Structure representsation of Move Hue command payload
    @see ZCL spec, subclause 5.2.2.3.3 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_hue_req_s
{
  /** Move mode, see @ref zb_zcl_color_control_move_direction_e */
  zb_uint8_t move_mode;
  /** Rate */
  zb_uint8_t rate;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_hue_req_t;

/*! @brief Send Move Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param move_mode - Move mode, see @ref zb_zcl_color_control_move_direction_e
    @param rate - Rate
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_HUE_REQ(buffer, addr, dst_addr_mode,                         \
  dst_ep, ep, prfl_id, def_resp, cb, move_mode, rate)                                               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));                                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_HUE_REQ(data_ptr, buffer, status)             \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_hue_req_t))          \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_move_hue_req_t *src_ptr =                                  \
                   (zb_zcl_color_control_move_hue_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_move_hue_req_t));    \
  }                                                                                 \
}

/******************************* STEP_HUE ******************************/

/*! @brief Structure representsation of Step Hue command payload
    @see ZCL spec, subclause 5.2.2.3.4 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_step_hue_req_s
{
  /** Step mode, see @ref zb_zcl_color_control_step_mode_e */
  zb_uint8_t step_mode;
  /** Step size */
  zb_uint8_t step_size;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_step_hue_req_t;

/*! @brief Send Step Hue command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param step_mode - step mode value, see @ref zb_zcl_color_control_step_mode_e
    @param step_size - step size value
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_STEP_HUE_REQ(buffer, addr, dst_addr_mode,                         \
  dst_ep, ep, prfl_id, def_resp, cb, step_mode, step_size, transition_time)                         \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Step Hue command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_step_hue_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_STEP_HUE_REQ(data_ptr, buffer, status)             \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_step_hue_req_t))          \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_step_hue_req_t *src_ptr =                                  \
                   (zb_zcl_color_control_step_hue_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_step_hue_req_t));    \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));        \
  }                                                                                 \
}

/******************************* Move to Saturation ******************************/

/*! @brief Structure representsation of Move To Saturation command payload
    @see ZCL spec, subclause 5.2.2.3.5 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_to_saturation_req_s
{
  /** Saturation */
  zb_uint8_t saturation;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_to_saturation_req_t;

/*! @brief Send Move to Saturation command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param saturation - saturation
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_SATURATION_REQ(buffer, addr, dst_addr_mode,               \
  dst_ep, ep, prfl_id, def_resp, cb, saturation, transition_time)                                   \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move To Saturation command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_to_saturation_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_SATURATION_REQ(data_ptr, buffer, status)   \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_to_saturation_req_t))    \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_move_to_saturation_req_t *src_ptr =                        \
         (zb_zcl_color_control_move_to_saturation_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    (data_ptr)->saturation = src_ptr->saturation;                                     \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));          \
  }                                                                                 \
}

/******************************* Move Saturation ******************************/

/*! @brief Structure representsation of Move Saturation command payload
    @see ZCL spec, subclause 5.2.2.3.6 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_saturation_req_s
{
  /** Move mode, see @ref zb_zcl_color_control_move_direction_e */
  zb_uint8_t move_mode;
  /** Rate */
  zb_uint8_t rate;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_saturation_req_t;

/*! @brief Send Move Saturation command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param move_mode - Move mode, see @ref zb_zcl_color_control_move_direction_e
    @param rate - Rate
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_SATURATION_REQ(buffer, addr, dst_addr_mode,                  \
  dst_ep, ep, prfl_id, def_resp, cb, move_mode, rate)                                               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));                                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move Saturation command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_saturation_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_SATURATION_REQ(data_ptr, buffer, status)      \
{                                                                                   \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_saturation_req_t))   \
  {                                                                                 \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                          \
  }                                                                                 \
  else                                                                              \
  {                                                                                 \
    zb_zcl_color_control_move_saturation_req_t *src_ptr =                           \
                   (zb_zcl_color_control_move_saturation_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                         \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_move_saturation_req_t));    \
  }                                                                                 \
}

/******************************* STEP_SATURATION ******************************/

/*! @brief Structure representsation of Step Saturation command payload
    @see ZCL spec, subclause 5.2.2.3.7 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_step_saturation_req_s
{
  /** Step mode, see @ref zb_zcl_color_control_step_mode_e */
  zb_uint8_t step_mode;
  /** Step size */
  zb_uint8_t step_size;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_step_saturation_req_t;

/*! @brief Send Step Saturation command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param step_mode - step mode value, see @ref zb_zcl_color_control_step_mode_e
    @param step_size - step size value
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_STEP_SATURATION_REQ(buffer, addr, dst_addr_mode,                  \
  dst_ep, ep, prfl_id, def_resp, cb, step_mode, step_size, transition_time)                         \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                                                        \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));                                                        \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Step Saturation command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_step_saturation_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_STEP_SATURATION_REQ(data_ptr, buffer, status)          \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_step_saturation_req_t))       \
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_color_control_step_saturation_req_t *src_ptr =                               \
            (zb_zcl_color_control_step_saturation_req_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    ZB_MEMCPY((data_ptr), src_ptr, sizeof(zb_zcl_color_control_step_saturation_req_t)); \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));            \
  }                                                                                     \
}

/******************************* Move to Hue and Saturation ******************************/

/*! @brief Structure representsation of Move To Hue and Saturation command payload
    @see ZCL spec, subclause 5.2.2.3.8 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_to_hue_saturation_req_s
{
  /** Hue */
  zb_uint8_t hue;
  /** Saturation */
  zb_uint8_t saturation;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_to_hue_saturation_req_t;

/*! @brief Send Move to Hue and Saturation command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param hue - hue
    @param saturation - saturation
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ(buffer, addr, dst_addr_mode,           \
  dst_ep, ep, prfl_id, def_resp, cb, hue, saturation, transition_time)                              \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));                                                              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move To Hue and Saturation command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_to_hue_saturation_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_SATURATION_REQ(data_ptr, buffer, status)   \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_to_hue_saturation_req_t))\
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_color_control_move_to_hue_saturation_req_t *src_ptr =                        \
         (zb_zcl_color_control_move_to_hue_saturation_req_t*)ZB_BUF_BEGIN((buffer));    \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    (data_ptr)->hue = src_ptr->hue;                                                     \
    (data_ptr)->saturation = src_ptr->saturation;                                       \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));            \
  }                                                                                     \
}

/******************************* Move to Color ******************************/

/*! @brief Structure representsation of Move To Color command payload
    @see ZCL spec, subclause 5.2.2.3.9 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_to_color_req_s
{
  /** ColorX */
  zb_uint16_t color_x;
  /** ColorY */
  zb_uint16_t color_y;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_to_color_req_t;

/*! @brief Send Move to Color command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param color_x - colorX
    @param color_y - colorY
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_COLOR_REQ(buffer, addr, dst_addr_mode,                    \
  dst_ep, ep, prfl_id, def_resp, cb, color_x, color_y, transition_time)                             \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR);   \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_x));                                                     \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_y));                                                     \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move To Color command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_to_color_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_REQ(data_ptr, buffer, status)            \
{                                                                                       \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_to_color_req_t))         \
  {                                                                                     \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                              \
  }                                                                                     \
  else                                                                                  \
  {                                                                                     \
    zb_zcl_color_control_move_to_color_req_t *src_ptr =                                 \
         (zb_zcl_color_control_move_to_color_req_t*)ZB_BUF_BEGIN((buffer));             \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                             \
    ZB_HTOLE16(&((data_ptr)->color_x), &(src_ptr->color_x));                            \
    ZB_HTOLE16(&((data_ptr)->color_y), &(src_ptr->color_y));                            \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));            \
  }                                                                                     \
}

/******************************* Move Color ******************************/

/*! @brief Structure representsation of Move Color command payload
    @see ZCL spec, subclause 5.2.2.3.10 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_color_req_s
{
  /** RateX */
  zb_int16_t rate_x;
  /** RateY */
  zb_int16_t rate_y;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_color_req_t;

/*! @brief Send Move Color command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param rate_x - RateX
    @param rate_y - RateY
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_COLOR_REQ(buffer, addr, dst_addr_mode,                       \
    dst_ep, ep, prfl_id, def_resp, cb, rate_x, rate_y)                                              \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR);  \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_x));                                                      \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_y));                                                      \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Move Color command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_color_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_REQ(data_ptr, buffer, status)       \
{                                                                               \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_color_req_t))    \
  {                                                                             \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                      \
  }                                                                             \
  else                                                                          \
  {                                                                             \
    zb_zcl_color_control_move_color_req_t *src_ptr =                            \
         (zb_zcl_color_control_move_color_req_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                     \
    ZB_HTOLE16(&((data_ptr)->rate_x), &(src_ptr->rate_x));                      \
    ZB_HTOLE16(&((data_ptr)->rate_y), &(src_ptr->rate_y));                      \
  }                                                                             \
}

/******************************* Step Color ******************************/

/*! @brief Structure representsation of Step Color command payload
    @see ZCL spec, subclause 5.2.2.3.11 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_step_color_req_s
{
  /** StepX */
  zb_uint16_t step_x;
  /** StepY */
  zb_uint16_t step_y;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_step_color_req_t;

/*! @brief Send Step Color command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param step_x - StepX
    @param step_y - StepY
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_STEP_COLOR_REQ(buffer, addr, dst_addr_mode,                       \
  dst_ep, ep, prfl_id, def_resp, cb, step_x, step_y, transition_time)                               \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR);  \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_x));                                                      \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_y));                                                      \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Step Color command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_step_color_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_REQ(data_ptr, buffer, status)       \
{                                                                               \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_step_color_req_t))    \
  {                                                                             \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                      \
  }                                                                             \
  else                                                                          \
  {                                                                             \
    zb_zcl_color_control_step_color_req_t *src_ptr =                            \
         (zb_zcl_color_control_step_color_req_t*)ZB_BUF_BEGIN((buffer));        \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                     \
    ZB_HTOLE16(&((data_ptr)->step_x), &(src_ptr->step_x));                      \
    ZB_HTOLE16(&((data_ptr)->step_y), &(src_ptr->step_y));                      \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));    \
  }                                                                             \
}

/******************************* Move to Color Temperature ******************************/

/*! @brief Structure representsation of Move To Color Temperature command payload
    @see ZCL spec, subclause 5.2.2.3.12 */
typedef ZB_PACKED_PRE struct zb_zcl_color_control_move_to_color_temperature_req_s
{
  /** Color Temperature */
  zb_uint16_t color_temperature;
  /** Transition time field */
  zb_uint16_t transition_time;
} ZB_PACKED_STRUCT zb_zcl_color_control_move_to_color_temperature_req_t;

/*! @brief Send Move To Color Temperature command
    @param buffer - to put packet to
    @param addr - address to send packet to
    @param dst_addr_mode - addressing mode
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prfl_id - profile identifier
    @param def_resp - enable/disable default response
    @param cb - callback for getting command send status
    @param color_temperature - Color Temperature
    @param transition_time - Transition Time value
*/
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_COLOR_TEMPERATURE_REQ(buffer, addr, dst_addr_mode,        \
  dst_ep, ep, prfl_id, def_resp, cb, color_temperature, transition_time)                            \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                    \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                             \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE);  \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temperature));                                           \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_FINISH_PACKET((buffer), ptr)                                                              \
  ZB_ZCL_SEND_COMMAND_SHORT(                                                                        \
   buffer, addr, dst_addr_mode, dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);          \
}

/** @brief Macro for getting Step Color Temperature command
  * @attention Assumes that ZCL header already cut.
  * @param data_ptr - pointer to a variable of type @ref
  * zb_zcl_color_control_move_to_color_temperature_req_s.
  * @param buffer containing the packet (by pointer).
  * @param status - variable to put parse status to (see @ref zb_zcl_parse_status_t).
  */
#define ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_TEMPERATURE_REQ(data_ptr, buffer, status)    \
{                                                                                           \
  if (ZB_BUF_LEN((buffer)) != sizeof(zb_zcl_color_control_move_to_color_temperature_req_t)) \
  {                                                                                         \
   (status) = ZB_ZCL_PARSE_STATUS_FAILURE;                                                  \
  }                                                                                         \
  else                                                                                      \
  {                                                                                         \
    zb_zcl_color_control_move_to_color_temperature_req_t *src_ptr =                         \
         (zb_zcl_color_control_move_to_color_temperature_req_t*)ZB_BUF_BEGIN((buffer));     \
    (status) = ZB_ZCL_PARSE_STATUS_SUCCESS;                                                 \
    ZB_HTOLE16(&((data_ptr)->color_temperature), &(src_ptr->color_temperature));            \
    ZB_HTOLE16(&((data_ptr)->transition_time), &(src_ptr->transition_time));                \
  }                                                                                         \
}


/*! @} */ /* Color Control cluster commands */

/** @cond internals_doc
 *
    @name Added struct and define for specific Color Control cluster commands
    @{
*/

#define ZB_ZCL_COLOR_CONTROL_SEC_TO_MSEC        1000

/** Standard Color Control time uint = 1/10 sec */
#define ZB_ZCL_COLOR_CONTROL_TIMER_INTERVAL    100

/** Standard Color Control time uint = 1/10 sec, beacon */
#define ZB_ZCL_COLOR_CONTROL_TIMER_BEACON_INTERVAL    \
    ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_ZCL_COLOR_CONTROL_TIMER_INTERVAL)

/**
 * @brief Struct for process one interation of move command for one attribute
 */
typedef struct zb_zcl_color_control_loop_element_s
{
  zb_uint8_t endpoint;      /** Device endpoint */
  zb_uint8_t buffer_id;     /** buffer for invoke User App */

  zb_uint16_t attr_id;      /** Attribute ID */
  zb_int16_t value;         /** delta value */
  zb_bool_t is_continue;   /** If ZB_TRUE attribute when is already limit value, command
                                continious from another limit. If ZB_FALSE attribute when
                                is already limit value, command stop */
  zb_uint16_t limit;        /** limit of attribute value */

} zb_zcl_color_control_loop_element_t;

/**
 * @brief Define for fill struct process step command for one attribute,
 * see @ref zb_zcl_color_control_loop_element_s
 *
 * @param el_data - struct see @ref zb_zcl_color_control_loop_element_s 'zb_zcl_color_control_loop_element_t'
 * @param endp - endpoint
 * @param param - buffer for invoke User App
 * @param attr - attribute ID
 * @param value_ - value
 * @param is_continue_ - continue if limit dest
 * @param limit_ - limit of attribute value, min or max - depend of sing of rate field
 */
#define ZB_ZCL_COLOR_CONTROL_FILL_LOOP_ELEMENT(el_data, endp,   \
               param, attr, value_, is_continue_, limit_)       \
  (el_data).endpoint = (endp);                                  \
  (el_data).buffer_id = (param);                                \
  (el_data).attr_id = (attr);                                   \
  (el_data).value = (value_);                                   \
  (el_data).is_continue = (is_continue_);                       \
  (el_data).limit = (limit_);


/**
 * @brief Copy 16bit attribute value for Color Control cluster
 *
 * @param endpoint - device endpoint
 * @param attr_id_to - destination attribute ID
 * @param attr_id_from - source attribute ID
 */
#define ZB_ZLL_COLOR_CONTROL_COPY_ATTRIBUTE_16(endpoint, attr_id_to, attr_id_from)      \
{                                                                                       \
  zb_uint16_t value;                                                                    \
  zb_zcl_attr_t * attr_desc = zb_zcl_get_attr_desc_a((endpoint),                        \
      ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, (attr_id_from));                                 \
  ZB_ASSERT(attr_desc);                                                                 \
  value = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);                                       \
                                                                                        \
  attr_desc = zb_zcl_get_attr_desc_a((endpoint),                                        \
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, (attr_id_to));                                 \
  ZB_ASSERT(attr_desc);                                                                 \
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, value);                                     \
}

/**
 * @brief Struct invoke User App and set attribute value
 */
typedef struct zb_zcl_color_control_invoke_user_s
{
  zb_uint8_t endpoint;
  zb_uint16_t attr_id;
  zb_uint16_t new_value;        /** New value attribute */
} zb_zcl_color_control_invoke_user_t;

/** @}
 *
 * @endcond */ /* internals_doc */


/*! @} */ /* ZCL Color Control cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_color_control_init_server();
zb_void_t zb_zcl_color_control_init_client();

#define ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SERVER_ROLE_INIT zb_zcl_color_control_init_server
#define ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_CLIENT_ROLE_INIT zb_zcl_color_control_init_client

#endif /* ZB_ZCL_COLOR_CONTROL_H */
