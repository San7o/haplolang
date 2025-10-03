// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_FUNCTION_H_
#define _HAPLO_FUNCTION_H_

#include "value.h"
#include "interpreter.h"

// --- Types ---

typedef struct {
  HaploValue (*run) (HaploInterpreter *interpreter, HaploValueList *args);
} HaploFunction;

#endif // _HAPLO_FUNCTION_H_
