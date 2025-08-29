#!/bin/sh

#
# MIT License
#
# Copyright (c) 2025 Giovanni Santini
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

HAPLO_EXECUTABLE=./haplo
TMP_FILE=.haplo_tests_e2e_tmp
SAMPLES="sample1.haplo sample2.haplo sample3.haplo"
SAMPLES_DIR=./samples
OK="true"

set -e

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
    OUTPUT=$($HAPLO_EXECUTABLE $SAMPLES_DIR/$SAMPLE)
    EXPECTED_OUTPUT=$(cat $SAMPLES_DIR/$SAMPLE.out)
    if [ ! "$OUTPUT" = "$EXPECTED_OUTPUT"  ]; then
        e2e_error "e2e test failed for sample $SAMPLE"
        echo -e "Expected: \n"
        echo $EXPECTED_OUTPUT
        echo -e "\nGot: \n"
        echo $OUTPUT
        echo -e ""
        $OK="false"
    else
        e2e_ok "$SAMPLE"
    fi
done

echo "E2E tests done..."

if [ "$OK" = "false" ]; then
    exit 1
fi
