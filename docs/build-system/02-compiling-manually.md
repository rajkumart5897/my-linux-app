# Compiling Manually (Without `make`)

*Previous: [Makefiles Explained](01-makefiles-explained.md) · Next: [`.gitignore` Explained](../git-and-project/01-gitignore-explained.md)*

`make` is convenient, but it's just running `gcc` with specific flags on your behalf. Understanding the underlying command demystifies both `make` and the general process of building any C program that depends on an external library like GTK.

## The command, piece by piece

To compile `src/app.c` by hand (assuming GTK 4's dev package is installed — see [Installing the Tools](../getting-started/02-installing-tools.md)):

```bash
gcc `pkg-config --cflags gtk4` -o app app.c `pkg-config --libs gtk4`
```

Let's take this apart left to right.

### `gcc`

The compiler itself — the program that turns `.c` source files into a runnable binary.

### `` `pkg-config --cflags gtk4` ``

Remember: backticks mean "run this and substitute its output here." Running `pkg-config --cflags gtk4` on its own prints something like:

```
-pthread -I/usr/include/gtk-4.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 ...
```

These are **compiler flags** telling `gcc` where to find the header files (`.h` files — see [A C Crash Course](../c-language/01-c-crash-course.md#include--pulling-in-other-code)) that `#include <gtk/gtk.h>` needs. GTK isn't one single header — it pulls in headers from several related libraries (Pango for text, GLib for core utilities, etc.), and `pkg-config` knows all of their locations on your specific system so you don't have to.

### `-o app`

Tells `gcc` to name the output binary `app`. Without `-o`, `gcc` would default to naming it `a.out`.

### `app.c`

The source file to compile.

### `` `pkg-config --libs gtk4` ``

This runs `pkg-config --libs gtk4`, which prints something like:

```
-pthread -lgtk-4 -lpango-1.0 -lgobject-2.0 -lglib-2.0 ...
```

These are **linker flags** — unlike `--cflags` (which only affects finding headers during *compilation*), these tell the compiler which actual compiled library files to **link** against so the final binary contains (or correctly references) GTK's real, compiled code. Each `-lsomething` means "link against the library named `libsomething`." Header files only declare that functions exist; the library files are where those functions are actually implemented, so both are required.

## Why two separate `pkg-config` calls (`--cflags` and `--libs`)?

Compiling a C program with external libraries happens in (conceptually) two phases:

1. **Compilation** — turn your `.c` text into object code, checking that every function/type you use has been declared somewhere (via headers). This is what `--cflags`'s `-I` (include path) flags support.
2. **Linking** — stitch your compiled code together with the actual compiled library code it calls into, producing one final runnable binary. This is what `--libs`'s `-l` (link) flags support.

`gcc app.c -o app` as shown above does both phases in one command for convenience, but it's worth knowing they're conceptually separate steps, because larger projects sometimes split them (compiling multiple `.c` files into separate object files first, then linking them all together at the end).

## Running it

```bash
./app
```

The `./` is necessary on Linux because, for security reasons, the current directory generally isn't automatically searched for executables the way it might be on other systems — you have to explicitly say "run the file named `app` in this directory" rather than just typing `app`.

## What `make` was actually saving you from

Compare the manual command above to simply typing:

```bash
make
```

Once the Makefile exists, `make` remembers the exact `gcc` invocation (including the correct `pkg-config` calls) for you, and — critically — only re-runs it when `app.c` has actually changed since the last successful build. As a project grows beyond one file, this "only rebuild what changed" behavior becomes genuinely important for keeping build times fast; for a single-file project like this one it's mostly about convenience and reproducibility (anyone who clones this repo can just type `make` without knowing the correct flags by heart).

## If something goes wrong

- **`fatal error: gtk/gtk.h: No such file or directory`** — the GTK development package isn't installed, or `pkg-config` can't find it. Revisit [Installing the Tools](../getting-started/02-installing-tools.md).
- **`undefined reference to 'gtk_...'`** — the headers were found, but the linker flags (`--libs`) are missing or wrong, so the compiler doesn't know which library file actually implements the functions you're calling. Double check you're not missing the `` `pkg-config --libs ...` `` portion of the command.
- **Mixing GTK 3 and GTK 4 flags** — as discussed in [Makefiles Explained](01-makefiles-explained.md#a-heads-up-this-makefile-targets-gtk-3-but-testgtkappc-is-gtk-4-code), make sure the `gtk+-3.0`/`gtk4` string you pass to `pkg-config` actually matches the GTK version your source file's function calls were written against.

Next: a small but important file for keeping your repository clean — [`.gitignore` Explained](../git-and-project/01-gitignore-explained.md).
