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

- [Pointer-based gist](https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f)
- [Index-based gist](https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956)
- [Compiler Explorer comparison — Pointer vs Index](https://godbolt.org/#z=OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAKwA7KSuADJ4DJgAcj4ARpjEIIHBAA6oCoRODB7evgGp6ZkC4ZExLPGJybaY9o4CQgRMxAQ5Pn5mGtW1WQ1NBCXRcQlJwQqNza15Hbbj/RGD5cPJAJS2qF7EyOwcAPQ7ANTmAMz7bnibXgbE%2BxYAngSYN15UVAn7AGIk%2B9iL6Fjo%2BwAgikUrQ8KI6gwFPsIMpUBEHtcLEwlOhliYNIC9kCvAQECQQDdiHhDPsANIILxeGEAKUut32HQ6yxhsWJhgA1pSvAA6NAsFkQFiofgKW6RAACwBYTDofNQAoxWIOACVMCjnPsALJiWioZD7KjETCYUj7FgRPAsHz7VAANwSCHVAKtoMwbEETEhtqo%2ByY%2B2Q52Qlya%2B1iz1exB5Sux%2B32vUcBoiYMiAIQhnQ9GI0P41wUKRNAIz%2B3VSh9%2By8CiYwEeEX2dqaTkr%2BxSxFQ2kwDgU0cxsf2cIRb1iKMwLpYbo9jW9TBBbaYyAQ%2Bw5mBSBENbZY%2B3T2YQHIYYA40Id2ayMYOcZR%2B0I5qYDPihpRiP2eNJESwqn2WEjjEDRj9s9QecEB7ZU4yBWgFFQCslGhWJ7keYdUX2YgvSda4XwYK8Hk3RDR0NL5SzwN4vAYLBs0aWI6EIW4zzAgB3J1MMrPDc32DJxyzMN4L9YgUIZQMLiuMMIwSbtaLjABJLcmAdP0IKgujiQIB5MNfUjMA/XCAQE4MhPDF43mPDIBBA7FkSQgRpJ3PcD2hHSQ2ufTI0JBBlJSBQQD2YA8DGHlvLxLxYnlFgdm3BRdwYHYAE4NCi/xJAANlogAVBAfP2ABxHy11heFBASZYXLcjyvOyvzCEpIL%2BR2YVRXFTAdnQQImEkJgAA50ASph/ASwIuCOI4eq4ZAuDapgmD%2BKh0DasxMASqgUrS6EsrGGEJPU1RCq3YrPJ2bzfP8yrgpqkV0nqnYZo0Dp0H8ZANEwLgHq4JgqHu2IotiDRbsCI4fskOKksxJVjgiZBvCwQ4jjcMZ8EEHkEBMI5sH2bEvARNqAH0CGBswjlB8HHkR6GCD%2BTAqHhxHkexDIAC9MCxnG8YYMGvAhomYdiVBPAppGUYOTnPGB3sDmUY0aFoWhTiDByngM64IEBCXUDonMvgUDk8BBCJgEOBKNHsq4LBE4gMbRwRMfWwgIHRPX0SB3Gv3mANpcN43TfRjGxmILwHAxqhLloDHWzJuhaAgI2XiEPA6bNCOqGUAhiBZEx/DcYHAgsMCs%2BznPc7z/OC8LuMU7TzE4z5Gd52o4ujgAEVlqgo5jouW9bsCS6Vcu0BItcs8R%2BvOjbofh8OVPO/2HknLedu64bhOyBHxec47suJ6n4hsFIyH67j%2BfDksBum9NJei5XwFy6dCbl9n3fE7NE%2BT7P8vGjoa%2Bd4jPeH8fsegcCWvGcdpEZ2gkmhxwSO7c2ntE4%2BwIEHMWodw4RhZAbUBbszYEExl7GBfsA5wJDhLKAtNMAikQS8ZYOwiEkLjinCwGgU612WKsfeCVqH%2BFofQu2gIlQEFuAWL8bFoEOGAbpVBcsIEYIZr/KwLq8QLATxAQTmAXrTpLrvjDrnekNZdVhBDSozFMoEQoL1lrd3jxoCglwqW9cuoddFrkNylqlvNlF6lv9EAqb6btO16abtAa41FkkgIJK%2BwCjQg/YgIQgQg2A/8qc5FklkbABpbVb/1YAm60b1euMulVAHAqwtAnA/gvAfgHAWgpAqAnAacNTbE6wmwhMuMPApABAmgE7qwHIAQnQU7HAkgs7e7i7nAvAHknQu287E7pAcAsASAZZjuZAFAoKCo5ZWYIAwAUgUl4siIHkEAsQF7lEzAxAtwnA274HTQtwDGsQHYGp3AvA/Ik44xtAUH97pAgFXgwAuhEsHkKHOH7ohgwA4g2H%2BAxoDgeADoRHC7GknYuI2w27g4J7C7YIbIcHHgWAF7icVo0HvAx4nMSgcGMoRgKYoA97qwVABgwACgAAakRHRAxgWHO9u/wIICIGIOwFIDIIIIoCoOoNh7oFwPoGRygCo/oHgLEB5JAKsKgKuFkER7wP68SFgHZ9bF0ChVkC4KRJMO0GZ2EPMK4sMFwPFKQGkBkJCAFyAGYGZ1F0UAwAMKF4kOF9IHYD5/ULMLF/F95zR9l30Cl/ohIBF58hMJ4G0HF2Z%2BV3MKUCV%2Bl6sJBBsFsBIJO9O%2Be9h0uxwPsKoG1AlFKglJIPsMAC%2BllpIDyMqRALgIQF8McLg7wHe1oIwqQIe/4Me5wGe6QHOwu919eyALe3uytye2YJ17t1ezu0d6sEZL55IEAA%3D)

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
