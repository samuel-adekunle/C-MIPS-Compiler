#!/bin/bash

cd "$(dirname "$0")"
cd ..

set -e

./scripts/build_c_compiler.sh
TOTAL=0
for folder in ./compiler_tests/*/; do
	folder=${folder%"/"}
	folder=${folder##*/}
	
	set +e
	./scripts/run_single_compiler_test.sh $folder
	TOTAL=$(($TOTAL+$?))

	set -e
	echo ""
done
echo "Passed $TOTAL tests"
set +e