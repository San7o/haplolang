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

void haplo_function_list_free(HaploFunctionList *list)
{
  if (list == NULL) return;

  HaploFunctionList *next = list->next;
  haplo_function_list_free(next);
  free(list);
  
  return;
}

int haplo_function_map_init(HaploFunctionMap *map, int capacity)
{
  if (map == NULL) return -HAPLO_ERROR_FUNCTION_MAP_NULL;

  map->capacity = capacity;
  map->_map = (HaploFunctionList**) calloc(sizeof(HaploFunctionList*), capacity);
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
  }

  return 0;
}

int haplo_function_map_update(HaploFunctionMap *map, HaploFunctionKey key, HaploFunction function)
{
  HAPLO_TODO("haplo_function_map_update");
  return 0;
}

HaploFunction haplo_function_map_lookup(HaploFunctionMap *map, HaploFunctionKey key)
{
  HAPLO_TODO("haplo_function_map_lookup");
  return (HaploFunction){0};
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
