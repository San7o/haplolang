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

// Register a test case, It should end with HAPLO_TEST_SUCCESS or
// HAPLO_TEST_FAILURE
// Thanks Sam P.
#define HAPLO_TEST(suiteName, uTtestName)                      \
    static int suiteName##_##uTtestName(void);                 \
    static HaploTest Record_##suiteName##_##uTtestName         \
    __attribute__((used, section(".utest_records"), aligned(sizeof(_Alignof(HaploTest))))) = { \
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
