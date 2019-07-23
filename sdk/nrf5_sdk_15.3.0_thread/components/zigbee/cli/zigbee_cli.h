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
#ifndef ZIGBEE_CLI_H__
#define ZIGBEE_CLI_H__

#include <stdint.h>

#include "zboss_api.h"
#include "zigbee_helpers.h"

/** @brief Name of the module used for logger messaging.
 */
#define ZIGBEE_CLI_LOG_NAME     ZIGBEE_NRF_LOG_MODULE_NAME

/**@brief Function for returning the number of the endpoint used by the CLI.
 */
zb_uint8_t zb_get_cli_endpoint(void);

/**@brief Function for initialising the Zigbee CLI subsystem that is using the CLI module.
 *
 * @param ep Number of the endpoint to be used for the CLI Agent.
 */
void zb_cli_init(uint8_t ep);

/**@brief Function for processing the Zigbee CLI subsystem.
 *
 * This function must be called in the main loop.
 *
 */
void zb_cli_process(void);

/**@brief Function for intercepting every frame coming to the endpoint.
 *
 * @param param    Reference to the ZBOSS buffer.
 */
zb_uint8_t cli_agent_ep_handler(zb_uint8_t param);

#if defined(ZIGBEE_CLI_DEBUG) || defined(DOXYGEN)
/**@brief Function for setting the state of the debug mode of the CLI.
 *
 * @param debug    Turns the debug mode on (ZB_TRUE) or off (ZB_FALSE).
 */
zb_void_t zb_cli_debug_set(zb_bool_t debug);
#endif

#if defined(ZIGBEE_CLI_DEBUG) || defined(DOXYGEN)
/**@brief Function for getting the state of the debug mode of the CLI.
 *
 * @retval ZB_TRUE  Debug mode is turned on.
 * @retval ZB_FALSE Debug mode is turned off.
 */
zb_bool_t zb_cli_debug_get(zb_void_t);
#endif

#if defined(ZIGBEE_CLI_DEBUG) || defined(DOXYGEN)
/**@brief Function for suspending the processing of the Zigbee main loop.
 */
zb_void_t zb_cli_suspend(zb_void_t);
#endif

#if defined(ZIGBEE_CLI_DEBUG) || defined(DOXYGEN)
/**@brief Function for resuming the processing of the Zigbee main loop.
 */
zb_void_t zb_cli_resume(zb_void_t);
#endif

#if defined(ZIGBEE_CLI_DEBUG) || defined(DOXYGEN)
/**@brief Function for getting the state of the Zigbee scheduler processing suspension.
 *
 * @retval ZB_TRUE  Scheduler processing is suspended.
 * @retval ZB_FALSE Scheduler processing is not suspended.
 */
zb_bool_t zb_cli_stack_is_suspended(zb_void_t);
#endif

/**@brief Function for checking if the Zigbee stack has been started
 *
 * @retval ZB_TRUE  Zigbee stack has been started (CLI command 'bdb start' has been executed successfully)
 * @retval ZB_FALSE Zigbee stack has not been started yet
 */
zb_bool_t zb_cli_is_stack_started(void);

#endif /* ZIGBEE_CLI_H__ */
