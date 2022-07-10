#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

make bin/c_compiler > /dev/null
