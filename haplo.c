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

// The Haplolang cli interpreter

#define HAPLO_NO_PREFIX
#include "haplo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void process_line(char* input, ssize_t len)
{
  int err;
  Parser parser = {0};
  err = parser_init(&parser, input, len);
  if (err < 0)
  {
    fprintf(stderr, "Error %d after parser_init\n", err);
    return;
  }

  Expr *expr = parser_parse(&parser);
  if (expr == NULL)
  {
    if (parser.error != 0)
    {
      fprintf(stderr, "Error parser_parse returned a null expression\n");
    }
    return;
  }

  Interpreter interpreter = {0};
  interpreter_init(&interpreter);
  Value val = interpreter_interpret(&interpreter, expr);
  
  char buf[1024] = {0};
  
  haplo_value_string(val, buf, 1024);
  printf("%s\n", buf);

  haplo_value_free(val);
  interpreter_destroy(&interpreter);
  expr_free(expr);
  return;
}

void print_headline()
{
  printf("The Haplolang interpreter by Giovanni Santini\n");
  return;
}

void print_help()
{
  print_headline();
  printf("Usage:\n");
  printf("    help    show help message\n");
  printf("    <file>  interpret <file>\n");
  return;
}

// Much faster than read(2) or fread since it does not copy the
// contents in a new buffer. Not portable.
char* mmap_file(const char* filename, size_t* out_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return NULL;
    }

    size_t size = st.st_size;
    if (size == 0) {
        // empty file
        close(fd);
        if (out_size) *out_size = 0;
        return NULL;
    }

    void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    if (out_size) {
        *out_size = size;
    }
    return (char*)addr;
}

void unmap_file(char* addr, size_t size) {
    if (addr) {
        munmap(addr, size);
    }
}

void interpret_file(char* file)
{
  size_t size;
  char* data = mmap_file(file, &size);
  if (data == NULL)
  {
    fprintf(stderr, "Error opening file %s\n", file);
    return;
  }
  
  process_line(data, size);
  
  unmap_file(data, size);
  return;
}

void interpret_cmdline()
{
  print_headline();

  while(1) {
    char *line = readline("> ");
    add_history(line);
    process_line(line, strlen(line));
    free(line);
  }
}

int main(int argc, char** argv)
{
  if (argc > 1)
  {
    if (strcmp(argv[1], "help") == 0)
    {
      print_help();
      return 0;
    }

    interpret_file(argv[1]);
    return 0;
  }

  interpret_cmdline();
  
  return 0;
}
