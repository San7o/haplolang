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
#include <string.h>

HaploValueList *haplo_value_list_push_front(HaploValue value,
                                              HaploValueList *list)
{
  HaploValueList *new_list =
    (HaploValueList*) malloc(sizeof(HaploValueList));
  new_list->val = value;
    
  if (list == NULL)
  {
    new_list->next = NULL;
    return new_list;
  }

  new_list->next = list;
  return new_list;
}

// TODO: implement this
HaploValueList *haplo_value_list_push_back(HaploValue value,
                                           HaploValueList *list)
{
  HaploValueList *new_list =
    (HaploValueList*) malloc(sizeof(HaploValueList));
  new_list->val = value;
    
  if (list == NULL)
  {
    new_list->next = NULL;
    return new_list;
  }

  list->next = new_list;
  new_list->next = list;
  return new_list;
}

int haplo_value_list_len(HaploValueList *list)
{
  if (list == NULL) return 0;
  return haplo_value_list_len(list->next) + 1;
}

void haplo_value_list_print(HaploValueList *list)
{
  if (list == NULL)
    return;

  char buf[1024] = {0};
  haplo_value_string(list->val, &buf[0], 1024);
  printf("  %s: %s\n", haplo_value_type_string(list->val.type), buf);
  
  haplo_value_list_print(list->next);
  return;
}

void haplo_value_list_free(HaploValueList *list)
{
  if (list == NULL)
    return;

  haplo_value_free(list->val);

  haplo_value_list_free(list->next);
  free(list);
  return;
}

void haplo_value_free(HaploValue value)
{
  switch(value.type)
  {
  case HAPLO_VAL_STRING:
    if (value.string != NULL) free(value.string);
    break;
  case HAPLO_VAL_FUNC:
    if (value.function != NULL) free(value.function);
    break;
  case HAPLO_VAL_LIST:
    if (value.list != NULL) haplo_value_list_free(value.list);
    break;
  default:
    break;
  }
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

HaploValue haplo_value_deep_copy(HaploValue value)
{
  HaploValue new_value = {0};
  switch(value.type)
  {
  case HAPLO_VAL_INTEGER:
    return value;
  case HAPLO_VAL_FLOAT:
    return value;
  case HAPLO_VAL_STRING: ;
    char* new_string = (char*) malloc(strlen(value.string));
    strcpy(new_string, value.string);
    new_value.type = HAPLO_VAL_STRING;
    new_value.string = new_string;
    break;
  case HAPLO_VAL_BOOL:
    return value;
  case HAPLO_VAL_FUNC: ;
    char* new_func = (char*) malloc(strlen(value.function));
    strcpy(new_func, value.function);
    new_value.type = HAPLO_VAL_FUNC;
    new_value.function = new_func;
    break;
  case HAPLO_VAL_LIST:
    return value; // TODO: may need to deepcopy list
  case HAPLO_VAL_EMPTY:
    return value;
  case HAPLO_VAL_ERROR:
    return value;
  }
  return new_value;
}

int haplo_value_string(HaploValue value, char* buf, int buf_len)
{
  if (buf_len <= 0) return 0;
  
  switch(value.type)
  {
  case HAPLO_VAL_INTEGER:
    return snprintf(buf, buf_len, "%ld", value.integer);
  case HAPLO_VAL_FLOAT:
    return snprintf(buf, buf_len, "%f", value.floating_point);
  case HAPLO_VAL_STRING:
    return snprintf(buf, buf_len, "\"%s\"", value.string);
  case HAPLO_VAL_BOOL:
    return snprintf(buf, buf_len, "%s", value.boolean ? "true" : "false");
  case HAPLO_VAL_FUNC:
    return snprintf(buf, buf_len, "%s", value.function);
  case HAPLO_VAL_LIST: ;
    HaploValueList *this = value.list;
    int offset = 0;
    offset += snprintf(buf, buf_len, "list: ");
    while (this != NULL)
    {
      offset += haplo_value_string(this->val, buf + offset, buf_len - offset);
      if (this->next != NULL)
      {
        offset += snprintf(buf + offset, buf_len - offset, ", ");
      }
      this = this->next;
    }
    return offset;
  case HAPLO_VAL_EMPTY:
    return snprintf(buf, buf_len, "empty");
  case HAPLO_VAL_ERROR:
    return snprintf(buf, buf_len, "%s", haplo_error_string(value.error));
  }
  return 0;
}
