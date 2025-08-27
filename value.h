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

#ifndef _HAPLO_VALUE_H_
#define _HAPLO_VALUE_H_

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define ValueType HaploValueType
  #define Value HaploValue
  #define Value_t HaploValue_t
  #define ValueList HaploValueList
  #define ValueList_t HaploValueList_t
  #define value_string haplo_value_string
  #define value_list_len haplo_value_list_len
  #define value_list_print haplo_value_list_print
  #define value_list_free haplo_value_list_free
  #define value_type_string haplo_value_type_string
#endif

#define HAPLO_VAL_MAX_STRING_LEN 1024

// --- Types ---

enum HaploValueType {
  HAPLO_VAL_INTEGER = 0,
  HAPLO_VAL_FLOAT,
  HAPLO_VAL_STRING,
  HAPLO_VAL_BOOL,
  HAPLO_VAL_FUNC,
  HAPLO_VAL_LIST,
  HAPLO_VAL_EMPTY,
  HAPLO_VAL_ERROR,
};

struct HaploValueList;
typedef struct HaploValueList HaploValueList_t;

struct HaploValue {
  enum HaploValueType type;
  union {
    long integer;
    double floating_point;
    char* string;
    bool boolean;
    char* function;
    HaploValueList_t *list;
    int error;
  };
};

typedef struct HaploValue HaploValue_t;

struct HaploValueList {
  struct HaploValueList *next;
  HaploValue_t val;
};

// --- Functions ---

HaploValueList_t *haplo_value_list_push_front(HaploValue_t value,
                                              HaploValueList_t *list);
// Returns the length of the list
int haplo_value_list_len(HaploValueList_t *list);
void haplo_value_list_free(HaploValueList_t *list);
void haplo_value_list_print(HaploValueList_t *list);
char* haplo_value_type_string(enum HaploValueType type);
void haplo_value_string(HaploValue_t value, char buf[HAPLO_VAL_MAX_STRING_LEN]);

#endif // _HAPLO_VALUE_H_
