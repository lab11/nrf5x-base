// Better app error handler
//
// Prints app error conditions over printf and blinks LEDs. If printf is
// already set up, the print statements should appear over the normal medium
// (probably RTT). LED pins are board-specific configurations. Mostly copied
// from `app_error_weak.c`

#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "hardfault.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrfx_gpiote.h"

// include softdevice faults if applicable
#if defined(SOFTDEVICE_PRESENT) && SOFTDEVICE_PRESENT
#include "nrf_sdm.h"
#endif

// ***** Board Specific Configuration *****
#include "nrf52840dk.h"
#define LED_COUNT 4
static const uint8_t LEDS[LED_COUNT] = {LED1,LED2,LED3,LED4};
// ****************************************

// app error handler. Overwrites weak definition in app_error_weak.c
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
  // halt all existing state
  __disable_irq();
  NRF_LOG_FINAL_FLUSH();

  // print banner
  printf("\n\n***** App Error *****\n");

  // check cause of error
  switch (id) {
#if defined(SOFTDEVICE_PRESENT) && SOFTDEVICE_PRESENT
    case NRF_FAULT_ID_SD_ASSERT: {
      printf("SOFTDEVICE: ASSERTION FAILED");
      break;
    }
    case NRF_FAULT_ID_APP_MEMACC: {
      printf("SOFTDEVICE: INVALID MEMORY ACCESS");
      break;
    }
#endif
    case NRF_FAULT_ID_SDK_ASSERT: {
        assert_info_t * p_info = (assert_info_t *)info;
        printf("ASSERTION FAILED at %s:%u\n",
            p_info->p_file_name,
            p_info->line_num);
        break;
      }
    case NRF_FAULT_ID_SDK_ERROR: {
        error_info_t * p_info = (error_info_t *)info;
        printf("ERROR %lu [%s] at %s:%lu\t\tPC at: 0x%08lX\n",
            p_info->err_code,
            nrf_strerror_get(p_info->err_code),
            p_info->p_file_name,
            p_info->line_num,
            pc);
        break;
      }
    default: {
      printf("UNKNOWN FAULT at 0x%08lX\n", pc);
      break;
    }
  }

  // Note: we are ignoring error codes from the following GPIO calls. We don't
  // have a way to deal with them other than ignore them at this point anyways

  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    nrfx_gpiote_init();
  }

  // configure leds
  // manually-controlled (simple) output, initially set
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  for (int i=0; i<LED_COUNT; i++) {
    nrfx_gpiote_out_init(LEDS[i], &out_config);
  }

  // loop forever blinking LEDs in a particular pattern to attact attention
  while (1) {
    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_clear(LEDS[i]);
    }
    nrf_delay_ms(500);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_set(LEDS[i]);
    }
    nrf_delay_ms(50);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_clear(LEDS[i]);
    }
    nrf_delay_ms(500);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_set(LEDS[i]);
    }
    nrf_delay_ms(250);
  }
}

// process-specific HardFault handler. Overwrites weak definition in hardfault_implementation.c
void HardFault_process(HardFault_stack_t* p_stack) {
  // Note: we are ignoring error codes from the following GPIO calls. We don't
  // have a way to deal with them other than ignore them at this point anyways

  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    nrfx_gpiote_init();
  }

  // configure leds
  // manually-controlled (simple) output, initially set
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  for (int i=0; i<LED_COUNT; i++) {
    nrfx_gpiote_out_init(LEDS[i], &out_config);
  }

  // loop forever blinking LEDs in a particular pattern to attact attention
  while (1) {
    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_clear(LEDS[i]);
    }
    nrf_delay_ms(500);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_set(LEDS[i]);
    }
    nrf_delay_ms(50);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_clear(LEDS[i]);
    }
    nrf_delay_ms(500);

    for (int i=0; i<LED_COUNT; i++) {
      nrfx_gpiote_out_set(LEDS[i]);
    }
    nrf_delay_ms(250);
  }
}

