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
    m_adv_params.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
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
