/**
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#ifndef SDK_NRF5_COMPONENTS_ZIGBEE_COMMON_ZIGBEE_LOGGER_EPRXZCL_H_
#define SDK_NRF5_COMPONENTS_ZIGBEE_COMMON_ZIGBEE_LOGGER_EPRXZCL_H_

#include "zboss_api.h"

/**@brief Handler function which may be called to log an incoming frame onto zigbee endpoint.
 *
 * @details When this function is called as a callback bound to endpoint via @ref ZB_AF_SET_ENDPOINT_HANDLER,
 * (directly or indirectly) it produces a log line similar to the following:
 * @code
 * Received ZCL command (17): src_addr=0x0000(short) src_ep=64 dst_ep=64 cluster_id=0x0000
 * profile_id=0x0104 rssi=0 cmd_dir=0 common_cmd=1 cmd_id=0x00 cmd_seq=128
 * disable_def_resp=0 manuf_code=void payload=[0700] (17)
 * @endcode
 *
 * @param  param     Reference to zigbee buffer holding received zcl command to be logged
 *
 * @retval ZB_FALSE  in all conditions. This enables possibility to use this function directly
 *                   as zigbee stack endpoint handler.
 *
 */
zb_uint8_t zigbee_logger_eprxzcl_ep_handler(zb_uint8_t param);


#endif /* SDK_NRF5_COMPONENTS_ZIGBEE_COMMON_ZIGBEE_LOGGER_EPRXZCL_H_ */
