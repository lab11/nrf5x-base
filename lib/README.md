nRF5x Libraries
===============

This folder contains libraries that make it easier to write BLE apps on the
nRF5x BLE SoCs.

## `simple_ble.c`

`simple_ble` sets up the Nordic BLE stack with reasonable defaults. This
removes a lot of the copy-pasting that used to be necessary to create new BLE apps.
It picks reasonable defaults for most parameters and exposes a few settings
that are more application-specific. If you are looking to make a very specific
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
        simple_ble_app_t* simple_ble_app = simple_ble_init(&ble_config);

- `void simple_ble_add_service (simple_ble_service_t* service_handle)`

    This adds a BLE service to the node.

        //  UUID created by `uuidgen -r`
        //  16-bit short uuid is 0x890f (bytes 12 and 13 of 128-bit UUID)
        simple_ble_service_t my_service = {
            .uuid128 = {{0x87, 0xa4, 0xde, 0xa0, 0x96, 0xea, 0x4e, 0xe6,
                         0x87, 0x45, 0x83, 0x28, 0x89, 0x0f, 0xad, 0x7b}}
        };
        simple_ble_add_service(&my_service);

- `void simple_ble_add_characteristic (...)`

        void simple_ble_add_characteristic (uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                            uint16_t len, uint8_t* buf,
                                            simple_ble_service_t* service_handle,
                                            simple_ble_char_t* char_handle);

    Add a characteristic to a service.

        simple_ble_char_t my_char = {.uuid16 = 0x8910};
        simple_ble_add_characteristic(0, 1, 0, 0, // read, write, notify, vlen
                                      1, &app.my_char_value,
                                      &my_service,
                                      &my_char);

- `void simple_ble_update_char_len (simple_ble_char_t* char_handle, uint16_t len)`

    This updates the length of a variable-length characteristic. This can only
    be used on a characteristic that was created with vlen set to 1

        simple_ble_update_char_len(&my_vlen_char, 5);

- `void simple_ble_notify_char (simple_ble_char_t* char_handle)`

    This sends a BLE notification with an updated value for a notify-enabled
    characteristic. This function can be safely used even if there is no
    connection or if the central has not requested notification.

        app.my_notify_char_value = 10;
        simple_ble_notify_char(&my_notify_char);

- `void simple_ble_is_char_event (ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle)`

    This checks if a BLE write event corresponds to the given characteristic

        void ble_evt_write(ble_evt_t* p_ble_evt) {
            
            if (simple_ble_is_char_event(p_ble_evt, &my_char)) {
                // code to be run on a write event here
                // ...
            }
        }


## `simple_timer.c`

`simple_timer` allows for easy default use of timers. It allows periodic
callbacks to be created using a single function call. Up to four callbacks can
be created in this way. If you want non-repeating timers or more than four, you
should use the app_timer library directly instead.

### API

- `void simple_timer_init(void)`

    Initializes timer system if `simple_ble` is not in use

    Note: if you are using the `simple_ble` library you do NOT need to call
    this function. Doing so will waste about 500 bytes of RAM space (although
    the timers will still work).

- `void simple_timer_start (uint32_t milliseconds, app_timer_timeout_handler_t callback)`

    Creates a timer to call a function at the given period

        void toggle_led (void);

        // toggle led every second
        simple_timer_start(1000, toggle_led);

