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

#ifndef _HAPLO_FUNCTION_H_
#define _HAPLO_FUNCTION_H_

#include "value.h"

// --- Macros ---

#ifndef HAPLO_NO_PREFIX
  #define FunctionKey HaploFunctionKey
  #define Function HaploFunction
  #define FunctionList HaploFunctionList
  #define FunctionMap HaploFunctionMap
  #define function_list_free haplo_function_list_free
  #define function_map_init haplo_function_map_init
  #define function_map_destroy haplo_function_map_destroy
  #define function_map_update haplo_function_map_update
  #define function_map_lookup haplo_function_map_lookup
  #define function_hash haplo_function_hash
#endif // HAPLO_NO_PREFIX

// --- Types ---

typedef char* HaploFunctionKey;

typedef struct HaploFunction {
  HaploValue (*func) (HaploValueList *args);
} HaploFunction;

typedef struct HaploFunctionList {
  HaploFunction func;
  struct HaploFunctionList *next;
} HaploFunctionList;

typedef struct HaploFunctionMap {
  HaploFunctionList **_map;
  int capacity;
} HaploFunctionMap;

// --- Functions ---

void haplo_function_list_free(HaploFunctionList *list);
int haplo_function_map_init(HaploFunctionMap *map, int capacity);
int haplo_function_map_destroy(HaploFunctionMap *map);
int haplo_function_map_update(HaploFunctionMap *map, HaploFunctionKey key,
                              HaploFunction function);
HaploFunction haplo_function_map_lookup(HaploFunctionMap *map, HaploFunctionKey key);
unsigned int haplo_function_hash(HaploFunctionKey key, int max_value);

#endif // _HAPLO_FUNCTION_H_
