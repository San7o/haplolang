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
  #define AtomType HaploAtomType
  #define Atom HaploAtom
  #define atom_free haplo_atom_free
#endif // HAPLO_NO_PREFIX

#define HAPLO_ATOM_MAX_STRING_LEN 1024
// --- Types ---

enum HaploAtomType {
  HAPLO_ATOM_STRING = 0,    // "Hello World"
  HAPLO_ATOM_INTEGER,       // 69, -420
  HAPLO_ATOM_FLOAT,         // 69.420
  HAPLO_ATOM_BOOL,          // true, false
  HAPLO_ATOM_SYMBOL,        // print, +
};

typedef struct HaploAtom {
  enum HaploAtomType type;
  union {
    char* string;
    long int integer;
    double floating_point;
    bool boolean;
    char* symbol;
  };
} HaploAtom;

// --- Functios ---

void haplo_atom_free(HaploAtom atom);
// Writes to buff the string representation of the atom.
void haplo_atom_string(HaploAtom atom, char buf[HAPLO_ATOM_MAX_STRING_LEN]);

#endif // _HAPLO_ATOM_H
