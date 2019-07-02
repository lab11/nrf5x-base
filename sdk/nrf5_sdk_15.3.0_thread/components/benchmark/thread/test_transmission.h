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
/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#ifndef TEST_TRANSMISSION_H__
#define TEST_TRANSMISSION_H__

#include "benchmark_api.h"

/**@brief Maximum possible size of a data structure describing a single frame (excluding payload). */
#define TEST_TRANSMISSION_FRAME_INFO_MAX_SIZE  BENCHMARK_SCHED_MAX_EVENT_DATA_SIZE

/**@brief Function that initializes the test transmission. */
void test_transmission_init(void);

/**@brief Function that handles the test transmission procedure until it sends given number of packets.
 *
 * @param[inout]  p_test_conf    Current test configuration.
 * @param[inout]  p_test_status  Current status of the benchmark application.
 *
 * @returns  Status of the test.
*/
benchmark_test_state_t test_transmission_packet_transmit(benchmark_configuration_t * p_test_conf,
                                                          benchmark_status_t        * p_test_status);

/**************************************************************************************************
 * Internal API.
 *************************************************************************************************/

/**@brief   Function that returns test results structure pointer.
 *
 * @retval A pointer to the benchmark result structure.
 */
benchmark_result_t * benchmark_local_result_get(void);

/**@brief   Function that triggers calculation of the test duration. */
void benchmark_test_duration_calculate(void);

#endif // TEST_TRANSMISSION_H__

/** @}
 *  @endcond
 */
