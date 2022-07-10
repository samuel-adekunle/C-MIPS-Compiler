#!/bin/bash
set -e

cd "$(dirname "$0")"
cd ..

./scripts/build_c_compiler.sh

./bin/c_compiler -S $1 -o ./out/compiled.s # $1 path to src file from root
mips-linux-gnu-gcc -mfp32 -march=mips1 -S $2 -o ./out/driver.s

mips-linux-gnu-gcc -mfp32 -static -o ./out/compiled.o -c ./out/compiled.s
mips-linux-gnu-gcc -mfp32 -static -o ./out/driver.o -c ./out/driver.s

mips-linux-gnu-gcc -mfp32 -static -o ./out/compiled ./out/compiled.o ./out/driver.o

set +e

qemu-mips ./out/compiled
RETURNED=$?

exit $RETURNED