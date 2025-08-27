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

// Enable debug printing
#define DEBUG_PRINT 0

// --- Tests ---

int test_lexer_1()
{
  {
    char *input = "  ( 123";
    int expected_val = 2;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      printf("Error lexer_trim_left on \"%s\", expected %d, got %d",
             input, expected_val, trimmed);
      goto test_lexer1_failed;
    }
  }

  {
    char *input = "    123";
    int expected_val = 4;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      printf("Error lexer_trim_left on \"%s\", expected %d, got %d",
             input, expected_val, trimmed);
      goto test_lexer1_failed;
    }
  }

  {
    char *input = "  \n  \t  \n\n 123";
    int expected_val =11;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      printf("Error lexer_trim_left on \"%s\", expected %d, got %d",
             input, expected_val, trimmed);
      goto test_lexer1_failed;
    }
  }

  printf("OK Test Lexer 1\n");
  return 0;

 test_lexer1_failed:
  printf("ERR Test Lexer 1\n");
  return -1;
}

int test_lexer_2()
{
  {
    char *input = "( 123";
    int token_len = 0;
    
    int expected_token_len = 1;
    enum Token expected_token = HAPLO_LEX_OPEN;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL);
    if (token != expected_token)
    {
      printf("Error lexer_next_token on \"%s\", expected %d, got %d",
             input, expected_token, token);
      goto test_lexer2_failed;
    }
    if (token_len != expected_token_len)
    {
      printf("Error lexer_next_token on \"%s\", expected token len %d, got %d",
             input, expected_token_len, token_len);
      goto test_lexer2_failed;
    }
  }

  {
    char *input = ") 123";
    int token_len = 0;
    
    int expected_token_len = 1;
    enum Token expected_token = HAPLO_LEX_CLOSE;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL);
    if (token != expected_token)
    {
      printf("Error lexer_next_token on \"%s\", expected %d, got %d",
             input, expected_token, token);
      goto test_lexer2_failed;
    }
    if (token_len != expected_token_len)
    {
      printf("Error lexer_next_token on \"%s\", expected token len %d, got %d",
             input, expected_token_len, token_len);
      goto test_lexer2_failed;
    }
  }

  {
    char *input = "# 123";
    int token_len = 0;
    
    int expected_token_len = 1;
    enum Token expected_token = HAPLO_LEX_COMMENT;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL);
    if (token != expected_token)
    {
      printf("Error lexer_next_token on \"%s\", expected %d, got %d",
             input, expected_token, token);
      goto test_lexer2_failed;
    }
    if (token_len != expected_token_len)
    {
      printf("Error lexer_next_token on \"%s\", expected token len %d, got %d",
             input, expected_token_len, token_len);
      goto test_lexer2_failed;
    }
  }

  {
    char *input = "123";
    int token_len = 0;
    
    int expected_token_len = 3;
    enum Token expected_token = HAPLO_LEX_ATOM;
    long int expected_atom = 123;
    Atom atom;
    int token = lexer_next_token(input, strlen(input), &token_len, &atom);
    if (token != expected_token)
    {
      printf("Error lexer_next_token on \"%s\", expected %d, got %d",
             input, expected_token, token);
      goto test_lexer2_failed;
    }
    if (token_len != expected_token_len)
    {
      printf("Error lexer_next_token on \"%s\", expected token len %d, got %d",
             input, expected_token_len, token_len);
      goto test_lexer2_failed;
    }
    if (atom.type != HAPLO_ATOM_INTEGER || atom.integer != expected_atom)
    {
      printf("Error lexer_next_token on \"%s\", expected atom value %ld, got %ld",
             input, expected_atom, atom.integer);
      goto test_lexer2_failed;
    }
  }

  {
    char *input = "69.420";
    int token_len = 0;
    
    int expected_token_len = 6;
    enum Token expected_token = HAPLO_LEX_ATOM;
    double expected_atom = 69.420;
    Atom atom;
    int token = lexer_next_token(input, strlen(input), &token_len, &atom);
    if (token != expected_token)
    {
      printf("Error lexer_next_token on \"%s\", expected %d, got %d",
             input, expected_token, token);
      goto test_lexer2_failed;
    }
    if (token_len != expected_token_len)
    {
      printf("Error lexer_next_token on \"%s\", expected token len %d, got %d",
             input, expected_token_len, token_len);
      goto test_lexer2_failed;
    }
    if (atom.type != HAPLO_ATOM_FLOAT || atom.floating_point != expected_atom)
    {
      printf("Error lexer_next_token on \"%s\", expected atom value %f, got %f",
             input, expected_atom, atom.floating_point);
      goto test_lexer2_failed;
    }
  }

  printf("OK Test Lexer 2\n");
  return 0;

 test_lexer2_failed:
  printf("ERR Test Lexer 2\n");
  return -1;
}

int test_parser_1()
{
  int err;
  char* input = "( c ( a ( b ) ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser1_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_parser1_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("ast:      %s\n", str);
  }

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test_parser1_failed;
  }

  expr_free(expr);

  printf("OK Test Parser 1\n");
  return 0;

 test_parser1_failed:
  printf("ERR Test Parser 1\n");
  return -1;
}

int test_parser_2()
{
  int err;
  char* input = "( + ( 1 ( * ( 2 ( 3 ) ) ) ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser2_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_parser2_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("ast:      %s\n", str);
  }

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test_parser2_failed;
  }

  expr_free(expr);

  printf("OK Test Parser 2\n");
  return 0;

 test_parser2_failed:
  printf("ERR Test Parser 2\n");
  return -1;
}

int test_parser_3()
{
  int err;
  char* input = "(( 1 2 3 )";

  if (DEBUG_PRINT)
  {
    printf("malformed expression: %s\n", input);
  }
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser3_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr != NULL || parser.error == 0)
  {
    printf("Error parser_parse should have returned an error\n");
    goto test_parser3_failed;
  }

  printf("OK Test Parser 3\n");
  return 0;

 test_parser3_failed:
  printf("ERR Test Parser 3\n");
  return -1;
}

int test_parser_4()
{
  int err;
  char* input = "( + 2 3 )";
  char* expected_ast = "( + ( 2 ( 3 ) ) )";

  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser4_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
  }
  
  Expr *expr = parser_parse(&parser);  
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_parser4_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("expected ast: %s\n", expected_ast);
    printf("ast:          %s\n", str);
  }

  if (strcmp(str, expected_ast) != 0)
  {
    printf("Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test_parser4_failed;
  }

  expr_free(expr);
  
  printf("OK Test Parser 4\n");
  return 0;

 test_parser4_failed:
  printf("ERR Test Parser 4\n");
  return -1;
}

int test_parser_5()
{
  int err;
  char* input = "( print \"Hello, World! )";

  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser4_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
  }
  
  Expr *expr = parser_parse(&parser);  
  if (expr != NULL || parser.error != -HAPLO_ERROR_PARSER_STRING_LITERAL_END)
  {
    printf("Error parser_parse returned the wrong error\n");
    goto test_parser4_failed;
  }
  
  printf("OK Test Parser 5\n");
  return 0;

 test_parser4_failed:
  printf("ERR Test Parser 5\n");
  return -1;
}

int test_parser_6()
{
  int err;
  char* input = "print \"Hello, World!\"";
  char* expected_ast = "( print ( \"Hello, World!\" ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser2_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_parser2_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    printf("Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_parser2_failed;
  }

  expr_free(expr);

  printf("OK Test Parser 6\n");
  return 0;

 test_parser2_failed:
  printf("ERR Test Parser 6\n");
  return -1;
}

int test_parser_7()
{
  int err;
  char* input = "# This is a comment";

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
  }
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser2_failed;
  }
  
  Expr *expr = parser_parse(&parser);
  if (expr != NULL)
  {
    printf("Error parser_parse should return null\n");
    expr_free(expr);
    goto test_parser2_failed;
  }

  printf("OK Test Parser 7\n");
  return 0;

 test_parser2_failed:
  printf("ERR Test Parser 7\n");
  return -1;
}

int test_parser_8()
{
  int err;
  char* input = "( * ( ( + 1 2 ) 3 ) )";
  char* expected_ast = "( * ( ( + ( 1 ( 2 ) ) ) ( 3 ) ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_parser2_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
  }
  
  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_parser2_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    printf("Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_parser2_failed;
  }

  expr_free(expr);

  printf("OK Test Parser 8\n");
  return 0;

 test_parser2_failed:
  printf("ERR Test Parser 8\n");
  return -1;
}

int test_interpreter_1()
{
  int err;
  char* input = "( + ( 1 ( 2 ) ) )";
  long expected_result = 3;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_interpreter1_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_interpreter1_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("ast:      %s\n", str);
  }

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");

    expr_free(expr);
    goto test_interpreter1_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    printf("Error %s in interpreter_interpret\n", error_string(val.error));
    expr_free(expr);
    goto test_interpreter1_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    printf("Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
           value_type_string(val.type));
    expr_free(expr);
    goto test_interpreter1_failed;
  }
  if (val.integer != expected_result)
  {
    printf("Error in interpreter_interpret, expected result %ld, got %ld\n",
           expected_result, val.integer);
    expr_free(expr);
    goto test_interpreter1_failed;
  }

  haplo_interpreter_clean(&interpreter);
  expr_free(expr);
  
  printf("OK Test Interpreter 1\n");
  return 0;

 test_interpreter1_failed:
  printf("ERR Test Interpreter 1\n");
  return -1;
}

int test_interpreter_2()
{
  int err;
  char* input = "( * ( 4 ( + ( 1 ( 2 ) ) ) ) )";
  long expected_result = 12;
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    goto test_interpreter1_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    goto test_interpreter1_failed;
  }

  char str[50] = {0};
  expr_string(expr, str);

  if (DEBUG_PRINT)
  {
    printf("original: %s\n", input);
    printf("ast:      %s\n", str);
  }

  if (strcmp(input, str) != 0)
  {
    printf("Error reconstructed expression does not match original\n");

    expr_free(expr);
    goto test_interpreter1_failed;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  if (val.type == HAPLO_VAL_ERROR)
  {
    printf("Error %s in interpreter_interpret\n", error_string(val.error));
    expr_free(expr);
    goto test_interpreter1_failed;
  }
  if (val.type != HAPLO_VAL_INTEGER)
  {
    printf("Error in interpreter_interpret, expected type HAPLO_VAL_INTEGER, got %s\n",
           value_type_string(val.type));
    expr_free(expr);
    goto test_interpreter1_failed;
  }
  if (val.integer != expected_result)
  {
    printf("Error in interpreter_interpret, expected result %ld, got %ld\n",
           expected_result, val.integer);
    expr_free(expr);
    goto test_interpreter1_failed;
  }

  haplo_interpreter_clean(&interpreter);
  expr_free(expr);
  
  printf("OK Test Interpreter 2\n");
  return 0;

 test_interpreter1_failed:
  printf("ERR Test Interpreter 2\n");
  return -1;
}

int main(void)
{
  int out = 0;
  
  printf("Running tests...\n");

  out += test_lexer_1();
  out += test_lexer_2();
  out += test_parser_1();
  out += test_parser_2();
  out += test_parser_3();
  out += test_parser_4();
  out += test_parser_5();
  out += test_parser_6();
  out += test_parser_7();
  out += test_parser_8();
  out += test_interpreter_1();
  out += test_interpreter_2();
  
  printf("Tests done.\n");

  return out;
}
