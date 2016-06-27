
#include <stdint.h>
#include <stdbool.h>
#include "simple_logger.h"
#include "simple_timer.h"
#include "chanfs/ff.h"
#include "chanfs/diskio.h"
#include "stdarg.h"

static uint32_t simple_logger_timer = 0;
static uint8_t simple_logger_inited = 0;
static uint8_t simple_logger_file_exists = 0;
static uint8_t busy = 0;
static uint8_t header_written = 0;

const char *file = NULL;
const char *perm = NULL;

#ifdef SIMPLE_LOGGER_BUFFER_SIZE
	static char buffer[SIMPLE_LOGGER_BUFFER_SIZE];
	static char header_buffer[SIMPLE_LOGGER_BUFFER_SIZE];
	static uint32_t buffer_size = SIMPLE_LOGGER_BUFFER_SIZE;
#else
	static char buffer[256];
	static char header_buffer[256];
	static uint32_t buffer_size = 256;
#endif


static FIL* 	simple_logger_fpointer;
static FATFS 	simple_logger_fs;

static void heartbeat (void* p_context) {
	disk_timerproc();
}


//an sd card was inserted after being gone for a bit
//let's reopen the file, and try to rewrite the header if it's necessary
static uint8_t logger_init() {

	f_mount(&simple_logger_fs, "", 1);

	//we must have not timed out and a card is available
	if((perm[0] != 'w' && perm[0] != 'a') || perm[1] != '\0') {
		//the person didn't use the right permissions
		return SIMPLE_LOGGER_BAD_PERMISSIONS;
	}
	
	//see if the file exists already
	FIL* temp;
	temp = f_open(file,"r");
	if(temp) {
		//the file exists
		f_close(temp);
		simple_logger_file_exists = 1;
	} else {
		simple_logger_file_exists = 0;
	}

	simple_logger_fpointer = f_open(file,perm);
	if(!simple_logger_fpointer) {
		return SIMPLE_LOGGER_BAD_FPOINTER_INIT; //idk what's up
	}

	if(header_written && !simple_logger_file_exists) {
		f_puts(header_buffer, simple_logger_fpointer);
		f_flush(simple_logger_fpointer);
		if(f_error(simple_logger_fpointer)) 
			return SIMPLE_LOGGER_FILE_ERROR;
	}

	simple_logger_inited = 1;
	return SIMPLE_LOGGER_SUCCESS;
}

uint8_t simple_logger_init(const char *filename, const char *permissions) {

	if(busy) {
		return SIMPLE_LOGGER_BUSY;
	} else {
		busy = 1;
	}

	if(simple_logger_inited) {
		busy = 0;
		return SIMPLE_LOGGER_ALREADY_INITIALIZED; //can only initialize once 
	}

	//initialize a simple timer
	simple_timer_init();
	simple_timer_start (1, heartbeat);
	
	file = filename;
	perm = permissions;

	uint8_t err_code = logger_init();
	busy = 0;
	return  err_code;
}

//the function meant to log data
uint8_t simple_logger_log(const char *format, ...) {

	if(busy) {
		return SIMPLE_LOGGER_BUSY;
	} else {
		busy = 1;
	}
	
	if(!simple_logger_inited) {
		uint8_t err_code = logger_init();
		if(err_code) {
			busy = 0;
			return err_code;
		}
	}

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, buffer_size, format, argptr);
	va_end(argptr);

	if(simple_logger_fpointer) {
		f_puts(buffer, simple_logger_fpointer);
		f_flush(simple_logger_fpointer);
		if(f_error(simple_logger_fpointer)) {
			busy = 0;
			return SIMPLE_LOGGER_FILE_ERROR;
		} else {
			busy = 0;
			return SIMPLE_LOGGER_SUCCESS;
		}
	} else {
		busy = 0;
		return SIMPLE_LOGGER_BAD_FPOINTER;
	}
}

uint8_t simple_logger_log_header(const char *format, ...) {

	if(busy) {
		return SIMPLE_LOGGER_BUSY;
	} else {
		busy = 1;
	}

	header_written = 1;
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(header_buffer, buffer_size, format, argptr);
	va_end(argptr);

	if(!simple_logger_inited) {
		uint8_t err_code = logger_init();
		if(err_code) {
			busy = 0;
			return err_code;
		}
	}

	if(!simple_logger_file_exists) {
		if(simple_logger_fpointer) {
			f_puts(header_buffer, simple_logger_fpointer);
			f_flush(simple_logger_fpointer);
			if(f_error(simple_logger_fpointer)) {
				busy = 0;
				return SIMPLE_LOGGER_FILE_ERROR;
			} else {
				busy = 0;
				return SIMPLE_LOGGER_SUCCESS;
			}
		} else {
			busy = 0;
			return SIMPLE_LOGGER_BAD_FPOINTER;
		}
	} else {
		busy = 0;
		return SIMPLE_LOGGER_FILE_EXISTS;
	}
}

