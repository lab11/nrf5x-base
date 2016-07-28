/* Blink with simple timer
 */

#include "nordic_common.h"
#include "softdevice_handler.h"

#include "board.h"

#include "tcmp441.h"


int main(void) {

    tcmp441_init(18, 19, 20, 24, 23, 22);
    tcmp441_writeStringAtLocation("Hello World!", 0, 0, 1);
    tcmp441_updateDisplay();

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}
