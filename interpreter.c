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
  int err, a, b;
  
  if (expr->is_atom)
  {
    // May be an integer or a function name (single char)
    // TODO: may be a string
    // TODO: may be a multiple digit number
    if (expr->atom >= '0' && expr->atom <= '9')
    {
      haplo_stack_push(&interpreter->stack, expr->atom - '0');
    } else {
      switch (expr->atom)
      {
      case '+': ;
        int sum;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;
        
        sum = a + b;
        haplo_stack_push(&interpreter->stack, sum);
        break;
      case '-': ;
        int diff;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;
        
        diff = a - b;
        haplo_stack_push(&interpreter->stack, diff);
        break;
      case '*': ;
        int prod;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;
        
        prod = a * b;
        haplo_stack_push(&interpreter->stack, prod);
        break;
      case '/': ;
        int quot;
        err = haplo_stack_pop(&interpreter->stack, &a);
        if (err < 0) return err;
        err = haplo_stack_pop(&interpreter->stack, &b);
        if (err < 0) return err;
        
        quot = a / b;
        haplo_stack_push(&interpreter->stack, quot);
        break;
      case 'p': ; // p for print
        int val;
        err = haplo_stack_pop(&interpreter->stack, &val);
        printf("%d\n", val);
        break;
      default:
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
