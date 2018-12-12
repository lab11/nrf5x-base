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

#ifndef MQTTSN_PLATFORM_H
#define MQTTSN_PLATFORM_H

#include "mqttsn_client.h"

typedef void (*timer_timeout_handler_t)(void * p_context);


/**@brief Initializes the MQTT-SN client's platform.  
 *
 * @return NRF_SUCCESS if the initialization has been successful. Otherwise error code is returned.
 */
uint32_t mqttsn_platform_init(void);


/**@brief Initializes the MQTT-SN platform's timer.  
 *
 * @return NRF_SUCCESS if the initialization has been successful. Otherwise error code is returned.
 */
uint32_t mqttsn_platform_timer_init(void);


/**@brief Starts the MQTT-SN platform's timer. 
 *
 * @note Calling this function on a running timer should be ignored.
 *
 * @param[in]    p_client            Pointer to MQTT-SN client instance.
 * @param[in]    timeout_ms          Timeout in milliseconds.
 *
 * @return NRF_SUCCESS if the start operation has been successful. Otherwise error code is returned.
 */
uint32_t mqttsn_platform_timer_start(mqttsn_client_t * p_client, uint32_t timeout_ms);


/**@brief Stops the MQTT-SN platform's timer.  
 *
 * @return NRF_SUCCESS if the stop operation has been successful. Otherwise error code is returned.
 */
uint32_t mqttsn_platform_timer_stop(void);


/**@brief Gets the current MQTT-SN platform's timer value.  
 *
 * @return       Current timer value in milliseconds.
 */
uint32_t mqttsn_platform_timer_cnt_get(void);


/**@brief Gets the MQTT-SN platform's timer max value in milliseconds.  
 *
 * @return       Maximum timer value in milliseconds.
 */
uint32_t mqttsn_platform_timer_resolution_get(void);


/**@brief Converts milliseconds to the MQTT-SN platform's timer ticks.  
 *
 * @param[in]    timeout_ms Time in milliseconds to convert to timer ticks.
 *
 * @return       Calculated number of ticks.
 */
uint32_t mqttsn_platform_timer_ms_to_ticks(uint32_t timeout_ms);


/**@brief Sets the MQTT-SN platform's timer timeout.  
 *
 * @param[in]    timeout_ms Timeout in milliseconds, calculated with current timer value.
 *
 * @return       Calculated timeout in ticks.
 */
uint32_t mqttsn_platform_timer_set_in_ms(uint32_t timeout_ms);


/**@brief MQTT-SN platform's random number generator.  
 *
 * @param[in]    max_val  The maximum value of the generated random number.
 *
 * @return       Calculated random number. 
 */
uint16_t mqttsn_platform_rand(uint16_t max_val);

#endif // MQTTSN_PLATFORM_H
