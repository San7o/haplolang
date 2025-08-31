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

HAPLO_TEST(function_map_test, init_destroy)
{
  FunctionMap map;
  int capacity = 10;
  int err;
  
  err = function_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Function map init returned error %s\n", error_string(err));
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

  err = function_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Function map destroy returned error %s\n", error_string(err));
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

Value my_test_function_1(ValueList *expr)
{
  return (Value){0};
}

HAPLO_TEST(function_map_test, update_lookup)
{
  FunctionMap map;
  int capacity = 10;
  int err;
  
  err = function_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Function map init returned error %s\n", error_string(err));
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
  Function func = { .func = &my_test_function_1 };
  err = haplo_function_map_update(&map, key, func);
  if (err == 1)
  {
    fprintf(stderr, "Function map update returned 1 instead of 0\n");
    function_map_destroy(&map);
    goto test_failed;
  }
  else if (err < 0) {
    fprintf(stderr, "Function map update returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }

  Function lookup_function;
  err = haplo_function_map_lookup(&map, key, &lookup_function);
  if (err < 0)
  {
    fprintf(stderr, "Function map lookup returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_function.func != func.func)
  {
    fprintf(stderr, "Function map lookup returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }
  
  err = function_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Function map destroy returned error %s\n", error_string(err));
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

Value my_test_function_2(ValueList *expr)
{
  return (Value){0};
}

HAPLO_TEST(function_map_test, delete)
{
  FunctionMap map;
  int capacity = 10;
  int err;
  
  err = function_map_init(&map, capacity);
  if (err < 0)
  {
    fprintf(stderr, "Function map init returned error %s\n", error_string(err));
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
  Function func = { .func = &my_test_function_2 };
  err = haplo_function_map_update(&map, key, func);
  if (err == 1)
  {
    fprintf(stderr, "Function map update returned 1 instead of 0\n");
    function_map_destroy(&map);
    goto test_failed;
  }
  else if (err < 0) {
    fprintf(stderr, "Function map update returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }

  Function lookup_function;
  err = haplo_function_map_lookup(&map, key, &lookup_function);
  if (err < 0)
  {
    fprintf(stderr, "Function map lookup returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }
  if (lookup_function.func != func.func)
  {
    fprintf(stderr, "Function map lookup returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }

  err = haplo_function_map_delete(&map, key);
  if (err < 0)
  {
    fprintf(stderr, "Function map delete returned error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }

  err = haplo_function_map_lookup(&map, key, &lookup_function);
  if (err != -HAPLO_ERROR_FUNCTION_MAP_LOOKUP_NOT_FOUND)
  {
    fprintf(stderr, "Function map lookup unexpected error %s\n", error_string(err));
    function_map_destroy(&map);
    goto test_failed;
  }
  
  err = function_map_destroy(&map);
  if (err < 0)
  {
    fprintf(stderr, "Function map destroy returned error %s\n", error_string(err));
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
