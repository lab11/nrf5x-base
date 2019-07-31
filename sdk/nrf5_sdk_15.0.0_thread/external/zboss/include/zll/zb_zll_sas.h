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
 * PURPOSE: ZLL startup attribute sets definitions
*/
#if ! defined ZB_ZLL_SAS_H
#define ZB_ZLL_SAS_H

#if defined ZB_ENABLE_ZLL_SAS || defined DOXYGEN

/**
 *  @internal
 *  @defgroup ZB_ZLL_SAS ZLL SAS
 *  @ingroup ZB_ZLL
 *  @addtogroup ZB_ZLL_SAS
 *  ZLL startup attribute sets definitions
 *  @{
 */


/** SAS short address */
#define ZB_ZLL_SAS_SHORT_ADDRESS ZB_NWK_BROADCAST_ALL_DEVICES

/** SAS extension Pan ID*/
#define ZB_ZLL_SAS_EXT_PAN_ID {0, 0, 0, 0, 0, 0, 0, 0}

/** SAS 16bit Pan ID */
#define ZB_ZLL_SAS_PAN_ID ZB_BROADCAST_PAN_ID

/** SAS channel mask */
#define ZB_ZLL_SAS_CHANNEL_MASK ZB_TRANSCEIVER_ALL_CHANNELS_MASK

/** SAS permit join is enable */
#define ZB_ZLL_SAS_DISABLE_PERMIT_JOIN  0

/** SAS insecure join is true */
#define ZB_ZLL_SAS_INSECURE_JOIN ZB_TRUE

/** @brief SAS procedure */
zb_void_t zb_zll_process_sas();

#endif /* defined ZB_ENABLE_ZLL_SAS */

/**
 * @}
 */

#endif /* ! defined ZB_ZLL_SAS_H */
