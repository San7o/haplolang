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

#include "errors.h"
#include "value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

HaploValueList *haplo_value_list_deep_copy(HaploValueList *list)
{
  if (list == NULL) return NULL;

  HaploValueList *new_list = malloc(sizeof(HaploValueList));
  new_list->val = haplo_value_deep_copy(list->val);
  new_list->next = haplo_value_list_deep_copy(list->next);
  return new_list;
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

static_assert(_HAPLO_VAL_MAX == 9,
              "Added a new value type, maybe update haplo_value_free");
void haplo_value_free(HaploValue value)
{
  switch(value.type)
  {
  case HAPLO_VAL_STRING:
    if (value.string != NULL) free(value.string);
    break;
  case HAPLO_VAL_QUOTE:
    if (value.quote != NULL) free(value.quote);
    break;
  case HAPLO_VAL_SYMBOL:
    if (value.symbol != NULL) free(value.symbol);
    break;
  case HAPLO_VAL_LIST:
    if (value.list != NULL) haplo_value_list_free(value.list);
    break;
  default:
    break;
  }
  return;
}

static_assert(_HAPLO_VAL_MAX == 9,
              "Added a new value type, update haplo_value_type_string");
const char* haplo_value_type_string(enum HaploValueType type)
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
  case HAPLO_VAL_SYMBOL:
    return "HAPLO_VAL_SYMBOL";
  case HAPLO_VAL_LIST:
    return "HAPLO_VAL_LIST";
  case HAPLO_VAL_QUOTE:
    return "HAPLO_VAL_QUOTE";
  case HAPLO_VAL_EMPTY:
    return "HAPLO_VAL_EMPTY";
  case HAPLO_VAL_ERROR:
    return "HAPLO_VAL_ERROR";
  default:
    break;
  }
  return "HAPLO_VAL_UNKNOWN_VALUE";
}

static_assert(_HAPLO_VAL_MAX == 9,
              "Added a new value type, update haplo_value_deep_copy");
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
  case HAPLO_VAL_SYMBOL: ;
    char* new_symbol = (char*) malloc(strlen(value.symbol));
    strcpy(new_symbol, value.symbol);
    new_value.type = HAPLO_VAL_SYMBOL;
    new_value.symbol = new_symbol;
    break;
  case HAPLO_VAL_LIST:
    new_value.type = HAPLO_VAL_LIST;
    new_value.list = haplo_value_list_deep_copy(value.list);
    break;
  case HAPLO_VAL_QUOTE: ;
    char* new_quote = (char*) malloc(strlen(value.quote));
    strcpy(new_quote, value.quote);
    new_value.type = HAPLO_VAL_QUOTE;
    new_value.string = new_quote;
    break;
  case HAPLO_VAL_EMPTY:
    return value;
  case HAPLO_VAL_ERROR:
    return value;
  default:
    new_value.type = HAPLO_VAL_ERROR;
    new_value.error = -HAPLO_ERROR_VALUE_TYPE_UNRECOGNIZED;
    break;
  }
  return new_value;
}

static_assert(_HAPLO_VAL_MAX == 9,
              "Added a new value type, update haplo_value_string");
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
  case HAPLO_VAL_SYMBOL:
    return snprintf(buf, buf_len, "%s", value.symbol);
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
  case HAPLO_VAL_QUOTE:
    return snprintf(buf, buf_len, "'%s", value.quote);
  case HAPLO_VAL_EMPTY:
    return snprintf(buf, buf_len, "empty");
  case HAPLO_VAL_ERROR:
    return snprintf(buf, buf_len, "Error: %s", haplo_error_string(value.error));
  default:
    break;
  }
  return 0;
}
