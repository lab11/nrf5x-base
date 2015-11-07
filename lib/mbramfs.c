// Most Basic RAM Filesystem
//
// Supports 1 file


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MBRAMFS_FILESIZE_IN_BYTES 10000

FILE onefile;

static uint8_t  fexists = 0;
static uint8_t  fisopen = 0;
static uint32_t fptr = 0;
static uint8_t  filebuf[MBRAMFS_FILESIZE_IN_BYTES] = {0};
static uint32_t flen = 0;

static uint8_t  read_enabled = 0;
static uint8_t  write_enabled = 0;


FILE* fopen (const char* restrict fname, const char* restrict flags) {
	uint8_t read = 0;
	uint8_t write = 0;
	uint8_t append = 0;

	int i = 0;

	// Parse the flags
	while (flags[i]) {
		if      (flags[i] == 'w') write = 1;
		else if (flags[i] == 'r') read = 1;
		else if (flags[i] == 'a') append = 1;

		i++;
	}

	// Write has priority (?)
	if (write && append) {
		append = 0;
	}

	// Cannot read from a file that does not exist
	if (read && !fexists) return NULL;

	// Writing a file makes it exist
	if (read) {
		fptr = 0;
	} else if (write) {
		fexists = 1;
		flen = 0;
		fptr = 0;
	} else if (append) {
		fexists = 1;
		fptr = flen;
	}

	read_enabled = read;
	write_enabled = write | append;

	fisopen = 1;
	return &onefile;
}

size_t fread (void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t copy_len = size*count;

	if (!fisopen || !read_enabled) return 0;

	// Make sure we don't read past the end of the file
	if (fptr + copy_len > flen) {
		copy_len = flen - fptr;
	}

	// memcpy the "file" to the user buffer and return how much we copied
	memcpy(ptr, filebuf+fptr, copy_len);
	fptr += copy_len;
	return copy_len;
}

size_t fwrite (const void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t write_len = size*count;

	if (!fisopen || !write_enabled) return 0;

	// Make sure we don't read off the end of the array
	if (fptr + write_len > MBRAMFS_FILESIZE_IN_BYTES) {
		write_len = MBRAMFS_FILESIZE_IN_BYTES - fptr;
	}

	memcpy(filebuf+fptr, ptr, write_len);
	fptr += write_len;
	flen += write_len;
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

	if (new_position > flen) {
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
	fisopen = 0;
	return 0;
}

// "delete" file by clearing it
int remove (const char* filename) {
	memset(filebuf, 0, MBRAMFS_FILESIZE_IN_BYTES);
	fptr = 0;
	flen = 0;
	fexists = 0;
	return 0;
}
