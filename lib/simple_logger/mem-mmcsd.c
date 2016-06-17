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
//MMC / SD CARD DRIVER C CODE FILE

#define MMCSD_C
#include "board.h"
#include "mem-mmcsd.h"
#include "mem-ffs.h"


//*******************************************************
//*******************************************************
//********** DO BACKGROUND MMC / SD CARD TASKS **********
//*******************************************************
//*******************************************************
//This function needs to be called reguarly to detect a new card being inserted so that it can be initialised ready for access.

//0 is low
//1 is high
static uint8_t speed = 0;

void ffs_init (void) {
	FFS_INIT_PINS;
}

void ffs_process (void)
{
	BYTE head;
	WORD cylinder_no;
	BYTE sector;

	volatile BYTE b_temp;
	WORD w_temp;
	DWORD dw_temp;
	DWORD lba;
	DWORD main_partition_start_sector;
	WORD number_of_reserved_sectors;
	BYTE number_of_copies_of_fat;
	BYTE *buffer_pointer;


	switch (sm_ffs_process)
	{
	case FFS_PROCESS_NO_CARD:
		//-------------------------------
		//----- NO CARD IS INSERTED -----
		//-------------------------------
		FFS_CE(1);
		
		ffs_card_ok = 0;					//Flag that card not OK

		//Reset all file handlers
		for (b_temp = 0; b_temp < FFS_FOPEN_MAX; b_temp++)
			ffs_file[b_temp].flags.bits.file_is_open = 0;

		//Has a card has been inserted?
		if (ffs_is_card_present() == 0)
			return;

		//A card has been inserted
		//Pause for 500mS seconds
		ffs_10ms_timer = 50;
		sm_ffs_process = FFS_PROCESS_WAIT_FOR_CARD_FULLY_INSERTED;

		return;


    case FFS_PROCESS_WAIT_FOR_CARD_FULLY_INSERTED:
		//------------------------------------------------------------
		//----- CARD INSERTED - WAIT FOR IT TO BE FULLY INSERTED -----
		//------------------------------------------------------------
		//(To allow for users that don't insert the card in one nice quick movement)

		//Ensure card is still inserted
		if (ffs_is_card_present() == 0)
		{
			sm_ffs_process = FFS_PROCESS_NO_CARD;
			return;
		}

		//Wait for timer to expire
		if (ffs_10ms_timer)
			return;
		
		//Initialise the card
		sm_ffs_process = FFS_PROCESS_CARD_INITIALSIED;

		//Actually exit this switch statement to run the card initialise procedure below (this is the only state that doesn't return)
		break;

    case FFS_PROCESS_CARD_INITIALSIED:
		//-----------------------------------------
		//----- CARD INSERTED AND INITIALSIED -----
		//-----------------------------------------

		//If card is still inserted then exit
		if (ffs_is_card_present())
			return;

		//CARD HAS BEEN REMOVED
		ffs_card_ok = 0;
		
		sm_ffs_process = FFS_PROCESS_NO_CARD;

		return;


	default:
		sm_ffs_process = FFS_PROCESS_NO_CARD;
		return;
	}

	//----------------------------------------
	//----------------------------------------
	//----- INITIALISE NEW MMC / SD CARD -----
	//----------------------------------------
	//----------------------------------------
	//(The only state that exits the switch statement above is FFS_PROCESS_WAIT_FOR_CARD_RESET when it completes)

	SPI_BUS_SET_TO_LOW_SPEED;
	speed = 0;

	ffs_card_ok = 0;						//Default to card not OK

	FFS_CE(1);								//Ensure card is de-selected

	//Send at least 74 clocks with DI high for card to enter native command mode
	for (b_temp = 0; b_temp < 10; b_temp++)
	{
		ffs_write_byte(0xff);
	}

	FFS_CE(0);								//Select card, telling card to go into SPI mode when it gets the RESET command

	//----- SEND COMMAND 0  -----
	//Reset into SPI mode
	ffs_write_byte(0x40 + 0);				//Command
	ffs_write_byte(0);						//All arguments zero
	ffs_write_byte(0);
	ffs_write_byte(0);
	ffs_write_byte(0);
	ffs_write_byte(0x95);					//Hard coded CRC

	if (!ffs_check_command_response_byte(0x81, 0x01))		//Get response
		goto	init_new_ffs_card_fail;

	ffs_write_byte(0xff);					//Ensure card is ready for next command
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);
	ffs_write_byte(0xff);



	//----- SEND COMMAND 8 -----
	//Send Interface Condition - mandatory for V2.00 SD specification
	//(This command will fail on MMC cards and SD cards prior to V2.00)
	
	ffs_write_byte(0x40 + 8);				//Command
	ffs_write_byte(0);
	ffs_write_byte(0);
	ffs_write_byte(1);						//Voltage supplied = 2.7 - 3.6V
	ffs_write_byte(0xaa);					//SD spec recomended check pattern
	ffs_write_byte(0x87);					//Hard coded CRC (CMD8 CRC verification is always required)

	//If command is accepted we should get a R1 response of 0x01
	b_temp = ffs_check_command_response_byte(0xff, 0x01);		//Get response

	card_is_high_capacity = 1;				//Default to assuming card is High Capacity type

	if (b_temp)
	{
		//COMMAND WAS ACCEPTED - CARD IS A V2.00 COMPLIANT SD CARD
		//Get the CMD8 R7 response
		ffs_read_byte();
		ffs_read_byte();
		b_temp = ffs_read_byte();			//Voltage accepted
		if ((b_temp & 0x0f) != 0x01)
			goto init_new_ffs_card_fail;	//Our standard voltage range is not accepted by this card
		b_temp = ffs_read_byte();			//Copy of our check pattern
		if (b_temp != 0xaa)
			card_is_high_capacity = 0;
	}
	else
	{
		card_is_high_capacity = 0;			//Card is not V2.00 SD complaint - can't be SDHC card
	}
	ffs_read_byte();



	//----- TURN ON CRC VERIFICATION -----
	ffs_write_byte(0x40 + 59);					//Command
	ffs_write_byte(0);
	ffs_write_byte(0);
	ffs_write_byte(0);
	ffs_write_byte(0x01);					//Bit 0 = 1 = CRC check on
	ffs_write_byte(0xff);					//Don't bother with CRC any more as not requried (dummy value)

	ffs_check_command_response_byte(0xff, 0x01);
	ffs_read_byte();



	//----- SEND COMMAND ACMD41 -----
	//SD card Do initialise - send until the card has completed its initialisation and is ready
	//(This can take some time, especially on larger cards - 100's of mS)
	//If card is MMC we expect it to fail this process as MMC cards do not accept ACMD41 (this is how we detect card is not MMC)
	ffs_10ms_timer = 250;
	b_temp = 0;
	while (1)
	{
		//Do timeout just in case
		if(ffs_10ms_timer == 0)
			goto init_new_ffs_card_fail;

		//First send Application Command Follows command
		ffs_write_byte(0x40 + 55);					//Command
		ffs_write_byte(0);							//All arguments zero
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0x65);						//Hard coded CRC value
	
		ffs_check_command_response_byte(0x80, 0x00);	//Bit 7 will be low for the response byte

		b_temp = chk_cmd_response_data;				//Get the response byte that ffs_check_command_response_byte just received
		if (b_temp & 0x04)
			goto init_new_ffs_card_init_mmc;		//Illegal command response - this command is mandatory for SD card and not implemented for MMC card, so card must be MMC.

		if (b_temp != 0x01)
			continue;								//Response was not sucessful so try CMD55 again

		ffs_read_byte();


		//Then send Initialise Command (including us informing the SD card of our host capacity support)
		ffs_write_byte(0x40 + 41);					//Command
		ffs_write_byte(0x40);						//Bit 30 high to indicate high capacity support, remaining bits must be zero
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0x77);						//Hard coded CRC value

		ffs_check_command_response_byte(0x80, 0x00);	//Bit 7 will be low for the response byte

		b_temp = chk_cmd_response_data;				//Get the response byte that ffs_check_command_response_byte just received
		if (b_temp & 0x04)
			goto init_new_ffs_card_init_mmc;		//Illegal command response - this command is mandatory for SD card and not implemented for MMC card, so card must be MMC.

		if (b_temp == 0x00)
			break;									//Response is no longer idle so SD card is detected and not busy.  Now get the remainder of the response.

		ffs_read_byte();

	}
	//---------------------------
	//----- CARD IS SD CARD -----
	//---------------------------
	ffs_read_byte();


	//----- SEND COMMAND CMD58 -----
	//Read OCR
	
	//SD OCR Register:-
	//Bit 31 Card power up status bit - LOW if the card has not finished the power up routine
	//Bit 30 Card Capacity Status (CCS) - 1 if card is High Capacity SD Memory Card.
	//Bits 29:24 reserved
	//Bits 23:15 Voltage ranges
	//Bits 14:0 reserved
	
	ffs_10ms_timer = 100;
	b_temp = 0;
	while ((b_temp & 0x80) == 0)				//Bit 31 of the OCR must be high for bit 30 to be valid
	{
		//Do timeout just in case
		if(ffs_10ms_timer == 0)
			goto init_new_ffs_card_fail;

		ffs_write_byte(0x40 + 58);				//Command
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0xfd);					//Hard coded CRC value
	
		b_temp = ffs_check_command_response_byte(0xff, 0x00);
		b_temp = chk_cmd_response_data;			//Get the response byte that ffs_check_command_response_byte just received
		if (b_temp != 0)						//Response should be 0
		{
			b_temp = 0;
			continue;
		}
		b_temp = ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
	}

	if (b_temp & 0x40)				//If bit 30 is 1 then card is SDHC type
	{
		card_is_high_capacity = 1;			//The addressing mode is block address - data read/write operations are commanded in block address insted of byte address (block size fixed to 512 bytes)
	}
	else
	{
		card_is_high_capacity = 0;			//The addressing mode is standard byte address
	}


	//----- TURN OFF CRC VERIFICATION -----
	for (b_temp = 0; b_temp < 3; b_temp++)			//Send 3 times to ensure it is receieved
	{
		ffs_write_byte(0x40 + 59);					//Command
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0x00);					//Bit 0 = 1 = CRC check on
		ffs_write_byte(0x91);					//Hard coded CRC

		ffs_check_command_response_byte(0xff, 0x01);
		ffs_read_byte();
	}

	//SET BUS TO FULL SPEED FOR SD CARD (25MHz)
	SPI_BUS_SET_TO_FULL_SPEED_SD; 			//(This must not be done before ACDM41)
	speed = 1;


	goto init_new_ffs_card_init_mmc_done;






init_new_ffs_card_init_mmc:
	//----------------------------
	//----- CARD IS MMC CARD -----
	//----------------------------
	card_is_high_capacity = 0;					//Default to MMC card is not high capacity type

	//----- TURN OFF CRC VERIFICATION -----
	for (b_temp = 0; b_temp < 3; b_temp++)			//Send 3 times to ensure it is receieved
	{
		ffs_write_byte(0x40 + 59);					//Command
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0x00);					//Bit 0 = 1 = CRC check on
		ffs_write_byte(0x91);					//Hard coded CRC

		ffs_check_command_response_byte(0xff, 0x01);
		ffs_read_byte();
	}


	//----- SEND COMMAND 1 -----
	//Do initialise - send until the card has completed its initialisation and is ready
	//(This can take some time, especially on larger cards - 100's of mS)
	ffs_10ms_timer = 250;
	b_temp = 0;
	while ((b_temp & 0x01) == 0)
	{
		ffs_write_byte(0x40 + 1);				//Command
		ffs_write_byte(0x40);					//Bit 30 high to indicate high capacity support, remaining bits must be zero
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0xff);					//Don't bother with CRC any more as not requried (dummy value)

		b_temp = ffs_check_command_response_byte(0x81, 0x00);	//Check for valid response
		ffs_read_byte();

		//Do timeout just in case
		if(ffs_10ms_timer == 0)
			goto init_new_ffs_card_fail;
	}


	//----- SEND COMMAND CMD58 -----
	//Read OCR
	
	//MMC OCR Register:-
	//Bit 31 Card power up status bit - LOW if the card has not finished the power up routine
	//Bit 30:29 Access mode - 10 if card is High Capacity MMC Memory Card with sector addressing, 00 if card is standard byte addressing
	//Bits 28:24 reserved
	//Bit 23:7 Voltage ranges
	//Bits 6:0 reserved

	ffs_10ms_timer = 100;
	b_temp = 0;
	while ((b_temp & 0x80) == 0)				//Bit 31 of the OCR must be high for bit 30 to be valid
	{
		//Do timeout just in case
		if(ffs_10ms_timer == 0)
			goto init_new_ffs_card_fail;

		ffs_write_byte(0x40 + 58);				//Command
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0);
		ffs_write_byte(0xfd);					//Hard coded CRC value
	
		b_temp = ffs_check_command_response_byte(0xff, 0x00);
		if (b_temp == 0)
		{
			//No response - card does not accept CMD58
			b_temp = 0;
			break;
		}
		b_temp = chk_cmd_response_data;			//Get the response byte that ffs_check_command_response_byte just received
		if (b_temp != 0)						//Response should be 0
		{
			b_temp = 0;
			continue;
		}
		b_temp = ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
		ffs_read_byte();
	}

	if (b_temp & 0x40)				//If bit 30 is 1 then card is high capacity type
	{
		card_is_high_capacity = 1;			//The addressing mode is sector address
	}
	else
	{
		card_is_high_capacity = 0;			//The addressing mode is standard byte address
	}


	//SET BUS TO FULL SPEED FOR MMC CARD (20MHz)
	SPI_BUS_SET_TO_FULL_SPEED_MMC; 			//(This must not be done before ACDM41)
	speed = 1;

init_new_ffs_card_init_mmc_done:






	FFS_CE(1);									//De-select card

	ffs_write_byte(0xff);						//Output extra clock pulses in case card is still completing an operation

	FFS_CE(0);									//Select card


	//----- SEND COMMAND 16 -----
	//Block length
	ffs_write_byte(0x40 + 16);				//Command
	ffs_write_byte(0x00);					//Data
	ffs_write_byte(0x00);
	ffs_write_byte(0x02);					//Must be 512 bytes (requried by this driver and SD V2.00 compatibility
	ffs_write_byte(0x00);
	ffs_write_byte(0xff);					//Dummy CRC


	if (ffs_check_command_response_byte(0xff, 0x00) == 0)		//Get response
		goto init_new_ffs_card_fail;

	//Set the number of bytes per sector before we move on to general access
	ffs_bytes_per_sector = 512;


	FFS_CE(1);							//De-select card
	ffs_write_byte(0xff);				//Send extra clock pulses in case card is still completing an operation
	FFS_CE(0); 						//Re-select card


	//---------------------------------------
	//----- READ THE MASTER BOOT RECORD -----
	//---------------------------------------
	// read sector 1 (LBA 0x00)
	ffs_read_sector_to_buffer(0);

	//Dump the first 446 bytes of boot up executable code
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];
	buffer_pointer += 446;

	//Now at start of the partition table [0x000001be]
	//Check for Partition 1 active (0x00 = inactive, 0x80 = active) [1]
	//(We allow a value of 0x00 for partition 1 as this partition must be present and on some disks a value of 0x00 has been found)
	//if (*buffer_pointer++ != 0x80)
	//	goto init_new_ffs_card_fail
	buffer_pointer++;

	//Get 'Beginning of Partition - Head' [0x000001bf]
	head = *buffer_pointer++;


	//Get 'Beginning of Partition - Cylinder + Sector' [0x000001c0]
	w_temp = (WORD)*buffer_pointer++;
	w_temp |= (WORD)(*buffer_pointer++) << 8;

	cylinder_no = (w_temp >> 8);				//Yes this is correct - strange bit layout in this register!
	if (w_temp & 0x0040)
		cylinder_no += 0x0100;
	if (w_temp & 0x0080)
		cylinder_no += 0x0200;

	sector = (BYTE)(w_temp & 0x003f);

	//Read the 'Type Of Partition' [0x000001c2]
	//(We accept FAT16 or FAT32)
	b_temp = *buffer_pointer++;

	if (b_temp == 0x04)						//FAT16 (smaller than 32MB)
		disk_is_fat_32 = 0;
	else if (b_temp == 0x06)				//FAT16 (larger than 32MB)
		disk_is_fat_32 = 0;
	else if (b_temp == 0x0b)				//FAT32 (Partition Up to 2048GB)
		disk_is_fat_32 = 1;
	else if (b_temp == 0x0c)				//FAT32 (Partition Up to 2048GB - uses 13h extensions)
		disk_is_fat_32 = 1;
	else if (b_temp == 0x0e)				//FAT16 (partition larger than 32MB, uses 13h extensions)
		disk_is_fat_32 = 0;
	else
		goto init_new_ffs_card_fail;

	//Get end of partition - head [0x000001c3]
	buffer_pointer++;

	//Get end of partition - Cylinder & Sector [0x000001c4]
	buffer_pointer++;
	buffer_pointer++;

	//----- GET START ADDRESS OF PARTITION 1 -----
	//Get no of sectors between MBR and the first sector in the partition [0x000001c6]
	main_partition_start_sector = (DWORD)*buffer_pointer++;
	main_partition_start_sector |= (DWORD)(*buffer_pointer++) << 8;
	main_partition_start_sector |= (DWORD)(*buffer_pointer++) << 16;
	main_partition_start_sector |= (DWORD)(*buffer_pointer++) << 24;
	//WE NOW HAVE THE START ADDRESS OF THE FIRST PARTITION (THE ONLY PARTITION WE LOOK AT)

	//Get no of sectors in the partition [0x000001ca]
	ffs_no_of_partition_sectors = (DWORD)*buffer_pointer++;
	ffs_no_of_partition_sectors |= (DWORD)(*buffer_pointer++) << 8;
	ffs_no_of_partition_sectors |= (DWORD)(*buffer_pointer++) << 16;
	ffs_no_of_partition_sectors |= (DWORD)(*buffer_pointer++) << 24;



	//------------------------------------------
	//---- READ THE PARTITION 1 BOOT RECORD ----
	//------------------------------------------
	//Setup for finding the FAT1 table start address root directory start address and data area start address
	lba = main_partition_start_sector;

	ffs_read_sector_to_buffer(lba);
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];

	//Dump jump code & OEM name (11 bytes)
	buffer_pointer += 11;

	//Get 'Bytes Per Sector' [# + 0x000b]
	//Check value is 512
	ffs_bytes_per_sector = (WORD)*buffer_pointer++;
	ffs_bytes_per_sector |= (WORD)(*buffer_pointer++) << 8;
	if (ffs_bytes_per_sector != 512)
		goto init_new_ffs_card_fail;

	//Get 'Sectors Per Cluster' [# + 0x000d]
	//(Restricted to powers of 2 (1, 2, 4, 8, 16, 32?))
	sectors_per_cluster = *buffer_pointer++;
	
	b_temp = 0;											//Check its power of 2 (other functions rely on this check)
	for (w_temp = 0x01; w_temp < 0x0100; w_temp <<= 1)
	{
		if (sectors_per_cluster & (BYTE)w_temp)
			b_temp++;
	}
	if (b_temp != 1)
		goto init_new_ffs_card_fail;

	//Get '# of reserved sectors' [# + 0x000e]
	//Adjust the start addresses acordingly
	number_of_reserved_sectors = (WORD)*buffer_pointer++;
	number_of_reserved_sectors |= (WORD)(*buffer_pointer++) << 8;

	//Get 'no of copies of FAT' [# + 0x0010]
	//(any number >= 1 is permitted, but no value other than 2 is recomended)
	number_of_copies_of_fat = *buffer_pointer++;
	if ((number_of_copies_of_fat > 4) || (number_of_copies_of_fat == 0))		//We set a limit on there being a maximum of 4 copies of fat
		goto init_new_ffs_card_fail;

	//Get 'max root directory entries' [# + 0x0011]
	//(Used by FAT16, but not for FAT32)
	dw_temp = (DWORD)*buffer_pointer++;
	dw_temp |= (DWORD)(*buffer_pointer++) << 8;
	number_of_root_directory_sectors = ((dw_temp * 32) + (DWORD)(ffs_bytes_per_sector - 1)) / ffs_bytes_per_sector;		//Multiply no of entries by 32 (no of bytes per entry)
																														//This calculation rounds up
	//Get 'number of sectors in partition < 32MB' [# + 0x0013]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//Get 'media descriptor' [# + 0x0015]
	//(Should be 0xF8 for hard disk)
	if (*buffer_pointer++ != 0xf8)
		goto init_new_ffs_card_fail;

	//Get 'sectors per fat'  [# + 0x0016]
	//(Used by FAT16, but not for FAT32 - for FAT32 the value is a double word and located later on in this table - variable will be overwritten)
	sectors_per_fat = (DWORD)*buffer_pointer++;
	sectors_per_fat |= (DWORD)(*buffer_pointer++) << 8;


	if(disk_is_fat_32 == 0)
	{
		//---------------------------------------------------------------------------------------
		//----- PARTITION IS FAT 16 - COMPLETE BOOT RECORD & INITAILISATION FOR THIS SYSTEM -----
		//---------------------------------------------------------------------------------------

		//CALCULATE THE PARTITION AREAS START ADDRESSES
		fat1_start_sector = main_partition_start_sector + (DWORD)number_of_reserved_sectors;
		root_directory_start_sector_cluster = main_partition_start_sector + (DWORD)number_of_reserved_sectors + (sectors_per_fat * number_of_copies_of_fat);
		data_area_start_sector = main_partition_start_sector + (DWORD)number_of_reserved_sectors + (sectors_per_fat * number_of_copies_of_fat) + number_of_root_directory_sectors;

		//SET THE ACTIVE FAT TABLE FLAGS
		active_fat_table_flags = 0;						// #|#|#|#|USE_FAT_TABLE_3|USE_FAT_TABLE_2|USE_FAT_TABLE_1|USE_FAT_TABLE_0
		for (b_temp = 0; b_temp < number_of_copies_of_fat; b_temp++)
		{
			active_fat_table_flags <<= 1;
			active_fat_table_flags++;
		}
		
		//SET UNUSED REGISTERS (used for FAT32)
		file_system_information_sector = 0xffff;

	}
	else
	{
		//---------------------------------------------------------------------------------------
		//----- PARTITION IS FAT 32 - COMPLETE BOOT RECORD & INITAILISATION FOR THIS SYSTEM -----
		//---------------------------------------------------------------------------------------

		//Dump sectors per track, # of heads, # of hidden sectors in partition (12 bytes)
		buffer_pointer += 12;

		//Get 'sectors per fat'  [# + 0x0024]
		sectors_per_fat = (DWORD)*buffer_pointer++;
		sectors_per_fat |= (DWORD)(*buffer_pointer++) << 8;
		sectors_per_fat |= (DWORD)(*buffer_pointer++) << 16;
		sectors_per_fat |= (DWORD)(*buffer_pointer++) << 24;

		//Get 'Flags' [# + 0x0028]
		//(Bits 0-4 Indicate Active FAT Copy)
		//(Bit 7 Indicates whether FAT Mirroring is Enabled or Disabled <Clear is Enabled>) (If FAT Mirroringis Disabled, the FAT Information is
		//only written to the copy indicated by bits 0-4)
		w_temp = (DWORD)*buffer_pointer++;
		w_temp |= (DWORD)(*buffer_pointer++) << 8;
		if (w_temp & 0x0080)
		{
			//BIT7 = 1, FAT MIRRORING IS DISABLED
			//Bits 3:0 set which FAT table is active
			if ((w_temp & 0x000f) > number_of_copies_of_fat)
				goto init_new_ffs_card_fail;
			
			switch (w_temp & 0x000f)
			{
			case 0:
				active_fat_table_flags = 0x01;						// #|#|#|#|USE_FAT_TABLE_3|USE_FAT_TABLE_2|USE_FAT_TABLE_1|USE_FAT_TABLE_0
				break;
			case 1:
				active_fat_table_flags = 0x02;
				break;
			case 2:
				active_fat_table_flags = 0x04;
				break;
			case 3:
				active_fat_table_flags = 0x08;
				break;
			}
		}
		else
		{
			//BIT7 = 0, FAT MIRRORING IS ENABLED INTO ALL FATS
			active_fat_table_flags = 0;						// #|#|#|#|USE_FAT_TABLE_3|USE_FAT_TABLE_2|USE_FAT_TABLE_1|USE_FAT_TABLE_0
			for (b_temp = 0; b_temp < number_of_copies_of_fat; b_temp++)
			{
				active_fat_table_flags <<= 1;
				active_fat_table_flags++;
			}
		}

		//Get 'Version of FAT32 Drive' [# + 0x002A]
		//(High Byte = Major Version, Low Byte = Minor Version)
		buffer_pointer++;
		buffer_pointer++;

		//Get 'Cluster Number of the Start of the Root Directory' [# + 0x2C]
		//(Usually 2, but not requried to be 2)
		root_directory_start_sector_cluster = (DWORD)*buffer_pointer++;
		root_directory_start_sector_cluster |= (DWORD)(*buffer_pointer++) << 8;
		root_directory_start_sector_cluster |= (DWORD)(*buffer_pointer++) << 16;
		root_directory_start_sector_cluster |= (DWORD)(*buffer_pointer++) << 24;

		//Get 'Sector Number of the File System Information Sector' [# + 0x0030]
		//(Referenced from the Start of the Partition. Usually 1, but not requried to be 1)
		file_system_information_sector = (DWORD)*buffer_pointer++;
		file_system_information_sector |= (DWORD)(*buffer_pointer++) << 8;

		//CALCULATE THE PARTITION AREAS START ADDRESSES
		fat1_start_sector = main_partition_start_sector + (DWORD)number_of_reserved_sectors;			//THE FAT START ADDRESS IS NOW GOOD (has correct offset)
		//root_directory_start_sector_cluster already done above
		data_area_start_sector = main_partition_start_sector + (DWORD)number_of_reserved_sectors + (sectors_per_fat * number_of_copies_of_fat);

		//SET UNUSED REGISTERS (used for FAT16)
		number_of_root_directory_sectors = 0;
	}

	//------------------------------------------------------------------------
	//----- BOOT RECORD IS DONE - ALL REQUIRED DISK PARAMETERS ARE KNOWN -----
	//------------------------------------------------------------------------


	//---------------------------------------------------
	//----- READ SD WRITE PROTECT SWITCH IF PRESENT -----
	//---------------------------------------------------
#ifdef FFS_WP_PIN_REGISTER					//Ignore if we're not using a write protect input
	//If the input is via a latch IC then read it now
	#ifdef	FFS_WP_PIN_FUNCTION
		FFS_WP_PIN_FUNCTION();
	#endif

	#ifdef FFS_WP_PIN_NC
		//IF WP pin is high then write protect is active
		if (FFS_WP_PIN_REGISTER & FFS_WP_PIN_BIT)
			ffs_card_write_protected = 1;
		else
			ffs_card_write_protected = 0;
	#else
		//IF WP pin is low then write protect is active
		if (FFS_WP_PIN_REGISTER & FFS_WP_PIN_BIT)
			ffs_card_write_protected = 0;
		else
			ffs_card_write_protected = 1;
	#endif

#else
	ffs_card_write_protected = 0;
#endif




	//-----------------------------
	//----- CARD IS OK TO USE -----
	//-----------------------------
	FFS_CE(1);						//Deselect the card
	ffs_card_ok = 1;				//Flag that the card is OK

	//Do Driver specific initialisations
	last_found_free_cluster = 0;		//When we next look for a free cluster, start from the beginning


	return;

//----------------------------------
//----- CARD IS NOT COMPATIBLE -----
//----------------------------------
init_new_ffs_card_fail:
	FFS_CE(1);							//De-select the card
	ffs_card_ok = 0;					//Flag that the card is not OK
	return;

}









//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//****************************** DRIVER SUB FUNCTIONS BELOW HERE ******************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************








//***************************************
//***************************************
//********** IS CARD PRESENT ? **********
//***************************************
//***************************************
//Returns
//	1 if present, 0 if not
BYTE ffs_is_card_present (void)
{
	return FFS_CD_PIN_FUNCTION;
}



//*******************************************
//*******************************************
//********** READ SECTOR TO BUFFER **********
//*******************************************
//*******************************************
//lba = start sector address
//The card must be deselected after all of the data has been read using:
//	FFS_CE(1);					//Deselect the card
void ffs_read_sector_to_buffer (DWORD sector_lba)
{
	WORD count;
	BYTE *buffer_pointer;
	BYTE read_successful = 0;

	//----- IF LBA MATCHES THE LAST LBA DON'T BOTHER RE-READING AS THE DATA IS STILL IN THE BUFFER -----
	if (ffs_buffer_contains_lba == sector_lba)
	{
		return;
	}

	FFS_CE(0);									//Select the card

	//If card DO (processor DI) is low then card is busy from previous write operation - wait
	DO_BUSY_STATE_ACCESS_DELAY;
	ffs_read_byte();
	while (FFC_DI == 0)
		ffs_read_byte();			//A clock is requried for the busy signal (DO held low) to be cleared

	//----- IF THE BUFFER CONTAINS DATA THAT IS WAITING TO BE WRITTEN THEN WRITE IT FIRST -----
	if (ffs_buffer_needs_writing_to_card)
	{
		if (ffs_buffer_contains_lba != 0xffffffff)			//This should not be possible but check is made just in case!
			ffs_write_sector_from_buffer(ffs_buffer_contains_lba);

		FFS_CE(0);											//Select the card again

		ffs_buffer_needs_writing_to_card = 0;
	}

	ffs_10ms_timer = 20;									//Set operation timeout just in case
	while ((read_successful == 0) && (ffs_10ms_timer))		//Retry until the read is successful
	{
		FFS_CE(0);
		DO_BUSY_STATE_ACCESS_DELAY;

		//If card DO (processor DI) is low then card is busy from previous write operation - wait
		DO_BUSY_STATE_ACCESS_DELAY;
		ffs_read_byte();
		while (FFC_DI == 0)
			ffs_read_byte();			//A clock is requried for the busy signal (DO held low) to be cleared


		//----- NEW BLOCK TO BE LOADED -----
		//Send command 17 - Read Single Block
		ffs_write_byte(0x40 + 17);						//Command
		if (card_is_high_capacity)
		{
			//High capacity sector addressing
			ffs_write_byte((BYTE)((sector_lba & 0xff000000) >> 24));		//Data
			ffs_write_byte((BYTE)((sector_lba & 0x00ff0000) >> 16));
			ffs_write_byte((BYTE)((sector_lba & 0x0000ff00) >> 8));
			ffs_write_byte((BYTE)(sector_lba & 0x000000ff));
		}
		else
		{
			//Standard byte addressing
			ffs_write_byte((BYTE)((sector_lba & 0x007f8000) >> 15));		//Data
			ffs_write_byte((BYTE)((sector_lba & 0x00007f80) >> 7));
			ffs_write_byte((BYTE)((sector_lba & 0x0000007f) << 1));
			ffs_write_byte(0x00);
		}
		ffs_write_byte(0xff);							//Dummy CRC

		//Get command response
		if (ffs_check_command_response_byte(0xff, 0x00) == 0)		//R1 response: | 0 | ParameterError | AddressError | Erase Sequence Error | Com CRC Error | Illegal Command | Erase Reset | In Idle State |
			goto ffs_read_sector_to_buffer_exit;			//Error - shouldn't happen

		//Wait for data token
		for (count = 1000; count > 0; count--)			//Repeate many times as read data is allowed to take a lot of cycles to be ready (the exact number is dependant on clock speed so we just use a generic over the top timeout)
		{
			if (ffs_check_command_response_byte(0xff, 0xfe))
				break;
				
			if (count == 1)
				goto ffs_read_sector_to_buffer_exit;		//Error - shouldn't happen
		}

		read_successful = 1;


		//We have to read the entire data block for an SD or MMC card and these cards also have a timeout for commands
		//so we get the entire block now to avoid possible timeout problems.

		buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];
		
		for (count = 0; count < ffs_bytes_per_sector; count++)
		{
			*buffer_pointer++ = ffs_read_byte();
		}

		ffs_buffer_contains_lba = sector_lba;				//Flag that the data buffer currently contains data for this LBA (logged to avoid re-loading the buffer again if its not necessary)

		//Get the CRC
		ffs_read_byte();
		ffs_read_byte();



ffs_read_sector_to_buffer_exit:
		FFS_CE(1);											//De - select the card

		//Give 8 clocks for the card to complete the operation
		ffs_write_byte(0xff);

	}  //while ((read_successful == 0) && (ffs_10ms_timer))		//Retry until the read is successful

}






//**********************************************
//**********************************************
//********** WRITE SECTOR FROM BUFFER **********
//**********************************************
//**********************************************
void ffs_write_sector_from_buffer (DWORD sector_lba)
{
	WORD count;
	BYTE *buffer_pointer;
	BYTE write_successful = 0;
	
	

	ffs_buffer_needs_writing_to_card = 0;			//Flag that buffer is no longer waiting to write to card (must be at top as this function
													//calls other functions that check this flag and would call the function back)

	ffs_10ms_timer = 20;									//Set operation timeout just in case
	while ((write_successful == 0) && (ffs_10ms_timer))		//Retry until the write is successful
	{

		//----- SETUP TO WRITE THE SECTOR -----
		FFS_CE(0);										//Select the card

		//If card DO (processor DI) is low then card is busy from previous write operation - wait
		DO_BUSY_STATE_ACCESS_DELAY;
		ffs_read_byte();
		while (FFC_DI == 0)
			ffs_read_byte();			//A clock is requried for the busy signal (DO held low) to be cleared


		if (ffs_card_write_protected == 0)					//Don't write if the card is write protected
		{
			//Send command 24 - Write Block
			ffs_write_byte(0x40 + 24);						//Command
			if (card_is_high_capacity)
			{
				//High capacity sector addressing
				ffs_write_byte((BYTE)((sector_lba & 0xff000000) >> 24));		//Data
				ffs_write_byte((BYTE)((sector_lba & 0x00ff0000) >> 16));
				ffs_write_byte((BYTE)((sector_lba & 0x0000ff00) >> 8));
				ffs_write_byte((BYTE)(sector_lba & 0x000000ff));
			}
			else
			{
				//Standard byte addressing
				ffs_write_byte((BYTE)((sector_lba & 0x007f8000) >> 15));		//Data
				ffs_write_byte((BYTE)((sector_lba & 0x00007f80) >> 7));
				ffs_write_byte((BYTE)((sector_lba & 0x0000007f) << 1));
				ffs_write_byte(0x00);
			}
			ffs_write_byte(0xff);							//Dummy CRC

			//Get command response
			if (ffs_check_command_response_byte(0xff, 0x00) == 0)		//R1 response: | 0 | ParameterError | AddressError | Erase Sequence Error | Com CRC Error | Illegal Command | Erase Reset | In Idle State |
				goto ffs_write_sector_from_buffer_exit;

			//Send the data token
			ffs_write_byte(0xfe);


			//----- WRITE THE BUFFER TO THE CARD SECTOR -----
			buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];
			for (count = 0; count < ffs_bytes_per_sector; count++)
			{
				ffs_write_byte(*buffer_pointer++);
			}
		
			//Send dummy CRC
			ffs_write_byte(0xff);
			ffs_write_byte(0xff);
		
			//Get command response
			if (ffs_check_command_response_byte(0x0f, 0x05) == 0)		//Data response token. xxx0SSS1.  SSS=010=data accepted, 101=data rejected due to a CRC error, 110=data rejected due to a write error
				goto ffs_write_sector_from_buffer_exit;
			else
				write_successful = 1;
		}


ffs_write_sector_from_buffer_exit:
		FFS_CE(1);										//Deselect the card

		if (write_successful == 0)
		{
			//Write failed so instert a little pause before retrying
			count = 1000;
			while (count)
				count--;
		}

		//Give 8 clocks for the card to complete the operation
		ffs_write_byte(0xff);							//Don't do this - causes errors

	}  //while ((write_successful == 0) && (ffs_10ms_timer))	
}









//****************************************
//****************************************
//********** WRITE BYTE TO CARD **********
//****************************************
//****************************************
BYTE ffs_write_byte (BYTE data)
{
	if(speed == 1) {
		SPI_BUS_SET_TO_FULL_SPEED_SD;
	} else {
		SPI_BUS_SET_TO_LOW_SPEED;
	}

	BYTE data_rx;

	//Send byte
	FFS_SPI_TX_BYTE(data);

	//Wait for tx to complete
	while (!FFS_SPI_BUF_FULL)
		;
	
	//Read the received byte (some SPI peripherals require this)
	data_rx = FFS_SPI_RX_BYTE_BUFFER;

	FFS_SPI_CLEAR_FLAG;
	
	return (1);

}






//*****************************************
//*****************************************
//********** READ WORD FROM CARD **********
//*****************************************
//*****************************************
WORD ffs_read_word (void)
{
	WORD data;

	data = (WORD)ffs_read_byte();
	data += ((WORD)ffs_read_byte() << 8);

	return (data);
}






//*****************************************
//*****************************************
//********** READ BYTE FROM CARD **********
//*****************************************
//*****************************************
BYTE ffs_read_byte (void)
{
	if(speed == 1) {
		SPI_BUS_SET_TO_FULL_SPEED_SD;
	} else {
		SPI_BUS_SET_TO_LOW_SPEED;
	}

	BYTE rxData;

	//Send dummy byte
	FFS_SPI_TX_BYTE(0xff);
	
	//Wait for tx to complete
	while (!FFS_SPI_BUF_FULL);

	rxData = FFS_SPI_RX_BYTE_BUFFER;

	FFS_SPI_CLEAR_FLAG;
	
	return rxData;
}


//******************************************************
//******************************************************
//********** CHECK COMMAND RESPONSE FROM CARD **********
//******************************************************
//******************************************************
BYTE ffs_check_command_response_byte (BYTE mask, BYTE data_requried)
{
	if(speed == 1) {
		SPI_BUS_SET_TO_FULL_SPEED_SD;
	} else {
		SPI_BUS_SET_TO_LOW_SPEED;
	}

	BYTE count;

	//Response can take up to 9 bytes to be returned
	for (count = 0; count < 9; count++)
	{
		//Send dummy byte
		FFS_SPI_TX_BYTE(0xff);
	
		//Wait for tx to complete
		while (!FFS_SPI_BUF_FULL)
			;

		chk_cmd_response_data = FFS_SPI_RX_BYTE_BUFFER;

		FFS_SPI_CLEAR_FLAG;
		
		if ((chk_cmd_response_data & mask) == (data_requried & mask))
			return(1);
	}
	
	//Valid response was not received
	return(0);

}






