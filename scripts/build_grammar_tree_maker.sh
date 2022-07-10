#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

make bin/parseTree bin/grammar_tree_maker > /dev/null