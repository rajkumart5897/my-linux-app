# A C Crash Course

*Previous: [Installing the Tools](../getting-started/02-installing-tools.md) · Next: [Pointers and Memory](02-pointers-and-memory.md)*

This isn't a full C course — it's the minimum vocabulary you need to read the code in this repo without getting lost. We'll use real snippets from `src/app.c` as examples.

## C is compiled, not interpreted

Languages like Python or JavaScript are typically **interpreted**: you run the source file directly, and another program reads it line by line as it goes. C is **compiled**: a separate program (the compiler, `gcc`) reads your entire `.c` file and translates it, ahead of time, into a binary file your CPU can execute directly. That compiled file is what you actually run — the original `.c` text isn't needed anymore once you have it.

```bash
gcc app.c -o app   # compile app.c into a runnable file named "app"
./app              # run it
```

This is also why **Makefiles exist**: compiling by hand gets tedious once a project has multiple files and specific flags, so a Makefile automates the exact compile command. More on that in [Makefiles Explained](../build-system/01-makefiles-explained.md).

## `#include` — pulling in other code

```c
#include <gtk/gtk.h>
```

C programs are built from small, reusable pieces. `#include` tells the compiler "before you process this file, paste in the contents of this other file." `<gtk/gtk.h>` is a **header file** — it declares all the function names, types, and constants that the GTK library provides, so the compiler knows they exist and what their signatures look like, even though their actual implementation lives in a separately compiled library file. Angle brackets (`<...>`) mean "look for this in the system's standard header locations" (which is where `pkg-config` and the `-dev` package come in — see [Installing the Tools](../getting-started/02-installing-tools.md)).

## Functions

```c
static void activate(GtkApplication *app, gpointer user_data)
{
    // ...
}
```

A function has:
- A **return type** (`void` here — this function doesn't give back a value when it finishes).
- A **name** (`activate`).
- A **parameter list** in parentheses — the inputs the function accepts, each with its own type.
- A **body** in `{ }` — the code that runs when the function is called.

`static` in front means this function is only visible/usable within this one `.c` file — it can't be called from other files in the project. This is a common way to mark "internal helper" functions.

## `main` — where every C program starts

```c
int main(int argc, char **argv)
{
    // ...
    return status;
}
```

Every C program has exactly one `main` function, and it's always the first thing that runs. Its return value becomes the program's **exit status** — a number the operating system (and other programs/scripts) can check to see whether the program succeeded (`0`) or failed (anything else).

`int argc` and `char **argv` are how command-line arguments get into your program — `argc` is the count of arguments, `argv` is the list of argument text. If you ran `./app --fullscreen`, `argc` would be `2` and `argv` would contain `"./app"` and `"--fullscreen"`. This project doesn't currently use any arguments, but GTK's `g_application_run` still expects you to pass them through, in case GTK itself wants to read standard flags.

## Variables and types

```c
GtkApplication *app;
int status;
```

C is **statically typed** — every variable's type is declared up front and can't change. `int status` is an ordinary integer. `GtkApplication *app` is a **pointer** to a `GtkApplication` — the `*` is important and is covered in depth in [Pointers and Memory](02-pointers-and-memory.md), because almost every GTK type you'll touch is used through a pointer like this.

## Semicolons and blocks

Every C statement ends with a semicolon `;`. Curly braces `{ }` group statements into a block — the body of a function, or an `if`, or a loop. Indentation (how the code is visually spaced) is purely for human readability in C; the compiler doesn't care about it at all, unlike Python.

## Comments

```c
// This is a single-line comment, ignored by the compiler
/* This is a
   multi-line comment */
```

You'll see both styles used in `testGtkApp.c` — they're purely documentation for humans reading the code.

## What's "calling a function" in this project's style?

```c
window = gtk_application_window_new(app);
```

This calls the function `gtk_application_window_new`, passing in `app` as its argument, and stores whatever it returns into the variable `window`. GTK's functions are consistently named `gtk_<noun>_<verb>` (e.g. `gtk_window_set_title`, `gtk_button_new_with_label`) — once you notice the pattern, GTK code becomes much easier to skim, because the function name usually tells you exactly what it does.

Next: pointers are everywhere in this code, and they're the concept most beginners trip on — [Pointers and Memory](02-pointers-and-memory.md) covers them properly before we move into GTK itself.
