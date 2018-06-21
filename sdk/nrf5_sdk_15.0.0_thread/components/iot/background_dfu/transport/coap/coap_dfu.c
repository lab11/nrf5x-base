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

/** @file
 *
 * @defgroup background_dfu_coap_transport background_dfu_coap.c
 * @{
 * @ingroup background_dfu
 * @brief Background DFU CoAP transport implementation.
 *
 */

#include "background_dfu_transport.h"
#include "coap_dfu.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "sdk_config.h"
#include "boards.h"
#include "app_util.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "nrf_delay.h"
#include "nrf_error.h"
#include "nrf_log_ctrl.h"
#include "nrf_dfu_settings.h"
#include "nrf_dfu_req_handler.h"
#include "nrf_dfu_utils.h"
#include "nordic_common.h"
#include "iot_errors.h"
#include "coap_api.h"
#include "coap_block.h"
#include "coap_option.h"
#include "background_dfu_block.h"
#include "openthread/platform/random.h"

#define NRF_LOG_LEVEL 4
#define NRF_LOG_MODULE_NAME COAP_DFU
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/** @brief Maximum delay (in ms) between requesting consecutive image blocks. */
#define DEFAULT_DELAY_MAX_MS    128

#define REALM_LOCAL_ADDR        (uint8_t []){0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}

#ifndef DFU_DEBUG_RANDOM_INIT_BLOCK_LOSS_ENABLE
#define DFU_DEBUG_RANDOM_INIT_BLOCK_LOSS_ENABLE  0  /**< Enable random init block loss for debugging purposes. */
#endif

#ifndef DFU_DEBUG_RANDOM_IMAGE_BLOCK_LOSS_ENABLE
#define DFU_DEBUG_RANDOM_IMAGE_BLOCK_LOSS_ENABLE 0  /**< Enable random image block loss for debugging purposes. */
#endif

#ifndef DFU_DEBUG_RANDOM_BLOCK_LOSS_RATE
#define DFU_DEBUG_RANDOM_BLOCK_LOSS_RATE         50 /**< Block loss rate, in percent.*/
#endif

#define DFU_RESOURCE_PREFIX     "dfu"

#define INIT_RESOURCE_NAME      "i"
#define IMAGE_RESOURCE_NAME     "f"
#define TRIGGER_RESOURCE_NAME   "t"
#define DIAG_RESOURCE_NAME      "d"
#define BITMAP_RESOURCE_NAME    "b"
#define RESET_RESOURCE_NAME     "r"
#define RESOURCE_PATH(name)     (DFU_RESOURCE_PREFIX "/" name)

#define SCHED_QUEUE_SIZE      32                                                              /**< Maximum number of events in the scheduler queue. */
#define SCHED_EVENT_DATA_SIZE MAX(sizeof(nrf_dfu_request_t), APP_TIMER_SCHED_EVENT_DATA_SIZE) /**< Maximum app_scheduler event size. */

typedef struct
{
    coap_resource_t          root;            /**< Root resource. */
    coap_resource_t          prefix;          /**< Prefix resource. */
    coap_resource_t          trigger;         /**< Trigger resource. */
    coap_resource_t          init;            /**< Init resource. Used in multicast DFU transfer. */
    coap_resource_t          image;           /**< Image resource. Used in multicast DFU transfer. */
    coap_resource_t          diagnostic;      /**< Diagnostic resource. */
    coap_resource_t          reset;           /**< Reset resource. */

    coap_response_callback_t handler;         /**< A pointer to the current response handler. */
    const char             * p_resource_path; /**< Downloaded resource path on the remote host. */
    coap_remote_t            remote;          /**< Remote host from which the resource is being downloaded. */
    bool                     timer_active;    /**< True if a CoAP request is pending, false otherwise. */
} coap_dfu_context_t;

/** @brief Coap Message Id. */
static uint16_t m_coap_message_id = 0;

/** @brief CoAP token. */
static uint16_t m_coap_token;

/** @brief DFU client context. */
static background_dfu_context_t m_dfu_ctx;
static coap_dfu_context_t       m_coap_dfu_ctx;

/** @brief Request delay timer. */
APP_TIMER_DEF(m_send_timer);
APP_TIMER_DEF(m_reset_timer);

static void reset_application(void)
{
    NRF_LOG_FINAL_FLUSH();

#if NRF_MODULE_ENABLED(NRF_LOG_BACKEND_RTT)
    // To allow the buffer to be flushed by the host.
    nrf_delay_ms(100);
#endif

    NVIC_SystemReset();
}

/** @brief Check if given address is multicast.
 *
 *  @param[in] p_remote A pointer to the address.
 *
 *  @return True if address is a multicast, false otherwise.
 **/
static bool is_addr_multicast(const coap_remote_t * p_remote)
{
    return p_remote->addr[0] == 0xff;
}

/**
 * @brief Function notifies certain events in DFU process.
 */
static void dfu_observer(nrf_dfu_evt_type_t evt_type)
{
    switch (evt_type)
    {
        case NRF_DFU_EVT_DFU_COMPLETED:
            if (!m_dfu_ctx.reset_suppress)
            {
                NRF_LOG_INFO("Reset after DFU");
                reset_application();
            }
            else
            {
                NRF_LOG_INFO("Automatic reset suppressed by the server");
            }
            break;

        default:
            break;
    }
}

/***************************************************************************************************
 * @section Block processing
 **************************************************************************************************/

/** @brief Parse and return CoAP Block1 option from a given message.
 *
 *  @param[in]  p_message        A pointer to CoAP message.
 *  @param[out] p_block1_option  A pointer to parsed Block1 option.
 *
 *  @return True if message has Block1 option, false otherwise.
 */
static bool get_block1_opt(const coap_message_t    * p_message,
                           coap_block_opt_block1_t * p_block1_option)
{
    uint32_t        err_code;
    uint8_t         option_index;
    uint32_t        option_value;
    coap_option_t * p_option;

    if (coap_message_opt_index_get(&option_index,
                                   (coap_message_t *)p_message,
                                   COAP_OPT_BLOCK1) != NRF_SUCCESS)
    {
        return false;
    }

    p_option = (coap_option_t *)&p_message->options[option_index];
    err_code = coap_opt_uint_decode(&option_value,
                                    p_option->length,
                                    p_option->p_data);

    if (err_code != NRF_SUCCESS)
    {
        return false;
    }

    err_code = coap_block_opt_block1_decode(p_block1_option, option_value);

    return (err_code == NRF_SUCCESS);
}

/** @brief Parse and return CoAP block option from a given message.
 *
 *  @param[in]  p_message        A pointer to CoAP message.
 *  @param[out] p_block2_option  A pointer to parsed block 2 option.
 *
 *  @return True if message has Block2 option, false otherwise.
 */
static bool get_block2_opt(const coap_message_t    * p_message,
                           coap_block_opt_block2_t * p_block2_option)
{
    uint32_t        err_code;
    uint8_t         option_index;
    uint32_t        option_value;
    coap_option_t * p_option;

    if (coap_message_opt_index_get(&option_index,
                                   (coap_message_t *)p_message,
                                   COAP_OPT_BLOCK2) != NRF_SUCCESS)
    {
        return false;
    }

    p_option = (coap_option_t * )&p_message->options[option_index];
    err_code = coap_opt_uint_decode(&option_value,
                                    p_option->length,
                                    p_option->p_data);

    if (err_code != NRF_SUCCESS)
    {
        return false;
    }

    err_code = coap_block_opt_block2_decode(p_block2_option, option_value);

    return (err_code == NRF_SUCCESS);
}

/** @brief Set CoAP block2 option on a given message.
 *
 *  @param[inout]  p_message    A pointer to CoAP message.
 *  @param[in]     block_size   Block size to set.
 *  @param[in]     block_number Block number to set.
 *
 *  @return NRF_SUCCESS if succesful, error code otherwise.
 */
static uint32_t set_block2_opt(coap_message_t * p_message,
                               uint16_t         block_size,
                               uint32_t         block_number)
{
    uint32_t err_code;
    uint32_t option_value;

    coap_block_opt_block2_t block2_option =
    {
        .number = block_number,
        .size   = block_size
    };

    err_code = coap_block_opt_block2_encode(&option_value, &block2_option);
    if (err_code == NRF_SUCCESS)
    {
        err_code = coap_message_opt_uint_add(p_message, COAP_OPT_BLOCK2, option_value);
    }

    return err_code;
}

/***************************************************************************************************
 * @section Response handling
 **************************************************************************************************/

/**@brief Check status and common CoAP parameters for a response.
 *
 * @param[in] status     Status code indicating if response was received.
 * @param[in] p_response A response recevied. May be invalid if status is not NRF_SUCCESS.
 *
 * @return True if a valid response was received, false otherwise.
 */
static bool is_valid_response_received(uint32_t status, const coap_message_t * p_response)
{
    if (status != NRF_SUCCESS)
    {
        if (status == COAP_TRANSMISSION_TIMEOUT)
        {
            NRF_LOG_WARNING("Request timeout");
        }
        else
        {
            NRF_LOG_WARNING("Unknown error");
        }

        return false;
    }

    if (p_response->header.code != COAP_CODE_205_CONTENT)
    {
        NRF_LOG_WARNING("Request response code: %d", p_response->header.code);

        return false;
    }

    if ((p_response->header.token_len != sizeof(m_coap_token)) ||
        (memcmp(p_response->token, &m_coap_token, p_response->header.token_len) != 0))
    {
        NRF_LOG_WARNING("Token mismatch");

        return false;
    }

    return true;
}

/**@brief CoAP response handler for request sent to a trigger resource (@ref TRIGGER_RESOURCE_NAME).
 *
 * An implementation of @ref coap_response_callback_t function type.
 */
static void handle_trigger_response(uint32_t status, void * p_arg, coap_message_t * p_response)
{
    background_dfu_context_t * p_dfu_ctx = (background_dfu_context_t *)p_arg;

    if (p_dfu_ctx->dfu_state != BACKGROUND_DFU_DOWNLOAD_TRIG)
    {
        NRF_LOG_WARNING("Token response callback called in invalid state (s:%s)",
                (uint32_t)background_dfu_state_to_string(p_dfu_ctx->dfu_state));
        return;
    }

    if (!is_valid_response_received(status, p_response))
    {
        background_dfu_handle_event(&m_dfu_ctx, BACKGROUND_DFU_EVENT_TRANSFER_ERROR);
        return;
    }

    if (background_dfu_validate_trigger(&m_dfu_ctx, p_response->p_payload, p_response->payload_len))
    {
        m_coap_dfu_ctx.remote = p_response->remote;
        background_dfu_process_trigger(&m_dfu_ctx, p_response->p_payload, p_response->payload_len);
    }
}

/**@brief CoAP response handler for request sent to an init (@ref INIT_RESOURCE_NAME) or an image (@ref IMAGE_RESOURCE_NAME) resource.
 *
 * An implementation of @ref coap_response_callback_t function type.
 */
static void handle_block_response(uint32_t status, void * p_arg, coap_message_t * p_response)
{
    coap_block_opt_block2_t    block_opt = {0};
    background_dfu_context_t * p_dfu_ctx = (background_dfu_context_t *)p_arg;

    do
    {
        if ((p_dfu_ctx->dfu_state != BACKGROUND_DFU_DOWNLOAD_INIT_CMD) &&
            (p_dfu_ctx->dfu_state != BACKGROUND_DFU_DOWNLOAD_FIRMWARE))
        {
            NRF_LOG_WARNING("Block response callback called in invalid state (s:%s)",
                    (uint32_t)background_dfu_state_to_string(p_dfu_ctx->dfu_state));
            return;
        }

        if (!is_valid_response_received(status, p_response))
        {
            background_dfu_handle_event(&m_dfu_ctx, BACKGROUND_DFU_EVENT_TRANSFER_ERROR);
            break;
        }

        if (!get_block2_opt(p_response, &block_opt))
        {
            NRF_LOG_WARNING("No block 2 option in response message.");
            background_dfu_handle_event(p_dfu_ctx, BACKGROUND_DFU_EVENT_TRANSFER_CONTINUE);
            break;
        }

        NRF_LOG_DEBUG("Received block %3lu", block_opt.number);

        if (block_opt.number != p_dfu_ctx->block_num)
        {
            NRF_LOG_WARNING("Requested %d but got %d", p_dfu_ctx->block_num, block_opt.number);
            background_dfu_handle_event(p_dfu_ctx, BACKGROUND_DFU_EVENT_TRANSFER_CONTINUE);
            break;
        }

        background_dfu_block_t block;
        block.number    = block_opt.number;
        block.size      = block_opt.size;
        block.p_payload = p_response->p_payload;

        background_dfu_process_block(p_dfu_ctx, &block);
    } while (0);
}

/***************************************************************************************************
 * @section Message helpers
 **************************************************************************************************/

/** @brief Set CoAP URI option on a given message.
 *
 *  @param[inout] p_message  A pointer to CoAP message.
 *  @param[in]    p_resource An URI as NULL terminated string which should be converted to CoAP options.
 *
 *  @return NRF_SUCCESS if succesful, error code otherwise.
 */
static uint32_t set_uri_opt(coap_message_t * p_message, const char * p_resource)
{
    uint32_t     err_code;
    const char * p_start = p_resource;
    const char * p_end;
    uint32_t     len;

    if (*p_start == '/')
    {
        p_start++;
    }

    do {
        p_end = strchr(p_start, '/');
        if (p_end)
        {
            len = p_end - p_start;
        }
        else
        {
            len = strlen(p_start);
        }

        err_code = coap_message_opt_str_add(p_message,
                                            COAP_OPT_URI_PATH,
                                            (uint8_t *) p_start,
                                            len);
        p_start += (len + 1);

    } while ((err_code == NRF_SUCCESS) && p_end);

    return err_code;
}

/**@brief Get next message id.
 *
 * @return An unique message id.
 */
static uint16_t message_id_get(void)
{
    return m_coap_message_id++;
}

/**@brief Create a CoAP message with specific payload and remote.
 *
 * @param[in] p_conf      A pointer to the message configuration.
 * @param[in] p_resource  A pointer to a string with resource name.
 * @param[in] p_payload   A pointer to the message payload. NULL if message shall not contain any payload.
 * @param[in] payload_len Payload length in bytes, ignored if p_payload is NULL.
 * @param[in] p_remote    An address of the remote endpoint.
 *
 * @return A pointer to the message created or NULL if could not be created.
 */
static coap_message_t * message_create(const coap_message_conf_t * p_conf,
                                       const char                * p_resource,
                                       const uint8_t             * p_payload,
                                       uint16_t                    payload_len,
                                       const coap_remote_t       * p_remote)
{
    uint32_t         err_code;
    coap_message_t * p_message = NULL;

    do
    {
        err_code = coap_message_new(&p_message, (coap_message_conf_t *)p_conf);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_message_remote_addr_set(p_message, (coap_remote_t *)p_remote);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        if (p_resource != NULL)
        {
            err_code = set_uri_opt(p_message, p_resource);
            if (err_code != NRF_SUCCESS)
            {
                break;
            }
        }

        if (p_payload != NULL)
        {
            err_code = coap_message_payload_set(p_message, (void *)p_payload, payload_len);
            if (err_code != NRF_SUCCESS)
            {
                break;
            }
        }
    } while (0);

    if ((err_code != NRF_SUCCESS) && (p_message != NULL))
    {
        coap_message_delete(p_message);
        p_message = NULL;
    }

    return p_message;
}

/**@brief Create an empty reset response.
 *
 * @param[in] p_request A pointer to the request.
 *
 * @return A pointer to the reset response or NULL if could not be created.
 */
static coap_message_t * empty_reset_response_create(const coap_message_t * p_request)
{
    coap_message_conf_t message_conf;

    memset(&message_conf, 0, sizeof(message_conf));

    message_conf.type             = COAP_TYPE_RST;
    message_conf.code             = COAP_CODE_EMPTY_MESSAGE;
    message_conf.id               = p_request->header.id;
    message_conf.port.port_number = DFU_UDP_PORT;

    return message_create(&message_conf, NULL, NULL, 0, &p_request->remote);
}

/**@brief Create a response with diag resource.
 *
 * @param[in] type      A message type of the response.
 * @param[in] code      A message code to send back.
 * @param[in] p_request A pointer to the request.
 *
 * @return A pointer to the response message or NULL if could not be created.
 */
static coap_message_t * diagnostic_response_create(coap_msg_type_t         type,
                                                   coap_msg_code_t         code,
                                                   const coap_message_t  * p_request)
{
    coap_message_conf_t message_conf;

    memset(&message_conf, 0, sizeof(message_conf));

    message_conf.type             = type;
    message_conf.code             = code;
    message_conf.port.port_number = DFU_UDP_PORT;
    message_conf.token_len        = p_request->header.token_len;
    memcpy(message_conf.token, p_request->token, message_conf.token_len);

    if (type == COAP_TYPE_ACK)
    {
        message_conf.id = p_request->header.id;
    }
    else
    {
        message_conf.id = message_id_get();
    }

    // Create a copy and invert endianess.
    background_dfu_diagnostic_t diag = m_dfu_ctx.dfu_diag;

    diag.state                       = m_dfu_ctx.dfu_state;
    diag.build_id                    = uint32_big_decode((const uint8_t *)&diag.build_id);
    diag.image_blocks_requested      = uint16_big_decode((const uint8_t *)&diag.image_blocks_requested);
    diag.init_blocks_requested       = uint16_big_decode((const uint8_t *)&diag.init_blocks_requested);
    diag.total_image_blocks_received = uint16_big_decode((const uint8_t *)&diag.total_image_blocks_received);
    diag.total_init_blocks_received  = uint16_big_decode((const uint8_t *)&diag.total_init_blocks_received);
    diag.triggers_received           = uint16_big_decode((const uint8_t *)&diag.triggers_received);


    return message_create(&message_conf,
                          NULL,
                          (uint8_t *)&diag,
                          sizeof(diag),
                          &p_request->remote);
}


/**@brief A function for sending CoAP messages.
 *
 * @param[inout] p_request A pointer to CoAP message which should be sent.
 */
static void coap_dfu_message_send(coap_message_t * p_message)
{
    uint32_t handle;

    NRF_LOG_DEBUG("Sending message [mid:%d]", p_message->header.id);

    if (coap_message_send(&handle, p_message) != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Failed to send CoAP message");
    }

    coap_message_delete(p_message);
}

/**@brief A function for sending delayed CoAP messages.
 *
 * Maximum delay is specified by @ref DEFAULT_DELAY_MAX_MS.
 * In case that another message is pending for sending, the message is dropped.
 *
 * @param[inout] p_request A pointer to CoAP message which should be sent.
 */
static void coap_dfu_delayed_message_send(coap_message_t * p_message)
{
    if (!m_coap_dfu_ctx.timer_active)
    {
        uint8_t delay = (background_dfu_random() % DEFAULT_DELAY_MAX_MS) + 1;

        app_timer_start(m_send_timer, APP_TIMER_TICKS(delay), p_message);
        m_coap_dfu_ctx.timer_active = true;
    }
    else
    {
        // Drop the message in case another message is pending for transmission.
        coap_message_delete(p_message);
    }
}

/***************************************************************************************************
 * @section Resources
 **************************************************************************************************/

/**@brief A request callback for /dfu/t resource.
 *
 * This method is called when a request to trigger resource is received.
 * An implementation of @ref coap_method_callback_t function type.
 *
 * @param[in] p_resource A pointer to resource.
 * @param[in] p_request  A pointer to request.
 */
static void trigger_request_callback(coap_resource_t * p_resource, coap_message_t * p_request)
{
    NRF_LOG_INFO("DFU trigger request received");

    if (p_request->header.code != COAP_CODE_POST)
    {
        NRF_LOG_INFO("Trigger request: method not supported");
        return;
    }

    m_dfu_ctx.dfu_diag.triggers_received++;

    if (background_dfu_validate_trigger(&m_dfu_ctx, p_request->p_payload, p_request->payload_len))
    {
        m_coap_dfu_ctx.remote = p_request->remote;
        background_dfu_process_trigger(&m_dfu_ctx, p_request->p_payload, p_request->payload_len);
    }
}

/**@brief A request callback for /dfu/i resource.
 *
 * An implementation of @ref coap_method_callback_t function type.
 *
 * @param[in] p_resource A pointer to resource.
 * @param[in] p_request  A pointer to request.
 */
static void init_request_callback(coap_resource_t * p_resource, coap_message_t * p_request)
{
    coap_message_t          * p_response = NULL;
    coap_block_opt_block1_t   block_opt  = {0};

    do
    {
        // Resource only active in multicast mode firmware download state.
        if ((m_dfu_ctx.dfu_mode != BACKGROUND_DFU_MODE_MULTICAST) ||
            (m_dfu_ctx.dfu_state != BACKGROUND_DFU_DOWNLOAD_INIT_CMD))
        {
            NRF_LOG_WARNING("Init data received in invalid state: (m:%d s:%d).",
                            m_dfu_ctx.dfu_mode, m_dfu_ctx.dfu_state);

            if (!is_addr_multicast(&p_request->local))
            {
                p_response = empty_reset_response_create(p_request);
            }

            break;
        }

        if (!get_block1_opt(p_request, &block_opt))
        {
            NRF_LOG_WARNING("No block 1 option in init request message.");
        }
        else
        {
            background_dfu_block_t block;
            block.number    = block_opt.number;
            block.size      = block_opt.size;
            block.p_payload = p_request->p_payload;

#if (DFU_DEBUG_RANDOM_INIT_BLOCK_LOSS_ENABLE == 1)
            if ((background_dfu_random() % 100) >= DFU_DEBUG_RANDOM_BLOCK_LOSS_RATE)
            {
                background_dfu_process_block(&m_dfu_ctx, &block);
            }
#else  // DFU_DEBUG_RANDOM_INIT_BLOCK_LOSS_ENABLE
            background_dfu_process_block(&m_dfu_ctx, &block);
#endif // DFU_DEBUG_RANDOM_INIT_BLOCK_LOSS_ENABLE
        }

        m_dfu_ctx.dfu_diag.total_init_blocks_received++;

    } while (0);

    // Send response, if created.
    if (p_response != NULL)
    {
        coap_dfu_message_send(p_response);
    }
}

/**@brief A request callback for /dfu/f resource.
 *
 * An implementation of @ref coap_method_callback_t function type.
 *
 * @param[in] p_resource A pointer to resource.
 * @param[in] p_request  A pointer to request.
 */
static void image_request_callback(coap_resource_t * p_resource, coap_message_t * p_request)
{
    coap_message_t          * p_response = NULL;
    coap_block_opt_block1_t   block_opt = {0};

    do
    {
        // Resource only active in multicast mode firmware download state.
        if ((m_dfu_ctx.dfu_mode != BACKGROUND_DFU_MODE_MULTICAST) ||
            (m_dfu_ctx.dfu_state != BACKGROUND_DFU_DOWNLOAD_FIRMWARE))
        {
            NRF_LOG_WARNING("Image data received in invalid state: (m:%d s:%d).",
                            m_dfu_ctx.dfu_mode, m_dfu_ctx.dfu_state);

            if (!is_addr_multicast(&p_request->local))
            {
                p_response = empty_reset_response_create(p_request);
            }

            break;
        }

        if (!get_block1_opt(p_request, &block_opt))
        {
            NRF_LOG_WARNING("No block 1 option in image request message.");
        }
        else
        {
            background_dfu_block_t block;
            block.number    = block_opt.number;
            block.size      = block_opt.size;
            block.p_payload = p_request->p_payload;

#if (DFU_DEBUG_RANDOM_IMAGE_BLOCK_LOSS_ENABLE == 1)
            if ((background_dfu_random() % 100) >= DFU_DEBUG_RANDOM_BLOCK_LOSS_RATE)
            {
                background_dfu_process_block(&m_dfu_ctx, &block);
            }
#else  // DFU_DEBUG_RANDOM_IMAGE_BLOCK_LOSS_ENABLE
            background_dfu_process_block(&m_dfu_ctx, &block);
#endif // DFU_DEBUG_RANDOM_IMAGE_BLOCK_LOSS_ENABLE
        }

        m_dfu_ctx.dfu_diag.total_image_blocks_received++;

    } while (0);

    // Send response, if created.
    if (p_response != NULL)
    {
        coap_dfu_message_send(p_response);
    }
}

/**@brief A request callback for /dfu/d resource.
 *
 * An implementation of @ref coap_method_callback_t function type.
 *
 * @param[in] p_resource A pointer to resource.
 * @param[in] p_request  A pointer to request.
 */
static void diagnostic_request_callback(coap_resource_t * p_resource, coap_message_t * p_request)
{
    coap_message_t * p_response = NULL;

    // Process only CON messages.
    if (p_request->header.type == COAP_TYPE_CON)
    {
        p_response = diagnostic_response_create(COAP_TYPE_ACK, COAP_CODE_205_CONTENT, p_request);
    }
    else
    {
        p_response = diagnostic_response_create(COAP_TYPE_NON, COAP_CODE_205_CONTENT, p_request);
    }

    // Send response, if created.
    if (p_response != NULL)
    {
        coap_dfu_delayed_message_send(p_response);
    }
}

/**@brief A request callback for /dfu/r resource.
 *
 * An implementation of @ref coap_method_callback_t function type.
 *
 * @param[in] p_resource A pointer to resource.
 * @param[in] p_request  A pointer to request.
 */
static void reset_request_callback(coap_resource_t * p_resource, coap_message_t * p_request)
{
    uint32_t reset_delay_ms;

    do
    {
        if (m_dfu_ctx.dfu_state != BACKGROUND_DFU_WAIT_FOR_RESET)
        {
            NRF_LOG_WARNING("Reset command received in invalid state: (m:%d s:%d).",
                    m_dfu_ctx.dfu_mode, m_dfu_ctx.dfu_state);

            break;
        }

        if (p_request->payload_len != sizeof(uint32_t))
        {
            NRF_LOG_WARNING("Invalid reset command payload length (l:%d).",
                    p_request->payload_len);

            break;
        }

        if (!is_addr_multicast(&p_request->local))
        {
            NRF_LOG_WARNING("Received unicast reset command. Only multicast is allowed.");

            break;
        }

        reset_delay_ms = uint32_big_decode((const uint8_t *)p_request->p_payload);

        app_timer_start(m_reset_timer, APP_TIMER_TICKS(reset_delay_ms), NULL);

    } while (0);
}

/**@brief Initialize DFU Server resources.
 *
 * @param[in] p_dfu_ctx A pointer to DFU client context.
 *
 * @return NRF_SUCCESS on success, otherwise an error code is returned.
 */
static uint32_t endpoints_init(coap_dfu_context_t * p_coap_dfu_ctx)
{
    uint32_t err_code = NRF_SUCCESS;

    do
    {
        err_code = coap_resource_create(&(p_coap_dfu_ctx->root), "/");
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_resource_create(&(p_coap_dfu_ctx->trigger), TRIGGER_RESOURCE_NAME);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        // We allow for GET method, so that the CoAP library doesn't send 'Method not allowed'
        // response. Methods other that POST are rejected in the callback.
        p_coap_dfu_ctx->trigger.permission = (COAP_PERM_POST | COAP_PERM_GET);
        p_coap_dfu_ctx->trigger.callback   = trigger_request_callback;

        err_code = coap_resource_create(&(p_coap_dfu_ctx->init), INIT_RESOURCE_NAME);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        p_coap_dfu_ctx->init.permission = COAP_PERM_PUT;
        p_coap_dfu_ctx->init.callback   = init_request_callback;

        err_code = coap_resource_create(&(p_coap_dfu_ctx->image), IMAGE_RESOURCE_NAME);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        p_coap_dfu_ctx->image.permission = COAP_PERM_PUT;
        p_coap_dfu_ctx->image.callback   = image_request_callback;

        err_code = coap_resource_create(&(p_coap_dfu_ctx->diagnostic), DIAG_RESOURCE_NAME);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        p_coap_dfu_ctx->diagnostic.permission = COAP_PERM_GET;
        p_coap_dfu_ctx->diagnostic.callback   = diagnostic_request_callback;

        err_code = coap_resource_create(&(p_coap_dfu_ctx->reset), RESET_RESOURCE_NAME);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        p_coap_dfu_ctx->reset.permission = COAP_PERM_PUT;
        p_coap_dfu_ctx->reset.callback   = reset_request_callback;

        // Create resource hierarchy.

        err_code = coap_resource_child_add(&(p_coap_dfu_ctx->root), &(p_coap_dfu_ctx->trigger));
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_resource_child_add(&(p_coap_dfu_ctx->root), &(p_coap_dfu_ctx->init));
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_resource_child_add(&(p_coap_dfu_ctx->root), &(p_coap_dfu_ctx->image));
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_resource_child_add(&(p_coap_dfu_ctx->root), &(p_coap_dfu_ctx->diagnostic));
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = coap_resource_child_add(&(p_coap_dfu_ctx->root), &(p_coap_dfu_ctx->reset));
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        NRF_LOG_INFO("Endpoints initialized");
    } while (0);

    return err_code;
}

/***************************************************************************************************
 * @section Timer handlers
 **************************************************************************************************/

static void delayed_send_handler(void * p_context)
{
    coap_dfu_message_send((coap_message_t *)p_context);

    m_coap_dfu_ctx.timer_active = false;
}

static void delayed_reset_handler(void * p_context)
{
    NRF_LOG_INFO("Handling delayed reset");

    reset_application();
}

/***************************************************************************************************
 * @section Private API
 **************************************************************************************************/

/**@brief Create COAP GET request.
 *
 * @param[in] p_remote        A pointer to host.
 * @param[in] p_resource_path An URI of the resource which should be requested.
 * @param[in] block_size      Requested block size.
 * @param[in] block_num       Requested block number.
 * @param[in] callback        A pointer to the callback function that is called on response reception or error.
 *
 * @return A pointer to CoAP message or NULL on error.
 */
static coap_message_t * coap_dfu_create_request(coap_remote_t            * p_remote,
                                                const char               * p_resource_path,
                                                uint16_t                   block_size,
                                                uint32_t                   block_num,
                                                coap_response_callback_t   callback)
{
    uint32_t              err_code = NRF_SUCCESS;
    coap_message_t      * p_request;
    coap_message_conf_t   message_conf;

    do
    {
        memset(&message_conf, 0, sizeof(message_conf));

        if (is_addr_multicast(p_remote))
        {
            message_conf.type = COAP_TYPE_NON;
        }
        else
        {
            message_conf.type = COAP_TYPE_CON;
        }

        message_conf.code              = COAP_CODE_GET;
        message_conf.port.port_number  = DFU_UDP_PORT;
        message_conf.id                = message_id_get();
        message_conf.response_callback = callback;
        message_conf.token_len         = 2;

        m_coap_token = (uint16_t)background_dfu_random();
        memcpy(message_conf.token, &m_coap_token, 2);

        p_request = message_create(&message_conf, p_resource_path, NULL, 0, p_remote);
        if (p_request == NULL)
        {
            break;
        }

        p_request->p_arg = (void *)&m_dfu_ctx;

        if (block_size > 0)
        {
            err_code = set_block2_opt(p_request, block_size, block_num);
            if (err_code != NRF_SUCCESS)
            {
                break;
            }
        }
    } while (0);

    if ((err_code != NRF_SUCCESS) && (p_request != NULL))
    {
        coap_message_delete(p_request);
        p_request = NULL;
    }

    return p_request;
}

/**@brief Count blocks present in a block bitmap.
 *
 * @param[in] p_req_bmp A block bitmap to count.
 *
 * @return Number of blocks counted.
 */
static uint16_t blocks_count(const background_dfu_request_bitmap_t * p_req_bmp)
{
    uint16_t count = 0;

    for (uint8_t i = 0; i < p_req_bmp->size; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((p_req_bmp->bitmap[i] >> j) & 0x01)
            {
                count++;
            }
        }
    }

    return count;
}
/**@brief An implmentation of @ref coap_error_callback_t type. */
static void coap_error_handler(uint32_t error_code, coap_message_t * p_message)
{
    UNUSED_PARAMETER(error_code);
    UNUSED_PARAMETER(p_message);

    // Intentionally empty.
}

/**@brief Initialize CoAP protocol.
 *
 * @param[in] p_context An application specific context.
 *
 * @return NRF_SUCCESS on success, otherwise an error code is returned.
 */
static uint32_t coap_protocol_init(const void * p_context)
{
    uint32_t    err_code;
    coap_port_t local_port_list[COAP_PORT_COUNT] =
    {
        {
            .port_number = DFU_UDP_PORT
        }
    };

    coap_transport_init_t transport_params;
    transport_params.p_port_table = &local_port_list[0];
    transport_params.p_arg        = (void *)p_context;

    NRF_LOG_DEBUG("Init coap");

    m_coap_message_id = background_dfu_random();

    err_code = coap_init(otPlatRandomGet(), &transport_params);

    if (err_code == NRF_SUCCESS)
    {
        err_code = coap_error_handler_register(coap_error_handler);
    }

    return err_code;
}

void background_dfu_transport_block_request_send(background_dfu_context_t        * p_dfu_ctx,
                                                 background_dfu_request_bitmap_t * p_req_bmp)
{
    uint16_big_encode(p_req_bmp->offset, (uint8_t *)&p_req_bmp->offset);

    coap_message_conf_t message_conf;

    memset(&message_conf, 0, sizeof(message_conf));

    NRF_LOG_INFO("Sending block request!");

    message_conf.type             = COAP_TYPE_NON;
    message_conf.code             = COAP_CODE_PUT;
    message_conf.port.port_number = DFU_UDP_PORT;
    message_conf.id               = message_id_get();

    const char * p_resource_name  = NULL;

    if (m_dfu_ctx.dfu_state == BACKGROUND_DFU_DOWNLOAD_INIT_CMD)
    {
        m_dfu_ctx.dfu_diag.init_blocks_requested += blocks_count(p_req_bmp);
        p_resource_name = BITMAP_RESOURCE_NAME "/" INIT_RESOURCE_NAME;
    }
    else if (m_dfu_ctx.dfu_state == BACKGROUND_DFU_DOWNLOAD_FIRMWARE)
    {
        p_resource_name = BITMAP_RESOURCE_NAME "/" IMAGE_RESOURCE_NAME;
        m_dfu_ctx.dfu_diag.image_blocks_requested += blocks_count(p_req_bmp);
    }

    coap_message_t * p_message =  message_create(&message_conf,
                                                 p_resource_name,
                                                 (uint8_t *)(&p_req_bmp->offset),
                                                 sizeof(p_req_bmp->bitmap) + sizeof(uint16_t),
                                                 &m_coap_dfu_ctx.remote);
    if (p_message != NULL)
    {
        coap_dfu_message_send(p_message);
    }
}

uint32_t background_dfu_random(void)
{
    return otPlatRandomGet();
}

void background_dfu_transport_state_update(background_dfu_context_t * p_dfu_ctx)
{
    switch (p_dfu_ctx->dfu_state)
    {
        case BACKGROUND_DFU_DOWNLOAD_TRIG:
            m_coap_dfu_ctx.p_resource_path = TRIGGER_RESOURCE_NAME;
            m_coap_dfu_ctx.handler         = handle_trigger_response;
            break;

        case BACKGROUND_DFU_DOWNLOAD_INIT_CMD:
            m_coap_dfu_ctx.p_resource_path = INIT_RESOURCE_NAME;
            m_coap_dfu_ctx.handler         = handle_block_response;
            break;

        case BACKGROUND_DFU_DOWNLOAD_FIRMWARE:
            m_coap_dfu_ctx.p_resource_path = IMAGE_RESOURCE_NAME;
            m_coap_dfu_ctx.handler         = handle_block_response;
            break;

        case BACKGROUND_DFU_WAIT_FOR_RESET:
            break;

        default:
            NRF_LOG_WARNING("Unhandled state in background_dfu_transport_state_update (s: %s).",
                    (uint32_t)background_dfu_state_to_string(p_dfu_ctx->dfu_state));
    }
}

void background_dfu_transport_send_request(background_dfu_context_t * p_dfu_ctx)
{
    uint16_t block_size = (p_dfu_ctx->dfu_state == BACKGROUND_DFU_DOWNLOAD_TRIG) ?
                            0 : DEFAULT_BLOCK_SIZE;

    coap_message_t * p_request = coap_dfu_create_request(&m_coap_dfu_ctx.remote,
                                                         m_coap_dfu_ctx.p_resource_path,
                                                         block_size,
                                                         p_dfu_ctx->block_num,
                                                         m_coap_dfu_ctx.handler);

    if (p_request != NULL)
    {
        NRF_LOG_INFO("Requesting [%s] (block:%u mid:%d)",
                    (uint32_t)m_coap_dfu_ctx.p_resource_path,
                    p_dfu_ctx->block_num,
                    p_request->header.id);

        coap_dfu_delayed_message_send(p_request);
    }
    else
    {
        NRF_LOG_ERROR("Failed to create a request.")
    }
}

/***************************************************************************************************
 * @section Public API
 **************************************************************************************************/

void background_dfu_handle_error(void)
{
    coap_dfu_handle_error();
}

__WEAK void coap_dfu_handle_error(void)
{
    // Intentionally empty.
}

void coap_dfu_diagnostic_get(struct background_dfu_diagnostic *p_diag)
{
    if (p_diag)
    {
        memcpy(p_diag, &m_dfu_ctx.dfu_diag, sizeof(background_dfu_diagnostic_t));
        p_diag->state = m_dfu_ctx.dfu_state;
    }
}

uint32_t coap_dfu_trigger(const coap_remote_t * p_remote)
{
    NRF_LOG_INFO("Triggering DFU");

    if (m_dfu_ctx.dfu_state != BACKGROUND_DFU_IDLE)
    {
        NRF_LOG_WARNING("Invalid state");
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_remote != NULL)
    {
        memcpy(&m_coap_dfu_ctx.remote, p_remote, sizeof(*p_remote));
    }
    else
    {
        memcpy(&m_coap_dfu_ctx.remote.addr, REALM_LOCAL_ADDR, 16);
        m_coap_dfu_ctx.remote.port_number = DFU_UDP_PORT;
    }

    // Transition from DFU_IDLE to DFU_DOWNLOAD_TRIG.
    return background_dfu_handle_event(&m_dfu_ctx, BACKGROUND_DFU_EVENT_TRANSFER_COMPLETE);
}

uint32_t coap_dfu_init(const void * p_context)
{
    uint32_t err_code;

    do
    {
        memset(&m_coap_dfu_ctx, 0 , sizeof(m_coap_dfu_ctx));

        err_code = coap_protocol_init(p_context);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        err_code = endpoints_init(&m_coap_dfu_ctx);
        if (err_code != NRF_SUCCESS)
        {
            break;
        }

        nrf_dfu_settings_init(false);
        nrf_dfu_req_handler_init(dfu_observer);

        background_dfu_state_init(&m_dfu_ctx);

        app_timer_create(&m_send_timer, APP_TIMER_MODE_SINGLE_SHOT, delayed_send_handler);
        app_timer_create(&m_reset_timer, APP_TIMER_MODE_SINGLE_SHOT, delayed_reset_handler);

        APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
    } while (0);

    return err_code;
}

void coap_dfu_process(void)
{
    app_sched_execute();
}

void coap_dfu_reset_state(void)
{
    background_dfu_reset_state(&m_dfu_ctx);
}

/** @} */
