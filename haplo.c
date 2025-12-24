// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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

void process_line(Interpreter *interpreter, char* input, ssize_t len)
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
  if (!expr)
  {
    if (parser.error != 0)
    {
      fprintf(stderr, "Error parser_parse returned a null expression\n");
    }
    return;
  }

  Value val = interpreter_interpret(interpreter, expr);
  
  char buf[1024] = {0};
  
  haplo_value_string(val, buf, 1024);
  printf("%s\n", buf);

  haplo_value_free(val);
  expr_free(expr);
  return;
}

void print_headline(void)
{
  printf("The Haplolang interpreter by Giovanni Santini\n");
  return;
}

void print_help(void)
{
  print_headline();
  printf("Usage:  haplo [options] [file]\n");
  printf("\n");
  printf("options:\n");
  printf("      help    show help message\n");
  printf("      -i      start REPL interpreter after evaluating file\n");
  return;
}

// Much faster than read(2) or fread since it does not copy the
// contents in a new buffer. Not portable.
char* mmap_file(const char* filename, size_t* out_size)
{
  int fd = open(filename, O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return NULL;
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    perror("fstat");
    close(fd);
    return NULL;
  }

  size_t size = st.st_size;
  if (size == 0)
  {
    // empty file
    close(fd);
    if (out_size) *out_size = 0;
    return NULL;
  }

  void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (addr == MAP_FAILED)
  {
    perror("mmap");
    return NULL;
  }

  if (out_size)
  {
    *out_size = size;
  }
  return (char*)addr;
}

void unmap_file(char* addr, size_t size)
{
  if (addr)
    munmap(addr, size);
}

void interpret_file(Interpreter *interpreter, char* file)
{
  size_t size;
  char* data = mmap_file(file, &size);
  if (!data)
  {
    fprintf(stderr, "Error opening file %s\n", file);
    return;
  }
  
  process_line(interpreter, data, size);
  
  unmap_file(data, size);
  return;
}

void interpret_cmdline(Interpreter *interpreter)
{
  print_headline();

  while(1)
  {
    char *line = readline("> ");
    if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0)
    {
      free(line);
      return;
    }

    add_history(line);
    process_line(interpreter, line, strlen(line));
    free(line);
  }
}

int main(int argc, char** argv)
{ 
  Interpreter interpreter = {0};
  interpreter_init(&interpreter);

  bool interactive = false;
  if (argc > 1)
  {
    if (strcmp(argv[1], "help") == 0)
    {
      print_help();
      return 0;
    }

    if (strcmp(argv[1], "-i") == 0)
    {
      interactive = true;
    }

    interpret_file(&interpreter, argv[argc-1]);
    if (interactive)
    {
      interpret_cmdline(&interpreter);
    }
    
    interpreter_destroy(&interpreter);
    return 0;
  }

  interpret_cmdline(&interpreter);
  
  interpreter_destroy(&interpreter);
  return 0;
}
