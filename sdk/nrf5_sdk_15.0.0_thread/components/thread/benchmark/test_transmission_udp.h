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
/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#ifndef TEST_TRANSMISSION_UDP_H__
#define TEST_TRANSMISSION_UDP_H__

#include <stdint.h>

#include "thread_benchmark.h"

#define TEST_TRANSMISSION_UDP_SOURCE_PORT        22222 /**< Peer UDP port used as the source port for all tests. */
#define TEST_TRANSMISSION_UDP_LISTENER_PORT      33333 /**< Peer UDP port used for the UNIDIRECTIONAL test. */
#define TEST_TRANSMISSION_UDP_REPEATER_PORT      44444 /**< Peer UDP port used for the ECHO test. */
#define TEST_TRANSMISSION_UDP_ACK_RESPONDER_PORT 55555 /**< Peer UDP port used for the ACK reply test. */

#define MAX_PAYLOAD_SIZE  1024 /**< Maximal size of UDP payload in the test packet [B].  */

typedef union
{
    uint8_t  payload[MAX_PAYLOAD_SIZE];
    uint32_t current_frame_number;
} frame_payload_t;

/**@brief Structure storing information about current test frame used in sending acknowledgements. */
typedef struct
{   
    uint16_t          payload_length;
    frame_payload_t * p_payload;
    otMessageInfo     message_info;
} frame_information_t;

/**@brief Function that creates sockets for defined test UDP ports. */
void test_transmission_udp_init(void);

/**@brief Function that handles the UDP test transmission procedure until it sends given number of packets. */
thread_benchmark_test_status_t test_transmission_udp_packet_transmit(thread_benchmark_configuration_t * p_test_conf,
                                                                     thread_benchmark_status_t        * p_test_status);

#endif // TEST_TRANSMISSION_UDP_H__

/** @}
 *  @endcond
 */
