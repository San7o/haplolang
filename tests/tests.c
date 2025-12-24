// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>

extern char __start_utest_records[];
extern char __stop_utest_records[];

int main(void)
{
  int out = 0;
  printf("Running tests...\n");

  size_t count = (__stop_utest_records - __start_utest_records) / sizeof(HaploTest);
  HaploTest* test = (HaploTest*)__start_utest_records;
  
  if (DEBUG_PRINT)
  {
    printf("start=%p stop=%p count=%zu\n",
           (void*)__start_utest_records,
           (void*)__stop_utest_records,
           count);
  }

  for (size_t i = 0; i < count; i++)
  {
    HaploTest* current = &test[i];
    
    if (current->marker == 0xDeadBeaf)
    {
      int ret = current->functionPointer();       // Execute the test.
      if (ret < 0)
      {
        fprintf(stderr, "ERR: %s\n", current->testName);
      } else {
        printf("OK: %s\n", current->testName);
      }
      out += ret;
    }
  }
  
  printf("Tests done.\n");

  return out;
}
