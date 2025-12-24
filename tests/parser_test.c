// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>
#include <string.h>

HAPLO_TEST(parser_test, simple1)
{
  int err;
  char* input = "( c ( a ( b ) ) )";
  char* expected = "( c ( ( a ( ( b ) ) ) ) )";
  
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

  expr_free(expr);
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, no_parenthesis)
{
  int err;
  char* input = "( + 1 * 2 3 )";
  char* expected = "( + ( 1 ( * ( 2 ( 3 ) ) ) ) )";
  
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

  expr_free(expr);
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, malformed_parenthesis)
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
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  Expr *expr = parser_parse(&parser);
  if (expr != NULL || parser.error == 0)
  {
    fprintf(stderr, "Error parser_parse should have returned an error\n");
    goto test_failed;
  }

  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, simple2)
{
  int err;
  char* input = "( + 2 3 )";
  char* expected_ast = "( + ( 2 ( 3 ) ) )";

  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
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
    printf("expected ast: %s\n", expected_ast);
    printf("ast:          %s\n", str);
  }

  if (strcmp(str, expected_ast) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match original\n");
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, hello_world)
{
  int err;
  char* input = "( print \"Hello, World! )";

  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
  }
  
  Expr *expr = parser_parse(&parser);  
  if (expr != NULL || parser.error != -HAPLO_ERROR_PARSER_STRING_LITERAL_END)
  {
    fprintf(stderr, "Error parser_parse returned the wrong error\n");
    expr_free(expr);
    goto test_failed;
  }

  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, hello_world_no_parenthesis)
{
  int err;
  char* input = "print \"Hello, World!\"";
  char* expected_ast = "( print ( \"Hello, World!\" ) )";
  
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
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, comment)
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
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }
  
  Expr *expr = parser_parse(&parser);
  if (expr != NULL)
  {
    fprintf(stderr, "Error parser_parse should return null\n");
    expr_free(expr);
    goto test_failed;
  }

  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, nested_expr)
{
  int err;
  char* input = "( * ( ( + 1 2 ) 3 ) )";
  char* expected_ast = "( * ( ( ( + ( 1 ( 2 ) ) ) ( 3 ) ) ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
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
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(parser_test, nested_head_expr)
{
  int err;
  char* input = "( * ( + 1 2 ) 3 )";
  char* expected_ast = "( * ( ( + ( 1 ( 2 ) ) ) ( 3 ) ) )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
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
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}


HAPLO_TEST(parser_test, quote)
{
  int err;
  char* input = "'a";
  char* expected_ast = "( 'a )";
  
  Parser parser = {0};
  err = parser_init(&parser, input, strlen(input));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    goto test_failed;
  }

  if (DEBUG_PRINT)
  {
    printf("original:     %s\n", input);
    printf("expected_ast: %s\n", expected_ast);
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
    printf("ast:          %s\n", str);
  }

  if (strcmp(expected_ast, str) != 0)
  {
    fprintf(stderr, "Error reconstructed expression does not match the expected\n");
    expr_free(expr);
    goto test_failed;
  }

  expr_free(expr);
  HAPLO_TEST_SUCCESS;
  
 test_failed:
  HAPLO_TEST_FAILED;
}
