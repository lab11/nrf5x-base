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

#ifndef APP_THREAD_UTILS_H
#define APP_THREAD_UTILS_H

#include <openthread/instance.h>

/**@brief Enumeration describing thread roles.
 *
 * @details RX_ON_WHEN_IDLE causes the device to keep its receiver on when it is idle.
 *          Selecting RX_OFF_WHEN_IDLE results in creating the Sleepy End Device.
 */
typedef enum
{
    RX_ON_WHEN_IDLE = 0,  /**< Powered device */
    RX_OFF_WHEN_IDLE,     /**< Sleepy End Device */
} thread_role_t;

/**@brief Structure holding Thread configuration parameters.
 */
typedef struct
{
    thread_role_t role;                    /**< Selected Thread role. */
    bool          autostart_disable;       /**< If node should not start the Thread operation automatically. */
    bool          autocommissioning;       /**< If node should be commissioned automatically. */
    uint32_t      poll_period;             /**< Default SED poll period in miliseconds. */
    uint32_t      default_child_timeout;   /**< Thread child timeout value in seconds. */
} thread_configuration_t;

/**@brief Type definition of the function used to handle Thread Network state change taking taking uint32_t flags and void context pointer and returning void.
 *
 * @param[in] flags     Bit-field indicating state that has changed.
 * @param[in] p_context Context pointer to be used by the callback function.
 */
typedef void (*thread_state_change_callback_t)(uint32_t flags, void * p_context);

/**@brief Type definition of the function used to inform the application if the commissioning should can performed by the Joiner.
 *
 * @param[in] value     Set to true if the commissioning should can performed by the Joiner.
 */
typedef void (*thread_joiner_allow_start_function_set_t)(bool value);

/**@brief Function that initializes the Thread Stack.
 *
 * @details This function is used to obtain and store the OpenThread Instance structure pointer internally,
 *          initialize the Uart Commandline Interface, and set Thread Channel and PANID.
 *
 * @param[in] p_thread_configuration Thread configuration structure pointer.
 */
void thread_init(const thread_configuration_t * p_thread_configuration);

/**@brief Function that deinitializes the Thread Stack.
 */
void thread_deinit(void);

/**@brief Function that deinitializes the Thread Stack (without platform).
 */
void thread_soft_deinit(void);

/**@brief Function that initializes the UART Command Line Interface together with a diagnostic module.
 */
void thread_cli_init(void);

/**@brief Function that processess the Thread Stack.
 *
 * @details This function needs to be periodically executed to ensure the Thread operation.
 */
void thread_process(void);

/**@brief Function that puts the NRF to sleep in order to save energy.
 */
void thread_sleep(void);

/**@brief Function that returns OpenThread Instance pointer.
 *
 * @details The thread_init() function needs to be executed before calling this function.
 *
 * @return otInstance OpenThread instance structure pointer.
 */
otInstance * thread_ot_instance_get(void);

/**@brief Function that returns Thread configuration structure instance pointer.
 *
 * @return Thread_configuration structure instance pointer.
 */
thread_configuration_t * thread_configuration_get(void);

/**@brief Function used to set the thread_state_changed_callback function.
 *
 * @details Sets function that is exectuted when the state of the node changes.
 *
 * @param[in] handler Pointer to the function used as Thread State Changed callback taking uint32_t flags and void context pointer and returning void.
 */
void thread_state_changed_callback_set(thread_state_change_callback_t handler);

/**@brief Check if soft reset was requested (stack only).
 *
 * @return True if soft reset was requested, false otherwise.
 */
bool thread_soft_reset_was_requested(void);

#endif /* APP_THREAD_UTILS_H */
