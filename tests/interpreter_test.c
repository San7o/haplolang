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

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>
#include <string.h>

HAPLO_TEST(interpreter_test, simple_math)
{
  int err;
  char* input = "( + 1 2 )";
  char* expected = "( + ( 1 ( 2 ) ) )";
  long expected_result = 3;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    fprintf(stderr, "Error parser_parse returned a null expression\n");
    goto test_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("expected: %s\n", expected);
    printf("ast:      %s\n", str);
  }

  if (strcmp(expected, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match expected\n");

    expr_free(expr);
    goto test_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret\n", error_string(val.value.error));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
            value_type_string(val.type));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.value.integer != expected_result)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected result %ld, got %ld\n",
            expected_result, val.value.integer);
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }

  haplo_interpreter_destroy(&interpreter);
  expr_free(expr);
  
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(interpreter_test, should_eval_head)
{
  int err;
  char* input = "( * ( + 1 2 ) 3 )";
  char* expected = "( * ( ( + ( 1 ( 2 ) ) ) ( 3 ) ) )";
  long expected_result = 9;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    fprintf(stderr, "Error parser_parse returned a null expression\n");
    goto test_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("expected: %s\n", expected);
    printf("ast:      %s\n", str);
  }

  if (strcmp(expected, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match original\n");

    expr_free(expr);
    goto test_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret\n", error_string(val.value.error));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
            value_type_string(val.type));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.value.integer != expected_result)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected result %ld, got %ld\n",
            expected_result, val.value.integer);
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }

  haplo_interpreter_destroy(&interpreter);
  expr_free(expr);
  
  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(interpreter_test, simple_integer_val)
{
  int err;
  char* input = "123";
  char* expected = "( 123 )";
  long expected_result = 123;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    fprintf(stderr, "Error parser_parse returned a null expression\n");
    goto test_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("expected: %s\n", expected);
    printf("ast:      %s\n", str);
  }

  if (strcmp(expected, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match expected\n");

    expr_free(expr);
    goto test_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret\n", error_string(val.value.error));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
            value_type_string(val.type));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.value.integer != expected_result)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected result %ld, got %ld\n",
            expected_result, val.value.integer);
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }

  haplo_interpreter_destroy(&interpreter);
  expr_free(expr);
  
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(interpreter_test, simple_string_val)
{
  int err;
  char* input = "\"Hello!\"";
  char* expected = "( \"Hello!\" )";
  char* expected_result = "Hello!";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    fprintf(stderr, "Error parser_parse returned a null expression\n");
    goto test_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("expected: %s\n", expected);
    printf("ast:      %s\n", str);
  }

  if (strcmp(expected, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match expected\n");

    expr_free(expr);
    goto test_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret\n", error_string(val.value.error));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_STRING)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_STRING, got %s\n",
            value_type_string(val.type));
    haplo_interpreter_destroy(&interpreter);
    value_free(val);
    expr_free(expr);
    goto test_failed;
  }
  if (strcmp(val.value.string, expected_result) != 0)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected result %s, got %s\n",
            expected_result, val.value.string);
    haplo_interpreter_destroy(&interpreter);
    value_free(val);
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  value_free(val);
  haplo_interpreter_destroy(&interpreter);
  
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(interpreter_test, math)
{
  int err;
  char* input = "( + 4 ( * 2 3 ) ) ) )";
  char* expected = "( + ( 4 ( ( * ( 2 ( 3 ) ) ) ) ) )";
  long expected_result = 10;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    fprintf(stderr, "Error parser_parse returned a null expression\n");
    goto test_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("expected: %s\n", expected);
    printf("ast:      %s\n", str);
  }

  if (strcmp(expected, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match original\n");

    expr_free(expr);
    goto test_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret\n", error_string(val.value.error));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
            value_type_string(val.type));
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }
  if (val.value.integer != expected_result)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected result %ld, got %ld\n",
            expected_result, val.value.integer);
    haplo_interpreter_destroy(&interpreter);
    expr_free(expr);
    goto test_failed;
  }

  haplo_interpreter_destroy(&interpreter);
  expr_free(expr);
  
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}
