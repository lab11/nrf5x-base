
#include <stdint.h>
#include <stdbool.h>
#include "simple_logger.h"
#include "simple_timer.h"
#include "chanfs/ff.h"
#include "chanfs/diskio.h"
#include "stdarg.h"

static uint8_t simple_logger_inited = 0;
static uint8_t simple_logger_file_exists = 0;
static uint8_t busy = 0;
static uint8_t header_written = 0;
static uint8_t error_count = 0;

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
	res |= f_mount(&simple_logger_fs, "", 1);

	//see if the file exists already
	FIL temp;
	res |= f_open(&temp,file, FA_READ | FA_OPEN_EXISTING);
	if(res == FR_NO_FILE) {
		//the file doesn't exist
		simple_logger_file_exists = 0;
	} else if(res == FR_OK) {
		simple_logger_file_exists = 1;
		res |= f_close(&temp);
	}

	res |= f_open(&simple_logger_fpointer,file, simple_logger_opts);

	if(simple_logger_opts && FA_OPEN_ALWAYS) {
		//we are in append mode and should move to the end
		res |= f_lseek(&simple_logger_fpointer, f_size(&simple_logger_fpointer));
	}

	if(header_written && !simple_logger_file_exists) {
		f_puts(header_buffer, &simple_logger_fpointer);
		res |= f_sync(&simple_logger_fpointer);
	}

	simple_logger_inited = 1;
	return res;
}

uint8_t simple_logger_init(const char *filename, const char *permissions) {

	if(simple_logger_inited) {
		return SIMPLE_LOGGER_ALREADY_INITIALIZED; //can only initialize once 
	}

	//initialize a simple timer
	simple_timer_init();
	simple_timer_start (1, heartbeat);
	
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

	uint8_t err_code = logger_init();
	return  err_code;
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
			res == logger_init();
			if(res !== FR_OK) {
				error();
			}
			return res;	
		} 

		return res;
	} else {
		return SIMPLE_LOGGER_FILE_EXISTS;
	}
}

