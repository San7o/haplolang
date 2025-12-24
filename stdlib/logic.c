// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
  while(this) {

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
  while(this) {

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
