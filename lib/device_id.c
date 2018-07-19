#include <string.h>

#include "nrf.h"

void get_device_id(uint8_t* device_id) {
  if (((uint8_t*)(ID_FLASH_LOCATION))[0] == 0xFF) {
    uint32_t id = NRF_FICR->DEVICEADDR[1] & 0xffff;
    device_id[0] = ((uint8_t*)&id)[1];
    device_id[1] = ((uint8_t*)&id)[0];
    id = NRF_FICR->DEVICEADDR[0];
    device_id[2] = ((uint8_t*)&id)[3];
    device_id[3] = ((uint8_t*)&id)[2];
    device_id[4] = ((uint8_t*)&id)[1];
    device_id[5] = ((uint8_t*)&id)[0];
  } else {
    device_id[0] = ((uint8_t*)ID_FLASH_LOCATION)[5];
    device_id[1] = ((uint8_t*)ID_FLASH_LOCATION)[4];
    device_id[2] = ((uint8_t*)ID_FLASH_LOCATION)[3];
    device_id[3] = ((uint8_t*)ID_FLASH_LOCATION)[2];
    device_id[4] = ((uint8_t*)ID_FLASH_LOCATION)[1];
    device_id[5] = ((uint8_t*)ID_FLASH_LOCATION)[0];
  }
}
