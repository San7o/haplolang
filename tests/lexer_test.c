// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>
#include <string.h>

HAPLO_TEST(lexer_test, trim)
{
  {
    char *input = "  ( 123";
    Lexer l;
    haplo_lexer_init(&l, input, strlen(input), &haplo_default_token_char);
    
    int expected_val = 2;
    int trimmed = lexer_trim_left(&l);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d\n",
              input, expected_val, trimmed);
      goto test_failed;
    }
  }

  {
    char *input = "    123";
    Lexer l;
    haplo_lexer_init(&l, input, strlen(input), &haplo_default_token_char);
    
    int expected_val = 4;
    int trimmed = lexer_trim_left(&l);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d\n",
              input, expected_val, trimmed);
      goto test_failed;
    }
  }

  {
    char *input = "  \n  \t  \n\n 123";
    Lexer l;
    haplo_lexer_init(&l, input, strlen(input), &haplo_default_token_char);
    
    int expected_val =11;
    int trimmed = lexer_trim_left(&l);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d\n",
              input, expected_val, trimmed);
      goto test_failed;
    }
  }

  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

HAPLO_TEST(lexer_test, next)
{
  {
    char *input = "( 123";
    Lexer l;
    haplo_lexer_init(&l, input, strlen(input), &haplo_default_token_char);

    Token token;
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_OPEN;
    int ret = lexer_next(&l, &token, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d\n",
              input, expected_token, token);
      goto test_failed;
    }
    if (ret != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d\n",
              input, expected_token_len, ret);
      goto test_failed;
    }
  }

  {
    char *input = ") 123";
    Lexer l;
    lexer_init(&l, input, strlen(input), &haplo_default_token_char);

    Token token;
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_CLOSE;
    int ret = lexer_next(&l, &token, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d\n",
              input, expected_token, token);
      goto test_failed;
    }
    if (ret != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d\n",
              input, expected_token_len, ret);
      goto test_failed;
    }
  }

  {
    char *input = "# 123";
    Lexer l;
    lexer_init(&l, input, strlen(input), &haplo_default_token_char);

    Token token;
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_COMMENT;
    int ret = lexer_next(&l, &token, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d\n",
              input, expected_token, token);
      goto test_failed;
    }
    if (ret != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d\n",
              input, expected_token_len, ret);
      goto test_failed;
    }
  }

  {
    char *input = "123";
    Lexer l;
    lexer_init(&l, input, strlen(input), &haplo_default_token_char);
    
    int expected_token_len = 3;
    Token expected_token = HAPLO_LEX_ATOM;
    long int expected_atom = 123;
    Token token;
    Atom atom;
    int ret = lexer_next(&l, &token, &atom);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d\n",
              input, expected_token, token);
      goto test_failed;
    }
    if (ret != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d\n",
              input, expected_token_len, ret);
      goto test_failed;
    }
    if (atom.type != HAPLO_ATOM_INTEGER || atom.value.integer != expected_atom)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected atom value %ld, got %ld\n",
              input, expected_atom, atom.value.integer);
      goto test_failed;
    }
  }

  {
    char *input = "69.420";
    Lexer l;
    lexer_init(&l, input, strlen(input), &haplo_default_token_char);
    
    int expected_token_len = 6;
    Token expected_token = HAPLO_LEX_ATOM;
    double expected_atom = 69.420;
    Atom atom;
    Token token;
    int ret = lexer_next(&l, &token, &atom);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d\n",
              input, expected_token, token);
      goto test_failed;
    }
    if (ret != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d\n",
              input, expected_token_len, ret);
      goto test_failed;
    }
    if (atom.type != HAPLO_ATOM_FLOAT || atom.value.floating_point != expected_atom)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected atom value %f, got %f\n",
              input, expected_atom, atom.value.floating_point);
      goto test_failed;
    }
  }

  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

