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

#include "expr.h"
#include "errors.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void haplo_expr_free(HaploExpr_t *expr)
{
  if (expr == NULL) return;

  if (expr->is_atom)
  {
    if (expr->atom != NULL) free(expr->atom);
    return;
  }
  haplo_expr_free(expr->first);
  haplo_expr_free(expr->second);
  
  return;
}

// Printing / Formatting

void haplo_expr_print_rec(HaploExpr_t *expr)
{
  if (expr == NULL) return;
  
  if (expr->is_atom)
  {
    printf("%s", expr->atom);
  }
  else {
    printf("( ");
    haplo_expr_print_rec(expr->first);
    if (expr->second != NULL)
    {
      printf(" ");
      haplo_expr_print_rec(expr->second);
    }
    printf(" )");
  }
  
  return;
}

void haplo_expr_print(HaploExpr_t *expr)
{
  haplo_expr_print_rec(expr);
  printf("\n");
  return;
}

void haplo_expr_string_rec(HaploExpr_t *expr, char *str)
{
  if (expr == NULL) return;
  
  if (expr->is_atom && expr->atom != NULL)
  {
    char buf[MAX_ATOM_SIZE];
    sprintf(buf, "%s", expr->atom);
    strcat(str, buf);
  }
  else {
    strcat(str, "( \0");
    haplo_expr_string_rec(expr->first, str);
    if (expr->second != NULL)
    {
      strcat(str, " ");
      haplo_expr_string_rec(expr->second, str);
    }
    strcat(str, " )\0");
  }
  
  return;
}

void haplo_expr_string(HaploExpr_t *expr, char *str)
{
  haplo_expr_string_rec(expr, str);
  strcat(str, "\0");
  return;
}
