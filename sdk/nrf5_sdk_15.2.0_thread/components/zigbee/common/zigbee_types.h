/**
 * Copyright (c) 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef ZIGBEE_TYPES_H__
#define ZIGBEE_TYPES_H__

#include <stdint.h>
#include "zboss_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Basic cluster attributes according to ZCL Spec 3.2.2.2 */
typedef struct
{
    zb_uint8_t zcl_version;
    zb_uint8_t app_version;
    zb_uint8_t stack_version;
    zb_uint8_t hw_version;
    zb_char_t  mf_name[32];
    zb_char_t  model_id[32];
    zb_char_t  date_code[16];
    zb_uint8_t power_source;
    zb_char_t  location_id[15];
    zb_uint8_t ph_env;
} zb_dev_basic_attr_t;

/* Identify cluster attributes according to ZCL Spec 3.5.2.2 */
typedef struct
{
    zb_uint16_t identify_time;
    zb_uint8_t  commission_state;
} zb_dev_identify_attr_t;

/* ON/Off cluster attributes according to ZCL Spec 3.8.2.2 */
typedef struct
{
    zb_bool_t   on_off;
    zb_bool_t   global_scene_ctrl;
    zb_uint16_t on_time;
    zb_uint16_t off_wait_time;
} zb_dev_on_off_attr_t;

/* Level Control cluster attributes according to ZCL Spec 3.10.2.3 */
typedef struct
{
    zb_uint8_t  current_level;
    zb_uint16_t remaining_time;
} zb_dev_level_control_attr_t;

/* Scenes cluster attributes according to ZCL Spec 3.7.2.2 */
typedef struct
{
    zb_uint8_t  scene_count;
    zb_uint8_t  current_scene;
    zb_uint8_t  scene_valid;
    zb_uint8_t  name_support;
    zb_uint16_t current_group;
} zb_dev_scenes_attr_t;

/* Groups cluster attributes according to ZCL Spec 3.6.2.2 */
typedef struct
{
    zb_uint8_t name_support;
} zb_dev_groups_attr_t;

/* Color information set for Color cluster attributes according to ZCL Spec 5.2.2.2.1 */
typedef struct
{
    zb_uint8_t     current_hue;
    zb_uint8_t     current_saturation;
    zb_uint16_t    remaining_time;
    zb_uint16_t    current_X;
    zb_uint16_t    current_Y;
    zb_uint16_t    color_temperature;
    zb_uint8_t     color_mode;
    zb_uint8_t     options;
    zb_uint16_t    enhanced_current_hue;
    zb_uint8_t     enhanced_color_mode;
    zb_uint8_t     color_loop_active;
    zb_uint8_t     color_loop_direction;
    zb_uint16_t    color_loop_time;
    zb_uint16_t    color_loop_start_enhanced_hue;
    zb_uint16_t    color_loop_stored_enhanced_hue;
    zb_uint16_t    color_capabilities;
    zb_uint16_t    color_temp_physical_min_mireds;
    zb_uint16_t    color_temp_physical_max_mireds;
    zb_uint16_t    couple_color_temp_to_level_min_mireds;
    zb_uint16_t    start_up_color_temp_mireds;
} zb_dev_color_ctrl_atrr_set_color_inf_t;

/* Defined primaries information attribute set for Color cluster attributes according to ZCL Spec 5.2.2.2.2 */
typedef struct
{
    zb_uint8_t      number_primaries;
    zb_uint16_t     primary_1_X;
    zb_uint16_t     primary_1_Y;
    zb_uint8_t      primary_1_intensity;
    zb_uint16_t     primary_2_X;
    zb_uint16_t     primary_2_Y;
    zb_uint8_t      primary_2_intensity;
    zb_uint16_t     primary_3_X;
    zb_uint16_t     primary_3_Y;
    zb_uint8_t      primary_3_intensity;
} zb_dev_color_ctrl_atrr_set_defined_primaries_inf_t;

/*  for Color cluster attributes according to ZCL Spec 5.2.2.2.3 */
typedef struct
{
    zb_uint16_t     primary_4_X;
    zb_uint16_t     primary_4_Y;
    zb_uint8_t      primary_4_intensity;
    zb_uint16_t     primary_5_X;
    zb_uint16_t     primary_5_Y;
    zb_uint8_t      primary_5_intensity;
    zb_uint16_t     primary_6_X;
    zb_uint16_t     primary_6_Y;
    zb_uint8_t      primary_6_intensity;
} zb_dev_color_ctrl_atrr_set_add_prim_inf_t;

/* Color cluster attributes according to ZCL Spec 5.2.2.2 */
typedef struct
{
    zb_dev_color_ctrl_atrr_set_color_inf_t              set_color_info;
    zb_dev_color_ctrl_atrr_set_defined_primaries_inf_t  set_defined_primaries_info;
    zb_dev_color_ctrl_atrr_set_add_prim_inf_t           set_additional_defined_primaries_info;
} zb_dev_color_control_attr_t;

#ifdef __cplusplus
}
#endif

#endif // ZIGBEE_TYPES_H__
