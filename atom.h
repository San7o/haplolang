// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_ATOM_H
#define HAPLO_ATOM_H

#include <stdbool.h>

//
// Macros
//

#ifdef HAPLO_NO_PREFIX
  #define AtomType HaploAtomType
  #define Atom HaploAtom
  #define atom_free haplo_atom_free
  #define atom_deep_copy haplo_atom_deep_copy
  #define atom_string haplo_atom_string
#endif // HAPLO_NO_PREFIX

#define HAPLO_ATOM_MAX_STRING_LEN 1024

//
// Types
//

typedef enum {
  HAPLO_ATOM_STRING = 0,    // "Hello World"
  HAPLO_ATOM_INTEGER,       // 69, -420
  HAPLO_ATOM_FLOAT,         // 69.420
  HAPLO_ATOM_BOOL,          // true, false
  HAPLO_ATOM_SYMBOL,        // print, +
  HAPLO_ATOM_QUOTE,         // 'test
  _HAPLO_ATOM_MAX,
} HaploAtomType;

typedef struct {
  HaploAtomType type;
  union {
    char* string;
    long int integer;
    double floating_point;
    bool boolean;
    char* symbol;
    char* quote;
  } value;
} HaploAtom;

//
// Functions
//

void haplo_atom_free(HaploAtom *atom);
HaploAtom haplo_atom_deep_copy(HaploAtom atom);
// Writes to buff the string representation of the atom.
void haplo_atom_string(HaploAtom atom, char buf[HAPLO_ATOM_MAX_STRING_LEN]);

#endif // HAPLO_ATOM_H
