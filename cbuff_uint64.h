/* Generated from cbuff_uint8.h by generate.sh -- do not edit directly */
/*
    Circular Buffer For Embedded Applications (Index Based)
    Author: Brian Khuu (2025)

    Reason: Malloc free, minimum overhead implementation of a circular buffer.
    Static inlined handlers for speed and ease of usage in various projects.
    Index based implementation diverges from harshkn's version, since it is
    easier for me to grok. However may come at cost of speed and optimisation.

    Thread safety: head and tail are declared volatile, which is correct for
    single-core embedded use (e.g. one ISR writing, main loop reading). This
    is NOT sufficient for multi-core; use _Atomic or platform memory barriers
    in that case. See PHILOSOPHY.md for the full rationale.

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
#ifndef CBUFF_UINT64_H
#define CBUFF_UINT64_H

#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdint.h>  // uint64_t

/*
 * # Circular Buffer In Memory Representation
 *
 *   0                                      X Capacity
 *  [ ][ ][ ][ ][ ][D][C][B][A][ ][ ][ ][ ][ ]
 *                  |--->>---|
 *                 HEAD     TAIL
 *                INPUT     OUTPUT
 *
 * # Lock Free Approach (No Mutex Thread Safety)
 * 
 * The issue with wrapping the index by X for empty/full detection
 * is that you would need to leave one slot free to disambiguate
 * between the full and empty state.
 *
 *   We can solve this issue by projecting the index beyond X as shown:
 *
 *   0                             X :X+0                           2*X
 *  [B][A][ ][ ][ ][ ][ ][ ][ ][ ][ ]:[ ][ ][ ][ ][ ][ ][ ][ ][ ][D][C]
 *  >---|                            :                            |--->
 *      TAIL                         :MIRRORED                    HEAD
 *      OUTPUT                       :INDEX                       INPUT
 *
 */

// Prefill Circular Buffer (Allows for skipping `cbuff_uint64_init()`)
#define cbuff_uint64_struct_full_prefill(BuffSize, BuffPtr)                                                                                                                                             \
    {                                                                                                                                                                                                  \
        .capacity = BuffSize, .buffer = BuffPtr, .head = 0, .tail = 0                                                                                                                      \
    }
#define cbuff_uint64_struct_prefill(Buff) cbuff_uint64_struct_full_prefill((sizeof(Buff) / sizeof(Buff[0])), &Buff[0])

typedef struct cbuff_uint64_t
{
    size_t capacity;      ///< Maximum number of items in the buffer
    uint64_t *buffer;      ///< Data Buffer
    volatile size_t head; ///< Head Index (input)
    volatile size_t tail; ///< Tail Index (output)
} cbuff_uint64_t;

/*******************************************************************************
 * Init/IsInit/Reset
 *******************************************************************************/

static inline bool cbuff_uint64_init(cbuff_uint64_t *cb, size_t capacity, uint64_t *buffPtr)
{
    cbuff_uint64_t emptyCB = {0};
    if ((cb == NULL) || (buffPtr == NULL))
    {
        return false; ///< Failed
    }
    // Init Struct
    *cb = emptyCB;
    cb->capacity = capacity;
    cb->buffer = buffPtr;
    cb->head = 0;
    cb->tail = 0;
    return true; ///< Successful
}

static inline bool cbuff_uint64_is_init(cbuff_uint64_t *cb) { return cb->capacity && cb->buffer; }

static inline bool cbuff_uint64_reset(cbuff_uint64_t *cb)
{
    cb->head = 0;
    cb->tail = 0;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Enqueue/Dequeue (This will modify the buffer)
 *******************************************************************************/

static inline bool cbuff_uint64_enqueue_overwrite(cbuff_uint64_t *cb, const uint64_t b)
{
    // Snapshot of volatile value to enforce atomicity
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    // Check if buffer is full
    if (count >= cb->capacity)
    {
        // Discard head value
        cb->head = (head_index + 1) % (cb->capacity * 2);
    }
    // Push value
    cb->buffer[tail_index % cb->capacity] = b;
    // Increment tail
    cb->tail = (tail_index + 1) % (cb->capacity * 2);
    return true; ///< Successful
}

static inline bool cbuff_uint64_enqueue(cbuff_uint64_t *cb, const uint64_t b)
{
    // Snapshot of volatile value to enforce atomicity
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    // Check if buffer is full
    if (count >= cb->capacity)
    {
        return false; ///< Failed
    }
    // Push value
    cb->buffer[tail_index % cb->capacity] = b;
    // Increment tail
    cb->tail = (tail_index + 1) % (cb->capacity * 2);
    return true; ///< Successful
}

static inline bool cbuff_uint64_dequeue(cbuff_uint64_t *cb, uint64_t *b)
{
    // Snapshot of volatile value to enforce atomicity
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    // Check if buffer is empty
    if (tail_index == head_index)
    {
        return false; ///< Failed
    }
    // Pop value
    *b = cb->buffer[head_index % cb->capacity];
    // Increment head
    cb->head = (head_index + 1) % (cb->capacity * 2);
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer Peek (Will Not Modify Buffer)
 *******************************************************************************/

static inline bool cbuff_uint64_peek(cbuff_uint64_t *cb, uint64_t *b, const size_t offset)
{
    // Snapshot of volatile value to enforce atomicity
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    // Check if empty
    if (tail_index == head_index)
    {
        return false; ///< Failed
    }
    // Check if offset is within bound of the current buffer count usage
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    if (count <= offset)
    {
        return false; ///< Failed
    }
    // Peek Value
    *b = cb->buffer[(head_index + offset) % cb->capacity];
    return true; ///< Successful
}

/*******************************************************************************
 * Circular byte buffer utility functions (Will Not Modify Buffer)
 *******************************************************************************/

static inline size_t cbuff_uint64_capacity(cbuff_uint64_t *cb) { return cb->capacity; }

static inline size_t cbuff_uint64_count(cbuff_uint64_t *cb)
{
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    return count; 
}

static inline bool cbuff_uint64_is_full(cbuff_uint64_t *cb)
{
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    return count >= cb->capacity; 
}

static inline bool cbuff_uint64_is_empty(cbuff_uint64_t *cb) { return cb->tail == cb->head; }

#endif
