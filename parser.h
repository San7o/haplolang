// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_PARSER_H
#define HAPLO_PARSER_H

#include "atom.h"
#include "expr.h"
#include "lexer.h"

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

//
// Macros
//

#ifdef HAPLO_NO_PREFIX
  #define Parser HaploParser
  #define parser_init haplo_parser_init
  #define parser_dump haplo_parser_dump
  #define parser_check_error haplo_parser_check_error
  #define parser_parse haplo_parser_parse
#endif // HAPLO_NO_PREFIX

#define HAPLO_PARSER_ERROR() \
  do { \
    fprintf(stderr, "Parser Error! File %s, line %d\n", __FILE__, __LINE__); \
    longjmp(parser->jump_buf, 1);                                            \
  } while(0)

//
// Types
//

typedef struct {
  HaploLexer lexer;
  int error;
  jmp_buf jump_buf;
} HaploParser;

//
// Functions
//

int haplo_parser_init(HaploParser *parser, char *input, unsigned int len);
int haplo_parser_dump(HaploParser *parser);
bool haplo_parser_check_error(HaploParser *parser);
HaploExpr *haplo_parser_parse(HaploParser *parser);

#endif // HAPLO_PARSER_H
