// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "stdlib.h"

#include <stddef.h>

HaploSymbolMap __haplo_std_symbol_map = {
  ._map = NULL,
  .capacity = 0,
};

__attribute__((destructor))
static void __haplo_std_symbol_map_free(void) {
  haplo_symbol_map_destroy(&__haplo_std_symbol_map);
}
