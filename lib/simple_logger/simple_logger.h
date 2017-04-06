#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

//////////////USAGE GUIDE////////////
//	//REQUIRES: simple_ble, simple_timer
//	//USES: one simple timer
//
//	//In initialization
//	//permissions
//	//"w" - write
//	//"a" - append (just like c files)
//	simple_logger_init(filename, permissions);
//	
//	//In main loop
//	simple_logger_update()
//
//	//to create one time header in append mode (if the file exists, this won't append)
//	simple_logger_log_header("%s,%d,%0f.2 This is a format string",...vars)
//
//	//to log data
//	simple_logger_log("%s,%d,%f",...vars);
//
//	//currently supports final strings with substituted vars
//	//of max length 256 chars
//	//To have longer strings
//	#define SIMPLE_LOGGER_BUFFER_SIZE N
////////////////////////////////////

enum {
	SIMPLE_LOGGER_SUCCESS = 0,
	SIMPLE_LOGGER_BUSY,
	SIMPLE_LOGGER_BAD_FPOINTER,
	SIMPLE_LOGGER_BAD_FPOINTER_INIT,
	SIMPLE_LOGGER_BAD_CARD,
	SIMPLE_LOGGER_BAD_CARD_INIT,
	SIMPLE_LOGGER_FILE_EXISTS,
	SIMPLE_LOGGER_FILE_ERROR,
	SIMPLE_LOGGER_ALREADY_INITIALIZED,
	SIMPLE_LOGGER_BAD_PERMISSIONS
} SIMPLE_LOGGER_ERROR; 


uint8_t simple_logger_init(const char *filename, const char *permissions);
uint8_t simple_logger_ready(void);
void simple_logger_update();
uint8_t simple_logger_log(const char *format, ...)
		__attribute__ ((format (printf, 1, 2)));
uint8_t simple_logger_log_header(const char *format, ...)
		__attribute__ ((format (printf, 1, 2)));

int		simple_logger_fast_log(const char *format, ...);
uint8_t simple_logger_sync();
void 	simple_logger_debug_stopper();
uint8_t simple_logger_start();
uint8_t simple_logger_stop();
uint8_t simple_logger_fast_log_binary(const void *buf, unsigned int size);
#endif
