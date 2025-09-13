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

#ifndef _HAPLO_EXPR_H_
#define _HAPLO_EXPR_H_

#include "atom.h"

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Atom HaploAtom
  #define Expr HaploExpr
  #define expr_free haplo_expr_free
  #define expr_print haplo_expr_print
  #define expr_deep_copy haplo_expr_deep_copy
  #define expr_depth haplo_expr_depth
  #define expr_string haplo_expr_string
#endif // HAPLO_NO_PREFIX

// --- Types ---

typedef struct HaploExpr {
  bool is_atom;
  union {
    HaploAtom atom;
    struct {
      struct HaploExpr* head;
      struct HaploExpr* tail;
    };
  };
} HaploExpr;

// --- Functions ---

void haplo_expr_free(HaploExpr *expr);
void haplo_expr_print(HaploExpr *expr);
HaploExpr *haplo_expr_deep_copy(HaploExpr *expr);
// Returns the rightmost depth of the expression
int haplo_expr_depth(HaploExpr *expr);
void haplo_expr_string(HaploExpr *expr, char *str);

#endif // _HAPLO_EXPR_H_
