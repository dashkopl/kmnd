# Kmnd [![Build status](https://travis-ci.org/timvanelsloo/kmnd.svg?branch=master)](https://travis-ci.org/timvanelsloo/kmnd)

Kmnd is a library in C for creating beautiful, colorful and consistent Command
Line Interfaces (CLIs). Kmnd runs on Mac OS X and Linux and has zero
dependencies.

## Features

- Easy to integrate with your existing C or C++ project using Git submodules and
  CMake.
- Built-in support for color output using ANSI escape codes.
- Markup language for text formatting (colors and font styles) in `--help` usage
  text.
- Parsers and validators for common types (booleans, floats, doubles, integers,
  unsigned integers and strings).
- Option retrieval by path with a simple syntax
  (`subcommand.subcommand.option`).
- Simple unambiguous grammar that makes it easy for other programs to wrap your
  CLI and replace specific options without explicitly knowing every option your
  program provides.
- Extensive unit tests.
- Licensed under MIT.

## Sample Code

Start by configuring your Kmnd in `main(...)`. `kmnd_new(...)` has a variable
argument list so you can add unlimited subcommands, options and inputs.

```c
int main(int argc, const char **argv) {
    kmnd_t *kmnd = kmnd_new("sample", NULL, run_command,
        kmnd_usage_new("sample `PATH`",
                       "Sample, the ``sample`` sampler\nsample."),

        kmnd_new("try", "*Try* `something`", try_command,
            kmnd_string_new('u', "url", "Try something from this URL",
                            KMND_FLAGS_NONE, 0),
             NULL
        ),

        kmnd_boolean_new('V', "verbose", "Show more debugging information",
                         KMND_FLAGS_NONE, 0),

        kmnd_input_new("path", "Path to the file you want to do nothing with.",
                       KMND_FLAGS_REQUIRED, NULL),

        NULL
    );

    return kmnd_run(kmnd, argc, argv);
}
```

The first command callback is called when no further subcommands are provided:

```c
static int run_command(kmnd_t *kmnd) {
    printf("Verbose: %d\n", kmnd_boolean_get(kmnd, "verbose"));

    return 0;
}
```

You can add additional callbacks for each subcommand. Options are inherited by
each subcommand.

```c
static int try_command(kmnd_t *kmnd) {
    printf("Verbose: %d\n", kmnd_boolean_get(kmnd, "verbose"));
    printf("Url: %s\n", kmnd_string_get(kmnd, "try.url"));

    return 0;
}
```

### Screenshot

<p align="center">
    <img src="docs/screenshots/osx.png?raw=true" width="682" />
</p>

## Getting Started

### Add Kmnd to your Application

First, add Kmnd as a submodule to Git. It is convenient to create a new
directory `deps` and store all external dependencies in it.

```bash
mkdir -p deps/
cd deps/
git submodule add https://github.com/timvanelsloo/kmnd
cd ../
```

Then add the following two lines to your `CMakeLists.txt`:

```c
add_subdirectory(deps/kmnd)
target_include_directories([target] PUBLIC deps/kmnd/include)
target_link_libraries([target] kmnd)
```

### Write your first Kmnd CLI

Kmnds are created in a hierarchical manner. You can add unlimited subcomamnds,
options and inputs.

Start with a command by calling:

```c
kmnd_t *kmnd_new(const char *name, const char *description, kmnd_run_cb *run,
                 kmnd_t *child, ...);
```

The `run` callback is optional but recommended. You can create unlimited
subcommands, options and inputs and pass them as `child`ren. Subcommands are
created with the same `kmnd_new(...)` function.

#### Options

Options are typed. Kmnd implements `intN`, `uintN`, `double`, `float`,
`boolean` and `string`. `N` ranges from 8 to 64 and indicates the number of
bits. The function prototypes are very similar. Substitute X with the type you
want to use:

```c
kmnd_t *kmnd_X_new(const char character, const char *name,
                   const char *description, const kmnd_flags_t flags,
                   <type> value);
```

Each option can be specified either by its "short name" (character), e.g. `-u`
or its "long name", e.g. `--user`. You can pass `KMND_FLAGS_REQUIRED` if you
want Kmnd to ensure that the user provides the option. You can also specify a
default `value`. Of course, providing a default `value` only makes sense when
the option is not required.

#### Inputs

Inputs are strings that can be validated by your own function. Validating is
optional. If you do not pass a validator function, all non-empty input is
accepted.

```c
kmnd_t *kmnd_input_new(const char *name, const char *description,
                      const kmnd_flags_t flags, kmnd_validator_cb validator);
```

Again, you can pass `KMND_FLAGS_REQUIRED` for inputs that are required.
Note that you can add multiple inputs to your (sub)commands but the order is
the determinant factor. Therefore, you should not add required inputs after
optional inputs, since the optional inputs will be evaluated first.

#### Retrieval

In your callback functions (e.g. `run_command`), you can access the provided
option values and inputs with `kmnd_X_get(kmnd_t *kmnd, const char *path)`.
X can be any of the supported option types, or `input` for inputs.

The `path` consists of the name(s) of each subcommand and finally the option or
input that you want to retrieve. For example: suppose the user runs
`./sample test run --verbose`, we can query the boolean value with
`test.run.verbose` (note: if `verbose` is a global option, i.e. added to
`test` instead of `run`, then you should use `test.verbose` instead, same goes
for `./sample` which would map to `verbose`).

## Contributing

If you want to contribute, start by cloning this repo. You'll also have to
download GoogeTest. GoogleTest is a submodule in `deps/` so you can simply
`init`ialize, `sync`hronize and `update` the submodules to clone it.

```bash
git clone https://github.com/timvanelsloo/kmnd
git submodule init
git submodule sync
git submodule update
```

Then use CMake to generate some Makefiles
(`mkdir -p build; cd build; cmake ..`). You can test your own developments with
the provided `sample.c` and `sample` executable target. Run `make sample` in
your CMake build directory to compile the code.

Then fork the repo, create a new branch, commit your changes and open a pull
request. Make sure to run the existing unit tests
(`make kmnd_tests; ./tests/kmnd_tests`) and to add new tests for the
functionality you added or patched.
