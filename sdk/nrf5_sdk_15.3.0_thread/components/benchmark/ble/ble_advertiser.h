/**
 * Copyright (c) 2018 - 2019, Nordic Semiconductor ASA
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

#ifndef BLE_ADVERTISER_H_
#define BLE_ADVERTISER_H_

#include "ble.h"

#include <stdbool.h>
#include <stdlib.h>

/**************************************************************************************************
 * Public API.
 *************************************************************************************************/

/**@brief   Start Bluetooth advertising. */
void ble_advertiser_start(void);

/**@brief   Stop Bluetooth advertising. */
void ble_advertiser_stop(void);

/**@brief   Get current advertising state.
 *
 * @retval true    If Bluetooth is advertising.
 * @retval false   If Bluetooth is not advertising.
 */
bool ble_advertiser_is_started(void);

/**@brief   Function to set advertising interval. This interval will be used after next call to
 *          @ref ble_advertiser_start()
 *
 * @param[in] interval    Advertising interval in ms.
 */
void ble_advertiser_interval_set(uint32_t interval);

/**@brief   Get advertising interval that was last requested by @ref ble_advertiser_interval_set()
 *
 * @returns Requested advertising interval in ms.
 */
uint32_t ble_advertiser_interval_get_requested(void);

/**@brief   Get advertising interval that is currently used.
 *
 * @returns Current advertising interval in ms.
 */
uint32_t ble_advertiser_interval_get_current(void);

/**@brief   Set advertising data length.
 *
 * @param[in] len  Advertising interval in ms.
 *
 * @returns true    If the request data len was correct.
 */
bool ble_advertiser_adv_data_len_set(size_t len);

/**@brief   Get maximum length of data that can be advertised.
 *
 * @returns Maximum length of data that can be advertised.
 */
size_t ble_advertiser_adv_data_len_max(void);

/**@brief   Get advertising data length.
 *
 * @returns Advertising data length.
 */
size_t ble_advertiser_adv_data_len_get(void);

/**@brief   Application callback for handling BLE events.
 *
 * @param[in] p_evt  Bluetooth stack event.
 */
void ble_advertiser_evt_handle(const ble_evt_t * p_evt);

#endif //BLE_ADVERTISER_H_
