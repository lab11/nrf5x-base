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

#ifndef SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_LEVEL_CONTROL_ADDONS_H_
#define SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_LEVEL_CONTROL_ADDONS_H_

#include "sdk_config.h"
#include "zboss_api.h"

/*! \addtogroup zb_zcl_level_control_addons */
/*! @{ */

/**@brief Level Control cluster attributes according to ZCL Spec 3.10.2.3. */
typedef struct
{
    zb_uint8_t  current_level;
    zb_uint16_t remaining_time;
} zb_zcl_level_control_attrs_t;

/**@brief Macro equivalent to @ref ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD, but command is sent without APS ACK. */
#define ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD_NO_APS_ACK(buffer,           \
                                           addr,                        \
                                           dst_addr_mode,               \
                                           dst_ep,                      \
                                           ep,                          \
                                           prfl_id,                     \
                                           def_resp,                    \
                                           cb,                          \
                                           step_mode,                   \
                                           step_size,                   \
                                           transition_time,             \
                                           cmd_id)                      \
{                                                                       \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                     \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)    \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                            \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));                            \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                 \
  ZB_ZCL_FINISH_PACKET_NO_APS_ACK(buffer, ptr)                          \
  ZB_ZCL_SEND_COMMAND_SHORT(buffer,                                     \
                            addr,                                       \
                            dst_addr_mode,                              \
                            dst_ep,                                     \
                            ep,                                         \
                            prfl_id,                                    \
                            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,            \
                            cb);                                        \
}

/**@brief Macro equivalent to @ref ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ, but command is sent without APS ACK. */
#define ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ_NO_APS_ACK(buffer,        \
                                           addr,                     \
                                           dst_addr_mode,            \
                                           dst_ep,                   \
                                           ep,                       \
                                           prfl_id,                  \
                                           def_resp,                 \
                                           cb,                       \
                                           step_mode,                \
                                           step_size,                \
                                           transition_time)          \
{                                                                    \
  ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD_NO_APS_ACK(buffer,              \
                                     addr,                           \
                                     dst_addr_mode,                  \
                                     dst_ep,                         \
                                     ep,                             \
                                     prfl_id,                        \
                                     def_resp,                       \
                                     cb,                             \
                                     step_mode,                      \
                                     step_size,                      \
                                     transition_time,                \
                                     ZB_ZCL_CMD_LEVEL_CONTROL_STEP); \
}

/** @} */

#endif /* SDK_NRF5_EXTERNAL_ZBOSS_ADDONS_ZCL_ZB_ZCL_LEVEL_CONTROL_ADDONS_H_ */
