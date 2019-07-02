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

#ifndef BLE_CONNECTOR_H
#define BLE_CONNECTOR_H

#include "ble_gap.h"
#include "ble.h"

#define BDADDR_STRING_SIZE 13                              /**< Required string size to fit BLE address. */
#define MAX_SCAN_RESULTS   64                              /**< Maximum number of scan results. */


typedef struct
{
    uint16_t       m_conn_handle;
    uint16_t       m_conn_cur_interval;
    bool           m_used;
    bool           m_conn_param_update_requested;
    uint8_t        m_phy_mode;
    ble_gap_addr_t m_addr;
} peer_t;

typedef struct
{
    ble_gap_addr_t m_addrs[MAX_SCAN_RESULTS];
    uint8_t        m_addr_count;
} scan_results_t;

/**@brief Function for converting an array Bluetooth Device Address representation to a string.
 *
 * @param[out]  p_addr_string   Pointer to an address string.
 * @param[in]   p_gap_addr      Pointer to a GAP address structure.
 * @param[in]   addr_string_len Length of the string buffer.
 */
void address_to_string_convert(char * p_addr_string, const ble_gap_addr_t * p_gap_addr, uint8_t addr_string_len);

/**@brief Function for converting a string Bluetooth Device Address representation to an array.
 *
 * @param[out]   p_gap_addr      Pointer to a GAP address structure.
 * @param[in]    p_addr_string   Pointer to an address string.
 */
void string_to_address_convert(ble_gap_addr_t * p_gap_addr, const char * p_addr_string);

/**@brief Function for converting the physical layer mode identifier to its string representation.
 *
 * @param[in]   phy_mode         Physical layer mode. Set this to either BLE_GAP_PHY_1MBPS or BLE_GAP_PHY_2MBPS.
 * 
 * @retval 1Mbps String representation of the physical layer mode, when 1Mbps.
 * @retval 2Mbps String representation of the physical layer mode, when 2Mbps.
 * @retval Unknown Physical layer mode is not recognized.
 */
const char * phy_mode_to_string_convert(uint8_t phy_mode);

/**@brief Get Bluetooth Device Address of a device in a printable form.
 *
 * @param[out]  p_bdaddr    Pointer to an address string.
 */
void ble_connection_bdaddr_get(char * p_bdaddr, uint8_t bdaddr_len);

/**@brief Find connected peer by its Bluetooth Device Address.
 *
 * @param[in] p_addr    Pointer to Bluetooth Device Address.
 *
 * @returns Peer with given Bluetooth Device Address or NULL if peer is not found.
 */
peer_t * ble_conn_find_by_addr(const ble_gap_addr_t * p_addr);

/**@brief Get number of connected peers.
 *
 * @returns Number of connected peers.
 */
size_t ble_conn_count(void);

/**@brief Get peer with given index.
 *
 * @param[in] idx    Peer index.
 *
 * @returns Pointer to the peer with given index or NULL if index is higher than number of connected peers.
 */
peer_t * ble_conn_get(size_t idx);

/**@brief Get last requested connection interval.
 *
 * @returns Last requested connection interval.
 */
uint32_t ble_connection_interval_get(void);

/**@brief Application callback for handling BLE events.
 *
 * @param[in] p_evt   Bluetooth stack event.
 */
void ble_connection_evt_handle(const ble_evt_t * p_evt);

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
uint32_t ble_scan_interval_get(void);

/**@brief Set scan interval.
 *
 * @param[in] interval_ms   Scan interval in ms.
 */
void ble_scan_interval_set(uint32_t interval_ms);

/**@brief Get current scan window.
 *
 * @returns Current scan window in ms.
 */
uint32_t ble_scan_window_get(void);

/**@brief Set scan window.
 *
 * @param[in] window_ms   Scan window in ms.
 */
void ble_scan_window_set(uint32_t window_ms);

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

/**@brief Set the speed mode for the physical layer for the given connection.
 *
 * @param[in] p_addr_string   Pointer to the string representation of the address of the peer device.
 * @param[in] phy_mode        Physical layer mode. Set this to either BLE_GAP_PHY_1MBPS or BLE_GAP_PHY_2MBPS.
 *
 * @returns True, if correct peer address was passed as the argument.
 */
bool ble_conn_phy_mode_set(const char * p_addr_string, uint8_t phy_mode);

/**@brief Get the BLE connection event length extension mode.
 *
 * @returns True, if connection event length extension is enabled. False otherwise.
 */
bool ble_connection_event_length_extension_get(void);

/**@brief Set the BLE connection event length extension mode.
 *
 * @param[in] evt_ext_enable   Connection event length extension toggle. Pass true to enable or false to disable.
 * 
 * @returns True, if setting the BLE connection event length extension mode is successful. False otherwise.
 */
bool ble_connection_event_length_extension_set(bool evt_ext_enable);

/**@brief Connect to a specified Bluetooth address.
 *
 * @note  If called when there already is a connection established, this function has no effect.
 *
 * @param[in] p_addr_string   Pointer to the string representation of the address of the peer device.
 */
void ble_connection_start(const char * p_addr_string);

/**@brief Disconnect from currently connected device.
 *
 * @note  If called when there is no connection established, this function has no effect.
 */
void ble_connection_stop(const char * p_addr_string);

/**@brief Get Bluetooth scan state.
 *
 * @return True, if Bluetooth scan is currently in progress.
 */
bool ble_scan_enabled_get(void);

/**@brief Start Bluetooth scan.
 *
 * @note  If called when scan is in progress, this function has no effect.
 */
void ble_scan_start(void);

/**@brief Stop Bluetooth scan and return scan results.
 *
 * @note  If called when scan is not in progress, this function returns addresses collected during last scan.
 *
 * @return Returns structure containing Bluetooth addresses collected during scan.
 */
scan_results_t * ble_scan_stop(void);

#endif // BLE_CONNECTOR_H
