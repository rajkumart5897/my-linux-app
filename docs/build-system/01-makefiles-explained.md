# Makefiles Explained

*Previous: [The Scaffolding Files Explained](../code-walkthrough/02-scaffolding-files-explained.md) · Next: [Compiling Manually](02-compiling-manually.md)*

## What problem does `make` solve?

Compiling a C program by hand means typing a command like:

```bash
gcc `pkg-config --cflags gtk+-3.0` -o testGtkApp testGtkApp.c `pkg-config --libs gtk+-3.0`
```

That's a lot to remember and retype every time you change a line of code. `make` is a build-automation tool: you write the compile recipe **once**, in a file literally named `Makefile`, and from then on you just run:

```bash
make
```

`make` reads the `Makefile` in the current directory and figures out what needs rebuilding and how, based on the rules you wrote.

## This project's Makefile

From `src/Following-Tutorial-Scaffolding/Makefile`:

```makefile
# Makefile has it's own language, So it is not a shell script.
# It is a set of rules to build your project.

# Example of a simple action
# target: prerequisites
#	action

# It supports macro as well.
# PROG = test.c

# A command can be executed only if it is part of .PHONY: cmd-names
# Otherwise Make will assume it's a file.

PROG = myGtkWindow

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c $(LIBS)


.PHONY: push

push:
	git add -A
	@read -p "Enter commit message: " msg; \
	git commit -m "$$msg"
	git push
```

The comments at the top are already a good start (clearly written by someone learning `make` for the first time) — here's the fuller picture.

## Variables (the project calls them "macros")

```makefile
PROG = myGtkWindow
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
```

These define reusable names, referenced later as `$(PROG)`, `$(CC)`, etc. This is purely for convenience and consistency — if you ever renamed the program or changed compiler flags, you'd only need to edit one line instead of hunting through the whole file.

- `PROG = myGtkWindow` — the name of the program to build (without the `.c` extension). Note this doesn't match any filename sitting next to the Makefile any more — see the folder-layout note below.
- `CC = gcc` — which compiler to invoke. By convention, `CC` means "C Compiler" in Makefiles — it's a near-universal convention, not something unique to this project.
- `` CFLAGS = `pkg-config --cflags gtk+-3.0` `` — compiler flags needed to *find GTK's header files* during compilation. The backticks ``` `` ``` here are shell syntax, not Makefile syntax — they mean "run this command and substitute its output." So `pkg-config --cflags gtk+-3.0` actually runs and prints something like `-I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 ...` (a list of `-I` "include path" flags telling the compiler where to find GTK's headers), and that printed text becomes the value of `CFLAGS`.
- `` LIBS = `pkg-config --libs gtk+-3.0` `` — similarly, flags needed to *link against* the actual compiled GTK library files (things like `-lgtk-3 -lgdk-3 ...`), resolved the same way via `pkg-config`.

This is exactly why `pkg-config` is a required install (see [Installing the Tools](../getting-started/02-installing-tools.md)) — without it, you'd have to manually figure out and type GTK's exact include/library paths yourself, which vary between systems and GTK versions.

## A build rule — the actual recipe

```makefile
$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c $(LIBS)
```

This is a **rule**, written as:

```
target: prerequisites
	action
```

Read this as: "**`myGtkWindow`** (the target, i.e. the file we want to produce) depends on **`myGtkWindow.c`** (the prerequisite, i.e. the source file). If `myGtkWindow.c` is newer than `myGtkWindow` (or `myGtkWindow` doesn't exist yet), run this action to produce it."

The action line expands to:

```bash
gcc `pkg-config --cflags gtk+-3.0` -o myGtkWindow myGtkWindow.c `pkg-config --libs gtk+-3.0`
```

That's: compile `myGtkWindow.c` using `gcc`, with GTK's include paths, output a binary named `myGtkWindow` (`-o myGtkWindow`), and link in GTK's actual library files. The catch, covered fully below, is that there currently is no `myGtkWindow.c` sitting next to this Makefile for that rule to find.

**The action line must start with a literal Tab character, not spaces** — this is one of the most common beginner gotchas with Makefiles. If you ever hand-edit a Makefile and see an error like `missing separator`, this is almost always why.

This is also `make`'s core optimization: it only re-runs the action if the target is missing or out of date relative to its prerequisites, so running `make` repeatedly without changing anything does nothing (it'll just say the target is up to date).

## `.PHONY` — targets that aren't files

```makefile
.PHONY: push

push:
	git add -A
	@read -p "Enter commit message: " msg; \
	git commit -m "$$msg"
	git push
```

Normally, `make` treats a target name as a **file it should produce**, and checks "does this file already exist and is it up to date?" before deciding whether to run the recipe. But `push` isn't a file we're building — it's just a shortcut for a sequence of `git` commands (stage everything, prompt for a commit message, commit, push).

Declaring `.PHONY: push` tells `make`: "`push` is not a real file — always run its recipe when asked, don't try to check file timestamps for it." Without this, if a file literally named `push` ever existed in the folder, `make push` might do nothing, assuming it was already "up to date."

Inside the recipe:
- `git add -A` stages all changes in the repo.
- `@read -p "Enter commit message: " msg; \` — the `@` suppresses `make` from printing this line before running it (normally `make` echoes each command it runs); `read -p "..."` is a shell command that prompts you for input and stores it in the variable `msg`; the trailing `\` continues the recipe onto the next line as part of the same shell command (necessary because `read` sets a shell variable that needs to stay alive for the next line to use it).
- `git commit -m "$$msg"` — commits using the message you typed. The double `$$` is required because a single `$msg` would be interpreted by `make` itself as a Makefile variable (and likely resolve to nothing); `$$` tells `make` "pass a literal single `$` through to the shell," so the shell sees `$msg` and substitutes its own variable correctly.
- `git push` — pushes the commit to the remote repository.

You'd run this with `make push`.

## A heads-up: this Makefile targets a file that isn't where it expects it

`src/Following-Tutorial-Scaffolding/` now holds several subfolders — `Basics/`, `app-with-window/`, `app-with-window-fns/`, `gtk-window-fnctions/` — each with their own `.c` files (see [The Scaffolding Files Explained](../code-walkthrough/02-scaffolding-files-explained.md) for what's in each). The Makefile, though, lives one level up, directly in `Following-Tutorial-Scaffolding/`, and its rule — `$(PROG): $(PROG).c` with `PROG = myGtkWindow` — only ever looks for `myGtkWindow.c` **in the same directory as the Makefile itself**. The one file that name actually matches, `myGtkWindow.c`, is nested inside `app-with-window-fns/`, not sitting beside the Makefile. Running `make` from `Following-Tutorial-Scaffolding/` as it's currently laid out will fail with something like:

```
make: *** No rule to make target 'myGtkWindow.c', needed by 'myGtkWindow'.  Stop.
```

There are a few honest ways to fix this, none currently done in the repo:
- **Run `make` from inside the file's own folder**, after copying or symlinking the Makefile there (e.g. `cp ../Makefile app-with-window-fns/` then `cd app-with-window-fns && make`).
- **Point the rule at the nested path** by changing `PROG = myGtkWindow` to something like `PROG = app-with-window-fns/myGtkWindow` (and adjusting the `-o` output path), so the existing single Makefile can build this one file in place.
- **Compile directly with `gcc`**, bypassing the Makefile for now — see [Compiling Manually](02-compiling-manually.md), which shows exactly that command.

There's a second, independent issue worth knowing about too, but a narrower one than it might first look: this Makefile's `CFLAGS`/`LIBS` ask `pkg-config` for **`gtk+-3.0`** (GTK 3). Every scaffolding file in this folder (old-style and `GtkApplication`-style alike — `GtkApplication` itself isn't GTK-4-exclusive, see [The Scaffolding Files Explained](../code-walkthrough/02-scaffolding-files-explained.md) for why) builds fine against GTK 3 as written, so that part isn't actually a problem for anything in `Following-Tutorial-Scaffolding/`. It *would* matter if you ever tried to build `src/app.c` with this same Makefile, though — `app.c` uses `gtk_window_set_child`, a GTK-4-only function (GTK 3 uses `gtk_container_add` instead), so building it would need `gtk+-3.0` swapped for `gtk4` in both `pkg-config` calls, on top of pointing the rule at `app.c`'s own path.

## Running this Makefile, once the path is fixed

```bash
cd src/Following-Tutorial-Scaffolding/app-with-window-fns   # or wherever myGtkWindow.c actually ends up
make            # builds myGtkWindow (runs the $(PROG) rule — it's the first rule, so it's the default)
./myGtkWindow   # runs the resulting binary
```

Next: it's worth seeing exactly what `make` is doing for you by running the equivalent `gcc` command by hand — [Compiling Manually](02-compiling-manually.md).
