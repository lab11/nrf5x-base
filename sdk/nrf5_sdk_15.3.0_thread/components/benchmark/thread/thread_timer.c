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

#include "app_util.h"
#include "timer_api.h"
#include "nrf_timer.h"

#define NRF_TIMER_INST            NRF_TIMER4                       /**< Timer used for BLE messages. */
#define TIMER_CC_CHANNEL_COUNT    NRF_TIMER_CC_CHANNEL_COUNT(4)    /**< Number of timer channels. */
#define TIMER_FREQUENCY_KHZ       31                               /**< Approximate timer frequency in kHz. */
#define TIMER_DISABLE_ALL         UINT32_MAX                       /**< Mask for disabling all register bits. */
#define TIMER_PING_FREQ_CHANNEL   NRF_TIMER_CC_CHANNEL0            /**< Channel for interval timing. */
#define TIMER_THROUGHPUT_CHANNEL  NRF_TIMER_CC_CHANNEL1            /**< Channel for throughput calculation. */
#define SECONDS_TO_MILLISECONDS   1000                             /**< Seconds to milliseconds conversion constant. */

static timer_cb_t m_timer_cb;

void TIMER4_IRQHandler(void)
{
    if (nrf_timer_event_check(NRF_TIMER_INST, NRF_TIMER_EVENT_COMPARE0))
    {
        nrf_timer_event_clear(NRF_TIMER_INST, NRF_TIMER_EVENT_COMPARE0);
        if (m_timer_cb)
        {
            m_timer_cb();
        }
    }
}

uint32_t timer_capture(void)
{
    nrf_timer_task_trigger(NRF_TIMER_INST, NRF_TIMER_TASK_CAPTURE1);
    return nrf_timer_cc_read(NRF_TIMER_INST, TIMER_THROUGHPUT_CHANNEL);
}

void timer_init(void)
{
    m_timer_cb = NULL;

    nrf_timer_shorts_disable(NRF_TIMER_INST, TIMER_DISABLE_ALL);
    nrf_timer_int_enable(NRF_TIMER_INST, NRF_TIMER_INT_COMPARE0_MASK);

    for (uint32_t i = 0; i < TIMER_CC_CHANNEL_COUNT; i++)
    {
        nrf_timer_event_clear(NRF_TIMER_INST, nrf_timer_compare_event_get(i));
    }

    nrf_timer_mode_set(NRF_TIMER_INST, NRF_TIMER_MODE_TIMER);
    nrf_timer_frequency_set(NRF_TIMER_INST, NRF_TIMER_FREQ_31250Hz);
    nrf_timer_bit_width_set(NRF_TIMER_INST, NRF_TIMER_BIT_WIDTH_32);

    NVIC_ClearPendingIRQ(TIMER4_IRQn);
    NVIC_SetPriority(TIMER4_IRQn, APP_IRQ_PRIORITY_LOWEST);
    NVIC_EnableIRQ(TIMER4_IRQn);
}

void timer_set(uint32_t timeout_ms, timer_cb_t p_callback)
{
    uint32_t current_ticks;
    uint32_t target_time_ticks;

    m_timer_cb = p_callback;

    nrf_timer_task_trigger(NRF_TIMER_INST, NRF_TIMER_TASK_CAPTURE0);
    current_ticks = nrf_timer_cc_read(NRF_TIMER_INST, TIMER_PING_FREQ_CHANNEL);

    target_time_ticks = current_ticks + nrf_timer_ms_to_ticks(timeout_ms, NRF_TIMER_FREQ_31250Hz);
    nrf_timer_cc_write(NRF_TIMER_INST, TIMER_PING_FREQ_CHANNEL, target_time_ticks);
}

void timer_start(void)
{
    nrf_timer_task_trigger(NRF_TIMER_INST, NRF_TIMER_TASK_START);
}

void timer_stop(void)
{
    nrf_timer_task_trigger(NRF_TIMER_INST, NRF_TIMER_TASK_STOP);
    nrf_timer_int_disable(NRF_TIMER_INST, TIMER_DISABLE_ALL);

    NVIC_DisableIRQ(TIMER4_IRQn);
    NVIC_ClearPendingIRQ(TIMER4_IRQn);
    NVIC_SetPriority(TIMER4_IRQn, 0);
}

uint32_t timer_ticks_to_ms(uint32_t ticks)
{
    return ROUNDED_DIV(ticks, TIMER_FREQUENCY_KHZ);
}
