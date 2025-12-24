// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_EXPR_H
#define HAPLO_EXPR_H

#include "atom.h"

#include <stdbool.h>

//
// Macros
//

#ifdef HAPLO_NO_PREFIX
  #define Expr HaploExpr
  #define expr_free haplo_expr_free
  #define expr_print haplo_expr_print
  #define expr_deep_copy haplo_expr_deep_copy
  #define expr_depth haplo_expr_depth
  #define expr_string haplo_expr_string
#endif // HAPLO_NO_PREFIX

//
// Types
//

struct HaploExpr;
typedef struct HaploExpr HaploExpr;

struct HaploExpr {
  bool is_atom;
  HaploAtom atom;
  HaploExpr* head;
  HaploExpr* tail;
};

//
// Functions
//

void haplo_expr_free(HaploExpr *expr);
void haplo_expr_print(HaploExpr *expr);
HaploExpr *haplo_expr_deep_copy(HaploExpr *expr);
// Returns the rightmost depth of the expression
int haplo_expr_depth(HaploExpr *expr);
void haplo_expr_string(HaploExpr *expr, char *str);

#endif // HAPLO_EXPR_H
