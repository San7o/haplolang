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

#ifdef HAPLO_NO_PREFIX
  #define Function HaploFunction
  #define FunctionKey HaploFunctionKey
  #define FunctionList HaploFunctionList
  #define FunctionMap HaploFunctionMap
  #define function_list_free haplo_function_list_free
  #define function_list_deep_copy haplo_function_list_deep_copy
  #define function_map_init haplo_function_map_init
  #define function_map_destroy haplo_function_map_destroy
  #define function_map_deep_copy haplo_function_map_deep_copy
  #define function_map_lookup haplo_function_map_lookup
  #define function_map_update haplo_function_map_update
  #define function_map_delete haplo_function_map_delete
  #define function_hash haplo_function_hash
#endif // HAPLO_NO_PREFIX

// --- Types ---

typedef char* HaploFunctionKey;

typedef struct HaploFunction {
  HaploValue (*func) (HaploValueList *args);
} HaploFunction;

typedef struct HaploFunctionList {
  HaploFunctionKey key;
  HaploFunction func;
  struct HaploFunctionList *next;
} HaploFunctionList;

typedef struct HaploFunctionMap {
  HaploFunctionList **_map;
  int capacity;
} HaploFunctionMap;

// --- Functions ---

void haplo_function_list_free(HaploFunctionList *list);
// Returns a deep copy of the list
HaploFunctionList *haplo_function_list_deep_copy(HaploFunctionList *list);
int haplo_function_map_init(HaploFunctionMap *map, int capacity);
int haplo_function_map_destroy(HaploFunctionMap *map);
// Returns a deep copy of the map
HaploFunctionMap haplo_function_map_deep_copy(HaploFunctionMap map);
// Retuns 0 if key exists in map and fills func with the value if func
// is not null, or returns a negative number representing an error
int haplo_function_map_lookup(HaploFunctionMap *map,
                              HaploFunctionKey key,
                              HaploFunction *func);
// Inserts or updates key with func. Returns 0 for insertions and 1
// for updates, or a negative number representing an error
int haplo_function_map_update(HaploFunctionMap *map,
                              HaploFunctionKey key,
                              HaploFunction func);
// Deletes map entry with key, returns 0 on success or a negative
// number representing an error
int haplo_function_map_delete(HaploFunctionMap *map,
                              HaploFunctionKey key);
// Return the hashed key
unsigned int haplo_function_hash(HaploFunctionKey key, int max_value);

#endif // _HAPLO_FUNCTION_H_
