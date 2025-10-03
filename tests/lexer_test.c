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

#define HAPLO_NO_PREFIX
#include "../haplo.h"
#include "tests.h"

#include <stdio.h>
#include <string.h>

HAPLO_TEST(lexer_test, trim)
{
  {
    char *input = "  ( 123";
    int expected_val = 2;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d",
              input, expected_val, trimmed);
      goto test_failed;
    }
  }

  {
    char *input = "    123";
    int expected_val = 4;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d",
              input, expected_val, trimmed);
      goto test_failed;
    }
  }

  {
    char *input = "  \n  \t  \n\n 123";
    int expected_val =11;
    int trimmed = lexer_trim_left(input, strlen(input), NULL, NULL);
    if (trimmed != expected_val)
    {
      fprintf(stderr, "Error lexer_trim_left on \"%s\", expected %d, got %d",
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
    int token_len = 0;
    
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_OPEN;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d",
              input, expected_token, token);
      goto test_failed;
    }
    if (token_len != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d",
              input, expected_token_len, token_len);
      goto test_failed;
    }
  }

  {
    char *input = ") 123";
    int token_len = 0;
    
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_CLOSE;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d",
              input, expected_token, token);
      goto test_failed;
    }
    if (token_len != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d",
              input, expected_token_len, token_len);
      goto test_failed;
    }
  }

  {
    char *input = "# 123";
    int token_len = 0;
    
    int expected_token_len = 1;
    Token expected_token = HAPLO_LEX_COMMENT;
    int token = lexer_next_token(input, strlen(input), &token_len, NULL, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d",
              input, expected_token, token);
      goto test_failed;
    }
    if (token_len != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d",
              input, expected_token_len, token_len);
      goto test_failed;
    }
  }

  {
    char *input = "123";
    int token_len = 0;
    
    int expected_token_len = 3;
    Token expected_token = HAPLO_LEX_ATOM;
    long int expected_atom = 123;
    Atom atom;
    int token = lexer_next_token(input, strlen(input), &token_len, &atom, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d",
              input, expected_token, token);
      goto test_failed;
    }
    if (token_len != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d",
              input, expected_token_len, token_len);
      goto test_failed;
    }
    if (atom.type != HAPLO_ATOM_INTEGER || atom.integer != expected_atom)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected atom value %ld, got %ld",
              input, expected_atom, atom.integer);
      goto test_failed;
    }
  }

  {
    char *input = "69.420";
    int token_len = 0;
    
    int expected_token_len = 6;
    Token expected_token = HAPLO_LEX_ATOM;
    double expected_atom = 69.420;
    Atom atom;
    int token = lexer_next_token(input, strlen(input), &token_len, &atom, NULL);
    if (token != expected_token)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected %d, got %d",
              input, expected_token, token);
      goto test_failed;
    }
    if (token_len != expected_token_len)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected token len %d, got %d",
              input, expected_token_len, token_len);
      goto test_failed;
    }
    if (atom.type != HAPLO_ATOM_FLOAT || atom.floating_point != expected_atom)
    {
      fprintf(stderr, "Error lexer_next_token on \"%s\", expected atom value %f, got %f",
              input, expected_atom, atom.floating_point);
      goto test_failed;
    }
  }

  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}

