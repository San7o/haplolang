// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "parser.h"
#include "errors.h"

#include <stdlib.h>
#include <assert.h>

int haplo_parser_init(HaploParser *parser, char *input, unsigned int len)
{
  if (!parser) return HAPLO_ERROR_PARSER_NULL;
  if (!input) return HAPLO_ERROR_PARSER_INPUT_NULL;

  parser->error = 0;
  
  haplo_lexer_init(&parser->lexer, input, len, &haplo_default_token_char);
  
  return 0;
}

int haplo_parser_dump(HaploParser *parser)
{
  if (!parser) return HAPLO_ERROR_PARSER_NULL;
  
  fprintf(stderr, "Parser dump: error: %s, cursor: %d, line: %d, column: %d\n",
          haplo_error_string(parser->error), parser->lexer.cursor,
          parser->lexer.line, parser->lexer.column);
  
  return 0;
}

bool haplo_parser_check_error(HaploParser *parser)
{
  if (!parser) return false;
  return (parser->error < 0);
}

_Static_assert(_HAPLO_LEX_MAX == 7,
              "Updated HaploToken, maybe update haplo_parser_parse_rec");
HaploExpr *haplo_parser_parse_rec(HaploParser *parser)
{
  if (!parser) return NULL;  
  parser->error = 0;

  int ret;
  HaploToken token = HAPLO_LEX_NONE;
  HaploAtom atom = {0};
  HaploExpr *expr = calloc(1, sizeof(HaploExpr));
  assert(expr);
  
  // Head expression

  ret = haplo_lexer_peek(&parser->lexer, &token, &atom);
  if (ret < 0)
  {
    parser->error = ret;
    haplo_expr_free(expr);
    if (parser->error == HAPLO_ERROR_LEXER_END_OF_INPUT)
      return NULL;
    
    HAPLO_PARSER_ERROR();
  }
  
  switch (token)
  {
  case HAPLO_LEX_QUOTE:
    // Quote expects to be followed by an atom of type symbol
    // eg: 'test
    haplo_lexer_next(&parser->lexer, NULL, NULL);
    ret = haplo_lexer_next(&parser->lexer, &token, &atom);
    if (ret < 0)
    {
      parser->error = ret;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    
    if (token != HAPLO_LEX_ATOM)
    {
      parser->error = HAPLO_ERROR_PARSER_UNEXPECTED_TOKEN;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    if (atom.type != HAPLO_ATOM_SYMBOL)
    {
      haplo_atom_free(&atom);
      parser->error = HAPLO_ERROR_PARSER_UNEXPECTED_TOKEN;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }

    atom.type = HAPLO_ATOM_QUOTE;
    expr->head = malloc(sizeof(HaploExpr));
    *expr->head = (HaploExpr){
      .is_atom = true,
      .atom = atom,
    };
    break;
  case HAPLO_LEX_ATOM:
    haplo_atom_free(&atom);
    ret = haplo_lexer_next(&parser->lexer, &token, &atom);
    if (ret < 0)
    {
      parser->error = ret;
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    expr->head = malloc(sizeof(HaploExpr));
    *expr->head = (HaploExpr){
      .is_atom = true,
      .atom = atom,
    };
    break;
  case HAPLO_LEX_OPEN:

    haplo_lexer_next(&parser->lexer, NULL, NULL);
    
    expr->head = haplo_parser_parse_rec(parser);

    ret = haplo_lexer_peek(&parser->lexer, &token, &atom);
    if (ret < 0)
    {
      parser->error = ret;
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
    if (token != HAPLO_LEX_CLOSE)
    {
      haplo_expr_free(expr);
      if (token == HAPLO_LEX_ATOM)
        haplo_atom_free(&atom);
      parser->error = HAPLO_ERROR_MALFORMED_PARENTHESIS;
      HAPLO_PARSER_ERROR();
    }
    ret = haplo_lexer_next(&parser->lexer, &token, &atom);
    if (ret < 0)
    {
      parser->error = ret;
      haplo_expr_free(expr);    
      HAPLO_PARSER_ERROR();
    }
    break;
  case HAPLO_LEX_CLOSE:
    haplo_expr_free(expr);
    return NULL;
  case HAPLO_LEX_EOF:
    haplo_expr_free(expr);
    return NULL;
  default:
    haplo_expr_free(expr);
    parser->error = HAPLO_ERROR_PARSER_TOKEN_UNRECOGNIZED;
    HAPLO_PARSER_ERROR();
  }

  // Optional tail expression
  expr->tail = haplo_parser_parse_rec(parser);
  return expr;
}

HaploExpr *haplo_parser_parse(HaploParser *parser)
{
  if (!parser) return NULL;
  if (!parser->lexer.input) return NULL;
  if (parser->lexer.input_size == 0) return NULL;
  parser->error = 0;

  if (setjmp(parser->jump_buf)) {
    // The parser jumps here when it encounters an error
    haplo_parser_dump(parser);
    return NULL;
  }

  int ret;
  HaploToken token = HAPLO_LEX_NONE;
  HaploAtom atom = {0};
  HaploExpr *expr;

  ret = haplo_lexer_peek(&parser->lexer, &token, &atom);
  if (ret < 0)
  {
    parser->error = ret;
    if (token == HAPLO_LEX_ATOM)
      haplo_atom_free(&atom);
    HAPLO_PARSER_ERROR();
  }

  if (token == HAPLO_LEX_COMMENT)
    return NULL;
  
  if (token == HAPLO_LEX_ATOM)
    haplo_atom_free(&atom);
  
  if (token == HAPLO_LEX_OPEN)
  {
    haplo_lexer_next(&parser->lexer, NULL, NULL);
    
    expr = haplo_parser_parse_rec(parser);

    ret = haplo_lexer_peek(&parser->lexer, &token, &atom);
    if (ret < 0)
    {
      haplo_expr_free(expr);
      parser->error = ret;
      HAPLO_PARSER_ERROR();
    }
    if (token != HAPLO_LEX_CLOSE)
    {
      haplo_expr_free(expr);
      if (token == HAPLO_LEX_ATOM)
        haplo_atom_free(&atom);
      parser->error = HAPLO_ERROR_MALFORMED_PARENTHESIS;
      HAPLO_PARSER_ERROR();
    }
    ret = haplo_lexer_next(&parser->lexer, NULL, NULL);
    if (ret < 0)
    {
      haplo_expr_free(expr);
      HAPLO_PARSER_ERROR();
    }
    return expr;
  }

  expr = haplo_parser_parse_rec(parser);
  return expr;
}
