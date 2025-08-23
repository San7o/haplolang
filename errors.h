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

// Remember to update error_string when adding a new error
#define HAPLO_ERROR_PARSER_INPUT_EMPTY           1
#define HAPLO_ERROR_PARSER_END_OF_INPUT          2
#define HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED    3
#define HAPLO_ERROR_MALFORMED_PARENTHESIS        4
#define HAPLO_ERROR_PARSER_MISSING_OPEN          5
#define HAPLO_ERROR_PARSER_MISSING_CLOSE         6
#define HAPLO_ERROR_INTERPRETER_UNKNOWN_FUNCTION 7
#define HAPLO_ERROR_STACK_NULL                   8
#define HAPLO_ERROR_INTERPRETER_NULL             9
#define HAPLO_ERROR_EXPR_NULL                    10
#define HAPLO_ERROR_PARSER_NULL                  12
#define HAPLO_ERROR_PARSER_INPUT_NULL            13
#define HAPLO_ERROR_STACK_EMPTY                  14
#define HAPLO_ERROR_PARSER_NEXT_TOKEN            15

char* error_string(int error);

#endif // _HAPLO_ERRORS_H_
