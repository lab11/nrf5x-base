nRF5x Libraries
===============

This folder contains libraries that make it easier to write BLE apps on the
nRF5x BLE SoCs.

## `simple_ble.c`

`simple_ble` sets up the Nordic BLE stack with reasonable defaults. This
removes a lot of the copy-pasting that used to be necessary to create new BLE apps.
It picks reasonable defaults for most paramters and exposes a few settings
that are more appication-specific. If you are looking to make a very specific
BLE app you may want to use the Nordic APIs directly. However, if you want
to just setup some services, this is a much better starting point.

### API

- `simple_ble_app_t* simple_ble_init(const simple_ble_config_t* conf)`

    Setup all of the BLE settings. Called first in `main()`.

    The config structure looks like:

        typedef struct simple_ble_config_s {
            uint8_t     platform_id;        // used as 4th octet in device BLE address
            uint16_t    device_id;          // set the lower 16 bits of the device id. Set to DEVICE_ID_DEFAULT to use random.
            char*       adv_name;           // used in advertisements if there is room
            uint16_t    adv_interval;
            uint16_t    min_conn_interval;
            uint16_t    max_conn_interval;
        } simple_ble_config_t;

    and an example setup:

        simple_ble_config_t ble_config = {
            .platform_id       = 0x00,              // used as 4th octect in device BLE address
            .device_id         = DEVICE_ID_DEFAULT,
            .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
            .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
            .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
            .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
        };

- `uint16_t simple_ble_add_service (const ble_uuid128_t* uuid128, ble_uuid_t* uuid, uint16_t short_uuid)`

    This adds a BLE service to the node.


        #define SHORT_UUID 0x1234
        const ble_uuid128_t my_uuid128 = {
            {0xaa, 0xbb, 0x5e, 0x39, 0x31, 0x52, 0x45, 0x0c,
             0x90, 0xee, 0x3f, 0xa2, 0x9c, 0x86, 0x8c, 0xd6}
        };
        ble_uuid_t my_uuid;
        app.service_handle = simple_ble_add_service(&my_uuid128, &my_uuid, SHORT_UUID);

- `void simple_ble_add_characteristic (...)`

        void simple_ble_add_characteristic (uint8_t read,
                                            uint8_t write,
                                            uint8_t notify,
                                            uint8_t uuid_type,
                                            uint16_t uuid,
                                            uint16_t len,
                                            uint8_t* buf,
                                            uint16_t service_handle,
                                            ble_gatts_char_handles_t* char_handle);

    Add a characteristic to a service.

        #define CHAR_SHORT_UUID 0x1235
        simple_ble_add_characteristic(1, 0, 1,  // read, write, notify
                                      my_uuid.type,
                                      CHAR_SHORT_UUID,
                                      1, app.char_memory,
                                      app.service_handle,
                                      &app.char_handle);


