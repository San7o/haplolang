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

#ifndef _HAPLO_SYMBOL_H_
#define _HAPLO_SYMBOL_H_

#include "value.h"

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Function HaploFunction
  #define Symbol HaploSymbol
  #define SymbolKey HaploSymbolKey
  #define SymbolList HaploSymbolList
  #define SymbolMap HaploSymbolMap
  #define symbol_list_free haplo_symbol_list_free
  #define symbol_list_deep_copy haplo_symbol_list_deep_copy
  #define symbol_free haplo_symbol_free
  #define symbol_deep_copy haplo_symbol_deep_copy
  #define symbol_map_init haplo_symbol_map_init
  #define symbol_map_destroy haplo_symbol_map_destroy
  #define symbol_map_deep_copy haplo_symbol_map_deep_copy
  #define symbol_map_lookup haplo_symbol_map_lookup
  #define symbol_map_update haplo_symbol_map_update
  #define symbol_map_delete haplo_symbol_map_delete
  #define symbol_hash haplo_symbol_hash
#endif // HAPLO_NO_PREFIX

// --- Types ---

enum HaploSymbolType {
  HAPLO_SYMBOL_FUNCTION = 0,
  HAPLO_SYMBOL_VARIABLE,
};

typedef struct HaploFunction {
  HaploValue (*run) (HaploValueList *args);
} HaploFunction;

typedef char* HaploSymbolKey;

typedef struct HaploSymbol {
  enum HaploSymbolType type;
  union {
    HaploFunction func;
    HaploValue var;
  };
} HaploSymbol;

typedef struct HaploSymbolList {
  HaploSymbolKey key;
  struct HaploSymbolList *next;
  HaploSymbol val;
} HaploSymbolList;

typedef struct HaploSymbolMap {
  HaploSymbolList **_map;
  int capacity;
} HaploSymbolMap;

// --- Functions ---

void haplo_symbol_list_free(HaploSymbolList *list);
// Returns a deep copy of the list
HaploSymbolList *haplo_symbol_list_deep_copy(HaploSymbolList *list);
int haplo_symbol_map_init(HaploSymbolMap *map, int capacity);
int haplo_symbol_map_destroy(HaploSymbolMap *map);
void haplo_symbol_free(HaploSymbol symbol);
HaploSymbol haplo_symbol_deep_copy(HaploSymbol symbol);
// Returns a deep copy of the map
HaploSymbolMap haplo_symbol_map_deep_copy(HaploSymbolMap map);
// Retuns 0 if key exists in map and fills symbol with the value if
// symbol is not null, or returns a negative number representing an
// error
int haplo_symbol_map_lookup(HaploSymbolMap *map,
                            HaploSymbolKey key,
                            HaploSymbol *symbol);
// Inserts or updates key with symbol. Returns 0 for insertions and 1
// for updates, or a negative number representing an error
int haplo_symbol_map_update(HaploSymbolMap *map,
                            HaploSymbolKey key,
                            HaploSymbol symbol);
// Deletes map entry with key, returns 0 on success or a negative
// number representing an error
int haplo_symbol_map_delete(HaploSymbolMap *map,
                            HaploSymbolKey key);
// Return the hashed key
unsigned int haplo_symbol_hash(HaploSymbolKey key, int max_value);

#endif // _HAPLO_SYMBOL_H_
