// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "nrf52dk.h"

// LED and Button arrays
static uint8_t LEDS[4] = {NRF52DK_LED1, NRF52DK_LED2, NRF52DK_LED3, NRF52DK_LED4};
static uint8_t BTNS[4] = {NRF52DK_BUTTON1, NRF52DK_BUTTON2, NRF52DK_BUTTON3, NRF52DK_BUTTON4};

// Serial configuration
// create a uart instance for UARTE0
NRF_SERIAL_UART_DEF(serial_uart_instance, 0);
// configuration for uart, RX & TX pin, empty RTS and CTS pins,
//  flow control disabled, no parity bit, 115200 baud, default priority
NRF_SERIAL_DRV_UART_CONFIG_DEF(serial_uart_config, NRF52DK_UART_RXD, NRF52DK_UART_TXD, 0, 0,
      NRF_UART_HWFC_DISABLED, NRF_UART_PARITY_EXCLUDED, NRF_UART_BAUDRATE_115200, UART_DEFAULT_CONFIG_IRQ_PRIORITY);
// create serial queues for commands, tx length 32, rx length 32
NRF_SERIAL_QUEUES_DEF(serial_queues, 32, 32);
// create serial buffers for data, tx size 100 bytes, rx size 100 bytes
NRF_SERIAL_BUFFERS_DEF(serial_buffers, 100, 100);
// create a configuration using DMA with queues for commands and buffers for data storage
// both handlers are set to NULL as we do not need to support them
NRF_SERIAL_CONFIG_DEF(serial_config, NRF_SERIAL_MODE_DMA, &serial_queues, &serial_buffers, NULL, NULL);


// handler called whenever an input pin changes
void pin_change_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  ret_code_t error_code = NRF_SUCCESS;

  // iterate through the buttons to determine which caused the event
  for (int i=0; i<4; i++) {
    if (pin == BTNS[i]) {

      // change the state of the corresponding LED to match
      if (nrfx_gpiote_in_is_set(BTNS[i])) {
        // turn off LED
        nrfx_gpiote_out_set(LEDS[i]);

        // write message
        static char on_message[] = "LED on\n";
        error_code = nrf_serial_write(&serial_uart_instance, on_message, strlen(on_message), NULL, 0);
        APP_ERROR_CHECK(error_code);
        printf("LED %d on\n", i);

      } else {
        // turn on LED
        nrfx_gpiote_out_clear(LEDS[i]);

        // write message
        static char off_message[] = "LED off\n";
        error_code = nrf_serial_write(&serial_uart_instance, off_message, strlen(off_message), NULL, 0);
        APP_ERROR_CHECK(error_code);
        printf("LED %d off\n", i);
      }
    }
  }
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);

  // configure leds
  // manually-controlled (simple) output, initially set
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  for (int i=0; i<4; i++) {
    error_code = nrfx_gpiote_out_init(LEDS[i], &out_config);
    APP_ERROR_CHECK(error_code);
  }

  // configure buttons
  // input pin, trigger on either edge, low accuracy (allows low-power operation)
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
  in_config.pull = NRF_GPIO_PIN_PULLUP;
  for (int i=0; i<4; i++) {
    error_code = nrfx_gpiote_in_init(BTNS[i], &in_config, pin_change_handler);
    APP_ERROR_CHECK(error_code);

    // enable events from the input pin and enable interrupts
    nrfx_gpiote_in_event_enable(BTNS[i], true);
  }

  // initialize power management
  error_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(error_code);

  // initialize serial library
  error_code = nrf_serial_init(&serial_uart_instance, &serial_uart_config, &serial_config);
  APP_ERROR_CHECK(error_code);

  // write init message
  static char tx_message[] = "UART Initialized!\n";
  error_code = nrf_serial_write(&serial_uart_instance, tx_message, strlen(tx_message), NULL, 0);
  APP_ERROR_CHECK(error_code);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // two examples of writing messages to RTT
  NRF_LOG_INFO("Log messages initialized");
  printf("Printing with printf works too!\n");

  // loop forever
  while (1) {
    // enter sleep mode
    nrf_pwr_mgmt_run();
  }
}

