// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_VALUE_H_
#define _HAPLO_VALUE_H_

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define ValueType HaploValueType
  #define Value HaploValue
  #define ValueList HaploValueList
  #define value_string haplo_value_string
  #define value_list_len haplo_value_list_len
  #define value_list_print haplo_value_list_print
  #define value_list_free haplo_value_list_free
  #define value_list_deep_copy haplo_value_list_deep_copy
  #define value_free haplo_value_free
  #define value_deep_copy haplo_value_deep_copy
  #define value_type_string haplo_value_type_string
#endif

// --- Types ---

typedef enum {
  HAPLO_VAL_INTEGER = 0,
  HAPLO_VAL_FLOAT,
  HAPLO_VAL_STRING,
  HAPLO_VAL_BOOL,
  HAPLO_VAL_SYMBOL,
  HAPLO_VAL_LIST,
  HAPLO_VAL_QUOTE,
  HAPLO_VAL_EMPTY,
  HAPLO_VAL_ERROR,
  _HAPLO_VAL_MAX,
} HaploValueType;

struct HaploValueList;
typedef struct HaploValueList HaploValueList;

typedef struct {
  HaploValueType type;
  union {
    long integer;
    double floating_point;
    char* string;
    bool boolean;
    char* symbol;
    char* quote;
    HaploValueList *list;
    int error;
  } value;
} HaploValue;

struct HaploValueList {
  HaploValueList *next;
  HaploValue val;
};

// --- Functions ---

HaploValueList *haplo_value_list_push_front(HaploValue value,
                                            HaploValueList *list);
// Returns the length of the list
int haplo_value_list_len(HaploValueList *list);
void haplo_value_list_free(HaploValueList *list);
void haplo_value_list_print(HaploValueList *list);
// Returns a deep copy of the argument list
HaploValueList *haplo_value_list_deep_copy(HaploValueList *list);
const char* haplo_value_type_string(HaploValueType type);
// Returns a deep copy of the argument value
HaploValue haplo_value_deep_copy(HaploValue value);
void haplo_value_free(HaploValue value);
// Returns the number of bytes written to buf. At most buf_len bytes
// will be written.
int haplo_value_string(HaploValue value, char* buf, int buf_len);

#endif // _HAPLO_VALUE_H_
