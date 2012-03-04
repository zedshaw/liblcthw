#undef NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>

RingBuffer *RingBuffer_create(int length)
{
    RingBuffer *buffer = calloc(1, sizeof(RingBuffer));
    buffer->length  = length + 1;
    buffer->start = 0;
    buffer->end   = 0;
    buffer->buffer = calloc(buffer->length, 1);

    return buffer;
}

void RingBuffer_destroy(RingBuffer *buffer)
{
    if(buffer) {
        free(buffer->buffer);
        free(buffer);
    }
}

int RingBuffer_full(RingBuffer *buffer)
{
    return (buffer->end + 1) % buffer->length == buffer->start;
}

int RingBuffer_empty(RingBuffer *buffer)
{
    return buffer->end == buffer->start;
}

int RingBuffer_available_space(RingBuffer *buffer)
{
    return buffer->length - buffer->end - 1;
}

int RingBuffer_available_data(RingBuffer *buffer)
{
    return buffer->length - buffer->start - 1;
}

int RingBuffer_write(RingBuffer *buffer, char *data, int length)
{
    if(length > RingBuffer_available_space(buffer)) {
        return -1;
    }

    void *result = memcpy(buffer->buffer + buffer->end, data, length);
    check(result != NULL, "Failed to write data into buffer.");

    buffer->end = (buffer->end + length) % buffer->length;

    if (buffer->end == buffer->start) {
        buffer->start = (buffer->start + length) % buffer->length;
    }

    return length;
error:
    return -1;
}

int RingBuffer_read(RingBuffer *buffer, char *target, int amount)
{
    if(amount > RingBuffer_available_data(buffer)) {
        debug("Available is: %d, amount: %d", RingBuffer_available_space(buffer), amount);
        return -1;
    }

    void *result = memcpy(target, buffer->buffer + buffer->start, amount);
    check(result != NULL, "Failed to write buffer into data.");

    buffer->start = (buffer->start + amount) % buffer->length;

    return amount;
error:
    return -1;
}

