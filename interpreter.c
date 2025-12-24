// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
  if (!interpreter) return HAPLO_ERROR_INTERPRETER_NULL;

  interpreter->symbol_map = haplo_symbol_map_deep_copy(&__haplo_std_symbol_map);
  
  return 0;
}

void haplo_interpreter_destroy(HaploInterpreter *interpreter)
{
  if (!interpreter) return;

  haplo_symbol_map_destroy(interpreter->symbol_map);
  if (interpreter->symbol_map)
  {
    free(interpreter->symbol_map);
    interpreter->symbol_map = NULL;
  }

  return;
}

_Static_assert(_HAPLO_ATOM_MAX == 6,
              "updated HaploAtomType, update haplo_interpreter_eval_atom");
HaploValue haplo_interpreter_eval_atom(HaploAtom atom)
{
  HaploValue new_value = {0};
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING: ;
    char* new_string = (char*) malloc(strlen(atom.value.string)+1);
    strcpy(new_string, atom.value.string);
    new_value.type = HAPLO_VAL_STRING;
    new_value.value.string = new_string;
    break;
  case HAPLO_ATOM_INTEGER:
    new_value.type = HAPLO_VAL_INTEGER;
    new_value.value.integer = atom.value.integer;
    break;
  case HAPLO_ATOM_FLOAT:
    new_value.type = HAPLO_VAL_FLOAT;
    new_value.value.floating_point = atom.value.floating_point;
    break;
  case HAPLO_ATOM_BOOL:
    new_value.type = HAPLO_VAL_BOOL;
    new_value.value.boolean = atom.value.boolean;
    break;
  case HAPLO_ATOM_SYMBOL: ;
    char* new_symbol = (char*) malloc(strlen(atom.value.symbol)+1);
    strcpy(new_symbol, atom.value.symbol);
    new_value.type = HAPLO_VAL_SYMBOL;
    new_value.value.symbol = new_symbol;
    break;
  case HAPLO_ATOM_QUOTE: ;
    char* new_quote = (char*) malloc(strlen(atom.value.quote)+1);
    strcpy(new_quote, atom.value.quote);
    new_value.type = HAPLO_VAL_QUOTE;
    new_value.value.quote = new_quote;
    break;
  default:
    new_value.type = HAPLO_VAL_ERROR;
    new_value.value.error = HAPLO_ERROR_INTERPRETER_INVALID_ATOM;
    break;
  }
  return new_value;
}

HaploValue haplo_interpreter_interpret(HaploInterpreter *interpreter,
                                       HaploExpr *expr)
{
  if (!interpreter)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_NULL,
    };
  }
  if (!expr)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_EMPTY,
    };
  }

  if (expr->is_atom)
    return haplo_interpreter_eval_atom(expr->atom);

  HaploValue out_val = {0};
  HaploValue func = haplo_interpreter_interpret(interpreter, expr->head);

  // Special symbols
  if (func.type == HAPLO_VAL_SYMBOL)
  {
    // If statement. "(if (CONDITION) (CASE TRUE) (CASE FALSE))"
    if (strcmp(func.value.symbol, "if") == 0)
    {
      haplo_value_free(func);
      int expr_depth = haplo_expr_depth(expr->tail);
      if (expr_depth != 3)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
        };
      }
      
      HaploValue condition = haplo_interpreter_interpret(interpreter, expr->tail->head);
      if (condition.type != HAPLO_VAL_BOOL)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
        };
      }

      // Decision
      if (condition.value.boolean)
      {
        out_val = haplo_interpreter_interpret(interpreter, expr->tail->tail->head);
      } else if (expr_depth == 3) {
        out_val = haplo_interpreter_interpret(interpreter, expr->tail->tail->tail->head);
      }
      return out_val;
    }
    // While loop. "(while CONDITION FUNCTION)"
    if (strcmp(func.value.symbol, "while") == 0)
    {
      int expr_depth = haplo_expr_depth(expr->tail);
      if (expr_depth < 2)
      {
        haplo_value_free(func);
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
        };
      }
      haplo_value_free(func);

      bool should_loop = true;
      HaploValue condition = haplo_interpreter_interpret(interpreter, expr->tail->head);
      if (condition.type != HAPLO_VAL_BOOL)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
        };
      }

      should_loop = condition.value.boolean;
      while (should_loop) {
        HaploValue a_val = haplo_interpreter_interpret(interpreter, expr->tail->tail->head);
        haplo_value_free(a_val); // Ignore the return value

        // Update should_loop
        condition = haplo_interpreter_interpret(interpreter, expr->tail->head);
        should_loop = condition.value.boolean;
      }

      return (HaploValue) {
        .type = HAPLO_VAL_EMPTY,
      };
    }
    // Function definition. "(defunc 'FUNCTION_NAME (FUNCTION_BODY))"
    if (strcmp(func.value.symbol, "defunc") == 0)
    {
      haplo_value_free(func);
      if (haplo_expr_depth(expr->tail) != 2)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
        };
      }

      HaploExpr *func_name = expr->tail->head;
      HaploExpr *func_body = expr->tail->tail->head;

      if (!func_name->is_atom || func_name->atom.type != HAPLO_ATOM_QUOTE)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
        };
      }

      // Register function
      HaploSymbol new_function = {
        .type = HAPLO_SYMBOL_FUNCTION,
        .func = func_body,
      };
      int err = haplo_symbol_map_update(interpreter->symbol_map,
                                        func_name->atom.value.quote,
                                        new_function);
      if (err < 0)
      {
        return (HaploValue) {
          .type = HAPLO_VAL_ERROR,
          .value.error = err,
        };
      }
      
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
  if (!interpreter || !expr)
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

_Static_assert(_HAPLO_SYMBOL_MAX == 3,
              "Updated HaploSymbolType, maybe should update haplo_interpreter_call");
// Should not free args here
HaploValue haplo_interpreter_call(HaploInterpreter *interpreter,
                                  HaploValue value,
                                  HaploValueList* args)
{
  if (!interpreter)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_NULL,
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
                                    value.value.symbol,
                                    &symbol);
  if (err < 0)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL,
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
    .value.error = HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL_TYPE,
  };
}
