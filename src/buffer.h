#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} Buffer;

// Initialize a buffer
void init_buffer(Buffer *buf) {
    buf->buffer = (char *)malloc(BUFFER_SIZE);
    buf->size = 0;
    buf->capacity = BUFFER_SIZE;
}

// Append to a buffer (dynamically resize if needed)
void append_to_buffer(Buffer *buf, const char *entry) {
    size_t entry_len = strlen(entry);
    if (buf->size + entry_len >= buf->capacity) {
        buf->capacity *= 2;
        buf->buffer = (char *)realloc(buf->buffer, buf->capacity);
    }
    strcpy(buf->buffer + buf->size, entry);
    buf->size += entry_len;
}

// Flush buffer contents to file
void flush_buffer(FILE *file, Buffer *buf) {
    fwrite(buf->buffer, 1, buf->size, file);
}
