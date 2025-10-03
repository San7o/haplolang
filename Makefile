# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

## --- Settings ---

NAME=haplo

CFLAGS=-Wall -Werror -Wpedantic -ggdb -std=c99
CC=gcc

LIB_NAME=lib${NAME}.a
LIB_OBJ=atom.o\
        parser.o\
        lexer.o\
        expr.o\
        interpreter.o\
        value.o\
        errors.o\
        symbol.o
STDLIB_NAME=lib${NAME}std.a
STDLIB_OBJ=stdlib/stdlib.o\
           stdlib/core.o\
           stdlib/io.o\
           stdlib/list.o\
           stdlib/math.o\
           stdlib/logic.o
TEST_NAME=${NAME}_tests
TEST_OBJ=tests/tests.o\
         tests/lexer_test.o\
         tests/parser_test.o\
         tests/interpreter_test.o\
         tests/symbol_map_test.o\
         tests/setq_test.o\
         tests/defunc_test.o
TEST_LINKER_SCRIPT=tests/linker.ld
TEST_E2E_NAME=${NAME}_tests_e2e.sh
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
	./${TEST_E2E_NAME}

.PHONY: check-e2e
check-e2e: cli
	chmod +x ${TEST_E2E_NAME}
	./${TEST_E2E_NAME}

# --- Cleanup ---

clean:
	rm ${LIB_OBJ} ${TEST_OBJ} ${HAPLO_OBJ} ${STDLIB_OBJ} 2>/dev/null || :

distclean:
	rm ${LIB_NAME} ${STDLIB_NAME} ${NAME} ${TEST_NAME} 2>/dev/null || :

# --- Docs ---

INDEX_FILE=docs/index.md
HTML_INTRO=docs/html/intro.html
HTML_OUTRO=docs/html/outro.html
TMP_FILE=/tmp/html.tmp
HTML_DIR=docs/html
html: ${INDEX_FILE} ${HTML_INTRO} ${HTML_OUTRO} 
	pandoc ${INDEX_FILE} -o ${TMP_FILE} ${PANDOC_FLAGS}
	cp ${HTML_INTRO} ${HTML_DIR}/index.html
	cat ${TMP_FILE} >> ${HTML_DIR}/index.html
	cat ${HTML_OUTRO} >> ${HTML_DIR}/index.html
