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
//FAT FILING SYSTEM DRIVER C CODE FILE




#define FFS_C
#include "board.h"
#include "mem-ffs.h"
#include "mem-mmcsd.h"


//*******************************
//*******************************
//********** OPEN FILE **********
//*******************************
//*******************************
//For ease of use this driver does not differentiate between text and binary mode.  You may open a file in either mode (or
//neither) and all file operations will be exactly the same (basically is if the file was opened in binary mode.  LF characters
//will not be converted to a pair CRLF characters and vice versa.  This makes using functions like fseek much simpler and avoids
//operating system difference issues.  (If you are not aware there is no difference between a binary file and a text file - the
//difference is in how the operating system chooses to handle text files)
//
//filename
//	Only 8 character DOS compatible root directory filenames are allowed.  Format is F.E where F may be between 1 and 8 characters
//	and E may be between 1 and 3 characters, null terminated, non-case sensitive.  The '*' and '?' wildcard characters may be used.
//
//access_mode
//	"r"		Open a file for reading. The file must exist.
//	"r+"	Open a file for reading and writing. The file must exist.
//	"w"		Create an empty file for writing. If a file with the same name already exists its content is erased.
//	"w+"	Create an empty file for writing and reading. If a file with the same name already exists its content is erased before it is opened.
//	"a"		Append to a file. Writing operations append data at the end of the file. The file is created if it doesn't exist.
//	"a+"	Open a file for reading and appending. All writing operations are done at the end of the file protecting the previous
//			content from being overwritten.  You can reposition (fseek) the pointer to anywhere in the file for reading, but
//			writing operations will move back to the end of file.  The file is created if it doesn't exist.
//
//Return value.
//	If the file has been successfully opened the function will return a pointer to the file. Otherwise a null pointer is returned (0x00).

FFS_FILE* ffs_fopen (const char *filename, const char *access_mode)
{
	BYTE file_number;
	BYTE attribute_byte;
	BYTE count;
	BYTE read_file_name[8];
	BYTE read_file_extension[3];

	//----------------------------------------------
	//----- LOOK FOR AN AVAILABLE FILE HANDLER -----
	//----------------------------------------------
	for (file_number = 0; file_number < FFS_FOPEN_MAX; file_number++)
	{
		if (ffs_file[file_number].flags.bits.file_is_open == 0)
		{
			//THIS HANDLER IS AVAILABLE
			break;
		}
		if (file_number == (FFS_FOPEN_MAX - 1))
		{
			//LAST HANDLER TRIED - THE MAXIMUM NUMBER OF FILES ARE ALREADY OPEN
			return(0);
		}
	}

	//----- AVAILABLE FILE HANDLER FOUND BUT NOT YET ASSIGNED -----
	//(file_number = the available handler)


	//----------------------------------------------
	//----- LOOK TO SEE IF FILE ALREADY EXISTS -----
	//----------------------------------------------
	ffs_file[file_number].current_cluster = ffs_find_file (filename, &ffs_file[file_number].file_size, &attribute_byte, &ffs_file[file_number].directory_entry_sector, &ffs_file[file_number].directory_entry_within_sector, &read_file_name[0], &read_file_extension[0]);


	//--------------------------------------------------------
	//----- IF FILE EXISTS ENSURE IT IS NOT ALREADY OPEN -----
	//--------------------------------------------------------
	if (ffs_file[file_number].current_cluster != 0xffffffff)		//0xffffffff = file not found
	{
		for (count = 0; count < FFS_FOPEN_MAX; count++)
		{
			if (ffs_file[count].flags.bits.file_is_open)
			{
				if (
					(count != file_number) &&
					(ffs_file[count].directory_entry_sector == ffs_file[file_number].directory_entry_sector) &&
					(ffs_file[count].directory_entry_within_sector == ffs_file[file_number].directory_entry_within_sector)
					)
				{
					return(0);
				}
			}
		}
	}

	//-------------------------------------------------------------
	//----- IF OPENING A FILE FOR READ THEN CHECK FILE EXISTS -----
	//-------------------------------------------------------------
	if (ffs_file[file_number].current_cluster == 0xffffffff)		//0xffffffff = file not found
	{
		if (*access_mode == 'r')
		{
			//ERROR - CAN'T OPEN FILE FOR READ AS IT DOESN'T EXIST
			return(0);
		}
	}

	//-----------------------------------------------------------------
	//----- IF WRITING A FILE AND IT ALREADY EXISTS THEN ERASE IT -----
	//-----------------------------------------------------------------
	if (ffs_file[file_number].current_cluster != 0xffffffff)		//0xffffffff = file not found
	{
		if (*access_mode == 'w')
		{
			//ERASE FILE
			ffs_remove(filename);
			ffs_file[file_number].current_cluster = 0xffffffff;
		}
		else
		{
			//FILE EXISTS AND IS NOT BEEING DELETED FIRST - MODIFY THE LAST ACCESSED TIME AND DATE IF USING A RTC
			#ifdef USE_REAL_TIME_CLOCK
				ffs_overwrite_last_directory_entry (&read_file_name[0], &read_file_extension[0], &attribute_byte, &ffs_file[file_number].file_size, &ffs_file[file_number].current_cluster, 0x02);
			#endif
		}
	}

	//----------------------------------------------------------------------
	//----- IF WRITING OR APPENDING A FILE THEN CREATE IT IF NECESSARY -----
	//----------------------------------------------------------------------
	if (ffs_file[file_number].current_cluster == 0xffffffff)
	{
		if (ffs_create_new_file(filename, &ffs_file[file_number].current_cluster, &ffs_file[file_number].directory_entry_sector,
								&ffs_file[file_number].directory_entry_within_sector) == 0)		//This function checks for any wildcard characters in the filename and aborts if there are any
		{
			//ERROR - CAN'T CREATE A NEW FILE
			return(0);
		}
		ffs_file[file_number].file_size = 0;
	}


	//------------------------------------------------------------
	//----- FLAG THAT FILE IS OPEN AND SETUP FOR FILE ACCESS -----
	//------------------------------------------------------------
	ffs_file[file_number].flags.bits.file_is_open = 1;
	ffs_file[file_number].flags.bits.access_error = 0;
	ffs_file[file_number].flags.bits.end_of_file = 0;
	ffs_file[file_number].flags.bits.file_size_has_changed = 0;


	//--------------------------------------------------
	//----- SET THE ACCESS MODE FLAGS FOR THE FILE -----
	//--------------------------------------------------
	if (*access_mode == 'r')
	{
		if (
			(*(access_mode + 1) == '+') ||
			((*(access_mode + 1) != 0x00) && (*(access_mode + 2) == '+'))
			)
		{
			//----- ACCESS MODE IS 'R+' -----
			ffs_file[file_number].flags.bits.read_permitted = 1;
			ffs_file[file_number].flags.bits.write_permitted = 1;
			ffs_file[file_number].flags.bits.write_append_only = 0;
		}
		else
		{
			//----- ACCESS MODE IS 'R' -----
			ffs_file[file_number].flags.bits.read_permitted = 1;
			ffs_file[file_number].flags.bits.write_permitted = 0;
			ffs_file[file_number].flags.bits.write_append_only = 0;
		}
		//Set the current location to the beginning of the file
		ffs_file[file_number].current_sector = 0;
		ffs_file[file_number].current_byte = 0;
		ffs_file[file_number].current_byte_within_file = 0;
		ffs_file[file_number].flags.bits.inc_posn_before_next_rw = 0;			//No increment before next read or write as we're already pointing to the next byte to access
	}
	else if (*access_mode == 'w')
	{
		if (
			(*(access_mode + 1) == '+') ||
			((*(access_mode + 1) != 0x00) && (*(access_mode + 2) == '+'))
			)
		{
			//----- ACCESS MODE IS 'W+' -----
			ffs_file[file_number].flags.bits.read_permitted = 1;
			ffs_file[file_number].flags.bits.write_permitted = 1;
			ffs_file[file_number].flags.bits.write_append_only = 0;
		}
		else
		{
			//----- ACCESS MODE IS 'W' -----
			ffs_file[file_number].flags.bits.read_permitted = 0;
			ffs_file[file_number].flags.bits.write_permitted = 1;
			ffs_file[file_number].flags.bits.write_append_only = 0;
		}
		//Set the current location to the beginning of the file
		ffs_file[file_number].current_sector = 0;
		ffs_file[file_number].current_byte = 0;
		ffs_file[file_number].current_byte_within_file = 0;
		ffs_file[file_number].flags.bits.inc_posn_before_next_rw = 0;			//No increment before next read or write as we're already pointing to the next byte to access
	}
	else if (*access_mode == 'a')
	{
		if (
			(*(access_mode + 1) == '+') ||
			((*(access_mode + 1) != 0x00) && (*(access_mode + 2) == '+'))
			)
		{
			//----- ACCESS MODE IS 'A+' -----
			ffs_file[file_number].flags.bits.read_permitted = 1;
			ffs_file[file_number].flags.bits.write_permitted = 1;
			ffs_file[file_number].flags.bits.write_append_only = 1;
		}
		else
		{
			//----- ACCESS MODE IS 'A' -----
			ffs_file[file_number].flags.bits.read_permitted = 0;
			ffs_file[file_number].flags.bits.write_permitted = 1;
			ffs_file[file_number].flags.bits.write_append_only = 1;
		}

		//Set the current location to the last byte of the file
		ffs_file[file_number].current_sector = 0;							//These need setting to file start before calling ffs_fseek
		ffs_file[file_number].current_byte = 0;
		ffs_file[file_number].current_byte_within_file = 0;
		ffs_file[file_number].flags.bits.inc_posn_before_next_rw = 0;

		ffs_fseek(&ffs_file[file_number], 0, FFS_SEEK_END);					//Use the fseek function to do this

		if (ffs_file[file_number].file_size)
		{
			ffs_file[file_number].flags.bits.inc_posn_before_next_rw = 1;		//Increment before doing the next read or write as we're pointing to the last byte of the file and the next byte will be a new byte
		}

	}

	return(&ffs_file[file_number]);
}




//************************************************
//************************************************
//********** MOVE THE FILE BYTE POINTER **********
//************************************************
//************************************************
//This function is quite complex as it looks to see if the new location is in the same cluster as the current location to avoid having
//to read all of the FAT table entries for the file from the file start where possible.  This results in a large speed improvement.
//
//origin
//	The initial position from where the offset is applied
//	FFS_SEEK_SET (0) Beginning of file
//	FFS_SEEK_CUR (1) Current position of the file pointer
//	FFS_SEEK_END (2) End of file
//offset
//	Signed offset from the position set by origin
//returns
//	0 if successful, 1 otherwise
int ffs_fseek (FFS_FILE *file_pointer, long offset, int origin)
{
	DWORD dw_temp;
	DWORD bytes_to_new_posn;
	DWORD bytes_per_cluster;
	BYTE calculate_new_posn;


	bytes_per_cluster = sectors_per_cluster * ffs_bytes_per_sector;


	//-----------------------------------
	//----- ENSURE THE FILE IS OPEN -----
	//-----------------------------------
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);


	if (origin == FFS_SEEK_SET)
	{
		//---------------------------------------
		//----- ORIGIN IS BEGINNING OF FILE -----
		//---------------------------------------
		if (offset < 0)
		{
			//----- NEGATIVE VALUE - ILLEGAL -----
			return(1);
		}
		else if (offset == 0)
		{
			//----- ZERO VALUE - SET TO START OF FILE -----
			bytes_to_new_posn = 0;
		}
		else
		{
			//----- POSITIVE VALUE -----
			bytes_to_new_posn = (DWORD)offset;				//Set bytes from start value ready to move to requried location
		}
	}
	else if (origin == FFS_SEEK_CUR)
	{
		//----------------------------------------------
		//----- ORIGIN IS CURRENT POSITION IN FILE -----
		//----------------------------------------------
		bytes_to_new_posn = file_pointer->current_byte_within_file;

		if (file_pointer->flags.bits.inc_posn_before_next_rw)
			bytes_to_new_posn++;

		if (offset < 0)
		{
			//OFFSET IS NEGATIVE
			bytes_to_new_posn -= (DWORD)(0 - offset);
		}
		else
		{
			//OFFSET IS POSITIVE
			bytes_to_new_posn += (DWORD)offset;
		}

	}
	else if (origin == FFS_SEEK_END)
	{
		//---------------------------------
		//----- ORIGIN IS END OF FILE -----
		//---------------------------------
		if (offset > 1)
		{
			//----- POSITIVE VALUE > 1 ILLEGAL -----
			return(1);
		}
		else if (offset == 1)
		{
			bytes_to_new_posn = file_pointer->file_size;
		}
		else
		{
			bytes_to_new_posn = (file_pointer->file_size) - (DWORD)(0 - offset) - 1;			//Set bytes from start value ready to move to requried location
		}
	}
	else
	{
		//----- INVALID ORIGIN VALUE -----
		return(1);
	}




	//------------------------------------------------
	//----- ORIGIN IS NOW FROM BEGINNING OF FILE -----
	//------------------------------------------------

	//----- CHECK OFFSET IS VALID -----
	if (bytes_to_new_posn > (file_pointer->file_size))
	{
		//NEW POSITION IS > FILE SIZE + 1 = ERROR
		return(1);
	}
	else if (bytes_to_new_posn == (file_pointer->file_size))
	{
		//NEW POSITION = FILE SIZE SO IS THE POSITION READY FOR WRITING A NEW BYTE (DOESN'T ACTUALLY EXIST IN FILE YET)
		if (bytes_to_new_posn)
		{
			bytes_to_new_posn--;
			file_pointer->flags.bits.inc_posn_before_next_rw = 1;
		}
		else
		{
			file_pointer->flags.bits.inc_posn_before_next_rw = 0;
		}
	}
	else
	{
		//NEW POSITION IS WITHIN FILE
		file_pointer->flags.bits.inc_posn_before_next_rw = 0;
	}


	if (bytes_to_new_posn == 0)
	{
		//-------------------------------------------------
		//----- OFFSET IS ZERO - SET TO START OF FILE -----
		//-------------------------------------------------
		file_pointer->current_cluster = get_file_start_cluster(file_pointer);
		file_pointer->current_sector = 0;
		file_pointer->current_byte = 0;
		file_pointer->current_byte_within_file = 0;

	}
	else
	{
		//----- NON-ZERO VALUE -----

		//--------------------------------------------------------------------------------------------------------------------------------------------
		//----- IF NEW LOCATION IS WITHIN CURRENT CLUSTER THEN DO ADJUSTMENT AVOIDING HAVING TO READ EVERY FAT CLUSTER ENTRY FROM THE FILE START -----
		//--------------------------------------------------------------------------------------------------------------------------------------------

		if (bytes_to_new_posn == file_pointer->current_byte_within_file)
		{
			//----- NEW POSITION IS SAME AS EXISTING POSITION -----
			calculate_new_posn = 0;
		}
		else if (bytes_to_new_posn < file_pointer->current_byte_within_file)
		{
			//----- NEW POSITION IS BEFORE CURRENT POSITION -----
			if (
				(file_pointer->current_byte_within_file - bytes_to_new_posn) <
				(file_pointer->current_byte + (file_pointer->current_sector * ffs_bytes_per_sector))
				)
			{
				//IN SAME CLUSTER

				calculate_new_posn = 0;
				//file_pointer->current_cluster = file_pointer->current_cluster;			//Stays the same
				
				dw_temp = bytes_to_new_posn;
				bytes_to_new_posn = (file_pointer->current_byte_within_file - bytes_to_new_posn);
				file_pointer->current_byte_within_file = dw_temp;

				while (bytes_to_new_posn >= ffs_bytes_per_sector)
				{
					bytes_to_new_posn -= ffs_bytes_per_sector;
					if (file_pointer->current_sector)
						file_pointer->current_sector--;
					else
						calculate_new_posn = 1;				//Error!
				}


				file_pointer->current_byte -= (WORD)bytes_to_new_posn;
				if (file_pointer->current_byte >= ffs_bytes_per_sector)		//Check for overflow into previous sector
				{
					file_pointer->current_byte += ffs_bytes_per_sector;		//(Yes the '+' is correct!)
					if (file_pointer->current_sector)
						file_pointer->current_sector--;
					else
						calculate_new_posn = 1;				//Error!
				}
			}
			else
			{
				//IN DIFFERENT CLUSTER
				calculate_new_posn = 1;
			}
		}
		else
		{
			//----- NEW POSITION IS AFTER CURRENT POSITION -----
			if (
				(bytes_to_new_posn - file_pointer->current_byte_within_file) <
				(bytes_per_cluster - (file_pointer->current_byte + (file_pointer->current_sector * ffs_bytes_per_sector)))
				)
			{
				//IN SAME CLUSTER

				calculate_new_posn = 0;
				//file_pointer->current_cluster = file_pointer->current_cluster;			//Stays the same

				dw_temp = bytes_to_new_posn;
				bytes_to_new_posn = (bytes_to_new_posn - file_pointer->current_byte_within_file);
				file_pointer->current_byte_within_file = dw_temp;

				while (bytes_to_new_posn >= ffs_bytes_per_sector)
				{
					bytes_to_new_posn -= ffs_bytes_per_sector;
					if (file_pointer->current_sector < sectors_per_cluster)
						file_pointer->current_sector++;
					else
						calculate_new_posn = 1;				//Error!
				}

				file_pointer->current_byte += (WORD)bytes_to_new_posn;
				if (file_pointer->current_byte >= ffs_bytes_per_sector)
				{
					file_pointer->current_byte -= ffs_bytes_per_sector;
					if (file_pointer->current_sector < sectors_per_cluster)
						file_pointer->current_sector++;
					else
						calculate_new_posn = 1;				//Error!
				}
			}
			else
			{
				//IN DIFFERENT CLUSTER
				calculate_new_posn = 1;
			}
		}
		


		if (calculate_new_posn)
		{
			//-------------------------------------------------------------------------------------------
			//----- NEW LOCATION IS NOT IN THE CURRENT CLUSTER - FIND IT FROM THE START OF THE FILE -----
			//-------------------------------------------------------------------------------------------
			file_pointer->current_cluster = get_file_start_cluster(file_pointer);
			file_pointer->current_sector = 0;
			file_pointer->current_byte_within_file = bytes_to_new_posn;
	
			//KEEP MOVING TO NEXT CLUSTER UNTIL WE'RE IN THE REQURIED CLUSTER
			while (bytes_to_new_posn >= bytes_per_cluster)
			{
				bytes_to_new_posn -= bytes_per_cluster;
				file_pointer->current_cluster = ffs_get_next_cluster_no(file_pointer->current_cluster);
			}
			//KEEP MOVING TO NEXT SECTOR UNTIL WE'RE IN THE REQURIED SECTOR (OF THE ALREADY FOUND CLUSTER)
	
			while (bytes_to_new_posn >= ffs_bytes_per_sector)
			{
				bytes_to_new_posn -= ffs_bytes_per_sector;
				file_pointer->current_sector++;
			}
			//STORE THE CURRENT BYTE NUMBER
			file_pointer->current_byte = (WORD)bytes_to_new_posn;
		}
	}

	file_pointer->flags.bits.end_of_file = 0;			//The EOF flag is cleared when this function sucessfully is called
	return(0);


}



//************************************************
//************************************************
//********** MOVE THE FILE BYTE POINTER **********
//************************************************
//************************************************
//(The value returned by ffs_getpos intended to be file system specific and to be used with this function.  However as the position type is recomended
//to be a long we just use the ffs_ftell function as a long doesn't give us enough space to store everything we need for the low level position)
//Returns
//	0 if successful, 1 otherwise
int ffs_fsetpos (FFS_FILE *file_pointer, long *position)
{
	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);

	file_pointer->flags.bits.end_of_file = 0;			//Clear the end of file flag

	return(ffs_fseek(file_pointer, *position, FFS_SEEK_SET));
}



//******************************************************
//******************************************************
//********** GET THE CURRENT POSITION IN FILE **********
//******************************************************
//******************************************************
//Get the current file position.
long ffs_ftell (FFS_FILE *file_pointer)
{

	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);

	//----- RETURN THE CURRENT POSITION -----
	if (file_pointer->flags.bits.inc_posn_before_next_rw)
		return((long)file_pointer->current_byte_within_file + 1);
	else
		return((long)file_pointer->current_byte_within_file);
}



//******************************************************
//******************************************************
//********** GET THE CURRENT POSITION IN FILE **********
//******************************************************
//******************************************************
//(The value returned is intended to be file system specific and only to be used with fsetpos.  However as the position type is recomended
//to be a long we just use the ffs_ftell function as a long doesn't give us enough space to store everything we need for the low level position)
//Returns
//	0 if successful, 1 otherwise
int ffs_fgetpos (FFS_FILE *file_pointer, long *position)
{


	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);

	//----- RETURN THE CURRENT POSITION -----
	*position = ffs_ftell(file_pointer);
	return(0);

}




//***********************************************************************
//***********************************************************************
//********** SET POSITION TO BEGINNING OF FILE AND CLEAR FLAGS **********
//***********************************************************************
//***********************************************************************
void ffs_rewind (FFS_FILE *file_pointer)
{

	ffs_fseek (file_pointer, 0, FFS_SEEK_SET);

	file_pointer->flags.bits.access_error = 0;
}





//****************************************
//****************************************
//********** WRITE BYTE TO FILE **********
//****************************************
//****************************************
//Data is converted to a byte before writing (int type is specified by ANSI-C)
//If there are no errors the written character is returned. If an error occurs, EOF is returned.

//#define	ffs_putc(data, file_pointer)	ffs_fputc(data, file_pointer)			//Defined in header file - these 2 functions are equivalent

int ffs_fputc (int data, FFS_FILE *file_pointer)
{
	DWORD dw_temp;
	BYTE *buffer_pointer;


	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(FFS_EOF);

	//-------------------------------------------------
	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	//-------------------------------------------------
	if (file_pointer->flags.bits.file_is_open == 0)
		return(FFS_EOF);


	//---------------------------------------------------------------------------------------------------------------------------
	//----- CHECK THAT WRITING IN THIS POSITION IS PERMITTED FOR THE FOPEN MODE THAT WAS SPECIFIED WHEN THE FILE WAS OPENED -----
	//---------------------------------------------------------------------------------------------------------------------------
	if (file_pointer->flags.bits.write_permitted == 0)
	{
		//----- WRITING IS NOT PERMITTED -----
		file_pointer->flags.bits.access_error = 1;
		return(FFS_EOF);
	}
	if (file_pointer->flags.bits.write_append_only)
	{
		//----- APPEND MODE - WRITING MAY ONLY OCCUR AS NEW BYTES AT THE END OF THE FILE -----
		dw_temp = file_pointer->current_byte_within_file;
		if (file_pointer->flags.bits.inc_posn_before_next_rw)
		{
			dw_temp++;
		}

		if (dw_temp < file_pointer->file_size)
		{
			//CURRENTLY POINTING TO WITHIN FILE - SET TO END OF FILE
			ffs_fseek(file_pointer, 1, FFS_SEEK_END);
		}
	}


	//-----------------------------------------------------------------------
	//----- CHECK FOR NEED TO MOVE TO NEXT BYTE POSITION BEFORE WRITING -----
	//-----------------------------------------------------------------------
	if (file_pointer->flags.bits.inc_posn_before_next_rw)
	{
		//----- INCREMENT BYTE COUNT -----
		file_pointer->current_byte_within_file++;

		file_pointer->current_byte++;
		
		if (file_pointer->current_byte >= ffs_bytes_per_sector)
		{
			//----- MOVE TO NEXT SECTOR -----
			file_pointer->current_byte = 0;

			file_pointer->current_sector++;

			if (file_pointer->current_sector >= sectors_per_cluster)
			{
				//----- MOVE TO NEXT CLUSTER -----
				file_pointer->current_sector = 0;

				if (file_pointer->current_byte_within_file >= file_pointer->file_size)
				{
					//ADD NEW CLUSTER TO END OF FILE
					dw_temp = ffs_get_next_free_cluster();
					if (dw_temp == 0xffffffff)			//0xffffffff = no empty cluster found
					{
						//NOT ENOUGH SPACE FOR ANY MORE OF FILE
						FFS_CE(1);
						file_pointer->flags.bits.end_of_file = 1;
						return(FFS_EOF);
					}

					//UPDATE THE CURRENT CLUSTER TO LINK TO THE NEXT CLUSTER
					ffs_modify_cluster_entry_in_fat (file_pointer->current_cluster, dw_temp);

					//UPDATE THE NEXT CLUSTER WITH THE END OF FILE MARKER
					ffs_modify_cluster_entry_in_fat (dw_temp, 0x0fffffff);
				}
				else
				{
					//MOVE TO NEXT EXISTING CLUSTER
                    dw_temp = ffs_get_next_cluster_no(file_pointer->current_cluster);
					if (dw_temp == 0xffffffff)			//0xffffffff = no empty cluster found
					{
						//ERROR
						FFS_CE(1);
						file_pointer->flags.bits.access_error = 1;
						return(FFS_EOF);
					}
				}

				file_pointer->current_cluster = dw_temp;
			}
		}

		file_pointer->flags.bits.inc_posn_before_next_rw = 0;
	}

	
	//-------------------------------------------------------------
	//----- CHECK FOR NEED TO LOAD CURRENT SECTOR INTO BUFFER -----
	//-------------------------------------------------------------
	dw_temp = (
				((file_pointer->current_cluster - 2) * sectors_per_cluster) +
				(DWORD)file_pointer->current_sector +
				data_area_start_sector
				);
	if (ffs_buffer_contains_lba != dw_temp)
	{
		ffs_read_sector_to_buffer(dw_temp);
	}


	//--------------------------
	//----- WRITE THE BYTE -----
	//--------------------------
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + file_pointer->current_byte;
	*buffer_pointer = (BYTE)(data & 0x00ff);


	//---------------------------------------------------------------------------
	//----- ADJUST FILE SIZE IF WE HAVE JUST WRITTEN TO THE END OF THE FILE -----
	//---------------------------------------------------------------------------
	if (file_pointer->current_byte_within_file >= file_pointer->file_size)
	{
		file_pointer->file_size++;
		file_pointer->flags.bits.file_size_has_changed = 1;
	}


	//------------------------------------------------------------------------
	//----- FLAG THAT WE NEED TO MOVE ON 1 BYTE NEXT TIME ACCESS IS MADE -----
	//------------------------------------------------------------------------
	//(we do this instead of incrementing now in case we have just done the last byte of the file)
	file_pointer->flags.bits.inc_posn_before_next_rw = 1;

	//----------------------------------------------------------
	//----- FLAG THAT THE BUFFER NEEDS WRITING TO THE CARD -----
	//----------------------------------------------------------
	ffs_buffer_needs_writing_to_card = 1;

	//-----------------------------------------------
	//----- EXIT WITH THE BYTE THAT WAS WRITTEN -----
	//-----------------------------------------------
	return(data);
}




//**********************************************
//**********************************************
//********** READ NEXT BYTE FROM FILE **********
//**********************************************
//**********************************************
//The byte read is returned as an int value (int type is specified by ANSI-C)
//If the End Of File has been reached or there has been an error reading, the function returns EOF.

//#define ffs_getc(file_pointer)		ffs_fgetc(file_pointer)			//Defined in header file - these 2 functions are equivalent

int ffs_fgetc (FFS_FILE *file_pointer)
{
	BYTE data;
	DWORD dw_temp;
	BYTE *buffer_pointer;

	//-------------------------------------------------
	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	//-------------------------------------------------
	if (file_pointer->flags.bits.file_is_open == 0)
		return(FFS_EOF);

	//---------------------------------------------------------------------------------------------------------------------------
	//----- CHECK THAT READING IN THIS POSITION IS PERMITTED FOR THE FOPEN MODE THAT WAS SPECIFIED WHEN THE FILE WAS OPENED -----
	//---------------------------------------------------------------------------------------------------------------------------
	if (file_pointer->flags.bits.read_permitted == 0)
	{
		//READ IS NOT PERMITTED
		file_pointer->flags.bits.access_error = 1;
		return(FFS_EOF);
	}


	//---------------------------------
	//----- CHECK FOR END OF FILE -----
	//---------------------------------
	dw_temp = file_pointer->current_byte_within_file;
	if (file_pointer->flags.bits.inc_posn_before_next_rw)
		dw_temp++;

	if (dw_temp >= file_pointer->file_size)
	{
		//TRYING TO READ PAST END OF FILE
		file_pointer->flags.bits.end_of_file = 1;
		return(FFS_EOF);
	}


	//-----------------------------------------------------------------------
	//----- CHECK FOR NEED TO MOVE TO NEXT BYTE POSITION BEFORE READING -----
	//-----------------------------------------------------------------------
	if (file_pointer->flags.bits.inc_posn_before_next_rw)
	{
		//----- INCREMENT BYTE COUNT -----
		file_pointer->current_byte_within_file++;

		file_pointer->current_byte++;
		
		if (file_pointer->current_byte >= ffs_bytes_per_sector)
		{
			//----- MOVE TO NEXT SECTOR -----
			file_pointer->current_byte = 0;

			file_pointer->current_sector++;

			if (file_pointer->current_sector >= sectors_per_cluster)
			{
				//----- MOVE TO NEXT CLUSTER -----
				file_pointer->current_sector = 0;

				//Get the next cluster number
				dw_temp = ffs_get_next_cluster_no(file_pointer->current_cluster);

				if (disk_is_fat_32)
				{
					//FAT32
					if (dw_temp >= 0x0ffffff8)
					{
						//There is no next cluster - all of file has been read
						FFS_CE(1);
						file_pointer->flags.bits.end_of_file = 1;
						return(FFS_EOF);
					}
				}
				else
				{
					//FAT16
					if (dw_temp >= 0xfff8)
					{
						//There is no next cluster - all of file has been read
						FFS_CE(1);
						file_pointer->flags.bits.end_of_file = 1;
						return(FFS_EOF);
					}
				}

				file_pointer->current_cluster = dw_temp;

			}
		}

		file_pointer->flags.bits.inc_posn_before_next_rw = 0;
	}


	//-------------------------------------------------------------
	//----- CHECK FOR NEED TO LOAD CURRENT SECTOR INTO BUFFER -----
	//-------------------------------------------------------------
	dw_temp = (
				((file_pointer->current_cluster - 2) * sectors_per_cluster) +
				(DWORD)file_pointer->current_sector +
				data_area_start_sector
				);
	if (ffs_buffer_contains_lba != dw_temp)
	{
		ffs_read_sector_to_buffer(dw_temp);
	}


	//------------------------
	//----- GET THE BYTE -----
	//------------------------
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + file_pointer->current_byte;
	data = (int)*buffer_pointer;



	//------------------------------------------------------------------------
	//----- FLAG THAT WE NEED TO MOVE ON 1 BYTE NEXT TIME ACCESS IS MADE -----
	//------------------------------------------------------------------------
	//(we do this instead of incrementing now in case we have just doen the last byte of the file)
	file_pointer->flags.bits.inc_posn_before_next_rw = 1;

	//--------------------------------------------
	//----- EXIT WITH THE BYTE THAT WAS READ -----
	//--------------------------------------------
	return((int)data);
}






//******************************************
//******************************************
//********** WRITE STRING TO FILE **********
//******************************************
//******************************************
//Writes string to file until the null termination is reached.  The null termination is not written to the
//file.  If a new line character (\n) is required it should be included at the end of the string
//
//Returns
//	Non-negative value if successful
//	EOF if errored
int ffs_fputs (const char *string, FFS_FILE *file_pointer)
{
	char *string_pointer;
	int return_value;


	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(FFS_EOF);

	//----- WRITE THE STRING -----
	string_pointer = (char*)string;
	
	while (*string_pointer != 0x00)
	{
		return_value = ffs_fputc ((int)*string_pointer++, file_pointer);

		if (return_value == FFS_EOF)
			return(FFS_EOF);
	}

	return(0);
}

//THIS IS A NON ANSI VARIENT OF THIS FUNCTION FOR USE WHEN WRITING A STRING FROM RAM WITH COMPILERS THAT WON'T
//DEAL WITH CONVERTING THE RAM STRING TO A CONSTANT STRING.
int ffs_fputs_char (char *string, FFS_FILE *file_pointer)
{
	char *string_pointer;
	int return_value;



	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(FFS_EOF);

	//----- WRITE THE STRING -----
	string_pointer = string;
	
	while (*string_pointer != 0x00)
	{
		return_value = ffs_fputc ((int)*string_pointer++, file_pointer);

		if (return_value == FFS_EOF)
			return(FFS_EOF);
	}

	return(0);
}





//*******************************************
//*******************************************
//********** READ STRING FROM FILE **********
//*******************************************
//*******************************************
//Reads characters from file and stores them into the specified buffer until a newline (\n) or EOF character is read or
//(length - 1) characters have been read.
//A newline character (\n) is not discarded.  A null termination is added to the string
//
//Returns
//	Pointer to the buffer if successful
//	Null pointer if end-of-file or error (use ffs_ferror or ffs_feof to check what happened)
char* ffs_fgets (char *string, int length, FFS_FILE *file_pointer)
{
	char *string_pointer;
	int return_value;
	
	//----- READ THE STRING -----
	string_pointer = string;

	return_value = ffs_fgetc(file_pointer);
	while ((return_value != FFS_EOF) && (length > 1))
	{
		*string_pointer++ = (char)return_value;
		
		length--;

		if (return_value == (int)'\n')
			break;

		return_value = ffs_fgetc(file_pointer);
	}

	//----- STORE THE TERMINATING NULL -----
	*string_pointer++ = 0x00;

	if (return_value == FFS_EOF)
		return(0x00);
	else
		return(string);

}





//**********************************************
//**********************************************
//********** WRITE DATA BLOCK TO FILE **********
//**********************************************
//**********************************************
//Writes count number of items, each one with a size of size bytes, from the specified buffer
//No translation occurs for files opened in text mode
//The total number of bytes to be written is (size x count).
//
//Returns
//	Number of full items (not bytes) successfully written. This may be less than the requested number if an error occurred.
 
int ffs_fwrite (const void *buffer, int size, int count, FFS_FILE *file_pointer)
{
	int size_count;
	BYTE *string_Pointer;
	int number_of_items_written = 0;


	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(FFS_EOF);

	string_Pointer = (BYTE*)buffer;

	//STORE ALL OF THE ITEMS
	while (count)
	{
		//STORE EACH ITEM
		for (size_count = 0; size_count < size; size_count++)
		{

			if (ffs_fputc((int)*string_Pointer++, file_pointer) == FFS_EOF)
			{
				return(number_of_items_written);
			}
		}
		number_of_items_written++;
		count--;
	}

	return(number_of_items_written);
}




//***********************************************
//***********************************************
//********** READ DATA BLOCK FROM FILE **********
//***********************************************
//***********************************************
//Read count number of items each one with a size of size bytes from the file to the specified buffer.
//Total amount of bytes read is (size x count)
//
//Returns
//	Number of items (not bytes) read is returned.  If this number differs from the requested amount (count) an error has
//	occurred or End Of File has been reached (use ffs_ferror or ffs_feof to check what happened)
int ffs_fread (void *buffer, int size, int count, FFS_FILE *file_pointer)
{
	int size_count;
	int return_value;
	BYTE *string_Pointer;
	int number_of_items_written = 0;


	string_Pointer = (BYTE*)buffer;

	//READ ALL OF THE ITEMS
	while (count)
	{
		//STORE EACH ITEM
		for (size_count = 0; size_count < size; size_count++)
		{

			return_value = ffs_fgetc(file_pointer);

			if (return_value == FFS_EOF)
				return(number_of_items_written);
			
			*string_Pointer++ = (BYTE)return_value;
		}
		number_of_items_written++;
		count--;
	}

	return(number_of_items_written);
}






//**********************************************************
//**********************************************************
//********** STORE ANY UNWRITTEN DATA TO THE CARD **********
//**********************************************************
//**********************************************************
//Write any data that is currently held in microcontroller / processor ram that is waiting to be
//written to the card.  Update the file filesize value if it has changed.
//Returns
//	0 if successful, 1 otherwise
int ffs_fflush (FFS_FILE *file_pointer)
{
	BYTE *buffer_pointer;
#ifdef USE_REAL_TIME_CLOCK
	WORD_VAL w_temp;
#endif


	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(1);


	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);


	//If the data buffer contains data that is waiting to be written then write it
	//(We just store any unwritten data regardless of what file this funciton is called with as there is only 1 buffer)
	if (ffs_buffer_needs_writing_to_card)
	{
		if (ffs_buffer_contains_lba != 0xffffffff)			//This should not be possible but check is made just in case!
			ffs_write_sector_from_buffer(ffs_buffer_contains_lba);

		ffs_buffer_needs_writing_to_card = 0;
	}

	if (file_pointer->flags.bits.file_size_has_changed)
	{
		//----- STORE THE NEW FILE SIZE IN THE FILES DIRECTORY ENTRY -----
		ffs_read_sector_to_buffer (file_pointer->directory_entry_sector);

		//Offset to the start of the entry
		buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + ((WORD)file_pointer->directory_entry_within_sector << 5) + 18;			//Start of the dates is 18 bytes into the entry


		//WRITE LAST ACESSED DATE [2]
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = (((WORD)FFS_RTC_YEAR + 20) << 9) | ((WORD)FFS_RTC_MONTH << 5) | ((WORD)FFS_RTC_DATE);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			buffer_pointer++;
			buffer_pointer++;
		#endif

		//WRITE EXTENDED ATTRIBUTE [2]
		buffer_pointer += 2;

		//LAST MODIFIED TIME [2]
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = ((WORD)FFS_RTC_HOURS << 11) | ((WORD)FFS_RTC_MINUTES << 5) | ((WORD)FFS_RTC_SECONDS >> 1);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			buffer_pointer++;
			buffer_pointer++;
		#endif

		//LAST MODIFIED DATE [2]
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = (((WORD)FFS_RTC_YEAR + 20) << 9) | ((WORD)FFS_RTC_MONTH << 5) | ((WORD)FFS_RTC_DATE);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			buffer_pointer++;
			buffer_pointer++;
		#endif
			
		//WRITE CLUSTER [2]
		buffer_pointer += 2;

		//FILE SIZE [2]
		*buffer_pointer++ = (BYTE)(file_pointer->file_size & 0x000000ff);
		*buffer_pointer++ = (BYTE)((file_pointer->file_size & 0x0000ff00) >> 8);
		*buffer_pointer++ = (BYTE)((file_pointer->file_size & 0x00ff0000) >> 16);
		*buffer_pointer++ = (BYTE)((file_pointer->file_size & 0xff000000) >> 24);

		ffs_write_sector_from_buffer(file_pointer->directory_entry_sector);

		file_pointer->flags.bits.file_size_has_changed = 0;
	}

	FFS_CE(1);										//De-select the card


	return(0);
}




//********************************
//********************************
//********** CLOSE FILE **********
//********************************
//********************************
//Return value
//	0 = file successfully closed
//	1 = error
int ffs_fclose (FFS_FILE *file_pointer)
{
	//----- EXIT IF THIS FILE ISN'T ACTUALLY OPEN -----
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);

	//----- ENSURE ANY UNWRITTEN DATA AND ANY CHANGE IN FILE SIZE IS STORED -----
	ffs_fflush(file_pointer);

	//----- FLAG THAT THE FILE IS NO LOGER OPEN AND THIS FILE HANDLER IS AVAILABLE AGAIN -----
	file_pointer->flags.bits.file_is_open = 0;


	//----- ENSURE CARD HAS COMPLETED LAST WRITE PROCESS -----
	//When closing a file if the last write to the card before it is removed or powered down just occured some cards have been found to not store
	//the last sector written.  If the card is flagging that its busy then provide clock pulses to allow it to complete its last operation
	FFS_CE(0);										//Select the card
	DO_BUSY_STATE_ACCESS_DELAY;
	while (FFC_DI == 0)
		ffs_read_byte();
	FFS_CE(1);										//Deselect the card

	return(0);
}





//*********************************
//*********************************
//********** DELETE FILE **********
//*********************************
//*********************************
//Return value
// 0 = file is succesfully deleted
// 1 = error (file doesn't exist or can't be deleted as its currently open)
int ffs_remove (const char *filename)
{
	BYTE temp;
	DWORD next_cluster;
	BYTE converted_file_name[8];
	BYTE converted_file_extension[3];
	DWORD read_file_size;
	DWORD read_cluster_number;
	BYTE attribute_byte;
	DWORD lowest_cluster_number_released = 0xffffffff;
	DWORD directory_entry_sector;
	BYTE directory_entry_within_sector;
	DWORD fat_entries_per_sector;
	DWORD dw_temp;
	BYTE *buffer_pointer;
	DWORD lba;
	BYTE fat_needs_storing = 0;
	BYTE count;
	DWORD current_cluster;
	DWORD cluster_to_modify;
	DWORD lba_last;




	//Check card is inserted and has been initialised
	if (ffs_card_ok == 0)
		return(1);

	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(1);


	//----- FIND THE FILE -----
	read_cluster_number = ffs_find_file(filename, &read_file_size, &attribute_byte, &directory_entry_sector, &directory_entry_within_sector, converted_file_name, converted_file_extension);
	FFS_CE(1);
	if (read_cluster_number == 0xffffffff)		//0xffffffff = file not found
	{
		//FILE DOES NOT EXIST
		return(1);
	}


	//------------------------------------
	//----- FOUND THE FILE TO DELETE -----
	//------------------------------------

	//----- CHECK FILE IS NOT BEING ACCESSED BY ANY CURRENT FILE HANDLER -----
	//if (read_cluster_number != 0xffffffff)		//0xffffffff = file not found
	//{
		for (temp = 0; temp < FFS_FOPEN_MAX; temp++)
		{
			if (ffs_file[temp].flags.bits.file_is_open)
			{
				if (
					(ffs_file[temp].directory_entry_sector == directory_entry_sector) &&
					(ffs_file[temp].directory_entry_within_sector == directory_entry_within_sector)
					)
				{
					return(1);
				}
			}
		}
	//}


	//----- SET THE 1ST CHARACTER OF THE FILE NAME TO 0xE5 TO INDICATE ITS A DELETED ENTRY IN THE DIRECTORY -----
	converted_file_name[0] = 0xe5;
	
	//----- STORE THE MODIFIED DIRECTORY ENTRY BACK TO THE DISK -----
	ffs_overwrite_last_directory_entry(converted_file_name, converted_file_extension, &attribute_byte, &read_file_size, &read_cluster_number, 0x00);

	//----- CHANGE ALL ENTRIES IN THE FAT TABLE FOR THIS FILE BACK TO 0 TO INDICATE THE CLUSTERS ARE NOW FREE -----
	if (disk_is_fat_32)
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 2);		//FAT32 - Divide no of bytes per sector by 4 as each fat entry is 1 double word
	else
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 1);		//FAT16 - Divide no of bytes per sector by 2 as each fat entry is 1 word

	while(1)
	{
		#ifdef CLEAR_WATCHDOG_TIMER
			CLEAR_WATCHDOG_TIMER;
		#endif


		//IF THE NEXT CLUSTER ENTRY IS IN THE SAME SECTOR AS THE LAST CLUSTER ENTRY THEN JUST UPDATE THE SECTOR BUFFER WITHOUT READ / WRITE TO THE CARD
		//(If we don't do this then for each change to the FAT table we have to do a sector read followed by 2 sector writes
		//as there are 2 FAT tables to update.  This makes deleting increadibly slow as the efficiency of working on a single
		//sector as much as possible before writing it and reading the next is lost.  This solution solves the problem based
		//on the assumption that most files will largely use concurrent clusters and therefore concurrent entries in the FAT table).

		//next_cluster = ffs_get_next_cluster_no(read_cluster_number);		//Instead we implement the working of this function directly here:-
		current_cluster = read_cluster_number;
		lba = fat1_start_sector;

		//----- MOVE TO THE SECTOR IN THE FAT TABLE FOR THE CURRENT CLUSTER NUMBER -----
		dw_temp = (current_cluster / fat_entries_per_sector);
		lba += dw_temp;
		current_cluster -= (dw_temp * fat_entries_per_sector);

		if (lba != ffs_buffer_contains_lba)
		{
			//WE'VE MOVED TO A NEW SECTOR
			if (fat_needs_storing)
			{
				//WE NEED TO STORE THE PREVIOUS SECTOR OF FAT TABLE ENTRIES
				for (count = 0x01; count < 0x10; count <<= 1)
				{
					if (count & active_fat_table_flags)									//Only modify FAT tables that are active
						ffs_write_sector_from_buffer (lba_last);

					lba_last += sectors_per_fat;										//Move to next FAT table
				}
				fat_needs_storing = 0;
			}

			ffs_read_sector_to_buffer(lba);
		}
        lba_last = lba;

		//----- GET THE NEXT CLUSTER ENTRY FROM THE FAT TABLE -----
		if (disk_is_fat_32)
		{
			buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (current_cluster << 2);
	
			lba = (DWORD)*buffer_pointer++;									//FAT32 - 1 double word per entry
			lba |= ((DWORD)(*buffer_pointer++) << 8);
			lba |= ((DWORD)(*buffer_pointer++) << 16);
			lba |= ((DWORD)((*buffer_pointer++) & 0x0f) << 24);				//Top nibble reserved bits need to be removed
		}
		else
		{
			buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (current_cluster << 1);
	
			lba = (DWORD)*buffer_pointer++;
			lba |= ((DWORD)(*buffer_pointer++) << 8);						//FAT16 - 1 word per entry
		}
		next_cluster = lba;




		//ffs_modify_cluster_entry_in_fat(read_cluster_number, ((DWORD)0x00000000));		//Instead we implement the working of this function directly here (in the knowledge that we already have the correct sector loaded):-
		cluster_to_modify = read_cluster_number;

		//----- MOVE TO THE SECTOR FOR THIS CLUSTER ENTRY IN THE FAT1 TABLE -----
		lba = fat1_start_sector;

		//Move to the relevant sector in the FAT table
		dw_temp = (cluster_to_modify / fat_entries_per_sector);
		lba += dw_temp;
		cluster_to_modify -= (dw_temp * fat_entries_per_sector);


		if (disk_is_fat_32)									//carried accross to other tables which is very unlikely (and anyway an error has then already occured)
			buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (cluster_to_modify << 2);
		else
			buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (cluster_to_modify << 1);

		*buffer_pointer++ = (BYTE)(((DWORD)0x00000000) & 0x000000ff);
		*buffer_pointer++ = (BYTE)((((DWORD)0x00000000) & 0x0000ff00) >> 8);
		if (disk_is_fat_32)
		{
			*buffer_pointer++ = (BYTE)((((DWORD)0x00000000) & 0x00ff0000) >> 16);
			temp = (*buffer_pointer & 0xf0);
			*buffer_pointer++ = ((BYTE)((((DWORD)0x00000000) & 0x0f000000) >> 24) | temp);		//The top 4 bits are reserved and should not be modified
		}
        fat_needs_storing = 1;



        read_cluster_number = next_cluster;



		//CHECK FOR THAT WAS THE LAST CLUSTER
		if (disk_is_fat_32)
		{
			//FAT32
			if ((next_cluster & 0x0fffffff) >= 0x0ffffff8)
			{
				//There is no next cluster - all of file has been done
				break;
			}
		}
		else
		{
			//FAT16
			if (next_cluster >= 0xfff8)
			{
				//There is no next cluster - all of file has been done
				break;
			}
		}
		//Check for error - shouldn't be able to happen (0 & 1 are reserved cluster numbers)
		if (next_cluster < 2)
		{
			break;
		}
		
		read_cluster_number = next_cluster;
		if (read_cluster_number < lowest_cluster_number_released)
			lowest_cluster_number_released = read_cluster_number;
	}

	//Store each fat table just modified
	if (fat_needs_storing)
	{
		for (count = 0x01; count < 0x10; count <<= 1)
		{
			if (count & active_fat_table_flags)									//Only modify FAT tables that are active
				ffs_write_sector_from_buffer(lba_last);

			lba_last += sectors_per_fat;										//Move to next FAT table
		}
	}

	FFS_CE(1);

	//If we have free'd up some lower clusters than the current cluster to start looking in when writing new clusters then change the value
	if (last_found_free_cluster > lowest_cluster_number_released)
		last_found_free_cluster = lowest_cluster_number_released;

	//----- ENSURE CARD HAS COMPLETED LAST WRITE PROCESS -----
	//If the last write to the card before it is removed or powered down just occured some cards have been found to not store
	//the last sector written.  If the card is flagging that its busy then provide clock pulses to allow it to complete its last operation
	DO_BUSY_STATE_ACCESS_DELAY;
	FFS_CE(0);										//Select the card
	DO_BUSY_STATE_ACCESS_DELAY;
	ffs_read_byte();
	while (FFC_DI == 0)
		ffs_read_byte();
	FFS_CE(1);										//Deselect the card

	return(0);	
}








//**************************************
//**************************************
//********** CHANGE FILE SIZE **********
//**************************************
//**************************************
//Return value
// 0 = file size succesfully changed
// 1 = error (file doesn't exist or can't be changed as its currently open)
int ffs_change_file_size (const char *filename, DWORD new_file_size)
{
	BYTE temp;
	DWORD next_cluster;
	BYTE converted_file_name[8];
	BYTE converted_file_extension[3];
	DWORD read_file_size;
	DWORD read_cluster_number;
	BYTE attribute_byte;
	DWORD lowest_cluster_number_released = 0xffffffff;
	DWORD directory_entry_sector;
	BYTE directory_entry_within_sector;
	DWORD fat_entries_per_sector;
	DWORD dw_temp;
	BYTE *buffer_pointer;
	DWORD lba;
	DWORD lba_last;
	DWORD bytes_per_cluster;
	BYTE fat_needs_storing = 0;
	BYTE count;
    DWORD next_free_cluster_number;


	//Check card is inserted and has been initialised
	if (ffs_card_ok == 0)
		return(1);

	//Exit if card is write protected
	if (ffs_card_write_protected)
		return(1);


	//----- FIND THE FILE -----
	read_cluster_number = ffs_find_file(filename, &read_file_size, &attribute_byte, &directory_entry_sector, &directory_entry_within_sector, converted_file_name, converted_file_extension);
	FFS_CE(1);
	if (read_cluster_number == 0xffffffff)		//0xffffffff = file not found
	{
		//FILE DOES NOT EXIST
		return(1);
	}


	//------------------------------------
	//----- FOUND THE FILE TO CHANGE -----
	//------------------------------------
	//----- CHECK FILE IS NOT BEING ACCESSED BY ANY CURRENT FILE HANDLER -----
	//if (read_cluster_number != 0xffffffff)		//0xffffffff = file not found
	//{
		for (temp = 0; temp < FFS_FOPEN_MAX; temp++)
		{
			if (ffs_file[temp].flags.bits.file_is_open)
			{
				if (
					(ffs_file[temp].directory_entry_sector == directory_entry_sector) &&
					(ffs_file[temp].directory_entry_within_sector == directory_entry_within_sector)
					)
				{
					return(1);
				}
			}
		}
	//}


	//----- CHECK NEW FILE SIZE IS ACTUALLY A CHANGE -----
	if (read_file_size == new_file_size)
		return(0);
	
	//----- CHECK NEW FILE SIZE IS VALID -----
	if (new_file_size == 0)
		return(1);


	bytes_per_cluster = (DWORD)sectors_per_cluster * 512;

	if (read_file_size > new_file_size)
	{
		//--------------------------------
		//--------------------------------
		//----- REDUCE THE FILE SIZE -----
		//--------------------------------
		//--------------------------------

		//----- STORE THE NEW DIRECTORY ENTRY FILE SIZE -----
        read_file_size = new_file_size;
		ffs_overwrite_last_directory_entry(converted_file_name, converted_file_extension, &attribute_byte, &read_file_size, &read_cluster_number, 0x00);
	

		//----- MOVE TO LAST USED CLUSTER -----
        dw_temp = new_file_size;
		while (dw_temp > bytes_per_cluster)
		{
			dw_temp -= bytes_per_cluster;
            
			next_cluster = ffs_get_next_cluster_no(read_cluster_number);
			if (disk_is_fat_32)
			{
				//FAT32
				if (next_cluster >= 0x0ffffff8)
					return(1);		//Error - There is no next cluster - all of file has been read
			}
			else
			{
				//FAT16
				if (next_cluster >= 0xfff8)
					return(1);		//Error - There is no next cluster - all of file has been read
			}
			
			read_cluster_number = next_cluster;
		}
        //read_cluster_number = final cluster of new file size

        next_cluster = ffs_get_next_cluster_no(read_cluster_number);
		if (disk_is_fat_32)
		{
			//FAT32
			if (next_cluster >= 0x0ffffff8)
				return(0);		//This is already the last cluster
		}
		else
		{
			//FAT16
			if (next_cluster >= 0xfff8)
				return(0);		//This is already the last cluster
		}


		//----- MARK CLUSTER AS LAST USED -----
        ffs_modify_cluster_entry_in_fat(read_cluster_number, (DWORD) 0x0fffffff);
        read_cluster_number = next_cluster;


		//----- DELETE ALL REMAINING CLUSTERS ----
		while(1)
		{
			#ifdef CLEAR_WATCHDOG_TIMER
				CLEAR_WATCHDOG_TIMER;
			#endif
	
			//Get next cluster number
			next_cluster = ffs_get_next_cluster_no(read_cluster_number);
	
			//IF THE NEXT CLUSTER IS IN THE SAME AS THIS CLUSTER THEN JUST UPDATE THE SECTOR BUFFER
			//(If we don't do this then for each change to the FAT table we have to do a sector read followed by 2 sector writes
			//as there are 2 FAT tables to update.  This makes deleting increadibly slow as the efficiency of working on a single
			//sector as much as possible before writing it and reading the next is lost.  This solution solves the problem based
			//on the assumption that most files will largely use concurrent clusters and therefore concurrent entries in the FAT table).
	
			lba = fat1_start_sector;
	
			if (disk_is_fat_32)
				fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 2);		//FAT32 - Divide no of bytes per sector by 4 as each fat entry is 1 double word
			else
				fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 1);		//FAT16 - Divide no of bytes per sector by 2 as each fat entry is 1 word
	
			dw_temp = (read_cluster_number / fat_entries_per_sector);				//dw_temp now has the sector address that will need to be modifed for this cluster entry
			lba += dw_temp;
            lba_last = lba;
	
			if (lba == ffs_buffer_contains_lba)
			{
				//----- NEXT CLUSTER ENTRY IS IN SAME FAT TABLE SECTOR AS THE CURRENT CLUSTER ENTRY -----
				fat_needs_storing = 1;
				read_cluster_number -= (dw_temp * fat_entries_per_sector);
	
				//Point to this entry in the FAT table
				if (disk_is_fat_32)
				{
					buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (read_cluster_number << 2);
	
					*buffer_pointer++ = 0x00;										//FAT32 - 1 double word per entry
					*buffer_pointer++ = 0x00;
					*buffer_pointer++ = 0x00;
					*buffer_pointer++ &= 0xf0;										//(The top 4 bits are reserved and should not be modified)
				}
				else
				{
					buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (read_cluster_number << 1);
	
					*buffer_pointer++ = 0x00;										//FAT16 - 1 word per entry
					*buffer_pointer++ = 0x00;
				}
			}
			else
			{
				//----- NEXT CLUSTER ENTRY IS NOT IN THE SAME FAT TABLE SECTOR - DO UPDATE AS NORMAL -----
	
				//Store each fat table just modified
				if (fat_needs_storing)
				{
					for (count = 0x01; count < 0x10; count <<= 1)
					{
						if (count & active_fat_table_flags)									//Only modify FAT tables that are active
							ffs_write_sector_from_buffer(lba_last);
	
						lba_last += sectors_per_fat;												//Move to next FAT table
					}
					fat_needs_storing = 0;
				}
	
				//Clear the current cluster entry
				ffs_modify_cluster_entry_in_fat(read_cluster_number, (DWORD) 0x00000000);
			}
	
			//CHECK FOR THAT WAS THE LAST CLUSTER
			if (disk_is_fat_32)
			{
				//FAT32
				if ((next_cluster & 0x0fffffff) >= 0x0ffffff8)
				{
					//There is no next cluster - all of file has been done
					break;
				}
			}
			else
			{
				//FAT16
				if (next_cluster >= 0xfff8)
				{
					//There is no next cluster - all of file has been done
					break;
				}
			}
			//Check for error - shouldn't be able to happen (0 & 1 are reserved cluster numbers)
			if (next_cluster < 2)
			{
				break;
			}
			
			read_cluster_number = next_cluster;
			if (read_cluster_number < lowest_cluster_number_released)
				lowest_cluster_number_released = read_cluster_number;
		}
	
	
		//Store each fat table just modified
		if (fat_needs_storing)
		{
			for (count = 0x01; count < 0x10; count <<= 1)
			{
				if (count & active_fat_table_flags)									//Only modify FAT tables that are active
					ffs_write_sector_from_buffer (lba_last);
	
				lba_last += sectors_per_fat;												//Move to next FAT table
			}
		}
	
		FFS_CE(1);
	}
	else
	{
		//----------------------------------
		//----------------------------------
		//----- INCREASE THE FILE SIZE -----
		//----------------------------------
		//----------------------------------

		//----- MOVE TO LAST USED CLUSTER -----
        dw_temp = read_file_size;
		while (dw_temp > bytes_per_cluster)
		{
			dw_temp -= bytes_per_cluster;
            
			next_cluster = ffs_get_next_cluster_no(read_cluster_number);
			if (disk_is_fat_32)
			{
				//FAT32
				if (next_cluster >= 0x0ffffff8)
					return(1);		//Error - There is no next cluster - all of file has been read
			}
			else
			{
				//FAT16
				if (next_cluster >= 0xfff8)
					return(1);		//Error - There is no next cluster - all of file has been read
			}
			
			read_cluster_number = next_cluster;
		}
		//read_cluster_number = final cluster of existing file
		//dw_temp = number of bytes already used in the final cluster

		dw_temp += (new_file_size - read_file_size);


		//----- ADD THE ADDITIONAL CLUSTERS REQUIRED -----
		while (dw_temp > bytes_per_cluster)
		{
			dw_temp -= bytes_per_cluster;

			next_free_cluster_number = ffs_get_next_free_cluster();
			if (next_free_cluster_number == 0xffffffff)			//0xffffffff = no empty cluster found
			{
				//NOT ENOUGH SPACE FOR ANY MORE OF FILE
				FFS_CE(1);
				return(1);
			}

			//UPDATE THE CURRENT CLUSTER TO LINK TO THE NEXT CLUSTER
			ffs_modify_cluster_entry_in_fat (read_cluster_number, next_free_cluster_number);
            ffs_modify_cluster_entry_in_fat (next_free_cluster_number, 0x0fffffff);
            read_cluster_number = next_free_cluster_number;
		}
		
		//UPDATE THE NEXT CLUSTER WITH THE END OF FILE MARKER
		ffs_modify_cluster_entry_in_fat (read_cluster_number, 0x0fffffff);



		//----- STORE THE NEW DIRECTORY ENTRY FILE SIZE -----
		read_cluster_number = ffs_find_file(filename, &read_file_size, &attribute_byte, &directory_entry_sector, &directory_entry_within_sector, converted_file_name, converted_file_extension);
		if (read_cluster_number == 0xffffffff)		//0xffffffff = file not found
			return(1);			//Should not be possible
		
		read_file_size = new_file_size;
		ffs_overwrite_last_directory_entry(converted_file_name, converted_file_extension, &attribute_byte, &read_file_size, &read_cluster_number, 0x00);

		FFS_CE(1);
	
	}

	//If we have free'd up some lower clusters than the current cluster to start looking in when writing new clusters then change the value
	if (last_found_free_cluster > lowest_cluster_number_released)
		last_found_free_cluster = lowest_cluster_number_released;

	//----- ENSURE CARD HAS COMPLETED LAST WRITE PROCESS -----
	//If the last write to the card before it is removed or powered down just occured some cards have been found to not store
	//the last sector written.  If the card is flagging that its busy then provide clock pulses to allow it to complete its last operation
	DO_BUSY_STATE_ACCESS_DELAY;
	FFS_CE(0);										//Select the card
	DO_BUSY_STATE_ACCESS_DELAY;
	ffs_read_byte();
	while (FFC_DI == 0)
		ffs_read_byte();
	FFS_CE(1);										//Deselect the card

	return(0);
}



//*********************************
//*********************************
//********** RENAME FILE **********
//*********************************
//*********************************
//Return value
// 0 = file is succesfully renamed
// 1 = error (file doesn't exist or can't be renamed as its currently open)
int ffs_rename (const char *old_filename, const char *new_filename)
{
	BYTE converted_file_name[8];
	BYTE converted_file_extension[3];
	DWORD read_cluster_number = 0;
	DWORD read_file_size;
	BYTE attribute_byte;
	DWORD directory_entry_sector;
	BYTE directory_entry_within_sector;
	BYTE temp;


	//CHECK CARD IS INSERTED AND HAS BEEN INITIALISED
	if (ffs_card_ok == 0)
		return(1);

	//Exit of card is write protected
	if (ffs_card_write_protected)
		return(1);

	//----- CHECK DESTINATION FILENAME IS NOT ALREADY IN USE -----
	read_cluster_number = ffs_find_file(new_filename, &read_file_size, &attribute_byte, &directory_entry_sector, &directory_entry_within_sector, converted_file_name, converted_file_extension);
	if (read_cluster_number != 0xffffffff)		//0xffffffff = file not found
	{
		//FILE ALREADY EXISTS
		FFS_CE(1);
		return(1);
	}

	//----- FIND THE FILE -----
	read_cluster_number = ffs_find_file(old_filename, &read_file_size, &attribute_byte, &directory_entry_sector, &directory_entry_within_sector, converted_file_name, converted_file_extension);
	if (read_cluster_number == 0xffffffff)		//0xffffffff = file not found
	{
		//FILE DOES NOT EXIST
		FFS_CE(1);
		return(1);
	}

	//------------------------------------
	//----- FOUND THE FILE TO RENAME -----
	//------------------------------------

	//----- CHECK FILE IS NOT BEING ACCESSED BY ANY CURRENT FILE HANDLER -----
	if (read_cluster_number != 0xffffffff)		//0xffffffff = file not found
	{
		for (temp = 0; temp < FFS_FOPEN_MAX; temp++)
		{
			if (ffs_file[temp].flags.bits.file_is_open)
			{
				if (
					(ffs_file[temp].directory_entry_sector == directory_entry_sector) &&
					(ffs_file[temp].directory_entry_within_sector == directory_entry_within_sector)
					)
				{
					return(1);
				}
			}
		}
	}

	//CONVERT THE NEW FILENAME TO DOS FILENAME
	if (ffs_convert_filename_to_dos (new_filename, converted_file_name, converted_file_extension))		//Returns 1 if filename had wildcard characters
		return(1);

	//STORE THE MODIFIED DIRECTORY ENTRY BACK TO THE DISK
	ffs_overwrite_last_directory_entry(converted_file_name, converted_file_extension, &attribute_byte, &read_file_size, &read_cluster_number, 0x02);

	return(0);
}





//*******************************************************
//*******************************************************
//********** CLEAR ERROR AND END OF FILE FLAGS **********
//*******************************************************
//*******************************************************
void ffs_clearerr (FFS_FILE *file_pointer)
{

	file_pointer->flags.bits.access_error = 0;
	file_pointer->flags.bits.end_of_file = 0;
}






//**************************************************
//**************************************************
//********** HAS END OF FILE BEEN REACHED **********
//**************************************************
//**************************************************
int ffs_feof (FFS_FILE *file_pointer)
{

	if (file_pointer->flags.bits.end_of_file)
		return(1);
	else
		return(0);
}






//*************************************************************
//*************************************************************
//********** HAS AN ERROR OCCURED DURING FILE ACCESS **********
//*************************************************************
//*************************************************************
int ffs_ferror (FFS_FILE *file_pointer)
{

	if (file_pointer->flags.bits.access_error)
		return(1);
	else
		return(0);
}







//******************************************************
//******************************************************
//********** IS CARD INSERTED AND AVAILABLE ************
//******************************************************
//******************************************************
//Returns:
BYTE ffs_is_card_available (void)
{
		return(ffs_card_ok);
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



//*******************************
//*******************************
//********** FIND FILE **********
//*******************************
//*******************************
//filename
//	Only 8 character DOS compatible root directory filenames are allowed.  Format is F.E where F may be between 1 and 8 characters
//	and E may be between 1 and 3 characters, null terminated.  The '*' and '?' wildcard characters are allowed.
//file_size
//	File size (bytes) will be written to here
//attribute_byte
//	Attribute byte will be written to here
//directory_entry_sector
//	Sector that contains the files directory entry will be written to here
//directory_entry_within_sector
//	The file directory entry number within the sector that contains the file will be written to here
//read_file_name
//	8 character buffer containing the filename read from the directory entry (may be needed if using this function with wildcard characters)
//read_file_extension
//	3 character buffer containing the filename extension read from the directory entry (may be needed if using this function with wildcard characters)
//
//Returns
//	file start cluster number (0xffffffff = file not found)
//
//NOTES
//	ffs_overwrite_last_directory_file_name may be used to modify the file entry in the directory after finding it with this function

DWORD ffs_find_file (const char *filename, DWORD *file_size, BYTE *attribute_byte, DWORD *directory_entry_sector,
					 BYTE *directory_entry_within_sector, BYTE *read_file_name, BYTE *read_file_extension)
{
	BYTE temp;
	BYTE this_is_the_file;
	BYTE this_is_first_read;
	BYTE converted_file_name[8];
	BYTE converted_file_extension[3];
	DWORD read_cluster_number;
	

	//----- CHECK CARD IS INSERTED AND HAS BEEN INITIALISED -----
	if(ffs_card_ok == 0)
		return((DWORD)0xffffffff);

	//----- CONVERT NULL TERMINATED FILE NAME TO 8 CHARACTER DOS FILENAME -----
	ffs_convert_filename_to_dos (filename, converted_file_name, converted_file_extension);

	this_is_first_read = 1;
	//----- GET EACH DIRECTORY ENTRY UNTIL WE FIND THE MATCHING FILE -----
	while (1)
	{
		#ifdef CLEAR_WATCHDOG_TIMER
			CLEAR_WATCHDOG_TIMER;
		#endif

		//GET NEXT ENTRY
		if (ffs_read_next_directory_entry (read_file_name, read_file_extension, attribute_byte, file_size,
											&read_cluster_number, this_is_first_read, directory_entry_sector,
											directory_entry_within_sector) == 0)
		{
			FFS_CE(1);								//Deselect the card
			return((DWORD)0xffffffff);				//Reached end of directory
		}
		this_is_first_read = 0;

		this_is_the_file = 1;								//Default to this is the file

		//If 1st value is 0x00 then entry has never been used (erased entries are 0xe5) so 0x00 is the end of used directory marker)
		if (*(read_file_name + 0) == 0x00)
		{
			FFS_CE(1);						//Deselect the card
			return((DWORD)0xffffffff);
		}

		//If 1st value is 0xe5 then entry has been deleted
		if (*(read_file_name + 0) == 0xe5)
		{
				this_is_the_file = 0;
		}
		
		//Does the file name match?
		for (temp = 0; temp < 8; temp++)
		{
			if ((converted_file_name[temp] != '?') && (*(read_file_name + temp) != converted_file_name[temp]))
				this_is_the_file = 0;
		}


		//Does file extension match?
		for (temp = 0; temp < 3; temp++)
		{
			if ((converted_file_extension[temp] != '?') && (*(read_file_extension + temp) != converted_file_extension[temp]))
				this_is_the_file = 0;
		}

		//Is attribute byte for a file?
		if (*attribute_byte & 0x08)						//Not valid if the 'volume' bit is set
			this_is_the_file = 0;

		if (*attribute_byte & 0x10)						//Not valid if the 'directory' bit is set
			this_is_the_file = 0;

		if (*attribute_byte & 0x02)						//Not valid if the 'hidden' bit is set
			this_is_the_file = 0;

		//----- IS THIS THE FILE? -----
		if (this_is_the_file)
		{
			//THIS IS THE FILE
			FFS_CE(1);						//Deselect the card
			return(read_cluster_number);
		}
	}
}






//*******************************************************************
//*******************************************************************
//********** CONVERT FILE NAME TO 8 CHARACTER DOS FILENAME **********
//*******************************************************************
//*******************************************************************
//Source filename is a case insensitive string with between 1 and 8 filename characters, a period (full stop) character, between 1 and 3 extension characters and a terminating null.
//Returns:
//	1 if the filename contained any wildcard characters, 0 if not (this allow calling functions to detect invalid names if they are creating a new file)
BYTE ffs_convert_filename_to_dos (const char *source_filename, BYTE *dos_filename, BYTE *dos_extension)
{
	BYTE temp;
	BYTE no_null_found_yet;
	BYTE wildcard_character_found = 0;
	BYTE asterix_wildcard_used;

	//----- DO THE FILENAME -----
	no_null_found_yet = 1;
	asterix_wildcard_used = 0;
	for (temp = 0; temp < 8; temp++)
	{
		//Flag if filename contains a wildcard character (for return)
		if ((*source_filename == '?') || (*source_filename == '*'))
			wildcard_character_found = 1;

		//Check for asterix wildcard character
		if (*source_filename == '*')
			asterix_wildcard_used = 1;

		//Check for terminating '.'
		if (*source_filename == '.')
			no_null_found_yet = 0;

		//COPY THE CHARACTER
		if (asterix_wildcard_used)
		{
			//Doing asterix wildcard replace
			*dos_filename++ = '?';
			if (no_null_found_yet)
				*source_filename++;
		}
		else if (no_null_found_yet)
		{
			//Copy the character
			*dos_filename++ = *source_filename++;
		}
		else
		{
			//Pad rest of name with spaces as characters are unused
			*dos_filename++ = 0x20;				//0x20 is used for unused trailing characters
		}
	}

	//DUMP THE '.'
	*source_filename++;

	//----- DO THE EXTENSION -----
	no_null_found_yet = 1;
	asterix_wildcard_used = 0;
	for (temp = 0; temp < 3; temp++)
	{
		//Flag if filename contains a wildcard character (for return)
		if ((*source_filename == '?') || (*source_filename == '*'))
			wildcard_character_found = 1;

		//Check for asterix wildcard character
		if (*source_filename == '*')
			asterix_wildcard_used = 1;

		//Check for terminating null
		if (*source_filename == 0x00)
			no_null_found_yet = 0;

		//COPY THE CHARACTER
		if (asterix_wildcard_used)
		{
			//Doing asterix wildcard replace
			*dos_extension++ = '?';
			if (no_null_found_yet)
				*source_filename++;
		}
		else if (no_null_found_yet)
		{
			//Copy the character
			*dos_extension++ = *source_filename++;
		}
		else
		{
			//Pad rest of name with spaces as characters are unused
			*dos_extension++ = 0x20;				//0x20 is used for unused trailing characters
		}
	}


	//----- CONVERT TO UPPERCASE -----
	dos_filename -= 8;
	dos_extension -= 3;

	for (temp = 0; temp < 8; temp++)
	{
		if ((dos_filename[temp] >= 0x61) && (dos_filename[temp] <= 0x7a))
			dos_filename[temp] -= 0x20;
	}
	for (temp = 0; temp < 3; temp++)
	{
		if ((dos_extension[temp] >= 0x61) && (dos_extension[temp] <= 0x7a))
			dos_extension[temp] -= 0x20;
	}

	return(wildcard_character_found);
}






//***********************************************
//***********************************************
//********** READ NEXT DIRECTORY ENTRY **********
//***********************************************
//***********************************************
//file_name
//	8 character array filename will be written to here
//file_extension
//	3 character array filename extension will be written to here
//attribute_byte
//	file attribute byte will be written to here
//file_size
//	file size will be written to here
//cluster_number
//	start cluster for the file will be written to here
//start_from_beginning
//	set to cause routine to start from 1st directory entry (must be set if the drivers data buffer has been modified since the last call)
//directory_entry_sector
//	Sector that contains the files directory entry will be written to here
//directory_entry_within_sector
//	The file directory entry number within the sector that contains the file will be written to here
//Returns
//	1 = file entry found
//	0 = not found = end of directory
BYTE ffs_read_next_directory_entry (BYTE *file_name, BYTE *file_extension, BYTE *attribute_byte,
									DWORD *file_size, DWORD *cluster_number, BYTE start_from_beginning,
									DWORD *directory_entry_sector, BYTE *directory_entry_within_sector)
{
	BYTE b_temp;
	WORD w_temp;
	DWORD dw_temp;
	BYTE *buffer_pointer;
	static BYTE sectors_left_in_this_cluster;
	static DWORD current_cluster;

	//----- START FROM BEGINNING OF DIRECTORY? -----
	if (start_from_beginning)
	{
		if (disk_is_fat_32)
		{
			//----- FAT32 -----
			read_write_directory_last_lba = (data_area_start_sector + ((root_directory_start_sector_cluster - 2) * sectors_per_cluster) - 1);		//(For FAT32 it contains the start cluster)
			current_cluster = root_directory_start_sector_cluster;
			sectors_left_in_this_cluster = sectors_per_cluster;
		}
		else
		{
			//----- FAT16 -----
			read_write_directory_last_lba = (root_directory_start_sector_cluster - 1);		//(For FAT16 it contains the start sector)
			sectors_left_in_this_cluster = number_of_root_directory_sectors;
		}
		read_write_directory_last_entry = 0xffff;							//Cause the next cluster to be read
	}

	//----- LOAD A NEW SECTOR OF THE DIRECTORY? -----
	if (read_write_directory_last_entry >= ((ffs_bytes_per_sector - 32) >> 5))			// /32 as each directory entry is 32 bytes
	{
		read_write_directory_last_lba++;										//Move to next sector

		//----- CHECK FOR MOVE TO NEXT CLUSTER -----
		if (sectors_left_in_this_cluster == 0)
		{
			//----- NEED TO MOVE TO NEXT CLUSTER -----
			if (disk_is_fat_32)
			{
				//----- FAT32 -----
				//Move to next cluster that contains the next part of the directory
				sectors_left_in_this_cluster = sectors_per_cluster - 1;

				dw_temp = ffs_get_next_cluster_no(current_cluster);
				
				if ((dw_temp & 0x0fffffff) >= 0x0ffffff8)
				{
					//DIRECTORY HAS NO MORE CLUSTERS - ADD A NEW CLUSTER
					dw_temp = ffs_get_next_free_cluster();
					if (dw_temp == 0x0fffffff)
					{
						//No more space to extend the directory
						FFS_CE(1);						//Deselect the card
						return (0);
					}
					ffs_modify_cluster_entry_in_fat(current_cluster, dw_temp);
					ffs_modify_cluster_entry_in_fat(dw_temp, 0x0fffffff);
					current_cluster = dw_temp;
					
					//SET THE CONTENTS OF THE NEW CLUSTER TO 0x00 = all entries unused
					buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];
					for (w_temp = 0; w_temp < 512; w_temp++)
						*buffer_pointer++ = 0x00;

					for (b_temp = 0; b_temp < sectors_per_cluster; b_temp++)
					{
						ffs_write_sector_from_buffer(
													data_area_start_sector + ((current_cluster - 2) * sectors_per_cluster) + b_temp		//(Data on a Partition starts with cluster number 2)
													);			
					}
				}
				else
				{
					current_cluster = dw_temp;
				}

				//Set the address of the next sector we're going to read
				read_write_directory_last_lba = data_area_start_sector + ((current_cluster - 2) * sectors_per_cluster);
			}
			else
			{
				//----- FAT16 -----
				//We've reached the end of the root directory
				FFS_CE(1);						//Deselect the card
				return (0);
			}
		}
		else
		{
			//----- GET NEXT SECTOR OF THIS CLUSTER -----
			sectors_left_in_this_cluster--;
		}

		//Read the next sector to our buffer
		ffs_read_sector_to_buffer (read_write_directory_last_lba);

		read_write_directory_last_entry = 0xffff;
	}


	//----- GET THE NEXT DIRECTORY ENTRY FROM THE BUFFER -----
	read_write_directory_last_entry++;

	//Offset to the start of the entry
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (read_write_directory_last_entry << 5);

	//GET FILE NAME [8]
	file_name[0] = *buffer_pointer++;
	file_name[1] = *buffer_pointer++;
	file_name[2] = *buffer_pointer++;
	file_name[3] = *buffer_pointer++;
	file_name[4] = *buffer_pointer++;
	file_name[5] = *buffer_pointer++;
	file_name[6] = *buffer_pointer++;
	file_name[7] = *buffer_pointer++;

	//Convert to uppercase
	for (b_temp = 0; b_temp < 8; b_temp++)
	{
		if ((file_name[b_temp] >= 0x61) && (file_name[b_temp] <= 0x7a))
			file_name[b_temp] -= 0x20;
	}

	//GET FILENAME EXTENSION [3]
	file_extension[0] = *buffer_pointer++;
	file_extension[1] = *buffer_pointer++;
	file_extension[2] = *buffer_pointer++;

	//Convert to uppercase
	for (b_temp = 0; b_temp < 3; b_temp++)
	{
		if ((file_extension[b_temp] >= 0x61) && (file_extension[b_temp] <= 0x7a))
			file_extension[b_temp] -= 0x20;
	}

	//GET ATTRIBUTE BYTE [1]
	//Bit	7-6					 5		   4		   3			  2		   1		0 
	//		Reserved, set to 0 | Archive | Directory | Volume Label | System | Hidden | Read Only 
	*attribute_byte = *buffer_pointer++;

	//GET NT [1]
	//(Dump)
	buffer_pointer++;

	//GET CREATED TIME (mS portion) [1]
	//(Dump)
	buffer_pointer++;

	//GET CREATED TIME (h + m) [2]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//GET CREATED DATE [2]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//GET LAST ACESSED DATE [2]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//GET EXTENDED ATTRIBUTE [2]
	//0x0000 for FAT16, high word of cluster number for FAT32
	if (disk_is_fat_32)
	{
		*cluster_number = ((DWORD)*buffer_pointer++ << 16);
		*cluster_number += ((DWORD)*buffer_pointer++ << 24);
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
		*cluster_number = 0;
	}

	//GET TIME [2]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//GET DATE [2]
	//(Dump)
	buffer_pointer++;
	buffer_pointer++;

	//GET CLUSTER [2]
	*cluster_number += (DWORD)*buffer_pointer++;
	*cluster_number += ((DWORD)*buffer_pointer++ << 8);
			
	//GET FILE SIZE [4]
	*file_size = (DWORD)*buffer_pointer++;
	*file_size += ((DWORD)*buffer_pointer++ << 8);
	*file_size += ((DWORD)*buffer_pointer++ << 16);
	*file_size += ((DWORD)*buffer_pointer++ << 24);

	//Return the location of this directory entry
	*directory_entry_sector = read_write_directory_last_lba;
	*directory_entry_within_sector = read_write_directory_last_entry;

	FFS_CE(1);						//Deselect the card
	return(1);
}






//********************************************************
//********************************************************
//********** OVERWRITE LAST DIRECTORY FILE NAME **********
//********************************************************
//********************************************************
//file_name = 8 character array filename (must be DOS compatible - uppercase and any trailing unused characters set to 0x20)
//file_extension = 3 character array filename extension (must be DOS compatible - uppercase and any trailing unused characters set to 0x20)
//attribute_byte = file attribute byte
//file_size = file size
//cluster_number = start cluster for the file
//write_time_date = Bit0 = overwrite created. Bit1 = overwrite accessed, Bit2 = overwrite modified
void ffs_overwrite_last_directory_entry (BYTE *file_name, BYTE *file_extension, BYTE *attribute_byte,
												DWORD *file_size, DWORD *cluster_number, BYTE write_time_date)
{
	BYTE *buffer_pointer;
#ifdef USE_REAL_TIME_CLOCK
	WORD_VAL w_temp;
#endif

	//----- WRITE THE NEW ENTRY TO THE BUFFER -----
	//Offset to the start of the entry
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (read_write_directory_last_entry << 5);

	//WRITE FILE NAME [8]
	*buffer_pointer++ = file_name[0];
	*buffer_pointer++ = file_name[1];
	*buffer_pointer++ = file_name[2];
	*buffer_pointer++ = file_name[3];
	*buffer_pointer++ = file_name[4];
	*buffer_pointer++ = file_name[5];
	*buffer_pointer++ = file_name[6];
	*buffer_pointer++ = file_name[7];

	//WRITE FILENAME EXTENSION [3]
	*buffer_pointer++ = file_extension[0];
	*buffer_pointer++ = file_extension[1];
	*buffer_pointer++ = file_extension[2];

	//WRITE ATTRIBUTE BYTE [1]
	//Bit	7-6					 5		   4		   3			  2		   1		0 
	//		Reserved, set to 0 | Archive | Directory | Volume Label | System | Hidden | Read Only 
	*buffer_pointer++ = *attribute_byte;

	//WRITE NT [1]
	*buffer_pointer++ = 0x00;

	//WRITE CREATED TIME (fine resolution x 10ms, range 0 - 199) [1]
	if (write_time_date & 0x01)
	{
		#ifdef USE_REAL_TIME_CLOCK
			*buffer_pointer++ = (FFS_RTC_SECONDS & 0x01) * 100;
		#else
			*buffer_pointer++ = 0x00;
		#endif
	}
	else
	{
		buffer_pointer++;
	}

	//WRITE CREATED TIME (h + m) [2]
	//Bits	15-11								10-5								  4-0 
	//		Hours - valid from 0-23 inclusive | Minutes - valid from 0-59 inclusive | Seconds / 2 - valid from 0-29 inclusive (0-58 seconds, use 0x0d for the extra resolution). 
	if (write_time_date & 0x01)
	{
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = ((WORD)FFS_RTC_HOURS << 11) | ((WORD)FFS_RTC_MINUTES << 5) | ((WORD)FFS_RTC_SECONDS >> 1);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			*buffer_pointer++ = 0x00;
			*buffer_pointer++ = 0x00;
		#endif
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
	}

	//WRITE CREATED DATE [2]
	//Bits	15-9													    8-5													 4-0 
	//		Years since 1980 - valid from 0-127 inclusive (1980-2107) | Month of year - valid from 1-12 inclusive(1 = Jan) | Day of the month - valid from 1-31 inclusive. 
	if (write_time_date & 0x01)
	{
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = (((WORD)FFS_RTC_YEAR + 20) << 9) | ((WORD)FFS_RTC_MONTH << 5) | ((WORD)FFS_RTC_DATE);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			*buffer_pointer++ = 0x21;			//Hard coded to 1st Jan 2000
			*buffer_pointer++ = 0x28;
		#endif
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
	}

	//WRITE LAST ACESSED DATE [2]
	//Bits	15-9													    8-5													 4-0 
	//		Years since 1980 - valid from 0-127 inclusive (1980-2107) | Month of year - valid from 1-12 inclusive(1 = Jan) | Day of the month - valid from 1-31 inclusive. 
	if (write_time_date & 0x02)
	{
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = (((WORD)FFS_RTC_YEAR + 20) << 9) | ((WORD)FFS_RTC_MONTH << 5) | ((WORD)FFS_RTC_DATE);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			*buffer_pointer++ = 0x21;			//Hard coded to 1st Jan 2000
			*buffer_pointer++ = 0x28;
		#endif
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
	}

	//WRITE EXTENDED ATTRIBUTE [2]
	*buffer_pointer++ = (BYTE)(*cluster_number >> 16);		//0x0000 for FAT16, high word of cluster number for FAT32
	*buffer_pointer++ = (BYTE)(*cluster_number >> 24);

	//LAST MODIFIED TIME [2]
	//Bits	15-11								10-5								  4-0 
	//		Hours - valid from 0-23 inclusive | Minutes - valid from 0-59 inclusive | Seconds / 2 - valid from 0-29 inclusive (0-58 seconds, use 0x0d for the extra resolution). 
	if (write_time_date & 0x04)
	{
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = ((WORD)FFS_RTC_HOURS << 11) | ((WORD)FFS_RTC_MINUTES << 5) | ((WORD)FFS_RTC_SECONDS >> 1);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			*buffer_pointer++ = 0x00;
			*buffer_pointer++ = 0x00;
		#endif
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
	}

	//LAST MODIFIED DATE [2]
	//Bits	15-9													    8-5													 4-0 
	//		Years since 1980 - valid from 0-127 inclusive (1980-2107) | Month of year - valid from 1-12 inclusive(1 = Jan) | Day of the month - valid from 1-31 inclusive. 
	if (write_time_date & 0x04)
	{
		#ifdef USE_REAL_TIME_CLOCK
			w_temp.val = (((WORD)FFS_RTC_YEAR + 20) << 9) | ((WORD)FFS_RTC_MONTH << 5) | ((WORD)FFS_RTC_DATE);
			*buffer_pointer++ = w_temp.v[0];
			*buffer_pointer++ = w_temp.v[1];
		#else
			*buffer_pointer++ = 0x21;			//Hard coded to 1st Jan 2000
			*buffer_pointer++ = 0x28;
		#endif
	}
	else
	{
		buffer_pointer++;
		buffer_pointer++;
	}

	//WRITE CLUSTER [2]
	*buffer_pointer++ = (BYTE)*cluster_number;
	*buffer_pointer++ = (BYTE)(*cluster_number >> 8);
			
	//WRITE FILE SIZE [4]
	*buffer_pointer++ = (BYTE)*file_size;
	*buffer_pointer++ = (BYTE)(*file_size >> 8);
	*buffer_pointer++ = (BYTE)(*file_size >> 16);
	*buffer_pointer++ = (BYTE)(*file_size >> 24);

	//WRITE THE BUFFER BACK TO THE DISK SECTOR
	ffs_write_sector_from_buffer(read_write_directory_last_lba);
	FFS_CE(1);						//Deselect the card
}






//********************************************
//********************************************
//********** GET FILE START CLUSTER **********
//********************************************
//********************************************
DWORD get_file_start_cluster(FFS_FILE *file_pointer)
{
	DWORD start_cluster;
	BYTE *buffer_pointer;

	//-----------------------------------
	//----- ENSURE THE FILE IS OPEN -----
	//-----------------------------------
	if (file_pointer->flags.bits.file_is_open == 0)
		return(1);

	//---------------------------------------------------
	//----- READ THE DIRECTORY SECTOR TO THE BUFFER -----
	//---------------------------------------------------
	ffs_read_sector_to_buffer (file_pointer->directory_entry_sector);


	//Set pointer to start of extended attribute in the directory entry
	buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + ((WORD)file_pointer->directory_entry_within_sector << 5) + 20;

	if (disk_is_fat_32)
	{
		start_cluster = (DWORD)*buffer_pointer++ << 16;			//0x0000 for FAT16, high word of cluster number for FAT32
		start_cluster += (DWORD)*buffer_pointer++ << 24;
		buffer_pointer += 4;
	}
	else
	{
		start_cluster = 0;
		buffer_pointer += 6;
	}

	start_cluster += (DWORD)*buffer_pointer++;					//Cluster
	start_cluster += (DWORD)*buffer_pointer++ << 8;


	return(start_cluster);
}






//*************************************
//*************************************
//********** CREATE NEW FILE **********
//*************************************
//*************************************
//
//file_name
//	8 character array filename will be read from here
//write_file_start_cluster
//	start cluster for the file will be read from here
//directory_entry_sector
//	Sector that contains the files directory entry will be written to here
//directory_entry_within_sector
//	The file directory entry number within the sector that contains the file will be written to here
//
//Return value
//	1 = successful
//	0 = failed
BYTE ffs_create_new_file (const char *file_name, DWORD *write_file_start_cluster, DWORD *directory_entry_sector, BYTE *directory_entry_within_sector)
{
	DWORD file_size;
	BYTE converted_file_name[8];
	BYTE converted_file_extension[3];
	BYTE read_file_name[8];
	BYTE read_file_extension[3];
	BYTE read_attribute_byte;
	DWORD read_file_size;
	DWORD read_cluster_number;
	BYTE attribute_byte;

	//CHECK CARD IS INSERTED AND HAS BEEN INITIALISED
	if(ffs_card_ok == 0)
		return(0);

	//----- CHECK FILE NAME DOES NOT ALREADY EXIST -----
	//Disabled to avoid wasted time in large directories as this should not be possbile because fopen deletes the file first if it exists


	//----- CONVERT FILE NAME TO 8 CHARACTER DOS FILENAME -----
	if (ffs_convert_filename_to_dos (file_name, converted_file_name, converted_file_extension))		//Returns 1 if filename had wildcard characters
		return(0);

	//----- FIND THE NEXT EMPTY CLUSTER TO USE FOR THE FILE
	*write_file_start_cluster = ffs_get_next_free_cluster();
	if (*write_file_start_cluster == 0xffffffff)			//0xffffffff = no empty cluster found
	{
		//No cluster available - disk is full
		FFS_CE(1);						//Deselect the card
		return(0);
	}

	//----- FIND EMPTY DIRECTORY ENTRY TO USE FOR THE FILE -----
	//Get the first directory entry
	if (ffs_read_next_directory_entry (&read_file_name[0], &read_file_extension[0], &read_attribute_byte, &file_size,
										&read_cluster_number, 1, directory_entry_sector, directory_entry_within_sector) == 0)			//1 = start from beginning of directory
	{
		//Error - should not be able to happen
		FFS_CE(1);						//Deselect the card
		return(0);						//Reached end of directory
	}

	//Get each entry until we find an empty entry
	//(If 1st value is 0xe5 or 0x00 then entry is available - 0xe5 = deleted file, 0 = unused entry)
	while ((read_file_name[0] != 0xe5) && (read_file_name[0] != 0x00))
	{
		//GET NEXT ENTRY
		if (ffs_read_next_directory_entry (&read_file_name[0], &read_file_extension[0], &read_attribute_byte, &file_size,
											&read_cluster_number, 0, directory_entry_sector, directory_entry_within_sector) == 0)
		{
			//Directory is full - no space for another entry
			FFS_CE(1);						//Deselect the card
			return(0);						//Reached end of directory
		}
	}

	//----- STORE FILE ENTRY IN DIRECTORY -----
	read_file_size = 0;
	attribute_byte = 0;
	ffs_overwrite_last_directory_entry (converted_file_name, converted_file_extension, &attribute_byte, &read_file_size, write_file_start_cluster, 0x07);

	//----- STORE END OF FILE MARKER FOR THE CLUSTER ENTRY IN THE FAT TABLE -----
	ffs_modify_cluster_entry_in_fat(*write_file_start_cluster, 0x0fffffff);



	FFS_CE(1);						//Deselect the card
	return(1);
}






//*******************************************
//*******************************************
//********** FIND NEXT FREE CLUSTER *********
//*******************************************
//*******************************************
//Find the next free cluster in the FAT table
//Returns the cluster number, or 0xffffffff if no free cluster found (card full)
DWORD ffs_get_next_free_cluster (void)
{
	WORD w_data;
	DWORD dw_data;
	DWORD lba;
	DWORD dw_count;
	DWORD dw_count1;
	DWORD fat_entries_per_sector;
	DWORD next_free_cluster;
	BYTE *buffer_pointer;


	//----- START READING THE FAT TABLE FROM THE LAST ENTRY WHERE WE FOUND A FREE CLUSTER -----
	lba = fat1_start_sector;

	if (disk_is_fat_32)
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 2);	//FAT32 - Divide no of bytes per sector by 4 as each fat entry is 1 double word
	else
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 1);	//FAT16 - Divide no of bytes per sector by 2 as each fat entry is 1 word

	//Move to the sector the last free cluster was found in
	//(To avoid having to search from the start each time which could waste a lot of time)
	dw_count = (last_found_free_cluster / fat_entries_per_sector);
	lba += dw_count;
	next_free_cluster = (dw_count * fat_entries_per_sector);
	

	//----- SEARCH FOR THE NEXT FREE CLUSTER -----
	//Incrementally read each sector of the FAT table
	for (dw_count = (lba - fat1_start_sector); dw_count < (sectors_per_fat - 1); dw_count++)	//We use (sectors_per_fat - 1) to avoid the need to calculate the total number of sectors per fat
	{																							//(this saves code space and should only result in a relatively small amount of wasted clusters)
		#ifdef CLEAR_WATCHDOG_TIMER
			CLEAR_WATCHDOG_TIMER;
		#endif

		//Read the next sector
		ffs_read_sector_to_buffer(lba);
		buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0];

		//Check each entry in the sector
		for (dw_count1 = 0; dw_count1 < fat_entries_per_sector; dw_count1++)
		{

			if (disk_is_fat_32)
			{
				dw_data = (DWORD)*buffer_pointer++;
				dw_data |= (DWORD)(*buffer_pointer++) << 8;
				dw_data |= (DWORD)(*buffer_pointer++) << 16;
				dw_data |= (DWORD)(*buffer_pointer++) << 24;
				dw_data &= 0x0fffffff;							//The top 4 bits are reserved
				if (dw_data == 0)								//0x00000000 = free cluster
					goto ffs_get_next_free_cluster_found;
			}
			else
			{
				w_data = (DWORD)*buffer_pointer++;
				w_data |= (DWORD)(*buffer_pointer++) << 8;
				if (w_data == 0)								//0x0000 = free cluster
					goto ffs_get_next_free_cluster_found;
			}
			next_free_cluster++;	

			//Check for overrun into unavailable clusters (the values below denote special meanings in the fat table)
			if (disk_is_fat_32)
			{
				if (next_free_cluster >= 0x0ffffff7)
					return(0xffffffff);
			}
			else
			{
				if (next_free_cluster >= 0x0000fff7)
					return(0xffffffff);
			}
		}
		lba++;
	}

	//---------------------------------------------------
	//----- THERE IS NO FREE CLUSTER - DISK IS FULL -----
	//---------------------------------------------------
	return(0xffffffff);


ffs_get_next_free_cluster_found:
	//---------------------------------------
	//----- FOUND THE NEXT FREE CLUSTER -----
	//---------------------------------------
	last_found_free_cluster = next_free_cluster;			//Store ready for the next time we search

	return(next_free_cluster);
}






//*********************************************
//*********************************************
//********** GET NEXT CLUSTER NUMBER **********
//*********************************************
//*********************************************
//Get the next cluster number from the FAT table
DWORD ffs_get_next_cluster_no (DWORD current_cluster)
{
	DWORD fat_entries_per_sector;
	DWORD lba;
	DWORD dw_temp;
	BYTE *buffer_pointer;

	lba = fat1_start_sector;

	if (disk_is_fat_32)
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 2);		//FAT32 - Divide no of bytes per sector by 4 as each fat entry is 1 double word
	else
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 1);		//FAT16 - Divide no of bytes per sector by 2 as each fat entry is 1 word


	//----- MOVE TO THE SECTOR IN THE FAT TABLE FOR THE CURRENT CLUSTER NUMBER -----
	dw_temp = (current_cluster / fat_entries_per_sector);
	lba += dw_temp;
	current_cluster -= (dw_temp * fat_entries_per_sector);

	ffs_read_sector_to_buffer(lba);


	//----- GET THE NEXT CLUSTER ENTRY FROM THE FAT TABLE -----
	if (disk_is_fat_32)
	{
		buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (current_cluster << 2);

		lba = (DWORD)*buffer_pointer++;									//FAT32 - 1 double word per entry
		lba |= ((DWORD)(*buffer_pointer++) << 8);
		lba |= ((DWORD)(*buffer_pointer++) << 16);
		lba |= ((DWORD)((*buffer_pointer++) & 0x0f) << 24);				//Top nibble reserved bits need to be removed
	}
	else
	{
		buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (current_cluster << 1);

		lba = (DWORD)*buffer_pointer++;
		lba |= ((DWORD)(*buffer_pointer++) << 8);						//FAT16 - 1 word per entry
	}
	return (lba);
}






//********************************************************
//********************************************************
//********** MODIFY CLUSTER VALUE IN FAT TABLES **********
//********************************************************
//********************************************************
//The driver data buffer is used by this function
void ffs_modify_cluster_entry_in_fat (DWORD cluster_to_modify, DWORD cluster_entry_new_value)
{
	DWORD lba;
	BYTE *buffer_pointer;
	BYTE count;
	DWORD fat_entries_per_sector;
	BYTE temp;
	DWORD dw_temp;
	BYTE done_read_of_fat_sector = 0;


	//----- MOVE TO THE SECTOR FOR THIS CLUSTER ENTRY IN THE FAT1 TABLE -----
	lba = fat1_start_sector;

	if (disk_is_fat_32)
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 2);		//FAT32 - Divide no of bytes per sector by 4 as each fat entry is 1 double word
	else
		fat_entries_per_sector = (DWORD)(ffs_bytes_per_sector >> 1);		//FAT16 - Divide no of bytes per sector by 2 as each fat entry is 1 word

	//Move to the relevant sector in the FAT table
	dw_temp = (cluster_to_modify / fat_entries_per_sector);
	lba += dw_temp;
	cluster_to_modify -= (dw_temp * fat_entries_per_sector);

	//----- MODIFY EACH FAT TABLE -----
	for (count = 0x01; count < 0x10; count <<= 1)
	{
		
		if (count & active_fat_table_flags)									//Only modify FAT tables that are active
		{
			if (done_read_of_fat_sector == 0)						//We read the first FAT table sector, alter it and then write it back and to the other FAT tables.
			{														//This isn't perfect as ideally we would read each sector from each FAT table in case there was an
				done_read_of_fat_sector = 1;						//error in the first table.  However this has a significant impact on speed and would stop our
				ffs_read_sector_to_buffer(lba);						//fast file delete aproach of only writing a FAT sector when we are moving to the next sector from
																	//working so its worth doing this and accepting that any error in the first FAT table will be
				if (disk_is_fat_32)									//carried accross to other tables which is very unlikely (and anyway an error has then already occured)
					buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (cluster_to_modify << 2);
				else
					buffer_pointer = &FFS_DRIVER_GEN_512_BYTE_BUFFER[0] + (cluster_to_modify << 1);
	
				*buffer_pointer++ = (BYTE)(cluster_entry_new_value & 0x000000ff);
				*buffer_pointer++ = (BYTE)((cluster_entry_new_value & 0x0000ff00) >> 8);
				if (disk_is_fat_32)
				{
					*buffer_pointer++ = (BYTE)((cluster_entry_new_value & 0x00ff0000) >> 16);
					temp = (*buffer_pointer & 0xf0);
					*buffer_pointer++ = ((BYTE)((cluster_entry_new_value & 0x0f000000) >> 24) | temp);		//The top 4 bits are reserved and should not be modified
				}
			}
			ffs_write_sector_from_buffer (lba);
		}
		lba += sectors_per_fat;												//Move to next FAT table
	}


}















































