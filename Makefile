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

LIB_OBJ=parser.o\
        expr.o\
        interpreter.o\
        stack.o
TEST_OBJ=tests.o
CLI_OBJ=haplo.o

## --- Commands ---

# --- Lib ---

all: lib

lib: lib${NAME}.a

lib${NAME}.a: ${LIB_OBJ}
	ar rcs $@ $^

# --- Cli ---

cli: ${CLI_OBJ} lib
	${CC} ${CLI_OBJ} lib${NAME}.a -lreadline ${FLAGS} -o ${NAME}

# --- Testing ---

tests: ${TEST_OBJ} lib
	${CC} ${TEST_OBJ} lib${NAME}.a ${FLAGS} -o ${NAME}_tests

check: tests
	chmod +x ${NAME}_tests
	./${NAME}_tests

# --- Cleanup ---

clean:
	rm ${LIB_OBJ} ${TEST_OBJ} ${HAPLO_OBJ} 2>/dev/null || :

distclean:
	rm lib${NAME}.a

# --- Misc ---

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
