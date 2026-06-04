# cbuff.h — History

A record of significant decisions and the experiments that shaped this library.

---

## Origins: pointer-based gist

The library started as a personal circular buffer gist using a pointer-based
head/tail approach — the most common implementation style.

- [Pointer-based gist](https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f)

## Switch to index-based approach

A comparison on Compiler Explorer between the pointer and index approaches
showed the index version produced fewer instructions. The likely cause: pointer
arithmetic requires copying the base address into a register, whereas index
arithmetic works directly with offsets from a known base.

This led to a second gist and eventually became the foundation of this library.

- [Index-based gist](https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956)

## Lock-free redesign: virtual index approach

The original implementation used a sentinel slot (one slot always left empty)
to disambiguate full vs empty state — the standard approach for lock-free
single-producer/single-consumer buffers.

Reviewing [god233012yamil/Circular-Buffer](https://github.com/god233012yamil/Circular-Buffer)
and its [technical writeup](https://github.com/god233012yamil/Circular-Buffer/blob/main/technical-docs.md)
clarified the tradeoffs. The sentinel slot wastes one element of capacity and
complicates the index math.

The virtual index approach solves this: indices run from `0` to `2*capacity`,
wrapping at `2*capacity`. The actual memory slots are still `0..capacity-1`,
but the extended index range allows unambiguous full/empty detection without
sacrificing a slot. `head == tail` means empty; `|tail - head| == capacity`
means full.

This was introduced in v0.1.3 as an extensive rewrite.

Relevant readings that informed this:
- https://www.codeproject.com/articles/43510/lock-free-single-producer-single-consumer-circular
- https://andrea.lattuada.me/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html

## Packaged as a clib library (v0.1.x)

The gists were consolidated into a proper clib-compatible single-header library
(`cbuff_uint8.h`) with a Makefile, CI, and tests.

## uint16 / uint32 / uint64 variants via code generation (v0.2.0)

Rather than using C macro metaprogramming (`CBUFF_DEFINE(type)`), type variants
are generated from the `uint8` template via `generate.sh` (a `sed`-based script)
and committed to the repository. This keeps each header readable as plain C
without any macro expansion overhead.

The decision to use code generation over macros, and unsigned types only
(with caller-side casting for signed/float), was driven by the embedded-first
philosophy established in earlier versions. See `PHILOSOPHY.md`.
