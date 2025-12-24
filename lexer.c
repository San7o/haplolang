// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "errors.h"
#include "lexer.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

_Static_assert(_HAPLO_LEX_MAX == 7,
              "Number of tokens changed, maybe defaults should be updated?");
HaploTokenChar haplo_default_token_char = {
  [HAPLO_LEX_OPEN] = '(',
  [HAPLO_LEX_CLOSE] = ')',
  [HAPLO_LEX_COMMENT] = '#',
  [HAPLO_LEX_QUOTE] = '\'',
};

_Static_assert(_HAPLO_LEX_MAX == 7,
                "Number of tokens changed, maybe strings need to be updated?");
const char* haplo_lexer_token_string(HaploToken token)
{
  switch(token)
  {
  case HAPLO_LEX_OPEN:
    return "OPEN";
  case HAPLO_LEX_CLOSE:
    return "CLOSE";
  case HAPLO_LEX_ATOM:
    return "ATOM";
  case HAPLO_LEX_COMMENT:
    return "COMMENT";
  case HAPLO_LEX_QUOTE:
    return "QUOTE";
  case HAPLO_LEX_NONE:
    return "NONE";
  case HAPLO_LEX_EOF:
    return "EOF";
  default:
    break;
  }
  return "TOKEN_UNRECOGNIZED";
}

int haplo_lexer_atom_len(HaploLexer *l)
{
  if (!l) return 0;
  if (l->input == NULL || l->cursor >= l->input_size) return 0;

  int len = 0;
  int i = 0;
  
  if (l->input[l->cursor] == '"') {

    i++;
    len++;
    
    while (l->input[l->cursor + i] != '"'
           && l->cursor + i < l->input_size) {
      i++;
      len++;
    }
    if (l->cursor + i == l->input_size)
    {
      return -HAPLO_ERROR_PARSER_STRING_LITERAL_END;
    }
    len++;
  }
  else {
    while (l->input[l->cursor + i] != ' ' &&
           l->input[l->cursor + i] != '\n' &&
           l->input[l->cursor + i] != '\t' &&
           l->input[l->cursor + i] != '(' &&
           l->input[l->cursor + i] != ')' &&
           l->input[l->cursor + i] != '#' &&
           l->cursor + i < l->input_size)
      {
        i++;
        len++;
      }
  }
  return len;
}

int haplo_lexer_init(HaploLexer *l, char* input,
                     unsigned int input_size, HaploTokenChar *token_char)
{
  if (!l) return -HAPLO_ERROR_LEXER_NULL;

  l->input = input;
  l->input_size = input_size;
  l->cursor = 0;
  l->token_char = token_char;
  l->line = 0;
  l->column = 0;
  return 0;
}

int haplo_lexer_trim_left(HaploLexer *l)
{
  if (!l) return -HAPLO_ERROR_LEXER_NULL;
  if (!l->input) return -HAPLO_ERROR_LEXER_INPUT_NULL;

  int start = l->cursor;
  while(l->cursor < l->input_size &&
        (l->input[l->cursor] == ' ' ||
         l->input[l->cursor] == '\n' ||
         l->input[l->cursor] == '\t'))
  {
    if (l->input[l->cursor] == '\n') {
      l->line += 1;
      l->column = 0;
    } else {
      l->column += 1;
    }
    l->cursor++;
  }

  return l->cursor - start;
}

int haplo_lexer_next(HaploLexer *l, HaploToken *tok, HaploAtom *atom)
{
  int ret = haplo_lexer_peek(l, tok, atom);
  if (ret < 0) return ret;
  
  l->cursor += ret;
  l->column += ret;
  return ret;
}

_Static_assert(_HAPLO_LEX_MAX == 7,
              "Number of tokens changed, maybe haplo_lexer_peek needs to be updated?");
int haplo_lexer_peek(HaploLexer *l, HaploToken *tok, HaploAtom *atom)
{
  if (!l) return -HAPLO_ERROR_LEXER_NULL;
  if (!l->input) return -HAPLO_ERROR_LEXER_INPUT_NULL;

  haplo_lexer_trim_left(l);

  if (l->cursor >= l->input_size)
  {
    if (tok) *tok = HAPLO_LEX_EOF;
    return 0;
  }
  
  if (l->input[l->cursor] == (*l->token_char)[HAPLO_LEX_OPEN])
  {
    if (tok) *tok = HAPLO_LEX_OPEN;
    return 1;
  }
  if (l->input[l->cursor] == (*l->token_char)[HAPLO_LEX_CLOSE])
  {
    if (tok) *tok = HAPLO_LEX_CLOSE;
    return 1;
  }
  if (l->input[l->cursor] == (*l->token_char)[HAPLO_LEX_COMMENT])
  {
    if (tok) *tok = HAPLO_LEX_COMMENT;
    return 1;
  }
  if (l->input[l->cursor] == (*l->token_char)[HAPLO_LEX_QUOTE])
  {
    if (tok) *tok = HAPLO_LEX_QUOTE;
    return 1;
  }
  if (l->input[l->cursor] ==  '"') // Atom of type HAPLO_ATOM_STRING
  {
    int ret = haplo_lexer_atom_len(l);
    if (ret < 0) return ret;
    if (ret < 2) return -HAPLO_ERROR_LEXER_ATOM_STRING_SIZE;

    if (atom)
    {
      atom->type = HAPLO_ATOM_STRING;
      atom->value.string = (char *) malloc(ret);
      strncpy(atom->value.string, l->input + l->cursor + 1, ret - 2); // Ignore the '"'
      atom->value.string[ret-1] = '\0';
    }
    if (tok) *tok = HAPLO_LEX_ATOM;
    return ret;
  }

  // Atom may be INTEGER, BOOL or SYMBOL
  int ret = haplo_lexer_atom_len(l);
  if (ret < 0) return ret;

  // Check for BOOL
  if (strncmp(l->input + l->cursor, "true", ret) == 0)
  {
    if (atom)
    {
      atom->type = HAPLO_ATOM_BOOL;
      atom->value.boolean = true;
    }
    if (tok) *tok = HAPLO_LEX_ATOM;
    return ret;
  }
  if (strncmp(l->input + l->cursor, "false", ret) == 0)
  {
    if (atom)
    {
      atom->type = HAPLO_ATOM_BOOL;
      atom->value.boolean = false;
    }
    if (tok) *tok = HAPLO_LEX_ATOM;
    return ret;
  }
  
  // Check for INTEGER
  char *endptr = NULL;
  long integer = strtol(l->input + l->cursor, &endptr, 10);
  if (endptr == l->input + l->cursor + ret)
  {
    if (atom)
    {
      atom->type = HAPLO_ATOM_INTEGER;
      atom->value.integer = integer;
    }
    if (tok) *tok = HAPLO_LEX_ATOM;
    return ret;
  }

  // Check for FLOAT
  double floating_point = strtod(l->input + l->cursor, &endptr);
  if (endptr == l->input + l->cursor + ret)
  {
    if (atom)
    {
      atom->type = HAPLO_ATOM_FLOAT;
      atom->value.floating_point = floating_point;
    }
    if (tok) *tok = HAPLO_LEX_ATOM;
    return ret;
  }

  // Otherwise Atom is SYMBOL
  if (atom)
  {
    atom->type = HAPLO_ATOM_SYMBOL;
    atom->value.symbol = (char *) malloc(ret + 1);
    strncpy(atom->value.symbol, l->input + l->cursor, ret);
    atom->value.symbol[ret] = '\0';
  }
  if (tok) *tok = HAPLO_LEX_ATOM;
  return ret;
}
