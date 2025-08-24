/*
 * MIT License
 *
 * Copyright (c) 2025 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _HAPLO_INTERPRETER_H_
#define _HAPLO_INTERPRETER_H_

#include "expr.h"
#include "stack.h"

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Interpreter HaploInterpreter
  #define Interpreter_t HaploInterpreter_t
  #define interpreter_init haplo_interpreter_init
  #define interpreter_interpret haplo_interpreter_interpret
  #define interpreter_clean haplo_interpreter_clean
#endif // HAPLO_NO_PREFIX

// --- Types ---

struct HaploInterpreter {
  HaploStack_t stack;
};

typedef struct HaploInterpreter HaploInterpreter_t;

// --- Functions ---

int haplo_interpreter_init(HaploInterpreter_t *interpreter);
int haplo_interpreter_interpret(HaploInterpreter_t *interpreter, HaploExpr_t *expr);
void haplo_interpreter_clean(HaploInterpreter_t *interpreter);

#endif // _HAPLO_INTERPRETER_H_
