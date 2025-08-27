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

int haplo_interpreter_init(HaploInterpreter *interpreter)
{
  if (interpreter == NULL) return -HAPLO_ERROR_INTERPRETER_NULL;
  // For future use
  return 0;
}

void haplo_interpreter_clean(HaploInterpreter *interpreter)
{
  if (interpreter == NULL) return;
  // For future use
  return;
}

HaploValue haplo_interpreter_eval_atom(HaploAtom atom)
{
  HaploValue new_value = {0};
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING: ;
    char* new_string = (char*) malloc(strlen(atom.string));
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
    char* new_symbol = (char*) malloc(strlen(atom.symbol));
    strcpy(new_symbol, atom.symbol);
    new_value.type = HAPLO_VAL_FUNC;
    new_value.function = new_symbol;
    break;
  default:
    new_value.type = HAPLO_VAL_ERROR;
    new_value.error = -HAPLO_ERROR_INTERPRETER_INVALID_ATOM;
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

  HaploValue func = haplo_interpreter_interpret(interpreter, expr->head);
  HaploValueList *args = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  HaploValue val = haplo_interpreter_call(interpreter, func, args);

  haplo_value_list_free(args);
  return val;
}

HaploValueList *haplo_interpreter_interpret_tail(HaploInterpreter *interpreter,
                                                   HaploExpr *expr)
{
  if (interpreter == NULL || expr == NULL)
    return NULL;

  HaploValue head = haplo_interpreter_interpret(interpreter, expr->head);
  HaploValueList *tail = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  if (head.type == HAPLO_VAL_FUNC)
  {
    head = haplo_interpreter_call(interpreter, head, tail);
    haplo_value_list_free(tail);
    return haplo_value_list_push_front(head, NULL);
  }

  return haplo_value_list_push_front(head, tail);
}

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

  // Useful debug prints
  /*
  char buf[HAPLO_VAL_MAX_STRING_LEN] = {0};
  haplo_value_string(value, buf);
  printf("Calling value: %s\n", buf);

  printf("With args:\n");
  haplo_value_list_print(args);
  */

  // TODO: implement proper function calling infrastructure

  if (value.type != HAPLO_VAL_FUNC)
  {
    return value;
  }
  
  if (strcmp(value.function, "+") == 0)
  {
    if (haplo_value_list_len(args) != 2)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
      };
    }
    
    HaploValue a, b;
    a = args->val;
    b = args->next->val;

    if (a.type == HAPLO_VAL_INTEGER || b.type == HAPLO_VAL_INTEGER)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_INTEGER,
        .integer = a.integer + b.integer,
      };
    } else if (a.type == HAPLO_VAL_FLOAT || b.type == HAPLO_VAL_FLOAT)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_FLOAT,
        .floating_point = a.floating_point + b.floating_point,
      };
    } else {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
  }
  else if (strcmp(value.function, "-") == 0)
  {
    if (haplo_value_list_len(args) != 2)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
      };
    }
    
    HaploValue a, b;
    a = args->val;
    b = args->next->val;

    if (a.type == HAPLO_VAL_INTEGER || b.type == HAPLO_VAL_INTEGER)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_INTEGER,
        .integer = a.integer - b.integer,
      };
    } else if (a.type == HAPLO_VAL_FLOAT || b.type == HAPLO_VAL_FLOAT)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_FLOAT,
        .floating_point = a.floating_point - b.floating_point,
      };
    } else {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
  }
  else if (strcmp(value.function, "*") == 0)
  {
    if (haplo_value_list_len(args) != 2)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
      };
    }

    HaploValue a, b;
    a = args->val;
    b = args->next->val;

    if (a.type == HAPLO_VAL_INTEGER || b.type == HAPLO_VAL_INTEGER)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_INTEGER,
        .integer = a.integer * b.integer,
      };
    } else if (a.type == HAPLO_VAL_FLOAT || b.type == HAPLO_VAL_FLOAT)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_FLOAT,
        .floating_point = a.floating_point * b.floating_point,
      };
    } else {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
  }
  else if (strcmp(value.function, "/") == 0)
  {
    if (haplo_value_list_len(args) != 2)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
      };
    }
    
    HaploValue a, b;
    a = args->val;
    b = args->next->val;

    if (a.type == HAPLO_VAL_INTEGER || b.type == HAPLO_VAL_INTEGER)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_INTEGER,
        .integer = a.integer / b.integer,
      };
    } else if (a.type == HAPLO_VAL_FLOAT || b.type == HAPLO_VAL_FLOAT)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_FLOAT,
        .floating_point = a.floating_point / b.floating_point,
      };
    } else {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
  }
  else if (strcmp(value.function, "print") == 0)
  {
    if (haplo_value_list_len(args) != 1)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
      };
    }
    
    HaploValue val;
    val = args->val;
    
    char buf[1024] = {0};
    haplo_value_string(val, &buf[0], 1024);
    printf("%s\n", buf);
  }
  else if (strcmp(value.function, "list") == 0)
  {
    HaploValueList* new_list = NULL;
    HaploValue new_value = {0};
    HaploValueList* this = args;
    while (this != NULL)
    {
      new_value = haplo_value_deep_copy(this->val);
      new_list = haplo_value_list_push_front(new_value, new_list);
      this = this->next;
    }
    
    return (HaploValue) {
      .type = HAPLO_VAL_LIST,
      .list = new_list,
    };
  }
  else {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_UNKNOWN_FUNCTION,
    };
  }

  return (HaploValue) {
    .type = HAPLO_VAL_EMPTY,
  };
}
