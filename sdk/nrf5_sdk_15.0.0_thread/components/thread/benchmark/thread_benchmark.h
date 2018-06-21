/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
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
/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#ifndef THREAD_BENCHMARK_H__
#define THREAD_BENCHMARK_H__

#include <stdint.h>
#include <openthread/ip6.h>

#define THREAD_BENCHMARK_MAX_PEER_NUMBER 16 /**< Maximal number of discovered peers that can be stored in the database. */

typedef enum
{
    TEST_START_REQUEST = 0, /**< Notify the receiver that we want to start the test. */
    TEST_STOP_REQUEST,      /**< Notify the receiver that we want to stop the test. */
} thread_benchmark_ctrl_t;

typedef enum
{
    TEST_IN_PROGRESS_FRAME_SENT,                 /**< Frame has been just sent. */
    TEST_IN_PROGRESS_FRAME_SENT_WAITING_FOR_ACK, /**< Frame has been just sent and acknowledgement is expected. */ 
    TEST_IN_PROGRESS_WAITING_FOR_ACK,            /**< Frame has been already sent and we are still waiting for the acknowledgement. */
    TEST_IN_PROGRESS_WAITING_FOR_TX_BUFFER,      /**< No more messagess can be appended to the transmission buffer at this time. */
    TEST_IDLE,                                   /**< No test in progress. */
    TEST_ERROR,                                  /**< Application error occured. */
    TEST_FINISHED,                               /**< Test has just finished. */
} thread_benchmark_test_status_t;

typedef struct
{
    otIp6Address peer_address; /**< IPv6 ML-EID peer address. */
    uint64_t     device_id;    /**< Device ID read from FICR. */
} thread_benchmark_peer_entry_t;

typedef struct
{
    uint16_t                      current_peer_number;                           /**< Number of found peers. */
    thread_benchmark_peer_entry_t peer_table[THREAD_BENCHMARK_MAX_PEER_NUMBER];  /**< Table of found peers. */
} thread_benchmark_peer_information_t;

typedef enum
{
    THREAD_BENCHMARK_MODE_UNIDIRECTIONAL = 0, /**< Transmitter will just send UDP packets to receiver in one direction. */
    THREAD_BENCHMARK_MODE_ECHO,               /**< Receiver replies with the same data back to the transmitter. */
    THREAD_BENCHMARK_MODE_ACK,                /**< Receiver replies with a short information that the data has been received. */
} thread_benchmark_mode_t;

typedef struct
{
    uint16_t                length;      /**< Length of the UDP payload. */
    uint16_t                ack_timeout; /**< Timeout used in ACK and ECHO modes */
    uint32_t                count;       /**< Number of packets to send. */
    thread_benchmark_mode_t mode;        /**< Receiver mode. */
} thread_benchmark_configuration_t;

typedef struct
{
    bool                            test_in_progress;   /**< Indicates that the test is ongoing. */
    uint32_t                        acks_lost;          /**< Total number of acknowledgements lost. */
    uint32_t                        waiting_for_ack;    /**< Frame number for which acknowledgement is awaited. */
    uint32_t                        packets_left_count; /**< Number of sent packets. */
    uint32_t                        frame_number;       /**< Number of the test frame included in the payload. */ 
    thread_benchmark_peer_entry_t * p_selected_peer;    /**< Peer which serves as the receiver for unicast transmission. */
} thread_benchmark_status_t;

typedef struct
{
    uint32_t bytes_received;   /**< Total bytes of UDP payload received [B]. */
    uint32_t packets_received; /**< Total number of UDP test packets received. */
    uint32_t rx_error;         /**< Total number of incorrectly received frames. */
    uint32_t rx_total;         /**< Total number of correctly received frames. */
} thread_benchmark_rx_counters_t;

typedef struct
{
    uint32_t                       duration;   /**< Test duration [ms]. */
    float                          per;        /**< UDP Packet Error rate [%]. */
    float                          throughput; /**< UDP throughput [B/s]. */
    thread_benchmark_rx_counters_t rx_counters; /**< Counter values from peer that are transferred to the local node afer the test. */
} thread_benchmark_result_t;

typedef union
{
    thread_benchmark_result_t           * p_result;           /**< Pointer to the test result structure. */
    thread_benchmark_peer_information_t * p_peer_information; /**< Pointer to the peer information. */
    otError                               error;              /**< Error related to the last operation. OT_ERROR_NONE in case of success. */
} thread_benchmark_event_context_t;

typedef enum
{
    THREAD_BENCHMARK_TEST_COMPLETED = 0,  /**< Test completed event, takes place after results reception and throughput/per calculation. */
    THREAD_BENCHMARK_TEST_STARTED,        /**< Test started event, transmission is starting. */
    THREAD_BENCHMARK_TEST_STOPPED,        /**< Test stopped event, peer has been sent the test stop control message. */
    THREAD_BENCHMARK_DISCOVERY_COMPLETED, /**< Discovery completed event, local node should receive discovery responses from available peers by now. */
} thread_benchmark_event_t;

typedef struct
{
    thread_benchmark_event_t         evt;     /**< Thread benchmark event. */
    thread_benchmark_event_context_t context; /**< Additional event information to be used in application. */
} thread_benchmark_evt_t;

/**@brief Type definition of the the benchmark callback.
 *
 * @param[in] p_evt Pointer to the event structure.
 */
typedef void (*thread_benchmark_callback_t)(thread_benchmark_evt_t * p_evt);

/**************************************************************************************************
 * Public API.
 *************************************************************************************************/

/**@brief   Function for initializing the CoAP communication used for the peer test control. */
void thread_benchmark_init(void);

/**@brief   Function for initializing the temporary test setting before the run.
 *
 * @param[in] callback    Callback function to asonchrynously inform the application about the benchmark state.
 */
void thread_benchmark_test_init(thread_benchmark_callback_t callback);

/**@brief   Function for discovering peers that can be used as receiver during the test.
 *
 * @retval OT_ERROR_NONE    If the request has been succesfully sent.
 */
otError thread_benchmark_peer_discover(void);

/**@brief   Function that returns table pointer of found peers which can serve as the receiver during the test.
 *
 * @retval A pointer to the Thread benchmark peer information structure.
 */
const thread_benchmark_peer_information_t * thread_benchmark_peer_table_get(void);

/**@brief   Function that returns test configuration structure pointer.
 *
 * @retval A pointer to the Thread benchmark configuration structure.
 */
thread_benchmark_configuration_t * thread_benchmark_configuration_get(void);

/**@brief   Function that returns test status structure pointer.
 *
 * @retval A pointer to the Thread benchmark status structure.
 */
thread_benchmark_status_t * thread_benchmark_status_get(void);

/**@brief   Function that processess the test. */
void thread_benchmark_process(void);

/**@brief   Function to set peer to serve as the receiver during the test.
 *
 * @param[in] peer_index    Index of peer in the discovered peer table to be used during the test.
 */
void thread_benchmark_peer_select(uint16_t peer_index);

/**@brief   Function to send the control command to the peer.
 *
 * @param[in] cmd    Thread benchmark control command to be sent to peer.
 *
 * @retval OT_ERROR_NONE    If the request has been succesfully sent.
 */
otError thread_benchmark_peer_ctrl_request_send(thread_benchmark_ctrl_t cmd);

/**@brief   Function that returns Device ID read from FICR.
 *
 * @retval Device ID read from the FICR register.
 */
uint64_t thread_benchmark_local_device_id_get(void);

/**@brief   Function that returns peer that is selected as receiver during the test.
 *
 * @retval Index of the current peer in the table of discovered peers.
 */
uint16_t thread_benchmark_peer_selected_id_get(void);

/**@brief   Function to send result request to the peer.
 *
 * @retval OT_ERROR_NONE    If the request has been succesfully sent.
 */
otError thread_benchmark_results_request_send(void);

/**@brief   Function returning selected peer pointer.
 *
 * @retval A pointer to the Thread benchmark peer entry structure of the selected peer.
 */
const thread_benchmark_peer_entry_t * thread_benchmark_peer_selected_get(void);

/**************************************************************************************************
 * Internal API.
 *************************************************************************************************/

/**@brief   Function that returns test results structure pointer.
 *
 * @retval A pointer to the Thread benchmark result structure.
 */
thread_benchmark_result_t * thread_benchmark_result_get(void);

/**@brief   Function that triggers calculation of the test duration */
void thread_benchmark_test_duration_calculate(void);

#endif // THREAD_BENCHMARK_H__

/** @}
 *  @endcond
 */
