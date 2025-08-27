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

#include "errors.h"
#include "value.h"

#include <stdio.h>
#include <stdlib.h>

HaploValueList_t *haplo_value_list_push_front(HaploValue_t value,
                                              HaploValueList_t *list)
{
  HaploValueList_t *new_list =
    (HaploValueList_t*) malloc(sizeof(HaploValueList_t));
  new_list->val = value;
    
  if (list == NULL)
  {
    new_list->next = NULL;
    return new_list;
  }

  new_list->next = list;
  return new_list;
}

int haplo_value_list_len(HaploValueList_t *list)
{
  if (list == NULL) return 0;
  return haplo_value_list_len(list->next) + 1;
}

void haplo_value_list_print(HaploValueList_t *list)
{
  if (list == NULL)
    return;

  char buf[HAPLO_VAL_MAX_STRING_LEN] = {0};
  haplo_value_string(list->val, buf);
  printf("  %s: %s\n", haplo_value_type_string(list->val.type), buf);
  
  haplo_value_list_print(list->next);
  return;
}

void haplo_value_list_free(HaploValueList_t *list)
{
  if (list == NULL)
    return;

  switch(list->val.type)
  {
  case HAPLO_VAL_STRING:
    if (list->val.string != NULL) free(list->val.string);
    break;
  case HAPLO_VAL_FUNC:
    if (list->val.function != NULL) free(list->val.function);
    break;
  case HAPLO_VAL_LIST:
    if (list->val.list != NULL) haplo_value_list_free(list->val.list);
    break;
  default:
    break;
  }

  haplo_value_list_free(list->next);
  free(list);
  return;
}

char* haplo_value_type_string(enum HaploValueType type)
{
  switch(type)
  {
  case HAPLO_VAL_INTEGER:
    return "HAPLO_VAL_INTEGER";
  case HAPLO_VAL_FLOAT:
    return "HAPLO_VAL_FLOAT";
  case HAPLO_VAL_STRING:
    return "HAPLO_VAL_STRING";
  case HAPLO_VAL_BOOL:
    return "HAPLO_VAL_BOOL";
  case HAPLO_VAL_FUNC:
    return "HAPLO_VAL_FUNC";
  case HAPLO_VAL_LIST:
    return "HAPLO_VAL_LIST";
  case HAPLO_VAL_EMPTY:
    return "HAPLO_VAL_EMPTY";
  case HAPLO_VAL_ERROR:
    return "HAPLO_VAL_ERROR";
  }
  return "HAPLO_VAL_UNKNOWN_VALUE";
}

void haplo_value_string(HaploValue_t value, char buf[HAPLO_VAL_MAX_STRING_LEN])
{
  switch(value.type)
  {
  case HAPLO_VAL_INTEGER:
    sprintf(buf, "%ld", value.integer);
    break;
  case HAPLO_VAL_FLOAT:
    sprintf(buf, "%f", value.floating_point);
    break;
  case HAPLO_VAL_STRING:
    sprintf(buf, "\"%s\"", value.string);
    break;
  case HAPLO_VAL_BOOL:
    sprintf(buf, "%s", value.boolean ? "true" : "false");
    break;
  case HAPLO_VAL_FUNC:
    sprintf(buf, "%s", value.function);
    break;
  case HAPLO_VAL_LIST:
    sprintf(buf, "list");
    break;
  case HAPLO_VAL_EMPTY:
    sprintf(buf, "empty");
    break;
  case HAPLO_VAL_ERROR:
    sprintf(buf, "%s", haplo_error_string(value.error));
    break;
  }
  return;
}
