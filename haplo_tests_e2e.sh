#!/bin/sh
# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

HAPLO_EXECUTABLE=./haplo
TMP_FILE=.haplo_tests_e2e_tmp
SAMPLES_DIR=./samples
SAMPLES=$(echo $SAMPLES_DIR/*.haplo)
OK="true"

echo "Running E2E tests..."

e2e_error()
{
    echo ERR: $1
}

e2e_ok()
{
    echo OK: $1
}

if [ ! -e $HAPLO_EXECUTABLE ]; then
    e2e_error "haplo executable not found"
    exit 1
fi

for SAMPLE in $SAMPLES; do
    OUTPUT=$("$HAPLO_EXECUTABLE" "$SAMPLE")
    EXPECTED_OUTPUT=$(cat $SAMPLE.out)
    if [ ! "$OUTPUT" = "$EXPECTED_OUTPUT"  ]; then
        e2e_error "e2e test failed for sample $SAMPLE"
        echo -e "Expected: \n"
        echo $EXPECTED_OUTPUT
        echo -e "\nGot: \n"
        echo $OUTPUT
        echo -e ""
        OK="false"
    else
        e2e_ok "$SAMPLE"
    fi
done

echo "E2E tests done..."

if [ "$OK" = "false" ]; then
    exit 1
fi
