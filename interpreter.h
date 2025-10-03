// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_INTERPRETER_H_
#define _HAPLO_INTERPRETER_H_

#include "expr.h"
#include "value.h"

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Interpreter HaploInterpreter
  #define interpreter_init haplo_interpreter_init
  #define interpreter_destroy haplo_interpreter_destroy
  #define interpreter_interpret haplo_interpreter_interpret
  #define interpreter_interpret_tail haplo_interpreter_interpret_tail
  #define interpreter_call haplo_interpreter_call
#endif // HAPLO_NO_PREFIX

#ifndef HAPLO_INTERPRETER_SYMBOL_MAP_CAPACITY
#define HAPLO_INTERPRETER_SYMBOL_MAP_CAPACITY 1024
#endif // HAPLO_INTERPRETER_SYMBOL_MAP_CAPACITY

// --- Types ---

// Forward declaration to prevent circular dependency
struct HaploSymbolMap;
typedef struct HaploSymbolMap HaploSymbolMap;

typedef struct {
  HaploSymbolMap *symbol_map;
} HaploInterpreter;

// --- Functions ---

int haplo_interpreter_init(HaploInterpreter *interpreter);
void haplo_interpreter_destroy(HaploInterpreter *interpreter);
HaploValue haplo_interpreter_interpret(HaploInterpreter *interpreter,
                                       HaploExpr *expr);
HaploValueList *haplo_interpreter_interpret_tail(HaploInterpreter *interpreter,
                                                  HaploExpr *expr);
HaploValue haplo_interpreter_call(HaploInterpreter *interpreter,
                                  HaploValue value,
                                  HaploValueList *args);
  
#endif // _HAPLO_INTERPRETER_H_
