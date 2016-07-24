
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_drv_config.h"

#include "led.h"

#define LED0 13

#define MAX_TEST_DATA_BYTES  (15U)
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

#define CMD_BUFFER_LEN 256
char cmd_buf[CMD_BUFFER_LEN];
int cmd_index = 0;

void check_cmd () {
    if (strncmp(cmd_buf, "on", CMD_BUFFER_LEN) == 0) {
        led_on(LED0);
    } else if (strncmp(cmd_buf, "off", CMD_BUFFER_LEN) == 0) {
        led_off(LED0);
    }
}

void uart_error_handle (app_uart_evt_t * p_event) {
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
        // ignore
    } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
        // ignore
    } else if (p_event->evt_type == APP_UART_DATA_READY) {

        // Read the UART character
        char cr;
        app_uart_get((uint8_t*) &cr);
        app_uart_put(cr);

        // Check array bounds
        if (cmd_index > 255) {
            cmd_index = 0;
        }

        // Check character
        if (cr == '\r') {
            // ignore
        } else if (cr == '\n') {
            cmd_buf[cmd_index] = '\0';
            cmd_index = 0;
            check_cmd();
        } else {
            cmd_buf[cmd_index] = cr;
            cmd_index++;
        }
    }
}

int main (void) {
    led_init(LED0);
    led_off(LED0);

    uint32_t err_code;
    const app_uart_comm_params_t comm_params = {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          0,
          0,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud115200
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);
    APP_ERROR_CHECK(err_code);

    while (true) { }
}
