# Documentation Index

Welcome! This is the documentation hub for **My First Linux App**. It's written for someone who has never built a native Linux app, never written C, and never heard of GTK — every doc starts from first principles and builds up.

You don't have to read these in order, but if you're totally new, reading top to bottom will make the most sense, because later docs lean on ideas explained earlier.

## 1. Getting Started

| Doc | What it covers |
|---|---|
| [What Is Linux (and a Native App)?](getting-started/01-what-is-linux.md) | What Linux actually is, what "native app" means, and what the terminal is |
| [Installing the Tools](getting-started/02-installing-tools.md) | Getting a C compiler, `make`, and the GTK libraries onto your machine |

## 2. The C Language

| Doc | What it covers |
|---|---|
| [A C Crash Course](c-language/01-c-crash-course.md) | The minimum C you need to read this project: functions, headers, `main`, compiling |
| [Pointers and Memory](c-language/02-pointers-and-memory.md) | What `*` and `&` mean, what a pointer actually is, why GTK code is full of them |

## 3. GTK (the GUI toolkit)

| Doc | What it covers |
|---|---|
| [What Is GTK?](gtk/01-what-is-gtk.md) | What a "toolkit" is, what widgets are, where GTK fits on a Linux system |
| [The Application Lifecycle](gtk/02-application-lifecycle.md) | How a GTK program starts, runs, and stops — the event loop |
| [Signals and Callbacks](gtk/03-signals-and-callbacks.md) | How GTK tells your code "the user clicked something" |

## 4. Walking Through This Project's Code

| Doc | What it covers |
|---|---|
| [`app.c` Explained Line by Line](code-walkthrough/01-app-c-explained.md) | The main app in this repo, explained top to bottom |
| [The Scaffolding Files Explained](code-walkthrough/02-scaffolding-files-explained.md) | `test.c` vs `testGtkApp.c` — old-style vs modern GTK, compared |

## 5. The Build System

| Doc | What it covers |
|---|---|
| [Makefiles Explained](build-system/01-makefiles-explained.md) | What `make` is, how `Makefile` syntax works, what this project's Makefile does |
| [Compiling Manually (without `make`)](build-system/02-compiling-manually.md) | What `make` is actually doing under the hood, run by hand with `gcc` and `pkg-config` |

## 6. Git & Project Hygiene

| Doc | What it covers |
|---|---|
| [`.gitignore` Explained](git-and-project/01-gitignore-explained.md) | What the `.gitignore` file does and why these specific patterns are in it |

## Suggested reading path for a total beginner

1. [What Is Linux (and a Native App)?](getting-started/01-what-is-linux.md)
2. [Installing the Tools](getting-started/02-installing-tools.md)
3. [A C Crash Course](c-language/01-c-crash-course.md)
4. [Pointers and Memory](c-language/02-pointers-and-memory.md)
5. [What Is GTK?](gtk/01-what-is-gtk.md)
6. [The Application Lifecycle](gtk/02-application-lifecycle.md)
7. [Signals and Callbacks](gtk/03-signals-and-callbacks.md)
8. [`app.c` Explained Line by Line](code-walkthrough/01-app-c-explained.md)
9. [The Scaffolding Files Explained](code-walkthrough/02-scaffolding-files-explained.md)
10. [Makefiles Explained](build-system/01-makefiles-explained.md)
11. [Compiling Manually](build-system/02-compiling-manually.md)
12. [`.gitignore` Explained](git-and-project/01-gitignore-explained.md)
