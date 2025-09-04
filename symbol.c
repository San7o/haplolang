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

#include "symbol.h"
#include "errors.h"
#include "utils.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void haplo_symbol_list_free(HaploSymbolList *list)
{
  if (list == NULL) return;

  HaploSymbolList *next = list->next;
  haplo_symbol_list_free(next);
  free(list->key);
  haplo_symbol_free(list->val);
  free(list);
  
  return;
}

HaploSymbolList *haplo_symbol_list_deep_copy(HaploSymbolList *list)
{
  if (list == NULL) return NULL;

  HaploSymbolList *new_list = malloc(sizeof(HaploSymbolList));
  new_list->val = haplo_symbol_deep_copy(list->val);
  new_list->key = (HaploSymbolKey) malloc(strlen(list->key));
  strcpy(new_list->key, list->key);
  new_list->next = haplo_symbol_list_deep_copy(list->next);
  return new_list;
}

int haplo_symbol_map_init(HaploSymbolMap *map, int capacity)
{
  if (map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NULL;

  map->capacity = capacity;
  map->_map = (HaploSymbolList**) calloc(sizeof(HaploSymbolList*), capacity);
  assert(map->_map != NULL);
  return 0;
}

int haplo_symbol_map_destroy(HaploSymbolMap *map)
{
  if (map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NULL;
  
  if (map->_map != NULL)
  {
    for (int i = 0; i < map->capacity; ++i)
    {
      haplo_symbol_list_free(map->_map[i]);
    }
    map->_map = NULL;
  }

  return 0;
}

void haplo_symbol_free(HaploSymbol symbol)
{
  if (symbol.type == HAPLO_SYMBOL_VARIABLE)
  {
    haplo_value_free(symbol.var);
  }
  return;
}

HaploSymbol haplo_symbol_deep_copy(HaploSymbol symbol)
{
  HaploSymbol new_symbol = {
    .type = symbol.type,
  };
  switch(symbol.type)
  {
  case HAPLO_SYMBOL_FUNCTION:
    new_symbol.func = symbol.func;
    break;
  case HAPLO_SYMBOL_VARIABLE:
    new_symbol.var = haplo_value_deep_copy(symbol.var);
    break;
  }

  return new_symbol;
}

HaploSymbolMap haplo_symbol_map_deep_copy(HaploSymbolMap map)
{
  HaploSymbolMap map_copy = {0};

  if (map.capacity == 0) return map_copy;

  map_copy.capacity = map.capacity;
  map_copy._map = (HaploSymbolList**) calloc(sizeof(HaploSymbolList*), map.capacity);
  for (int i = 0; i < map.capacity; ++i)
  {
    map_copy._map[i] = haplo_symbol_list_deep_copy(map._map[i]);
  }
  
  return map_copy;
}

int haplo_symbol_map_lookup(HaploSymbolMap *map,
                            HaploSymbolKey key,
                            HaploSymbol* symbol)
{
  if (map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_symbol_hash(key, map->capacity);

  HaploSymbolList *symbol_list = map->_map[hash];
  if (symbol_list == NULL) return -HAPLO_ERROR_SYMBOL_MAP_LOOKUP_NOT_FOUND;

  bool found = false;
  while (symbol_list != NULL)
  {
    if (strcmp(key, symbol_list->key) == 0)
    {
      found = true;
      break;
    }
    symbol_list = symbol_list->next;
  }

  if (found)
  {
    *symbol = symbol_list->val;
    return 0;
  }
  
  return -HAPLO_ERROR_SYMBOL_MAP_LOOKUP_NOT_FOUND;
}

int haplo_symbol_map_update(HaploSymbolMap *map,
                            HaploSymbolKey key,
                            HaploSymbol symbol)
{
  if (map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_symbol_hash(key, map->capacity);
  
  HaploSymbolList *symbol_list = map->_map[hash];
  if (symbol_list == NULL)
  {
    symbol_list = (HaploSymbolList*) malloc(sizeof(HaploSymbolList));
    symbol_list->next = NULL;
    symbol_list->val = symbol;
    symbol_list->key = malloc(strlen(key));
    strcpy(symbol_list->key, key);
    map->_map[hash] = symbol_list;    
    return 0;
  }

  if (strcmp(symbol_list->key, key) == 0)
  {
    symbol_list->val = symbol;
    return 1;
  }
  
  bool found = false;
  do {
    symbol_list = symbol_list->next;
    if (strcmp(symbol_list->key, key) == 0)
    {
      found = true;
      break;
    }
  } while (symbol_list->next != NULL);

  if (found)
  {
    symbol_list->val = symbol;
    return 1;
  }

  HaploSymbolList * new_list = (HaploSymbolList*) malloc(sizeof(HaploSymbolList));
  new_list->next = NULL;
  new_list->val = symbol;
  new_list->key = malloc(strlen(key));
  strcpy(new_list->key, key);
  symbol_list->next = new_list;
  
  return 0;
}

int haplo_symbol_map_delete(HaploSymbolMap *map,
                            HaploSymbolKey key)
{
  if (map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_SYMBOL_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_symbol_hash(key, map->capacity);
  
  HaploSymbolList *symbol_list = map->_map[hash];
  if (strcmp(symbol_list->key, key) == 0)
  {
    map->_map[hash] = symbol_list->next;
    free(symbol_list->key);
    free(symbol_list);
    return 0;
  }
  
  bool found = false;
  HaploSymbolList *prev;
  do {
    prev = symbol_list;
    symbol_list = symbol_list->next;
    if (strcmp(symbol_list->key, key) == 0)
    {
      found = true;
      break;
    }
  } while (symbol_list != NULL);
  
  if (found)
  {
    prev->next = symbol_list->next;
    free(symbol_list->key);
    free(symbol_list);
    return 0;
  }
  
  return 0;
}

// Credits to http://www.cse.yorku.ca/~oz/hash.html
unsigned int djb2(const char *bytes, size_t len)
{
  unsigned int hash = 5381;
  for (size_t i = 0; i < len; ++i)
    hash = hash * 33 + bytes[i];
  return hash;
}

unsigned int haplo_symbol_hash(HaploSymbolKey key, int max_value)
{
  return djb2(key, strlen(key)) % max_value;
}
