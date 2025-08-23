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

#include "interpreter.h"
#include "errors.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int haplo_interpreter_init(HaploInterpreter_t *interpreter)
{
  if (interpreter == NULL) return -HAPLO_ERROR_INTERPRETER_NULL;
  
  haplo_stack_init(&interpreter->stack);
  
  return 0;
}

int haplo_interpreter_interpret(HaploInterpreter_t *interpreter, HaploExpr_t *expr)
{
  if (interpreter == NULL) return -HAPLO_ERROR_INTERPRETER_NULL;
  if (expr == NULL) return -HAPLO_ERROR_EXPR_NULL;
  int err;
  
  if (expr->is_atom)
  {
    if (strcmp(expr->atom, "+") == 0)
    {
      HaploAtom a, b;
      err = haplo_stack_pop(&interpreter->stack, &a);
      if (err < 0) return err;
      err = haplo_stack_pop(&interpreter->stack, &b);
      if (err < 0) return err;

      long a_num = strtol(a, NULL, 10);
      long b_num = strtol(b, NULL, 10);
      
      long sum = a_num + b_num;

      char* sum_buff = (char*) malloc(32);
      sprintf(sum_buff, "%ld", sum);
      haplo_stack_push(&interpreter->stack, sum_buff);
      
      free(a);
      free(b);
    }
    else if (strcmp(expr->atom, "-") == 0)
    {
      HaploAtom a, b;
      err = haplo_stack_pop(&interpreter->stack, &a);
      if (err < 0) return err;
      err = haplo_stack_pop(&interpreter->stack, &b);
      if (err < 0) return err;

      long a_num = strtol(a, NULL, 10);
      long b_num = strtol(b, NULL, 10);
      
      long diff = a_num - b_num;

      char* diff_buff = (char*) malloc(32);
      sprintf(diff_buff, "%ld", diff);
      haplo_stack_push(&interpreter->stack, diff_buff);
      
      free(a);
      free(b);
    }
    else if (strcmp(expr->atom, "*") == 0)
    {
      HaploAtom a, b;
      err = haplo_stack_pop(&interpreter->stack, &a);
      if (err < 0) return err;
      err = haplo_stack_pop(&interpreter->stack, &b);
      if (err < 0) return err;

      long a_num = strtol(a, NULL, 10);
      long b_num = strtol(b, NULL, 10);
      
      long prod = a_num * b_num;

      char* prod_buff = (char*) malloc(32);
      sprintf(prod_buff, "%ld", prod);
      haplo_stack_push(&interpreter->stack, prod_buff);
      
      free(a);
      free(b);
    }
    else if (strcmp(expr->atom, "/") == 0)
    {
      HaploAtom a, b;
      err = haplo_stack_pop(&interpreter->stack, &a);
      if (err < 0) return err;
      err = haplo_stack_pop(&interpreter->stack, &b);
      if (err < 0) return err;

      long a_num = strtol(a, NULL, 10);
      long b_num = strtol(b, NULL, 10);
      
      long quot = a_num * b_num;

      char* quot_buff = (char*) malloc(32);
      sprintf(quot_buff, "%ld", quot);
      haplo_stack_push(&interpreter->stack, quot_buff);
      
      free(a);
      free(b);
    }
    else if (strcmp(expr->atom, "print") == 0)
    {
      HaploAtom val;
      err = haplo_stack_pop(&interpreter->stack, &val);
      printf("%s\n", val);
      free(val);
    } else  {
      // Try to convert the Atom to a number
      char endptr[32]; 
      strtol(expr->atom, (char**) &endptr, 10);
      if (strcmp(expr->atom, "") != 0 && strcmp(endptr, "") != 0)
      {
        HaploAtom new_atom = malloc(strlen(expr->atom));
        strcpy(new_atom, expr->atom);
        haplo_stack_push(&interpreter->stack, new_atom);
      } else {     
        return -HAPLO_ERROR_INTERPRETER_UNKNOWN_FUNCTION;
      }
    }
  } else {
    if (expr->second)
    {
      err = haplo_interpreter_interpret(interpreter, expr->second);
      if (err < 0) return err;
    }
    if (expr->first)
    {
      err = haplo_interpreter_interpret(interpreter, expr->first);
      if (err < 0) return err;
    }
  }

  return 0;
}

void haplo_interpreter_clean(HaploInterpreter_t *interpreter)
{
  if (interpreter == NULL) return;
  haplo_stack_free(&interpreter->stack);
  return;
}
