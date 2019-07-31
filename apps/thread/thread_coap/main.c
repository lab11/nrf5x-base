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

#include "simple_thread.h"
#include "thread_coap.h"

APP_TIMER_DEF(coap_send_timer);

#define COAP_SERVER_ADDR "64:ff9b::23a6:b3ac"

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

#define DEFAULT_CHILD_TIMEOUT    40                                         /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      1000                                       /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define NUM_SLAAC_ADDRESSES      4                                          /**< Number of SLAAC addresses. */

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


void send_timer_callback() {
  const uint8_t* data = (uint8_t*)"hello";
  otInstance* thread_instance = thread_get_instance();
  thread_coap_send(thread_instance, OT_COAP_CODE_PUT, OT_COAP_TYPE_NON_CONFIRMABLE, &m_peer_address, "test", data, strnlen((char*)data, 6));
  NRF_LOG_INFO("Sent test message!");
}

int main(void) {
//#ifdef SOFTDEVICE_PRESENT
//    nrf_sdh_enable_request();
//    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
//#else
    nrf_power_dcdcen_set(1);
//#endif

    log_init();

    // Initialize.
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_pin_set(LED2);

    otIp6AddressFromString(COAP_SERVER_ADDR, &m_peer_address);

    thread_config_t thread_config = {
      .channel = 25,
      .panid = 0xFACE,
      .sed = true,
      .poll_period = DEFAULT_POLL_PERIOD,
      .child_period = DEFAULT_CHILD_TIMEOUT,
      .autocommission = true,
    };

    thread_init(&thread_config);
    otInstance* thread_instance = thread_get_instance();
    thread_coap_client_init(thread_instance);

    app_timer_init();
    app_timer_create(&coap_send_timer, APP_TIMER_MODE_REPEATED, send_timer_callback);
    app_timer_start(coap_send_timer, APP_TIMER_TICKS(5000), NULL);

    // Enter main loop.
    while (1) {
        thread_process();
        if (NRF_LOG_PROCESS() == false)
        {
          thread_sleep();
        }
    }
}
