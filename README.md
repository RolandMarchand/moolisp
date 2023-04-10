# Moolisp

Moolisp is a simple Lisp-like language implemented in C. It is a
project created as part of the "Make a Lisp" challenge, which
challenges participants to create their own Lisp interpreter in any
language.

## Getting Started
### Prerequisites

To build and run Moolisp, you will need:

* A C compiler
* CMake
* GNU Readline

### Building Moolisp

To build Moolisp, run the following command:

```bash
git clone --recurse-submodules git@github.com:RolandMarchand/moolisp.git &&
cd moolisp &&
mkdir build &&
cmake -S . -B build &&
cd build &&
make
```

This will compile the `moolisp` executable. You can then run Moolisp
with:

```bash
./src/moolisp
```

### Using Moolisp

Moolisp supports a subset of Lisp syntax, including lists, symbols,
and basic arithmetic. You can interact with the Moolisp interpreter by
entering expressions at the prompt.

For example, you can evaluate a simple arithmetic expression like
this:

```scheme
🐑λ (+ 1 2)
3
```

You can also define variables and functions:

```scheme
🐑) (define pi 3.14159)
pi
🐑) (define square (lambda (x) (* x x)))
square
🐑) (square pi)
9.86959
```

For more information on using Moolisp, see the included docs
directory.

## Contributing

Moolisp is an open-source project, and contributions are welcome! If
you find a bug, have an idea for a new feature, or just want to
contribute to the project, please open an issue or submit a pull
request on GitHub.

## License

Copyright (C) 2023 by Roland Marchand <roland.marchand@protonmail.com>

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
