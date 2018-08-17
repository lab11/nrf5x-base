#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_uart.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_power.h"
#include "nrf_uarte.h"

// Pin configurations
#define LED NRF_GPIO_PIN_MAP(0,17)
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

// error handler for UART
void uart_error_handle (app_uart_evt_t * p_event) {
  // just call app error handler
  if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
    APP_ERROR_HANDLER(p_event->data.error_communication);
  } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
    APP_ERROR_HANDLER(p_event->data.error_code);
  }
}

// initialization of UART
void uart_init(void) {
  uint32_t err_code;

  // configure RX and TX pins
  // no RTS or CTS pins with flow control disabled
  // no parity
  // baudrate 115200
  const app_uart_comm_params_t comm_params = {
    UART_RX, UART_TX,
    0, 0, APP_UART_FLOW_CONTROL_DISABLED,
    false,
    NRF_UARTE_BAUDRATE_115200
  };

  // actually initialize UART
  APP_UART_FIFO_INIT(&comm_params, UART_RX_BUF_SIZE, UART_TX_BUF_SIZE,
                     uart_error_handle, APP_IRQ_PRIORITY_LOW, err_code);
  APP_ERROR_CHECK(err_code);
}

int main(void) {

  // init led
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_set(LED);

  // init uart
  uart_init();

  int val = 0;
  while (1) {
    printf("Loop Count %d\n", val++);
    nrf_gpio_pin_toggle(LED);
    nrf_delay_ms(500);
  }
}

