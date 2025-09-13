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

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>

HAPLO_TEST(symbol_map_test, init_destroy)
{
  SymbolMap map;
  int capacity = 10;
  int err;
  
  err = symbol_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map init returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map == NULL)
  {
    fprintf(stderr, "Error map is null\n");
    goto test_failed;
  }
  if (map.capacity != 10)
  {
    fprintf(stderr, "Error capacity is %d instead of %d\n", map.capacity, capacity);
    goto test_failed;
  }

  err = symbol_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map destroy returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map != NULL)
  {
    fprintf(stderr, "Error destroy did not set the map to null");
    goto test_failed;
  }
  
  HAPLO_TEST_SUCCESS;
 test_failed:
  HAPLO_TEST_FAILED;
}

Value my_test_symbol_1(Interpreter *interpreter, ValueList *expr)
{
  return (Value){0};
}

HAPLO_TEST(symbol_map_test, update_lookup)
{
  SymbolMap map;
  int capacity = 10;
  int err;
  
  err = symbol_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map init returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map == NULL)
  {
    fprintf(stderr, "Error map is null\n");
    goto test_failed;
  }
  if (map.capacity != 10)
  {
    fprintf(stderr, "Error capacity is %d instead of %d\n", map.capacity, capacity);
    goto test_failed;
  }

  char* key = "test";
  Symbol symbol = {
    .type = HAPLO_SYMBOL_C_FUNCTION,
    .c_func = (HaploFunction) {
      .run = &my_test_symbol_1,
    },
  };
  err = haplo_symbol_map_update(&map, key, symbol);
  if (err == 1)
  {
    fprintf(stderr, "Symbol map update returned 1 instead of 0\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }
  else if (err < 0) {
    fprintf(stderr, "Symbol map update returned error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }

  Symbol lookup_symbol;
  err = haplo_symbol_map_lookup(&map, key, &lookup_symbol);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map lookup returned error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_symbol.type != HAPLO_SYMBOL_C_FUNCTION)
  {
    fprintf(stderr, "Symbol map lookup type is not correct\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_symbol.c_func.run != symbol.c_func.run)
  {
    fprintf(stderr, "Symbol map lookup function does not match\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }
  
  err = symbol_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map destroy returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map != NULL)
  {
    fprintf(stderr, "Error destroy did not set the map to null");
    goto test_failed;
  }
  
  HAPLO_TEST_SUCCESS;
 test_failed:
  HAPLO_TEST_FAILED;
}

Value my_test_symbol_2(Interpreter *interpreter, ValueList *expr)
{
  return (Value){0};
}

HAPLO_TEST(symbol_map_test, delete)
{
  SymbolMap map;
  int capacity = 10;
  int err;
  
  err = symbol_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map init returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map == NULL)
  {
    fprintf(stderr, "Error map is null\n");
    goto test_failed;
  }
  if (map.capacity != 10)
  {
    fprintf(stderr, "Error capacity is %d instead of %d\n", map.capacity, capacity);
    goto test_failed;
  }

  char* key = "test_with_a_decently_long_name";
  Symbol symbol = {
    .type = HAPLO_SYMBOL_C_FUNCTION,
    .c_func = (HaploFunction) {
      .run = &my_test_symbol_2,
    },
  };
  err = haplo_symbol_map_update(&map, key, symbol);
  if (err == 1)
  {
    fprintf(stderr, "Symbol map update returned 1 instead of 0\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }
  else if (err < 0) {
    fprintf(stderr, "Symbol map update returned error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }

  Symbol lookup_symbol;
  err = haplo_symbol_map_lookup(&map, key, &lookup_symbol);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map lookup returned error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_symbol.type != HAPLO_SYMBOL_C_FUNCTION)
  {
    fprintf(stderr, "Symbol map lookup type does not match\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_symbol.c_func.run != symbol.c_func.run)
  {
    fprintf(stderr, "Symbol map lookup function does not match\n");
    symbol_map_destroy(&map);
    goto test_failed;
  }

  err = haplo_symbol_map_delete(&map, key);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map delete returned error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }

  err = haplo_symbol_map_lookup(&map, key, &lookup_symbol);
  if (err != -HAPLO_ERROR_SYMBOL_MAP_LOOKUP_NOT_FOUND)
  {
    fprintf(stderr, "Symbol map lookup unexpected error %s\n", error_string(err));
    symbol_map_destroy(&map);
    goto test_failed;
  }
  
  err = symbol_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Symbol map destroy returned error %s\n", error_string(err));
    goto test_failed;
  }
  if (map._map != NULL)
  {
    fprintf(stderr, "Error destroy did not set the map to null");
    goto test_failed;
  }
  
  HAPLO_TEST_SUCCESS;
 test_failed:
  HAPLO_TEST_FAILED;
}
