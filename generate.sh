#!/bin/sh
# Generates type-specific cbuff headers and tests from the uint8 template.
# Usage: sh generate.sh  (or: make generate)
# Generated files are committed so users can grab a single header directly.

set -e

generate() {
    UTYPE=$1  # e.g. uint16_t
    UNAME=$2  # e.g. uint16
    UUPPER=$(echo "$UNAME" | tr '[:lower:]' '[:upper:]')

    for SRC_DST in "cbuff_uint8.h:cbuff_${UNAME}.h" "cbuff_uint8_test.c:cbuff_${UNAME}_test.c"; do
        IN="${SRC_DST%%:*}"
        OUT="${SRC_DST##*:}"
        printf "Generating %s from %s\n" "$OUT" "$IN"
        {
            printf "/* Generated from %s by generate.sh -- do not edit directly */\n" "$IN"
            sed \
                -e "s/uint8_t/${UTYPE}/g" \
                -e "s/uint8/${UNAME}/g"   \
                -e "s/UINT8/${UUPPER}/g"  \
                "$IN"
        } > "$OUT"
    done
}

generate uint16_t uint16
generate uint32_t uint32
