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

// The Haplolang cli interpreter

#define HAPLO_NO_PREFIX
#include "haplo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void process_line(char* input, ssize_t len)
{
  int err;
  Parser_t parser = {0};
  err = parser_init(&parser, input, len);
  if (err < 0)
  {
    printf("Error %d after parser_init\n", err);
    return;
  }

  Expr_t *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    printf("Error parser_parse returned a null expression\n");
    return;
  }

  Interpreter_t interpreter = {0};
  interpreter_init(&interpreter);
  err = interpreter_interpret(&interpreter, expr);
  if (err < 0)
  {
    printf("Error %d in interpreter_interpret\n", err);
    expr_free(expr);
    return;
  }

  haplo_interpreter_clean(&interpreter);
  expr_free(expr);
  return;
}

int main(void)
{
  printf("The Haplolang interpreter by Giovanni Santini\n");

  while(1) {
    char *line = readline("> ");
    add_history(line);
    process_line(line, strlen(line));
    free(line);
  }
  
  return 0;
}
