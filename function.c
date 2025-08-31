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

#include "function.h"
#include "errors.h"
#include "utils.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void haplo_function_list_free(HaploFunctionList *list)
{
  if (list == NULL) return;

  HaploFunctionList *next = list->next;
  haplo_function_list_free(next);
  free(list->key);
  free(list);
  
  return;
}

HaploFunctionList *haplo_function_list_deep_copy(HaploFunctionList *list)
{
  if (list == NULL) return NULL;

  HaploFunctionList *new_list = malloc(sizeof(HaploFunctionList));
  new_list->func = list->func;
  new_list->key = (HaploFunctionKey) malloc(strlen(list->key));
  strcpy(new_list->key, list->key);
  new_list->next = haplo_function_list_deep_copy(list->next);
  return new_list;
}

int haplo_function_map_init(HaploFunctionMap *map, int capacity)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;

  map->capacity = capacity;
  map->_map = (HaploFunctionList**) calloc(sizeof(HaploFunctionList*), capacity);
  assert(map->_map != NULL);
  return 0;
}

int haplo_function_map_destroy(HaploFunctionMap *map)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;
  
  if (map->_map != NULL)
  {
    for (int i = 0; i < map->capacity; ++i)
    {
      haplo_function_list_free(map->_map[i]);
    }
    map->_map = NULL;
  }

  return 0;
}

HaploFunctionMap haplo_function_map_deep_copy(HaploFunctionMap map)
{
  HaploFunctionMap map_copy = {0};

  if (map.capacity == 0) return map_copy;

  map_copy.capacity = map.capacity;
  map_copy._map = (HaploFunctionList**) calloc(sizeof(HaploFunctionList*), map.capacity);
  for (int i = 0; i < map.capacity; ++i)
  {
    map_copy._map[i] = haplo_function_list_deep_copy(map._map[i]);
  }
  
  return map_copy;
}

int haplo_function_map_lookup(HaploFunctionMap *map,
                              HaploFunctionKey key,
                              HaploFunction* func)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_function_hash(key, map->capacity);

  HaploFunctionList *func_list = map->_map[hash];
  if (func_list == NULL) return -HAPLO_ERROR_FUNCTION_MAP_LOOKUP_NOT_FOUND;

  bool found = false;
  while (func_list != NULL)
  {
    if (strcmp(key, func_list->key) == 0)
    {
      found = true;
      break;
    }
    func_list = func_list->next;
  }

  if (found)
  {
    *func = func_list->func;
    return 0;
  }
  
  return -HAPLO_ERROR_FUNCTION_MAP_LOOKUP_NOT_FOUND;
}

int haplo_function_map_update(HaploFunctionMap *map,
                              HaploFunctionKey key,
                              HaploFunction func)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_function_hash(key, map->capacity);
  
  HaploFunctionList *func_list = map->_map[hash];
  if (func_list == NULL)
  {
    func_list = (HaploFunctionList*) malloc(sizeof(HaploFunctionList));
    func_list->next = NULL;
    func_list->func = func;
    func_list->key = malloc(strlen(key));
    strcpy(func_list->key, key);
    map->_map[hash] = func_list;    
    return 0;
  }

  if (strcmp(func_list->key, key) == 0)
  {
    func_list->func = func;
    return 1;
  }
  
  bool found = false;
  do {
    func_list = func_list->next;
    if (strcmp(func_list->key, key) == 0)
    {
      found = true;
      break;
    }
  } while (func_list->next != NULL);

  if (found)
  {
    func_list->func = func;
    return 1;
  }

  HaploFunctionList * new_list = (HaploFunctionList*) malloc(sizeof(HaploFunctionList));
  new_list->next = NULL;
  new_list->func = func;
  new_list->key = malloc(strlen(key));
  strcpy(new_list->key, key);
  func_list->next = new_list;
  
  return 0;
}

int haplo_function_map_delete(HaploFunctionMap *map,
                              HaploFunctionKey key)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;
  if (map->_map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NOT_INITIALIZED;

  unsigned int hash = haplo_function_hash(key, map->capacity);
  
  HaploFunctionList *func_list = map->_map[hash];
  if (strcmp(func_list->key, key) == 0)
  {
    map->_map[hash] = func_list->next;
    free(func_list->key);
    free(func_list);
    return 0;
  }
  
  bool found = false;
  HaploFunctionList *prev;
  do {
    prev = func_list;
    func_list = func_list->next;
    if (strcmp(func_list->key, key) == 0)
    {
      found = true;
      break;
    }
  } while (func_list != NULL);
  
  if (found)
  {
    prev->next = func_list->next;
    free(func_list->key);
    free(func_list);
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

unsigned int haplo_function_hash(HaploFunctionKey key, int max_value)
{
  return djb2(key, strlen(key)) % max_value;
}
