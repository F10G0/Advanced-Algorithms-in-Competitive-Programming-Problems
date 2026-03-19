#!/bin/bash

set -e

EXE="../src/toy"

if [ ! -x "$EXE" ]; then
    echo "ERROR: $EXE not found or not executable."
    echo "Run 'make' in src/ first."
    exit 1
fi

for f in sample_inputs/*.in; do
    name=$(basename "$f" .in)

    "$EXE" < "$f" > tmp.out

    if diff -q tmp.out "sample_outputs/$name.out" >/dev/null; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
    fi
done

rm -f tmp.out
