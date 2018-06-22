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
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"

#include <openthread/openthread.h>
#include <openthread/icmp6.h>
#include <openthread/platform/platform.h>
#include <openthread/cli.h>
#include <openthread/diag.h>


#define UART_RX              26
#define UART_TX              27
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

#define THREAD_PANID 0xFACE
#define THREAD_CHANNEL 11
#define AUTOCOMMISSION 1
#define DEFAULT_CHILD_TIMEOUT    40                                         /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      1000                                       /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define SHORT_POLL_PERIOD        100                                        /**< Thread Sleepy End Device polling period when MQTT-SN Awake. [ms] */
#define NUM_SLAAC_ADDRESSES      4                                          /**< Number of SLAAC addresses. */

static otInstance * mp_ot_instance;
static otNetifAddress m_slaac_addresses[NUM_SLAAC_ADDRESSES];               /**< Buffer containing addresses resolved by SLAAC */

/**@brief Function for initializing the Thread Stack.
 */

void uart_error_handle (app_uart_evt_t * p_event) {
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void uart_init(void) {
  uint32_t err_code;

  const app_uart_comm_params_t comm_params =
  {
    UART_RX,
    UART_TX,
    0,
    0,
    APP_UART_FLOW_CONTROL_DISABLED,
    false,
    NRF_UARTE_BAUDRATE_115200
  };
  APP_UART_FIFO_INIT(&comm_params,
                     UART_RX_BUF_SIZE,
                     UART_TX_BUF_SIZE,
                     uart_error_handle,
                     APP_IRQ_PRIORITY_LOW,
                     err_code);
  APP_ERROR_CHECK(err_code);

}

void thread_init()
{
    otError error;

    PlatformInit(0, NULL);

    mp_ot_instance = otInstanceInitSingle();
    ASSERT(mp_ot_instance != NULL);

    printf("Thread version: %s\r\n", otGetVersionString());
    printf("Network name:   %s\r\n",
                 otThreadGetNetworkName(mp_ot_instance));

    if (!otDatasetIsCommissioned(mp_ot_instance) && AUTOCOMMISSION)
    {
        error = otLinkSetChannel(mp_ot_instance, THREAD_CHANNEL);
        ASSERT(error == OT_ERROR_NONE);
        printf("Thread Channel: %d\r\n", otLinkGetChannel(mp_ot_instance));

        error = otLinkSetPanId(mp_ot_instance, THREAD_PANID);
        ASSERT(error == OT_ERROR_NONE);
        printf("Thread PANID: %lx\r\n", (uint32_t)otLinkGetPanId(mp_ot_instance));
    }

    otLinkModeConfig mode;
    memset(&mode, 0, sizeof(mode));
    // sleepy end device
    mode.mSecureDataRequests = true;
    mode.mRxOnWhenIdle       = false; // Join network as SED.
    otLinkSetPollPeriod(mp_ot_instance, DEFAULT_POLL_PERIOD);
    // regular end device
    //mode.mRxOnWhenIdle       = true;
    //mode.mSecureDataRequests = true;
    //mode.mDeviceType         = true;
    //mode.mNetworkData        = true;

    error = otThreadSetLinkMode(mp_ot_instance, mode);
    ASSERT(error == OT_ERROR_NONE);

    otThreadSetChildTimeout(mp_ot_instance, DEFAULT_CHILD_TIMEOUT);

    //if (!p_thread_configuration->autostart_disable)
    //{
    //    error = otIp6SetEnabled(mp_ot_instance, true);
    //    ASSERT(error == OT_ERROR_NONE);

    //    if (otDatasetIsCommissioned(mp_ot_instance) || p_thread_configuration->autocommissioning)
    //    {
    //        error = otThreadSetEnabled(mp_ot_instance, true);
    //        ASSERT(error == OT_ERROR_NONE);

    //        NRF_LOG_INFO("Thread interface has been enabled.");
    //        NRF_LOG_INFO("802.15.4 Channel: %d", otLinkGetChannel(mp_ot_instance));
    //        NRF_LOG_INFO("802.15.4 PAN ID:  0x%04x", otLinkGetPanId(mp_ot_instance));
    //        NRF_LOG_INFO("rx-on-when-idle:  %s", otThreadGetLinkMode(mp_ot_instance).mRxOnWhenIdle ?
    //                                        "enabled" : "disabled");
    //    }
    //}
}

static void state_changed_callback(uint32_t flags, void * p_context)
{
    printf("State changed! Flags: 0x%08lx Current role: %d\r\n",
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
         otIp6SlaacUpdate(mp_ot_instance, m_slaac_addresses,
                          sizeof(m_slaac_addresses) / sizeof(m_slaac_addresses[0]),
                          otIp6CreateRandomIid, NULL);
    }
}

void fix_errata_78_in_nrf_802154(void)
{
    /* Temporary workaround for anomaly 78. When timer is stopped using STOP task
     * its power consumption will be higher.
     * Issuing task SHUTDOWN fixes this condition. This have to be removed when
     * proper fix will be pushed to nRF-IEEE-802.15.4-radio-driver.
     */
    if ((otPlatRadioGetState(mp_ot_instance) == OT_RADIO_STATE_SLEEP) ||
        (otPlatRadioGetState(mp_ot_instance) == OT_RADIO_STATE_DISABLED))
    {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_SHUTDOWN);
    }
}

void thread_process(void)
{
    ASSERT(mp_ot_instance != NULL);
    otTaskletsProcess(mp_ot_instance);
    PlatformProcessDrivers(mp_ot_instance);
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

int main(void) {
    nrf_power_dcdcen_set(1);

    // Initialize.
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_pin_set(LED2);

    uart_init();

    thread_init();
    otError error = otSetStateChangedCallback(mp_ot_instance, state_changed_callback, mp_ot_instance);

    // Enter main loop.
    while (1) {
        thread_process();
        thread_sleep();
        //nrf_delay_ms(500);
        nrf_gpio_pin_toggle(LED2);
    }
}
