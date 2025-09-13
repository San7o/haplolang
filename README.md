# Haplolang

Haplolang an s-expression based, imperative, interpreted programming
language implemented in C, greatly inspired by Emacs Lisp. The term
"Haplo" is a Greek word meaning simple, which describes my philosophy
in implementing the language.

This is a [recreational programming](https://giovanni-diary.netlify.app/programming/notes/recreational-programming) project.

The project is composed of:

- `libhaplo`: the library containing all the logic
- `haplostdlib.a`: the haplo standard library in C
- `haplo`: cli-based REPL interpreter
- `haplo_tests`: unit tests
- `haplo_tests_e2e.sh`: cli tool tests

You can use the interactive interpreter by running the `haplo` cli:

```lisp
$ ./haplo
The Haplolang interpreter by Giovanni Santini
> # Hello, this is a comment
> (print "Hello, World!")
"Hello, World!"
empty
> (+ 69 420)
489
```

You can also interpret a file:

```lisp
$ ./haplo samples/hello_world.haplo 
"Hello, World!"
empty
```

## Building

To build the library, just run `make`:

```
$ time make
gcc -Wall -Werror -Wpedantic -ggdb -c atom.c -o atom.o
gcc -Wall -Werror -Wpedantic -ggdb -c parser.c -o parser.o
gcc -Wall -Werror -Wpedantic -ggdb -c lexer.c -o lexer.o
gcc -Wall -Werror -Wpedantic -ggdb -c expr.c -o expr.o
gcc -Wall -Werror -Wpedantic -ggdb -c interpreter.c -o interpreter.o
gcc -Wall -Werror -Wpedantic -ggdb -c value.c -o value.o
gcc -Wall -Werror -Wpedantic -ggdb -c errors.c -o errors.o
ar rcs libhaplo.a atom.o parser.o lexer.o expr.o interpreter.o value.o errors.o

real    0m0.171s
user    0m0.125s
sys     0m0.044s
```

To build the cli interpreter, you need to have
[readline](https://savannah.gnu.org/git/?group=readline) installed
first, then run:

```
make cli
```

Build and run the tests and e2e tests:

```
make check
```

Clean object files:

```
make clean
```

## Quickstart

The language is strictly typed. The default value types are `integer`,
`float`, `string`, `bool`, `list`, `quote`, `empty` and `error`. If
you evaluate a value, you will get it back:

```lisp
> 123            # An integer
123
> 567.890        # A float
567.890000
> "Hello World"  # A string
"Hello World"
> true           # A bool
true
> list 1 2 3     # A list
list 1 2 3
> 'hello         # A quote
'hello
> hello          # A function
Error: ERROR_INTERPRETER_UNKNOWN_FUNCTION
> # The previous result was an error
```

Symbols are recognized as functions: they can be called with some
arguments and return a value. For example, the function `+` takes two
integers and returns an integer value representing the sum of the two
arguments, while the function `print` takes a value, prints it, and
returns an `empty` value.

```lisp
> (+ 68.1 0.9)
69.000000
> (print "Nice")
"Nice"
empty
```

A quote is a symbol prepended with `'`. Unlike functions, this symbol
will not be called, so symbols can be passed as arguments to other
functions. For example, you can define a variable with `setq` by
specifying the symbol that denotes the variable using a quote:

```lisp
> (setq 'hello "Hello, World!")
"Hello, World!"
> hello
"Hello, World!"
```

You can define functions with `defunc 'NAME (FUNCTION_BODY)`:

```lisp
> (defunc 'test (+ 2 3))
empty
> test
5
```

The real difference between a function and a variable is that
variables can only hold values, while functions have their own AST
which gets "jumped" to.

You can nest function calls with s-expressions, like lisp.

```lisp
$ cat samples/quick_math.haplo 
(print
 (*
  (+ 1 2)
  (- 7 5)))
$ ./haplo samples/quick_math.haplo 
6
empty
```

Parentheses may be omitted, in which case the expression will be
considered right associative. For example, these are all equal
expressions:

```lisp
> + 4 * 2 3
10
> ( + 4 ( * 2 3 ) ) )
10
> ( + ( 4 ) ( * ( 2 ) ( 3 ) ) ) ) )
10
```

You can do `if` statements with the structure `if (CONDITION) (CASE
TRUE) (CASE FALSE)`.

```lisp
$ cat samples/if_condition_true.haplo
(if (> 100 5)
    (print "Correct!")
    (print "Wrong :("))
$ ./haplo samples/if_condition_true.haplo
"Correct!"
empty
```

The only data structure currently present in the standard library is
the `list`, and you can do the usual list operations:

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

The grammars is as follows:

```ebnf
EXPR    ::= EXPR EXPR
          | ( EXPR EXPR )
          | ATOM
          |
ATOM    ::= STRING
          | INTEGER
          | FLOAT
          | BOOL
          | SYMBOL
          | QUOTE
STRING  ::= " [CHAR* | TERMINATOR] "
INTEGER ::= [+-]NUMBER*
FLOAT   ::= [+-]NUMBER*.NUMBER*
NUMBER  ::= [0-9]
BOOL    ::= true | false
SYMBOL  ::= CHAR*
QUOTE   ::= 'SYMBOL
CHAR    ::= [a-zA-Z0-9!@#$%^&*()_+{}|:"<>?[]\;,./_+]
TERMINATOR ::= [ |\n|\t]
```

The number of new lines, spaces and tabs is ignored. Comments are
currently not fully supported and should not be used to avoid
troubles.
