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

#ifndef _HAPLO_LEXER_H_
#define _HAPLO_LEXER_H_

// --- Macros ---

#ifdef HAPLO_NO_PREFIX
  #define token_string haplo_token_string
  #define atom_len haplo_atom_len
  #define lexer_trim_left haplo_lexer_trim_left
  #define lexer_next_token haplo_lexer_next_token
  #define Token HaploToken
#endif // HAPLO_NO_PREFIX

// --- Types ---

enum HaploToken {
  OPEN = 0,  // '('
  CLOSE,     // ')'
  ATOM,      // any other ASCII char except
             // spaces, newlines and tabs
  COMMENT,   // '#'
  NONE,
};

// --- Functions ---

char* haplo_token_string(enum HaploToken token);
int haplo_atom_len(char* input, int input_size);

// Returns the number of trimmed characters from input. If line and /
// or column are specified, their values will be incremented
// accordingly.
int haplo_lexer_trim_left(char* input, int input_size,
                          unsigned int* line, unsigned int* column);

// On success, returns a non-negative integer representing an
// HaploToken.  On failure, returns a negative integer with the error
// value.  If token_len is specified, its value will be set to the
// length of the token. If token_value is specified and the type of
// token is ATOM, a new Atom will be allocated and the user will be
// responsible to free it.
int haplo_lexer_next_token(char* input, int input_size,
                           int *token_len, char **token_value);

#endif // _HAPLO_LEXER_H_
