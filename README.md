# Haplolang

I decided it is finally time for me to implement a proper programming
language in C like a real programmer, so here is `Haplolang`. "Haplo"
is a Greek word meaning simple, which describes my philosophy in
implementing the language. Haplolang an s-expression based imperative
language, greatly inspired by Lisp, and my main motivation is purely
recreational.

The current implementation was developed in two sittings: first the
parser and then the interpreter + cli program. Some functions are
implemented for basic math operations and printing, you cannot define
functions or variables yet.

Everything is implemented in C. The project is composed of:

- `libhaplo`: the library containing all the logic
- `haplo`: cli-based REPL interpreter
- `haplo_tests`: unit tests

You can use the interactive interpreter by running the `haplo` cli:

```
$ ./haplo
The Haplolang interpreter by Giovanni Santini
> # Hello, this is a comment
> print "Hello, World!"
"Hello, World!"
empty
> print ( + 69 420 )
489
empty
```

You can also interpret a file:

```
$ ./haplo samples/sample1.haplo 
"Hello, World!"
empty
```

## Building

Build the library, just run `make`:

```bash
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

Build and run the tests and e2e tests:

```
make check
```

Build the cli interpreter, you need to have
[readline](https://savannah.gnu.org/git/?group=readline) installed:

```
make cli
```

Clean object files:

```
make clean
```

## Quickstart

The language is strictly typed. The default value types are `integer`,
`float`, `string`, `bool`, `func`, `list`, `empty` and `error`. If you
evaluate a value that is not a function, you will get back the same
value:

```python
> 123            # An integer
123
> 567.890        # A float
567.890000
> "Hello World"  # A string
"Hello World"
> true           # A bool
true
> list 1 2 3     # A list
list 3 2 1
> hello          # A function
Error: ERROR_INTERPRETER_UNKNOWN_FUNCTION
> # The previous result was an error
```

Functions are particular names that can be called with some arguments
and will return a value. For example, the function `+` takes two
integers and returns an integer value representing the sum of the two
arguments, while the function `print` takes a value, then prints it
and returns an `empty` value.

```python
> + 68.1 0.9
69.000000
> print "Nice"
"Nice"
empy
```

You nest function calls using s-expressions, like lisp.

```lisp
$ cat samples/sample2.haplo 
( print
 ( *
  ( + 1 2 )
  ( - 7 5 ) ) )
$ ./haplo samples/sample2.haplo 
6
empty
```

Parenthesys may be omitted, in which case the expression will be
considered right associative. For example, these are all equal
expressions:

```lisp
> + 4 * 2 3
10
> ( + 4 ( * 2 3 ) ) )
10
> ( + ( 4 ) ( * ( 2 ) ( 3 ) ) ) ) )
10
> ( ( ( ( 10 ) ) ) )
10
```

The only data structure currenly present in the standard library is
the list, and you can do the usual list operations:

```lisp
> ( list 1 2 3 )
list: 3, 2, 1
> ( append 4 ( list 1 2 3 ) )
list: 4, 3, 2, 1
> ( head ( list 1 2 3 ) )
3
> ( tail ( list 1 2 3 ) )
list: 2, 1
```

Note that the elements are in reverse order from the order we
supplied. This happens because the parser is right associative and the
first element is the deepest expression. This may be changed in the
future.

The grammas is as follows:

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
STRING  ::= " * "
INTEGER ::= [+-][0-9]*
FLOAT   ::= [+-][0-9].[0-9]
BOOL    ::= true | false
SYMBOL  ::= [a-zA-Z0-9!@#$%^&*()_+{}|:"<>?[]\;',./_+]*
```

The number of new lines, spaces and tabs is ignored. Comments are
currently not fully supported and should not be used to avoid
troubles.
