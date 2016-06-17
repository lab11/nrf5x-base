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
//MMC / SD CARD DRIVER C CODE HEADER FILE

#ifndef MMCSD_C_INIT
#define	MMCSD_C_INIT

#include "mem-types.h"

#define NRF_SPI NRF_SPI1

//--------------------------------
//----- GENERAL USER OPTIONS -----
//--------------------------------
#define	FFS_ACCEPT_SDHC_CARDS								//Comment out to not accept SDHC cards 

#define FFS_INIT_PINS	FFS_INIT_SPI;\
						FFS_INIT_CS_PIN;\
						FFS_INIT_CD_PIN

#define FFS_INIT_SPI	SPI_BUS_SET_TO_LOW_SPEED

#define FFS_INIT_CS_PIN 	nrf_gpio_cfg_output(SPI_CS_PIN)
#define FFS_INIT_CD_PIN 	nrf_gpio_cfg_input(CD_PIN,	NRF_GPIO_PIN_NOPULL)

//----- CONTROL PINS -----
#define	FFS_CE(state)				(state ? nrf_gpio_pin_set(SPI_CS_PIN) : nrf_gpio_pin_clear(SPI_CS_PIN))	//Chip select pin (set high on chip power-up)
#define	FFC_DI						nrf_gpio_pin_read(SPI_MISO_PIN) 	//DO pin of SD card, DI pin of processor (used to check if pin is being pulled low by card)

//----- SPI BUS CONTROL REGISTERS -----
#define	FFS_SPI_BUF_FULL			NRF_SPI->EVENTS_READY			//>0 when the SPI receive buffer contains a received byte, also signifying that transmit is complete
#define	FFS_SPI_TX_BYTE(data)		NRF_SPI->TXD = data 			//Macro to write a byte and start transmission over the SPI bus (the check for empty shouldn't strictly be required, but with this part not including it can cause occasional failure)
#define	FFS_SPI_RX_BYTE_BUFFER		NRF_SPI->RXD 					//Register to read last received byte from

#define FFS_SPI_CLEAR_FLAG			NRF_SPI->EVENTS_READY = 0

//8 bit, MSB transmitted first
//Clock low in idle bus state
//Input data is valid on the rising edge of the clock (Our transmit transistion occurs on transition from active (high) to Idle (low) clock state)
//----- SPI BUS SPEED -----
//SPI BUS LOW SPEED (Min 100KHz, Max 400KHz)  (Min speed required for ACDM41 initalisation)
#define	SPI_BUS_SET_TO_LOW_SPEED	NRF_SPI->PSELSCK 	= SPI_SCK_PIN;\
									NRF_SPI->PSELMOSI 	= SPI_MOSI_PIN;\
									NRF_SPI->PSELMISO 	= SPI_MISO_PIN;\
									NRF_SPI->FREQUENCY 	= SPI_FREQUENCY_FREQUENCY_K250;\
									NRF_SPI->CONFIG 	= (uint32_t)(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) |\
																	(SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos) |\
																	(SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);\
									NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);\
									NRF_SPI->EVENTS_READY = 0

#define	SPI_BUS_SET_TO_FULL_SPEED_MMC	NRF_SPI->PSELSCK 	= SPI_SCK_PIN;\
										NRF_SPI->PSELMOSI 	= SPI_MOSI_PIN;\
										NRF_SPI->PSELMISO 	= SPI_MISO_PIN;\
										NRF_SPI->FREQUENCY 	= SPI_FREQUENCY_FREQUENCY_M8;\
										NRF_SPI->CONFIG 	= (uint32_t)(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) |\
																	(SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos) |\
																	(SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);\
										NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);\
										NRF_SPI->EVENTS_READY = 0

#define	SPI_BUS_SET_TO_FULL_SPEED_SD	NRF_SPI->PSELSCK 	= SPI_SCK_PIN;\
										NRF_SPI->PSELMOSI 	= SPI_MOSI_PIN;\
										NRF_SPI->PSELMISO 	= SPI_MISO_PIN;\
										NRF_SPI->FREQUENCY 	= SPI_FREQUENCY_FREQUENCY_M8;\
										NRF_SPI->CONFIG 	= (uint32_t)(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) |\
																	(SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos) |\
																	(SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);\
										NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);\
										NRF_SPI->EVENTS_READY = 0


//----- CARD DETECT PIN -----
//1 if present 0 if not
#define	FFS_CD_PIN_FUNCTION			!nrf_gpio_pin_read(CD_PIN)	//function or register to return state of the CD PIN

//----- MISC -----
#define	FFS_DRIVER_GEN_512_BYTE_BUFFER	ffs_general_buffer		//This general buffer is used by routines and may be the same as the buffer that
																//the application uses to read and write data from and to the card if ram is limited

#define	DO_BUSY_STATE_ACCESS_DELAY	__asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop")			//('Nop()' is a single cycle null instruction, include multiple times if required to provide time for the card to drive the DO line if it needs to)


//---------------------------------------------
//----- PROCESS CARD STATE MACHINE STATES -----
//---------------------------------------------
typedef enum _FFS_PROCESS_STATE
{
    FFS_PROCESS_NO_CARD,
    FFS_PROCESS_WAIT_FOR_CARD_FULLY_INSERTED,
    FFS_PROCESS_CARD_INITIALSIED
} FFS_PROCESS_STATE;


#endif




//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef MMCSD_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
BYTE ffs_check_command_response_byte (BYTE mask, BYTE data_requried);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void ffs_process (void);
void ffs_init (void);
BYTE ffs_is_card_present (void);
void ffs_read_sector_to_buffer (DWORD sector_lba);
void ffs_write_sector_from_buffer (DWORD sector_lba);
BYTE ffs_write_byte (BYTE data);
WORD ffs_read_word (void);
BYTE ffs_read_byte (void);




#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void ffs_process (void);
extern void ffs_init (void);
extern BYTE ffs_is_card_present (void);
extern void ffs_read_sector_to_buffer (DWORD sector_lba);
extern void ffs_write_sector_from_buffer (DWORD sector_lba);
extern BYTE ffs_write_byte (BYTE data);
extern WORD ffs_read_word (void);
extern BYTE ffs_read_byte (void);



#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef MMCSD_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
BYTE sm_ffs_process = FFS_PROCESS_NO_CARD;
WORD file_system_information_sector;
BYTE ffs_no_of_heads;
BYTE ffs_no_of_sectors_per_track;
DWORD ffs_no_of_partition_sectors;
BYTE card_is_high_capacity;
BYTE chk_cmd_response_data;


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
WORD number_of_root_directory_sectors;				//Only used by FAT16, 0 for FAT32
BYTE ffs_buffer_needs_writing_to_card;
DWORD ffs_buffer_contains_lba = 0xffffffff;
DWORD fat1_start_sector;
DWORD root_directory_start_sector_cluster;			//Start sector for FAT16, start clustor for FAT32
DWORD data_area_start_sector;
BYTE disk_is_fat_32;
BYTE sectors_per_cluster;
DWORD last_found_free_cluster;
DWORD sectors_per_fat;
BYTE active_fat_table_flags;
DWORD read_write_directory_last_lba;
WORD read_write_directory_last_entry;
BYTE ffs_card_write_protected = 0;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern WORD number_of_root_directory_sectors;				//Only used by FAT16, 0 for FAT32
extern BYTE ffs_buffer_needs_writing_to_card;
extern DWORD ffs_buffer_contains_lba;
extern DWORD fat1_start_sector;
extern DWORD root_directory_start_sector_cluster;
extern DWORD data_area_start_sector;
extern BYTE disk_is_fat_32;
extern BYTE sectors_per_cluster;
extern DWORD last_found_free_cluster;
extern DWORD sectors_per_fat;
extern BYTE active_fat_table_flags;
extern DWORD read_write_directory_last_lba;
extern WORD read_write_directory_last_entry;
extern BYTE ffs_card_write_protected;


#endif


