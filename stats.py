#!/usr/bin/env python3
"""
Generate code size statistics for all cbuff variants and optionally update README.md.

Usage:
  python3 stats.py            -- print table to stdout
  python3 stats.py --readme   -- also write table into <statsTable> block in README.md

Measures each function at both -O0 (per-function understanding) and -Os (real embedded
usage). Uses a probe file that calls every function to ensure static inline functions
are emitted. Compiled with -fno-inline -ffunction-sections to keep functions separate.
"""

import os
import re
import subprocess
import sys
import tempfile

TYPES = [
    ("uint8",  "uint8_t"),
    ("uint16", "uint16_t"),
    ("uint32", "uint32_t"),
]

PROBE_TEMPLATE = """\
#include <stddef.h>
#include "cbuff_{name}.h"
static {type} buf[8];
/* Pass non-constant pointers to prevent GCC constant-propagation clones */
void probe(cbuff_{name}_t *cb, {type} *val, size_t *offset) {{
    cbuff_{name}_init(cb, 8, buf);
    cbuff_{name}_is_init(cb);
    cbuff_{name}_reset(cb);
    cbuff_{name}_enqueue(cb, *val);
    cbuff_{name}_enqueue_overwrite(cb, *val);
    cbuff_{name}_dequeue(cb, val);
    cbuff_{name}_peek(cb, val, *offset);
    cbuff_{name}_capacity(cb);
    cbuff_{name}_count(cb);
    cbuff_{name}_is_full(cb);
    cbuff_{name}_is_empty(cb);
}}
"""

SIZEOF_TEMPLATE = """\
#include <stdio.h>
#include "cbuff_{name}.h"
int main(void) {{ printf("%zu\\n", sizeof(cbuff_{name}_t)); return 0; }}
"""


def compile_probe(name, ctype, extra_flags, outdir, tag):
    src = os.path.join(outdir, f"probe_{name}_{tag}.c")
    obj = os.path.join(outdir, f"probe_{name}_{tag}.o")
    with open(src, "w") as f:
        f.write(PROBE_TEMPLATE.format(name=name, type=ctype))
    subprocess.run(
        ["cc", "-std=c11", "-Wall",
         "-fno-inline", "-fno-ipa-cp", "-fno-ipa-sra",
         "-ffunction-sections", "-I."]
        + extra_flags + ["-c", src, "-o", obj],
        check=True,
        capture_output=True,
    )
    return obj


def get_function_sizes(obj, prefix):
    result = subprocess.run(
        ["nm", "-S", "--defined-only", obj],
        capture_output=True,
        text=True,
        check=True,
    )
    sizes = {}
    for line in result.stdout.splitlines():
        parts = line.split()
        # format: address size type name
        if len(parts) >= 4 and parts[2].lower() == "t":
            raw = parts[3]
            if not raw.startswith(prefix):
                continue
            # strip GCC clone suffixes e.g. .constprop.0, .isra.0, .part.0
            name = re.sub(r"\.(constprop|isra|part|clone)\.\d+$", "", raw)
            size = int(parts[1], 16)
            # keep the larger of any duplicates (base function, not specialised clone)
            if name not in sizes or size > sizes[name]:
                sizes[name] = size
    return sizes


def get_instance_size(name, ctype):
    with tempfile.TemporaryDirectory() as d:
        src = os.path.join(d, "sizeof.c")
        bin_ = os.path.join(d, "sizeof_bin")
        with open(src, "w") as f:
            f.write(SIZEOF_TEMPLATE.format(name=name, type=ctype))
        subprocess.run(
            ["cc", "-std=c11", "-I.", src, "-o", bin_],
            check=True,
            capture_output=True,
        )
        return int(subprocess.run([bin_], capture_output=True, text=True).stdout.strip())


def build_table():
    sections = []
    for name, ctype in TYPES:
        with tempfile.TemporaryDirectory() as d:
            obj_o0 = compile_probe(name, ctype, ["-O0"], d, "O0")
            obj_os = compile_probe(name, ctype, ["-Os"], d, "Os")
            sizes_o0 = get_function_sizes(obj_o0, f"cbuff_{name}_")
            sizes_os = get_function_sizes(obj_os, f"cbuff_{name}_")
            instance_bytes = get_instance_size(name, ctype)

        all_funcs = sorted(set(sizes_o0) | set(sizes_os))
        rows = []
        for func in all_funcs:
            short = func.replace(f"cbuff_{name}_", "")
            o0 = sizes_o0.get(func, "—")
            os_ = sizes_os.get(func, "—")
            rows.append(f"| `{short}` | {o0} | {os_} |")

        total_o0 = sum(sizes_o0.values())
        total_os = sum(sizes_os.values())

        section = "\n".join([
            f"### `cbuff_{name}.h`",
            "",
            f"Instance size (RAM): **{instance_bytes} bytes**",
            "",
            "| Function | `-O0` bytes | `-Os` bytes |",
            "|---|---:|---:|",
            *rows,
            f"| **total** | **{total_o0}** | **{total_os}** |",
        ])
        sections.append(section)

    return "\n\n".join(sections)


def update_readme(table):
    with open("README.md") as f:
        content = f.read()
    replacement = f"<statsTable>\n\n{table}\n\n</statsTable>"
    new_content, n = re.subn(
        r"<statsTable>.*?</statsTable>", replacement, content, flags=re.DOTALL
    )
    if n == 0:
        print("WARNING: <statsTable> block not found in README.md — skipping update")
        return
    with open("README.md", "w") as f:
        f.write(new_content)
    print("README.md updated.")


if __name__ == "__main__":
    table = build_table()
    print(table)
    if "--readme" in sys.argv:
        update_readme(table)
