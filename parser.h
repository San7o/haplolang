/*
 * MIT License
 *
 * Copyright (c) 2025 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _HAPLO_PARSER_H_
#define _HAPLO_PARSER_H_

#include "expr.h"

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define parser_init haplo_parser_init
  #define parser_trim_left haplo_parser_trim_left
  #define parser_next_token haplo_parser_next_token
  #define parser_peek_next_token haplo_parser_peek_next_token
  #define parser_dump haplo_parser_dump
  #define parser_check_error haplo_parser_check_error
  #define parser_parse haplo_parser_parse
  #define Token HaploToken
  #define Parser HaploParser
  #define Parser_t HaploParser_t
#endif // HAPLO_NO_PREFIX

#define HAPLO_PARSER_ERROR() \
  do { \
    fprintf(stderr, "Parser Error! File %s, line %d\n", __FILE__, __LINE__); \
    longjmp(parser->jump_buf, 1);                                            \
  } while(0)

// --- Types ---

enum HaploToken {
  OPEN = 0,  // '('
  CLOSE,     // ')'
  ATOM,      // any other ASCII char except
             // spaces, newlines and tabs
  COMMENT,   // '#'
  NONE,
};

struct HaploParser {
  char* input;
  size_t input_len;
  enum HaploToken last_token;
  HaploAtom last_atom;
  unsigned int pos;
  unsigned int line;
  unsigned int column;
  int error;
  jmp_buf jump_buf;
};

typedef struct HaploParser HaploParser_t;

// -- Functions --

int haplo_parser_init(HaploParser_t *parser, char *input, size_t len);
void haplo_parser_trim_left(HaploParser_t *parser);
int haplo_parser_next_token(HaploParser_t *parser);
int haplo_parser_peek_next_token(HaploParser_t *parser);
int haplo_parser_dump(HaploParser_t *parser);
bool haplo_parser_check_error(HaploParser_t *parser);
HaploExpr_t *haplo_parser_parse(HaploParser_t *parser);

#endif // _HAPLO_PARSER_H_
