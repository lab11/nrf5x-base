DFU Bootloader
==========

DFU Bootloader to support over-the-air firmware updates

Note:

make flash target in Makefile.posix must be changed to use loadfile with
the bootloader hex file rather than loadbin with the bootloader bin file

dfu_ble_peer_data_get is missing from SDK, so I commented it out in
dfu_transport_ble.c and set its error code to 1.


