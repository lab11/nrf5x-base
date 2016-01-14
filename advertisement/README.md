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

If you would like to advertise a service uuid as well:

    void simple_adv_service (ble_uuid_t* service_uuid);

Examples:

    ble_uuid_t my_service_uuid;
    simple_adv_service(&my_service_uuid);

Or, if you are using simple_ble services:

    //  UUID created by `uuidgen -r`
    //  16-bit short uuid is 0x890f (bytes 12 and 13 of 128-bit UUID)
    simple_ble_service_t my_service = {
        .uuid128 = {{0x87, 0xa4, 0xde, 0xa0, 0x96, 0xea, 0x4e, 0xe6,
                     0x87, 0x45, 0x83, 0x28, 0x89, 0x0f, 0xad, 0x7b}}
    };
    simple_ble_add_service(&my_service);
    simple_adv_service(&my_service.uuid_handle);

You can also advertise manufacturer specific data:

    void simple_adv_manuf_data(&manuf_specific_data);

Example:

    ble_advdata_manuf_data_t manuf_specific_data;
    manuf_specific_data.company_identifier = MY_COMPANY_IDENTIFIER;
    manuf_specific_data.data.p_data = my_data_buf;
    manuf_specific_data.data.size   = my_data_buf_len;
    simple_adv_manuf_data(&manuf_specific_data);

