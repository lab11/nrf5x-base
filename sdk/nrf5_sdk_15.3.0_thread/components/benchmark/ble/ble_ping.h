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

#ifndef BLE_PING_H
#define BLE_PING_H

#include "ble.h"
#include <stdint.h>

/**@brief Initializes BLE ping module.
 */
void ble_ping_init(void);

/**@brief Callback for handling BLE events.
 *
 * @param[in] p_evt   Bluetooth stack event.
 */
void ble_ping_evt_handle(const ble_evt_t * p_evt);

/**@brief Function for sending a single diagnostic message.
 *
 * @note  If called when there is no Bluetooth connection established, this function has no effect.
 *
 * @param[in] requested_length  Number of bytes to send.
 */
void ble_ping_send(uint32_t requested_length);

/**@brief Function for getting maximum GATT data length.
 *
 * @details This function returns the maximum GATT data length used for future connections.
 *
 * @return GATT data length that will be used for future connections.
 */
uint8_t ble_ping_gatt_data_length_get(void);

/**@brief Function for getting the maximum GATT data length.
 *
 * @details This function sets the maximum GATT data length send over the air in a single packet.
 *          The actual maximum ATT payload length that can be sent in single packet
 *          is smaller by L2CAP and ATT header sizes (7 octets). This value should
 *          be set before connections are established for both central and peripherals.
 *          The setting applies to all future connections.
 *          Requested value must be between 27 and 251 bytes.
 *
 * @param[in] data_length  Requested GATT data length.
 *
 * @return True, if request was accepted.
 */
bool ble_ping_gatt_data_length_set(uint32_t data_length);

/**@brief Function for continuously sending diagnostic messages with the specified size.
 *
 * @note  This function has no effect if called when there is no Bluetooth connection established.
 */
void ble_flood_start(void);

/**@brief Function for setting the size of a single flood packet.
 *
 * @param[in] requested_length  Number of bytes to send (per ATT payload).
 */
uint32_t ble_flood_length_set(uint32_t requested_length);

/**@brief Function for setting the BLE flood autostart.
 *
 * @param[in] autostart_enable  Set to true to enable BLE flood autostart on benchmark test start.
 */
void ble_flood_autostart_set(bool autostart_enable);

/**@brief Function for stopping sending diagnostic messages.
 *
 * @note  If called when continuous data exchange is not enabled, this function has no effect.
 */
void ble_flood_stop(void);

#endif // BLE_PING_H
