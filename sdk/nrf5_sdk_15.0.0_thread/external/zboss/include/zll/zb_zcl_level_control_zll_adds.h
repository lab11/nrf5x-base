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
 * PURPOSE: ZLL additions to ZCL Level control cluster
*/

#if ! defined ZB_ZCL_LEVEL_CONTROL_ZLL_ADDS_H
#define ZB_ZCL_LEVEL_CONTROL_ZLL_ADDS_H

#include "zcl/zb_zcl_level_control.h"

/**
 *  @defgroup ZB_ZLL_LEVEL_CONTROL_ADDS ZLL additions to Level control cluster.
 *  @ingroup ZB_ZLL_CLUSTERS_ADDS
 *  @addtogroup ZB_ZLL_LEVEL_CONTROL_ADDS
 *  ZLL additions to Level control cluster.
 *  @{
 */

/* TODO implement */

/**
 *  @name ZLL Level control cluster additions extensions.
 *  Internal structures for attribute representation in cluster additions.
 *  @internal
 *  @{
 */

/**
 *  @brief Declare attribute list for Level control cluster with ZLL additions. Each attribute type corresponds
 *  to attribute description in ZCL specification (Zigbee Cluster Library Specification, chapter 3.10 Level Control Cluster)
 *  @param attr_list [IN] - attribure list name being declared by this macro.
 *  @param current_level [IN] - pointer to a unsigned 8-bit integer variable current_level attribute value.
 *  @param remaining_time [IN] - pointer to a unsigned 16-bit integer variable remaining time attribute value.
 */
#define ZB_ZCL_DECLARE_LEVEL_CONTROL_ATTRIB_LIST_ZLL(attr_list, current_level, remaining_time) \
    ZB_ZCL_DECLARE_LEVEL_CONTROL_ATTRIB_LIST(attr_list, current_level, remaining_time)
/**
 * @}
 */

/**
 * @}
 */
#endif /* ! defined ZB_ZCL_LEVEL_CONTROL_ZLL_ADDS_H */
