/**
 * Copyright (c) 2017 - 2019, Nordic Semiconductor ASA
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

#ifndef MQTTSN_PACKET_INTERNAL_H
#define MQTTSN_PACKET_INTERNAL_H

/***************************************************************************************************
 * @section INCLUDES
 **************************************************************************************************/

#include "mqttsn_client.h"

/***************************************************************************************************
 * @section DEFINES
 **************************************************************************************************/

/**@brief IPv6 address SEARCHGW message is sent to. */
#define MQTTSN_BROADCAST_ADDR { 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/**@brief Default port MQTT-SN gateway binds to. */
#define MQTTSN_DEFAULT_GATEWAY_PORT           47193

/**@brief Maximum MQTT-SN packet ID. */
#define MQTTSN_MAX_PACKET_ID                  65535

/**@brief Offset of Gateway ID in GWINFO message. */
#define MQTTSN_OFFSET_GATEWAY_INFO_ID         2

/**@brief Offset of Message Type field when Message Length is two byte long. */
#define MQTTSN_OFFSET_TWO_BYTE_LENGTH         3

/**@brief Offset of Message Type field when Message Length is one byte long. */
#define MQTTSN_OFFSET_ONE_BYTE_LENGTH         1

/**@brief First byte of a message value when Message Length is two byte long. */
#define MQTTSN_TWO_BYTE_LENGTH_CODE           0x01

/**@brief Maximum value of SEARCHGW message delay. */
#define MQTTSN_SEARCH_GATEWAY_MAX_DELAY_IN_MS 2000

/**@brief Values of message types eligible for retransmission. For retransmission purpose only. */
#define MQTTSN_MSGTYPE_CONNECT      0x04
#define MQTTSN_MSGTYPE_REGISTER     0x0a
#define MQTTSN_MSGTYPE_PUBLISH      0x0c
#define MQTTSN_MSGTYPE_SUBSCRIBE    0x12
#define MQTTSN_MSGTYPE_UNSUBSCRIBE  0x14
#define MQTTSN_MSGTYPE_PINGREQ      0x16
#define MQTTSN_MSGTYPE_WILLTOPICUPD 0x1a
#define MQTTSN_MSGTYPE_WILLMSGUPD   0x1c

/**@brief Search mode in packet_dequeue. */
typedef enum mqttsn_packet_dequeue_t
{
    MQTTSN_MESSAGE_ID,
    MQTTSN_MESSAGE_TYPE
} mqttsn_packet_dequeue_t;

/**@brief MQTT-SN events that cause MQTT-SN client's state to change. */
typedef enum client_state_fsm_event_t
{
    SENT_SEARCHGW,                  /**< SEARCH GATEWAY message was sent. */
    SENT_CONNECT,                   /**< CONNECT message was sent. */
    SENT_PINGREQ,                   /**< PINGREQ message was sent. */
    SENT_SLEEP_REQUEST,             /**< DISCONNECT message with sleep duration was sent. */
    SENT_DISCONNECT_REQUEST,        /**< DISCONNECT message without sleep duration was sent. */

    RECEIVED_CONNACK,               /**< CONNACK message was received. */
    RECEIVED_SLEEP_PERMISSION,      /**< Response to a DISCONNECT message with sleep duration was received. */
    RECEIVED_DISCONNECT_PERMISSION, /**< Response to a DISCONNECT message without sleep duration was received. */
    RECEIVED_DISCONNECT,            /**< DISCONNECT message sent not as a response (disconnected by the gateway). */
    RECEIVED_PINGRESP,              /**< PINGRESP message was received. */

    TIMEOUT_SEARCHGW,               /**< Timeout occurred when waiting for GWINFO message. */
    TIMEOUT_CONNECT,                /**< CONNECT message retransmission limit has been reached. */
    TIMEOUT_PINGREQ                 /**< PINGREQ message retransmission limit has been reached. */
} client_state_fsm_event_t;

/***************************************************************************************************
 * @section FIFO
 **************************************************************************************************/

/**@brief Initializes packet queue.
 *
 * @param[inout] p_client    Pointer to MQTT-SN client.
 */
void mqttsn_packet_fifo_init(mqttsn_client_t * p_client);

/**@brief Uninitializes packet queue.
 *
 * @param[inout] p_client    Pointer to initialized client.
 */
void mqttsn_packet_fifo_uninit(mqttsn_client_t * p_client);

/**@brief Enqueues packet.
 *
 * @param[inout] p_client         Pointer to initialized client.
 * @param[in]    p_packet         Pointer to MQTT-SN packet to enqueue.
 *
 * @retval       NRF_SUCCESS      If the packet has been enqueued successfully.
 * @retval       NRF_ERROR_NO_MEM If the packet queue is full.
 */
uint32_t mqttsn_packet_fifo_elem_add(mqttsn_client_t * p_client, mqttsn_packet_t * p_packet);
 
/**@brief Dequeues packet.
 *
 * @param[inout] p_client            Pointer to initialized client.
 * @param[in]    msg_to_dequeue      Identifier of the message to dequeue; depends on mode.
 * @param[in]    mode                Either message type or message ID.
 *
 * @retval       NRF_SUCCESS         If the packet has been dequeued successfully.
 * @retval       NRF_ERROR_NOT_FOUND If the packet has not been found.
 */
uint32_t mqttsn_packet_fifo_elem_dequeue(mqttsn_client_t        * p_client,
                                         uint16_t                 msg_to_dequeue,
                                         mqttsn_packet_dequeue_t  mode);

/**@brief Checks if given packet is in the queue.
 *
 * @param[inout] p_client        Pointer to initialized client.
 * @param[in]    msg_to_dequeue  Identifier of the message to dequeue; depends on mode.
 * @param[in]    mode            Either message type or message ID.
 *
 * @return       Index of the found message. MQTTSN_PACKET_FIFO_MAX_LENGTH if packet not found.
 */
uint32_t mqttsn_packet_fifo_elem_find(mqttsn_client_t        * p_client,
                                      uint16_t                 msg_to_dequeue,
                                      mqttsn_packet_dequeue_t  mode);


/***************************************************************************************************
 * @section SENDER
 **************************************************************************************************/

/**@brief Retransmits message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 * @param[in]    p_remote    Pointer to remote endpoint.
 * @param[in]    p_data      Pointer to the data to be retransmitted.
 * @param[in]    datalen     Length of the data to be retransmitted.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_retransmit(mqttsn_client_t       * p_client,
                                         const mqttsn_remote_t * p_remote,
                                         const uint8_t         * p_data,
                                         uint16_t                datalen);

/**@brief Sends SEARCHGW message.
 *
 * @param[inout] p_client Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_searchgw(mqttsn_client_t * p_client);

/**@brief Sends WILLTOPIC message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_willtopic(mqttsn_client_t * p_client);

/**@briefSends WILLMSG message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_willmsg(mqttsn_client_t * p_client);

/**@brief Sends CONNECT message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_connect(mqttsn_client_t * p_client);

/**@brief Sends REGISTER message.
*
* @param[inout] p_client       Pointer to initialized client.
* @param[in]    p_topic        Pointer to topic to be registered.
* @param[in]    topic_name_len Length of the topic.
*
* @return       NRF_SUCCESS if the message has been sent successfully.
*               Otherwise error code is returned.
*/
uint32_t mqttsn_packet_sender_register(mqttsn_client_t * p_client,
                                       mqttsn_topic_t  * p_topic,
                                       uint16_t          topic_name_len);

/**@brief Sends REGACK message. (For wildcard topics) 
 *
 * @param[inout] p_client    Pointer to initialized client.
 * @param[in]    topic_id    Pointer to topic to be registered.
 * @param[in]    packet_id   Length of the topic.
 * @param[in]    ret_code    MQTT-SN return code.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_regack(mqttsn_client_t * p_client,
                                     uint16_t          topic_id,
                                     uint16_t          packet_id,
                                     uint8_t           ret_code);

/**@brief Sends PUBLISH message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 * @param[in]    p_topic     Pointer to topic to publish on.
 * @param[in]    p_payload   Pointer to the data to be published.
 * @param[in]    payloadlen  Length of the data to be published.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_publish(mqttsn_client_t * p_client,
                                      mqttsn_topic_t  * p_topic,
                                      const uint8_t   * p_payload,
                                      uint16_t          payloadlen);

/**@brief Sends PUBACK message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 * @param[in]    topic_id    Pointer to topic to be registered.
 * @param[in]    packet_id   Length of the topic.
 * @param[in]    ret_code    MQTT-SN return code.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_puback(mqttsn_client_t * p_client,
                                     uint16_t          topic_id,
                                     uint16_t          packet_id,
                                     uint8_t           ret_code); 

/**@brief Sends SUBSCRIBE message.
 *
 * @param[inout] p_client       Pointer to initialized client.
 * @param[in]    p_topic        Pointer to topic to subscribe to.
 * @param[in]    topic_name_len Length of the topic.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_subscribe(mqttsn_client_t * p_client,
                                        mqttsn_topic_t  * p_topic,
                                        uint16_t          topic_name_len);

/**@brief Sends UNSUBSCRIBE message.
 *
 * @param[inout] p_client       Pointer to initialized client.
 * @param[in]    p_topic        Pointer to topic to unsubscribe.
 * @param[in]    topic_name_len Length of the topic.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_unsubscribe(mqttsn_client_t * p_client,
                                          mqttsn_topic_t  * p_topic, 
                                          uint16_t          topic_name_len);

/**@brief Sends DISCONNECT message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 * @param[in]    duration    Sleep duration (optional).
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_disconnect(mqttsn_client_t * p_client, uint16_t duration);

/**@brief Sends WILLTOPICUPD message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_willtopicupd(mqttsn_client_t * p_client);

/**@brief Sends WILLMSGUPD message.
 *
 * @param[inout] p_client    Pointer to initialized client.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_sender_willmsgupd(mqttsn_client_t * p_client);


/***************************************************************************************************
 * @section RECEIVER
 **************************************************************************************************/

/**@brief Handles received message according to its type.
 *
 * @param[inout] p_client Pointer to initialized client.
 * @param[in]    p_port   Pointer to destination port of the message.
 * @param[in]    p_remote Pointer to source endpoint of the message.
 * @param[in]    p_data   Pointer to received data.
 * @param[in]    datalen  Length of the received data.
 *
 * @return       NRF_SUCCESS if the message has been processed successfully.
 *               Otherwise error code is returned.
 */
uint32_t mqttsn_packet_receiver(mqttsn_client_t        * p_client,
                                const mqttsn_port_t    * p_port,
                                const mqttsn_remote_t  * p_remote,
                                const uint8_t          * p_data,
                                uint16_t                 datalen);

/**@brief Returns index of Message Type field.
 *
 * @param[inout] p_buffer Pointer to message to check.
 *
 * @return       Index of Message type field.
 */
uint32_t mqttsn_packet_msgtype_index_get(const uint8_t * p_buffer);

/**@brief Returns type of retranmission error.
 *
 * @param[inout] p_buffer Pointer to message to check.
 *
 * @return       Type of retransmission error.
 */
mqttsn_ack_error_t mqttsn_packet_msgtype_error_get(const uint8_t * p_buffer);

/***************************************************************************************************
 * @section CLIENT INTERNAL
 **************************************************************************************************/

/**@brief Schedules timer timeout.
 *
 * @param[inout] p_client Pointer to MQTT-SN client instance.
 */
uint32_t mqttsn_client_timeout_schedule(mqttsn_client_t * p_client);

/**@brief Handles timer timeout.
 *
 * @param[inout] p_client Pointer to MQTT-SN client instance.
 */
void mqttsn_client_timeout_handle(mqttsn_client_t * p_client);

/**@brief Launches the gateway discovery procedure.  
 *
 * @param[inout] p_client    Pointer to MQTT-SN client instance.
 *
 * @return       None.
 */
void mqttsn_client_gateway_discovery_start(mqttsn_client_t * p_client);

/**@brief Finishes the gateway discovery procedure.  
 *
 * @param[inout] p_client    Pointer to MQTT-SN client instance.
 *
 * @return       None.
 */
void mqttsn_client_gateway_discovery_finish(mqttsn_client_t * p_client);

/**@brief Updates the MQTT-SN client's state.  
 * 
 * @param[inout] p_client  Pointer to an MQTT-SN client instance.
 * @param[in]    fsm_event Event that causes an MQTT-SN client's state to change.
 *
 * @return       None.
 */
void mqttsn_client_state_update(mqttsn_client_t * p_client, const client_state_fsm_event_t fsm_event);

#endif // MQTTSN_PACKET_INTERNAL_H
