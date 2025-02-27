/*
    Circular Byte Buffer For Embedded Applications (Index Based)
    Author: Brian Khuu (2025)

    Reason: Malloc free, minimum overhead implementation of a circular buffer.
    Static inlined handlers for speed and ease of usage in various projects.
    Index based implementation diverges from harshkn's version, since it is
    easier for me to grok. However may come at cost of speed and optimisation.
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
#ifndef CBUFF_INDEX_H
#define CBUFF_INDEX_H

#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdint.h>  // uint8_t

// Prefill Circular Buffer (Allows for skipping `cbuff_Init()`)
#define cbuff_struct_full_prefill(BuffSize, BuffPtr) {.capacity = BuffSize, .count = 0, .buffer = BuffPtr, .head = 0, .tail = 0}
#define cbuff_struct_prefill(Buff) cbuff_struct_full_prefill((sizeof(Buff) / sizeof(Buff[0])), &Buff[0])

typedef struct cbuff_t
{
    size_t capacity; ///< Maximum number of items in the buffer
    size_t count;    ///< Number of items in the buffer
    uint8_t *buffer; ///< Data Buffer
    size_t head;     ///< Head Index
    size_t tail;     ///< Tail Index
} cbuff_t;

/*******************************************************************************
 * Init/IsInit/Reset
 *******************************************************************************/

static inline bool cbuff_init(cbuff_t *cb, size_t capacity, uint8_t *buffPtr)
{
    cbuff_t emptyCB = {0};
    if ((cb == NULL) || (buffPtr == NULL))
    {
        return false; ///< Failed
    }
    // Init Struct
    *cb = emptyCB;
    cb->capacity = capacity;
    cb->buffer = buffPtr;
    cb->count = 0;
    cb->head = 0;
    cb->tail = 0;
    return true; ///< Successful
}

static inline bool cbuff_is_init(cbuff_t *cb) { return cb->capacity && cb->buffer; }

static inline bool cbuff_reset(cbuff_t *cb)
{
    cb->count = 0;
    cb->head = 0;
    cb->tail = 0;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Enqueue/Dequeue (This will modify the buffer)
 *******************************************************************************/

static inline bool cbuff_enqueue_overwrite(cbuff_t *cb, const uint8_t b)
{
    if (cb->count >= cb->capacity)
    {
        // Full. Increment head
        cb->head = (cb->head + 1) % cb->capacity;
    }
    else
    {
        // Not Full. Update Counter
        cb->count = cb->count + 1;
    }
    // Push value
    cb->buffer[cb->tail] = b;
    // Increment tail
    cb->tail = (cb->tail + 1) % cb->capacity;
    return true; ///< Successful
}

static inline bool cbuff_enqueue(cbuff_t *cb, const uint8_t b)
{
    // Full
    if (cb->count >= cb->capacity)
    {
        return false; ///< Failed
    }
    // Push value
    cb->buffer[cb->tail] = b;
    // Increment tail
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count = cb->count + 1;
    return true; ///< Successful
}

static inline bool cbuff_dequeue(cbuff_t *cb, uint8_t *b)
{
    // Empty
    if (cb->count == 0)
    {
        return false; ///< Failed
    }
    // Pop value
    *b = cb->buffer[cb->head];
    // Increment head
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count = cb->count - 1;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Peek (Will Not Modify Buffer)
 *******************************************************************************/

static inline bool cbuff_peek(cbuff_t *cb, uint8_t *b, const size_t offset)
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
    *b = cb->buffer[(cb->head + offset) % cb->capacity];
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer utility functions (Will Not Modify Buffer)
 *******************************************************************************/

static inline size_t cbuff_capacity(cbuff_t *cb) { return cb->capacity; }

static inline size_t cbuff_count(cbuff_t *cb) { return cb->count; }

static inline bool cbuff_is_full(cbuff_t *cb) { return (cb->count >= cb->capacity); }

static inline bool cbuff_is_empty(cbuff_t *cb) { return (cb->count == 0); }

#endif
