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
#include "ble_db_discovery.h"
#include "nrf_ble_gatt.h"
#include "ble_nus.h"
#include "ble_nus_c.h"
#include "ble_central.h"
#include "ble_ping.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include "timer_api.h"

#define BLE_PING_MAX_BUFFER_LEN       BLE_NUS_MAX_DATA_LEN           /**< Highest possible payload length. */
#define BLE_PING_DEFAULT_BUFFER_LEN   16                             /**< Default message payload length. */
#define BYTES_PER_SEC_TO_BITS_PER_SEC 8                              /**< Bytes to bits conversion constant. */
#define BPS_TO_KBPS                   1024                           /**< Bits per seconds to kilobits per seconds conversion constant. */
#define MILLISECONDS_TO_SECONDS       1000                           /**< Milliseconds to seconds conversion constant. */
#define BLE_PING_ECHO_REQUEST         1                              /**< Echo response is requested. */
#define BLE_PING_ECHO_REPLY           0                              /**< No echo response is requested. */

BLE_NUS_C_DEF(m_ble_nus_c);                                          /**< BLE NUS service client instance. */
BLE_NUS_DEF(m_ble_nus_s, NRF_SDH_BLE_TOTAL_LINK_COUNT);              /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt_c);                                          /**< GATT module instance. */
NRF_BLE_GATT_DEF(m_gatt);                                            /**< GATT module instance. */
BLE_DB_DISCOVERY_DEF(m_db_disc);                                     /**< DB discovery module instance. */

typedef struct
{
    uint16_t length;                                                 /**< Payload length. */
    uint16_t interval;                                               /**< Message sending frequency in milliseconds. */
    uint32_t start_time;                                             /**< Start time of the message exchange. */
    uint32_t duration;                                               /**< Duration of the message exchange. */
    uint32_t messages_sent;                                          /**< Number of messages sent. */
} continuous_ping_t;                                                 /**< Structure that represents continuous message exchange over BLE. */

typedef enum
{
    IDLE = 0,                                                        /**< Idle, ready for message reception. */
    SINGLE_SHOT_MODE,                                                /**< Single burst of data mode. */
    CONTINUOUS_MODE,                                                 /**< Continuous data exchange mode. */
    CONTINUOUS_MODE_STOPPING,                                        /**< Continuous data exchange stopping mode. */
} ping_state_t;                                                      /**< Structure that represents state of the module. */

typedef enum
{
    ECHO_RESPONSE = 0,                                               /**< No response is requested. */
    ECHO_REQUEST,                                                    /**< Echo response is requested. */
} ping_message_t;

static bool              m_initialized;                              /**< Flag that determines whether the module has been initialized. */
static uint8_t           m_data_buffer[BLE_PING_MAX_BUFFER_LEN];     /**< Buffer for message payload. */
static ping_state_t      m_ping_state;                               /**< Current state of the module. */
static continuous_ping_t m_continuous_ping;                          /**< Continuous data exchange structure. */

/**@brief Forward declaration */
static void echo_reply_send(const uint8_t * p_buffer, uint16_t buffer_len);

/***************************************************************************************************
 * @section Private API implementation
 **************************************************************************************************/

/**@brief Function for handling the Nordic Uart Service Client database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in]   p_evt  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_nus_c_on_db_disc_evt(&m_ble_nus_c, p_evt);
}

/**@brief Function for handling the Nordic Uart Service Client events.
 *
 * @details This function is called to notify the application of NUS client events.
 *
 * @param[in]   p_ble_nus_c   NUS Client Handle. This identifies the NUS client.
 * @param[in]   p_ble_nus_evt Pointer to the NUS Client event.
 */
static void ble_nus_c_evt_handler(ble_nus_c_t * p_ble_nus_c, ble_nus_c_evt_t const * p_ble_nus_evt)
{
    uint32_t err_code;

    switch (p_ble_nus_evt->evt_type)
    {
        case BLE_NUS_C_EVT_DISCOVERY_COMPLETE:
            err_code = ble_nus_c_handles_assign(p_ble_nus_c, p_ble_nus_evt->conn_handle, &p_ble_nus_evt->handles);
            APP_ERROR_CHECK(err_code);

            err_code = ble_nus_c_tx_notif_enable(p_ble_nus_c);
            APP_ERROR_CHECK(err_code);

            break;

        case BLE_NUS_C_EVT_NUS_TX_EVT:
            switch (m_ping_state)
            {
                case IDLE:
                    echo_reply_send(p_ble_nus_evt->p_data, p_ble_nus_evt->data_len);
                    break;

                case SINGLE_SHOT_MODE:
                    NRF_LOG_INFO("Received %dB reply over BLE.", p_ble_nus_evt->data_len)
                    m_ping_state = IDLE;
                    break;

                case CONTINUOUS_MODE:
                    NRF_LOG_INFO("Received %dB reply over BLE.", p_ble_nus_evt->data_len);
                    break;

                case CONTINUOUS_MODE_STOPPING:
                    m_ping_state = IDLE;
                    break;

                default:
                    ASSERT(false);
            }
            break;

        case BLE_NUS_C_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected.");
            break;
    }
}

/**@brief Function for handling the data from the Nordic UART Service Server.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in]   p_ble_nus_evt   Nordic UART Service event.
 */
static void ble_nus_s_data_handler(ble_nus_evt_t * p_ble_nus_evt)
{
    if (p_ble_nus_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        switch (m_ping_state)
        {
            case IDLE:
                echo_reply_send(p_ble_nus_evt->params.rx_data.p_data, p_ble_nus_evt->params.rx_data.length);
                break;

            case SINGLE_SHOT_MODE:
                NRF_LOG_INFO("Received %dB reply over BLE.", p_ble_nus_evt->params.rx_data.length)
                m_ping_state = IDLE;
                break;

            case CONTINUOUS_MODE:
                NRF_LOG_INFO("Received %dB reply over BLE.", p_ble_nus_evt->params.rx_data.length)
                break;

            case CONTINUOUS_MODE_STOPPING:
                m_ping_state = IDLE;
                break;

            default:
                ASSERT(false);
        }
    }
}

/**@brief Function for initializing the Nordic Uart Service Client.
 */
static void ble_ping_client_init(void)
{
    uint16_t         conn_handle = ble_connection_handle_get();
    uint32_t         err_code;
    ble_nus_c_init_t nus_init;

    memset(&nus_init, 0, sizeof(nus_init));
    nus_init.evt_handler = ble_nus_c_evt_handler;

    /* GATT initialization. */
    err_code = nrf_ble_gatt_init(&m_gatt_c, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_central_set(&m_gatt_c, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);

    /* NUS Client initialization. */
    err_code = ble_db_discovery_init(db_disc_handler);
    APP_ERROR_CHECK(err_code);

    err_code = ble_nus_c_init(&m_ble_nus_c, &nus_init);
    APP_ERROR_CHECK(err_code);

    err_code = ble_nus_c_handles_assign(&m_ble_nus_c, conn_handle, NULL);
    APP_ERROR_CHECK(err_code);

    /* Discovery of services start. */
    err_code = ble_db_discovery_start(&m_db_disc, conn_handle);
    APP_ERROR_CHECK(err_code);

    m_initialized = true;
}

/**@brief Function for initializing the Nordic Uart Service Server.
 */
static void ble_ping_server_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t nus_init;

    memset(&nus_init, 0, sizeof(nus_init));
    nus_init.data_handler = ble_nus_s_data_handler;

    /* GATT initialization. */
    err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);

    /* NUS Server initialization. */
    err_code = ble_nus_init(&m_ble_nus_s, &nus_init);
    APP_ERROR_CHECK(err_code);

    m_initialized = true;
}

/**@brief Function for initializing the BLE ping module.
 */
static void ble_ping_init(void)
{
    if (!m_initialized)
    {
        if (ble_connection_is_central())
        {
            ble_ping_client_init();
        }
        else
        {
            ble_ping_server_init();
        }
    }
}

/**@brief Function for generating a sequence of random numbers.
 *
 * @param[in] requested_length  Requested length of random sequence in bytes.
 */
static void random_data_generate(uint32_t requested_length)
{
    uint8_t  copy_length = 0;
    uint32_t index       = 0;
    uint32_t err_code;

    requested_length = requested_length % BLE_PING_MAX_BUFFER_LEN;

    memset(m_data_buffer, 0, BLE_PING_MAX_BUFFER_LEN);

    do
    {
        err_code = sd_rand_application_bytes_available_get(&copy_length);
        APP_ERROR_CHECK(err_code);

        if (copy_length > requested_length - index)
        {
            copy_length = requested_length - index;
        }

        if (copy_length > 0)
        {
            err_code = sd_rand_application_vector_get(m_data_buffer + index, copy_length);
            APP_ERROR_CHECK(err_code);

            index += copy_length;
        }
    } while (index < requested_length);
}

/**@brief Function for sending provided number of random bytes.
 *
 * @param[in] buffer_len   Length of the data to send.
 */
static void data_send(uint16_t buffer_len)
{
    uint32_t err_code;

    if (ble_connection_is_central())
    {
        err_code = ble_nus_c_string_send(&m_ble_nus_c, m_data_buffer, buffer_len);
        if (err_code != NRF_SUCCESS)
        {
            NRF_LOG_DEBUG("ble_ping: ble_nus_c_string_send() returned %s", nrf_strerror_get(err_code));
        }
    }
    else
    {
        err_code = ble_nus_data_send(&m_ble_nus_s, m_data_buffer, &buffer_len, ble_connection_handle_get());
        if (err_code != NRF_SUCCESS)
        {
            NRF_LOG_DEBUG("ble_ping: ble_nus_data_send() returned %s", nrf_strerror_get(err_code));
        }
    }
}

/**@brief Function for setting response request flag.
 *
 * @param[in] p_buffer       Data buffer.
 * @param[in] echo_request   Flag that determines whether response request byte should be set.
 */
static void echo_request_flag_set(uint8_t * p_buffer, ping_message_t echo_request)
{
    p_buffer[0] = (uint8_t)echo_request;
}

/**@brief Function for setting response request flag.
 *
 * @param[in] p_buffer  Data buffer.
 *
 * @retval    true      If response request flag is set.
 * @retval    false     Otherwise.
 */
static bool echo_request_flag_get(const uint8_t * p_buffer)
{
    return (ping_message_t)p_buffer[0] == ECHO_REQUEST;
}

/**@brief Function for sending an echo response.
 *
 * @param[in] p_buffer     Data buffer.
 * @param[in] buffer_len   Length of the data to send.
 */
static void echo_reply_send(const uint8_t * p_buffer, uint16_t buffer_len)
{
    if (echo_request_flag_get(p_buffer))
    {
        if (buffer_len > BLE_PING_MAX_BUFFER_LEN)
        {
            NRF_LOG_WARNING("Payload too long. Cannot sent specified number of bytes. "
                            "Maximum payload: %dB", BLE_PING_MAX_BUFFER_LEN);
            return;
        }

        memset(m_data_buffer, 0, BLE_PING_MAX_BUFFER_LEN);
        memcpy(m_data_buffer, p_buffer, buffer_len);

        NRF_LOG_INFO("Received %dB over BLE. Sending echo reply.", buffer_len);
        echo_request_flag_set(m_data_buffer, ECHO_RESPONSE);
        data_send(buffer_len);
    }
    else
    {
        NRF_LOG_INFO("Received %dB over BLE.", buffer_len);
    }
}

/**@brief Function for sending an echo response request.
 *
 * @param[in] requested_length  Requested length of random sequence in bytes.
 */
static void echo_request_send(uint16_t requested_length)
{
    if (requested_length > BLE_PING_MAX_BUFFER_LEN)
    {
        NRF_LOG_WARNING("Payload too long. Cannot sent specified number of bytes. "
                        "Maximum payload: %dB", BLE_PING_MAX_BUFFER_LEN);
        return;
    }

    requested_length = requested_length == 0 ? BLE_PING_DEFAULT_BUFFER_LEN : requested_length;
    random_data_generate(requested_length);

    echo_request_flag_set(m_data_buffer, ECHO_REQUEST);
    data_send(requested_length);
}

/***************************************************************************************************
 * @section Public API implementation
 **************************************************************************************************/

void ble_ping_evt_handle(const ble_evt_t * p_evt)
{
    switch (p_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            if (!m_initialized)
            {
                ble_ping_init();
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_initialized = false;
            break;

        default:
            break;
    }
}

void ble_ping_send(uint32_t requested_length, uint32_t interval_ms)
{
    if (!ble_connection_is_established())
    {
        NRF_LOG_WARNING("No Bluetooth connection established. Data could not be sent.");
        return;
    }

    /* Single burst of data. */
    if (interval_ms == 0)
    {
        echo_request_send(requested_length);
        m_ping_state = SINGLE_SHOT_MODE;
    }
    /* Send messages continuously. */
    else
    {
        uint16_t current_interval = ble_connection_interval_get();

        if (current_interval > interval_ms)
        {
            NRF_LOG_ERROR("BLE messages cannot be sent with specified throughput. "
                          "Try changing throughput to match current connection interval "
                          "or changing the connection interval.");
            return;
        }
        else
        {
            timer_init();
            timer_start();

            m_continuous_ping.length     = requested_length;
            m_continuous_ping.interval   = interval_ms;
            m_continuous_ping.start_time = timer_capture();

            echo_request_send(m_continuous_ping.length);
            timer_set(m_continuous_ping.interval, ble_ping_timer_timeout_handle);

            m_ping_state = CONTINUOUS_MODE;
        }
    }
}

void ble_ping_stop(void)
{
    if (m_ping_state == CONTINUOUS_MODE)
    {
        uint32_t throughput;
        uint32_t bytes_sent;

        m_continuous_ping.duration = timer_capture() - m_continuous_ping.start_time;
        m_continuous_ping.duration = timer_ticks_to_ms(m_continuous_ping.duration);

        bytes_sent = m_continuous_ping.messages_sent * m_continuous_ping.length;
        throughput = BYTES_PER_SEC_TO_BITS_PER_SEC *
                     ROUNDED_DIV(MILLISECONDS_TO_SECONDS * bytes_sent, m_continuous_ping.duration);

        NRF_LOG_INFO("BLE ping test stopped. Average throughput on BLE: %d.%04d kbps.",
                     throughput / BPS_TO_KBPS, throughput % BPS_TO_KBPS);

        timer_stop();
        memset(&m_continuous_ping, 0, sizeof(m_continuous_ping));
        m_ping_state = CONTINUOUS_MODE_STOPPING;
    }
}

void ble_ping_timer_timeout_handle(void)
{
    if (m_ping_state == CONTINUOUS_MODE)
    {
        echo_request_send(m_continuous_ping.length);
        timer_set(m_continuous_ping.interval, ble_ping_timer_timeout_handle);

        m_continuous_ping.messages_sent++;
    }
}
