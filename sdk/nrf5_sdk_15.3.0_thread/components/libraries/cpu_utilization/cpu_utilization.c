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
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(CPU_UTILIZATION)
#include "cpu_utilization.h"
#include "nrfx_log.h"
#include "nrfx_timer.h"
#include "nrfx_ppi.h"
#include "nrfx_power.h"
#include "nrf_assert.h"
#if defined(CPU_UTILIZATION_GPIOTE_ENABLED) && (CPU_UTILIZATION_GPIOTE_ENABLED == 1)
#include "nrfx_gpiote.h"
#endif //CPU_UTILIZATION_GPIOTE_ENABLED

/**@brief Timers used to measure total test and sleep time. */
static nrfx_timer_t const m_measurement_timer = NRFX_TIMER_INSTANCE(CPU_UTILIZATION_MEASUREMENT_TIMER);
static nrfx_timer_t const m_sleep_timer       = NRFX_TIMER_INSTANCE(CPU_UTILIZATION_SLEEP_TIMER);

static nrf_ppi_channel_t m_ppi_sleep_enter_channel; /**< Peripheral Peripheral Interconnect used to start measurement of the sleep time. */
static nrf_ppi_channel_t m_ppi_sleep_exit_channel;  /**< Peripheral Peripheral Interconnect used to stop measurement of the sleep time. */

static volatile uint32_t m_measurement_timer_msw; /**< Incremented when m_measurement_timer overflows. */
static volatile uint32_t m_sleep_timer_msw;       /**< Incremented when m_sleep_timer overflows. */


/**@brief Timer Event Handler handling timer overflows. */
static void timer_event_handler(nrf_timer_event_t evt, void * p_context)
{
    switch (evt)
    {
        // Increment msw variable in case of timer overflow.
        case NRF_TIMER_EVENT_COMPARE1:
            if (p_context == (void *)&m_measurement_timer)
            {
                m_measurement_timer_msw++;
            }
            else if (p_context == (void *)&m_sleep_timer)
            {
                m_sleep_timer_msw++;
            }
            else
            {
                ASSERT(false);
            }
            break;

        default:
            break;
    }
}

/**@brief Function that initializes timers and configures overflow management. */
static nrfx_err_t timers_init(void)
{
    nrfx_err_t err;

    nrfx_timer_config_t timer_config = 
    {
        .frequency          = CPU_UTILIZATION_TIMER_FREQUENCY,
        .mode               = NRF_TIMER_MODE_TIMER,
        .bit_width          = NRF_TIMER_BIT_WIDTH_32,
        .interrupt_priority = 7,
    };

    timer_config.p_context = (void *)&m_measurement_timer;
    err                    = nrfx_timer_init(&m_measurement_timer, &timer_config, timer_event_handler);
    VERIFY_SUCCESS(err);

    timer_config.p_context = (void *)&m_sleep_timer;
    err                    = nrfx_timer_init(&m_sleep_timer, &timer_config, timer_event_handler);
    VERIFY_SUCCESS(err);

    nrfx_timer_compare(&m_measurement_timer, NRF_TIMER_CC_CHANNEL1, UINT32_MAX, true);
    nrfx_timer_compare(&m_sleep_timer, NRF_TIMER_CC_CHANNEL1, UINT32_MAX, true);

    return err;
}

static void timers_deinit(void)
{
    nrfx_timer_uninit(&m_measurement_timer);
    nrfx_timer_uninit(&m_sleep_timer);
}

/**@brief PPI for timers and GPIO pin output. */
static nrfx_err_t ppi_init(void)
{
    nrfx_err_t err;

    err = nrfx_ppi_channel_alloc(&m_ppi_sleep_enter_channel);
    VERIFY_SUCCESS(err);

    err = nrfx_ppi_channel_alloc(&m_ppi_sleep_exit_channel);
    VERIFY_SUCCESS(err);

    err = nrfx_ppi_channel_assign(m_ppi_sleep_enter_channel,
                                  (uint32_t)&NRF_POWER->EVENTS_SLEEPENTER,
                                  nrfx_timer_task_address_get(&m_sleep_timer,
                                                              NRF_TIMER_TASK_START));
    VERIFY_SUCCESS(err);

    err = nrfx_ppi_channel_assign(m_ppi_sleep_exit_channel,
                                  (uint32_t)&NRF_POWER->EVENTS_SLEEPEXIT,
                                  nrfx_timer_task_address_get(&m_sleep_timer,
                                                              NRF_TIMER_TASK_STOP));
    VERIFY_SUCCESS(err);

#if defined(CPU_UTILIZATION_GPIOTE_ENABLED) && (CPU_UTILIZATION_GPIOTE_ENABLED == 1)
    err = nrfx_ppi_channel_fork_assign(m_ppi_sleep_enter_channel, 
                                      nrfx_gpiote_out_task_addr_get(CPU_UTILIZATION_GPIOTE_PIN));
    VERIFY_SUCCESS(err);

    err = nrfx_ppi_channel_fork_assign(m_ppi_sleep_exit_channel, 
                                       nrfx_gpiote_out_task_addr_get(CPU_UTILIZATION_GPIOTE_PIN));
    VERIFY_SUCCESS(err);
#endif // CPU_UTILIZATION_GPIOTE_ENABLED

    return err;
}

static nrfx_err_t ppi_deinint(void)
{
    nrfx_err_t err;

    err = nrfx_ppi_channel_free(m_ppi_sleep_enter_channel);
    VERIFY_SUCCESS(err);

    err = nrfx_ppi_channel_free(m_ppi_sleep_exit_channel);
    VERIFY_SUCCESS(err);

    return err;
}

#if defined(CPU_UTILIZATION_GPIOTE_ENABLED) && (CPU_UTILIZATION_GPIOTE_ENABLED == 1)

/**@brief Function that initializes GPIO pin. */
static nrfx_err_t gpiote_init(void)
{
    nrfx_err_t err;
    nrfx_gpiote_out_config_t config = NRFX_GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);

    err = nrfx_gpiote_out_init(CPU_UTILIZATION_GPIOTE_PIN, &config);
    NRF_LOG_INFO("GPIOTE: %d", err);
    
    nrfx_gpiote_out_task_enable(CPU_UTILIZATION_GPIOTE_PIN);

    return err;
}

static void gpiote_deinit(void)
{
    nrfx_gpiote_out_task_disable(CPU_UTILIZATION_GPIOTE_PIN);
    nrfx_gpiote_out_uninit(CPU_UTILIZATION_GPIOTE_PIN);
}

#endif // CPU_UTILIZATION_GPIOTE_ENABLED

/**@brief Function for clearing the CPU Utilization timer values. */
void cpu_utilization_clear(void)
{
    m_measurement_timer_msw = 0;
    m_sleep_timer_msw       = 0;

    nrfx_timer_clear(&m_measurement_timer);
    nrfx_timer_clear(&m_sleep_timer);
}

/**@brief Function for initializing the CPU Utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_init(void)
{
    nrfx_err_t err;
#if defined(CPU_UTILIZATION_GPIOTE_ENABLED) && (CPU_UTILIZATION_GPIOTE_ENABLED == 1)
    err = gpiote_init();
    VERIFY_SUCCESS(err);
#endif // CPU_UTILIZATION_GPIOTE_ENABLED

    err = ppi_init();
    VERIFY_SUCCESS(err);

    err = timers_init();
    VERIFY_SUCCESS(err);

    nrfx_timer_enable(&m_measurement_timer);
    nrfx_timer_pause(&m_measurement_timer);

    cpu_utilization_clear();

    return err;
}

/**@brief Function for deinitializing the CPU Utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_deinit(void)
{
#if defined(CPU_UTILIZATION_GPIOTE_ENABLED) && (CPU_UTILIZATION_GPIOTE_ENABLED == 1)
    gpiote_deinit();
#endif // CPU_UTILIZATION_GPIOTE_ENABLED

    nrfx_err_t err = ppi_deinint();
    VERIFY_SUCCESS(err);

    nrfx_timer_disable(&m_measurement_timer);
    timers_deinit();

    return err;
}

/**@brief Function for starting the CPU Utilization module.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_start(void)
{
    nrfx_err_t err;

    err = nrfx_ppi_channel_enable(m_ppi_sleep_enter_channel);
    ASSERT(err == NRF_SUCCESS);

    err = nrfx_ppi_channel_enable(m_ppi_sleep_exit_channel);
    ASSERT(err == NRF_SUCCESS);

    nrfx_timer_resume(&m_measurement_timer);

    return err;
}

/**@brief Function for stopping the CPU Utilization module
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t cpu_utilization_stop(void)
{
    nrfx_err_t err;

    err = nrfx_ppi_channel_disable(m_ppi_sleep_enter_channel);
    ASSERT(err == NRF_SUCCESS);

    err = nrfx_ppi_channel_disable(m_ppi_sleep_exit_channel);
    ASSERT(err == NRF_SUCCESS);

    nrfx_timer_pause(&m_measurement_timer);

    return err;
}

/**@brief Function for getting the CPU utilization.
 *
 * @retval  CPU utilization in units of 0.01%
 */
uint32_t cpu_utilization_get(void)
{
    uint32_t time_measurement_timer_value;
    uint32_t time_sleep_timer_value;
    uint32_t measurement_timer_msw;
    uint32_t sleep_timer_msw;
    uint32_t measurement_timer_msw_new;
    uint32_t sleep_timer_msw_new;

    measurement_timer_msw  = m_measurement_timer_msw;
    sleep_timer_msw        = m_sleep_timer_msw;

    time_measurement_timer_value = nrfx_timer_capture(&m_measurement_timer, NRF_TIMER_CC_CHANNEL0);
    time_sleep_timer_value       = nrfx_timer_capture(&m_sleep_timer, NRF_TIMER_CC_CHANNEL0);

    measurement_timer_msw_new  = m_measurement_timer_msw;
    sleep_timer_msw_new        = m_sleep_timer_msw;

    if ((measurement_timer_msw != measurement_timer_msw_new)
        || (sleep_timer_msw != sleep_timer_msw_new))
    {
        measurement_timer_msw = measurement_timer_msw_new;
        sleep_timer_msw       = sleep_timer_msw_new;

        time_measurement_timer_value = nrfx_timer_capture(&m_measurement_timer, NRF_TIMER_CC_CHANNEL0);
        time_sleep_timer_value       = nrfx_timer_capture(&m_sleep_timer, NRF_TIMER_CC_CHANNEL0);
    }

    uint64_t measurement_time = ((uint64_t)measurement_timer_msw << 32) | time_measurement_timer_value;
    uint64_t sleep_time       = ((uint64_t)sleep_timer_msw << 32) | time_sleep_timer_value;

    if (measurement_time)
    {
        return (uint32_t)(10000 - (10000 * sleep_time / measurement_time));
    }
    else
    {
        return 0;
    }
}
#endif //NRF_MODULE_ENABLED(CPU_UTILIZATION)

