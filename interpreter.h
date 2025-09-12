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

typedef struct HaploInterpreter {
  struct HaploSymbolMap *symbol_map;
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
