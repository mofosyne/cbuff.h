# cbuff.h — Design Philosophy

This document captures the reasoning behind design decisions in this project.
It is intended for both human contributors and AI agents working on the codebase.

---

## Do one thing well

Each header implements one specific circular buffer variant and does it completely.
Prefer a focused, correct implementation over a flexible, abstract one.
A developer should be able to read any single header and fully understand it without
cross-referencing other files.

## Embedded systems first

This library targets microcontrollers and resource-constrained environments.
Every design decision is evaluated against that context:

- No dynamic memory allocation (`malloc`, `free`)
- No hidden overhead — cost is always visible at the call site
- Static inline functions so the compiler can optimise aggressively
- No dependencies beyond the C standard library

If a feature wouldn't be appropriate on a bare-metal microcontroller, it probably
doesn't belong here.

## No overhead for overhead's sake

A circular buffer is a hot path. Extra indirection, extra function calls, and extra
arithmetic per enqueue/dequeue are real costs on a microcontroller.

This is why there is no generic `void *` + `memcpy` implementation.
For `uint8_t`, direct array assignment is one instruction.
A generic version would add a runtime multiply and a `memcpy` call on every access —
overhead that cannot be optimised away when the element size is a runtime variable.

Separate type-specific headers keep each variant at maximum efficiency.

## Unsigned types only

The buffer stores and retrieves bits — it does not interpret them.
`cbuff_uint8.h` handles `int8_t` use cases via a cast at the call site;
`cbuff_uint32.h` handles `int32_t` and `float` the same way.

Separate signed or float variants would double the file count with no
implementation difference, violating the "do one thing well" principle.

## Code generation over C macro metaprogramming

Type variants (`uint8`, `uint16`, `uint32`, `uint64`) are produced by a code generator,
not by `#define` tricks. Generated files are committed to the repository so
users can grab a single header without running any tooling.

C macros that generate entire APIs are notoriously difficult to read, debug,
and maintain. A generated `.h` file is just C — readable, greppable, and
understandable without macro expansion.

The macro-template tradeoff (e.g. `CBUFF_DEFINE(my_type)`) only makes sense
when the type list is open-ended and user-defined. Here the type set is closed:
`uint8/16/32/64` covers every standard fixed-width integer in C. There is no
open-ended case to handle, so macros add complexity with no benefit.

## Lock-free by design

The virtual index approach (indices run `0` to `2*capacity`, wrapping at `2*capacity`)
eliminates the need for a sentinel slot and allows unambiguous full/empty detection
without a mutex. This suits single-producer/single-consumer embedded use cases where
a mutex would be too costly or unavailable.

This is a deliberate tradeoff: the design is correct for SPSC only. Multi-producer
or multi-consumer use cases require additional synchronisation outside this library.

`volatile` on `head` and `tail` is sufficient for single-core embedded use (one ISR
writing, one reader in the main loop) because there is no cache coherency issue and
the compiler is prevented from caching the values in registers. It is NOT sufficient
for multi-core — use `_Atomic` (C11) or explicit platform memory barriers there.

## What this library is not

- A general-purpose queue for application code on a full OS
- A replacement for FreeRTOS queues, CMSIS-RTOS, or similar RTOS primitives
- A library that tries to cover every possible type or use case

If you need struct queues, dynamic sizing, or multi-producer safety, there are
better-suited libraries available.

## Index approach over pointer approach

The implementation uses integer indices for head/tail rather than pointers.
Compared against a pointer-based approach on Compiler Explorer, the index
approach produces fewer instructions — likely because pointer arithmetic
requires copying the base address into a register whereas index arithmetic
works directly with offsets.

The original comparison was made in these gists:
- [Pointer approach](https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f)
- [Index approach](https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956)

## Recommended readings

These informed the lock-free design:

- https://www.codeproject.com/articles/43510/lock-free-single-producer-single-consumer-circular
- https://andrea.lattuada.me/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html

## History

This library originated as a pair of GitHub Gists exploring pointer-based vs
index-based circular buffer implementations, then evolved into a clib-packaged
header-only library with the lock-free virtual-index approach, type variants
via code generation, and hardened test coverage.
