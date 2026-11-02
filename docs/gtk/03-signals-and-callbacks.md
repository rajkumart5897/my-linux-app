# Signals and Callbacks

*Previous: [The Application Lifecycle](02-application-lifecycle.md) · Next: [`app.c` Explained Line by Line](../code-walkthrough/01-app-c-explained.md)*

This is the mechanism behind every bit of interactivity in a GTK app: how does your code find out that the user clicked a button?

## The core idea: "call me when X happens"

GTK (via GLib/GObject) uses a **signal** system. A GTK object — a button, a window, the application itself — can **emit** signals at specific moments: a button emits `"clicked"` when it's clicked; the application emits `"activate"` when it's ready to show UI. Your code doesn't poll ("check every second: was the button clicked yet?") — instead, you **register a function to be called automatically** when a particular signal fires. That registered function is called a **callback** (or "signal handler").

This inversion — "don't call us, we'll call you" — is a very common pattern in event-driven programming, and it's why GTK code is structured the way it is: you mostly spend your time writing small functions and then wiring them up to specific signals, rather than writing one long top-to-bottom script.

## `g_signal_connect` — the wiring

```c
g_signal_connect(
    button,
    "clicked",
    G_CALLBACK(on_button_clicked),
    NULL
);
```

Four pieces:

1. **`button`** — the object whose signal we're listening to.
2. **`"clicked"`** — the name of the signal, as a string. Every widget type documents which signals it can emit (buttons emit `"clicked"`; applications emit `"activate"`; windows can emit things like `"close-request"`).
3. **`G_CALLBACK(on_button_clicked)`** — the function to call when that signal fires. `G_CALLBACK(...)` is a cast macro, similar in spirit to `GTK_WINDOW(...)` from the pointers doc — it tells the compiler to treat your function pointer as the generic callback type GTK's signal system expects internally.
4. **`NULL`** — the "user data" to pass through to the callback as its last argument (see below). `NULL` here means "nothing extra to pass."

This line does **not** call `on_button_clicked` right away — it just registers it. The function only actually runs later, whenever (and if) the `"clicked"` signal is emitted — which happens when the user clicks that specific button, at some unknown future point while the event loop is running.

## The callback function itself

```c
static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Button clicked!\n");
}
```

Notice its signature matches what GTK expects for a `"clicked"` handler: it receives the specific widget that emitted the signal (`GtkButton *button` — useful if the same callback is reused for multiple buttons and needs to tell them apart) and a generic `user_data` pointer (whatever you passed as the last argument to `g_signal_connect` — `NULL` in this case, so it's unused here).

`g_print` is GLib's version of C's standard `printf` — it writes text to the terminal. `\n` is a newline character. So clicking the button in this project just prints `Button clicked!` to whatever terminal you launched the app from — nothing shows up in the window itself.

Different signals pass different arguments to their callbacks (a `"clicked"` handler's shape differs from an `"activate"` handler's shape), which is why `activate`'s signature looks different:

```c
static void activate(GtkApplication *app, gpointer user_data)
```

This matches what `GApplication`'s `"activate"` signal hands back: the application object itself, plus `user_data`.

## Why the exact function signature matters

GTK's signal system is implemented generically in C (which doesn't have the built-in safety nets of some newer languages), so it relies on you providing a callback function whose parameter types and count match what that particular signal expects, and on `G_CALLBACK(...)` to paper over the type differences at the call site. If your callback's signature doesn't match, you'll get subtle bugs or crashes rather than a clean error — so when writing your own signal handlers, it's worth checking GTK's documentation for the exact signature a given signal expects.

## Putting it together: the full chain of events

1. Program starts → `main()` runs → `g_application_run()` is called → this blocks and triggers the `"activate"` signal.
2. `"activate"` fires → your registered `activate` function runs → it builds the window and button, and connects `"clicked"` → `on_button_clicked`.
3. `activate` finishes, but the program doesn't exit — `g_application_run()` is still blocking, inside the event loop, waiting for the next event.
4. User clicks the button → GTK detects it → emits `"clicked"` on that button → your registered `on_button_clicked` function runs → prints to the terminal.
5. Event loop goes back to waiting.
6. User closes the window → GTK winds the application down → `g_application_run()` finally returns → `main()` finishes cleanup and exits.

Now that you understand pointers, the GTK object model, the application lifecycle, and signals, you have everything needed to read this project's actual code line by line: [`app.c` Explained Line by Line](../code-walkthrough/01-app-c-explained.md).
