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

#include "MQTTSNConnect.h"
#include "MQTTSNPacket.h"
#include "mqttsn_packet_internal.h"
#include "mqttsn_transport.h"
#include "mqttsn_platform.h"
#include "mem_manager.h"
#include "nrf_assert.h"
#include "nrf_error.h"
#include "nrf_log.h"

#define MQTTSN_PACKET_SEARCHGW_LENGTH     3
#define MQTTSN_PACKET_CONNECT_LENGTH     (6 + MQTTSN_CLIENT_ID_MAX_LENGTH)
#define MQTTSN_PACKET_WILLTOPIC_LENGTH    3
#define MQTTSN_PACKET_WILLMSG_LENGTH      2
#define MQTTSN_PACKET_REGISTER_LENGTH     6
#define MQTTSN_PACKET_REGACK_LENGTH       7
#define MQTTSN_PACKET_PUBLISH_LENGTH      7
#define MQTTSN_PACKET_PUBACK_LENGTH       7
#define MQTTSN_PACKET_SUBSCRIBE_LENGTH    5
#define MQTTSN_PACKET_UNSUBSCRIBE_LENGTH  5
#define MQTTSN_PACKET_DISCONNECT_LENGTH   2
#define MQTTSN_PACKET_WILLTOPICUPD_LENGTH 4
#define MQTTSN_PACKET_WILLMSGUPD_LENGTH   4
#define MQTTSN_PACKET_DISCONNECT_DURATION -1

/**@brief Calculates next message ID. 
 *
 * @param[in]    p_client    Pointer to MQTT-SN client instance.
 *
 * @return       Next packet ID to use.
 */
static uint16_t next_packet_id_get(mqttsn_client_t * p_client)
{
    return (p_client->message_id == MQTTSN_MAX_PACKET_ID) ? 1 : (++(p_client->message_id));
}

/**@brief Sends MQTT-SN message.  
 *
 * @param[inout] p_client    Pointer to initialized and connected client. 
 * @param[in]    p_remote    Pointer to remote endpoint.
 * @param[in]    p_data      Buffered data to send.
 * @param[in]    datalen     Length of the buffered data.
 *
 * @return       NRF_SUCCESS if the message has been sent successfully.
 *               Otherwise error code is returned.
 */
static uint32_t mqttsn_packet_sender_send(mqttsn_client_t       * p_client,
                                          const mqttsn_remote_t * p_remote,
                                          const uint8_t         * p_data,
                                          uint16_t                datalen)
{
    return mqttsn_transport_write(p_client, p_remote, p_data, datalen);
}

uint32_t mqttsn_packet_sender_retransmit(mqttsn_client_t       * p_client,
                                         const mqttsn_remote_t * p_remote,
                                         const uint8_t         * p_data,
                                         uint16_t                datalen)
{
    return mqttsn_packet_sender_send(p_client, p_remote, p_data, datalen);
}

uint32_t mqttsn_packet_sender_searchgw(mqttsn_client_t * p_client)
{
    unsigned char radius   = 1;
    uint32_t      err_code = NRF_SUCCESS;
    uint8_t     * p_data   = nrf_malloc(MQTTSN_PACKET_SEARCHGW_LENGTH);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("SEARCHGW message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = MQTTSNSerialize_searchgw(p_data, MQTTSN_PACKET_SEARCHGW_LENGTH, radius);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        const mqttsn_remote_t broadcast_search =
        {
            .addr        = MQTTSN_BROADCAST_ADDR,
            .port_number = MQTTSN_DEFAULT_CLIENT_PORT,
        };

        err_code = mqttsn_packet_sender_send(p_client, &broadcast_search, p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_willtopic(mqttsn_client_t * p_client)
{
    uint8_t   will_qos    = 0;
    uint8_t   will_retain = 0;

    uint32_t  err_code    = NRF_SUCCESS;
    uint16_t  packet_len  = MQTTSN_PACKET_WILLTOPIC_LENGTH + p_client->connect_info.will_topic_len;
    uint8_t * p_data      = nrf_malloc(packet_len);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("WILLTOPIC message cannot be allocated\r\n");
            break;
        }

        MQTTSNString will_topic;
        
        memset(&will_topic, 0, sizeof(MQTTSNString));
        will_topic.lenstring.data = (char *)p_client->connect_info.p_will_topic;
        will_topic.lenstring.len  = p_client->connect_info.will_topic_len;

        uint16_t datalen = MQTTSNSerialize_willtopic(p_data, packet_len, will_qos, will_retain, will_topic);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_willmsg(mqttsn_client_t * p_client)
{
    uint32_t  err_code   = NRF_SUCCESS;
    uint16_t  packet_len = MQTTSN_PACKET_WILLMSG_LENGTH + p_client->connect_info.will_msg_len;
    uint8_t * p_data     = nrf_malloc(packet_len);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("WILLMSG message cannot be allocated\r\n");
            break;
        }

        MQTTSNString will_msg;
        memset(&will_msg, 0, sizeof(MQTTSNString));
        will_msg.lenstring.data = (char *)p_client->connect_info.p_will_msg;
        will_msg.lenstring.len  = p_client->connect_info.will_msg_len;

        uint16_t datalen = MQTTSNSerialize_willmsg(p_data, packet_len, will_msg);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_connect(mqttsn_client_t * p_client)
{
    uint32_t                 err_code = NRF_SUCCESS;
    char                     p_buf[MQTTSN_CLIENT_ID_MAX_LENGTH];
    MQTTSNPacket_connectData options  = MQTTSNPacket_connectData_initializer;
    options.clientID.cstring          = p_buf;

    memset(options.clientID.cstring, 0, MQTTSN_CLIENT_ID_MAX_LENGTH);
    memcpy(options.clientID.cstring,
           p_client->connect_info.p_client_id,
           p_client->connect_info.client_id_len);
    options.willFlag     = p_client->connect_info.will_flag;
    options.duration     = p_client->connect_info.alive_duration;
    options.cleansession = p_client->connect_info.clean_session;

    uint8_t * p_data      = nrf_malloc(MQTTSN_PACKET_CONNECT_LENGTH);
    uint8_t * p_packet_copy = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("CONNECT message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = MQTTSNSerialize_connect(p_data, MQTTSN_PACKET_CONNECT_LENGTH, &options);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        memcpy(p_packet_copy, p_data, datalen);

        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       MQTTSN_MSGTYPE_CONNECT,
                                                                       MQTTSN_MESSAGE_TYPE);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_register(mqttsn_client_t * p_client,
                                       mqttsn_topic_t  * p_topic,
                                       uint16_t          topic_name_len)
{
    uint32_t  err_code    = NRF_SUCCESS;
    uint16_t  packet_len  = MQTTSN_PACKET_REGISTER_LENGTH + topic_name_len;
    uint8_t * p_data      = nrf_malloc(packet_len);
    uint8_t * p_packet_copy = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("REGISTER message cannot be allocated\r\n");
            break;
        }

        MQTTSNString topic_name;
        memset(&topic_name, 0, sizeof(topic_name));
        topic_name.lenstring.data = (char *)(p_topic->p_topic_name);
        topic_name.lenstring.len  = topic_name_len;

        uint16_t datalen = MQTTSNSerialize_register(p_data,
                                                    packet_len,
                                                    0,
                                                    next_packet_id_get(p_client),
                                                    &topic_name);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
        memcpy(p_packet_copy, p_data, datalen);


        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.id                 = p_client->message_id;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);
        retransmission_packet.topic = *p_topic;

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       p_client->message_id,
                                                                       MQTTSN_MESSAGE_ID);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_regack(mqttsn_client_t * p_client,
                                     uint16_t          topic_id,
                                     uint16_t          packet_id,
                                     uint8_t           ret_code)
{
    uint32_t err_code = NRF_SUCCESS;
    uint8_t * p_data  = nrf_malloc(MQTTSN_PACKET_REGACK_LENGTH);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("REGACK message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = MQTTSNSerialize_regack(p_data,
                                                  MQTTSN_PACKET_REGACK_LENGTH,
                                                  topic_id,
                                                  packet_id,
                                                  ret_code);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_publish(mqttsn_client_t * p_client,
                                      mqttsn_topic_t  * p_topic,
                                      const uint8_t   * payload,
                                      uint16_t          payload_len)
{
    uint32_t err_code      = NRF_SUCCESS;

    unsigned char dup      = 0;
    unsigned char retained = 0;
    uint8_t  qos           = 1;

    uint32_t  packet_len   = MQTTSN_PACKET_PUBLISH_LENGTH + payload_len;
    uint8_t * p_data       = nrf_malloc(packet_len);
    uint8_t * p_packet_copy  = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("PUBLISH message cannot be allocated\r\n");
            break;
        }

        MQTTSN_topicid topic;
        memset(&topic, 0, sizeof(MQTTSN_topicid));
        topic.type    = MQTTSN_TOPIC_TYPE_NORMAL;
        topic.data.id = p_topic->topic_id;

        uint16_t datalen = MQTTSNSerialize_publish(p_data,
                                                   packet_len,
                                                   dup,
                                                   qos,
                                                   retained,
                                                   next_packet_id_get(p_client),
                                                   topic,
                                                   (uint8_t *)payload,
                                                   payload_len);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        memcpy(p_packet_copy, p_data, datalen);

        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.id                 = p_client->message_id;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);
        retransmission_packet.topic = *p_topic;

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       p_client->message_id,
                                                                       MQTTSN_MESSAGE_ID);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

            err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_puback(mqttsn_client_t * p_client,
                                     uint16_t          topic_id,
                                     uint16_t          packet_id,
                                     uint8_t           ret_code)
{
    uint32_t err_code = NRF_SUCCESS;
    uint8_t * p_data  = nrf_malloc(MQTTSN_PACKET_PUBACK_LENGTH);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("PUBACK message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = MQTTSNSerialize_puback(p_data,
                                                  MQTTSN_PACKET_PUBACK_LENGTH,
                                                  topic_id,
                                                  packet_id,
                                                  ret_code);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_subscribe(mqttsn_client_t * p_client,
                                        mqttsn_topic_t * p_topic,
                                        uint16_t topic_name_len)
{
    uint32_t  err_code    = NRF_SUCCESS;
    uint16_t  packet_len  = MQTTSN_PACKET_SUBSCRIBE_LENGTH + topic_name_len;
    uint8_t   qos         = 1;
    uint8_t   dup         = 0;
    uint8_t * p_data      = nrf_malloc(packet_len);
    uint8_t * p_packet_copy = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("SUBSCRIBE message cannot be allocated\r\n");
            break;
        }

        MQTTSN_topicid topic;
        memset(&topic, 0, sizeof(MQTTSN_topicid));
        topic.type            = MQTTSN_TOPIC_TYPE_NORMAL;
        topic.data.long_.name = (char *)(p_topic->p_topic_name);
        topic.data.long_.len  = (int)topic_name_len;

        uint16_t datalen = MQTTSNSerialize_subscribe(p_data,
                                                     packet_len,
                                                     dup,
                                                     qos,
                                                     next_packet_id_get(p_client),
                                                     &topic);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
        memcpy(p_packet_copy, p_data, datalen);

        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.id                 = p_client->message_id;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);
        retransmission_packet.topic = *p_topic;

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       p_client->message_id,
                                                                       MQTTSN_MESSAGE_ID);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_unsubscribe(mqttsn_client_t * p_client,
                                          mqttsn_topic_t  * p_topic,
                                          uint16_t          topic_name_len)
{
    uint32_t  err_code    = NRF_SUCCESS;
    uint16_t  packet_len  = MQTTSN_PACKET_UNSUBSCRIBE_LENGTH + topic_name_len;
    uint8_t * p_data      = nrf_malloc(packet_len);
    uint8_t * p_packet_copy = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("UNSUBSCRIBE message cannot be allocated\r\n");
            break;
        }

        MQTTSN_topicid topic;
        memset(&topic, 0, sizeof(MQTTSN_topicid));
        topic.type            = MQTTSN_TOPIC_TYPE_NORMAL;
        topic.data.long_.name = (char *)(p_topic->p_topic_name);
        topic.data.long_.len  = (int)topic_name_len;

        uint16_t datalen =
            MQTTSNSerialize_unsubscribe(p_data, packet_len, next_packet_id_get(p_client), &topic);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
        memcpy(p_packet_copy, p_data, datalen);

        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.id                 = p_client->message_id;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);
        retransmission_packet.topic = *p_topic;

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       p_client->message_id,
                                                                       MQTTSN_MESSAGE_ID);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_disconnect(mqttsn_client_t * p_client, uint16_t duration)
{
    uint32_t              err_code = NRF_SUCCESS;
    uint16_t              packet_len;
    int16_t               disconnect_duration;
    mqttsn_client_state_t next_state;

    if (duration > 0)
    {
        disconnect_duration           = duration + (MQTTSN_DEFAULT_RETRANSMISSION_CNT + 1) *
                                                    MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS / 1000;
        next_state                    = MQTTSN_CLIENT_WAITING_FOR_SLEEP;
        packet_len                    = MQTTSN_PACKET_DISCONNECT_LENGTH + sizeof(duration);
        p_client->keep_alive.duration = duration * 1000;
    }
    else
    {
        disconnect_duration = MQTTSN_PACKET_DISCONNECT_DURATION;
        next_state          = MQTTSN_CLIENT_WAITING_FOR_DISCONNECT;
        packet_len          = MQTTSN_PACKET_DISCONNECT_LENGTH;
    }

    uint8_t * p_data = nrf_malloc(packet_len);

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("DISCONNECT message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = MQTTSNSerialize_disconnect(p_data, packet_len, disconnect_duration);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (err_code == NRF_SUCCESS)
    {
        p_client->client_state = next_state;
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_willtopicupd(mqttsn_client_t * p_client)
{
    uint8_t   will_qos      = 1;
    uint8_t   will_retain   = 0;

    uint32_t  err_code      = NRF_SUCCESS;
    uint16_t  packet_len    = MQTTSN_PACKET_WILLTOPIC_LENGTH + p_client->connect_info.will_topic_len;
    uint8_t * p_data        = nrf_malloc(packet_len);
    uint8_t * p_packet_copy   = NULL;

    do
    {
        if (p_data == NULL)
        {
            NRF_LOG_ERROR("WILLTOPICUPD message cannot be allocated\r\n");
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        MQTTSNString will_topic;
        memset(&will_topic, 0, sizeof(MQTTSNString));
        will_topic.lenstring.data = (char *)p_client->connect_info.p_will_topic;
        will_topic.lenstring.len  = p_client->connect_info.will_topic_len;

        uint16_t datalen = MQTTSNSerialize_willtopicupd(p_data, packet_len, will_qos, will_retain, will_topic);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
        memcpy(p_packet_copy, p_data, datalen);
        
        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t)); 
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.timeout            =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
    
        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       MQTTSN_MSGTYPE_WILLTOPICUPD,
                                                                       MQTTSN_MESSAGE_TYPE);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }
    
        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}

uint32_t mqttsn_packet_sender_willmsgupd(mqttsn_client_t * p_client)
{
    uint32_t  err_code    = NRF_SUCCESS;
    uint16_t  packet_len  = MQTTSN_PACKET_WILLMSG_LENGTH + p_client->connect_info.will_msg_len;
    uint8_t * p_data      = nrf_malloc(packet_len);
    uint8_t * p_packet_copy = NULL;

    do
    {
        if (p_data == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            NRF_LOG_ERROR("WILLMSGUPD message cannot be allocated\r\n");
            break;
        }

        uint16_t datalen = 0;

        MQTTSNString will_msg;
        memset(&will_msg, 0, sizeof(MQTTSNString));
        will_msg.lenstring.data = (char *)p_client->connect_info.p_will_msg;
        will_msg.lenstring.len  = p_client->connect_info.will_msg_len;

        datalen = MQTTSNSerialize_willmsgupd(p_data, packet_len, will_msg);
        if (datalen == 0)
        {
            err_code = NRF_ERROR_INVALID_PARAM;
            break;
        }

        p_packet_copy = nrf_malloc(datalen);
        if (p_packet_copy == NULL)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }
        memcpy(p_packet_copy, p_data, datalen);

        mqttsn_packet_t retransmission_packet;
        memset(&retransmission_packet, 0, sizeof(mqttsn_packet_t));
        retransmission_packet.retransmission_cnt = MQTTSN_DEFAULT_RETRANSMISSION_CNT;
        retransmission_packet.p_data             = p_packet_copy;
        retransmission_packet.len                = datalen;
        retransmission_packet.timeout =
            mqttsn_platform_timer_set_in_ms(MQTTSN_DEFAULT_RETRANSMISSION_TIME_IN_MS);

        if (mqttsn_packet_fifo_elem_add(p_client, &retransmission_packet) != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_NO_MEM;
            break;
        }

        if (mqttsn_client_timeout_schedule(p_client) != NRF_SUCCESS)
        {
            uint32_t fifo_dequeue_rc = mqttsn_packet_fifo_elem_dequeue(p_client,
                                                                       MQTTSN_MSGTYPE_WILLMSGUPD,
                                                                       MQTTSN_MESSAGE_TYPE);
            ASSERT(fifo_dequeue_rc == NRF_SUCCESS);
            err_code = NRF_ERROR_INTERNAL;
            break;
        }

        err_code = mqttsn_packet_sender_send(p_client, &(p_client->gateway_info.addr), p_data, datalen);

    } while (0);

    if (p_data)
    {
        nrf_free(p_data);
    }

    if (p_packet_copy && err_code != NRF_SUCCESS)
    {
        nrf_free(p_packet_copy);
    }

    return err_code;
}
