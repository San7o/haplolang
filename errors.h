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

#ifndef _HAPLO_ERRORS_H_
#define _HAPLO_ERRORS_H_

// --- Errors ---

// Remember to update haplo_error_string when adding a new error
#define HAPLO_ERROR_LEXER_END_OF_INPUT           1
#define HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED    2
#define HAPLO_ERROR_MALFORMED_PARENTHESIS        3
#define HAPLO_ERROR_PARSER_MISSING_OPEN          4
#define HAPLO_ERROR_PARSER_MISSING_CLOSE         5
#define HAPLO_ERROR_INTERPRETER_UNKNOWN_FUNCTION 6
#define HAPLO_ERROR_STACK_NULL                   7
#define HAPLO_ERROR_INTERPRETER_NULL             8
#define HAPLO_ERROR_EXPR_NULL                    9
#define HAPLO_ERROR_LEXER_INPUT_NULL             10
#define HAPLO_ERROR_PARSER_INPUT_NULL            11
#define HAPLO_ERROR_STACK_EMPTY                  12
#define HAPLO_ERROR_PARSER_NEXT_TOKEN            13
#define HAPLO_ERROR_PARSER_STRING_LITERAL_END    14
#define HAPLO_ERROR_PARSER_NULL                  15
#define HAPLO_ERROR_LEXER_ATOM_STRING_SIZE       16
#define HAPLO_ERROR_INTERPRETER_INVALID_TYPE     17
#define HAPLO_ERROR_INTERPRETER_INVALID_ATOM     18
#define HAPLO_ERROR_INTERPRETER_NOT_FUNCTION     19
#define HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS 20

#ifdef HAPLO_NO_PREFIX
  #define error_string haplo_error_string
#endif // HAPLO_NO_PREFIX

// --- Functions ---

char* haplo_error_string(int error);

#endif // _HAPLO_ERRORS_H_
