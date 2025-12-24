// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "stdlib.h"
#include "../value.h"
#include "../errors.h"

#include <stdio.h>

// setq QUOTE VALUE
// Returns: VALUE
HAPLO_STD_FUNC(setq)
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }

  HaploValue first, second;
  first = args->val;
  second = args->next->val;
  
  if (first.type == HAPLO_VAL_QUOTE && second.type != HAPLO_VAL_SYMBOL)
  {
    HaploSymbol var = (HaploSymbol) {
      .type = HAPLO_SYMBOL_VARIABLE,
      .var = haplo_value_deep_copy(second),
    };

    int err = haplo_symbol_map_update(interpreter->symbol_map,
                                      first.value.quote,
                                      var);
    if (err < 0)
    {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .value.error = err,
      };
    }
    
    //printf("Registered symbol: %s\n", first.quote);
    return var.var;
  }

  goto type_error;
 type_error:
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}
