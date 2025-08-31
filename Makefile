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

LIB_OBJ=atom.o\
        parser.o\
        lexer.o\
        expr.o\
        interpreter.o\
        value.o\
        errors.o\
        function.o\
        stdlib/stdlib.o\
        stdlib/io.o\
        stdlib/list.o\
        stdlib/math.o
TEST_OBJ=tests/tests.o\
         tests/lexer_test.o\
         tests/parser_test.o\
         tests/interpreter_test.o\
         tests/function_map_test.o
TEST_LINKER_SCRIPT=tests/linker.ld
CLI_OBJ=haplo.o

## --- Commands ---

# --- Lib ---

all: lib

lib: lib${NAME}.a

lib${NAME}.a: ${LIB_OBJ}
	ar rcs $@ $^

# --- Cli ---

cli: ${CLI_OBJ} lib
	${CC} ${CLI_OBJ} -Wl,--whole-archive lib${NAME}.a -Wl,--no-whole-archive -lreadline ${FLAGS} -o ${NAME}

# --- Testing ---

tests: ${TEST_OBJ} lib
	${CC} ${TEST_OBJ} -Wl,--whole-archive lib${NAME}.a -Wl,--no-whole-archive ${FLAGS} -Wl,-T,${TEST_LINKER_SCRIPT} -o ${NAME}_tests

check: tests
	chmod +x ${NAME}_tests
	./${NAME}_tests
	./${NAME}_tests_e2e.sh

.PHONY: check-e2e
check-e2e:
	chmod +x ${NAME}_tests_e2e.sh
	./${NAME}_tests_e2e.sh

# --- Cleanup ---

clean:
	rm ${LIB_OBJ} ${TEST_OBJ} ${HAPLO_OBJ} 2>/dev/null || :

distclean:
	rm lib${NAME}.a ${NAME} ${NAME}_tests 2>/dev/null || :

# --- Misc ---

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
