// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "expr.h"
#include "errors.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void haplo_expr_free(HaploExpr *expr)
{
  if (!expr) return;

  if (expr->is_atom)
  {
    haplo_atom_free(&expr->atom);
  } else {
    haplo_expr_free(expr->head);
    haplo_expr_free(expr->tail);
  }
  free(expr);
  return;
}

void haplo_expr_print_rec(HaploExpr *expr)
{
  if (!expr) return;
  
  if (expr->is_atom)
  {
    char buf[HAPLO_ATOM_MAX_STRING_LEN] = {0};
    haplo_atom_string(expr->atom, buf);
    printf("%s", buf);
  }
  else {
    printf("( ");
    haplo_expr_print_rec(expr->head);
    if (expr->tail)
    {
      printf(" ");
      haplo_expr_print_rec(expr->tail);
    }
    printf(" )");
  }
  
  return;
}

void haplo_expr_print(HaploExpr *expr)
{
  haplo_expr_print_rec(expr);
  printf("\n");
  return;
}

HaploExpr *haplo_expr_deep_copy(HaploExpr *expr)
{
  if (!expr) return NULL;

  HaploExpr *new_expr = malloc(sizeof(HaploExpr));
  new_expr->is_atom = expr->is_atom;
  if (expr->is_atom)
  {
    new_expr->atom = haplo_atom_deep_copy(expr->atom);
  } else {
    new_expr->head = haplo_expr_deep_copy(expr->head);
    new_expr->tail = haplo_expr_deep_copy(expr->tail);
  }

  return new_expr;
}

int haplo_expr_depth(HaploExpr *expr)
{
  if (!expr) return 0;
  if (expr->is_atom) return 0;
  return 1 + haplo_expr_depth(expr->tail);
}

void haplo_expr_string_rec(HaploExpr *expr, char *str)
{
  if (!expr) return;
  
  if (expr->is_atom)
  {
    char buf[HAPLO_ATOM_MAX_STRING_LEN] = {0};
    haplo_atom_string(expr->atom, buf);
    strcat(str, buf);
  }
  else {
    strcat(str, "( \0");
    haplo_expr_string_rec(expr->head, str);
    if (expr->tail)
    {
      strcat(str, " ");
      haplo_expr_string_rec(expr->tail, str);
    }
    strcat(str, " )\0");
  }
  
  return;
}

void haplo_expr_string(HaploExpr *expr, char *str)
{
  haplo_expr_string_rec(expr, str);
  strcat(str, "\0");
  return;
}
