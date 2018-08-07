#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"

#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

#define LED0 NRF_GPIO_PIN_MAP(0,16)
#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,19)


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
  if (err_code != NRF_SUCCESS) {
    nrf_gpio_pin_clear(LED1);
  }
  //if (err_code == NRF_ERROR_INVALID_ADDR) {
  //  nrf_gpio_pin_clear(LED2);
  //}
  APP_ERROR_CHECK(err_code);

}

int main(void) {
  // Set DCDC mode
  //nrf_power_dcdcen_set(1);

  // init led
  nrf_gpio_cfg_output(LED0);
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_pin_set(LED0);
  nrf_gpio_pin_set(LED1);
  nrf_gpio_pin_set(LED2);

  // init uart
  uart_init();

  int val = 0;
  while (1) {
    //uint32_t error_code = app_uart_put('*');
    printf("Test %d\n", val);
    val++;
    nrf_gpio_pin_toggle(LED0);
    nrf_delay_ms(500);
  }

  /*
  printf("\nBegin UART Test\n");

  uint32_t i = 0;
  while (1) {
    //__WFE()
    nrf_delay_ms(1000);
    printf("UART Test %ld\r\n", i++);
  }
  */
}
