#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"

#define UART_RX              26
#define UART_TX              27
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

#define LED0 NRF_GPIO_PIN_MAP(0,4)
#define LED1 NRF_GPIO_PIN_MAP(0,5)
#define LED2 NRF_GPIO_PIN_MAP(0,6)

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

int main(void) {
  // Set DCDC mode
  nrf_power_dcdcen_set(1);

  // init led
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_pin_clear(LED1);

  // init uart
  uart_init();

  printf("\nBegin UART Test\n");

  uint32_t i = 0;
  while (1) {
    //__WFE()
    nrf_delay_ms(1000);
    printf("UART Test %ld\r\n", i++);
  }
}
