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

#include "app_error.h"
#include "app_util.h"
#include "ble_gap.h"
#include "ble_central.h"
#include "ble_ping.h"
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include <stdlib.h>

#define APP_BLE_CONN_CFG_TAG                  1                                   /**< A tag identifying the SoftDevice BLE configuration. */
#define HEX_BASE                              16                                  /**< Base of BLE address numerical representation. */
#define MICROS_TO_MILLIS                      1000                                /**< Microseconds to milliseconds conversion constant. */

#define DEFAULT_CONN_MIN_INTERVAL             20                                  /**< Determines minimum connection interval in milliseconds. */
#define DEFAULT_CONN_MAX_INTERVAL             75                                  /**< Determines maximum connection interval in milliseconds.  */
#define DEFAULT_CONN_SLAVE_LATENCY            0                                   /**< Determines slave latency in counts of connection events. */
#define DEFAULT_CONN_SUPERVISION_TIMEOUT      4000                                /**< Determines supervision time-out in milliseconds. */

#define DEFAULT_SCAN_INTERVAL                 100                                 /**< Determines scan interval in milliseconds. */
#define DEFAULT_SCAN_WINDOW                   50                                  /**< Determines scan window in milliseconds. */
#define DEFAULT_SCAN_DURATION                 0                                   /**< Timout when scanning. 0x0000 disables timeout. */
#define DEFAULT_SCAN_FILTER_POLICY            BLE_GAP_SCAN_FP_ACCEPT_ALL          /**< Default scanning filter policy. */
#define DEFAULT_SCAN_PHYS                     BLE_GAP_PHY_1MBPS                   /**< Default bitfield of PHYs to scan on. */
#define DEFAULT_SCAN_ACTIVE                   1                                   /**< Default active scanning flag. */

static uint16_t m_conn_min_interval           = DEFAULT_CONN_MIN_INTERVAL;        /**< Minimum Connection Interval in milliseconds. */
static uint16_t m_conn_max_interval           = DEFAULT_CONN_MAX_INTERVAL;        /**< Maximum Connection Interval in milliseconds. */
static uint16_t m_conn_slave_latency          = DEFAULT_CONN_SLAVE_LATENCY;       /**< Slave Latency in number of connection events. */
static uint16_t m_conn_sup_timeout            = DEFAULT_CONN_SUPERVISION_TIMEOUT; /**< Connection Supervision Timeout in milliseconds. */

static uint16_t m_scan_interval               = DEFAULT_SCAN_INTERVAL;            /**< Scan interval in milliseconds. */
static uint16_t m_scan_window                 = DEFAULT_SCAN_WINDOW;              /**< Scan window in milliseconds. */
static uint16_t m_scan_timeout                = DEFAULT_SCAN_DURATION;            /**< Scan timeout in milliseconds. */

static uint16_t m_conn_handle                 = BLE_CONN_HANDLE_INVALID;          /**< Handle of the current connection. */
static uint16_t m_conn_cur_interval           = ROUNDED_DIV(UNIT_1_25_MS * BLE_GAP_CP_MIN_CONN_INTVL_MIN,
                                                            MICROS_TO_MILLIS);    /**< Current Connection Interval in milliseconds. */

static bool     m_conn_central                = false;                            /**< Flag that indicates if the device is acting as a central. */
static bool     m_conn_initiated              = false;                            /**< Flag that indicates if the device initiated a connection. */
static bool     m_conn_param_update_requested = false;                            /**< Flag that indicates that connection parameters should be updated. */

static ble_gap_addr_t m_requested_peer;                                           /**< Bluetooth Device Address of a peer requested to connect to. */

/**@brief Function for converting a string Bluetooth Device Address representation to an array.
 *
 * @param[out]   p_gap_addr      Pointer to a GAP address structure.
 * @param[in]    p_addr_string   Pointer to an address string.
 */
static void string_to_address_convert(ble_gap_addr_t * p_gap_addr, const char * p_addr_string)
{
    uint64_t addr_numeric = strtoull(p_addr_string, NULL, HEX_BASE);

    ASSERT(sizeof(p_gap_addr->addr) <= sizeof(addr_numeric));

    memcpy(p_gap_addr->addr, &addr_numeric, sizeof(p_gap_addr->addr));
}

/**@brief Function for converting an array Bluetooth Device Address representation to a string.
 *
 * @param[out]  p_addr_string   Pointer to an address string.
 * @param[in]   p_gap_addr      Pointer to a GAP address structure.
 * @param[in]   addr_string_len Length of the string buffer.
 */
static void address_to_string_convert(char * p_addr_string, const ble_gap_addr_t * p_gap_addr, uint8_t addr_string_len)
{
    ASSERT(addr_string_len >= 2 * sizeof(p_gap_addr->addr) + 1)
    UNUSED_VARIABLE(addr_string_len);

    int8_t index  = 0;

    for (int i = sizeof(p_gap_addr->addr) - 1; i >= 0; i--)
    {
        index += sprintf((p_addr_string + index), "%x", *(p_gap_addr->addr + i));
    }
}

void ble_connection_bdaddr_get(char * p_bdaddr, uint8_t bdaddr_len)
{
    ble_gap_addr_t local_bdaddr;
    uint32_t       err_code = sd_ble_gap_addr_get(&local_bdaddr);
    APP_ERROR_CHECK(err_code);

    address_to_string_convert(p_bdaddr, &local_bdaddr, bdaddr_len);
}

uint16_t ble_connection_handle_get(void)
{
    return m_conn_handle;
}

void ble_connection_handle_set(uint16_t conn_handle)
{
    m_conn_handle = conn_handle;
}

uint16_t ble_connection_interval_get(void)
{
    if (ble_connection_is_established())
    {
        return m_conn_cur_interval;
    }
    else
    {
        NRF_LOG_INFO("No BLE connection established.")
        return 0;
    }
}

void ble_connection_interval_set(uint16_t interval_ms)
{
    uint32_t interval_units = MSEC_TO_UNITS(interval_ms, UNIT_1_25_MS);

    if (interval_units < BLE_GAP_CP_MIN_CONN_INTVL_MIN)
    {
        NRF_LOG_WARNING("Requested interval is lower than allowed: %d ms",
                        ROUNDED_DIV(UNIT_1_25_MS * BLE_GAP_CP_MIN_CONN_INTVL_MIN, MICROS_TO_MILLIS));
        return;
    }
    else if (interval_units > BLE_GAP_CP_MAX_CONN_INTVL_MAX)
    {
        NRF_LOG_WARNING("Requested interval is higher than allowed: %d ms",
                        ROUNDED_DIV(UNIT_1_25_MS * BLE_GAP_CP_MAX_CONN_INTVL_MAX, MICROS_TO_MILLIS));
        return;
    }
    else
    {
        m_conn_cur_interval = interval_ms;

        ble_gap_conn_params_t conn_params =
        {
            .min_conn_interval = interval_units,
            .max_conn_interval = interval_units,
            .slave_latency     = m_conn_slave_latency,
            .conn_sup_timeout  = MSEC_TO_UNITS(m_conn_sup_timeout, UNIT_10_MS)
        };

        uint32_t err_code = sd_ble_gap_conn_param_update(m_conn_handle, &conn_params);
        APP_ERROR_CHECK(err_code);

        m_conn_param_update_requested = true;
    }
}

bool ble_connection_is_central(void)
{
    return m_conn_central;
}

bool ble_connection_is_established(void)
{
    return m_conn_handle != BLE_CONN_HANDLE_INVALID;
}

void ble_connection_evt_handle(const ble_evt_t * p_evt)
{
    ble_gap_evt_t const * p_gap_evt = &p_evt->evt.gap_evt;

    switch (p_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            ble_connection_handle_set(p_gap_evt->conn_handle);
            m_conn_cur_interval = p_gap_evt->params.connected.conn_params.max_conn_interval;
            m_conn_central      = m_conn_initiated ? true : false;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            ble_connection_handle_set(BLE_CONN_HANDLE_INVALID);
            m_conn_cur_interval = ROUNDED_DIV(UNIT_1_25_MS * BLE_GAP_CP_MIN_CONN_INTVL_MIN,
                                              MICROS_TO_MILLIS);;
            m_conn_initiated    = false;
            m_conn_central      = false;
            break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            if (m_conn_param_update_requested)
            {
                if (m_conn_slave_latency != p_gap_evt->params.conn_param_update.conn_params.slave_latency)
                {
                    NRF_LOG_INFO("Slave latency update has not succeeded. "
                                 "Requested value: %d. Established value: %d.",
                                 m_conn_slave_latency,
                                 p_gap_evt->params.conn_param_update.conn_params.slave_latency);
                }

                if (MSEC_TO_UNITS(m_conn_cur_interval, UNIT_1_25_MS)
                    != p_gap_evt->params.conn_param_update.conn_params.max_conn_interval)
                {
                    NRF_LOG_INFO("Connection interval update has not succeeded. "
                                 "Requested value: %d units. Established value: %d units.",
                                 MSEC_TO_UNITS(m_conn_cur_interval, UNIT_1_25_MS),
                                 p_gap_evt->params.conn_param_update.conn_params.max_conn_interval);
                }

                m_conn_param_update_requested = false;
            }
            m_conn_slave_latency = p_gap_evt->params.conn_param_update.conn_params.slave_latency;

            m_conn_cur_interval = p_gap_evt->params.conn_param_update.conn_params.max_conn_interval;
            m_conn_cur_interval = ROUNDED_DIV(UNIT_1_25_MS * m_conn_cur_interval, MICROS_TO_MILLIS);

            break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            if (ble_connection_is_central())
            {
                uint32_t interval_units = p_gap_evt->params.conn_param_update.conn_params.max_conn_interval;
                uint32_t interval_ms    = ROUNDED_DIV(UNIT_1_25_MS * interval_units, MICROS_TO_MILLIS);
                ble_connection_interval_set(interval_ms);
            }
            break;

        default:
            break;
    }
}

uint32_t ble_connection_scan_interval_get(void)
{
    return m_scan_interval;
}

void ble_connection_scan_interval_set(uint32_t interval_ms)
{
    uint32_t interval_units = MSEC_TO_UNITS(interval_ms, UNIT_0_625_MS);

    if (interval_units < BLE_GAP_SCAN_INTERVAL_MIN)
    {
        NRF_LOG_WARNING("Requested interval is lower than allowed: %d ms",
                        ROUNDED_DIV(UNIT_0_625_MS * BLE_GAP_SCAN_INTERVAL_MIN, MICROS_TO_MILLIS));
        return;
    }
    else if (interval_units > BLE_GAP_SCAN_INTERVAL_MAX)
    {
        NRF_LOG_WARNING("Requested interval is higher than allowed: %d ms",
                        ROUNDED_DIV(UNIT_0_625_MS * BLE_GAP_SCAN_INTERVAL_MAX, MICROS_TO_MILLIS));
        return;
    }
    else
    {
        m_scan_interval = interval_ms;
    }
}

uint32_t ble_connection_scan_window_get(void)
{
    return m_scan_window;
}

void ble_connection_scan_window_set(uint32_t window_ms)
{
    uint32_t window_units = MSEC_TO_UNITS(window_ms, UNIT_0_625_MS);

    if (window_units < BLE_GAP_SCAN_WINDOW_MIN)
    {
        NRF_LOG_WARNING("Requested window is lower than allowed: %d ms",
                        ROUNDED_DIV(UNIT_0_625_MS * BLE_GAP_SCAN_WINDOW_MIN, MICROS_TO_MILLIS));
        return;
    }
    else if (window_ms > m_scan_interval)
    {
        NRF_LOG_WARNING("Requested window is higher than current scan interval: %d ms", m_conn_cur_interval);
        return;
    }
    else
    {
        m_scan_window = window_ms;
    }
}

uint32_t ble_connection_slave_latency_get(void)
{
    return m_conn_slave_latency;
}

void ble_connection_slave_latency_set(uint32_t slave_latency)
{
    if (slave_latency > BLE_GAP_CP_SLAVE_LATENCY_MAX)
    {
        NRF_LOG_WARNING("Requested slave latency is higher than allowed %dms", BLE_GAP_CP_SLAVE_LATENCY_MAX);
        return;
    }
    else
    {
        m_conn_slave_latency = slave_latency;

        ble_gap_conn_params_t conn_params =
        {
            .min_conn_interval = MSEC_TO_UNITS(m_conn_min_interval, UNIT_1_25_MS),
            .max_conn_interval = MSEC_TO_UNITS(m_conn_max_interval, UNIT_1_25_MS),
            .slave_latency     = m_conn_slave_latency,
            .conn_sup_timeout  = MSEC_TO_UNITS(m_conn_sup_timeout, UNIT_10_MS)
        };

        uint32_t err_code = sd_ble_gap_conn_param_update(m_conn_handle, &conn_params);
        APP_ERROR_CHECK(err_code);

        m_conn_param_update_requested = true;
    }
}

void ble_connection_start(const char * p_addr_string)
{
    uint32_t err_code;

    do
    {
        if (ble_connection_is_established())
        {
            break;
        }

        if (p_addr_string == NULL)
        {
            break;
        }

        ble_gap_conn_params_t conn_params =
        {
            .min_conn_interval = MSEC_TO_UNITS(m_conn_min_interval, UNIT_1_25_MS),
            .max_conn_interval = MSEC_TO_UNITS(m_conn_max_interval, UNIT_1_25_MS),
            .slave_latency     = m_conn_slave_latency,
            .conn_sup_timeout  = MSEC_TO_UNITS(m_conn_sup_timeout, UNIT_10_MS)
        };

        ble_gap_scan_params_t scan_params =
        {
            .active        = DEFAULT_SCAN_ACTIVE,
            .interval      = MSEC_TO_UNITS(m_scan_interval, UNIT_0_625_MS),
            .window        = MSEC_TO_UNITS(m_scan_window, UNIT_0_625_MS),
            .timeout       = m_scan_timeout,
            .scan_phys     = DEFAULT_SCAN_PHYS,
            .filter_policy = DEFAULT_SCAN_FILTER_POLICY,
        };

        memset(&m_requested_peer, 0, sizeof(ble_gap_addr_t));
        string_to_address_convert(&m_requested_peer, p_addr_string);
        m_requested_peer.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;

        err_code = sd_ble_gap_connect(&m_requested_peer,
                                      &scan_params,
                                      &conn_params,
                                      APP_BLE_CONN_CFG_TAG);
        APP_ERROR_CHECK(err_code);

        m_conn_initiated = true;

    } while (false);
}

void ble_connection_stop(void)
{
    if (ble_connection_is_established())
    {
        uint32_t err_code = sd_ble_gap_disconnect(m_conn_handle,
                                                  BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        APP_ERROR_CHECK(err_code);
    }
}
