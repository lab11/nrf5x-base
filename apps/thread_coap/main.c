/* Blink
 */

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
#include "nrf_sdh.h"
#include "nrf_soc.h"

#include <openthread/openthread.h>
#include <openthread/icmp6.h>
#include <openthread/coap.h>
#include <openthread/platform/platform.h>
#include <openthread/cli.h>
#include <openthread/diag.h>

APP_TIMER_DEF(coap_send_timer);

#define COAP_SERVER_ADDR "64:ff9b::23a6:b3ac"

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

#define AUTOCOMMISSION 1
#define DEFAULT_CHILD_TIMEOUT    40                                         /**< Thread child timeout [s]. */
#define DEFAULT_POLL_PERIOD      1000                                       /**< Thread Sleepy End Device polling period when MQTT-SN Asleep. [ms] */
#define SHORT_POLL_PERIOD        100                                        /**< Thread Sleepy End Device polling period when MQTT-SN Awake. [ms] */
#define NUM_SLAAC_ADDRESSES      4                                          /**< Number of SLAAC addresses. */

static otInstance * m_ot_instance;
static otNetifAddress m_slaac_addresses[NUM_SLAAC_ADDRESSES];               /**< Buffer containing addresses resolved by SLAAC */
static const otIp6Address m_unspecified_ipv6 =
{
    .mFields =
    {
        .m8 = {0}
    }
};
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

/**@brief Function for initializing the Thread Stack.
 */

void thread_init()
{
    otError error;

    PlatformInit(0, NULL);

    m_ot_instance = otInstanceInitSingle();
    ASSERT(m_ot_instance != NULL);

    NRF_LOG_INFO("Thread version: %s", otGetVersionString());
    NRF_LOG_INFO("Network name:   %s",
                 otThreadGetNetworkName(m_ot_instance));

    if (!otDatasetIsCommissioned(m_ot_instance) || AUTOCOMMISSION)
    {
        error = otLinkSetChannel(m_ot_instance, THREAD_CHANNEL);
        ASSERT(error == OT_ERROR_NONE);
        NRF_LOG_INFO("Thread Channel: %d", otLinkGetChannel(m_ot_instance));

        error = otLinkSetPanId(m_ot_instance, THREAD_PANID);
        ASSERT(error == OT_ERROR_NONE);
        NRF_LOG_INFO("Thread PANID: 0x%lx", (uint32_t)otLinkGetPanId(m_ot_instance));
    }

    otLinkModeConfig mode;
    memset(&mode, 0, sizeof(mode));
    // sleepy end device
    mode.mSecureDataRequests = true;
    mode.mRxOnWhenIdle       = false; // Join network as SED.
    otLinkSetPollPeriod(m_ot_instance, DEFAULT_POLL_PERIOD);
    // regular end device
    //mode.mRxOnWhenIdle       = true;
    //mode.mSecureDataRequests = true;
    //mode.mDeviceType         = true;
    //mode.mNetworkData        = true;

    error = otThreadSetLinkMode(m_ot_instance, mode);
    ASSERT(error == OT_ERROR_NONE);

    otThreadSetChildTimeout(m_ot_instance, DEFAULT_CHILD_TIMEOUT);

    error = otIp6SetEnabled(m_ot_instance, true);
    ASSERT(error == OT_ERROR_NONE);

    if (otDatasetIsCommissioned(m_ot_instance) || AUTOCOMMISSION)
    {
      error = otThreadSetEnabled(m_ot_instance, true);
      ASSERT(error == OT_ERROR_NONE);

      NRF_LOG_INFO("Thread interface has been enabled.");
      NRF_LOG_INFO("802.15.4 Channel: %d", otLinkGetChannel(m_ot_instance));
      NRF_LOG_INFO("802.15.4 PAN ID:  0x%04x", otLinkGetPanId(m_ot_instance));
      NRF_LOG_INFO("rx-on-when-idle:  %s", otThreadGetLinkMode(m_ot_instance).mRxOnWhenIdle ?
          "enabled" : "disabled");
    }
}

static void thread_coap_handler(void                * p_context,
                                otCoapHeader        * p_header,
                                otMessage           * p_message,
                                const otMessageInfo * p_message_info)
{
    (void)p_context;
    (void)p_header;
    (void)p_message;
    (void)p_message_info;

    NRF_LOG_INFO("Received CoAP message that does not match any request or resource\r\n");
}


void thread_coap_init() {
    otError error = otCoapStart(m_ot_instance, OT_DEFAULT_COAP_PORT);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(m_ot_instance, thread_coap_handler, NULL);
}

void thread_coap_send(uint16_t data) {
    otError       error = OT_ERROR_NONE;
    otMessage   * message;
    otMessageInfo message_info;
    otCoapHeader  header;

    do
    {
        if (otIp6IsAddressEqual(&m_peer_address, &m_unspecified_ipv6))
        {
            NRF_LOG_INFO("Failed to send the CoAP Request to the Unspecified IPv6 Address\r\n");
            break;
        }

        otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapHeaderGenerateToken(&header, 2);
        otCoapHeaderAppendUriPathOptions(&header, "test");
        otCoapHeaderSetPayloadMarker(&header);

        message = otCoapNewMessage(m_ot_instance, &header);
        if (message == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        error = otMessageAppend(message, &data, sizeof(data));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        memcpy(&message_info.mPeerAddr, &m_peer_address, sizeof(message_info.mPeerAddr));

        error = otCoapSendRequest(m_ot_instance,
                                  message,
                                  &message_info,
                                  NULL,
                                  m_ot_instance);
    } while (false);

    if (error != OT_ERROR_NONE && message != NULL)
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(message);
    }
}

static void state_changed_callback(uint32_t flags, void * p_context)
{
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
         otIp6SlaacUpdate(m_ot_instance, m_slaac_addresses,
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
    if ((otPlatRadioGetState(m_ot_instance) == OT_RADIO_STATE_SLEEP) ||
        (otPlatRadioGetState(m_ot_instance) == OT_RADIO_STATE_DISABLED))
    {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_SHUTDOWN);
    }
}

void thread_process(void)
{
    ASSERT(m_ot_instance != NULL);
    otTaskletsProcess(m_ot_instance);
    PlatformProcessDrivers(m_ot_instance);
}

void thread_sleep(void)
{
    ASSERT(m_ot_instance != NULL);

    // Enter sleep state if no more tasks are pending.
    if (!otTaskletsArePending(m_ot_instance))
    {
        fix_errata_78_in_nrf_802154();

//#ifdef SOFTDEVICE_PRESENT
//        ret_code_t err_code = sd_app_evt_wait();
//        ASSERT(err_code == NRF_SUCCESS);
//#else
        __WFE();
//#endif
    }
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

    thread_init();
    thread_coap_init();
    otError error = otSetStateChangedCallback(m_ot_instance, state_changed_callback, m_ot_instance);


    app_timer_init();
    app_timer_create(&coap_send_timer, APP_TIMER_MODE_REPEATED, thread_coap_send);
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
