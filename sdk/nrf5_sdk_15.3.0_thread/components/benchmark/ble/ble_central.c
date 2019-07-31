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

#include "app_error.h"
#include "app_util.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_central.h"
#include "ble_ping.h"
#include "ble_conn_state.h"
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include <stdlib.h>

#define MAX_PEER_CNT                     BLE_CONN_STATE_MAX_CONNECTIONS  /**< Maximum number of possible active connections. */

#define APP_BLE_CONN_CFG_TAG             1                               /**< A tag identifying the SoftDevice BLE configuration. */
#define HEX_BASE                         16                              /**< Base of BLE address numerical representation. */
#define MICROS_TO_MILLIS                 1000                            /**< Microseconds to milliseconds conversion constant. */

#define DEFAULT_CONN_INTERVAL            100                             /**< Determines default connection interval in milliseconds. */
#define DEFAULT_CONN_MIN_INTERVAL        20                              /**< Determines minimum connection interval in milliseconds. */
#define DEFAULT_CONN_MAX_INTERVAL        4000                            /**< Determines maximum connection interval in milliseconds.  */
#define DEFAULT_CONN_SLAVE_LATENCY       0                               /**< Determines slave latency in counts of connection events. */
#define DEFAULT_CONN_SUPERVISION_TIMEOUT 4000                            /**< Determines supervision time-out in milliseconds. */

#define DEFAULT_SCAN_INTERVAL            100                             /**< Determines scan interval in milliseconds. */
#define DEFAULT_SCAN_WINDOW              50                              /**< Determines scan window in milliseconds. */
#define DEFAULT_SCAN_DURATION            0                               /**< Timout when scanning. 0x0000 disables timeout. */
#define DEFAULT_SCAN_FILTER_POLICY       BLE_GAP_SCAN_FP_ACCEPT_ALL      /**< Default scanning filter policy. */
#define DEFAULT_SCAN_PHYS                BLE_GAP_PHY_1MBPS               /**< Default bitfield of PHYs to scan on. */
#define DEFAULT_SCAN_ACTIVE              1                               /**< Default active scanning flag. */

static uint16_t m_conn_interval      = DEFAULT_CONN_INTERVAL;            /**< Default Connection Interval in milliseconds. */
static uint16_t m_conn_min_interval  = DEFAULT_CONN_MIN_INTERVAL;        /**< Minimum Connection Interval in milliseconds. */
static uint16_t m_conn_max_interval  = DEFAULT_CONN_MAX_INTERVAL;        /**< Maximum Connection Interval in milliseconds. */
static uint16_t m_conn_slave_latency = DEFAULT_CONN_SLAVE_LATENCY;       /**< Slave Latency in number of connection events. */
static uint16_t m_conn_sup_timeout   = DEFAULT_CONN_SUPERVISION_TIMEOUT; /**< Connection Supervision Timeout in milliseconds. */

static uint16_t m_scan_interval      = DEFAULT_SCAN_INTERVAL;            /**< Scan interval in milliseconds. */
static uint16_t m_scan_window        = DEFAULT_SCAN_WINDOW;              /**< Scan window in milliseconds. */
static uint16_t m_scan_timeout       = DEFAULT_SCAN_DURATION;            /**< Scan timeout in milliseconds. */
static bool     m_scan_enabled       = false;                            /**< Flag that indicated if scan was enabled. */

static bool     m_conn_central       = false;                            /**< Flag that indicates if the device is acting as a central. */
static bool     m_conn_initiated     = false;                            /**< Flag that indicates if the device initiated a connection. */
static bool     m_conn_evt_ext_en    = false;                            /**< Flag that indicates if the connection event extension is enabled. */

static peer_t         m_peers[MAX_PEER_CNT];                             /**< Handles of the current connections. */
static scan_results_t m_scan_results;                                    /**< Container for scanned Bluetooth addresses. */

static uint8_t  m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN];             /**< Advertising report buffer. */

static ble_data_t m_scan_buffer =
{
    .p_data = m_scan_buffer_data,
    .len    = BLE_GAP_SCAN_BUFFER_MIN
};

void address_to_string_convert(char * p_addr_string, const ble_gap_addr_t * p_gap_addr, uint8_t addr_string_len)
{
    ASSERT(addr_string_len >= 2 * sizeof(p_gap_addr->addr) + 1)
    UNUSED_VARIABLE(addr_string_len);

    int8_t index  = 0;

    for (int i = sizeof(p_gap_addr->addr) - 1; i >= 0; i--)
    {
        index += sprintf((p_addr_string + index), "%02x", *(p_gap_addr->addr + i));
    }
}

void string_to_address_convert(ble_gap_addr_t * p_gap_addr, const char * p_addr_string)
{
    uint64_t addr_numeric = strtoull(p_addr_string, NULL, HEX_BASE);

    ASSERT(sizeof(p_gap_addr->addr) <= sizeof(addr_numeric));

    memcpy(p_gap_addr->addr, &addr_numeric, sizeof(p_gap_addr->addr));
}

void ble_connection_bdaddr_get(char * p_bdaddr, uint8_t bdaddr_len)
{
    ble_gap_addr_t local_bdaddr;
    uint32_t       err_code = sd_ble_gap_addr_get(&local_bdaddr);
    APP_ERROR_CHECK(err_code);

    address_to_string_convert(p_bdaddr, &local_bdaddr, bdaddr_len);
}

/**@brief Try to allocate new peer handle.
 *
 * @returns New peer handle structure or NULL if failed to allocate.
 */
static peer_t * ble_conn_alloc(void)
{
    for (size_t i = 0; i < MAX_PEER_CNT; i++)
    {
        if (!m_peers[i].m_used)
        {
            m_peers[i].m_used                        = true;
            m_peers[i].m_conn_cur_interval           = m_conn_interval;
            m_peers[i].m_conn_handle                 = BLE_CONN_HANDLE_INVALID;
            m_peers[i].m_conn_param_update_requested = false;
            m_peers[i].m_phy_mode                    = DEFAULT_SCAN_PHYS;

            NRF_LOG_INFO("Allocated: %u", &m_peers[i]);

            return &m_peers[i];
        }
    }

    return NULL;
}

/**@brief Free peer handle structure.
 *
 * @param[in] p_peer Peer handle structure to be freed.
 */
static void ble_conn_free(peer_t * p_peer)
{
    p_peer->m_used = false;
}

peer_t * ble_conn_find_by_addr(const ble_gap_addr_t * addr)
{
    for (size_t i = 0; i < MAX_PEER_CNT; i++)
    {
        if (m_peers[i].m_used && (memcmp(m_peers[i].m_addr.addr, addr->addr, sizeof(addr->addr)) == 0))
        {
            NRF_LOG_INFO("Found: %u", &m_peers[i]);
            return &m_peers[i];
        }
    }

    return NULL;
}

/**@brief Find peer handle structure by connection handle.
 *
 * @param[in] handle  Connection handle.
 * 
 * @returns New peer handle structure or NULL if failed to allocate.
 */
static peer_t * ble_conn_find_by_handle(uint16_t handle)
{
    for (size_t i = 0; i < MAX_PEER_CNT; i++)
    {
        if (m_peers[i].m_used && (m_peers[i].m_conn_handle == handle))
        {
            return &m_peers[i];
        }
    }

    return NULL;
}

/**@brief Check, if given address is already saved in scan results.
 *
 * @param[in] addr  Bluetooth address to check.
 * 
 * @returns True, if given address was found in scan results.
 */
static bool ble_scan_addr_find(const ble_gap_addr_t * addr)
{
    for (size_t i = 0; i < m_scan_results.m_addr_count; i++)
    {
        if(memcmp(addr, &m_scan_results.m_addrs[i], sizeof(ble_gap_addr_t)) == 0)
        {
            return true;
        }
    }
    return false;
}

/**@brief Add address to scan results.
 * 
 * @note This function only adds unique addresses.
 *
 * @param[in] addr  Bluetooth address to add.
 */
static void ble_scan_addr_add(const ble_gap_addr_t * addr)
{
    if (ble_scan_addr_find(addr) == true)
    {
        return;
    }
    else
    {
        uint8_t addr_count = m_scan_results.m_addr_count;

        if (addr_count < MAX_SCAN_RESULTS)
        {
            memcpy(&m_scan_results.m_addrs[addr_count], addr, sizeof(ble_gap_addr_t));
            m_scan_results.m_addr_count++;

            NRF_LOG_INFO("New scan result added. Stored address count: %d", m_scan_results.m_addr_count);
        }
        else
        {
            NRF_LOG_ERROR("Could not save a scan result. Buffer is full.");
        }
    }
}

const char * phy_mode_to_string_convert(uint8_t phy_mode)
{
    const char * p_str_phy_mode = NULL;

    switch (phy_mode)
    {
        case BLE_GAP_PHY_1MBPS:
            p_str_phy_mode = "1Mbps";
            break;

        case BLE_GAP_PHY_2MBPS:
            p_str_phy_mode = "2Mbps";
            break;

        default:
            p_str_phy_mode = "unknown";
            break;
    }

    return p_str_phy_mode;
}

size_t ble_conn_count(void)
{
    size_t ret = 0;

    for (size_t i = 0; i < MAX_PEER_CNT; i++)
    {
        if (m_peers[i].m_used && (m_peers[i].m_conn_handle != BLE_CONN_HANDLE_INVALID))
        {
            ret++;
        }
    }

    return ret;
}

peer_t * ble_conn_get(size_t idx)
{
    size_t cnt = 0;

    for (size_t i = 0; i < MAX_PEER_CNT; i++)
    {
        if (m_peers[i].m_used && (m_peers[i].m_conn_handle != BLE_CONN_HANDLE_INVALID))
        {
            if (cnt == idx)
            {
                return &m_peers[i];
            }
            cnt++;
        }
    }

    return NULL;
}

bool ble_connection_is_central(void)
{
    return m_conn_central;
}

uint32_t ble_connection_interval_get(void)
{
    return m_conn_interval;
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
        m_conn_interval = interval_ms;

        ble_gap_conn_params_t conn_params =
        {
            .min_conn_interval = interval_units,
            .max_conn_interval = interval_units,
            .slave_latency     = m_conn_slave_latency,
            .conn_sup_timeout  = MSEC_TO_UNITS(m_conn_sup_timeout, UNIT_10_MS)
        };

        for (size_t i = 0; i < ble_conn_count(); i++)
        {
            peer_t * p_peer = ble_conn_get(i);

            if (p_peer == NULL)
            {
                ASSERT(false); // May happen only if we call ble_central API from multiple contexts.
                return;
            }

            p_peer->m_conn_cur_interval = interval_ms;

            uint32_t err_code = sd_ble_gap_conn_param_update(p_peer->m_conn_handle, &conn_params);
            APP_ERROR_CHECK(err_code);

            p_peer->m_conn_param_update_requested = true;
        }
    }
}

void ble_connection_evt_handle(const ble_evt_t * p_evt)
{
    ble_gap_evt_t const * p_gap_evt = &p_evt->evt.gap_evt;
    peer_t              * p_peer    = NULL;
    uint32_t              err_code  = 0;

    switch (p_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            if (ble_conn_count() == 0)
            {
                m_conn_central = m_conn_initiated ? true : false;
            }

            if (ble_connection_is_central())
            {
                m_scan_enabled = false;
                p_peer = ble_conn_find_by_addr(&p_gap_evt->params.connected.peer_addr);
            }
            else
            {
                p_peer = ble_conn_alloc();

                if (p_peer == NULL)
                {
                    NRF_LOG_ERROR("Failed to allocate new peer.");
                }

                p_peer->m_addr = p_gap_evt->params.connected.peer_addr;
            }

            if (p_peer != NULL)
            {
                p_peer->m_conn_handle       = p_gap_evt->conn_handle;
                p_peer->m_conn_cur_interval = p_gap_evt->params.connected.conn_params.max_conn_interval;
            }
            else
            {
                NRF_LOG_ERROR("Connected to unknown board.");
            }

            break;

        case BLE_GAP_EVT_DISCONNECTED:
            p_peer = ble_conn_find_by_handle(p_gap_evt->conn_handle);

            if (p_peer != NULL)
            {
                p_peer->m_conn_handle       = BLE_CONN_HANDLE_INVALID;
                p_peer->m_conn_cur_interval = m_conn_interval;

                if (ble_conn_count() == 0)
                {
                    m_conn_initiated = false;
                    m_conn_central   = false;
                }

                ble_conn_free(p_peer);
            }
            else
            {
                NRF_LOG_ERROR("Disconnected from unknown board.");
            }
            break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            p_peer = ble_conn_find_by_handle(p_gap_evt->conn_handle);

            if (p_peer != NULL)
            {
                if (p_peer->m_conn_param_update_requested)
                {
                    if (m_conn_slave_latency != p_gap_evt->params.conn_param_update.conn_params.slave_latency)
                    {
                        NRF_LOG_INFO("Slave latency update has not succeeded. "
                                    "Requested value: %d. Established value: %d.",
                                    m_conn_slave_latency,
                                    p_gap_evt->params.conn_param_update.conn_params.slave_latency);
                    }

                    if (MSEC_TO_UNITS(p_peer->m_conn_cur_interval, UNIT_1_25_MS)
                        != p_gap_evt->params.conn_param_update.conn_params.max_conn_interval)
                    {
                        NRF_LOG_INFO("Connection interval update has not succeeded. "
                                    "Requested value: %d units. Established value: %d units.",
                                    MSEC_TO_UNITS(p_peer->m_conn_cur_interval, UNIT_1_25_MS),
                                    p_gap_evt->params.conn_param_update.conn_params.max_conn_interval);
                    }

                    p_peer->m_conn_param_update_requested = false;
                }

                m_conn_slave_latency        = p_gap_evt->params.conn_param_update.conn_params.slave_latency;
                p_peer->m_conn_cur_interval = p_gap_evt->params.conn_param_update.conn_params.max_conn_interval;
                p_peer->m_conn_cur_interval = ROUNDED_DIV(UNIT_1_25_MS * p_peer->m_conn_cur_interval, MICROS_TO_MILLIS);
            }
            else
            {
                NRF_LOG_ERROR("Conn_param_update from unknown board.");
            }
            break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            if (ble_connection_is_central())
            {
                uint32_t interval_units = p_gap_evt->params.conn_param_update.conn_params.max_conn_interval;
                uint32_t interval_ms    = ROUNDED_DIV(UNIT_1_25_MS * interval_units, MICROS_TO_MILLIS);
                ble_connection_interval_set(interval_ms);
            }
            break;

        case BLE_GAP_EVT_ADV_REPORT:
            ble_scan_addr_add(&p_gap_evt->params.adv_report.peer_addr);
            err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
            {
                m_scan_enabled = false;

                NRF_LOG_INFO("Scan event timed out.")
            }
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
            NRF_LOG_INFO("Received GAP PHY UPDATE request.");

            /* Accept peer preferred PHY settings. */
            err_code = sd_ble_gap_phy_update(p_gap_evt->conn_handle, &p_gap_evt->params.phy_update_request.peer_preferred_phys);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE:
            if (p_gap_evt->params.phy_update.status != BLE_HCI_STATUS_CODE_SUCCESS)
            {
                NRF_LOG_ERROR("Failed to change PHY mode.");
            }
            else
            {
                p_peer = ble_conn_find_by_handle(p_gap_evt->conn_handle);

                if (p_peer == NULL)
                {
                    NRF_LOG_ERROR("Received event from unrecognized peer.");
                }
                else
                {
                    if (p_gap_evt->params.phy_update.tx_phy != p_gap_evt->params.phy_update.rx_phy)
                    {
                        NRF_LOG_WARNING("RX and TX PHY modes differ.");
                    }
                    else
                    {
                        p_peer->m_phy_mode = p_gap_evt->params.phy_update.tx_phy;
                        NRF_LOG_INFO("Switched to new PHY mode: %s", phy_mode_to_string_convert(p_peer->m_phy_mode));
                    }
                }
            }
            break;

        default:
            break;
    }
}

uint32_t ble_scan_interval_get(void)
{
    return m_scan_interval;
}

void ble_scan_interval_set(uint32_t interval_ms)
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

uint32_t ble_scan_window_get(void)
{
    return m_scan_window;
}

void ble_scan_window_set(uint32_t window_ms)
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
        NRF_LOG_WARNING("Requested window is higher than current scan interval: %d ms", m_scan_interval);
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

        for (size_t i = 0; i < ble_conn_count(); i++)
        {
            peer_t * p_peer = ble_conn_get(i);

            if (p_peer == NULL)
            {
                break;
            }

            uint32_t err_code = sd_ble_gap_conn_param_update(p_peer->m_conn_handle, &conn_params);
            APP_ERROR_CHECK(err_code);

            p_peer->m_conn_param_update_requested = true;
        }

    }
}

bool ble_conn_phy_mode_set(const char * p_addr_string, uint8_t phy_mode)
{
    uint32_t       err_code = 0;
    peer_t       * p_peer   = NULL;
    ble_gap_addr_t gap_addr;
    ble_gap_phys_t gap_phys;

    string_to_address_convert(&gap_addr, p_addr_string);

    p_peer = ble_conn_find_by_addr(&gap_addr);

    if (p_peer == NULL)
    {
        NRF_LOG_ERROR("Cannot set PHY mode. Peer was not found.");

        return false;
    }
    else
    {
        gap_phys.tx_phys = phy_mode;
        gap_phys.rx_phys = phy_mode;

        err_code = sd_ble_gap_phy_update(p_peer->m_conn_handle, &gap_phys);
        APP_ERROR_CHECK(err_code);

        return true;
    }
}

bool ble_connection_event_length_extension_get(void)
{
    return m_conn_evt_ext_en;
}

bool ble_connection_event_length_extension_set(bool evt_ext_enable)
{
    ret_code_t err_code;
    ble_opt_t  ble_opt;

    memset(&ble_opt, 0, sizeof(ble_opt));
    ble_opt.common_opt.conn_evt_ext.enable = evt_ext_enable ? 1 : 0;

    err_code = sd_ble_opt_set(BLE_COMMON_OPT_CONN_EVT_EXT, &ble_opt);
    APP_ERROR_CHECK(err_code);

    m_conn_evt_ext_en = evt_ext_enable;

    NRF_LOG_INFO("Event length extension is %s", m_conn_evt_ext_en ? "enabled" : "disabled");

    return true;
}

void ble_connection_start(const char * p_addr_string)
{
    uint32_t err_code;

    do
    {
        if (p_addr_string == NULL)
        {
            break;
        }

        peer_t * p_peer = ble_conn_alloc();
        if (p_peer == NULL)
        {
            NRF_LOG_ERROR("Failed to allocate new peer.");
            break;
        }

        string_to_address_convert(&p_peer->m_addr, p_addr_string);

        ble_gap_conn_params_t conn_params =
        {
            .min_conn_interval = m_conn_interval,
            .max_conn_interval = m_conn_interval,
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

        p_peer->m_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;

        err_code = sd_ble_gap_connect(&p_peer->m_addr,
                                      &scan_params,
                                      &conn_params,
                                      APP_BLE_CONN_CFG_TAG);
        APP_ERROR_CHECK(err_code);

        if (ble_conn_count() == 0)
        {
            m_conn_initiated = true;
        }

    } while (false);
}

void ble_connection_stop(const char * p_addr_string)
{
    do
    {
        if (p_addr_string == NULL)
        {
            break;
        }

        ble_gap_addr_t peer_addr = {0};

        string_to_address_convert(&peer_addr, p_addr_string);

        peer_t * p_peer = ble_conn_find_by_addr(&peer_addr);

        if (p_peer != NULL)
        {
            NRF_LOG_INFO("Disconnecting device with address %s", p_addr_string);
            uint32_t err_code = sd_ble_gap_disconnect(p_peer->m_conn_handle,
                                                      BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            NRF_LOG_ERROR("Device with address %s is not connected.", p_addr_string);
        }
    } while(false);
}

bool ble_scan_enabled_get(void)
{
    return m_scan_enabled;
}

void ble_scan_start(void)
{
    if (m_scan_enabled == true)
    {
        NRF_LOG_WARNING("Scan is already in progress. Ignoring.")
        return;
    }
    else
    {
        uint32_t              err_code = 0;
        ble_gap_scan_params_t scan_params =
        {
            .active        = DEFAULT_SCAN_ACTIVE,
            .interval      = MSEC_TO_UNITS(m_scan_interval, UNIT_0_625_MS),
            .window        = MSEC_TO_UNITS(m_scan_window, UNIT_0_625_MS),
            .timeout       = m_scan_timeout,
            .scan_phys     = DEFAULT_SCAN_PHYS,
            .filter_policy = DEFAULT_SCAN_FILTER_POLICY,
        };

        m_scan_results.m_addr_count = 0;
        m_scan_enabled              = true;

        err_code = sd_ble_gap_scan_start(&scan_params, &m_scan_buffer);
        APP_ERROR_CHECK(err_code);

        NRF_LOG_INFO("Scan has beed started.")
    }
}

scan_results_t * ble_scan_stop(void)
{
    uint32_t err_code = 0;

    if (m_scan_enabled == true)
    {
        m_scan_enabled = false;
        err_code = sd_ble_gap_scan_stop();
        APP_ERROR_CHECK(err_code);

        NRF_LOG_INFO("Scan has been stopped.")
    }

    return &m_scan_results;
}
