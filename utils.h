// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef _HAPLO_UTILS_H_
#define _HAPLO_UTILS_H_

#include <stdio.h>
#include <stdlib.h>

// --- Macros ----

#define HAPLO_UNUSED(value) (void)(value)
#define HAPLO_TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort(); } while(0)
#define HAPLO_UNREACHABLE(message) do { fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, message); abort(); } while(0)

#define cleanup_free __attribute__ ((cleanup (cleanup_freep)))

// --- Functions ---

static inline void cleanup_freep(void *p) {
    free(*(void **)p);
}

#endif // _HAPLO_UTILS_H_
