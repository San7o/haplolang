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
# all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

## --- Settings ---

NAME=haplo

CFLAGS=-Wall -Werror -Wpedantic -ggdb
CC=gcc

LIB_NAME=lib${NAME}.a
LIB_OBJ=atom.o\
        parser.o\
        lexer.o\
        expr.o\
        interpreter.o\
        value.o\
        errors.o\
        function.o
STDLIB_NAME=lib${NAME}std.a
STDLIB_OBJ=stdlib/stdlib.o\
           stdlib/io.o\
           stdlib/list.o\
           stdlib/math.o
TEST_NAME=${NAME}_tests
TEST_OBJ=tests/tests.o\
         tests/lexer_test.o\
         tests/parser_test.o\
         tests/interpreter_test.o\
         tests/function_map_test.o
TEST_LINKER_SCRIPT=tests/linker.ld
CLI_OBJ=haplo.o

## --- Commands ---

# --- Targets ---

all: lib

lib: ${LIB_NAME}

${LIB_NAME}: ${LIB_OBJ}
	ar rcs $@ $^

stdlib: ${STDLIB_NAME}

${STDLIB_NAME}: ${STDLIB_OBJ}
	ar rcs $@ $^

cli: ${CLI_OBJ} lib stdlib
	${CC} ${CLI_OBJ} -Wl,--whole-archive ${LIB_NAME} ${STDLIB_NAME} -Wl,--no-whole-archive -lreadline ${FLAGS} -o ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

# --- Testing ---

tests: ${TEST_OBJ} lib stdlib
	${CC} ${TEST_OBJ} -Wl,--whole-archive ${LIB_NAME} ${STDLIB_NAME} -Wl,--no-whole-archive ${FLAGS} -Wl,-T,${TEST_LINKER_SCRIPT} -o ${TEST_NAME}

check: tests cli
	chmod +x ${TEST_NAME}
	./${TEST_NAME}
	./${NAME}_tests_e2e.sh

.PHONY: check-e2e
check-e2e: cli
	chmod +x ${NAME}_tests_e2e.sh
	./${NAME}_tests_e2e.sh

# --- Cleanup ---

clean:
	rm ${LIB_OBJ} ${TEST_OBJ} ${HAPLO_OBJ} ${STDLIB_OBJ} 2>/dev/null || :

distclean:
	rm ${LIB_NAME} ${STDLIB_NAME} ${NAME} ${TEST_NAME} 2>/dev/null || :
