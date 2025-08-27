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
    .last_token = HAPLO_LEX_NONE,
    .last_atom = (HaploAtom_t) {0},
    .pos = 0,
    .line = 0,
    .column = 0,
    .error = 0,
  };
  
  return 0;
}

int haplo_parser_next_token(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  parser->last_token = HAPLO_LEX_NONE;
  parser->last_atom = (HaploAtom_t) {0};
  
  parser->pos += haplo_lexer_trim_left(parser->input + parser->pos,
                                       parser->input_len - parser->pos,
                                       &parser->line,
                                       &parser->column);
  int token_len = 0;
  int ret = haplo_lexer_next_token(parser->input + parser->pos,
                                   parser->input_len - parser->pos,
                                   &token_len,
                                   &parser->last_atom);
  if (ret < 0)
  {
    parser->error = ret;
    return -1;
  }

  parser->last_token = ret;
  parser->pos += token_len;
  parser->column += token_len;
  return 0;
}

int haplo_parser_peek_next_token(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  parser->last_token = HAPLO_LEX_NONE;
  parser->last_atom = (HaploAtom_t) {0};
  
  parser->pos += haplo_lexer_trim_left(parser->input + parser->pos,
                                        parser->input_len - parser->pos,
                                        &parser->line,
                                        &parser->column);
  
  int ret = haplo_lexer_next_token(parser->input + parser->pos,
                                   parser->input_len - parser->pos,
                                   NULL,
                                   &parser->last_atom);
  if (ret < 0)
  {
    parser->error = ret;
    return -1;
  }
  
  parser->last_token = ret;
  return 0;
}

int haplo_parser_dump(HaploParser_t *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  
  fprintf(stderr, "Parser dump: error: %s, pos: %d, line: %d, column: %d, last_token: %s\n",
          haplo_error_string(parser->error), parser->pos, parser->line, parser->column, haplo_lexer_token_string(parser->last_token));
  
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
  
  HaploExpr_t *expr = malloc(sizeof(HaploExpr_t));
  *expr = (HaploExpr_t){0};
  
  // First expression
  
  if (haplo_parser_next_token(parser) < 0)
  {
    haplo_expr_free(expr);
    HAPLO_PARSER_ERROR();
  }

  switch (parser->last_token)
  {
  case HAPLO_LEX_ATOM:
    expr->head = malloc(sizeof(HaploExpr_t));
    *expr->head = (HaploExpr_t){
      .is_atom = true,
      .atom = parser->last_atom,
    };
    break;
  case HAPLO_LEX_OPEN:
    expr->head = haplo_parser_parse_rec(parser, true);
    break;
  case HAPLO_LEX_CLOSE:
    haplo_expr_free(expr);
    return NULL;
  default:
    haplo_expr_free(expr);
    parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
    HAPLO_PARSER_ERROR();
  }

  // Optional tail expression

  if (haplo_parser_peek_next_token(parser) < 0)
  {
    if (parser->last_token == HAPLO_LEX_ATOM)
      haplo_atom_free(parser->last_atom);

    if (parser->error != -HAPLO_ERROR_LEXER_END_OF_INPUT)
    {
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
  } else {
    
    switch (parser->last_token)
    {
    case HAPLO_LEX_ATOM:
      haplo_atom_free(parser->last_atom);
      expr->tail = haplo_parser_parse_rec(parser, false);
      break;
    
    case HAPLO_LEX_OPEN:
      if (haplo_parser_next_token(parser) < 0)
      {
        haplo_expr_free(expr);
        HAPLO_PARSER_ERROR();
      }
      expr->tail = haplo_parser_parse_rec(parser, true);
      break;
    
    case HAPLO_LEX_CLOSE:
      if (is_paranthesized)
      {
        if (haplo_parser_next_token(parser) < 0)
        {
          haplo_expr_free(expr);
          HAPLO_PARSER_ERROR();
        }
      }    
      expr->tail = NULL;
      return expr;
    case HAPLO_LEX_COMMENT:
      break;
    
    default:
      haplo_expr_free(expr);
      parser->error = -HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
      HAPLO_PARSER_ERROR();
    }
  }
  
  if (!is_paranthesized) return expr;

  // Paranthesized expressions must end with a closing parenthesis
  if (haplo_parser_peek_next_token(parser) < 0)
  {
    haplo_expr_free(expr);
    HAPLO_PARSER_ERROR();
  }
  if (parser->last_token == HAPLO_LEX_ATOM)
    haplo_atom_free(parser->last_atom);
  
  if (parser->last_token != HAPLO_LEX_CLOSE)
  {
    haplo_expr_free(expr);
    parser->error = -HAPLO_ERROR_MALFORMED_PARENTHESIS;
    HAPLO_PARSER_ERROR();
  }
    
  if (haplo_parser_next_token(parser) < 0)
  {
    haplo_expr_free(expr);
    HAPLO_PARSER_ERROR();
  }
  
  return expr;
}

HaploExpr_t *haplo_parser_parse(HaploParser_t *parser)
{
  if (parser == NULL) return NULL;
  parser->error = 0;

  if (setjmp(parser->jump_buf)) {
    // The parser jumps here when encountering an error
    haplo_parser_dump(parser);
    return NULL;
  }

  if (haplo_parser_peek_next_token(parser) < 0)
  {
    if (parser->last_token == HAPLO_LEX_ATOM)
      haplo_atom_free(parser->last_atom);
    HAPLO_PARSER_ERROR();
  }
  
  if (parser->last_token == HAPLO_LEX_COMMENT)
    return NULL;
  
  if (parser->last_token == HAPLO_LEX_OPEN)
  {
    if (haplo_parser_next_token(parser) < 0)
      HAPLO_PARSER_ERROR();
    return haplo_parser_parse_rec(parser, true);
  }

  if (parser->last_token == HAPLO_LEX_ATOM)
    haplo_atom_free(parser->last_atom);

  return haplo_parser_parse_rec(parser, false);
}
