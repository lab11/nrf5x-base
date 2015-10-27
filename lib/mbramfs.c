// Most Basic RAM Filesystem
//
// Supports 1 file


#include "stdio.h"
#include "stdint.h"

#define MBRAMFS_FILESIZE_IN_BYTES 10000

FILE onefile;

static uint32_t fptr = 0;
static uint8_t filebuf[MBRAMFS_FILESIZE_IN_BYTES] = {0};


FILE* fopen (const char* restrict fname, const char* restrict flags) {
	return &onefile;
}

size_t fread (void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t copy_len = size*count;

	// Make sure we don't read off the end of the array
	if (fptr + copy_len > MBRAMFS_FILESIZE_IN_BYTES) {
		copy_len = MBRAMFS_FILESIZE_IN_BYTES - fptr;
	}

	// memcpy the "file" to the user buffer and return how much we copied
	memcpy(ptr, filebuf+fptr, copy_len);
	fptr += copy_len;
	return copy_len;
}

size_t fwrite (const void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t write_len = size*count;

	// Make sure we don't read off the end of the array
	if (fptr + write_len > MBRAMFS_FILESIZE_IN_BYTES) {
		write_len = MBRAMFS_FILESIZE_IN_BYTES - fptr;
	}

	memcpy(filebuf+fptr, ptr, write_len);
	fptr += write_len;
	return write_len;
}

int fseek (FILE* f, long int offset, int origin) {
	uint32_t new_position = 0;
	if (origin == SEEK_SET) {
		// Offset from beginning of file
		new_position = offset;
	} else if (origin == SEEK_CUR) {
		// From current position
		new_position = offset + fptr;
	}

	if (new_position > MBRAMFS_FILESIZE_IN_BYTES) {
		// Seek too far, past the end of the "file"
		return -1;
	}

	// Update internal pointer
	fptr = new_position;
	return 0;
}

void rewind (FILE* f) {
	// Just need to reset our array index into the file buffer
	fptr = 0;
}

int fclose (FILE* stream) {
	return 0;
}

// "delete" file by clearing it
int remove (const char* filename) {
	memset(filebuf, 0, MBRAMFS_FILESIZE_IN_BYTES);
	fptr = 0;
}
