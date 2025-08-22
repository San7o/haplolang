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

#define HAPLO_NO_PREFIX
#include "haplo.h"

#include <stdio.h>
#include <string.h>

// --- Tests ---

void test_parser_1()
{

  printf("Now running: Test 1\n");
  int err;
  char* input = "( c . ( a . b ) )";
  
  Parser_t parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test1_failed;
  }

  Expr_t *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test1_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);
  printf("original: %s\n", input);
  printf("ast:      %s\n", str);

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test1_failed;
  }

  expr_free(expr);

  printf("OK Test 1\n");
  return;

 test1_failed:
  printf("ERR Test 1\n");
  return;
}

void test_parser_2()
{
  printf("Now running: Test 2\n");
  int err;
  char* input = "( + . ( 1 . ( * . ( 2 . 3 ) ) ) )";
  
  Parser_t parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test2_failed;
  }

  Expr_t *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test2_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);
  printf("original: %s\n", input);
  printf("ast:      %s\n", str);

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test2_failed;
  }

  expr_free(expr);

  printf("OK Test 2\n");
  return;

 test2_failed:
  printf("ERR Test 2\n");
  return;
}

void test_interpreter_1()
{
  printf("Now running: Test Interpreter 1\n");
  int err;
  char* input = "( p . ( + . ( 1 . 2 ) ) )";
  
  Parser_t parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto interpreter1_failed;
  }

  Expr_t *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto interpreter1_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);
  printf("original: %s\n", input);
  printf("ast:      %s\n", str);

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
      
    expr_free(expr);
    goto interpreter1_failed;
  }

  Interpreter_t interpreter = {0};
  interpreter_init(&interpreter);
  err = interpreter_interpret(&interpreter, expr);
  if (err < 0)
  {
    printf("Error %d in interpreter_interpret\n", err);
    expr_free(expr);
    goto interpreter1_failed;
  }

  haplo_interpreter_clean(&interpreter);
  expr_free(expr);
  
  printf("OK Test Interpreter 1\n");
  return;

 interpreter1_failed:
  printf("ERR Test Interpreter 1\n");
  return;
}

int main(void)
{
  printf("Running tests...\n");

  test_parser_1();
  test_parser_2();
  test_interpreter_1();
  
  printf("Tests done.\n");
  
  return 0;
}
