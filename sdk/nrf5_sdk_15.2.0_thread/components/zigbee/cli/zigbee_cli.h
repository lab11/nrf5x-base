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
#ifndef ZIGBEE_CLI_H__
#define ZIGBEE_CLI_H__

#include <stdint.h>
#include <zboss_api.h>

/** @brief Name of the module used for logger messaging.
 */
#define ZIGBEE_CLI_LOG_NAME zigbee

/**@brief Returns the number of the Endpoint used by the CLI
 */
zb_uint8_t zb_get_cli_endpoint(void);

/**@brief Initialise the Zigbee CLI subsystem.
 * Initialises the Zigbee CLI subsystem which is using the CLI module
 *
 * @param ep Number of the Endpoint to use for the CLI Agent
 */
void zb_cli_init(uint8_t ep);

/**@brief Process the Zigbee CLI subsystem.
 *
 * Processes the Zigbee CLI subsystem. Function has to be called in the main loop.
 *
 */
void zb_cli_process(void);

/**@brief The Handler to 'intercept' every frame coming to the endpoint
 *
 * @param param    Reference to a ZBOSS buffer
 */
zb_uint8_t cli_agent_ep_handler(zb_uint8_t param);


#endif /* ZIGBEE_CLI_H__ */
