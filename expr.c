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

#include "expr.h"
#include "errors.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void haplo_expr_free(HaploExpr *expr)
{
  if (expr == NULL) return;

  if (expr->is_atom)
  {
    haplo_atom_free(expr->atom);
    return;
  }
  haplo_expr_free(expr->head);
  haplo_expr_free(expr->tail);
  
  return;
}

// Printing / Formatting

void haplo_expr_print_rec(HaploExpr *expr)
{
  if (expr == NULL) return;
  
  if (expr->is_atom)
  {
    char buf[HAPLO_ATOM_MAX_STRING_LEN] = {0};
    haplo_atom_string(expr->atom, buf);
    printf("%s", buf);
  }
  else {
    printf("( ");
    haplo_expr_print_rec(expr->head);
    if (expr->tail != NULL)
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

int haplo_expr_depth(HaploExpr *expr)
{
  if (expr == NULL) return 0;
  if (expr->is_atom) return 0;
  return 1 + haplo_expr_depth(expr->tail);
}

void haplo_expr_string_rec(HaploExpr *expr, char *str)
{
  if (expr == NULL) return;
  
  if (expr->is_atom)
  {
    char buf[HAPLO_ATOM_MAX_STRING_LEN] = {0};
    haplo_atom_string(expr->atom, buf);
    strcat(str, buf);
  }
  else {
    strcat(str, "( \0");
    haplo_expr_string_rec(expr->head, str);
    if (expr->tail != NULL)
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
