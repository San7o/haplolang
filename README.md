# Haplolang

I decided it is finally time for me to implement a proper programming
language in C like a real programmer, so here is `Haplolang`. "Haplo"
is a Greek word meaning simple, which describes my philosophy in
implementing the language. Haplolang an s-expression based imperative
language, greatly inspired by [Scheme](https://www.scheme.org/), and
my main motivation is purely recreational.

The current implementation was developed in two sittings: first the
parser and then the interpreter + cli program. Some functions are
implemented for basic math operations and printing, symbols are
currently limited to being only one character per symbol and you
cannot define functions or variables yet.

Everything is implemented in C. The project is composed of:

- `libhaplo`: the library containing all the logic
- `haplo`: cli-based interpreter
- `haplo_tests`: unit tests

```
$ ./haplo
The Haplolang interpreter by Giovanni Santini
> ( p ( + ( 1 2 ) )
3 
```

## Building

Simply build the library:

```bash
make lib
```

Build the library and the cli interpreter, you need to have
[readline](https://savannah.gnu.org/git/?group=readline) installed:

```
make cli
```

Build and run the tests:

```
make check
```
