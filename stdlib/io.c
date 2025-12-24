// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "stdlib.h"
#include "../value.h"
#include "../errors.h"

#include <stdio.h>

// print *
// Returns: EMPTY
HAPLO_STD_FUNC(print)
{
  if (haplo_value_list_len(args) != 1)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue val;
  val = args->val;
    
  char buf[1024] = {0};
  haplo_value_string(val, &buf[0], 1024);
  printf("%s\n", buf);

  return (HaploValue) {
    .type = HAPLO_VAL_EMPTY,
  };
}
