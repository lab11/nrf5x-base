
#include <stdint.h>
#include <stdbool.h>
#include "stdarg.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "simple_logger.h"
#include "chanfs/ff.h"
#include "chanfs/diskio.h"

static uint8_t simple_logger_inited = 0;
static uint8_t simple_logger_file_exists = 0;
static uint8_t header_written = 0;
static uint8_t error_count = 0;

const char *file = NULL;

// Define your own buffer size if required; 256 used as default
#ifndef SIMPLE_LOGGER_BUFFER_SIZE
#define SIMPLE_LOGGER_BUFFER_SIZE	256
#endif

static char buffer[SIMPLE_LOGGER_BUFFER_SIZE];
static char header_buffer[SIMPLE_LOGGER_BUFFER_SIZE];
static uint32_t buffer_size = SIMPLE_LOGGER_BUFFER_SIZE;


static FIL 		simple_logger_fpointer;	/* File object */
static FATFS 	simple_logger_fs;		/* Filesystem object */
static uint8_t 	simple_logger_opts;
static BYTE		work[FF_MAX_SS];		/* Work area (larger is better for processing time) */

extern void disk_timerproc(void);
extern void disk_restart(void);

static void error(void) {

	error_count++;

	if(error_count > 20) {

		disk_restart();
		error_count = 0;
	}
}

/*-----------------------------------------------------------------------*/
/* Device timer function                                                 */
/*-----------------------------------------------------------------------*/

static void heartbeat (void* p_context) {
	disk_timerproc();
}

APP_TIMER_DEF(clock_timer);

// Function starting the internal LFCLK oscillator (used for RTC1 required by app_timer)
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
/*static void lfclk_request(void)
{
	uint32_t err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);

	nrf_drv_clock_lfclk_request(NULL);
}*/

static void timer_init(void (*timeout_fct)(void*)) {

	// Request LFCLK if no SoftDevice is used
	//lfclk_request();

	// Init application timer module
	ret_code_t err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);

	// Create timer
	err_code = app_timer_create(&clock_timer, APP_TIMER_MODE_REPEATED, timeout_fct);
	APP_ERROR_CHECK(err_code);

}

static void timer_start(int period_ms) {

	uint32_t err_code = app_timer_start(clock_timer, APP_TIMER_TICKS(period_ms), NULL);
	APP_ERROR_CHECK(err_code);
}

/*static void timer_stop() {

	uint32_t err_code = app_timer_stop(clock_timer);
	APP_ERROR_CHECK(err_code);
}*/

/*-----------------------------------------------------------------------*/
/* SD card function                                                		 */
/*-----------------------------------------------------------------------*/

static uint8_t logger_init();

uint8_t simple_logger_init(const char *filename, const char *permissions) {

	if(simple_logger_inited) {
		return SIMPLE_LOGGER_ALREADY_INITIALIZED; // Can only initialize once
	}

	// Initialize a timer
	timer_init(heartbeat);
	timer_start(1);
	
	file = filename;

	// We must have not timed out and a card is available
	if((permissions[0] != 'w'  && permissions[0] != 'a') ||
	   (permissions[1] != '\0' && permissions[2] != 'r') ) {
		// We didn't set the right permissions
		return SIMPLE_LOGGER_BAD_PERMISSIONS;
	}

	// Set write/append permissions
	if(permissions[0] == 'w') {
		simple_logger_opts = (FA_WRITE | FA_CREATE_ALWAYS);
	} else if (permissions[0] == 'a'){
		simple_logger_opts = (FA_WRITE | FA_OPEN_ALWAYS);
	} else {
		return SIMPLE_LOGGER_BAD_PERMISSIONS;
	}

	// Set read permission
	if (permissions[1] == ',' && permissions[2] == 'r') {
		simple_logger_opts |= FA_READ;
	}

	uint8_t err_code = logger_init();
	return  err_code;
}


// An SD card was inserted after being gone for a bit
// Let's reopen the file, and try to rewrite the header if it's necessary
static uint8_t logger_init() {

	volatile FRESULT res = FR_OK;
	res |= f_mount(&simple_logger_fs, "", 1);

	// See if the file system already exists
	while (res != FR_OK) {
		switch (res) {
			case FR_NOT_READY: {
				// No disk found; check "Card Detected" signal before calling this function
				return res;
			}
			case FR_NO_FILESYSTEM: {
				// No existing file system
				res = f_mkfs("", FM_ANY, 0, work, sizeof(work));

				if (res != FR_OK) {
					printf("Failed to make a new filesystem: %d\n", res);
					return res;
				}

				// Retry mounting now
				res = f_mount(&simple_logger_fs, "", 1);
				if (res != FR_OK) {
					return res;
				}

				break;
			}
			default: {
				printf("Unexpected error while mounting SD card: %d\n", res);
				return res;
			}
		}
	}

	// See if the file already exists
	FIL temp;
	res |= f_open(&temp,file, FA_READ | FA_OPEN_EXISTING);

	if(res == FR_NO_FILE) {

		// The file doesn't exist
		simple_logger_file_exists = 0;

	} else if(res == FR_OK) {

		// The file does exist
		simple_logger_file_exists = 1;
		res |= f_close(&temp);
	}

	res |= f_open(&simple_logger_fpointer,file, simple_logger_opts);

	if(simple_logger_opts & FA_OPEN_ALWAYS) {
		// We are in append mode and should move to the end
		res |= f_lseek(&simple_logger_fpointer, f_size(&simple_logger_fpointer));
	}

	if(header_written && !simple_logger_file_exists) {
		f_puts(header_buffer, &simple_logger_fpointer);
		res |= f_sync(&simple_logger_fpointer);
	}

	simple_logger_inited = 1;
	return res;
}


uint8_t simple_logger_log_header(const char *format, ...) {

	header_written = 1;

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(header_buffer, buffer_size, format, argptr);
	va_end(argptr);

	if(!simple_logger_file_exists) {

		f_puts(header_buffer, &simple_logger_fpointer);
		FRESULT res = f_sync(&simple_logger_fpointer);

		if(res != FR_OK) {
			res = logger_init();
			if(res != FR_OK) {
				error();
			}
			return res;
		}

		return res;
	} else {
		return SIMPLE_LOGGER_FILE_EXISTS;
	}
}


// Log data
uint8_t simple_logger_log(const char *format, ...) {

	// ATTENTION: Make sure all strings are <= 255 bytes; the nRF SPI implementation does not allow longer transaction
	// Note: This is due to the underlying DMA being restricted to 255 bytes; for more information, see https://devzone.nordicsemi.com/f/nordic-q-a/16580/send-more-than-255-bytes-by-spi-nrf52

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, buffer_size, format, argptr);
	va_end(argptr);

	// As we are power-gaiting, we need to initiate the logger again and mount the file-system
	logger_init();

	f_puts(buffer, &simple_logger_fpointer);
	FRESULT res = f_sync(&simple_logger_fpointer);

	if(res != FR_OK) {
		res = logger_init();
		if(res == FR_OK) {
			f_puts(buffer, &simple_logger_fpointer);
			res = f_sync(&simple_logger_fpointer);
		} else {
			error();
		}
	}

	return res;
}

// Read data
uint8_t simple_logger_read(uint8_t* buf, uint8_t buf_len) {

    // Buffer should be cleared before calling this function
	UINT read_len = 0;

	// Set read/write pointer back by amount we want to read
	FRESULT res = f_lseek(&simple_logger_fpointer, f_size(&simple_logger_fpointer) - buf_len);

	if(res != FR_OK) {
		printf("ERROR: Failed reverting R/W pointer: %i\n", res);
		error();
	}

	// Read string
	res = f_read(&simple_logger_fpointer, (void*)buf, buf_len, &read_len);

	if (read_len != buf_len) {
		printf("ERROR: Should have read %i bytes, but only read %i\n", buf_len, read_len);
	}

	if(res != FR_OK) {
		printf("ERROR: Failed reading from SD card: %i\n", res);
		error();
	}

	// File pointer must be at EOF again; should be correct

	return res;
}


