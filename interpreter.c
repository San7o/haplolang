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
    switch(expr->atom.type)
    {
    case STRING: ;
      HaploAtom_t string_atom = (HaploAtom_t){
        .type = STRING,
      };
      string_atom.string = malloc(strlen(expr->atom.string));
      strcpy(string_atom.string, expr->atom.string);
      haplo_stack_push(&interpreter->stack, string_atom);
      break;
    case INTEGER:
      haplo_stack_push(&interpreter->stack, expr->atom);
      break;
    case BOOL:
      haplo_stack_push(&interpreter->stack, expr->atom);
      break;
    case SYMBOL:
      if (strcmp(expr->atom.symbol, "+") == 0)
      {
        HaploAtom_t a, b;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;

        if (a.type != INTEGER || b.type != INTEGER)
          return -HAPLO_ERROR_INTERPRETER_INVALID_TYPE;

        HaploAtom_t sum_atom = (HaploAtom_t){
          .type = INTEGER,
          .integer = a.integer + b.integer,
        };
        haplo_stack_push(&interpreter->stack, sum_atom);
      }
      else if (strcmp(expr->atom.symbol, "-") == 0)
      {
        HaploAtom_t a, b;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;

        if (a.type != INTEGER || b.type != INTEGER)
          return -HAPLO_ERROR_INTERPRETER_INVALID_TYPE;

        HaploAtom_t diff_atom = (HaploAtom_t){
          .type = INTEGER,
          .integer = a.integer - b.integer,
        };
        haplo_stack_push(&interpreter->stack, diff_atom);
      }
      else if (strcmp(expr->atom.symbol, "*") == 0)
      {
        HaploAtom_t a, b;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;

        if (a.type != INTEGER || b.type != INTEGER)
          return -HAPLO_ERROR_INTERPRETER_INVALID_TYPE;

        HaploAtom_t prod_atom = (HaploAtom_t){
          .type = INTEGER,
          .integer = a.integer * b.integer,
        };
        haplo_stack_push(&interpreter->stack, prod_atom);
      }
      else if (strcmp(expr->atom.symbol, "/") == 0)
      {
        HaploAtom_t a, b;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;

        if (a.type != INTEGER || b.type != INTEGER)
          return -HAPLO_ERROR_INTERPRETER_INVALID_TYPE;

        HaploAtom_t quot_atom = (HaploAtom_t){
          .type = INTEGER,
          .integer = a.integer / b.integer,
        };
        haplo_stack_push(&interpreter->stack, quot_atom);
      }
      else if (strcmp(expr->atom.symbol, "print") == 0)
      {
        HaploAtom_t val;
        err = haplo_stack_pop(&interpreter->stack, &val);
        if (err < 0) return err;

        char buf[MAX_ATOM_SIZE] = {0};
        haplo_atom_string(val, buf);
        printf("%s\n", buf);
        
        haplo_atom_free(val);
      }
      else {
        return -HAPLO_ERROR_INTERPRETER_UNKNOWN_FUNCTION;
      }
      break;
    }
  } else {
    if (expr->tail)
    {
      err = haplo_interpreter_interpret(interpreter, expr->tail);
      if (err < 0) return err;
    }
    if (expr->head)
    {
      err = haplo_interpreter_interpret(interpreter, expr->head);
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
