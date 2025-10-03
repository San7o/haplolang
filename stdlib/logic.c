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

// and BOOLEAN ...
// Returns: BOOLEAN | ERROR
HAPLO_STD_FUNC_STR(logical_and, "and")
{
  if (haplo_value_list_len(args) < 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }

  HaploValueList *this = args;
  bool result = true;
  while(this != NULL) {

    if (this->val.type != HAPLO_VAL_BOOL) {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .value.error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
    
    result &= this->val.value.boolean;
    this = this->next;
  }

  return (HaploValue) {
    .type = HAPLO_VAL_BOOL,
    .value.boolean = result,
  };
}

// and BOOLEAN ...
// Returns: BOOLEAN | ERROR
HAPLO_STD_FUNC_STR(logical_or, "or")
{
  if (haplo_value_list_len(args) < 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }

  HaploValueList *this = args;
  bool result = false;
  while(this != NULL) {

    if (this->val.type != HAPLO_VAL_BOOL) {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .value.error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
    
    result |= this->val.value.boolean;
    this = this->next;
  }

  return (HaploValue) {
    .type = HAPLO_VAL_BOOL,
    .value.boolean = result,
  };
}
