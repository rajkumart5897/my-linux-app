# The Scaffolding Files Explained

*Previous: [`app.c` Explained Line by Line](01-app-c-explained.md) · Next: [Makefiles Explained](../build-system/01-makefiles-explained.md)*

The `src/Following-Tutorial-Scaffolding/` folder holds practice files from working through a GTK tutorial. There are two C files here, and comparing them side by side is genuinely useful — they show the **old** and **new** ways of starting a GTK application, which otherwise might look like unrelated "magic" the first time you see each one.

## `test.c` — the old-style GTK 3 skeleton

```c
#include <gtk/gtk.h>

int main (int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // gtk code comes here.

    gtk_main();

    return 0;
}
```

This is GTK's older, more manual pattern:

- `gtk_init(&argc, &argv)` initializes the GTK library directly — no `GtkApplication` object involved. It's passed `&argc`/`&argv` (addresses, i.e. pointers — see [Pointers and Memory](../c-language/02-pointers-and-memory.md)) because `gtk_init` can strip out and handle GTK's own recognized command-line flags, modifying `argc`/`argv` in place for you.
- The comment `// gtk code comes here.` marks where you'd create a window, widgets, and connect signals — directly in `main`, with no separate `activate` function needed, because there's no `"activate"` signal to wait for in this style.
- `gtk_main()` starts the event loop directly (compare to `g_application_run()` in the modern style) and blocks until the app quits.
- `return 0;` — a plain, hardcoded success exit code, since there's no `GtkApplication` object to produce a `status` value from.

This file is a minimal, unfinished skeleton — it compiles, but running it does nothing visible yet, because no window-building code has been filled in.

## `testGtkApp.c` — the modern GApplication skeleton

```c
# include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data){
    //gtk code comes here.
}

int main (int argc, char *argv[]) {

    GtkApplication *app;
    int ret;

    app = gtk_application_new ("in.raeark", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    ret = g_application_run (G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return ret;
}
```

This is structurally identical to `src/app.c`'s `main` function and `activate` setup — a `GtkApplication` object, an `"activate"` callback (currently empty, marked with `//gtk code comes here.`), and `g_application_run`. The variable is named `ret` instead of `status`, and the app ID is `"in.raeark"` instead of `"com.example.FirstApp"`, but functionally this is the exact pattern explained in [The Application Lifecycle](../gtk/02-application-lifecycle.md) and [`app.c` Explained](01-app-c-explained.md).

This file is also heavily self-commented — the comments inside it (e.g. explaining what `gtk_application_new`'s arguments mean, what `G_CALLBACK` does) are a good sign of someone taking notes while learning, which is exactly the spirit of this whole project.

## Side-by-side comparison

| | `test.c` (GTK 3 style) | `testGtkApp.c` (GTK 4 style, modern) |
|---|---|---|
| Initialization | `gtk_init(&argc, &argv)` | `gtk_application_new(id, flags)` |
| "App is ready" signal | None — build UI directly in `main` | `"activate"` signal on the `GtkApplication` |
| Event loop | `gtk_main()` | `g_application_run(...)` |
| Exit status | Hardcoded `0` | Whatever `g_application_run` returns |
| App identity | None | A reverse-DNS app ID string |
| GTK version targeted | GTK 3 | GTK 4 |

## Why does this project have both?

Almost certainly because the tutorial being followed covered both as a learning progression — showing the simpler/older style first, then introducing the `GtkApplication`-based modern approach that `app.c` itself actually uses. Keeping both around is genuinely useful as a reference: if you ever read older GTK tutorials, blog posts, or codebases online, you'll recognize the `gtk_init`/`gtk_main` pattern immediately instead of being confused by it.

One practical consequence worth knowing: **the scaffolding folder's `Makefile` is set up for GTK 3** (`` `pkg-config --cflags gtk+-3.0` ``), but `testGtkApp.c` is written in the **GTK 4** style. See [Makefiles Explained](../build-system/01-makefiles-explained.md) for what that means for actually building this file, and what you'd need to change to build it correctly against GTK 4.

Next: let's understand the file that actually drives compilation — [Makefiles Explained](../build-system/01-makefiles-explained.md).
