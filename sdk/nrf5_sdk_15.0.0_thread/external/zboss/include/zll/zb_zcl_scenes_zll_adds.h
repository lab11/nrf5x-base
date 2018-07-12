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
 * PURPOSE: ZLL additions to ZCL Scenes cluster
*/

#if ! defined ZB_ZCL_SCENES_ZLL_ADDS_H
#define ZB_ZCL_SCENES_ZLL_ADDS_H

#include "zcl/zb_zcl_scenes.h"

/**
 *  @defgroup ZB_ZLL_SCENE_ADDS ZLL additions to Scene cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_SCENE_ADDS
 *  ZLL additions to Scene cluster.
 *  @{
 */

/* TODO implement */

/**
 *  @name ZLL Scene cluster additions extensions.
 *  Internal structures for attribute representation in cluster additions.
 *  @internal
 *  @{
 */

/**
 *  @brief Declare record for Scene table with ZLL additions,
 *  see @ref ZB_ZCL_DECLARE_DEVICE_SCENE_TABLE_RECORD_TYPE.
 *  @param type_name - type name.
 *  @param type_length - type length.
 */
#define ZB_ZCL_DECLARE_DEVICE_SCENE_TABLE_RECORD_TYPE_ZLL(type_name, type_length)  \
  ZB_ZCL_DECLARE_DEVICE_SCENE_TABLE_RECORD_TYPE(type_name, type_length)

/**
 *  @brief Declare Scene table with ZLL additions, see @ref ZB_ZCL_DEFINE_DEVICE_SCENE_TABLE.
 *  @param type_name - type name.
 *  @param table_name - table name.
 */
#define ZB_ZCL_DEFINE_DEVICE_SCENE_TABLE_ZLL(type_name, table_name)  \
  ZB_ZCL_DEFINE_DEVICE_SCENE_TABLE(type_name, table_name)

/**
 * @}
 */

/**
 * @}
 */
#endif /* ! defined ZB_ZCL_SCENES_ZLL_ADDS_H */
