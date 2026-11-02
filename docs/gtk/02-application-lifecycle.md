# The Application Lifecycle

*Previous: [What Is GTK?](01-what-is-gtk.md) · Next: [Signals and Callbacks](03-signals-and-callbacks.md)*

Most beginner programs run top to bottom and exit. GUI apps are different: once started, they have to sit and wait indefinitely for the user to do something — move the mouse, click a button, resize the window — and respond each time, without the program "ending" in between. This doc explains how GTK structures that.

## The event loop, conceptually

A GUI program's shape is usually:

```
1. Set things up (create the window, buttons, etc.)
2. Enter a loop that:
     - waits for something to happen (a click, a keypress, a redraw request)
     - runs whatever code is associated with that event
     - goes back to waiting
3. Exit the loop once the user closes the window (or the app quits itself)
4. Clean up and end the program
```

That middle loop is the **event loop** (also called the "main loop"). It's the reason a GUI app doesn't just flash on screen and immediately close — the loop is what keeps it alive and responsive.

## How this looks in `app.c`

```c
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "com.example.FirstApp",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
```

Step by step:

**1. `gtk_application_new(...)`** creates a `GtkApplication` object — this represents your whole running application (as opposed to any one window). The first argument, `"com.example.FirstApp"`, is a reverse-DNS-style **application ID**: a string that uniquely identifies your app to the system (useful for things like preventing two copies of the same app running at once, or letting the desktop environment group your windows). `G_APPLICATION_DEFAULT_FLAGS` just means "use standard behavior, no special options."

**2. `g_signal_connect(app, "activate", G_CALLBACK(activate), NULL)`** registers your `activate` function to run when the application's `"activate"` signal fires. This is covered fully in [Signals and Callbacks](03-signals-and-callbacks.md) — for now, just know: this line doesn't run `activate` immediately. It just says "when the time comes, call this."

**3. `g_application_run(...)`** is the big one — this is what actually starts the event loop. Calling this function:
   - Triggers the `"activate"` signal (which is what actually causes your window to get built and shown, since that's what the `activate` function does).
   - Then **blocks** — meaning the program sits inside this function call, handling events, for as long as the app is open.
   - Only returns once the application is finished (e.g. the user closed the last window), at which point it gives back an integer **exit status**.

**4. `g_object_unref(app)`** releases GTK's internal reference to the `app` object now that we're done with it, so its memory can be freed. GTK/GLib objects are **reference-counted**: internally they track how many places are "using" them, and the object is cleaned up once nothing references it anymore. This is a lightweight, manual form of memory management — not full garbage collection, but automatic enough that you don't need to track memory addresses by hand the way you might in raw C.

**5. `return status;`** passes the exit status back to the operating system, ending the C program itself.

## Why go through `activate` at all, instead of just building the window in `main`?

Because `GtkApplication` can do extra things before your UI is actually shown — like checking if another instance of your app is already running. The `"activate"` signal specifically marks the moment GTK has decided "okay, it's time to actually present UI to the user," so that's the appropriate place to build your window, not directly inside `main`.

## Comparing to the older GTK 3 style

The scaffolding file `test.c` shows the older pattern:

```c
gtk_init(&argc, &argv);
// gtk code comes here.
gtk_main();
```

- `gtk_init(&argc, &argv)` manually initializes GTK (parses GTK's own command-line flags, sets things up). Note the `&` — it's passed as a pointer so GTK can potentially modify `argc`/`argv` if it strips out GTK-specific flags.
- `gtk_main()` is the older, more manual event loop — equivalent in spirit to `g_application_run()`, but without the `GtkApplication` object wrapped around it (no built-in app-ID/single-instance handling, signal is connected to individual widgets directly rather than through an app-level `"activate"`).

Both patterns end up doing the same conceptual job — initialize, loop, wait for events — but `GtkApplication` + `g_application_run` is the modern, recommended approach, which is why `app.c` and `testGtkApp.c` use it.

Next: let's look closely at how your code actually finds out about a click — [Signals and Callbacks](03-signals-and-callbacks.md).
