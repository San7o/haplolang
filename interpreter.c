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

#include "errors.h"
#include "symbol.h"
#include "stdlib/stdlib.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int haplo_interpreter_init(HaploInterpreter *interpreter)
{
  if (interpreter == NULL) return -HAPLO_ERROR_INTERPRETER_NULL;

  interpreter->symbol_map = haplo_symbol_map_deep_copy(&__haplo_std_symbol_map);
  
  return 0;
}

void haplo_interpreter_destroy(HaploInterpreter *interpreter)
{
  if (interpreter == NULL) return;

  haplo_symbol_map_destroy(interpreter->symbol_map);
  if (interpreter->symbol_map != NULL)
  {
    free(interpreter->symbol_map);
    interpreter->symbol_map = NULL;
  }

  return;
}

static_assert(_HAPLO_ATOM_MAX == 6,
              "updated HaploAtomType, update haplo_interpreter_eval_atom");
HaploValue haplo_interpreter_eval_atom(HaploAtom atom)
{
  HaploValue new_value = {0};
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING: ;
    char* new_string = (char*) malloc(strlen(atom.string)+1);
    strcpy(new_string, atom.string);
    new_value.type = HAPLO_VAL_STRING;
    new_value.string = new_string;
    break;
  case HAPLO_ATOM_INTEGER:
    new_value.type = HAPLO_VAL_INTEGER;
    new_value.integer = atom.integer;
    break;
  case HAPLO_ATOM_FLOAT:
    new_value.type = HAPLO_VAL_FLOAT;
    new_value.floating_point = atom.floating_point;
    break;
  case HAPLO_ATOM_BOOL:
    new_value.type = HAPLO_VAL_BOOL;
    new_value.boolean = atom.boolean;
    break;
  case HAPLO_ATOM_SYMBOL: ;
    char* new_symbol = (char*) malloc(strlen(atom.symbol)+1);
    strcpy(new_symbol, atom.symbol);
    new_value.type = HAPLO_VAL_SYMBOL;
    new_value.symbol = new_symbol;
    break;
  case HAPLO_ATOM_QUOTE: ;
    char* new_quote = (char*) malloc(strlen(atom.quote)+1);
    strcpy(new_quote, atom.quote);
    new_value.type = HAPLO_VAL_QUOTE;
    new_value.quote = new_quote;
    break;
  default:
    new_value.type = HAPLO_VAL_ERROR;
    new_value.error = -HAPLO_ERROR_INTERPRETER_INVALID_ATOM;
    break;
  }
  return new_value;
}

HaploValue haplo_interpreter_interpret(HaploInterpreter *interpreter,
                                       HaploExpr *expr)
{
  if (interpreter == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_NULL,
    };
  }
  if (expr == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_EMPTY,
    };
  }

  if (expr->is_atom)
    return haplo_interpreter_eval_atom(expr->atom);

  HaploValue out_val;
  HaploValue func = haplo_interpreter_interpret(interpreter, expr->head);

  // Special symbols
  if (func.type == HAPLO_VAL_SYMBOL)
  {
    // If statement
    if (strcmp(func.symbol, "if") == 0)
    {
      int expr_depth = haplo_expr_depth(expr->tail);
      if (expr_depth < 2 && expr_depth > 3)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
        };
      }
      
      HaploValue condition = haplo_interpreter_interpret(interpreter, expr->tail->head);
      if (condition.type != HAPLO_VAL_BOOL)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
        };
      }

      // Decision
      if (condition.boolean)
      {
        out_val = haplo_interpreter_interpret(interpreter, expr->tail->tail->head);
      } else if (expr_depth == 3) {
        out_val = haplo_interpreter_interpret(interpreter, expr->tail->tail->tail->head);
      }
      haplo_value_free(func);
      return out_val;
    }
    // Function definition
    if (strcmp(func.symbol, "defunc") == 0)
    {
      if (haplo_expr_depth(expr->tail) != 2)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
        };
      }

      HaploExpr *func_name = expr->tail->head;
      HaploExpr *func_body = expr->tail->tail->head;

      if (!func_name->is_atom || func_name->atom.type != HAPLO_ATOM_QUOTE)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
        };
      }

      // Register function
      HaploSymbol new_function = {
        .type = HAPLO_SYMBOL_FUNCTION,
        .func = func_body,
      };
      int err = haplo_symbol_map_update(interpreter->symbol_map,
                                        func_name->atom.quote,
                                        new_function);
      if (err < 0)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .error = err,
        };
      }
      
      haplo_value_free(func);
      return (HaploValue) {
        .type = HAPLO_VAL_EMPTY,
      };
    }
  }
  
  HaploValueList *args = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  out_val = haplo_interpreter_call(interpreter, func, args);

  haplo_value_free(func);
  haplo_value_list_free(args);
  return out_val;
}

HaploValueList *haplo_interpreter_interpret_tail(HaploInterpreter *interpreter,
                                                   HaploExpr *expr)
{
  if (interpreter == NULL || expr == NULL)
    return NULL;

  HaploValue head = haplo_interpreter_interpret(interpreter, expr->head);
  HaploValueList *tail = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  if (head.type == HAPLO_VAL_SYMBOL)
  {
    head = haplo_interpreter_call(interpreter, head, tail);
    haplo_value_list_free(tail);
    return haplo_value_list_push_front(head, NULL);
  }

  return haplo_value_list_push_front(head, tail);
}

static_assert(_HAPLO_SYMBOL_MAX == 3,
              "Updated HaploSymbolType, maybe should update haplo_interpreter_call");
// Should not free args here
HaploValue haplo_interpreter_call(HaploInterpreter *interpreter,
                                  HaploValue value,
                                  HaploValueList* args)
{
  if (interpreter == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_NULL,
    };
  }

  // Useful debug
  /*
  char buf[1024] = {0};
  haplo_value_string(value, buf, 1024);
  printf("Calling value: %s\n", buf);
  printf("With args:\n");
  haplo_value_list_print(args);
  */

  if (value.type != HAPLO_VAL_SYMBOL)
  {
    return haplo_value_deep_copy(value);
  }

  HaploSymbol symbol;
  int err = haplo_symbol_map_lookup(interpreter->symbol_map,
                                    value.symbol,
                                    &symbol);
  if (err < 0)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL,
    };
  }

  switch(symbol.type)
  {
  case HAPLO_SYMBOL_C_FUNCTION:
    return symbol.c_func.run(interpreter, args);
  case HAPLO_SYMBOL_FUNCTION:
    return haplo_interpreter_interpret(interpreter, symbol.func);
  case HAPLO_SYMBOL_VARIABLE:
    return symbol.var;
  default:
    break;
  }
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .error = -HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL_TYPE,
  };
}
