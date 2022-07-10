#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

mkdir -p out

./scripts/build_grammar_tree_maker.sh

bin/parseTree < $1 > out/parsed.txt # $1 path to src file from root

bin/grammar_tree_maker out/parsed.txt out/grammar_tree.png
