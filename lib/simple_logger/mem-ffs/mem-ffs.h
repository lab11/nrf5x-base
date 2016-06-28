/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		FAT FILING SYSTEM FAT16 & FAT 32 DRIVER
//FAT FILING SYSTEM DRIVER C CODE HEADER FILE

#ifndef FFS_C_INIT
#define	FFS_C_INIT

#include "mem-types.h"



//##################################
//##################################
//########## USING DRIVER ##########
//##################################
//##################################

//############################
//##### ADD TO HEARTBEAT #####
//############################
/*
	//------------------------------
	//----- HERE EVERY 10 mSec -----
	//------------------------------
	//----- FAT FILING SYSTEM DRIVER TIMER -----
	if (ffs_10ms_timer)
		ffs_10ms_timer--;
*/

//############################
//##### ADD TO MAIN LOOP #####
//############################
/*
	//----- PROCESS FAT FILING SYSTEM -----
	ffs_process();
*/

//For further information please see the project technical manual


//---------------------------------------
//----- CLEAR WATCHDOG TIMER DEFINE -----					//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//---------------------------------------
//Use this if you have a watchdog timer that needs to be reset for operations that can take a long time
//Comment out if not requried.

#define	CLEAR_WATCHDOG_TIMER	

//------------------------
//----- USER DEFINES -----									//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//------------------------
#define	FFS_FOPEN_MAX				2		//Maximum number of files that may be opened simultaneously (1 - 254).  22 bytes or memory requried per file.


//---------------------------
//----- REAL TIME CLOCK -----						//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//---------------------------
//#define	USE_REAL_TIME_CLOCK						//Comment out if you do not have a real time clock
#ifdef USE_REAL_TIME_CLOCK							//Defines to your real time clock varaibles.  Add an include to the file that contains the variable definitions to the top of mem-ffs.c
#define	FFS_RTC_YEAR		rtc_year				//BYTE 00 (2000) - 99 (2099)
#define	FFS_RTC_MONTH		rtc_month				//BYTE 1 - 12 (1 = Jan)
#define	FFS_RTC_DATE		rtc_date				//BYTE 1 - 31
#define	FFS_RTC_HOURS		rtc_hours				//BYTE 0 - 23
#define	FFS_RTC_MINUTES		rtc_minutes				//BYTE 0 - 59
#define	FFS_RTC_SECONDS		rtc_seconds				//BYTE 0 - 59
#endif //#ifdef USE_REAL_TIME_CLOCK


//-------------------------------------------------
//----- USING STANDRD TYPE AND FUNCTION NAMES -----			//<<<<< CHECK FOR A NEW APPLICATION <<<<<
//-------------------------------------------------
//For ease of interoperability this driver uses modified version of the standard ANSI-C function names and FILE type.
//To avoid conflicting with your compilers stdio.h definitions you may comment out this section and use the modified names in your code.
//If you want to use the ANSI-C standard names un-comment this section.
/*
#define	fopen			ffs_fopen
#define	fseek			ffs_fseek
#define	ftell			ffs_ftell
#define	fgetpos			ffs_fgetpos
#define	fsetpos			ffs_fsetpos
#define	rewind			ffs_rewind
#define	fputc			ffs_fputc
#define	fgetc			ffs_fgetc
#define	fputs			ffs_fputs
#define	fgets			ffs_fgets
#define	fwrite			ffs_fwrite
#define	fread			ffs_fread
#define	fflush			ffs_fflush
#define	fclose			ffs_fclose
#define	remove			ffs_remove
#define	rename			ffs_rename
#define	clearerr		ffs_clearerr
#define	feof			ffs_feof
#define	ferror			ffs_ferror
#define	putc			ffs_putc
#define	getc			ffs_getc

#define	EOF				FFS_EOF
#define	SEEK_SET		FFS_SEEK_SET
#define	SEEK_CUR		FFS_SEEK_CUR
#define	SEEK_END		FFS_SEEK_END
*/






//----- DATA TYPE DEFINITIONS -----
typedef struct _FFS_FILE
{
	DWORD directory_entry_sector;						//The sector that contains the entry for the file being accessed
	BYTE directory_entry_within_sector;					//The entry number within that sector (512 / 32 = 16 so max range is 0-15 for a 512 bytes per sector disk)
	DWORD current_cluster;								//The current cluster being accessed for the file
	BYTE current_sector;								//The current sector within the current cluster
	WORD current_byte;									//The current byte within the current sector
	DWORD current_byte_within_file;						//The current byte within the overall file
	DWORD file_size;									//The current size of the file

	union
	{
		struct
		{
			unsigned int file_is_open				:1;	//This is the master 'this file handler is in use / not in use' flag
			unsigned int read_permitted				:1;
			unsigned int write_permitted			:1;
			unsigned int write_append_only			:1;
			unsigned int inc_posn_before_next_rw	:1;	//The location of the current byte pointer needs to be incremented before the next read or write operation
			unsigned int access_error				:1;
			unsigned int end_of_file				:1;
			unsigned int file_size_has_changed		:1;
			unsigned int reserved					:8;
		} bits;
		WORD word;
	} flags;

} FFS_FILE;



//FSEEK origin defines:-
#define	FFS_SEEK_SET		0			//Beginning of file
#define	FFS_SEEK_CUR		1			//Current position of the file pointer
#define	FFS_SEEK_END		2			//End of file


//EOF VALUE DEFINE:-
#define	FFS_EOF				-1


#endif




//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef FFS_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
DWORD ffs_find_file (const char *filename, DWORD *file_size, BYTE *attribute_byte, DWORD *directory_entry_sector, BYTE *directory_entry_within_sector, BYTE *read_file_name, BYTE *read_file_extension);
BYTE ffs_convert_filename_to_dos (const char *source_filename, BYTE *dos_filename, BYTE *dos_extension);
BYTE ffs_read_next_directory_entry (BYTE *file_name, BYTE *file_extension, BYTE *attribute_byte, DWORD *file_size, DWORD *cluster_number, BYTE start_from_beginning, DWORD *directory_entry_sector, BYTE *directory_entry_within_sector);
void ffs_overwrite_last_directory_entry (BYTE *file_name, BYTE *file_extension, BYTE *attribute_byte, DWORD *file_size, DWORD *cluster_number, BYTE write_time_date);
DWORD get_file_start_cluster(FFS_FILE *file_pointer);
BYTE ffs_create_new_file (const char *file_name, DWORD *write_file_start_cluster, DWORD *directory_entry_sector, BYTE *directory_entry_within_sector);
DWORD ffs_get_next_free_cluster (void);
DWORD ffs_get_next_cluster_no (DWORD current_cluster);
void ffs_modify_cluster_entry_in_fat (DWORD cluster_to_modify, DWORD cluster_entry_new_value);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
FFS_FILE* ffs_fopen (const char *filename, const char *access_mode);
int  ffs_fseek (FFS_FILE *file_pointer, long offset, int origin);
int  ffs_fsetpos (FFS_FILE *file_pointer, long *position);
long ffs_ftell (FFS_FILE *file_pointer);
int ffs_fgetpos (FFS_FILE *file_pointer, long *position);
void ffs_rewind (FFS_FILE *file_pointer);
int ffs_fputc (int data, FFS_FILE *file_pointer);
int ffs_fgetc (FFS_FILE *file_pointer);
int ffs_fputs (const char *string, FFS_FILE *file_pointer);
int ffs_fputs_char (char *string, FFS_FILE *file_pointer);
char* ffs_fgets (char *string, int length, FFS_FILE *file_pointer);
int ffs_fwrite (const void *buffer, int size, int count, FFS_FILE *file_pointer);
int ffs_fread (void *buffer, int size, int count, FFS_FILE *file_pointer);
int ffs_fflush (FFS_FILE *file_pointer);
int	ffs_fclose (FFS_FILE *file_pointer);
int ffs_remove (const char *filename);
int ffs_change_file_size (const char *filename, DWORD new_file_size);
int ffs_rename (const char *old_filename, const char *new_filename);
void ffs_clearerr (FFS_FILE *file_pointer);
int ffs_feof (FFS_FILE *file_pointer);
int ffs_ferror (FFS_FILE *file_pointer);
BYTE ffs_is_card_available (void);



#define	ffs_putc(data, file_pointer)	ffs_fputc(data, file_pointer)			//Defined in header file - these 2 functions are equivalent
#define ffs_getc(file_pointer)		ffs_fgetc(file_pointer)						//Defined in header file - these 2 functions are equivalent

#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void ffs_process (void);					//< This function is not actually in our matching c file, but we include here so that application files that use this driver only have to #include "mem-ffs.h" and not the lower level card driver .h file
extern void ffs_init (void);					//< This function is not actually in our matching c file, but we include here so that application files that use this driver only have to #include "mem-ffs.h" and not the lower level card driver .h file
extern FFS_FILE* ffs_fopen (const char *filename, const char *access_mode);
extern int  ffs_fseek (FFS_FILE *file_pointer, long offset, int origin);
extern int  ffs_fsetpos (FFS_FILE *file_pointer, long *position);
extern long ffs_ftell (FFS_FILE *file_pointer);
extern int ffs_fgetpos (FFS_FILE *file_pointer, long *position);
extern void ffs_rewind (FFS_FILE *file_pointer);
extern int ffs_fputc (int data, FFS_FILE *file_pointer);
extern int ffs_fgetc (FFS_FILE *file_pointer);
extern int ffs_fputs (const char *string, FFS_FILE *file_pointer);
extern int ffs_fputs_char (char *string, FFS_FILE *file_pointer);
extern char* ffs_fgets (char *string, int length, FFS_FILE *file_pointer);
extern int ffs_fwrite (const void *buffer, int size, int count, FFS_FILE *file_pointer);
extern int ffs_fread (void *buffer, int size, int count, FFS_FILE *file_pointer);
extern int ffs_fflush (FFS_FILE *file_pointer);
extern int	ffs_fclose (FFS_FILE *file_pointer);
extern int ffs_remove (const char *filename);
extern int ffs_change_file_size (const char *filename, DWORD new_file_size);
extern int ffs_rename (const char *old_filename, const char *new_filename);
extern void ffs_clearerr (FFS_FILE *file_pointer);
extern int ffs_feof (FFS_FILE *file_pointer);
extern int ffs_ferror (FFS_FILE *file_pointer);
extern BYTE ffs_is_card_available (void);



#define	ffs_putc(data, file_pointer)	ffs_fputc(data, file_pointer)			//Defined in header file - these 2 functions are equivalent
#define ffs_getc(file_pointer)		ffs_fgetc(file_pointer)						//Defined in header file - these 2 functions are equivalent

#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef FFS_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------




//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
FFS_FILE ffs_file[FFS_FOPEN_MAX];
BYTE ffs_card_ok = 0;
volatile BYTE ffs_10ms_timer = 0;
WORD ffs_bytes_per_sector;


//----- 512 BYTE DRIVER DATA BUFFER -----
//Required as read and write operations are carried out on complete sectors which are 512 bytes in size
//This buffer is only referenced through a pointer by the driver to allow for compillers and processors that have this requirement.
#ifdef FFS_USING_MICROCHIP_C18_COMPILER
	#pragma udata ffs_512_byte_ram_section			//This is the PIC C18 compiler command to use the specially defined section in the linker script (PIC18 project uses a modified linker script for this)
	BYTE ffs_general_buffer[512];					//(C18 large array requirement to use a special big section of ram defined in the linker script)
	#pragma udata
#else
	BYTE ffs_general_buffer[512];
#endif


#else	//FFS_C
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern FFS_FILE ffs_file[FFS_FOPEN_MAX];
extern BYTE ffs_card_ok;
volatile extern BYTE ffs_10ms_timer;
extern WORD ffs_bytes_per_sector;


//----- 512 BYTE DRIVER DATA BUFFER -----
//Required as read and write operations are carried out on complete sectors which are 512 bytes in size
//This buffer is only referenced through a pointer by the driver to allow for compillers and processors that have this requirement.
#ifdef FFS_USING_MICROCHIP_C18_COMPILER
	#pragma udata ffs_512_byte_ram_section			//This is the PIC C18 compiler command to use the specially defined section in the linker script (PIC18 project uses a modified linker script for this)
	extern BYTE ffs_general_buffer[512];			//(C18 large array requirement to use a special big section of ram defined in the linker script)
	#pragma udata
#else
	extern BYTE ffs_general_buffer[512];
#endif


#endif	//FFS_C


