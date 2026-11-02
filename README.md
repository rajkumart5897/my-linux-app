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
    └── Following-Tutorial-Scaffolding/    ← smaller practice files from following a GTK tutorial,
        │                                    roughly ordered from simplest to most developed
        ├── Makefile                       ← build rules, currently targets myGtkWindow.c
        ├── Basics/                        ← bare skeletons, empty bodies
        │   ├── test.c                     ← old-style GTK3 skeleton (gtk_init / gtk_main)
        │   └── testGtkApp.c               ← modern GApplication-style skeleton, heavily commented
        ├── app-with-window/               ← same two skeletons, now each actually builds a window
        │   ├── test.c                     ← old-style: window + title + size + destroy signal
        │   └── testGtkApp.c               ← GApplication-style: window created inside activate()
        ├── app-with-window-fns/           ← old-style window, plus GTK's window getter functions
        │   └── myGtkWindow.c              ← get_title / get_size / get_decorated / is_maximized / is_active
        └── gtk-window-fnctions/           ← next step in the tutorial — empty so far, nothing to build yet
```

## Quick start

You'll need a C compiler and the GTK development libraries installed first — see [Installing the Tools](docs/getting-started/02-installing-tools.md) if you don't have them yet.

The `Makefile` in `src/Following-Tutorial-Scaffolding/` currently expects a `myGtkWindow.c` sitting next to it, but the only file with that name actually lives one level down, in `app-with-window-fns/`, so running `make` from the `Following-Tutorial-Scaffolding/` root as-is will fail. Until that's reconciled (see the note in [Makefiles Explained](docs/build-system/01-makefiles-explained.md#a-heads-up-this-makefile-targets-a-file-that-isnt-where-it-expects-it)), build any one of the scaffolding files directly:

```bash
cd src/Following-Tutorial-Scaffolding/app-with-window
gcc `pkg-config --cflags gtk+-3.0` -o testGtkApp testGtkApp.c `pkg-config --libs gtk+-3.0`
./testGtkApp
```

For a full explanation of what that command is doing (and how it relates to the Makefile), see [Makefiles Explained](docs/build-system/01-makefiles-explained.md) and [Compiling Manually](docs/build-system/02-compiling-manually.md). For what each scaffolding file actually contains, see [The Scaffolding Files Explained](docs/code-walkthrough/02-scaffolding-files-explained.md).

## Where to learn more

Start here → **[docs/README.md](docs/README.md)** — it's an index of every topic this project touches, written assuming you know nothing about Linux, C, or GTK yet.
