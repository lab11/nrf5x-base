/**
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#include "freertos_mbedtls_mutex.h"

#include "FreeRTOS.h"
#include "nrf_assert.h"

#include <semphr.h>
#include <mbedtls/entropy.h>
#include <mbedtls/threading.h>

__STATIC_INLINE void mutex_init(mbedtls_threading_mutex_t * p_mutex)
{
    ASSERT(p_mutex != NULL);
    *p_mutex = xSemaphoreCreateMutex();
    ASSERT(*p_mutex != NULL);
}

__STATIC_INLINE void mutex_free(mbedtls_threading_mutex_t * p_mutex)
{
    ASSERT(p_mutex != NULL);
    ASSERT(*p_mutex != NULL);
    vSemaphoreDelete(*p_mutex);
}

__STATIC_INLINE int mutex_lock(mbedtls_threading_mutex_t * p_mutex)
{
    ASSERT(p_mutex != NULL);
    ASSERT(*p_mutex != NULL);
    return xSemaphoreTake(*p_mutex, portMAX_DELAY) != pdTRUE;
}

__STATIC_INLINE int mutex_unlock(mbedtls_threading_mutex_t * p_mutex)
{
    ASSERT(p_mutex != NULL);
    ASSERT(*p_mutex != NULL);
    return xSemaphoreGive(*p_mutex) != pdTRUE;
}

void freertos_mbedtls_mutex_init(void)
{
    mbedtls_threading_set_alt(mutex_init, mutex_free, mutex_lock, mutex_unlock);
}

void freertos_mbedtls_mutex_free(void)
{
    mbedtls_threading_free_alt();
}
