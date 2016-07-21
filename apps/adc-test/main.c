/* ADC-test
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "app_timer.h"
#include "softdevice_handler.h"
#include "nrf_drv_adc.h"
#include "app_util_platform.h"

#define ADC_BUFFER_SIZE 10                                /**< Size of buffer for ADC samples.  */
static nrf_adc_value_t       adc_buffer[ADC_BUFFER_SIZE]; /**< ADC buffer. */
static nrf_drv_adc_channel_t m_channel_config = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_7); /**< Channel instance. Default configuration used. */

#define LED 18

/**
 * @brief ADC interrupt handler.
 */
static void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        uint32_t i;
        for (i = 0; i < p_event->data.done.size; i++)
        {
            if(p_event->data.done.p_buffer[i] > 512)
            	led_on(LED);
            else
                led_off(LED);
            //NRF_LOG_PRINTF("Current sample value: %d\r\n", p_event->data.done.p_buffer[i]);
        }
    }
}

/**
 * @brief ADC initialization.
 */
static void adc_config(void)
{
    ret_code_t ret_code;
    nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;

    ret_code = nrf_drv_adc_init(&config, adc_event_handler);
    APP_ERROR_CHECK(ret_code);

    nrf_drv_adc_channel_enable(&m_channel_config);
}



int main(void) {

    // Initialize.
    led_init(LED);
    led_off(LED);

    adc_config();

    // Need to set the clock to something
    nrf_clock_lf_cfg_t clock_lf_cfg = {
        .source        = NRF_CLOCK_LF_SRC_RC,
        .rc_ctiv       = 16,
        .rc_temp_ctiv  = 2,
        .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM};

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    // Enter main loop.
    while (1) {
        APP_ERROR_CHECK(nrf_drv_adc_buffer_convert(adc_buffer,ADC_BUFFER_SIZE));
        uint32_t i;
        for (i = 0; i < ADC_BUFFER_SIZE; i++)
        {
            // manually trigger ADC conversion
            nrf_drv_adc_sample();        

            //sd_app_evt_wait();
            __SEV();
            __WFE();
        }
    }
}
