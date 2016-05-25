Device Firmware Update (DFU) Bootloader
==========

DFU Bootloader to support over-the-air firmware updates.
Currently supports devices using 
* SDK 10, softdevice s110 or SDK 11, softdevice s130.
* Linker scripts currently configured with 32 KB RAM and 256 KB Flash.

To include the bootloader with the app, simply add this line to your app Makefile:
    
    ENABLE_WIRELESS_DFU = 1

There are two methods in which a SimpleBLE app can be told to reset to the
bootloader and start the DFU process.

### As a Client

| Service | Characteristic        |  UUID                                |
|:-------:|:---------------------:|:------------------------------------:|
| DFU     |                       | 00001530-1212-efde-1523-785feabcd123 |
|         | DFU Control Point     | 00001531-1212-efde-1523-785feabcd123 |
|         | DFU Packet            | 00001532-1212-efde-1523-785feabcd123 |

For a SimpleBLE client in both s110 and s130, the DFU Service and Control Point
Characteristic are exposed when connected. Writing any value to the Control
Point will cause the application to reset to the bootloader, ready for an
incoming firmware update. The Packet Characteristic is only exposed by the
bootloader.

### As a Server

*DFU Control Advertisement Format*

| **Field**           | Length | AD Type | AD Data   |
|:-------------------:|:------:|:-------:|:---------:|
| **Byte Index**      | 0      | 1       | 2-        |
| **DFU Value**       | 0x16   | 0xFF    | 0xE002... |

A DFU Control Advertisement is sent as Manufacturer Specific Data (AD Type
0xFF). The Company Identifier is 0x02E0 (registered to the University of
Michigan), which is included in the advertisement in little-endian format
per the BLE specification.

*Protocol Version 1*

| **Field**      | Service ID | Version | Target |
|:--------------:|:----------:|:-------:|:------:|
| **Byte Index** | 0          | 1       | 2-8    |

An s130 device running a SimpleBLE app as a server will listen for incoming
advertisements matching the above and reset to the bootloader if the Target
address matches.

Data fields are as follows:
* **Service ID** Used to distinguish between devices using the University of
Michigan Company Identifier. DFU is assigned 0x16.
* **Version**: Protocol Version Number.
* **Target**: 6 byte BLE address in little-endian of the device that is to be reset to the
bootloader.
