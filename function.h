// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_FUNCTION_H
#define HAPLO_FUNCTION_H

#include "value.h"
#include "interpreter.h"

//
// Types
//

typedef struct {
  HaploValue (*run) (HaploInterpreter *interpreter, HaploValueList *args);
} HaploFunction;

#endif // HAPLO_FUNCTION_H
