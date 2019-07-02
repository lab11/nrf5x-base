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
#include "nrf_strerror.h"
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
#include "benchmark_api.h"

#define BLE_MESSAGE_MAX_BUFFER_LEN       BLE_NUS_MAX_DATA_LEN            /**< Highest possible payload length. */
#define BLE_MESSAGE_DEFAULT_BUFFER_LEN   16                              /**< Default message payload length. */
#define BYTES_TO_BITS                    8                               /**< Bytes to bits conversion constant. */
#define BPS_TO_KBPS                      1000                            /**< Bits per seconds to kilobits per seconds conversion constant. */
#define MILLISECONDS_TO_SECONDS          1000                            /**< Milliseconds to seconds conversion constant. */
#define BLE_GATT_DEFAULT_DATA_LENGTH     27                              /**< Default value of GATT data length. */
#define BLE_GATT_MAXIMUM_DATA_LENGTH     251                             /**< Maximum value of GATT data length. */
#define BLE_MEASUREMENT_INTERVAL         20                              /**< Packet interval between taking a measurement during ping flood. */

BLE_NUS_C_DEF(m_ble_nus_c);                                              /**< BLE NUS service client instance. */
BLE_NUS_DEF(m_ble_nus_s, NRF_SDH_BLE_TOTAL_LINK_COUNT);                  /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt_c);                                              /**< GATT module instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                /**< GATT module instance. */
BLE_DB_DISCOVERY_DEF(m_db_disc);                                         /**< DB discovery module instance. */

typedef struct
{
    uint16_t length;                                                     /**< Payload length. */
    uint32_t start_time;                                                 /**< Start time of the message exchange. */
    uint32_t messages_transfered;                                        /**< Number of messages sent. */
} continuous_transfer_t;                                                 /**< Structure that represents continuous message exchange over BLE. */

typedef enum
{
    IDLE = 0,                                                            /**< Idle, ready for message reception. */
    SINGLE_SHOT_MODE,                                                    /**< Single burst of data mode. */
    CONTINUOUS_MODE,                                                     /**< Continuous data exchange mode. */
    CONTINUOUS_MODE_STOPPING,                                            /**< Continuous data exchange stopping mode. */
} ping_state_t;                                                          /**< Structure that represents state of the module. */

typedef enum
{
    NOP = 0,                                                             /**< No operation is requested. */
    ECHO_RESPONSE,                                                       /**< Send on echo request. */
    ECHO_REQUEST,                                                        /**< Echo response is requested. */
    MEASUREMENT_RESPONSE,                                                /**< Send data transfer measurements. */
    MEASUREMENT_REQUEST,                                                 /**< Request data transfer measurements form peer. */
    FLOOD_START,                                                          /**< First packet sent during flood process. */
    FLOOD_STOP                                                            /**< Last packet sent during flood process. */
} message_type_t;                                                        /**< Enumeration representing message type. */

static bool                         m_initialized;                              /**< Flag that determines whether the module has been initialized. */
static bool                         m_autostart;                                /**< Flag that determines whether the BLE flood will be started on test start. */
static uint8_t                      m_data_buffer[BLE_MESSAGE_MAX_BUFFER_LEN];  /**< Buffer for message payload. */
static ping_state_t                 m_ping_state;                               /**< Current state of the module. */
static benchmark_ble_ping_results_t m_flood_results;                             /**< Structure for holding amount of data sent and throughput.
                                                                                     This variable is used in two contexts, depending on whether the
                                                                                     device is running as a NUS server or client.
                                                                                     If used by the NUS server, the variable contains the number of bytes
                                                                                     sent to the NUS client, time duration in ms and measured throughput.
                                                                                     This variable is updated periodically after every
                                                                                     BLE_MEASUREMENT_INTERVAL number of messages. 
                                                                                     If used by the NUS client, the variable contains the number of bytes
                                                                                     received from the NUS server, time duration since first FLOOD_START
                                                                                     message and measured throughput. This variable is updated on 
                                                                                     every MEASUREMENT_REQUEST message and sent to the NUS server. */
static continuous_transfer_t m_continuous_transfer =
{
    .length              = BLE_MESSAGE_DEFAULT_BUFFER_LEN,
    .start_time          = 0,
    .messages_transfered = 0
};                                                                              /**< Continuous data exchange structure. This variable is used in
                                                                                     two contexts, depending on whether the device is running as a
                                                                                     NUS server or client.
                                                                                     If used by the NUS server, the variable contains the number of messages
                                                                                     sent to the client and size of single message. 
                                                                                     If used by the NUS client, the variable contains the number of messages
                                                                                     and size of single message sent during flood. It also contains the
                                                                                     timestamp of the first received FLOOD_START message. */


/**@brief Forward declaration */
static void     message_process(const uint8_t * p_buffer, uint16_t buffer_len);
static uint32_t message_send(uint16_t requested_length, message_type_t message_type, const uint8_t * p_data, uint8_t length);
static uint32_t throughput_calculate(benchmark_ble_results_t * p_results);

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
static void nus_c_evt_handler(ble_nus_c_t * p_ble_nus_c, ble_nus_c_evt_t const * p_ble_nus_evt)
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
                    m_continuous_transfer.messages_transfered++;
                    message_process(p_ble_nus_evt->p_data, p_ble_nus_evt->data_len);
                    break;

                case SINGLE_SHOT_MODE:
                    message_process(p_ble_nus_evt->p_data, p_ble_nus_evt->data_len);
                    m_ping_state = IDLE;
                    break;

                case CONTINUOUS_MODE:
                case CONTINUOUS_MODE_STOPPING:
                    /* NUS client cannot flood messages. */
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

/**@brief Function for handling the BLE_NUS_EVT_TX_RDY event from the Nordic UART Service Server.
 *
 * @details This function will run the NUS server application state machine for sending and receiving messages.
 *
 * @param[in]   p_ble_nus_evt   Nordic UART Service event.
 */
static void on_nus_s_tx(ble_nus_evt_t * p_ble_nus_evt)
{
    uint32_t err_code;

    switch (m_ping_state)
    {
        case CONTINUOUS_MODE:
            do
            {
                message_type_t message = NOP;

                /* Request data transfer measurements from peer after every BLE_MEASUREMENT_INTERVAL number of packets sent. */
                if ((m_continuous_transfer.messages_transfered % BLE_MEASUREMENT_INTERVAL) == 0)
                {
                    message = MEASUREMENT_REQUEST;
                    m_flood_results.local_results.bytes_transfered = m_continuous_transfer.messages_transfered * m_continuous_transfer.length;
                    m_flood_results.local_results.duration         = timer_ticks_to_ms(timer_capture() - m_continuous_transfer.start_time);
                    m_flood_results.local_results.throughput       = throughput_calculate(&m_flood_results.local_results);
                }

                err_code = message_send(m_continuous_transfer.length, message, NULL, 0);
                if (err_code == NRF_SUCCESS)
                {
                    m_continuous_transfer.messages_transfered++;
                }
                else if (err_code == NRF_ERROR_RESOURCES)
                {
                    /* Intentionally empty. */
                }
                else
                {
                    NRF_LOG_ERROR("Message could not be sent.");
                }
            } while (err_code == NRF_SUCCESS);
            break;

        case CONTINUOUS_MODE_STOPPING:
            NRF_LOG_INFO("Sending stop message to the peer.");
            UNUSED_RETURN_VALUE(message_send(m_continuous_transfer.length, FLOOD_STOP, NULL, 0));
            m_ping_state = IDLE;
            break;

        case SINGLE_SHOT_MODE:
            m_ping_state = IDLE;
            break;

        case IDLE:
            break;

        default:
            ASSERT(false); /* Should never happen. */
            break;
    }
}

/**@brief Function for handling events from the Nordic UART Service Server.
 *
 * @details This function will process the event received from the Nordic UART BLE Service.
 *
 * @param[in]   p_ble_nus_evt   Nordic UART Service event.
 */
static void nus_s_data_handler(ble_nus_evt_t * p_ble_nus_evt)
{
    switch (p_ble_nus_evt->type)
    {
        case BLE_NUS_EVT_RX_DATA:
            message_process(p_ble_nus_evt->params.rx_data.p_data, p_ble_nus_evt->params.rx_data.length);
            break;

        case BLE_NUS_EVT_TX_RDY:
            on_nus_s_tx(p_ble_nus_evt);
            break;

        default:
            break;
    }
}

/**@brief Function for updating NUS handles.
 */
static void client_handle_update(void)
{
    peer_t * p_peer      = ble_conn_get(0);
    uint16_t conn_handle = p_peer->m_conn_handle;
    uint32_t err_code;

    err_code = ble_nus_c_handles_assign(&m_ble_nus_c, conn_handle, NULL);
    APP_ERROR_CHECK(err_code);

    /* Discovery of services start. */
    err_code = ble_db_discovery_start(&m_db_disc, conn_handle);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the BLE ping module.
 */
static void on_connect(void)
{
    if (!m_initialized)
    {
        NRF_LOG_DEBUG("BLE ping initialized.");

        m_initialized = true;
        m_autostart   = false;
    }

    if (ble_connection_is_central())
    {
        client_handle_update();
    }
}

/**@brief Function for sending provided number of random bytes.
 *
 * @param[in] buffer_len   Length of the data to send.
 * 
 * @returns Error code.
 */
static uint32_t data_send(uint16_t buffer_len)
{
    uint32_t err_code;

    if (buffer_len > BLE_MESSAGE_MAX_BUFFER_LEN)
    {
        NRF_LOG_WARNING("Payload too long. Cannot send specified number of bytes. "
                        "Maximum payload: %dB", BLE_MESSAGE_MAX_BUFFER_LEN);
        return NRF_ERROR_INVALID_LENGTH;
    }

    if (ble_connection_is_central())
    {
        err_code = ble_nus_c_string_send(&m_ble_nus_c, m_data_buffer, buffer_len);
        if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_RESOURCES))
        {
            NRF_LOG_ERROR("ble_ping: ble_nus_c_string_send() returned %s", nrf_strerror_get(err_code));
        }
    }
    else
    {
        peer_t * p_peer = ble_conn_get(0);

        err_code = ble_nus_data_send(&m_ble_nus_s, m_data_buffer, &buffer_len, p_peer->m_conn_handle);

        if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_RESOURCES))
        {
            NRF_LOG_ERROR("ble_ping: ble_nus_data_send() returned %s", nrf_strerror_get(err_code));
        }
    }

    return err_code;
}

/**@brief Function for setting message flags with associated data.
 *
 * @param[in]   p_buffer       Data buffer.
 * @param[in]   echo_request   Flag that determines whether response request byte should be set.
 * @param[in]   p_data         Source data buffer. Set this value to NULL if there is no data to send.
 * @param[in]   length         Length of the data to send.
 */
static void message_data_set(uint8_t * p_buffer, message_type_t command, const uint8_t * p_data, uint8_t length)
{
    p_buffer[0] = (uint8_t)command;

    if (p_data != NULL)
    {
        memcpy(&p_buffer[sizeof(message_type_t)], p_data, length);
    }
}

/**@brief Function for setting response request flag.
 *
 * @param[in] p_buffer  Data buffer.
 *
 * @retval    true      If response request flag is set.
 * @retval    false     Otherwise.
 */
static message_type_t message_type_get(const uint8_t * p_buffer)
{
    return (message_type_t)p_buffer[0];
}

/**@brief Function for reading extra data from the message.
 *
 * @param[in]    p_buffer     Message data buffer.
 * @param[inout] p_data       Destination buffer.
 * @param[in]    length       Length of the data to read.
 */
static void message_extra_data_get(const uint8_t * p_buffer, uint8_t * p_data, uint8_t length)
{
    memcpy(p_data, &p_buffer[sizeof(message_type_t)], length);
}

/**@brief Function for sending an echo response.
 *
 * @param[in] p_buffer     Data buffer.
 * @param[in] buffer_len   Length of the data to send.
 */
static void message_process(const uint8_t * p_buffer, uint16_t buffer_len)
{
    memset(m_data_buffer, 0, BLE_MESSAGE_MAX_BUFFER_LEN);
    memcpy(m_data_buffer, p_buffer, buffer_len);

    switch (message_type_get(p_buffer))
    {
        case ECHO_REQUEST:
            NRF_LOG_INFO("Received %dB echo request over BLE. Sending echo reply.", buffer_len);
            UNUSED_RETURN_VALUE(message_send(buffer_len, ECHO_RESPONSE, NULL, 0));
            break;

        case ECHO_RESPONSE:
            NRF_LOG_INFO("Received %dB echo response over BLE.", buffer_len);
            break;

        case MEASUREMENT_REQUEST:
            m_flood_results.local_results.bytes_transfered = m_continuous_transfer.messages_transfered * m_continuous_transfer.length;
            m_flood_results.local_results.duration         = timer_ticks_to_ms(timer_capture() - m_continuous_transfer.start_time);
            m_flood_results.local_results.throughput       = throughput_calculate(&m_flood_results.local_results);

            /* Send measurements. */
            UNUSED_RETURN_VALUE(message_send(sizeof(benchmark_ble_results_t) + sizeof(message_type_t),
                                MEASUREMENT_RESPONSE, (uint8_t *)&m_flood_results.local_results,
                                sizeof(m_flood_results.local_results)));
            break;

        case MEASUREMENT_RESPONSE:
            message_extra_data_get(p_buffer, (uint8_t *)&m_flood_results.remote_results, sizeof(m_flood_results.remote_results));
            break;

        case FLOOD_START:
            NRF_LOG_INFO("Flood started. Starting up the measurement.");

            timer_init();
            timer_start();

            memset(&m_continuous_transfer, 0, sizeof(m_continuous_transfer));

            m_continuous_transfer.length     = buffer_len;
            m_continuous_transfer.start_time = timer_capture();
            break;

        case FLOOD_STOP:
            NRF_LOG_INFO("Flood stopped. Stopping the measurement.");
            timer_stop();
            break;

        case NOP:
            NRF_LOG_INFO("Received %dB over BLE.", buffer_len);
            break;

        default:
            NRF_LOG_WARNING("Unrecognized message.");
            break;
    }
}

/**@brief Function for sending a message.
 *
 * @param[in] requested_length  Requested length of entire message.
 * @param[in] message_type      Type of sent message.
 * @param[in] p_data            Source extra data buffer. Set this value to NULL if there is no data to send.
 * @param[in] length            Length of the data to send.
 * 
 * @returns Error code.
 */
static uint32_t message_send(uint16_t requested_length, message_type_t message_type, const uint8_t * p_data, uint8_t length)
{
    requested_length = requested_length == 0 ? BLE_MESSAGE_DEFAULT_BUFFER_LEN : requested_length;
    memset(m_data_buffer, requested_length, requested_length);

    message_data_set(m_data_buffer, message_type, p_data, length);
    return data_send(requested_length);
}

/**@brief Function for calculating throughput.
 *
 * @param[in] p_results         Pointer to structure containing measurements.
 *
 * @returns Throughput in bits per second.
 */
static uint32_t throughput_calculate(benchmark_ble_results_t * p_results)
{
    return BYTES_TO_BITS * ROUNDED_DIV(MILLISECONDS_TO_SECONDS * p_results->bytes_transfered, p_results->duration);
}

/***************************************************************************************************
 * @section Public API implementation
 **************************************************************************************************/

/**@brief Initializes BLE ping module.
 */
void ble_ping_init(void)
{
    uint32_t         err_code;
    ble_nus_init_t   nus_init;
    ble_nus_c_init_t nus_c_init;

    /* GATT initialization. */
    err_code = nrf_ble_gatt_init(&m_gatt_c, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_central_set(&m_gatt_c, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);

    memset(&nus_init, 0, sizeof(nus_init));
    nus_init.data_handler = nus_s_data_handler;

    /* NUS Server initialization. */
    err_code = ble_nus_init(&m_ble_nus_s, &nus_init);
    APP_ERROR_CHECK(err_code);

    memset(&nus_c_init, 0, sizeof(nus_c_init));
    nus_c_init.evt_handler = nus_c_evt_handler;

    /* NUS Client initialization. */
    err_code = ble_db_discovery_init(db_disc_handler);
    APP_ERROR_CHECK(err_code);

    err_code = ble_nus_c_init(&m_ble_nus_c, &nus_c_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for getting maximum GATT data length.
 *
 * @details This function returns the maximum GATT data length used for future connections.
 *
 * @return GATT data length that will be used for future connections.
 */
uint8_t ble_ping_gatt_data_length_get(void)
{
    uint8_t    data_length = 0;
    ret_code_t err_code;

    err_code = nrf_ble_gatt_data_length_get(&m_gatt, BLE_CONN_HANDLE_INVALID, &data_length);
    APP_ERROR_CHECK(err_code);

    /* If data_length == 0 then no data length was set earlier. Default value is used. */
    if (data_length == 0)
    {
        return BLE_GATT_DEFAULT_DATA_LENGTH;
    }

    return data_length;
}

/**@brief Function for settting maximum GATT data length.
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
bool ble_ping_gatt_data_length_set(uint32_t data_length)
{
    ret_code_t err_code;

    if ((data_length > BLE_GATT_MAXIMUM_DATA_LENGTH) || (data_length < BLE_GATT_DEFAULT_DATA_LENGTH))
    {
        NRF_LOG_ERROR("Requested GATT data length not within bounds. Value should be between %d and %d",
                      BLE_GATT_DEFAULT_DATA_LENGTH, BLE_GATT_MAXIMUM_DATA_LENGTH);
        return false;
    }

    err_code = nrf_ble_gatt_data_length_set(&m_gatt, BLE_CONN_HANDLE_INVALID, data_length);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("GATT data length could not be set. Requested data length: %d", data_length);
        return false;
    }

    err_code = nrf_ble_gatt_data_length_set(&m_gatt_c, BLE_CONN_HANDLE_INVALID, data_length);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("GATT data length could not be set. Requested data length: %d", data_length);
        return false;
    }

    return true;
}

void ble_ping_evt_handle(const ble_evt_t * p_evt)
{
    switch (p_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect();
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            break;

        default:
            break;
    }
}

void ble_ping_send(uint32_t requested_length)
{
    if (ble_conn_count() == 0)
    {
        NRF_LOG_WARNING("No Bluetooth connection established. Data could not be sent.");
        return;
    }

    /* Single burst of data. */
    UNUSED_RETURN_VALUE(message_send(requested_length, ECHO_REQUEST, NULL, 0));
    m_ping_state = SINGLE_SHOT_MODE;
}

void ble_flood_start()
{
    uint32_t err_code;

    if (ble_conn_count() == 0)
    {
        NRF_LOG_WARNING("No Bluetooth connection established. Data could not be sent.");
    }
    else if(ble_connection_is_central() == true)
    {
        NRF_LOG_WARNING("BLE flood can only be started by the peripheral. Data could not be sent.");
    }
    else
    {
        timer_init();
        timer_start();

        memset(&m_flood_results, 0, sizeof(m_flood_results));

        m_continuous_transfer.start_time          = timer_capture();
        m_continuous_transfer.messages_transfered = 0;

        m_ping_state = CONTINUOUS_MODE;

        /* Send FLOOD_START message to let the peer start the timer. */
        err_code = message_send(m_continuous_transfer.length, FLOOD_START, NULL, 0);
        m_continuous_transfer.messages_transfered++;

        while (err_code == NRF_SUCCESS)
        {
            err_code = message_send(m_continuous_transfer.length, NOP, NULL, 0);
            if (err_code == NRF_SUCCESS)
            {
                m_continuous_transfer.messages_transfered++;
            }
            else if (err_code == NRF_ERROR_RESOURCES)
            {
                /* Intentionally empty. */
            }
            else
            {
                NRF_LOG_ERROR("Message could not be sent.");
            }
        }
    }
}

uint32_t ble_flood_length_set(uint32_t requested_length)
{
    if (BLE_MESSAGE_MAX_BUFFER_LEN < requested_length)
    {
        NRF_LOG_ERROR("Requested flood message length is too big. Maximum value is %d", BLE_MESSAGE_MAX_BUFFER_LEN);
        return m_continuous_transfer.length;
    }

    return m_continuous_transfer.length = (requested_length == 0) ? BLE_MESSAGE_DEFAULT_BUFFER_LEN : requested_length;
}

void ble_flood_autostart_set(bool autostart_enable)
{
    m_autostart = autostart_enable;
}

void ble_flood_stop(void)
{
    if (m_ping_state == CONTINUOUS_MODE)
    {
        uint32_t local_throughput  = m_flood_results.local_results.throughput;
        uint32_t remote_throughput = m_flood_results.remote_results.throughput;

        NRF_LOG_INFO("BLE flood test stopped. Average throughput on BLE: %d.%04d kbps.",
                     local_throughput / BPS_TO_KBPS, local_throughput % BPS_TO_KBPS);
        NRF_LOG_INFO("Average throughput on BLE measured by peer: %d.%04d kbps.",
                     remote_throughput / BPS_TO_KBPS, remote_throughput % BPS_TO_KBPS);

        timer_stop();
        m_ping_state = CONTINUOUS_MODE_STOPPING;
    }
}

/***************************************************************************************************
 * @section Public benchmark API for BLE implementation
 **************************************************************************************************/

benchmark_ble_ping_results_t * benchmark_ble_continuous_results_get(void)
{
    if (m_ping_state != CONTINUOUS_MODE)
    {
        return NULL;
    }
    else
    {
        return &m_flood_results;
    }
}

void benchmark_ble_flood_start(void)
{
    if (m_autostart == true)
    {
        ble_flood_start();
    }
}

void benchmark_ble_flood_stop(void)
{
    if (m_autostart)
    {
        ble_flood_stop();
    }
}
