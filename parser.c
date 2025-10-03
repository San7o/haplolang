// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "parser.h"
#include "errors.h"

#include <stdlib.h>
#include <assert.h>

int haplo_parser_init(HaploParser *parser, char *input, size_t len)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  if (input == NULL) return -HAPLO_ERROR_PARSER_INPUT_NULL;
  
  *parser = (HaploParser){
    .input = input,
    .input_len = len,
    .last_token = HAPLO_LEX_NONE,
    .last_atom = (HaploAtom) {0},
    .pos = 0,
    .line = 0,
    .column = 0,
    .error = 0,
  };
  
  return 0;
}

int haplo_parser_next_token(HaploParser *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  parser->last_token = HAPLO_LEX_NONE;
  parser->last_atom = (HaploAtom) {0};
  
  parser->pos += haplo_lexer_trim_left(parser->input + parser->pos,
                                       parser->input_len - parser->pos,
                                       &parser->line,
                                       &parser->column);
  int token_len = 0;
  int ret = haplo_lexer_next_token(parser->input + parser->pos,
                                   parser->input_len - parser->pos,
                                   &token_len,
                                   &parser->last_atom,
                                   NULL);
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

int haplo_parser_peek_next_token(HaploParser *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  parser->error = 0;
  parser->last_token = HAPLO_LEX_NONE;
  parser->last_atom = (HaploAtom) {0};
  
  parser->pos += haplo_lexer_trim_left(parser->input + parser->pos,
                                        parser->input_len - parser->pos,
                                        &parser->line,
                                        &parser->column);
  
  int ret = haplo_lexer_next_token(parser->input + parser->pos,
                                   parser->input_len - parser->pos,
                                   NULL,
                                   &parser->last_atom,
                                   NULL);
  if (ret < 0)
  {
    parser->error = ret;
    return -1;
  }
  
  parser->last_token = ret;
  return 0;
}

int haplo_parser_dump(HaploParser *parser)
{
  if (parser == NULL) return -HAPLO_ERROR_PARSER_NULL;
  
  fprintf(stderr, "Parser dump: error: %s, pos: %d, line: %d, column: %d, last_token: %s\n",
          haplo_error_string(parser->error), parser->pos, parser->line, parser->column, haplo_lexer_token_string(parser->last_token));
  
  return 0;
}

bool haplo_parser_check_error(HaploParser *parser)
{
  if (parser == NULL) return false;
  return (parser->error < 0);
}

_Static_assert(_HAPLO_LEX_MAX == 6,
              "Updated HaploToken, maybe update haplo_parser_parse_rec");
HaploExpr *haplo_parser_parse_rec(HaploParser *parser)
{
  if (parser == NULL) return NULL;  
  parser->error = 0;

  HaploExpr *expr = calloc(sizeof(HaploExpr), 1);
  
  // Head expression
  
  if (haplo_parser_peek_next_token(parser) < 0)
  {
    haplo_expr_free(expr);
    if (parser->error == -HAPLO_ERROR_LEXER_END_OF_INPUT)
      return NULL;
    
    HAPLO_PARSER_ERROR();
  }
  
  switch (parser->last_token)
  {
  case HAPLO_LEX_QUOTE:
    // Quote expects to be followed by an atom of type symbol
    // eg: 'test
    if (haplo_parser_next_token(parser) < 0)
    {
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    if (haplo_parser_next_token(parser) < 0)
    {
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    if (parser->last_token != HAPLO_LEX_ATOM)
    {
      parser->error = -HAPLO_ERROR_PARSER_UNEXPECTED_TOKEN;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    if (parser->last_atom.type != HAPLO_ATOM_SYMBOL)
    {
      haplo_atom_free(parser->last_atom);
      parser->error = -HAPLO_ERROR_PARSER_UNEXPECTED_TOKEN;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    parser->last_atom.type = HAPLO_ATOM_QUOTE;
    expr->head = malloc(sizeof(HaploExpr));
    *expr->head = (HaploExpr){
      .is_atom = true,
      .atom = parser->last_atom,
    };
    break;
  case HAPLO_LEX_ATOM:
    haplo_atom_free(parser->last_atom);
    if (haplo_parser_next_token(parser) < 0)
    {
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    expr->head = malloc(sizeof(HaploExpr));
    *expr->head = (HaploExpr){
      .is_atom = true,
      .atom = parser->last_atom,
    };
    break;
  case HAPLO_LEX_OPEN:

    if (haplo_parser_next_token(parser) < 0)
    {
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
    
    expr->head = haplo_parser_parse_rec(parser);
    
    if (haplo_parser_peek_next_token(parser) < 0)
    {
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
    if (parser->last_token != HAPLO_LEX_CLOSE)
    {
      haplo_expr_free(expr);
      if (parser->last_token == HAPLO_LEX_ATOM)
        haplo_atom_free(parser->last_atom);
      parser->error = -HAPLO_ERROR_MALFORMED_PARENTHESIS;
      HAPLO_PARSER_ERROR();
    }
    if (haplo_parser_next_token(parser) < 0)
    {
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
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
  expr->tail = haplo_parser_parse_rec(parser);
  return expr;
}

HaploExpr *haplo_parser_parse(HaploParser *parser)
{
  if (parser == NULL) return NULL;
  if (parser->input == NULL) return NULL;
  if (parser->input_len == 0) return NULL;
  parser->error = 0;

  if (setjmp(parser->jump_buf)) {
    // The parser jumps here when encountering an error
    haplo_parser_dump(parser);
    return NULL;
  }

  HaploExpr *expr;  
  if (haplo_parser_peek_next_token(parser) < 0)
  {
    if (parser->last_token == HAPLO_LEX_ATOM)
      haplo_atom_free(parser->last_atom);
    HAPLO_PARSER_ERROR();
  }

  if (parser->last_token == HAPLO_LEX_COMMENT)
    return NULL;
  
  if (parser->last_token == HAPLO_LEX_ATOM)
    haplo_atom_free(parser->last_atom);
  
  if (parser->last_token == HAPLO_LEX_OPEN)
  {
    if (haplo_parser_next_token(parser) < 0)
    {
      HAPLO_PARSER_ERROR();
    }
    
    expr = haplo_parser_parse_rec(parser);
    
    if (haplo_parser_peek_next_token(parser) < 0)
    {
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
    if (parser->last_token != HAPLO_LEX_CLOSE)
    {
      haplo_expr_free(expr);
      if (parser->last_token == HAPLO_LEX_ATOM)
        haplo_atom_free(parser->last_atom);
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

  expr = haplo_parser_parse_rec(parser);
  return expr;
}
