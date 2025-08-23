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

#ifndef _HAPLO_STACK_H_
#define _HAPLO_STACK_H_

#include "errors.h"

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define stack_init haplo_stack_init
  #define stack_pop haplo_stack_pop
  #define stack_push haplo_stack_push
  #define stack_empty haplo_stack_empty
  #define stack_free haplo_stack_free
  #define List HaploList
  #define List_t HaploList_t
  #define Stack HaploStack
  #define Stack_t HaploStack_t
#endif // HAPLO_NO_PREFIX

// --- Types ---

typedef char* HaploStackType;

struct HaploList {
  struct HaploList *next;
  HaploStackType val;
};

typedef struct HaploList HaploList_t;

struct HaploStack {
  HaploList_t *top;
};

typedef struct HaploStack HaploStack_t;

// --- Functions ---

int haplo_stack_init(HaploStack_t *stack);
int haplo_stack_pop(HaploStack_t *stack, HaploStackType *value);
int haplo_stack_push(HaploStack_t *stack, HaploStackType value);
bool haplo_stack_empty(HaploStack_t *stack);
void haplo_stack_free(HaploStack_t *stack);

#endif // _HAPLO_STACK_H_
