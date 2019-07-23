/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
PURPOSE: ZLL profile configuration
*/

#if ! defined ZB_ZLL_CONFIG_H
#define ZB_ZLL_CONFIG_H

#include "zcl/zb_zcl_config.h"

/*!
    @addtogroup ZB_ZLL_COMMON
    @internal
    @{
*/

/* Normally startup attribute sets should be processed */
#define ZB_ENABLE_ZLL_SAS

/********************** General ZLL config parameters **********************/

/** Defines default time for device to identify (in seconds)\. Shan't be zero. */
#define ZB_ZLL_IDENTIFY_TIME_DEFAULT_VALUE 2

/**
 *  @brief Defines length of a group identifier range provided to the address-assignment-capable
 *  commissioning target device.
 */
#define ZB_ZLL_GROUP_ID_RANGE_ALLOC_LENGTH  1000

/**
 *  @brief Defines length of network address range provided to the address-assignment-capable
 *  commissioning target device.
 */
#define ZB_ZLL_ADDR_RANGE_ALLOC_LENGTH 1000

/**
 *  @brief NLME-NETWORK-DISCOVERY.request.ScanDuration value.
 *  Being used in start new network procedure on the target side.
 *  @see ZLL spec, subclause 8.4.3.2.
 */
#define ZB_ZLL_NWK_DISC_DURATION  3

#define ZB_ZLL_DEVICE_ADDRESS_ASSIGNMENT_CAPABLE

#if defined ZB_ZLL_DEVICE_ADDRESS_ASSIGNMENT_CAPABLE
/** @brief Default address range begin value. */
#define ZB_ZLL_ADDR_BEGIN_INIT_VALUE  0x0001

/** @brief Default address range end value. */
#define ZB_ZLL_ADDR_END_INIT_VALUE  0xfff7

/** @brief Default group identifier range begin value. */
#define ZB_ZLL_GROUP_ID_BEGIN_INIT_VALUE  0x0001

/** @brief Default group identifier range end value. */
#define ZB_ZLL_GROUP_ID_END_INIT_VALUE  0xfeff

#endif /* defined ZB_ZLL_DEVICE_ADDRESS_ASSIGNMENT_CAPABLE */

/** @brief Default RSSI correction value. */
#define ZB_ZLL_DEFAULT_RSSI_CORRECTION  0x00

/** @brief Default minimum RSSI value to process ScanRequest packets. */
#define ZB_ZLL_DEFAULT_RSSI_THRESHOLD 47

/**
 * @}
 */

#endif /* ! defined ZB_ZLL_CONFIG_H */
