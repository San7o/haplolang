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

#include "parser.h"
#include "errors.h"

#include <stdlib.h>

int haplo_parser_init(HaploParser_t *parser, char *input, size_t len)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  if (input == NULL) return -HAPLO_ERROR_PARSER_INPUT_NULL;
  
  *parser = (HaploParser_t){
    .input = input,
    .input_len = len,
    .last_token = NONE,
    .last_atom = '\0',
    .pos = 0,
    .line = 0,
    .column = 0,
    .error = 0,
  };
  
  return 0;
}

void haplo_parser_trim_left(HaploParser_t *parser)
{
  if (parser == NULL) return;
  
  while(parser->pos < parser->input_len &&
        (parser->input[parser->pos] == ' ' ||
         parser->input[parser->pos] == '\n' ||
         parser->input[parser->pos] == '\t'))
  {
    if (parser->input[parser->pos] == '\n') {
      parser->line++;
      parser->column = 0;
    }
    parser->pos++;
  }

  return;
}

int haplo_parser_next_token(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  
  if (parser->input == NULL)
  {
    parser->error = -HAPLO_ERROR_PARSER_INPUT_EMPTY;
    return -1;
  }

  haplo_parser_trim_left(parser);
  
  if (parser->pos >= parser->input_len)
  {
    parser->error = -HAPLO_ERROR_PARSER_END_OF_INPUT;
    return -1;
  }

  char input = parser->input[parser->pos];
  parser->column++;
  parser->pos++;
  
  switch(input)
  {
  case '(':
    parser->last_token = OPEN;
    return 0;
  case ')':
    parser->last_token = CLOSE;
    return 0;
  case '#':
    parser->last_token = COMMENT;
    return 0;
  default:
    // TODO: get slice
    parser->last_token = ATOM;
    parser->last_atom = input;
    return 0;
  }

  parser->last_token = NONE;
  parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
  return -1;
}


int haplo_parser_peek_next_token(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  
  if (parser->input == NULL)
  {
    parser->error = -HAPLO_ERROR_PARSER_INPUT_EMPTY;
    return -1;
  }

  haplo_parser_trim_left(parser);
  
  if (parser->pos >= parser->input_len)
  {
    parser->error = -HAPLO_ERROR_PARSER_END_OF_INPUT;
    return -1;
  }

  char input = parser->input[parser->pos];
  
  switch(input)
  {
  case '(':
    parser->last_token = OPEN;
    return 0;
  case ')':
    parser->last_token = CLOSE;
    return 0;
  default:
    // TODO: get slice
    parser->last_token = ATOM;
    parser->last_atom = input;
    return 0;
  }

  parser->last_token = NONE;
  parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
  return -1;
}

int haplo_parser_dump(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  
  fprintf(stderr, "Parser dump: error: %s, pos: %d, line: %d, column: %d, last_token: %c\n",
          error_string(parser->error), parser->pos, parser->line, parser->column, parser->last_atom);
  
  return 0;
}

bool haplo_parser_check_error(HaploParser_t *parser)
{
  if (parser == NULL) return false;
  return (parser->error < 0);
}


HaploExpr_t *haplo_parser_parse_rec(HaploParser_t *parser, bool is_paranthesized)
{
  if (parser == NULL) return NULL;  
  parser->error = 0;
  
  HaploExpr_t *expr;
  expr = malloc(sizeof(HaploExpr_t));

  // First expression
  
  if (haplo_parser_next_token(parser) < 0)
    HAPLO_PARSER_ERROR();

  switch (parser->last_token)
  {
  case ATOM:
    expr->first = malloc(sizeof(HaploExpr_t));
    *expr->first = (HaploExpr_t){
      .is_atom = true,
      .atom = parser->last_atom,
    };
    break;
  case OPEN:
    expr->first = haplo_parser_parse_rec(parser, true);
    break;
  case CLOSE:
    free(expr);
    return NULL;
  default:
    parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
    HAPLO_PARSER_ERROR();
  }

  // Optional second expression
  
  if (haplo_parser_peek_next_token(parser) < 0)
    HAPLO_PARSER_ERROR();

  switch (parser->last_token)
  {
  case ATOM:
    expr->second = haplo_parser_parse_rec(parser, false);
    break;
    
  case OPEN:
    if (haplo_parser_next_token(parser) < 0)
      HAPLO_PARSER_ERROR();
    expr->second = haplo_parser_parse_rec(parser, true);
    break;
    
  case CLOSE:
    if (!is_paranthesized)
    {
      expr->second = NULL;
      return expr;
    }    
    if (haplo_parser_next_token(parser) < 0)
      HAPLO_PARSER_ERROR();
    expr->second = NULL;
    return expr;
    
  default:
    parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
    HAPLO_PARSER_ERROR();
  }

  // Paranthesized expressions must end with a closing parenthesis
  if (haplo_parser_peek_next_token(parser) < 0)
    HAPLO_PARSER_ERROR();
  if (is_paranthesized)
  {
    if (parser->last_token != CLOSE)
    {
      parser->error = -HAPLO_ERROR_MALFORMED_PARENTHESIS;
      HAPLO_PARSER_ERROR();
    }
    if (haplo_parser_next_token(parser) < 0)
      HAPLO_PARSER_ERROR();
  }
  
  return expr;
}

HaploExpr_t *haplo_parser_parse(HaploParser_t *parser)
{
  if (parser == NULL) return NULL;
  
  parser->error = 0;
  
  HaploExpr_t *expr;
  expr = malloc(sizeof(HaploExpr_t));

  if (setjmp(parser->jump_buf)) {
    // The parser jumps here when encountering an error
    haplo_parser_dump(parser);
    haplo_expr_free(expr);
    return NULL;
  }

  if (haplo_parser_next_token(parser) < 0)
    HAPLO_PARSER_ERROR();

  if (parser->last_token == COMMENT)
  {
    haplo_expr_free(expr);
    return NULL;
  }
  
  if (parser->last_token != OPEN)
  {
    parser->error = -HAPLO_ERROR_PARSER_MISSING_OPEN;
    HAPLO_PARSER_ERROR();
  }
  
  expr = haplo_parser_parse_rec(parser, true);
  
  return expr;
}
