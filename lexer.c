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

#include "errors.h"
#include "lexer.h"

#include <string.h>
#include <stdlib.h>

char* haplo_lexer_token_string(enum HaploToken token)
{
  switch(token)
  {
  case OPEN:
    return "OPEN";
  case CLOSE:
    return "CLOSE";
  case ATOM:
    return "ATOM";
  case COMMENT:
    return "COMMENT";
  case NONE:
    return "NONE";
  }
  return "TOKEN_UNRECOGNIZED";
}

int haplo_lexer_atom_len(char* input, int input_size)
{
  if (input == NULL || input_size == 0) return 0;

  int len = 0;
  int i = 0;
  
  if (input[0] == '"') {

    i++;
    len++;
    
    while (input[i] != '"' && i < input_size) {
      i++;
      len++;
    }
    if (i == input_size)
    {
      return -HAPLO_ERROR_PARSER_STRING_LITERAL_END;
    }
    len++;
  }
  else {
    while (input[i] != ' ' &&
           input[i] != '\n' &&
           input[i] != '\t' &&
           input[i] != '(' &&
           input[i] != ')' &&
           input[i] != '#' &&
           i < input_size)
      {
        i++;
        len++;
      }
  }
  return len;
}

int haplo_lexer_trim_left(char* input, int input_size,
                          unsigned int* line, unsigned int* column)
{
  if (input == NULL || input_size == 0) return 0;

  int pos = 0;
  while(pos < input_size &&
        (input[pos] == ' ' ||
         input[pos] == '\n' ||
         input[pos] == '\t'))
  {
    if (input[pos] == '\n') {
      if (line != NULL) *line += 1;
      if (column != NULL) *column = 0;
    } else {
      if (column != NULL) *column += 1;
    }
    pos++;
  }

  return pos;
}

int haplo_lexer_next_token(char* input, int input_size,
                           int *token_len, HaploAtom_t *atom)
{
  if (input == NULL) return -HAPLO_ERROR_LEXER_INPUT_NULL;
  if (input_size == 0) return -HAPLO_ERROR_LEXER_END_OF_INPUT;
  
  switch(input[0])
  {
  case '(':
    if (token_len != NULL) *token_len = 1;
    return OPEN;
  case ')':
    if (token_len != NULL) *token_len = 1;
    return CLOSE;
  case '#':
    if (token_len != NULL) *token_len = 1;
    return COMMENT;
  case '"': ; // Atom of type STRING
    int ret = haplo_lexer_atom_len(input, input_size);
    if (ret < 0) return ret;
    if (ret < 2) return -HAPLO_ERROR_LEXER_ATOM_STRING_SIZE;
    if (token_len != NULL) *token_len = ret;
    if (atom != NULL)
    {
      atom->type = STRING;
      atom->string = (char *) malloc(ret - 1);
      strncpy(atom->string, input + 1, ret - 2); // Ignore the '"'
      atom->string[ret-1] = '\0';
    }
    return ATOM;
  default: ; // Atom may be INTEGER, BOOL or SYMBOL
    ret = haplo_lexer_atom_len(input, input_size);
    if (ret < 0) return ret;
    if (token_len != NULL) *token_len = ret;
    if (atom != NULL)
    {
      // Check for BOOL
      if (strncmp(input, "true", ret) == 0)
      {
        atom->type = BOOL;
        atom->boolean = true;
        return ATOM;
      }
      else if (strncmp(input, "false", ret) == 0)
      {
        atom->type = BOOL;
        atom->boolean = false;
        return ATOM;
      }
      // Check for INTEGER
      else {
        char *endptr = NULL;
        long integer = strtol(input, &endptr, 10);
        if (endptr == input + ret)
        {
          atom->type = INTEGER;
          atom->integer = integer;
          return ATOM;
        }
      }
      
      // Otherwise Atom is SYMBOL
      atom->type = SYMBOL;
      atom->symbol = (char *) malloc(ret + 1);
      strncpy(atom->symbol, input, ret);
      atom->symbol[ret] = '\0';
    }
    return ATOM;
  }

  return -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
}
