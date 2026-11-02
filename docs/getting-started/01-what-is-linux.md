# What Is Linux (and What's a "Native App")?

*Previous: [Documentation Index](../README.md) · Next: [Installing the Tools](02-installing-tools.md)*

## What is Linux?

Linux is an **operating system kernel** — the core piece of software that manages your computer's hardware (CPU, memory, disk, screen) and lets every other program talk to that hardware safely. When people say "Linux" casually, they usually mean a full operating system *built around* that kernel — things like Ubuntu, Fedora, or Debian. These are sometimes called "Linux distributions" or "distros," because different groups package the same kernel with different tools, default software, and visual styles.

Compare this to Windows or macOS: same basic job (manage hardware, run programs, show you a desktop), different company and different internals. Linux is free, open-source (anyone can read and modify its code), and it's the operating system that powers most web servers, many phones (Android is built on the Linux kernel), and a growing share of desktop computers.

## What does "native app" mean?

A **native app** is a program compiled to run directly on a specific operating system, talking straight to its libraries and system calls — as opposed to:

- **A web app**, which runs inside a browser (the browser is the native app; your code is just text/data it interprets).
- **A cross-platform app**, which uses a compatibility layer so one codebase can (in theory) run on Windows, macOS, and Linux.

This project builds a native **Linux** app: it's compiled C code that links directly against Linux's GTK graphics libraries and produces a binary file that only runs on Linux (or Linux-compatible) systems. There's no browser involved and no "translation layer" — the compiled program talks to the operating system directly.

## The terminal

You'll see commands like `make` or `gcc app.c -o app` throughout this documentation. These are typed into a **terminal** (also called a shell, console, or command line) — a text-based interface for giving your computer instructions, instead of clicking icons.

A few conventions used throughout these docs:

- `$` at the start of a line means "type this into your terminal" — don't type the `$` itself.
- `cd some-folder` means "change directory" — move into that folder.
- Commands run **relative to whatever folder your terminal is currently in**, so if a doc says "run `make` from `src/Following-Tutorial-Scaffolding/`," you need to `cd` there first.

## Why C and a Makefile?

C is one of the oldest and most foundational programming languages still in active use. It compiles directly to machine code with very little hidden machinery, which makes it a common starting point for learning "how computers actually run programs," and it's the language most core Linux tools and libraries (including GTK) are written in.

A `Makefile` is a build-automation tool — instead of typing out a long, error-prone compiler command every time you change your code, you write the recipe once and just type `make`. The [Makefiles Explained](../build-system/01-makefiles-explained.md) doc covers this in depth.

## Where this fits

| Term | What it means here |
|---|---|
| Linux | The operating system this app is built for and runs on |
| Terminal | Where you'll type commands like `make` to build and run the app |
| Native app | A compiled program that talks directly to Linux, with no browser or VM |
| C | The programming language the app's source code is written in |
| GTK | The library that gives the app its window, button, and other visuals (see [What Is GTK?](../gtk/01-what-is-gtk.md)) |
| Makefile | The recipe file that tells the `make` tool how to compile the app |

Next, let's get the actual tools installed: [Installing the Tools](02-installing-tools.md).
