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

#include "stdlib.h"
#include "../value.h"
#include "../errors.h"

// list VALUE ...
// Returns: LIST
HAPLO_STD_FUNC(list)
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
    .value.list = new_list,
  };
}

// append VALUE LIST
// Returns: LIST
HAPLO_STD_FUNC(append)
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue val, list;
  val = args->val;
  list = args->next->val;

  if (val.type != HAPLO_VAL_LIST && list.type == HAPLO_VAL_LIST)
  {
    HaploValueList *new_list = haplo_value_list_deep_copy(list.value.list);
    HaploValue new_val = haplo_value_deep_copy(val);
    return (HaploValue) {
      .type = HAPLO_VAL_LIST,
      .value.list = haplo_value_list_push_front(new_val, new_list),
    };
  } else if (list.type == HAPLO_VAL_ERROR)
  {
    return list;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// head LIST
// returns: VALUE
HAPLO_STD_FUNC(head)
{
  if (haplo_value_list_len(args) != 1)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue val;
  val = args->val;

  if (val.type == HAPLO_VAL_LIST)
  {
    HaploValue head, new_head;
    head = val.value.list->val;
    new_head = haplo_value_deep_copy(head);
    return new_head;
  } else if (val.type == HAPLO_VAL_ERROR)
  {
    return val;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// tail LIST
// Returns: LIST
HAPLO_STD_FUNC(tail)
{
  if (haplo_value_list_len(args) != 1)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue val;
  val = args->val;

  if (val.type == HAPLO_VAL_LIST)
  {
    HaploValueList *list, *new_list;
    list = val.value.list->next;
    new_list = haplo_value_list_deep_copy(list);
    return (HaploValue) {
      .type = HAPLO_VAL_LIST,
      .value.list = new_list,
    };
  } else if (val.type == HAPLO_VAL_ERROR)
  {
    return val;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}
