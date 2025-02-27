/*
    Circular Byte Buffer For Embedded Applications (Pointer Based)
    Author: Brian Khuu (2025)

    Reason: Malloc free, minimum overhead implementation of a circular buffer.
      Static inlined handlers for speed and ease of usage in various projects.
      Pointer based implementation approach kept from harshkn's version
      as it may be faster than index deferencing approach.
      Also uses byte based rather than item based for easier understability
      when used for simpler byte array circular buffers.

    Based on harshkn's circular buffer: https://gist.github.com/harshkn/909546

    # MIT License

    Copyright (c) 2025 Brian Khuu

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
#ifndef CBUFF_POINTER_H
#define CBUFF_POINTER_H

#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdint.h>  // uint8_t

// Prefill Circular Buffer (Allows for skipping `cbuff_uint8_init()`)
#define cbuff_uint8_struct_full_prefill(BuffSize, BuffPtr) {.capacity = BuffSize, .count = 0, .buffer = BuffPtr, .bufferEnd = BuffPtr + BuffSize, .head = BuffPtr, .tail = BuffPtr}
#define cbuff_uint8_struct_prefill(Buff) cbuff_uint8_struct_full_prefill((sizeof(Buff) / sizeof(Buff[0])), &Buff[0])

typedef struct cbuff_uint8_t
{
    size_t capacity;    ///< Maximum number of bytes in the buffer
    size_t count;       ///< Number of bytes in the buffer
    uint8_t *buffer;    ///< Data Buffer
    uint8_t *bufferEnd; ///< Data Buffer end marker (1 item beyond buffer)
    uint8_t *head;      ///< Pointer to head
    uint8_t *tail;      ///< Pointer to tail
} cbuff_uint8_t;

/*******************************************************************************
 * Init/IsInit/Reset
 *******************************************************************************/

static inline bool cbuff_uint8_init(cbuff_uint8_t *cb, size_t capacity, uint8_t *buffPtr)
{
    if ((cb == NULL) || (buffPtr == NULL))
    {
        return false; ///< Failed
    }
    // Init Struct
    cb->capacity = capacity;
    cb->count = 0;
    cb->buffer = buffPtr;
    cb->bufferEnd = buffPtr + capacity;
    cb->head = buffPtr;
    cb->tail = buffPtr;
    return true; ///< Successful
}

static inline bool cbuff_uint8_is_init(cbuff_uint8_t *cb) { return cb->capacity && cb->buffer; }

static inline bool cbuff_uint8_reset(cbuff_uint8_t *cb)
{
    cb->count = 0;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Enqueue/Dequeue (This will modify the buffer)
 *******************************************************************************/

static inline bool cbuff_uint8_enqueue_overwrite(cbuff_uint8_t *cb, const uint8_t b)
{
    if (cb->count >= cb->capacity)
    {
        // Full. Increment head
        cb->head += 1;
        cb->head = (cb->head == cb->bufferEnd) ? cb->buffer : cb->head;
    }
    else
    {
        // Not Full. Update Counter
        cb->count = cb->count + 1;
    }
    // Write
    *(cb->tail) = b;
    // Increment tail
    cb->tail += 1;
    cb->tail = (cb->tail == cb->bufferEnd) ? cb->buffer : cb->tail;
    return true; ///< Successful
}

static inline bool cbuff_uint8_enqueue(cbuff_uint8_t *cb, const uint8_t b)
{
    // Full?
    if (cb->count >= cb->capacity)
    {
        return false; ///< Failed
    }
    // Write
    *(cb->tail) = b;
    // Increment tail
    cb->tail += 1;
    cb->tail = (cb->tail == cb->bufferEnd) ? cb->buffer : cb->tail;
    cb->count = cb->count + 1;
    return true; ///< Successful
}

static inline bool cbuff_uint8_dequeue(cbuff_uint8_t *cb, uint8_t *b)
{
    // Empty?
    if (cb->count == 0)
    {
        return false; ///< Failed
    }
    // Read
    *b = *cb->head;
    // Increment head
    cb->head += 1;
    cb->head = (cb->head == cb->bufferEnd) ? cb->buffer : cb->head;
    cb->count = cb->count - 1;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Peek (Will Not Modify Buffer)
 *******************************************************************************/

static inline bool cbuff_uint8_peek(cbuff_uint8_t *cb, uint8_t *b, const size_t offset)
{
    // Empty?
    if (cb->count == 0)
    {
        return false; ///< Failed
    }
    if (cb->count < offset)
    {
        return false; ///< Failed
    }
    const size_t headIndex = cb->head - cb->buffer;
    *b = cb->buffer[(headIndex + offset) % cb->capacity];
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer utility functions (Will Not Modify Buffer)
 *******************************************************************************/

static inline size_t cbuff_uint8_capacity(cbuff_uint8_t *cb) { return cb->capacity; }

static inline size_t cbuff_uint8_count(cbuff_uint8_t *cb) { return cb->count; }

static inline bool cbuff_uint8_is_full(cbuff_uint8_t *cb) { return (cb->count >= cb->capacity); }

static inline bool cbuff_uint8_is_empty(cbuff_uint8_t *cb) { return (cb->count == 0); }

#endif
