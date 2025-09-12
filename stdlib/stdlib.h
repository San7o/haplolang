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
                                .type = HAPLO_SYMBOL_FUNCTION,  \
                                .func = (HaploFunction) {\
                                  .run = __haplo_std_##fn \
                                }               \
                              });               \
    } \
    HaploValue __haplo_std_##fn(HaploInterpreter *interpreter, HaploValueList *args)
                                
#endif // _HAPLO_STDLIB_H_
