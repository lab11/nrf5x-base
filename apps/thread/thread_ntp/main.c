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

#include "thread_dns.h"
#include "thread_ntp.h"
#include "simple_thread.h"

#include "openthread/sntp.h"
#include "openthread/ip6.h"

APP_TIMER_DEF(ntp_update_timer);

#define COAP_SERVER_HOSTNAME "google.com"
#define NTP_SERVER_HOSTNAME "us.pool.ntp.org"
#define DNS_SERVER_ADDR "fdaa:bb:1::2"

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

#define DEFAULT_CHILD_TIMEOUT    40 /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      10 /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define NUM_SLAAC_ADDRESSES      4  /**< Number of SLAAC addresses. */

static otIp6Address m_peer_address =
{
    .mFields =
    {
        .m8 = {0}
    }
};

static const otIp6Address m_unspecified_ipv6 =
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

void ntp_response_handler(void* context, uint64_t time, otError error) {
  NRF_LOG_INFO("got time: %lu", (uint32_t)time & UINT32_MAX);
}

void ntp_update_callback() {
  otError error = OT_ERROR_NONE;
  otInstance* thread_instance = thread_get_instance();
  // If IPv6 address of the ntp server is unspecified try to resolve hostname.
  if (otIp6IsAddressEqual(&m_peer_address, &m_unspecified_ipv6))
  {
    thread_dns_hostname_resolve(thread_instance,
              DNS_SERVER_ADDR,
              NTP_SERVER_HOSTNAME,
              dns_response_handler,
              (void*) &m_peer_address);
    NRF_LOG_INFO("DNS ERROR: %d", error);
    return;
  }

  NRF_LOG_INFO("sent ntp poll!");
  error = thread_ntp_request(thread_instance, &m_peer_address, NULL, ntp_response_handler);
  NRF_LOG_INFO("error: %d", error);
}

int main(void) {
    nrf_power_dcdcen_set(1);

    // Initialize.
    log_init();

    thread_config_t thread_config = {
      .channel = 25,
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
        if (NRF_LOG_PROCESS() == false)
        {
          thread_sleep();
        }
    }
}
