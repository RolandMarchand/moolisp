# Moolisp

Moolisp is a simple Lisp-like language implemented in C. It is a project created as part of the "Make a Lisp" challenge, which challenges participants to create their own Lisp interpreter in any language.

## Getting Started
### Prerequisites

To build and run Moolisp, you will need:

* A C compiler
* CMake

### Building Moolisp

To build Moolisp, run the following command:

```bash
mkdir build
cmake -S . -B build
```

This will compile the `moolisp` executable. You can then run Moolisp with:

```bash
./build/moolisp
```

### Using Moolisp

Moolisp supports a subset of Lisp syntax, including lists, symbols, and basic arithmetic. You can interact with the Moolisp interpreter by entering expressions at the prompt.

For example, you can evaluate a simple arithmetic expression like this:

```
> (+ 1 2)
3
```

You can also define variables and functions:

```lua
> (define pi 3.14159)
nil
> (define (square x) (* x x))
nil
> (square pi)
9.869587728
```

For more information on using Moolisp, see the included docs directory.

## Contributing

Moolisp is an open-source project, and contributions are welcome! If you find a bug, have an idea for a new feature, or just want to contribute to the project, please open an issue or submit a pull request on GitHub.

## License

Moolisp is licensed under the BSD Zero Clause License. See the LICENSE file for more information.
