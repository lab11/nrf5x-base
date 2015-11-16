// Most Basic RAM Filesystem

#include <stdint.h>

#define __FILE_defined 0
#define _F_READ 1
#define _F_WRIT 2

typedef struct {
	uint32_t fpos;
	uint8_t flags;
	uint32_t handle;
	uint32_t index;
} FILE;

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MBRAMFS_MAX_FILENAME_LEN  128
#define MBRAMFS_FILESIZE_IN_BYTES 5000
#define MBRAMFS_NUM_FILE_POINTERS 10
#define MBRAMFS_NUM_FILES         2

typedef struct {
	char name[MBRAMFS_MAX_FILENAME_LEN];
	uint8_t buf[MBRAMFS_FILESIZE_IN_BYTES];
	uint32_t len;
} file_buf_t;


// Allocate all files and FILE objects here.
FILE              file_ptrs[MBRAMFS_NUM_FILE_POINTERS] = {{0, 0, 0}};
static file_buf_t files[MBRAMFS_NUM_FILES] = {{{0}, {0}, 0}};

static unsigned short handle_cnt = 1;


FILE* fopen (const char* fname, const char* flags) {
	uint8_t read = 0;
	uint8_t write = 0;
	uint8_t append = 0;

	int file_ptr_index;
	int i;

	// Find an open file handle
	file_ptr_index = -1;
	for (i=0; i<MBRAMFS_NUM_FILE_POINTERS; i++) {
		if (file_ptrs[i].handle == 0) {
			// Found one
			file_ptr_index = i;
			break;
		}
	}

	if (file_ptr_index == -1) {
		// No room
		return NULL;
	}

	// Parse the flags
	i = 0;
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

	// Determine if this file exists
	int file_index = -1;
	for (i=0; i<MBRAMFS_NUM_FILES; i++) {
		if (strncmp(fname, files[i].name, MBRAMFS_MAX_FILENAME_LEN) == 0) {
			file_index = i;
			break;
		}
	}

	// Cannot read from a file that does not exist
	if (read && file_index == -1) {
		return NULL;
	}

	// May need to create new file
	if (file_index == -1) {
		// Find space for it
		for (i=0; i<MBRAMFS_NUM_FILES; i++) {
			if (files[i].name[0] == '\0') {
				// This is free
				file_index = i;
				strncpy(files[i].name, fname, MBRAMFS_MAX_FILENAME_LEN);
				break;
			}
		}
	}

	// If we couldn't find this file or create it, error
	if (file_index == -1) {
		return NULL;
	}

	FILE*       file_ptr = &file_ptrs[file_ptr_index];
	file_buf_t* file = &files[file_index];

	// Save which file this points to
	file_ptr->index = file_index;

	// Writing a file makes it exist
	if (read) {
		file_ptr->fpos = 0;
		file_ptr->flags = _F_READ;
	} else if (write) {
		file->len = 0;
		file_ptr->fpos = 0;
		file_ptr->flags = _F_WRIT;
	} else if (append) {
		file_ptr->fpos = file->len;
		file_ptr->flags = _F_WRIT;
	}

	file_ptr->handle = handle_cnt;
	handle_cnt++;
	return file_ptr;
}

size_t fread (void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t copy_len = size*count;
	uint32_t fptr = (uint32_t) stream->fpos;
	file_buf_t* file = &files[stream->index];

	if (!(stream->flags & _F_READ)) return 0;

	// Make sure we don't read past the end of the file
	if (fptr + copy_len > file->len) {
		copy_len = ((file->len - fptr) / size) * size;
	}

	// memcpy the "file" to the user buffer and return how much we copied
	memcpy(ptr, file->buf+fptr, copy_len);

	stream->fpos += copy_len;
	return copy_len / size;
}

size_t fwrite (const void* ptr, size_t size, size_t count, FILE* stream) {
	uint32_t write_len = size*count;
	uint32_t fptr = (uint32_t) stream->fpos;
	file_buf_t* file = &files[stream->index];

	if (!(stream->flags & _F_WRIT)) return 0;

	// Make sure we don't read off the end of the array
	if (fptr + write_len > MBRAMFS_FILESIZE_IN_BYTES) {
		write_len = ((MBRAMFS_FILESIZE_IN_BYTES - fptr) / size) * size;
	}

	memcpy(file->buf+fptr, ptr, write_len);
	stream->fpos += write_len;
	file->len += write_len;
	return write_len / size;
}

int fseek (FILE* f, long int offset, int origin) {
	uint32_t fptr = (uint32_t) f->fpos;
	file_buf_t* file = &files[f->index];
	uint32_t new_position = 0;
	if (origin == SEEK_SET) {
		// Offset from beginning of file
		new_position = offset;
	} else if (origin == SEEK_CUR) {
		// From current position
		new_position = offset + fptr;
	}

	if (new_position > file->len) {
		// Seek too far, past the end of the "file"
		return -1;
	}

	// Update internal pointer
	f->fpos = new_position;
	return 0;
}

void rewind (FILE* f) {
	// Just need to reset our array index into the file buffer
	f->fpos = 0;
}

int fclose (FILE* stream) {
	stream->handle = 0;
	return 0;
}

// "delete" file by adding it to the allocatable pool
int remove (const char* filename) {
	int i;
	for (i=0; i<MBRAMFS_NUM_FILES; i++) {
		if (strncmp(files[i].name, filename, MBRAMFS_MAX_FILENAME_LEN) == 0) {
			memset(files[i].name, 0, MBRAMFS_MAX_FILENAME_LEN);
			files[i].len = 0;
			break;
		}
	}
	return 0;
}
