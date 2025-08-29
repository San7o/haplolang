//
// MIT License
//
// Copyright (c) 2025 Giovanni Santini
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef _HAPLO_PARSER_H_
#define _HAPLO_PARSER_H_

#include "atom.h"
#include "expr.h"
#include "lexer.h"

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Parser HaploParser
  #define parser_init haplo_parser_init
  #define parser_dump haplo_parser_dump
  #define parser_next_token haplo_parser_next_token
  #define parser_peek_next_token haplo_parser_peek_next_token
  #define parser_check_error haplo_parser_check_error
  #define parser_parse haplo_parser_parse
#endif // HAPLO_NO_PREFIX

#define HAPLO_PARSER_ERROR() \
  do { \
    fprintf(stderr, "Parser Error! File %s, line %d\n", __FILE__, __LINE__); \
    longjmp(parser->jump_buf, 1);                                            \
  } while(0)

// --- Types ---

typedef struct HaploParser {
  char* input;
  size_t input_len;
  enum HaploToken last_token;
  HaploAtom last_atom;
  unsigned int pos;
  unsigned int line;
  unsigned int column;
  int error;
  jmp_buf jump_buf;
} HaploParser;

// -- Functions --

int haplo_parser_init(HaploParser *parser, char *input, size_t len);
int haplo_parser_dump(HaploParser *parser);
bool haplo_parser_check_error(HaploParser *parser);
// Returns 0 on success, or a negative integer on error representing
// the error code.  If the next token is an ATOM (set in
// parser->last_token), the atom value is allocated in the heap. The
// user is responsible to free it.
int haplo_parser_next_token(HaploParser *parser);
// Returns 0 on success, or a negative integer on error representing
// the error code.  If the next token is an ATOM (set in
// parser->last_token), the atom value is allocated in the heap. The
// user is responsible to free it.
int haplo_parser_peek_next_token(HaploParser *parser);
HaploExpr *haplo_parser_parse(HaploParser *parser);

#endif // _HAPLO_PARSER_H_
