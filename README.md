# cbuff.h Circular Buffer For Embedded Applications

<versionBadge>![Version 0.2.0](https://img.shields.io/badge/version-0.2.0-blue.svg)</versionBadge>
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI/CD Status Badge](https://github.com/mofosyne/cbuff.h/actions/workflows/ci.yml/badge.svg)](https://github.com/mofosyne/cbuff.h/actions)

Malloc-free, single-header circular buffer for embedded applications.
Lock-free single-producer/single-consumer design. No RTOS required.

## Available Types

| Header | Element type | Also covers |
|---|---|---|
| `cbuff_uint8.h` | `uint8_t` | `int8_t` (cast) |
| `cbuff_uint16.h` | `uint16_t` | `int16_t` (cast) |
| `cbuff_uint32.h` | `uint32_t` | `int32_t`, `float` (cast) |
| `cbuff_uint64.h` | `uint64_t` | `int64_t`, `double` (cast) |

Copy the header you need into your project. No build system required.

## Quick Start

```c
#include "cbuff_uint8.h"

/* Static allocation — no malloc needed */
static uint8_t buf[64];
static cbuff_uint8_t cb = cbuff_uint8_struct_prefill(buf);

/* Or initialise at runtime */
cbuff_uint8_init(&cb, sizeof(buf), buf);
```

```c
/* Producer — e.g. in a UART ISR */
cbuff_uint8_enqueue(&cb, received_byte);

/* Producer — overwrite oldest data if full */
cbuff_uint8_enqueue_overwrite(&cb, received_byte);
```

```c
/* Consumer — e.g. in main loop */
uint8_t byte;
if (cbuff_uint8_dequeue(&cb, &byte)) {
    process(byte);
}

/* Peek without consuming */
uint8_t byte;
if (cbuff_uint8_peek(&cb, &byte, 0)) { /* offset 0 = oldest item */ }
```

## API

All functions are `static inline`. Replace `uint8` with your chosen type.

| Function | Description |
|---|---|
| `cbuff_uint8_init(cb, capacity, buf)` | Initialise at runtime |
| `cbuff_uint8_is_init(cb)` | True if initialised |
| `cbuff_uint8_reset(cb)` | Clear without zeroing memory |
| `cbuff_uint8_enqueue(cb, val)` | Add item; returns false if full |
| `cbuff_uint8_enqueue_overwrite(cb, val)` | Add item; overwrites oldest if full |
| `cbuff_uint8_dequeue(cb, &val)` | Remove and return oldest item |
| `cbuff_uint8_peek(cb, &val, offset)` | Read at offset without consuming |
| `cbuff_uint8_count(cb)` | Number of items currently held |
| `cbuff_uint8_capacity(cb)` | Maximum items the buffer can hold |
| `cbuff_uint8_is_full(cb)` | True if count == capacity |
| `cbuff_uint8_is_empty(cb)` | True if count == 0 |

## Thread Safety

Safe for **single-producer / single-consumer** on a single core (e.g. one ISR
writing, main loop reading). `head` and `tail` are `volatile` which prevents
the compiler from caching them in registers — correct for this use case.

This is **not** sufficient for multi-core or multi-producer scenarios.
Use `_Atomic` (C11) or platform memory barriers there, or switch to an
RTOS queue. The change is a two-line diff in the struct:

```diff
-    volatile size_t head;
-    volatile size_t tail;
+    _Atomic size_t head;
+    _Atomic size_t tail;
```

`_Atomic` subsumes `volatile`, so the qualifier can be dropped.
See [`PHILOSOPHY.md`](PHILOSOPHY.md) for the full rationale.

## Memory Layout

```
  0                                      Capacity
 [ ][ ][ ][ ][ ][D][C][B][A][ ][ ][ ][ ]
                 |--->>---|
                HEAD     TAIL
               INPUT     OUTPUT
```

Virtual indexing (`0` to `2*capacity`) avoids the need to leave a sentinel
slot empty for full/empty disambiguation, without requiring a mutex.

## Code Size Statistics

> Measured on x86-64 with GCC (`-fno-inline -ffunction-sections`). Absolute sizes differ
> on ARM Cortex-M but the relative pattern (especially `-O0` vs `-Os`) holds.
> `—` means the function was inlined away entirely by the compiler at `-Os`.
> Regenerate with `make stats`.

<statsTable>

### `cbuff_uint8.h`

Instance size (RAM): **32 bytes**

| Function | `-O0` bytes | `-Os` bytes |
|---|---:|---:|
| `capacity` | 17 | — |
| `count` | 86 | 30 |
| `dequeue` | 143 | 57 |
| `enqueue` | 193 | 81 |
| `enqueue_overwrite` | 224 | 96 |
| `init` | 177 | 33 |
| `is_empty` | 32 | 15 |
| `is_full` | 96 | 38 |
| `is_init` | 50 | — |
| `peek` | 185 | 74 |
| `reset` | 39 | 13 |
| **total** | **1242** | **437** |

### `cbuff_uint16.h`

Instance size (RAM): **32 bytes**

| Function | `-O0` bytes | `-Os` bytes |
|---|---:|---:|
| `capacity` | 17 | — |
| `count` | 86 | 30 |
| `dequeue` | 147 | 56 |
| `enqueue` | 198 | 81 |
| `enqueue_overwrite` | 229 | 90 |
| `init` | 177 | 33 |
| `is_empty` | 32 | 15 |
| `is_full` | 96 | 38 |
| `is_init` | 50 | — |
| `peek` | 192 | 76 |
| `reset` | 39 | 13 |
| **total** | **1263** | **432** |

### `cbuff_uint32.h`

Instance size (RAM): **32 bytes**

| Function | `-O0` bytes | `-Os` bytes |
|---|---:|---:|
| `capacity` | 17 | — |
| `count` | 86 | 30 |
| `dequeue` | 146 | 54 |
| `enqueue` | 194 | 80 |
| `enqueue_overwrite` | 225 | 89 |
| `init` | 177 | 33 |
| `is_empty` | 32 | 15 |
| `is_full` | 96 | 38 |
| `is_init` | 50 | — |
| `peek` | 191 | 74 |
| `reset` | 39 | 13 |
| **total** | **1253** | **426** |

</statsTable>

## Alternatives

This library targets bare-metal embedded with fixed integer types and zero overhead.
If your needs differ, one of these may be a better fit:

**Need generic struct / arbitrary element support (`void*` + `memcpy`)?**
* [lwrb — Lightweight Ring Buffer](https://github.com/MaJerle/lwrb) — malloc-free,
  void-based, DMA-friendly, SPSC. Well-maintained and widely used in embedded projects.

**Need multi-producer/multi-consumer or RTOS integration?**
* FreeRTOS Queues — thread-safe, ISR-safe, supports blocking, works across tasks.
* CMSIS-RTOS2 Message Queue — vendor-neutral RTOS abstraction for ARM.

**Using C++ or Arduino?**
* [CircularBuffer by rlogiacco](https://github.com/rlogiacco/CircularBuffer) —
  C++ template-based, Arduino-friendly, type-safe without macros.

**Reference / prior art:**
* [god233012yamil/Circular-Buffer](https://github.com/god233012yamil/Circular-Buffer)
  — good [technical writeup](https://github.com/god233012yamil/Circular-Buffer/blob/main/technical-docs.md)
  on lock-free considerations. Uses a sentinel slot rather than virtual indexing.

## Design & History

See [`PHILOSOPHY.md`](PHILOSOPHY.md) for design decisions, rationale, and project history.
