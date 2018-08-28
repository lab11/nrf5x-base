#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_timer.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_power.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include <openthread/openthread.h>

#include "simple_thread.h"
#include "ntp.h"

APP_TIMER_DEF(ntp_update_timer);

#define NTP_SERVER_ADDR "64:ff9b::8106:f1c"

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

#define DEFAULT_CHILD_TIMEOUT    40                                         /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      5000                                       /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define NUM_SLAAC_ADDRESSES      4                                          /**< Number of SLAAC addresses. */

static struct ntp_client_t ntp_client;

static otIp6Address m_peer_address =
{
    .mFields =
    {
        .m8 = {0}
    }
};

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void ntp_recv_callback(struct ntp_client_t* client) {
  NRF_LOG_INFO("time: %lu", client->tv.tv_sec);
}

void ntp_update_callback() {
  NRF_LOG_INFO("sent ntp poll!");
  int error = ntp_client_begin(thread_get_instance(), &ntp_client, &m_peer_address, 123, 127, ntp_recv_callback, NULL);
  NRF_LOG_INFO("error: %d", error);
  if (error) {
    memset(&ntp_client, 0, sizeof(struct ntp_client_t));
  }
}

int main(void) {
    nrf_power_dcdcen_set(1);

    log_init();

    // Initialize.
    //nrf_gpio_cfg_output(LED2);
    //nrf_gpio_pin_clear(LED2);

    otIp6AddressFromString(NTP_SERVER_ADDR, &m_peer_address);

    thread_config_t thread_config = {
      .channel = 11,
      .panid = 0xFACE,
      .sed = true,
      .poll_period = DEFAULT_POLL_PERIOD,
      .child_period = DEFAULT_CHILD_TIMEOUT,
      .autocommission = true,
    };

    thread_init(&thread_config);

    app_timer_init();
    app_timer_create(&ntp_update_timer, APP_TIMER_MODE_REPEATED, ntp_update_callback);
    app_timer_start(ntp_update_timer, APP_TIMER_TICKS(10000), NULL);

    // Enter main loop.
    while (1) {
        thread_process();
        ntp_client_process(&ntp_client);
        if (NRF_LOG_PROCESS() == false)
        {
          thread_sleep();
        }
    }
}
