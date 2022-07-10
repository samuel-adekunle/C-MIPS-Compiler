#!/bin/bash

cd "$(dirname "$0")"
cd ..

set -e

./scripts/build_c_compiler.sh

set +e

PASSED=0
TOTAL=0

for driver in ./compiler_tests/$1/*_driver.c; do
	program=${driver%"_driver.c"}.c

	./scripts/run_target.sh $program $driver > /dev/null 2> /dev/null
	RETURNED=$?

	gcc $program $driver -o ./out/gcc_compiled 2> /dev/null
	./out/gcc_compiled > /dev/null 2> /dev/null
	GCC_RETURNED=$?

	if [ $RETURNED -eq $GCC_RETURNED ]; then
		PASSED=$(($PASSED + 1))
	else
		echo "$program failed"
	fi

	TOTAL=$(($TOTAL + 1))
done

echo "-----------------------------------"
echo "Passed $PASSED / $TOTAL in $1 tests"
echo "-----------------------------------"

exit $PASSED