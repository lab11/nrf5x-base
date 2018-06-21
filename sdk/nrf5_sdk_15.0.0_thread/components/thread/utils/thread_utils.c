/**
 * Copyright (c) 2018 - 2018, Nordic Semiconductor ASA
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

#include "thread_utils.h"

#include "nrf_assert.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "nrf_timer.h"
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>
#include <openthread/cli.h>
#include <openthread/diag.h>

static otInstance * mp_ot_instance;

void thread_init(const thread_configuration_t * p_thread_configuration)
{
    otError error;

    PlatformInit(0, NULL);

    mp_ot_instance = otInstanceInitSingle();
    ASSERT(mp_ot_instance != NULL);

    NRF_LOG_INFO("Thread version: %s", (uint32_t)otGetVersionString());
    NRF_LOG_INFO("Network name:   %s",
                 (uint32_t)otThreadGetNetworkName(mp_ot_instance));

    if (!otDatasetIsCommissioned(mp_ot_instance) && p_thread_configuration->autocommissioning)
    {
        error = otLinkSetChannel(mp_ot_instance, THREAD_CHANNEL);
        ASSERT(error == OT_ERROR_NONE);

        error = otLinkSetPanId(mp_ot_instance, THREAD_PANID);
        ASSERT(error == OT_ERROR_NONE);
    }

    otLinkModeConfig mode;
    memset(&mode, 0, sizeof(mode));

    if (p_thread_configuration->role == RX_OFF_WHEN_IDLE)
    {
        mode.mSecureDataRequests = true;
        mode.mRxOnWhenIdle       = false; // Join network as SED.
        otLinkSetPollPeriod(mp_ot_instance, p_thread_configuration->poll_period);
    }
    else
    {
        mode.mRxOnWhenIdle       = true;
        mode.mSecureDataRequests = true;
        mode.mDeviceType         = true;
        mode.mNetworkData        = true;
    }

    error = otThreadSetLinkMode(mp_ot_instance, mode);
    ASSERT(error == OT_ERROR_NONE);

    if (p_thread_configuration->default_child_timeout != 0)
    {
        otThreadSetChildTimeout(mp_ot_instance, p_thread_configuration->default_child_timeout);
    }

    if (!p_thread_configuration->autostart_disable)
    {
        error = otIp6SetEnabled(mp_ot_instance, true);
        ASSERT(error == OT_ERROR_NONE);

        if (otDatasetIsCommissioned(mp_ot_instance) || p_thread_configuration->autocommissioning)
        {
            error = otThreadSetEnabled(mp_ot_instance, true);
            ASSERT(error == OT_ERROR_NONE);

            NRF_LOG_INFO("Thread interface has been enabled.");
            NRF_LOG_INFO("802.15.4 Channel: %d", otLinkGetChannel(mp_ot_instance));
            NRF_LOG_INFO("802.15.4 PAN ID:  0x%04x", otLinkGetPanId(mp_ot_instance));
            NRF_LOG_INFO("rx-on-when-idle:  %s", otThreadGetLinkMode(mp_ot_instance).mRxOnWhenIdle ?
                                            "enabled" : "disabled");
        }
    }
}

void thread_cli_init(void)
{
    ASSERT(mp_ot_instance != NULL);
    otCliUartInit(mp_ot_instance);
    otDiagInit(mp_ot_instance);
}

void thread_deinit(void)
{
    otInstanceFinalize(mp_ot_instance);
    PlatformDeinit();
    mp_ot_instance = NULL;
}

void thread_soft_deinit(void)
{
    otInstanceFinalize(mp_ot_instance);
    mp_ot_instance = NULL;
}

void thread_process(void)
{
    ASSERT(mp_ot_instance != NULL);
    otTaskletsProcess(mp_ot_instance);
    PlatformProcessDrivers(mp_ot_instance);
}

void fix_errata_78_in_nrf_802154(void)
{
    /* Temporary workaround for anomaly 78. When timer is stopped using STOP task
     * its power consumption will be higher.
     * Issuing task SHUTDOWN fixes this condition. This have to be removed when
     * proper fix will be pushed to nRF-IEEE-802.15.4-radio-driver.
     */
    if ((otPlatRadioGetState(thread_ot_instance_get()) == OT_RADIO_STATE_SLEEP) ||
        (otPlatRadioGetState(thread_ot_instance_get()) == OT_RADIO_STATE_DISABLED))
    {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_SHUTDOWN);
    }
}

void thread_sleep(void)
{
    ASSERT(mp_ot_instance != NULL);

    // Enter sleep state if no more tasks are pending.
    if (!otTaskletsArePending(mp_ot_instance))
    {
        fix_errata_78_in_nrf_802154();

#ifdef SOFTDEVICE_PRESENT
        ret_code_t err_code = sd_app_evt_wait();
        ASSERT(err_code == NRF_SUCCESS);
#else
        __WFE();
#endif
    }
}

otInstance * thread_ot_instance_get(void)
{
    return mp_ot_instance;
}

void thread_state_changed_callback_set(thread_state_change_callback_t handler)
{
    ASSERT(mp_ot_instance != NULL);

    otError error = otSetStateChangedCallback(mp_ot_instance, handler, mp_ot_instance);
    ASSERT(error == OT_ERROR_NONE);
}

bool thread_soft_reset_was_requested(void)
{
    return PlatformPseudoResetWasRequested();
}
