// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>
#include <string.h>

HAPLO_TEST(defunc_test, declare_function)
{
  int err;
  char* input = "(defunc 'test (+ 2 3) )";
  char* expected = "( defunc ( 'test ( ( + ( 2 ( 3 ) ) ) ) ) )";
  
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
    expr_free(expr);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  if (val.type != HAPLO_VAL_EMPTY)
  {
    fprintf(stderr, "Error in interpreter_interpret, expected type HAPLO_VAL_EMPTY, got %s\n",
           value_type_string(val.type));
    expr_free(expr);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  value_free(val);

  Symbol symbol;
  err = symbol_map_lookup(interpreter.symbol_map,
                          "test",
                          &symbol);
  if (err < 0)
  {
    fprintf(stderr, "Error in symbol_map_lookup: %s\n", error_string(err));
    expr_free(expr);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  if (symbol.type != HAPLO_SYMBOL_FUNCTION)
  {
    fprintf(stderr, "Error in symbol_map_lookup: symbol type is %s instead of HAPLO_SYMBOL_VARIABLE\n", symbol_type_string(symbol.type));
    expr_free(expr);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }

  char buff[1024] = {0};
  char* expected_func = "( + ( 2 ( 3 ) ) )";
  expr_string(symbol.func, &buff[0]);
  if (strcmp(buff, expected_func) != 0)
  {
    fprintf(stderr, "Error in symbol_map_lookup: expected %s, but got %s\n",
            expected_func, buff);
    expr_free(expr);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  expr_free(expr);

  // Try to call the new function
  char* input2 = "(test)";
  Parser parser2 = {0};
  err = parser_init(&parser2, input2, strlen(input2));
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init 2\n", err);
    goto test_failed;
  }
  Expr *expr2 = parser_parse(&parser2);
  Value val2 = interpreter_interpret(&interpreter, expr2);
  int expected_result = 5;
  if (val2.type == HAPLO_VAL_ERROR)
  {
    fprintf(stderr, "Error %s in interpreter_interpret 2\n", error_string(val2.value.error));
    expr_free(expr2);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  if (val2.type != HAPLO_VAL_INTEGER)
  {
    fprintf(stderr, "Error in interpreter_interpret 2, expected type HAPLO_VAL_INTEGER, got %s\n",
           value_type_string(val2.type));
    value_free(val2);
    expr_free(expr2);
    interpreter_destroy(&interpreter);
    goto test_failed;
  }
  if (val2.value.integer != expected_result)
  {
    fprintf(stderr, "Error in interpreter_interpret 2, expected type HAPLO_VAL_INTEGER, got %s\n",
            value_type_string(val2.type));
    value_free(val2);
    expr_free(expr2);
    interpreter_destroy(&interpreter);
    goto test_failed;    
  }
  expr_free(expr2);
  value_free(val2);
  
  interpreter_destroy(&interpreter);
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}
