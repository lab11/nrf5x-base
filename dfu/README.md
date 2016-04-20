DFU Bootloader
==========

DFU Bootloader to support over-the-air firmware updates
Currently only supports:

* SDK 10
* s110 softdevice
* 32 KB RAM
* 256 KB Flash

To include bootloader with app, simply add this line to your app Makefile:
    
    USE_BOOTLOADER = 1


