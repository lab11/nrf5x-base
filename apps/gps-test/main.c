#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_drv_config.h"

#include "led.h"

#define LED0 18
#define LED1 19

#define MAX_TEST_DATA_BYTES  (15U)
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256

#define CMD_BUFFER_LEN 256
char cmd_buf[CMD_BUFFER_LEN];
int cmd_index = 0;

double latitude = 0.0;
double longitude = 0.0;

typedef struct
{
  double UTCtime;
  char status;
  double latitude;
  double longitude;
  double altitude;
  double speed;
  int date;
} GPSdata;

GPSdata data;

void check_cmd () {
    char nmea_name[7];
    strncpy(nmea_name, cmd_buf, 6);
    nmea_name[6] = '\0';
 
    if(strcmp(nmea_name,"$GPRMC") == 0)
    {
      led_on(LED0);
      
      //split the data into an array
      int i = 0;
      char *p = strtok (cmd_buf, ",");

      char *array[14];
      while (p != NULL)
      {
        array[i++] = p;
        p = strtok (NULL, ",");
      }

      //interpret the data
      data.UTCtime = atof(array[1]);
      data.status = array[2];
      data.latitude = atof(array[3]);
      data.longitude = atof(array[5]);
      //data.speed = atof(array[7]);
      data.date = atoi(array[9]);
    }
    else if(strcmp(nmea_name, "$GPGLL") == 0)
    {
      //split the data into an array
      int i = 0;
      char *p = strtok (cmd_buf, ",");

      char *array[14];
      while (p != NULL)
      {
        array[i++] = p;
        p = strtok (NULL, ",");
      }

      //interpret the data
      data.latitude = atof(array[1]);
      data.longitude = atof(array[3]);
      data.status = array[6];
    }
    else if(strcmp(nmea_name, "$GPGGA") == 0)
    {
      //split the data into an array
      int i = 0;
      char *p = strtok (cmd_buf, ",");

      char *array[14];
      while (p != NULL)
      {
        array[i++] = p;
        p = strtok (NULL, ",");
      }

      //interpret the data
      data.UTCtime = atof(array[1]);
      data.latitude = atof(array[2]);
      data.longitude = atof(array[4]);
      data.altitude = atof(array[9]);
    }
    else
    {
      led_off(LED0);
    }
}

void sendCommand(uint8_t input_command_name_int, uint16_t data_int)
{
  //"$PMTK,103,,\r\n"
  char command[32];
  strcpy(command, "$PMTK,");

  char input_command_name_string[4];
  sprintf(input_command_name_string, "%d", input_command_name_int);

  strcat(command, input_command_name_string);
  strcat(command, ",");

  char data_string[8];
  sprintf(data_string, "%d", data_int);
  strcat(command, ",");

  //generate checksum
  int checksum = command[0] | command[1];
  for(int i = 2; i < strlen(command); i++)
  {
    checksum = checksum | command[i];
  }

  char checksum_string[3];
  sprintf(checksum_string, "%02X", checksum);
  strcat(command, checksum_string);

  strcat(command, ",");

  strcat(command, "\r\n");

  //send the command byte-by-byte
  for(int i = 0; i < strlen(command); i++)
  {
    char byte = command[i];
    app_uart_put(byte);
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
          24,
          23,
          0,
          0,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud9600
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
