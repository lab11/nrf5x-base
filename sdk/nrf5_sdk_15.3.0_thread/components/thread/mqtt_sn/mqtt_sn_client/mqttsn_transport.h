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

#ifndef MQTTSN_TRANSPORT_H
#define MQTTSN_TRANSPORT_H

#include "mqttsn_client.h"


/**@brief Initializes the MQTT-SN client's transport.  
 *
 * @param[inout] p_client            Pointer to uninitialized client. 
 * @param[in]    port                Number of the port the client will be bound to.
 * @param[in]    p_context           Pointer to transport layer specific context.
 *
 * @retval       NRF_SUCCESS         If the initialization has been successful.
 * @retval       NRF_ERROR_INTERNAL  Otherwise.
 */
uint32_t mqttsn_transport_init(mqttsn_client_t * p_client, uint16_t port, const void * p_context);


/**@brief Sends message.  
 *
 * @param[inout] p_client    Pointer to initialized and connected client. 
 * @param[in]    p_remote    Pointer to remote endpoint.
 * @param[in]    p_data      Buffered data to send.
 * @param[in]    datalen     Length of the buffered data.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_transport_write(mqttsn_client_t       * p_client,
                                const mqttsn_remote_t * p_remote,
                                const uint8_t         * p_data,
                                uint16_t                datalen);


/**@brief Receives message.  
 *
 * @param[inout] p_context          Pointer to transport layer specific context. 
 * @param[in]    p_port             Pointer to local port number.
 * @param[in]    p_remote           Pointer to remote endpoint.
 * @param[in]    p_data             Buffer for received data.
 * @param[in]    datalen            Length of the buffered data.
 *
 * @return       NRF_SUCCESS if the message has been processed successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_transport_read(void                   * p_context,
                               const mqttsn_port_t    * p_port,
                               const mqttsn_remote_t  * p_remote,
                               const uint8_t          * p_data,
                               uint16_t                 datalen);


/**@brief Unitializes the MQTT-SN client's transport.  
 *
 * @param[inout] p_client        Pointer to initialized and connected client.
 *
 * @retval    NRF_SUCCESS        If the uninitialization has been successful.
 * @retval    NRF_ERROR_INTERNAL Otherwise.
 */
uint32_t mqttsn_transport_uninit(mqttsn_client_t * p_client);

#endif // MQTTSN_TRANSPORT_H
