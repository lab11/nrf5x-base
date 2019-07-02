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

#include "nrf.h"
#include "app_error.h"
#include "nrf_sdh_ble.h"
#include "ble_advdata.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "nrf_soc.h"

#define APP_BLE_CONN_CFG_TAG            1                                               /**< A tag identifying the SoftDevice BLE configuration. */
#define MIN_ADV_INTERVAL                100                                             /**< Minimal value of advertising interval in ms. */
#define DEFAULT_ADV_INTERVAL            100                                             /**< Initial value of advertising interval in ms. */

static ble_gap_adv_params_t m_adv_params;                                               /**< Parameters to be passed to the stack when starting advertising. */
static bool                 m_advertising_started;                                      /**< Flag indicating that advertising has started. */
static bool                 m_advertising_resume;                                       /**< Flag indicating whether advertising should be resumed. */
static size_t               m_adv_data_len;                                             /**< Length of advertised data. */
static uint8_t              m_dummy_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                /**< Dummy data buffer that will be sent as advertising data. */

static uint32_t             m_adv_interval_requested = DEFAULT_ADV_INTERVAL;            /**< Advertising interval to be used on next call to ble_advertiser_start(). */
static uint32_t             m_adv_interval_current   = DEFAULT_ADV_INTERVAL;            /**< Current advertising interval. */
static uint8_t              m_adv_handle             = BLE_GAP_ADV_SET_HANDLE_NOT_SET;  /**< Advertising handle. */

static ble_gap_adv_data_t m_adv_data =
{
    .adv_data =
    {
        .p_data = m_dummy_data,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    }
};

static void advertising_prepare(void)
{
    m_dummy_data[0] = m_adv_data_len - 1;
    m_adv_data.adv_data.len = m_adv_data_len;

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
    m_adv_params.duration        = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED;
    m_adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    m_adv_params.p_peer_addr     = NULL;
    m_adv_params.filter_policy   = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval        = MSEC_TO_UNITS(m_adv_interval_requested, UNIT_0_625_MS);

    uint32_t err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &m_adv_params);
    APP_ERROR_CHECK(err_code);
}

void ble_advertiser_stop(void)
{
    if (m_advertising_started)
    {
        uint32_t err_code = sd_ble_gap_adv_stop(m_adv_handle);
        APP_ERROR_CHECK(err_code);

        m_advertising_started = false;
    }
}

void ble_advertiser_start(void)
{
    ble_advertiser_stop();

    if (!m_advertising_started)
    {
        advertising_prepare();

        uint32_t err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
        APP_ERROR_CHECK(err_code);

        m_adv_interval_current = m_adv_interval_requested;
        m_advertising_started = true;
    }
}

bool ble_advertiser_is_started(void)
{
    return m_advertising_started;
}

void ble_advertiser_interval_set(uint32_t interval)
{
    if (interval < MIN_ADV_INTERVAL)
    {
        NRF_LOG_WARNING("Requested interval is lower than allowed %dms", MIN_ADV_INTERVAL);
        return;
    }

    m_adv_interval_requested = interval;
}

uint32_t ble_advertiser_interval_get_requested(void)
{
    return m_adv_interval_requested;
}

uint32_t ble_advertiser_interval_get_current(void)
{
    return m_adv_interval_current;
}

size_t ble_advertiser_adv_data_len_max(void)
{
    return sizeof(m_dummy_data);
}

bool ble_advertiser_adv_data_len_set(size_t len)
{
    if (len > ble_advertiser_adv_data_len_max())
    {
        return false;
    }

    /* We cannot advertise 1 byte */
    if (len == 1)
    {
        return false;
    }

    m_adv_data_len = len;

    return true;
}

size_t ble_advertiser_adv_data_len_get(void)
{
    return m_adv_data_len;
}

void ble_advertiser_evt_handle(const ble_evt_t * p_evt)
{
    switch (p_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_advertising_resume  = m_advertising_started;
            m_advertising_started = false;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            if (m_advertising_resume)
            {
                ble_advertiser_start();
            }
            break;

        default:
            break;
    }
}
