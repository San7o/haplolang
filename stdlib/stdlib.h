// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_STDLIB_H_
#define _HAPLO_STDLIB_H_

#include "../value.h"
#include "../symbol.h"
#include "../interpreter.h"

#include <stddef.h>

extern HaploSymbolMap __haplo_std_symbol_map;

#define HAPLO_STD_FUNC(fn)    \
  HAPLO_STD_FUNC_STR(fn, #fn)

#define HAPLO_STD_FUNC_STR(fn, func_string)    \
    HaploValue __haplo_std_##fn(HaploInterpreter *, HaploValueList *); \
    __attribute__((constructor)) static void __haplo_std_register_##fn(void) \
    {                                                \
      if (__haplo_std_symbol_map._map == NULL) \
      { \
        haplo_symbol_map_init(&__haplo_std_symbol_map, \
                              HAPLO_INTERPRETER_SYMBOL_MAP_CAPACITY); \
      } \
      haplo_symbol_map_update(&__haplo_std_symbol_map,\
                              func_string,            \
                              (HaploSymbol){ \
                                .type = HAPLO_SYMBOL_C_FUNCTION,  \
                                .c_func = (HaploFunction) {\
                                  .run = __haplo_std_##fn \
                                }               \
                              });               \
    } \
    HaploValue __haplo_std_##fn(HaploInterpreter *interpreter, HaploValueList *args)
                                
#endif // _HAPLO_STDLIB_H_
