/* Blink
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_power.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include <openthread/dataset_ftd.h>
#include <openthread/thread_ftd.h>

#include "simple_thread.h"

#define LED0 NRF_GPIO_PIN_MAP(0,6)
#define LED1 NRF_GPIO_PIN_MAP(0,8)
#define LED2 NRF_GPIO_PIN_MAP(1,9)
#define LED3 NRF_GPIO_PIN_MAP(0,12)

#define DEFAULT_CHILD_TIMEOUT    40                                         /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      1000                                       /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define NUM_SLAAC_ADDRESSES      4                                          /**< Number of SLAAC addresses. */

static otNetifAddress m_slaac_addresses[6];               /**< Buffer containing addresses resolved by SLAAC */
void __attribute__((weak)) thread_state_changed_callback(uint32_t flags, void * p_context)
{
    uint32_t role = otThreadGetDeviceRole(p_context);
    NRF_LOG_INFO("State changed! Flags: 0x%08lx Current role: %d\r\n",
                 flags, otThreadGetDeviceRole(p_context));

    if (flags & OT_CHANGED_THREAD_NETDATA)
    {
        /**
         * Whenever Thread Network Data is changed, it is necessary to check if generation of global
         * addresses is needed. This operation is performed internally by the OpenThread CLI module.
         * To lower power consumption, the examples that implement Thread Sleepy End Device role
         * don't use the OpenThread CLI module. Therefore otIp6SlaacUpdate util is used to create
         * IPv6 addresses.
         */
         otIp6SlaacUpdate(p_context, m_slaac_addresses,
                          sizeof(m_slaac_addresses) / sizeof(m_slaac_addresses[0]),
                          otIp6CreateRandomIid, NULL);
    }
    nrf_gpio_pin_set(LED1);
    nrf_gpio_pin_set(LED2);
    nrf_gpio_pin_set(LED3);
    if (role == OT_DEVICE_ROLE_CHILD) {
        nrf_gpio_pin_clear(LED1);
    }
    if (role == OT_DEVICE_ROLE_ROUTER) {
        nrf_gpio_pin_clear(LED2);
    }
    if (role == OT_DEVICE_ROLE_LEADER) {
        nrf_gpio_pin_clear(LED3);
    }
}

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

int main(void) {
    nrf_power_dcdcen_set(1);

    log_init();

    // Initialize.
    nrf_gpio_cfg_output(LED0);
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_pin_set(LED1);
    nrf_gpio_pin_set(LED2);
    nrf_gpio_pin_set(LED3);
    nrf_gpio_pin_clear(LED0);

    thread_config_t thread_config = {
      .channel = 25,
      .tx_power = 8,
      .panid = 0xFACE,
      .sed = false,
      //.poll_period = DEFAULT_POLL_PERIOD,
      //.child_period = DEFAULT_CHILD_TIMEOUT,
      .autocommission = true,
    };

    thread_init(&thread_config);

    // Enter main loop.
    while (1) {
        thread_process();
        if (NRF_LOG_PROCESS() == false)
        {
          thread_sleep();
        }
    }
}
