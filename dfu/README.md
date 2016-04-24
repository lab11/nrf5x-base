DFU Bootloader
==========

DFU Bootloader to support over-the-air firmware updates.
Currently supports devices using 
* SDK 10, softdevice s110 or SDK 11, softdevice s130.
* Linker scripts currently configured with 32 KB RAM and 256 KB Flash.

To include bootloader with app, simply add this line to your app Makefile:
    
    ENABLE_WIRELESS_DFU = 1


