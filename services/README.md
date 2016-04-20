BLE Services
============

This folder contains libraries that enable and configure BLE services. They
are designed to work in conjunction with the `simple_ble` library.

A general example looks like this:

```c
#include "device_info_service.h"

...

int main () {
  // Setup BLE
  simple_ble_init(&ble_config);

  // Configure and start Device Information Service
  simple_device_info_service(...);

  // Configure and start advertising
  simple_adv_only_name();

  while(1);
}
```

## `device_info_service.c`

This file provides an interface for the BLE SIG
[Device Information Service](https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.device_information.xml).

The interface accepts:
 * Hardware Revision String
 * Firmware Revision String
 * Software Revision String

Replace any with `NULL` instead of string to leave them unused.
[System ID](https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.system_id.xml),
which contains the BLE address of the device is also included automatically.

This service can be used in coordination with Wireless DFU in order to specify
the current firmware revision number. If you are doing so, ensure that revision
numbers are always specified in [Semantic Versioning](http://semver.org/).

In order to use this library, include in your app Makefile:

```
APPLICATION_SRCS += ble_dis.c
APPLICATION_SRCS += device_info_service.c
```

