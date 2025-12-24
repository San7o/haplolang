// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "stdlib.h"
#include "../value.h"
#include "../errors.h"

#include <stdio.h>

// + INTEGER INTEGER
// Returns: INTEGER | ERROR
// + FLOAT FLOAT
// Returns: FLOAT | ERROR
HAPLO_STD_FUNC_STR(plus, "+")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .value.integer = a.value.integer + b.value.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .value.floating_point = a.value.floating_point + b.value.floating_point,
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
    
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// - INTEGER INTEGER
// Returns: INTEGER | ERROR
// - FLOAT FLOAT
// Returns: FLOAT | ERROR
HAPLO_STD_FUNC_STR(minus, "-")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .value.integer = a.value.integer - b.value.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .value.floating_point = a.value.floating_point - b.value.floating_point,
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// * INTEGER INTEGER
// Returns: INTEGER | ERROR
// * FLOAT FLOAT
// Returns: FLOAT | ERROR
HAPLO_STD_FUNC_STR(times, "*")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }

  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .value.integer = a.value.integer * b.value.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .value.floating_point = a.value.floating_point * b.value.floating_point,
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
    
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// / INTEGER INTEGER
// Returns: INTEGER | ERROR
// / FLOAT FLOAT
// Returns: FLOAT | ERROR
HAPLO_STD_FUNC_STR(div, "/")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .value.integer = a.value.integer / b.value.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .value.floating_point = a.value.floating_point / b.value.floating_point,
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// > INTEGER INTEGER
// Returns: BOOL | ERROR
// > FLOAT FLOAT
// Returns: BOOL | ERROR
HAPLO_STD_FUNC_STR(greater, ">")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.integer > b.value.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.floating_point > b.value.floating_point),
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// < INTEGER INTEGER
// Returns: BOOL | ERROR
// < FLOAT FLOAT
// Returns: BOOL | ERROR
HAPLO_STD_FUNC_STR(lesser, "<")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.integer < b.value.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.floating_point < b.value.floating_point),
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// = INTEGER INTEGER
// Returns: BOOL | ERROR
// = FLOAT FLOAT
// Returns: BOOL | ERROR
HAPLO_STD_FUNC_STR(equal, "=")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.integer == b.value.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.floating_point == b.value.floating_point),
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// >= INTEGER INTEGER
// Returns: BOOL | ERROR
// >= FLOAT FLOAT
// Returns: BOOL | ERROR
HAPLO_STD_FUNC_STR(greater_or_equal, ">=")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.integer >= b.value.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.floating_point >= b.value.floating_point),
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// <= INTEGER INTEGER
// Returns: BOOL | ERROR
// <= FLOAT FLOAT
// Returns: BOOL | ERROR
HAPLO_STD_FUNC_STR(lesser_or_equal, "<=")
{
  if (haplo_value_list_len(args) != 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .value.error = HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }
    
  HaploValue a, b;
  a = args->val;
  b = args->next->val;

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.integer <= b.value.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .value.boolean = (a.value.floating_point <= b.value.floating_point),
    };
  } else if (b.type == HAPLO_VAL_ERROR)
  {
    return b;
  } else if (a.type == HAPLO_VAL_ERROR)
  {
    return a;
  }
  
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .value.error = HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}
