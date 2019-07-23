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

#include "sdk_config.h"
#include "zboss_api.h"
#include "zb_zcl_common.h"

void zb_zcl_finish_and_send_packet_no_ack(zb_buf_t * p_buf, zb_uint8_t * p_cmd_ptr,
                                   zb_addr_u * p_dst_addr, zb_uint8_t dst_addr_mode,
                                   zb_uint8_t dst_ep, zb_uint8_t src_ep,
                                   zb_uint16_t profile_id, zb_uint16_t cluster_id,
                                   zb_callback_t p_cb)
{
    zb_uint16_t remote_addr_short;

    // Mark command space inside buffer as used.
    ZB_BUF_ALLOC_LEFT(p_buf, ZB_ZCL_GET_BYTES_WRITTEN(p_buf, p_cmd_ptr), p_cmd_ptr);

    switch (dst_addr_mode)
    {
        case ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT:
            remote_addr_short = ZB_UNKNOWN_SHORT_ADDR;
            break;

        case ZB_APS_ADDR_MODE_16_ENDP_PRESENT:
            remote_addr_short = p_dst_addr->addr_short;
            break;

        case ZB_APS_ADDR_MODE_64_ENDP_PRESENT:
            {
                remote_addr_short = zb_address_short_by_ieee(p_dst_addr->addr_long);
                if (remote_addr_short == ZB_UNKNOWN_SHORT_ADDR)
                {
                    /* Can't map long address to short, drop packet */
                    ZB_FREE_BUF(p_buf);
                    return;
                }
                dst_addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
            }
            break;

        default:
            ZB_FREE_BUF(p_buf);
            return;
    }

    ZB_ZCL_SEND_COMMAND_SHORT_WITHOUT_ACK(p_buf, remote_addr_short,
            dst_addr_mode, dst_ep, src_ep, profile_id,
        cluster_id, p_cb, 0);

}
