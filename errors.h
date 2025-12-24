// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_ERRORS_H
#define HAPLO_ERRORS_H

//
// Errors
//

// Remember to update haplo_error_string when adding a new error
#define HAPLO_ERROR_LEXER_END_OF_INPUT               1
#define HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED        2
#define HAPLO_ERROR_MALFORMED_PARENTHESIS            3
#define HAPLO_ERROR_PARSER_MISSING_OPEN              4
#define HAPLO_ERROR_PARSER_MISSING_CLOSE             5
#define HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL       6
#define HAPLO_ERROR_STACK_NULL                       7
#define HAPLO_ERROR_INTERPRETER_NULL                 8
#define HAPLO_ERROR_EXPR_NULL                        9
#define HAPLO_ERROR_LEXER_INPUT_NULL                 10
#define HAPLO_ERROR_PARSER_INPUT_NULL                11
#define HAPLO_ERROR_STACK_EMPTY                      12
#define HAPLO_ERROR_PARSER_NEXT_TOKEN                13
#define HAPLO_ERROR_PARSER_STRING_LITERAL_END        14
#define HAPLO_ERROR_PARSER_NULL                      15
#define HAPLO_ERROR_LEXER_ATOM_STRING_SIZE           16
#define HAPLO_ERROR_INTERPRETER_INVALID_TYPE         17
#define HAPLO_ERROR_INTERPRETER_INVALID_ATOM         18
#define HAPLO_ERROR_INTERPRETER_NOT_FUNCTION         19
#define HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS 20
#define HAPLO_ERROR_SYMBOL_MAP_NULL                  21
#define HAPLO_ERROR_SYMBOL_MAP_NOT_INITIALIZED       22
#define HAPLO_ERROR_SYMBOL_MAP_LOOKUP_NOT_FOUND      23
#define HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL_TYPE  24
#define HAPLO_ERROR_VALUE_TYPE_UNRECOGNIZED          25
#define HAPLO_ERROR_PARSER_UNEXPECTED_TOKEN          26
#define HAPLO_ERROR_LEXER_NULL                       27

#ifdef HAPLO_NO_PREFIX
  #define error_string haplo_error_string
#endif // HAPLO_NO_PREFIX

//
// Functions
//

const char* haplo_error_string(int error);

#endif // HAPLO_ERRORS_H
