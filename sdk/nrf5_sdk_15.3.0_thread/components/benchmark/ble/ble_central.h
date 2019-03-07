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

#ifndef BLE_CONNECTOR_H
#define BLE_CONNECTOR_H

#include "ble_gap.h"
#include "ble.h"

/**@brief Get Bluetooth Device Address of a device in a printable form.
 *
 * @param[out]  p_bdaddr    Pointer to an address string.
 */
void ble_connection_bdaddr_get(char * p_bdaddr, uint8_t bdaddr_len);

/**@brief Get current connection handle.
 *
 * @returns Connection handle.
 */
uint16_t ble_connection_handle_get(void);

/**@brief Set a connection handle.
 *
 * @param[in] conn_handle   Connection handle.
 */
void ble_connection_handle_set(uint16_t conn_handle);

/**@brief Get current connection state.
 *
 * @retval true    If device is currently connected on Bluetooth.
 * @retval false   If device is not currently connected on Bluetooth.
 */
bool ble_connection_is_established(void);

/**@brief Application callback for handling BLE events.
 *
 * @param[in] p_evt   Bluetooth stack event.
 */
void ble_connection_evt_handle(const ble_evt_t * p_evt);

/**@brief Get current connection interval.
 *
 * @returns Current connection interval in milliseconds.
 */
uint16_t ble_connection_interval_get(void);

/**@brief Set connection interval.
 *
 * @param[in] interval_ms   Requested connection interval in milliseconds.
 */
void ble_connection_interval_set(uint16_t interval_ms);

/**@brief Get current connection role.
 *
 * @retval true    If device is acting as a central.
 * @retval false   Otherwise.
 */
bool ble_connection_is_central(void);

/**@brief Get current scan interval.
 *
 * @returns Current scan interval in ms.
 */
uint32_t ble_connection_scan_interval_get(void);

/**@brief Set scan interval.
 *
 * @param[in] interval_ms   Scan interval in ms.
 */
void ble_connection_scan_interval_set(uint32_t interval_ms);

/**@brief Get current scan window.
 *
 * @returns Current scan window in ms.
 */
uint32_t ble_connection_scan_window_get(void);

/**@brief Set scan window.
 *
 * @param[in] window_ms   Scan window in ms.
 */
void ble_connection_scan_window_set(uint32_t window_ms);

/**@brief Get current slave latency.
 *
 * @returns Current slave latency in number of connection events.
 */
uint32_t ble_connection_slave_latency_get(void);

/**@brief Set slave latency.
 *
 * @param[in] slave_latency   Slave latency in number of connection events.
 */
void ble_connection_slave_latency_set(uint32_t slave_latency);

/**@brief Connect to a specified Bluetooth address.
 *
 * @note  If called when there already is a connection established, this function has no effect.
 *
 * @param[in] p_addr_string   Pointer to a string representation of peer device's address.
 */
void ble_connection_start(const char * p_addr_string);

/**@brief Disconnect from currently connected device.
 *
 * @note  If called when there is no connection established, this function has no effect.
 */
void ble_connection_stop(void);

#endif // BLE_CONNECTOR_H
