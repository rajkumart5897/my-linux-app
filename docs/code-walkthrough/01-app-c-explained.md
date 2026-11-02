# `app.c` Explained Line by Line

*Previous: [Signals and Callbacks](../gtk/03-signals-and-callbacks.md) · Next: [The Scaffolding Files Explained](02-scaffolding-files-explained.md)*

This is the full file, `src/app.c`, followed by a line-by-line explanation. If any individual concept below feels unfamiliar, it's covered in more depth in the earlier docs linked throughout — this page is meant to tie everything together on real code rather than re-teach each concept from scratch.

```c
#include <gtk/gtk.h>

static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Button clicked!\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;

    window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "My First App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    button = gtk_button_new_with_label("Click Me");

    gtk_window_set_child(GTK_WINDOW(window), button);

    g_signal_connect(
        button,
        "clicked",
        G_CALLBACK(on_button_clicked),
        NULL
    );

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "com.example.FirstApp",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );

    status = g_application_run(
        G_APPLICATION(app), argc, argv
    );

    g_object_unref(app);

    return status;
}
```

## `#include <gtk/gtk.h>`

Pulls in every declaration GTK provides — every function name and type used throughout this file (`GtkApplication`, `gtk_window_present`, and so on) comes from this one header. See [A C Crash Course](../c-language/01-c-crash-course.md#include--pulling-in-other-code).

## `on_button_clicked` — the click handler

```c
static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Button clicked!\n");
}
```

A callback matching the shape GTK expects for a button's `"clicked"` signal: the button that was clicked, plus generic `user_data` (unused here). All it does is print a line to the terminal via `g_print`. See [Signals and Callbacks](../gtk/03-signals-and-callbacks.md) for the full mechanics of how this gets invoked.

## `activate` — building the UI

```c
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
```

Declares two pointer variables that will hold the window and button once created — nothing exists yet at this point, these are just placeholders. See [Pointers and Memory](../c-language/02-pointers-and-memory.md).

```c
    window = gtk_application_window_new(app);
```

Creates a new top-level window belonging to `app`, and stores a pointer to it in `window`.

```c
    gtk_window_set_title(GTK_WINDOW(window), "My First App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
```

Sets the window's title bar text and its starting size (400 pixels wide, 300 tall — the user can still resize it afterward; this is just the initial size). `GTK_WINDOW(window)` casts the generic `GtkWidget *` to specifically `GtkWindow *`, which is required because `gtk_window_set_title`/`gtk_window_set_default_size` are functions that only operate on windows, not on arbitrary widgets.

```c
    button = gtk_button_new_with_label("Click Me");
```

Creates a new button widget with the visible text "Click Me", and stores a pointer to it in `button`.

```c
    gtk_window_set_child(GTK_WINDOW(window), button);
```

Places the button inside the window as its single child widget — this is what makes the button actually show up when the window is displayed. Right now the window can only hold one direct child; more complex layouts (multiple buttons, labels, etc.) require a container widget (like a box or grid) as that one child, which then holds several widgets inside itself.

```c
    g_signal_connect(
        button,
        "clicked",
        G_CALLBACK(on_button_clicked),
        NULL
    );
```

Wires the button's `"clicked"` signal to our `on_button_clicked` function, so clicking it prints to the terminal. See [Signals and Callbacks](../gtk/03-signals-and-callbacks.md).

```c
    gtk_window_present(GTK_WINDOW(window));
}
```

Actually shows the window on screen (makes it visible and focused). Everything above this builds the window and button *in memory*; this line is what makes them appear to the user.

## `main` — wiring the application together

```c
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "com.example.FirstApp",
        G_APPLICATION_DEFAULT_FLAGS
    );
```

Creates the `GtkApplication` object representing this whole program, identified by the ID `"com.example.FirstApp"`. See [The Application Lifecycle](../gtk/02-application-lifecycle.md).

```c
    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );
```

Registers `activate` (defined above) to run once the app is ready to present its UI.

```c
    status = g_application_run(
        G_APPLICATION(app), argc, argv
    );
```

Starts the event loop. This is the call that actually triggers `"activate"` and then keeps the program alive, responding to clicks, until the window is closed. `G_APPLICATION(app)` casts `app` to the generic `GApplication *` type this function expects (recall `GtkApplication` is GTK's specific flavor of GLib's more general `GApplication`).

```c
    g_object_unref(app);

    return status;
}
```

Releases GTK's reference to `app` now that the event loop has ended, and returns the exit status to the operating system.

## The big picture, in one sentence

`main` builds a `GtkApplication`, tells it to run `activate` once it's ready, and `activate` builds a window containing a button that prints a message when clicked — and the whole thing stays alive and responsive because `g_application_run` keeps the event loop spinning in the background until the window is closed.

Next, let's see how this modern style compares to the older GTK 3 approach, and look at the heavily-commented `testGtkApp.c`: [The Scaffolding Files Explained](02-scaffolding-files-explained.md).
