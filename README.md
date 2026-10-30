# My First Linux App

A beginner project for learning how to build a small **native Linux desktop app** in **C**, using the **GTK** toolkit for the window/UI and a **Makefile** to compile it.

If you've never touched Linux app development, C, or GTK before, that's exactly who this project is for. Every piece of it is explained from the ground up in [`docs/`](docs/README.md).

## What this app does

Right now it's intentionally simple: it opens a 400x300 window titled "My First App" with a single "Click Me" button. Clicking the button prints a message to the terminal. That's it — the point isn't the app itself, it's learning the *mechanics* of how a native Linux GUI program is put together: source code → compiler → binary → running window.

## Project layout

```
my-linux-app/
├── README.md                              ← you are here
├── docs/                                  ← full explanations, start at docs/README.md
└── src/
    ├── app.c                              ← the main app (GTK button demo)
    └── Following-Tutorial-Scaffolding/    ← smaller practice files from following a GTK tutorial
        ├── Makefile                       ← build rules for testGtkApp
        ├── test.c                         ← old-style GTK3 skeleton (gtk_init / gtk_main)
        └── testGtkApp.c                   ← modern GApplication-style skeleton, heavily commented
```

## Quick start

You'll need a C compiler and the GTK development libraries installed first — see [Installing the Tools](docs/getting-started/02-installing-tools.md) if you don't have them yet.

```bash
cd src/Following-Tutorial-Scaffolding
make
./testGtkApp
```

This builds and runs `testGtkApp.c` using the `Makefile` in that folder. For a full explanation of what each of those commands is actually doing, see [Makefiles Explained](docs/build-system/01-makefiles-explained.md) and [Compiling Manually](docs/build-system/02-compiling-manually.md).

## Where to learn more

Start here → **[docs/README.md](docs/README.md)** — it's an index of every topic this project touches, written assuming you know nothing about Linux, C, or GTK yet.
