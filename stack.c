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

#include "stack.h"

#include <stdlib.h>

int haplo_stack_init(HaploStack_t *stack)
{
  if (stack == NULL) return -HAPLO_ERROR_STACK_NULL;

  stack->top = NULL;
  
  return 0;
}

int haplo_stack_pop(HaploStack_t *stack, HaploAtom_t *value)
{
  if (stack == NULL) return -HAPLO_ERROR_STACK_NULL;
  if (stack->top == NULL) return -HAPLO_ERROR_STACK_EMPTY;
  
  *value = stack->top->val;
  HaploList_t *tmp = stack->top;
  stack->top = stack->top->next;
  free(tmp);
  
  return 0;
}

int haplo_stack_push(HaploStack_t *stack, HaploAtom_t value)
{
  if (stack == NULL) return -HAPLO_ERROR_STACK_NULL;

  if (stack->top == NULL)
  {
    stack->top = (HaploList_t*) malloc(sizeof(HaploList_t));
    stack->top->next = NULL;
    stack->top->val = value;
  } else {
    HaploList_t *new_item = (HaploList_t*) malloc(sizeof(HaploList_t));
    new_item->next = stack->top;
    new_item->val = value;
    stack->top = new_item;
  }
  
  return 0;
}

bool haplo_stack_empty(HaploStack_t *stack)
{
  if (stack == NULL) return -HAPLO_ERROR_STACK_NULL;
  return stack->top == NULL;
}

void haplo_list_free_rec(HaploList_t *list)
{
  if (list == NULL) return;
  haplo_list_free_rec(list->next);
  haplo_atom_free(list->val);
  free(list);
  
  return;
}

void haplo_stack_free(HaploStack_t *stack)
{
  if (stack->top == NULL) return;
  haplo_list_free_rec(stack->top);
  stack->top = NULL;

  return;
}
