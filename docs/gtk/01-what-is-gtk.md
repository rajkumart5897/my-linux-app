# What Is GTK?

*Previous: [Pointers and Memory](../c-language/02-pointers-and-memory.md) · Next: [The Application Lifecycle](02-application-lifecycle.md)*

## The problem GTK solves

Drawing a window, a button, handling mouse clicks and keyboard input, redrawing pixels on screen — all of this requires talking to very low-level, operating-system-specific graphics code. Writing that from scratch for every app would be enormous, repetitive, and error-prone.

A **GUI toolkit** is a library that does all of that plumbing for you and exposes a much simpler, higher-level programming interface: "create a window," "create a button," "run this function when it's clicked." **GTK** (originally "GIMP Toolkit," named after the GNU Image Manipulation Program it was first built for) is one of the major GUI toolkits for Linux. It's what many well-known Linux desktop apps, and the GNOME desktop environment itself, are built with.

Other platforms have their own equivalents — Windows has WinForms/WPF, macOS has AppKit/SwiftUI, and there are also cross-platform toolkits like Qt. GTK is specifically the one this project uses.

## Widgets — the building blocks

In GTK, every visible piece of UI — a window, a button, a label, a text box, a layout container — is called a **widget**. All widget types share a common base type, `GtkWidget`, which is why you'll see `GtkWidget *` used generically even when a variable actually holds something more specific, like a button:

```c
GtkWidget *button;
button = gtk_button_new_with_label("Click Me");
```

Widgets are usually arranged in a **tree**: a window contains a single "child" widget (which might be a container holding several more widgets inside it, arranged in a layout). In this project's `app.c`, the window's one child is a single button:

```c
gtk_window_set_child(GTK_WINDOW(window), button);
```

## GTK isn't just "the window" — it's a whole ecosystem

GTK comes bundled with (and depends on) several supporting libraries you'll see referenced indirectly:

- **GLib** — provides core non-UI utilities GTK is built on: the object/type system (`GObject`, which is where `g_signal_connect` and `g_object_unref` come from), data structures, and the application/event-loop machinery (`GApplication`).
- **Pango** — text layout and rendering.
- **Cairo** — 2D drawing.

You don't need to use these directly for this project, but you'll notice that many of the "plumbing" functions in this code (`g_print`, `g_signal_connect`, `g_object_unref`, `GApplication`) have a `g_`/`G` prefix rather than `gtk_`/`Gtk` — that's your signal that it's GLib/GObject machinery underneath GTK, not a GTK-specific widget function.

## GTK 3 vs GTK 4

This matters specifically for this repo because it currently mixes both. GTK went through a deliberate, breaking API redesign between major version 3 and version 4 — function names, recommended patterns, and some widget behavior changed. The practical difference you'll see in this project:

- **GTK 3 style** (`test.c`): manual initialization with `gtk_init()`, and a manual event loop with `gtk_main()`.
- **GTK 4 style** (`app.c`, `testGtkApp.c`): the modern pattern, where a `GtkApplication` object manages initialization and the event loop for you via `g_application_run()`.

The [Application Lifecycle](02-application-lifecycle.md) doc and [Scaffolding Files Explained](../code-walkthrough/02-scaffolding-files-explained.md) walk through exactly what that difference looks like in code.

## Where GTK fits in the bigger picture

```
Your C code (app.c)
      │
      ▼
GTK  (windows, buttons, widgets)
      │
      ▼
GLib/GObject  (signals, event loop, object system)
      │
      ▼
Lower-level graphics libraries (Cairo, Pango, the windowing system)
      │
      ▼
The Linux kernel / display server
```

Your code mostly only ever talks to the top layer (GTK functions) and occasionally the GLib layer directly (signals, `g_print`). Everything below that is handled for you.

Next: [The Application Lifecycle](02-application-lifecycle.md) — how a GTK program actually starts up, stays running, and shuts down.
