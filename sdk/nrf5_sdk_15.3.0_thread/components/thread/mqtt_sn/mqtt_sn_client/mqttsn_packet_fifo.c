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

#include "mqttsn_packet_internal.h"
#include "mem_manager.h"
#include "nrf_error.h"
#include "nrf_log.h"

void mqttsn_packet_fifo_init(mqttsn_client_t * p_client)
{
    memset(&p_client->packet_queue, 0, sizeof(mqttsn_packet_queue_t));
}

void mqttsn_packet_fifo_uninit(mqttsn_client_t * p_client)
{
    memset(&p_client->packet_queue, 0, sizeof(mqttsn_packet_queue_t));
}

uint32_t mqttsn_packet_fifo_elem_add(mqttsn_client_t * p_client, mqttsn_packet_t * packet)
{
    if (p_client->packet_queue.num_of_elements == MQTTSN_PACKET_FIFO_MAX_LENGTH)
    {
        NRF_LOG_ERROR("Packet ID fifo capacity exceeded\r\n");
        return NRF_ERROR_NO_MEM;
    }
    else
    {
        p_client->packet_queue.packet[p_client->packet_queue.num_of_elements] = *packet;

        p_client->packet_queue.num_of_elements++;
        return NRF_SUCCESS;
    }
}

uint32_t mqttsn_packet_fifo_elem_dequeue(mqttsn_client_t * p_client, uint16_t msg_to_dequeue, mqttsn_packet_dequeue_t mode)
{
    uint32_t elem_to_dequeue = mqttsn_packet_fifo_elem_find(p_client, msg_to_dequeue, mode);
    if (elem_to_dequeue == MQTTSN_PACKET_FIFO_MAX_LENGTH)
    {
        NRF_LOG_ERROR("Cannot dequeue packet. Packet does not exist\r\n");
        return NRF_ERROR_NOT_FOUND;
    }

    nrf_free(p_client->packet_queue.packet[elem_to_dequeue].p_data);

    for (int i = elem_to_dequeue; i < p_client->packet_queue.num_of_elements; i++)
    {
        p_client->packet_queue.packet[i] = p_client->packet_queue.packet[i+1];
    }

    p_client->packet_queue.num_of_elements--;

    return NRF_SUCCESS;
}

uint32_t mqttsn_packet_fifo_elem_find(mqttsn_client_t * p_client, uint16_t msg_to_dequeue, mqttsn_packet_dequeue_t mode)
{
    uint32_t index       = MQTTSN_PACKET_FIFO_MAX_LENGTH;
    uint32_t type_offset = 0;

    for (int i = 0; i < p_client->packet_queue.num_of_elements; i++)
    {
        switch (mode)
        {
            case MQTTSN_MESSAGE_ID:
                if (p_client->packet_queue.packet[i].id == msg_to_dequeue)
                {
                    return i;
                }
                break;

            case MQTTSN_MESSAGE_TYPE:

                type_offset = p_client->packet_queue.packet[i].p_data[0] == MQTTSN_TWO_BYTE_LENGTH_CODE ? 
                              MQTTSN_OFFSET_TWO_BYTE_LENGTH : MQTTSN_OFFSET_ONE_BYTE_LENGTH;
                
                if (p_client->packet_queue.packet[i].p_data[type_offset] == msg_to_dequeue)
                {
                    return i;
                }
                break;

            default:
                break;
        }
    }
    return index;
}
