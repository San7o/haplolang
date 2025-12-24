// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_TESTS_H_
#define _HAPLO_TESTS_H_

#include <stddef.h>
#include <stdint.h>

// --- Macros ---

// Enable or disable debug printing
#define DEBUG_PRINT 0

#define HAPLO_ASSERT(...)     \
  do                          \
  {                           \
    if (!(__VA_ARGS__))	      \
    {                         \
      return -1;              \
    }                         \
  }                           \
  while(0)

#if __STDC_VERSION__ >= 201112L
  #define __ALIGNOF(T) _Alignof(T)
#else
  #define __ALIGNOF(T) __alignof__(T)
#endif

// Register a test case, It should end with HAPLO_TEST_SUCCESS or
// HAPLO_TEST_FAILED
// Thanks Sam P.
#define HAPLO_TEST(suiteName, uTtestName)                      \
    static int suiteName##_##uTtestName(void);                 \
    static HaploTest Record_##suiteName##_##uTtestName         \
    __attribute__((used, section(".utest_records"), aligned(sizeof(__ALIGNOF(HaploTest))))) = { \
        .marker = 0xDeadBeaf,                                  \
        .testSuite = #suiteName,                               \
        .functionName = #uTtestName,                           \
        .testName = #suiteName "_" #uTtestName,	               \
        .fileName = __FILE__,                                  \
        .lineNumber = __LINE__,                                \
        .functionPointer = suiteName##_##uTtestName            \
    };                                                         \
    static int suiteName##_##uTtestName(void)

#define HAPLO_TEST_SUCCESS \
  do { return 0; } while(0)

#define HAPLO_TEST_FAILED \
  do { return -1; } while(0)

    
// --- Types ---

// Each test will create this struct in the section .utest_records
typedef struct __attribute__((packed)) {
    uint32_t marker;                   // To validate the Test
    const char* testSuite;
    const char* functionName;
    const char* testName;
    const char* fileName;
    uint32_t lineNumber;
    int (*functionPointer)(void);

} HaploTest;

#endif // _HAPLO_TESTS_H_
