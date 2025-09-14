# (`print` The Design and Implementation of Haplolang)

## (`print` A lisp-like programming language (to have fun))

(`print` Haplolang is an imperative, interpreted programming language inspired by
Emacs Lisp.)

```lisp
> (print "Hello, World!")
"Hello, World!"
empty
```

-----

### (`print` Here are some examples!)
(`print` Define variables)

```lisp
> (setq 'hello "Hello, World!")
"Hello, World!"
> hello
"Hello, World!"
```

(`print` Define functions)

```lisp
> (defunc 'test (+ 2 3))
empty
> test
5
```

(`print` `If` statements)

```lisp
$ cat samples/if_condition_true.haplo
(if (> 100 5)
    (print "Correct!")
    (print "Wrong :("))
$ ./haplo samples/if_condition_true.haplo
"Correct!"
empty
```

(`print` List operations.)

```lisp
> (list 1 2 3)
list: 1 2 3
> (append 4 (list 1 2 3 ))
list: 1 2 3 4
> (head (list 1 2 3 ))
3
> (tail (list 1 2 3))
list: 1 2
```

(`print` While loops.)

```lisp
$ cat samples/while_loop.haplo
(
 (setq 'a 0)
 (while (< (a) 20)
   (
    (print (a))
    (setq 'a
          (+ (a) 1)))
   )
 )
$ ./haplo samples/while_loop.haplo
0
1
2
3
...
```

-----

### (`print` Design)

(`print` The design and implementation of Haplolang is quite simple
and elegant. In this document I will give you an high-level
description of the inner workings of the language. My hope is that,
after reading this document, you will have a better idea on how to
implement a similar thing yourself, and to demystify the idea that
writing programming languages is really hard.)

(`print` More specifically, we will take a look at the following
sections.)

(`list` [lexer](#print-lexer) [parser](#print-parser) [interpreter](#print-interpreter) [standard-library](#print-standard-library) [cli-interpreter](#print-cli-interpreter) [testing](#print-testing))

-----

### (`print` Lexer)

(`print` The syntax of any language in the Lisp family is notoriously
minimalist, with only a few syntax characters namely `(`, `)`, `#`,
`'`, `"`. The lexer looks for these characters and returns a
corresponding enum value. If the lexer does not find any of them, then
we are lexing an `atom` which is either a `string`, `integer`,
`float`, `bool` or `symbol`. The lexer returns the length of the atom
and its type to the parser.)

(`code` lexer.h)
```c
enum HaploToken {
  HAPLO_LEX_OPEN = 0,  // '('
  HAPLO_LEX_CLOSE,     // ')'
  HAPLO_LEX_ATOM,      // see struct HaploAtom in atom.h
  HAPLO_LEX_COMMENT,   // '#'
  HAPLO_LEX_QUOTE,     // '''
  HAPLO_LEX_NONE,
  _HAPLO_LEX_MAX,
};

// On success, returns a non-negative integer representing an
// HaploToken.  On failure, returns a negative integer with the error
// value.  If token_len is specified, its value will be set to the
// length of the token. If atom is specified and the type of token is
// ATOM, a new Atom will be allocated and the user will be responsible
// to free it using haplo_atom_free. If token_char is non null, it
// will be used to map the HaploToken enum to a char, otherwise a
// default HaploTokenChar will be used instead
// (haplo_default_token_char).
int haplo_lexer_next_token(char* input, int input_size,
                           int* token_len, HaploAtom *atom,
                           HaploTokenChar token_char);
```

(`code` atom.h)

```c
enum HaploAtomType {
  HAPLO_ATOM_STRING = 0,    // "Hello World"
  HAPLO_ATOM_INTEGER,       // 69, -420
  HAPLO_ATOM_FLOAT,         // 69.420
  HAPLO_ATOM_BOOL,          // true, false
  HAPLO_ATOM_SYMBOL,        // print, +
  HAPLO_ATOM_QUOTE,         // 'test
  _HAPLO_ATOM_MAX,
};

typedef struct HaploAtom {
  enum HaploAtomType type;
  union {
    char* string;
    long int integer;
    double floating_point;
    bool boolean;
    char* symbol;
    char* quote;
  };
} HaploAtom;
```

-----

### (`print` Parser)

(`print` The parser is responsible to build the AST, which is
represented as a binary tree of expressions. An expression contains
either an atom or both an `head` and `tail` expressions.)

(`file` expr.h)

```c
typedef struct HaploExpr {
  bool is_atom;
  union {
    HaploAtom atom;
    struct {
      struct HaploExpr* head;
      struct HaploExpr* tail;
    };
  };
} HaploExpr;
```

(`print` When an atom is lexed, the head of the current expressions is
set with the atom, and the parser moves to the tail of the
expression. If an open parentheses is encountered, then the current
head is set as a new expression and the parser moves to this
expression. Additionally, the parser ensures that for every open
parentheses there is a closing one.)

(`file` parser.c) # Not super pretty code

```c
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
```

-----

### (`print` Interpreter)

(`print` The interpreter always evaluates the head with the tail set
as argument. When a non symbol is evaluated, it just returns a copy of
itself, otherwise the symbol is looked for in a symbol table which
contains either a haplo function (AST), a c function, or a variable
(which is a value).)

(`file` interpreter.c)

```c
HaploValue haplo_interpreter_interpret(HaploInterpreter *interpreter,
                                       HaploExpr *expr)
{
  if (interpreter == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_NULL,
    };
  }
  if (expr == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_EMPTY,
    };
  }

  if (expr->is_atom)
    return haplo_interpreter_eval_atom(expr->atom); // Returns a copy of itself

  HaploValue out_val;
  HaploValue func = haplo_interpreter_interpret(interpreter, expr->head);

  // Handle Special symbols
  // ...
  
  HaploValueList *args = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  out_val = haplo_interpreter_call(interpreter, func, args);

  haplo_value_free(func);
  haplo_value_list_free(args);
  return out_val;
}


HaploValueList *haplo_interpreter_interpret_tail(HaploInterpreter *interpreter,
                                                   HaploExpr *expr)
{
  if (interpreter == NULL || expr == NULL)
    return NULL;

  HaploValue head = haplo_interpreter_interpret(interpreter, expr->head);
  HaploValueList *tail = haplo_interpreter_interpret_tail(interpreter, expr->tail);

  if (head.type == HAPLO_VAL_SYMBOL)
  {
    head = haplo_interpreter_call(interpreter, head, tail);
    haplo_value_list_free(tail);
    return haplo_value_list_push_front(head, NULL);
  }

  return haplo_value_list_push_front(head, tail);
}

// Should not free args here
HaploValue haplo_interpreter_call(HaploInterpreter *interpreter,
                                  HaploValue value,
                                  HaploValueList* args)
{
  if (interpreter == NULL)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_NULL,
    };
  }

  if (value.type != HAPLO_VAL_SYMBOL)
  {
    return haplo_value_deep_copy(value);
  }

  HaploSymbol symbol;
  int err = haplo_symbol_map_lookup(interpreter->symbol_map,
                                    value.symbol,
                                    &symbol);
  if (err < 0)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL,
    };
  }

  switch(symbol.type)
  {
  case HAPLO_SYMBOL_C_FUNCTION:
    return symbol.c_func.run(interpreter, args);
  case HAPLO_SYMBOL_FUNCTION:
    return haplo_interpreter_interpret(interpreter, symbol.func);
  case HAPLO_SYMBOL_VARIABLE:
    return symbol.var;
  default:
    break;
  }
  return (HaploValue) {
    .type = HAPLO_VAL_ERROR,
    .error = -HAPLO_ERROR_INTERPRETER_UNKNOWN_SYMBOL_TYPE,
  };
}
```

(`print` The symbol table is an hashmap where the values are list of
symbols.)

(`file` symbol.h)

```c
typedef struct HaploSymbolMap {
  HaploSymbolList **_map;
  int capacity;
} HaploSymbolMap;

typedef struct HaploSymbolList {
  HaploSymbolKey key;
  struct HaploSymbolList *next;
  HaploSymbol val;
} HaploSymbolList;

typedef char* HaploSymbolKey;

enum HaploSymbolType {
  HAPLO_SYMBOL_C_FUNCTION = 0,   // stdlib
  HAPLO_SYMBOL_FUNCTION,
  HAPLO_SYMBOL_VARIABLE,
  _HAPLO_SYMBOL_MAX,
};

typedef struct HaploSymbol {
  enum HaploSymbolType type;
  union {
    HaploFunction c_func;    // function implemented in c
    HaploExpr* func;         // function defined as an AST
    HaploValue var;          // a variable
  };
} HaploSymbol;
```

(`print` `If`, `While`, `defunc` are special symbols for the
interpreter as they handle the arguments differently. For example, the
`if` statement evaluates one expression or the other depending on the
outcome of a condition. If we implemented `if` as a regular function
then all of its arguments would be evaluated first therefore executing
both outcomes. For this reason, we need to walk the AST differently
for certain functionalities.)

-----

### (`print` Standard Library)

(`print` By default, the symbol table in the interpreter is
initialized by default with some functions from the standard library,
the code of these functions lives under the `stdlib` directory. These
functions are implemented in C and automatically registered via a
macro. This macro uses a compiler directive to register a constructor
function, this may not be the best choice since it relies on a
compiler specific flag, but I thought it would not be an issue for
now.)

(`print` The standard library implements things like integer and
floating point operations and comparisons, logical operators, lists
and printing)

(`file` stdlib/stdlib.h)

```c
#define HAPLO_STD_FUNC(fn)    \
  HAPLO_STD_FUNC_STR(fn, #fn)

#define HAPLO_STD_FUNC_STR(fn, func_string)    \
    HaploValue __haplo_std_##fn(HaploInterpreter *, HaploValueList *); \
    __attribute__((constructor)) static void __haplo_std_register_##fn(void) \
    {                                                \
      if (__haplo_std_symbol_map._map == NULL) \
      { \
        haplo_symbol_map_init(&__haplo_std_symbol_map, \
                              HAPLO_INTERPRETER_SYMBOL_MAP_CAPACITY); \
      } \
      haplo_symbol_map_update(&__haplo_std_symbol_map,\
                              func_string,            \
                              (HaploSymbol){ \
                                .type = HAPLO_SYMBOL_C_FUNCTION,  \
                                .c_func = (HaploFunction) {\
                                  .run = __haplo_std_##fn \
                                }               \
                              });               \
    } \
    HaploValue __haplo_std_##fn(HaploInterpreter *interpreter, HaploValueList *args)
```

(`print` For example, the implementation of the `and` function is
shown below.)

(`file` stdlib/logic.c)

```c
// and BOOLEAN ...
// Returns: BOOLEAN | ERROR
HAPLO_STD_FUNC_STR(logical_and, "and")
{
  if (haplo_value_list_len(args) < 2)
  {
    return (HaploValue) {
      .type = HAPLO_VAL_ERROR,
      .error = -HAPLO_ERROR_INTERPRETER_WRONG_NUMBER_OF_ARGS,
    };
  }

  HaploValueList *this = args;
  bool result = true;
  while(this != NULL) {

    if (this->val.type != HAPLO_VAL_BOOL) {
      return (HaploValue) {
        .type = HAPLO_VAL_ERROR,
        .error = -HAPLO_ERROR_INTERPRETER_INVALID_TYPE,
      };
    }
    
    result &= this->val.boolean;
    this = this->next;
  }

  return (HaploValue) {
    .type = HAPLO_VAL_BOOL,
    .boolean = result,
  };
}
```

-----

### (`print` Cli interpreter)

(`print` The cli interpreter simply calls the interpreter either with input
from stdin or from a file. It also supports a few flags.)

(`file` haplo.c)

```c
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
  if (expr == NULL)
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
```

-----

### (`print` Testing)

(`print` This project comes with a decent amount of testing. Unit
tests live under `samples/` and can be run via `make tests`, while the
tests for the cli tool are under `samples/` and can be run via
`haplo_tests_e2e.sh`. In both cases, tests only need to be implemented
without the need to register them somewhere. I think it is
interesting to look at how the unit tests are registered because it is
pretty cool, it uses a neat trick with the linker.)

(`file` tests/tests/h)

```c
// Register a test case, It should end with HAPLO_TEST_SUCCESS or
// HAPLO_TEST_FAILED
// Thanks Sam P.
#define HAPLO_TEST(suiteName, uTtestName)                      \
    static int suiteName##_##uTtestName(void);                 \
    static HaploTest Record_##suiteName##_##uTtestName         \
    __attribute__((used, section(".utest_records"), aligned(sizeof(_Alignof(HaploTest))))) = { \
        .marker = 0xDeadBeaf,                                  \
        .testSuite = #suiteName,                               \
        .functionName = #uTtestName,                           \
        .testName = #suiteName "_" #uTtestName,	               \
        .fileName = __FILE__,                                  \
        .lineNumber = __LINE__,                                \
        .functionPointer = suiteName##_##uTtestName            \
    };                                                         \
    static int suiteName##_##uTtestName(void)

#define HAPLO_TEST_SUCCESS \
  do { return 0; } while(0)

#define HAPLO_TEST_FAILED \
  do { return -1; } while(0)
```

(`print` A test may look like the following.)

(`file` tests/lexer_test.c)

```c
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
  
  // ...
  
  HAPLO_TEST_SUCCESS;

 test_failed:
  HAPLO_TEST_FAILED;
}
```
