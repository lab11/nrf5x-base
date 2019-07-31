#pragma once

// returns device id stored at end of flash if exists
// otherwise returns pseudorandom id
// expects device_id to be 6 bytes (48 bits)
void get_device_id(uint8_t* device_id);

