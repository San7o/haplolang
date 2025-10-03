// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_SYMBOL_H_
#define _HAPLO_SYMBOL_H_

#include "value.h"
#include "expr.h"
#include "function.h"

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Symbol HaploSymbol
  #define SymbolKey HaploSymbolKey
  #define SymbolList HaploSymbolList
  #define SymbolMap HaploSymbolMap
  #define symbol_type_string haplo_symbol_type_string
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

typedef enum {
  HAPLO_SYMBOL_C_FUNCTION = 0,   // stdlib
  HAPLO_SYMBOL_FUNCTION,
  HAPLO_SYMBOL_VARIABLE,
  _HAPLO_SYMBOL_MAX,
} HaploSymbolType;

typedef char* HaploSymbolKey;

typedef struct HaploSymbol {
  HaploSymbolType type;
  union {
    HaploFunction c_func;    // function implemented in c
    HaploExpr* func;         // function defined as an AST
    HaploValue var;          // a variable
  };
} HaploSymbol;

struct HaploSymbolList;
typedef struct HaploSymbolList HaploSymbolList;

struct HaploSymbolList {
  HaploSymbolKey key;
  HaploSymbolList *next;
  HaploSymbol val;
};

struct HaploSymbolMap {
  HaploSymbolList **_map;
  int capacity;
};

// --- Functions ---

const char* haplo_symbol_type_string(HaploSymbolType type);
void haplo_symbol_list_free(HaploSymbolList *list);
// Returns a deep copy of the list
HaploSymbolList *haplo_symbol_list_deep_copy(HaploSymbolList *list);
int haplo_symbol_map_init(HaploSymbolMap *map, int capacity);
int haplo_symbol_map_destroy(HaploSymbolMap *map);
void haplo_symbol_free(HaploSymbol symbol);
HaploSymbol haplo_symbol_deep_copy(HaploSymbol symbol);
// Returns a deep copy of the map
HaploSymbolMap *haplo_symbol_map_deep_copy(HaploSymbolMap *map);
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
