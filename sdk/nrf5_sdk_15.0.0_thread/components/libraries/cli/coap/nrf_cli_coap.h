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
#ifndef NRF_CLI_COAP_H__
#define NRF_CLI_COAP_H__

#include "app_util.h"
#include "nordic_common.h"
#include "nrf_cli.h"
#include "nrf_ringbuf.h"

#include <openthread/openthread.h>
#include <openthread/coap.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@file
 *
 * @defgroup nrf_cli_coap CoAP command line interface transport layer
 * @ingroup nrf_cli
 *
 * @{
 *
 */

#define NRF_CLI_COAP_MAX_INSTANCES 16

/**
 * @brief Command line interface transport.
 */
extern const nrf_cli_transport_api_t nrf_cli_coap_transport_api;

typedef struct nrf_cli_coap_internal_s nrf_cli_coap_internal_t;

typedef struct {
    otCoapResource m_cmd_resource;
    otCoapResource m_resp_resource;
} nrf_cli_coap_resources_t;

/**
 * @brief CLI CoAP transport control block structure.
 */
typedef struct {
    nrf_cli_transport_handler_t handler;       //!< Event handler.
    void                      * p_context;     //!< User context.
    otInstance                * p_ot_instance; //!< OpenThread instance.
    otIp6Address                peer_address;
    nrf_cli_coap_internal_t   * p_next;
} nrf_cli_coap_internal_cb_t;

/**
 * @brief CLI CoAP transport instance structure.
 */
struct nrf_cli_coap_internal_s {
    nrf_cli_transport_t          transport;    //!< Transport structure.
    nrf_cli_coap_internal_cb_t * p_cb;         //!< Pointer to the instance control block.
    nrf_ringbuf_t        const * p_rx_ringbuf; //!< Ringbuf for received commands.
};

/**@brief CLI CoAP transport definition. */
#define NRF_CLI_COAP_DEF(_name_, _rx_buf_sz_)                     \
    NRF_RINGBUF_DEF(CONCAT_2(_name_,_rx_ringbuf), _rx_buf_sz_);   \
    static nrf_cli_coap_internal_cb_t CONCAT_2(_name_, _cb);      \
    static const nrf_cli_coap_internal_t _name_ = {               \
        .transport = {.p_api = &nrf_cli_coap_transport_api},      \
        .p_cb = &CONCAT_2(_name_, _cb),                           \
        .p_rx_ringbuf = &CONCAT_2(_name_,_rx_ringbuf),            \
    }

/**@brief Create indexed name from name and indexed. */
#define NRF_CLI_NAME_IDX(_idx_, _name_)                           \
    CONCAT_3(_name_, _, _idx_)

/**
 * @brief Helper macro for creating array of
 * pointers to name indexed items.
 */
#define NRF_CLI_NAME_IDX_LIST(_idx_, _name_)                      \
    &NRF_CLI_NAME_IDX(_idx_, _name_),

/**@brief CLI CoAP transport definition with indexed name. */
#define NRF_CLI_COAP_DEF_IDX(_idx_, _name_, _rx_buf_sz_)          \
    NRF_CLI_COAP_DEF(NRF_CLI_NAME_IDX(_idx_, _name_),             \
                     _rx_buf_sz_);

/**@brief Multiple CLI CoAP transport definition. */
/*lint -emacro(10, NRF_CLI_COAP_DEF_MULTI) */
#define NRF_CLI_COAP_DEF_MULTI(_cnt_, _name_, _rx_buf_sz_)        \
    MACRO_REPEAT_FOR(_cnt_,                                       \
                     NRF_CLI_COAP_DEF_IDX,                        \
                     _name_,                                      \
                     _rx_buf_sz_) 


/**@brief CLI definition with indexed name. */
#define NRF_CLI_DEF_IDX(_idx_,                                    \
                        _name_,                                   \
                        _cli_prefix_,                             \
                        _p_instance_,                             \
                        _newline_ch_,                             \
                        _log_queue_size_)                         \
    NRF_CLI_DEF(NRF_CLI_NAME_IDX(_idx_, _name_),                  \
                _cli_prefix_,                                     \
                &NRF_CLI_NAME_IDX(_idx_, _p_instance_).transport, \
                _newline_ch_,                                     \
                _log_queue_size_);

/**@brief Multiple CLI definition. */
/*lint -emacro(10 19 40 119 505 533 534, NRF_CLI_DEF_MULTI) */
#define NRF_CLI_DEF_MULTI(_cnt_,                                  \
                          _name_,                                 \
                          _cli_prefix_,                           \
                          _p_instance_,                           \
                          _newline_ch_,                           \
                          _log_queue_size_)                       \
    MACRO_REPEAT_FOR(_cnt_,                                       \
                     NRF_CLI_DEF_IDX,                             \
                     _name_,                                      \
                     _cli_prefix_,                                \
                     _p_instance_,                                \
                     _newline_ch_,                                \
                     _log_queue_size_);                           \
    static nrf_cli_t const * const _name_[] = {                   \
        MACRO_REPEAT_FOR(_cnt_, NRF_CLI_NAME_IDX_LIST, _name_)    \
    }

/**
 * @brief   Sends data as CLI command to remote peer.
 *
 * @param[in] p_cli  Pointer to the CoAP CLI instance.
 * @param[in] p_data Pointer to data to be sent as command to remote.
 * @param[in] length Size of data to be sent as command to remote.
 */
void cli_coap_send_cmd_to_peer(const nrf_cli_t  * p_cli,
                               const void       * p_data,
                               size_t             length);

/**
 * @brief   Set IPv6 address of remote peer.
 *
 * @param[in] p_cli          Pointer to the CoAP CLI instance.
 * @param[in] p_peer_address Pointer to IPv6 address of remote.
 */
void cli_coap_peer_set(const nrf_cli_t    * p_cli,
                       const otIp6Address * p_peer_address);

/**
 * @brief   Set output CLI for responses received from remote peer.
 *
 * @param[in] p_cli Pointer to the CLI instance on which remote output should appear.
 */
void cli_coap_response_cli_set(const nrf_cli_t * p_cli);

/** @} */


#ifdef __cplusplus
}
#endif

#endif /* NRF_CLI_COAP_H__ */
