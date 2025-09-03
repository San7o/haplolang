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

#ifndef _HAPLO_LEXER_H_
#define _HAPLO_LEXER_H_

#include "atom.h"

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define Token HaploToken
  #define TokenChar HaploTokenChar
  #define default_token_char haplo_default_token_char
  #define lexer_token_string haplo_lexer_token_string
  #define lexer_atom_len haplo_lexer_atom_len
  #define lexer_trim_left haplo_lexer_trim_left
  #define lexer_next_token haplo_lexer_next_token
#endif // HAPLO_NO_PREFIX

// --- Types ---

enum HaploToken {
  HAPLO_LEX_OPEN = 0,  // '('
  HAPLO_LEX_CLOSE,     // ')'
  HAPLO_LEX_ATOM,      // see struct HaploAtom in atom.h
  HAPLO_LEX_COMMENT,   // '#'
  HAPLO_LEX_NONE,
  _HAPLO_LEX_MAX,
};

// HaploTokenChar is used to define a mapping between the HaploToken
// enum and a char
typedef const char HaploTokenChar[_HAPLO_LEX_MAX];
// Default HaploToken to char mapping
extern HaploTokenChar haplo_default_token_char;

// --- Functions ---

// Returns the string representation of a token enum
const char* haplo_lexer_token_string(enum HaploToken token);
// Returns the length of an atom
int haplo_lexer_atom_len(char* input, int input_size);

// Returns the number of trimmed characters from input. If line and /
// or column are specified, their values will be incremented
// accordingly.
int haplo_lexer_trim_left(char* input, int input_size,
                          unsigned int* line, unsigned int* column);

// On success, returns a non-negative integer representing an
// HaploToken.  On failure, returns a negative integer with the error
// value.  If token_len is specified, its value will be set to the
// length of the token. If atom is specified and the type of token is
// ATOM, a new Atom will be allocated and the user will be responsible
// to free it using haplo_atom_free. If token_char is non null, it
// will be used to map the HaploToken enum to a char, otherwise a
// default HaploTokenChar will be used instead
// (haplo_default_token_char).
int haplo_lexer_next_token(char* input, int input_size,
                           int* token_len, HaploAtom *atom,
                           HaploTokenChar token_char);

#endif // _HAPLO_LEXER_H_
