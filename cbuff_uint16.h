/* Generated from cbuff_uint8.h by generate.sh -- do not edit directly */
/*
    Circular Buffer For Embedded Applications
    Author: Brian Khuu (2025)

    Malloc-free, lock-free single-producer/single-consumer circular buffer.
    Uses index-based head/tail and a virtual index range (0..2*capacity) to
    avoid the sentinel slot. See DESIGN.md for the technical rationale.

    Thread safety: head and tail are volatile — correct for single-core
    embedded use (one ISR writing, main loop reading). NOT sufficient for
    multi-core; use _Atomic or platform memory barriers there.

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
#ifndef CBUFF_UINT16_H
#define CBUFF_UINT16_H

#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdint.h>  // uint16_t

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

// Prefill Circular Buffer (Allows for skipping `cbuff_uint16_init()`)
#define cbuff_uint16_struct_full_prefill(BuffSize, BuffPtr)                                                                                                                                             \
    {                                                                                                                                                                                                  \
        .capacity = BuffSize, .buffer = BuffPtr, .head = 0, .tail = 0                                                                                                                      \
    }
#define cbuff_uint16_struct_prefill(Buff) cbuff_uint16_struct_full_prefill((sizeof(Buff) / sizeof(Buff[0])), &Buff[0])

typedef struct cbuff_uint16_t
{
    size_t capacity;      ///< Maximum number of items in the buffer
    uint16_t *buffer;      ///< Data Buffer
    volatile size_t head; ///< Head Index (input)
    volatile size_t tail; ///< Tail Index (output)
} cbuff_uint16_t;

/*******************************************************************************
 * Init/IsInit/Reset
 *******************************************************************************/

static inline bool cbuff_uint16_init(cbuff_uint16_t *cb, size_t capacity, uint16_t *buffPtr)
{
    cbuff_uint16_t emptyCB = {0};
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

static inline bool cbuff_uint16_is_init(cbuff_uint16_t *cb) { return cb->capacity && cb->buffer; }

static inline bool cbuff_uint16_reset(cbuff_uint16_t *cb)
{
    cb->head = 0;
    cb->tail = 0;
    return true; ///< Successful
}

/*******************************************************************************
 * Circular buffer Enqueue/Dequeue (This will modify the buffer)
 *******************************************************************************/

static inline bool cbuff_uint16_enqueue_overwrite(cbuff_uint16_t *cb, const uint16_t b)
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

static inline bool cbuff_uint16_enqueue(cbuff_uint16_t *cb, const uint16_t b)
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

static inline bool cbuff_uint16_dequeue(cbuff_uint16_t *cb, uint16_t *b)
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
 * Circular buffer Peek (Will Not Modify Buffer)
 *******************************************************************************/

static inline bool cbuff_uint16_peek(cbuff_uint16_t *cb, uint16_t *b, const size_t offset)
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
 * Circular buffer utility functions (Will Not Modify Buffer)
 *******************************************************************************/

static inline size_t cbuff_uint16_capacity(cbuff_uint16_t *cb) { return cb->capacity; }

static inline size_t cbuff_uint16_count(cbuff_uint16_t *cb)
{
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    return count; 
}

static inline bool cbuff_uint16_is_full(cbuff_uint16_t *cb)
{
    const size_t head_index = cb->head;
    const size_t tail_index = cb->tail;
    const size_t count = (tail_index >= head_index) ? (tail_index - head_index) : (tail_index + (cb->capacity * 2 - head_index));
    return count >= cb->capacity; 
}

static inline bool cbuff_uint16_is_empty(cbuff_uint16_t *cb) { return cb->tail == cb->head; }

#endif
