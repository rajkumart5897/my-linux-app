# The Scaffolding Files Explained

*Previous: [`app.c` Explained Line by Line](01-app-c-explained.md) · Next: [Makefiles Explained](../build-system/01-makefiles-explained.md)*

`src/Following-Tutorial-Scaffolding/` holds practice files from working through a GTK tutorial — not one finished app like `app.c`, but a sequence of stepping stones, roughly ordered from simplest to most developed:

```
Following-Tutorial-Scaffolding/
├── Makefile
├── Basics/                  ← bare skeletons, empty bodies
│   ├── test.c
│   └── testGtkApp.c
├── app-with-window/         ← same two skeletons, now each builds a real window
│   ├── test.c
│   └── testGtkApp.c
├── app-with-window-fns/     ← old-style window, plus GTK's window getter functions
│   └── myGtkWindow.c
└── gtk-window-fnctions/     ← next step — empty so far, nothing written yet
```

In both `Basics/` and `app-with-window/`, the same pair shows up — a `test.c` and a `testGtkApp.c` — and in each folder that pair represents the same contrast worth understanding on its own: the **old GTK 3** style versus the **modern, `GtkApplication`-based** style that `app.c` itself uses.

## `test.c` — the old-style GTK 3 skeleton

```c
# include <gtk/gtk.h>

int main (int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // gtk code comes here.

    gtk_main();

    return 0;
}
```
*(shown here as `Basics/test.c` — `app-with-window/test.c` is the same shape with real window code filling in the comment; see below)*

- `gtk_init(&argc, &argv)` initializes the GTK library directly — no `GtkApplication` object involved. It's passed `&argc`/`&argv` (addresses, i.e. pointers — see [Pointers and Memory](../c-language/02-pointers-and-memory.md)) because `gtk_init` can strip out and handle GTK's own recognized command-line flags, modifying `argc`/`argv` in place for you.
- The comment `// gtk code comes here.` marks where you'd create a window, widgets, and connect signals — directly in `main`, with no separate `activate` function needed, because there's no `"activate"` signal to wait for in this style.
- `gtk_main()` starts the event loop directly (compare to `g_application_run()` in the modern style) and blocks until the app quits.
- `return 0;` — a plain, hardcoded success exit code, since there's no `GtkApplication` object to produce a `status` value from.

In `Basics/`, this file is a minimal, unfinished skeleton — it compiles, but running it does nothing visible, since no window-building code has been filled in yet.

## `testGtkApp.c` — the modern `GApplication` skeleton

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

This is structurally identical to `app.c`'s `main` function and `activate` setup — a `GtkApplication` object, an `"activate"` callback (currently empty here, marked `//gtk code comes here.`), and `g_application_run`. The variable is named `ret` instead of `status`, and the app ID is `"in.raeark"` instead of `"com.example.FirstApp"`, but functionally this is the exact pattern explained in [The Application Lifecycle](../gtk/02-application-lifecycle.md) and [`app.c` Explained](01-app-c-explained.md). It's also heavily self-commented in the real file — the inline comments explaining what each call's arguments mean are worth reading alongside this page.

| | `test.c` style | `testGtkApp.c` style |
|---|---|---|
| Initialization | `gtk_init(&argc, &argv)` | `gtk_application_new(id, flags)` |
| "App is ready" signal | None — build UI directly in `main` | `"activate"` signal on the `GtkApplication` |
| Event loop | `gtk_main()` | `g_application_run(...)` |
| Exit status | Hardcoded `0` | Whatever `g_application_run` returns |
| App identity | None | A reverse-DNS app ID string |

One correction to a claim that's easy to assume and worth being precise about: `GtkApplication` itself is **not** a GTK-4-only feature — `gtk_application_new` has existed since GTK 3.0, and `gtk_application_window_new` since GTK 3.4. So "modern, `GtkApplication`-based" and "GTK 4" aren't actually the same thing — plenty of real GTK 3 code uses `GtkApplication`. Both `test.c` and `testGtkApp.c` here, as written, build fine against GTK 3 (which is what this folder's Makefile targets — see [Makefiles Explained](../build-system/01-makefiles-explained.md)). The one call in this whole project that genuinely *does* require GTK 4 is `gtk_window_set_child`, used in [`app.c`](01-app-c-explained.md) — GTK 3 has no such function on `GtkWindow` and uses the generic `gtk_container_add` instead. That's the real version boundary to watch for here, not the choice between `gtk_init`/`gtk_main` and `GtkApplication`.

## `app-with-window/` — the same pair, now building a real window

This folder fills in each skeleton's `// gtk code comes here.` placeholder with actual window-creation code.

**`app-with-window/test.c`:**

```c
# include <gtk/gtk.h>

int main (int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;

    // gtk window new needs an arg, it can be toplevel or popup window.

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // gtk_main_quit is used to terminate the GTK main loop when the window is closed.

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
```

Worth noticing here: with no `GtkApplication` to automatically end the event loop when the window closes, this file has to connect the window's `"destroy"` signal to `gtk_main_quit` itself — skip that line and closing the window would leave `gtk_main()` running forever with nothing left on screen. `gtk_widget_show_all` is the old-style call that recursively shows every widget in the tree; `app.c`'s modern equivalent is `gtk_window_present`.

**`app-with-window/testGtkApp.c`:**

```c
# include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data){
    GtkWidget *window;

    window = gtk_application_window_new (app);

    gtk_widget_show_all(window);
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

This fills in `activate()` with the minimum needed to see a window: create it with `gtk_application_window_new(app)`, then show it with `gtk_widget_show_all(window)` — one step past `Basics/testGtkApp.c`'s fully empty `activate`, and an earlier snapshot of the same growth that leads to `app.c`'s fuller version (title, size, a button). Both calls here are valid GTK 3 (see the correction above — `GtkApplication` isn't GTK-4-exclusive), so this file, as written, builds fine against the `gtk+-3.0` this folder's Makefile targets.

It's worth contrasting this with `app.c` anyway, since the two take a different final step: `app.c` calls `gtk_window_present(GTK_WINDOW(window))` to show its window, while this file calls `gtk_widget_show_all(window)`. Both work in GTK 3. If this file (or `app.c`'s approach) were ever ported to GTK 4, `gtk_widget_show_all` would need to change — GTK 4 removed it entirely (widgets are visible by default unless explicitly hidden) in favor of `gtk_window_present` or a plain `gtk_widget_show`. `app.c` already uses the GTK-4-compatible call here; this scaffolding file still uses the older one.

## `app-with-window-fns/myGtkWindow.c` — querying a window, not just building one

```c
# include <gtk/gtk.h>

int main (int argc, char *argv[]) {

    const char *title;
    int width, height;

    gtk_init(&argc, &argv);

    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Rae ARK");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    //gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    //gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    //gtk_window_maximize(GTK_WINDOW(window));
    //gtk_window_fullscreen(GTK_WINDOW(window));

    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);

    title = gtk_window_get_title(GTK_WINDOW(window));
    printf("Your app title is: %s\n", title);

    gtk_window_get_size(GTK_WINDOW(window), &width, &height);
    printf("Your app size is: %d x %d\n", width, height);

    printf("GTK Window Decorated? %d\n", gtk_window_get_decorated(GTK_WINDOW(window)));
    printf("GTK Window Maximized? %d\n", gtk_window_is_maximized(GTK_WINDOW(window)));
    printf("GTK Window is Active? %d\n", gtk_window_is_active(GTK_WINDOW(window)));

    g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
```

Old-style again (`gtk_init`/`gtk_main`, `gtk_widget_show_all` — all GTK 3), but exploring a different question: not how to *build* a window, but how to *ask GTK about one you already built*, using a family of `gtk_window_get_*` functions and `printf` (plain C standard library — fine here, though `g_print` is GTK's own near-equivalent).

A few details worth being precise about:

- **`gtk_window_get_title`** returns a `const char *` — a pointer to a string GTK owns internally (the `const` means this code may read it but shouldn't modify or free it). See [Pointers and Memory](../c-language/02-pointers-and-memory.md).
- **`gtk_window_get_size(..., &width, &height)`** writes its results through pointers rather than returning a value directly — a common C pattern for a function that needs to hand back more than one result at once.
- **The commented-out lines** (`set_position`, `set_resizable`, `maximize`, `fullscreen`) are exactly what they look like: other window functions tried or planned, left as notes rather than deleted. Real in-progress tutorial code like this is useful exposure to GTK's wider API surface beyond what `app.c` itself touches.
- **`gtk_window_is_maximized`** and **`gtk_window_is_active`** reflect the window's state at the moment they're called — since this runs immediately after window creation and before `gtk_main()` starts the event loop, both will reliably print `0` here, because nothing has shown, focused, or maximized the window yet.

## `gtk-window-fnctions/` — nothing here yet

This folder exists but is currently empty. Going by the naming pattern, it's a reasonable guess it continues what `app-with-window-fns/` started, but there's nothing in it to document yet — check back once it's populated.

## Why keep all of this around

Each folder is a snapshot of a learning progression: bare skeleton → skeleton with a window → window with introspection. Kept side by side with both the old and modern GTK styles, the contrast stays concrete instead of abstract, and — as the `app-with-window/testGtkApp.c` caveat above shows — it's also a realistic record of what actually happens partway through learning a toolkit that changed its API between major versions: code that mixes eras is normal, not a sign something's broken.

One practical consequence worth knowing: **the scaffolding folder's `Makefile` targets GTK 3** (`` `pkg-config --cflags gtk+-3.0` ``), and none of these files currently sit next to it any more — they're nested inside subfolders. See [Makefiles Explained](../build-system/01-makefiles-explained.md) for exactly what that means for building any of these as-is.

Next: let's understand the file that actually drives compilation — [Makefiles Explained](../build-system/01-makefiles-explained.md).
