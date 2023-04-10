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
🐑) (+ 1 2)
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

This software is licensed under the GNU General Public License v3.0
(GPLv3). You may use, distribute, and modify this software in
accordance with the terms and conditions of the GPLv3. A copy of the
license can be found in the LICENSE file included with this software.

Please note that by using, distributing, or modifying this software,
you are agreeing to be bound by the terms and conditions of the
GPLv3. If you do not agree with these terms and conditions, you may
not use, distribute, or modify this software.
