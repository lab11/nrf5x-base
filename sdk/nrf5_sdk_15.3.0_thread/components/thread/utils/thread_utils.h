/**
 * Copyright (c) 2017-2019, Nordic Semiconductor ASA
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

#ifndef THREAD_UTILS_H__
#define THREAD_UTILS_H__

#include <openthread/instance.h>

/**@brief Enumeration describing Thread radio receiver mode.
 *
 * @details THREAD_RADIO_MODE_RX_ON_WHEN_IDLE causes the device to keep its receiver on, when it is in idle state.
 *          Selecting THREAD_RADIO_MODE_RX_OFF_WHEN_IDLE results in creating the Thread Sleepy End Device.
 */
typedef enum
{
    THREAD_RADIO_MODE_RX_ON_WHEN_IDLE = 0, /**< Powered device */
    THREAD_RADIO_MODE_RX_OFF_WHEN_IDLE,    /**< Sleepy End Device */
} thread_radio_receiver_mode_t;


/**@brief Structure holding Thread configuration parameters. */
typedef struct
{
    thread_radio_receiver_mode_t radio_mode;              /**< Selected Thread radio receiver mode. */
    bool                         autostart_disable;       /**< Determines if node should attempt to join the
                                                               Thread network automatically. */
    bool                         autocommissioning;       /**< Determines if node should use pre-commissioned
                                                               data to join the Thread network. */
    uint32_t                     poll_period;             /**< Default SED poll period in miliseconds. */
    uint32_t                     default_child_timeout;   /**< Thread child timeout value in seconds. */
} thread_configuration_t;


/**@brief Function for initializing the Thread Stack.
 *
 * @details This function is used to initialize the OpenThread's platform and stack.
 *
 * @param[in] p_config Thread configuration structure pointer.
 *
 */
void thread_init(const thread_configuration_t * p_config);


/**@brief Function for deinitializing the Thread Stack. */
void thread_deinit(void);


/**@brief Function for deinitializing the Thread Stack (without re-initialization of the platform). */
void thread_soft_deinit(void);


/**@brief Function for initializing the Command Line Interface module together with a diagnostic module. */
void thread_cli_init(void);


/**@brief Function for processing the Thread Stack pending tasks.
 *
 * @details This function must be periodically executed to process the Thread Stack pending tasks.
 */
void thread_process(void);


/**@brief Function for putting the device to the sleep mode in order to save energy. */
void thread_sleep(void);


/**@brief Function for returning OpenThread Instance pointer.
 *
 * @details The @p thread_init function must be executed before calling this function.
 *
 * @return otInstance OpenThread instance structure pointer.
 */
otInstance * thread_ot_instance_get(void);


/**@brief Function for registering a callback to indicate when certain configuration or state
 *        changes within OpenThread.
 *
 * @details Sets function that is executed when the state of the node changes.
 *
 * @param[in] handler Pointer to the function that registers a callback to indicate when certain
 *            configuration or state changes within OpenThread.
 */
void thread_state_changed_callback_set(otStateChangedCallback handler);


/**@brief Function for checking if soft reset was requested (stack only).
 *
 * @return True if soft reset was requested, false otherwise.
 */
bool thread_soft_reset_was_requested(void);

#endif /* THREAD_UTILS_H__ */
