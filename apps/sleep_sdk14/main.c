/* Blink
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_delay.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

int main(void) {
    uint32_t err_code;

    // Initialize.
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    if (!nrf_sdh_is_enabled()) {
      printf("Failed to enable softdevice\n");
    }

    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_app_ram_start_get(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    while (1) {
      sd_app_evt_wait();
    }
}
