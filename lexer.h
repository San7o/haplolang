// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef HAPLO_LEXER_H
#define HAPLO_LEXER_H

#include "atom.h"

//
// Macros
//

#ifdef HAPLO_NO_PREFIX
  #define Token HaploToken
  #define TokenChar HaploTokenChar
  #define Lexer HaploLexer
  #define default_token_char haplo_default_token_char
  #define lexer_init haplo_lexer_init
  #define lexer_token_string haplo_lexer_token_string
  #define lexer_atom_len haplo_lexer_atom_len
  #define lexer_trim_left haplo_lexer_trim_left
  #define lexer_next haplo_lexer_next
  #define lexer_peek haplo_lexer_peek
#endif // HAPLO_NO_PREFIX

//
// Types
//

typedef enum {
  HAPLO_LEX_OPEN = 0,  // '('
  HAPLO_LEX_CLOSE,     // ')'
  HAPLO_LEX_ATOM,      // see struct HaploAtom in atom.h
  HAPLO_LEX_COMMENT,   // '#'
  HAPLO_LEX_QUOTE,     // '''
  HAPLO_LEX_NONE,
  HAPLO_LEX_EOF,
  _HAPLO_LEX_MAX,
} HaploToken;

// HaploTokenChar is used to define a mapping between the HaploToken
// enum and a char
typedef const char HaploTokenChar[_HAPLO_LEX_MAX];
// Default HaploToken to char mapping
extern HaploTokenChar haplo_default_token_char;
  
typedef struct {
  char *input;
  unsigned int input_size;
  unsigned int cursor;
  HaploTokenChar *token_char;
  unsigned int line;
  unsigned int column;
} HaploLexer;

//
// Functions
//

int haplo_lexer_init(HaploLexer *l, char* input,
                     unsigned int input_size, HaploTokenChar *token_char);
// On success, returns the number of bytes lexed and sets [tok] to the
// lexed token and updates [atom] if the token is an atom.
// Returns a negative error on failure, or 0 on end of input.
int haplo_lexer_next(HaploLexer *l, HaploToken *tok, HaploAtom *atom);
// Similar to haplo_lexer_next, but does not advance the cursor
int haplo_lexer_peek(HaploLexer *l, HaploToken *tok, HaploAtom *atom);
// Returns the number of trimmed bytes, or a negative error.
int haplo_lexer_trim_left(HaploLexer *l);
// Returns the string representation of a token enum
const char* haplo_lexer_token_string(HaploToken token);
// Returns the length of an atom, does not advance the lexer
int haplo_lexer_atom_len(HaploLexer *l);

#endif // HAPLO_LEXER_H
