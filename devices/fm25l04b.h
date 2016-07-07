#ifndef FM25L04B_H_
#define FM25L04B_H_

#include "stdint.h"

#define FM25L04B_WRITE_ENABLE_COMMAND  0x06
#define FM25L04B_WRITE_DISABLE_COMMAND 0x04
#define FM25L04B_READ_STATUS_COMMAND   0x05
#define FM25L04B_WRITE_STATUS_COMMAND  0x01
#define FM25L04B_READ_COMMAND          0x03
#define FM25L04B_WRITE_COMMAND         0x02

/* \brief adds the 9th bit of the address to a command */
#define FM25L04B_ADD_ADDRESS_BIT(address, command) \
  (((address & 0x100) >> 5) | command)

typedef struct {
    nrf_drv_spi_t* spi;
    uint8_t        sck_pin;
    uint8_t        mosi_pin;
    uint8_t        miso_pin;
    uint8_t        ss_pin;
} fm25l04b_t;

int fm25l04b_read(fm25l04b_t* dev, uint16_t address, uint8_t *buf, uint16_t len);
int fm25l04b_write(fm25l04b_t* dev, uint16_t address, uint8_t *buf, uint16_t len);

#endif
