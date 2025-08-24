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

#ifndef _HAPLO_ATOM_H
#define _HAPLO_ATOM_H

#include <stdbool.h>

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Atom HaploAtom
  #define Atom_t HaploAtom_t
  #define atom_free haplo_atom_free
#endif // HAPLO_NO_PREFIX

#define MAX_ATOM_SIZE 100

// --- Types ---

enum HaploAtomType {
  STRING = 0,    // "Hello World"
  INTEGER,       // 69, -420
  BOOL,          // true, false
  SYMBOL,        // print, +
};

struct HaploAtom {
  enum HaploAtomType type;
  union {
    char* string;
    long int integer;
    bool boolean;
    char* symbol;
  };
};

typedef struct HaploAtom HaploAtom_t;

// --- Functios ---

void haplo_atom_free(HaploAtom_t atom);
// Writes to buff the string representation of the atom.
void haplo_atom_string(HaploAtom_t atom, char buf[MAX_ATOM_SIZE]);

#endif // _HAPLO_ATOM_H
