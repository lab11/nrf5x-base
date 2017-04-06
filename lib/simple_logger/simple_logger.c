
#include <stdint.h>
#include <stdbool.h>
#include "simple_logger.h"
#include "app_timer.h"
#include "chanfs/ff.h"
#include "chanfs/diskio.h"
#include "stdarg.h"
#include "led.h"
#include "board.h"
#include "nrf_delay.h"

/* DEBUG FUNCS/VARIABLES */

/* Errors from logger_init */
static volatile uint8_t logger_init_mount_error=0;
static volatile uint8_t logger_init_file_exist_error=0;
static volatile uint8_t logger_init_file_open_error=0;
static volatile uint8_t logger_init_file_sync_error=0;
static volatile uint8_t logger_init_lseek_error=0;
static volatile uint8_t logger_init_res=0;

static volatile uint32_t total_errors=0;

static volatile uint8_t checker=0;

void simple_logger_debug_stopper() {checker += 1;}


/* END DEBUG STUFF */

static uint8_t simple_logger_inited = 0;
static uint8_t simple_logger_file_exists = 0;
static uint8_t header_written = 0;
static uint8_t error_count = 0;

static uint32_t heartbeat_ticks = APP_TIMER_TICKS(1,0);
APP_TIMER_DEF(simple_logger_heartbeat);

const char *file = NULL;

#ifdef SIMPLE_LOGGER_BUFFER_SIZE
	static char buffer[SIMPLE_LOGGER_BUFFER_SIZE];
	static char header_buffer[SIMPLE_LOGGER_BUFFER_SIZE];
	static uint32_t buffer_size = SIMPLE_LOGGER_BUFFER_SIZE;
#else
	static char buffer[256];
	static char header_buffer[256];
	static uint32_t buffer_size = 256;
#endif


static FIL 	simple_logger_fpointer;
static FATFS 	simple_logger_fs;
static uint8_t simple_logger_opts;

extern void disk_timerproc(void);
extern void disk_restart(void);

static void error(void) {
	error_count++;
	total_errors++;
	if(error_count > 20) {
		disk_restart();
		error_count = 0;
	}
}

static void heartbeat (void* p_context) {
	disk_timerproc();
}

//an sd card was inserted after being gone for a bit
//let's reopen the file, and try to rewrite the header if it's necessary
static uint8_t logger_init() {

	volatile FRESULT res;
	
	logger_init_mount_error= f_mount(&simple_logger_fs, "", 1);
	res |= logger_init_mount_error;
	if(logger_init_mount_error != 0) {
		simple_logger_debug_stopper();
	}
	while(logger_init_mount_error == 3) {
		led_toggle(LED_GREEN);
		disk_restart();
		logger_init_mount_error = f_mount(&simple_logger_fs, "", 1);
	}
	led_off(LED_GREEN);
	//see if the file exists already
	FIL temp;
	logger_init_file_exist_error |= f_open(&temp,file, FA_READ | FA_OPEN_EXISTING);
	if(logger_init_file_exist_error != FR_OK && logger_init_file_exist_error != FR_NO_FILE) {
		res |= logger_init_file_exist_error;
	}

	if(logger_init_file_exist_error == FR_NO_FILE) {
		simple_logger_file_exists = 0;
	} else if(logger_init_file_exist_error == FR_OK) {
		simple_logger_file_exists = 1;
		res |= f_close(&temp);
	}

	logger_init_file_open_error = f_open(&simple_logger_fpointer,file, simple_logger_opts);
	logger_init_file_sync_error = f_sync(&simple_logger_fpointer);
	res |= logger_init_file_open_error;
	res |= logger_init_file_sync_error;

	if(simple_logger_opts && FA_OPEN_ALWAYS) {
		//we are in append mode and should move to the end
		logger_init_lseek_error = f_lseek(&simple_logger_fpointer, f_size(&simple_logger_fpointer));
		res |= logger_init_lseek_error;
	}

	if(header_written && !simple_logger_file_exists) {
		f_puts(header_buffer, &simple_logger_fpointer);
		res |= f_sync(&simple_logger_fpointer);
	}
	
	logger_init_res = res;
	simple_logger_inited = 1;
	return res;
}

/* Inits AND starts simple_logger */
uint8_t simple_logger_init(const char *filename, const char *permissions) {
	if(simple_logger_inited) {
		return SIMPLE_LOGGER_ALREADY_INITIALIZED; //can only initialize once
	}
	disk_restart();

	/* Create and start a heartbeat timer for sdcard timing informatoin */
	app_timer_create(&simple_logger_heartbeat, APP_TIMER_MODE_REPEATED, heartbeat);
	app_timer_start(simple_logger_heartbeat, heartbeat_ticks, NULL);

	file = filename;

	//we must have not timed out and a card is available
	if((permissions[0] != 'w' && permissions[0] != 'a') || permissions[1] != '\0') {
		//the person didn't use the right permissions
		return SIMPLE_LOGGER_BAD_PERMISSIONS;
	} else if(permissions[0] == 'w') {
		simple_logger_opts = (FA_WRITE | FA_CREATE_ALWAYS);
	} else {
		simple_logger_opts = (FA_WRITE | FA_OPEN_ALWAYS);
	}

	return logger_init();
}

/* Stop simple logger and power down the SD card */
uint8_t simple_logger_stop() {
	uint8_t res = f_close(&simple_logger_fpointer);
	res |= f_mount(NULL, "", 1);
	app_timer_stop(simple_logger_heartbeat);
	disk_off();
	return res;
}

/* Starts simple logger up again. Only call after calling simple_logger_stop() */
uint8_t simple_logger_start() {
	disk_restart();
	app_timer_start(simple_logger_heartbeat, heartbeat_ticks, NULL);
	return logger_init();
}

//the function meant to log data
uint8_t simple_logger_log(const char *format, ...) {

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, buffer_size, format, argptr);
	va_end(argptr);

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

/* Logs data without calling f_sync */
int simple_logger_fast_log(const char *format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, buffer_size, format, argptr);
	va_end(argptr);

	return f_puts(buffer, &simple_logger_fpointer);
}

uint8_t simple_logger_fast_log_binary(const void *buf, unsigned int size) {
	unsigned int bw = 0;
	return f_write(&simple_logger_fpointer, buf, size, &bw);
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



uint8_t simple_logger_sync() {
	return f_sync(&simple_logger_fpointer);
}
