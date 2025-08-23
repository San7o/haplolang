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

#ifndef _HAPLO_EXPR_H_
#define _HAPLO_EXPR_H_

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define expr_free haplo_expr_free
  #define expr_print haplo_expr_print
  #define expr_string haplo_expr_string
  #define Atom HaploAtom
  #define Expr HaploExpr
  #define Expr_t HaploExpr_t
#endif // HAPLO_NO_PREFIX

#define MAX_ATOM_SIZE 100

// --- Types ---

// Heap allocated
typedef char* HaploAtom;

struct HaploExpr {
  bool is_atom;
  union {
    HaploAtom atom;
    struct {
      struct HaploExpr* first;
      struct HaploExpr* second;
    };
  };
};

typedef struct HaploExpr HaploExpr_t;

// --- Functions ---

void haplo_expr_free(HaploExpr_t *expr);
void haplo_expr_print(HaploExpr_t *expr);
void haplo_expr_string(HaploExpr_t *expr, char *str);

#endif // _HAPLO_EXPR_H_
