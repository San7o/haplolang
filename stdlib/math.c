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

// + INTEGER INTEGER
// Returns: INTEGER
// + FLOAT FLOAT
// Returns: FLOAT
HAPLO_STD_FUNC_STR(plus, "+")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .integer = a.integer + b.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .floating_point = a.floating_point + b.floating_point,
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// - INTEGER INTEGER
// Returns: INTEGER
// - FLOAT FLOAT
// Returns: FLOAT
HAPLO_STD_FUNC_STR(minus, "-")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .integer = a.integer - b.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .floating_point = a.floating_point - b.floating_point,
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// * INTEGER INTEGER
// Returns: INTEGER
// * FLOAT FLOAT
// Returns: FLOAT
HAPLO_STD_FUNC_STR(times, "*")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .integer = a.integer * b.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .floating_point = a.floating_point * b.floating_point,
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// / INTEGER INTEGER
// Returns: INTEGER
// / FLOAT FLOAT
// Returns: FLOAT
HAPLO_STD_FUNC_STR(div, "/")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_INTEGER,
      .integer = a.integer / b.integer,
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_FLOAT,
      .floating_point = a.floating_point / b.floating_point,
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// > INTEGER INTEGER
// Returns: BOOL
// > FLOAT FLOAT
// Returns: BOOL
HAPLO_STD_FUNC_STR(greater, ">")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.integer > b.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.floating_point > b.floating_point),
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// < INTEGER INTEGER
// Returns: BOOL
// < FLOAT FLOAT
// Returns: BOOL
HAPLO_STD_FUNC_STR(lesser, "<")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.integer < b.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.floating_point < b.floating_point),
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// = INTEGER INTEGER
// Returns: BOOL
// = FLOAT FLOAT
// Returns: BOOL
HAPLO_STD_FUNC_STR(equal, "=")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.integer == b.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.floating_point == b.floating_point),
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// >= INTEGER INTEGER
// Returns: BOOL
// >= FLOAT FLOAT
// Returns: BOOL
HAPLO_STD_FUNC_STR(greater_or_equal, ">=")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.integer >= b.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.floating_point >= b.floating_point),
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}

// <= INTEGER INTEGER
// Returns: BOOL
// <= FLOAT FLOAT
// Returns: BOOL
HAPLO_STD_FUNC_STR(lesser_or_equal, "<=")
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

  if (a.type == HAPLO_VAL_INTEGER && b.type == HAPLO_VAL_INTEGER)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.integer <= b.integer),
    };
  } else if (a.type == HAPLO_VAL_FLOAT && b.type == HAPLO_VAL_FLOAT)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_BOOL,
      .boolean = (a.floating_point <= b.floating_point),
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
    .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
  };
}
