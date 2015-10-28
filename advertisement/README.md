Advertisement Libraries
=======================

This folder contains libraries that configure how advertisements are formed.
The libraries are designed to work with the `simple_ble` library.


A general example looks like this:

```c
int main () {
	// Setup BLE
	simple_ble_init(&ble_config);

	// Configure and start advertising
	simple_adv_only_name();

	while (1);
}
```

More complete examples can be found in the `/apps` folder.


## `eddystone.c`

This file provides support for advertising URLs in the Eddystone format.

    void eddystone_adv(char* url_str, const ble_advdata_t* scan_response_data);

The basic use looks like:

    eddystone_adv("goo.gl/abc123", NULL);

To also include the device name in the scan response:

    ble_advdata_t srdata;
    memset(&srdata, 0, sizeof(srdata));
    srdata.name_type = BLE_ADVDATA_FULL_NAME;
    eddystone_adv("goo.gl/abc123", &srdata);


## `simple_adv.c`

This file makes it easy to advertise standard BLE advertisments.

To advertise just the name of the BLE device (that was passed into
`simple_ble_init()`), just call this function:

    void simple_adv_only_name ();

If you would like to advertise a service as well:

    void simple_adv_service (ble_uuid_t* service_uuid);

Examples:

    ble_uuid_t my_service_uuid;
    simple_adv_service(&my_service_uuid);
