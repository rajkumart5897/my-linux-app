# Pointers and Memory

*Previous: [A C Crash Course](01-c-crash-course.md) · Next: [What Is GTK?](../gtk/01-what-is-gtk.md)*

If you look through this repo's C files, you'll see `*` and `&` everywhere: `GtkWidget *window`, `char **argv`, `&argc`. This doc explains what they actually mean, because GTK's entire programming style leans on pointers.

## What is a pointer, really?

When your program creates a value — a window, a number, anything — that value gets stored somewhere in your computer's memory, at a particular numeric **address** (think of memory as a huge street of numbered mailboxes).

A **pointer** is a variable that doesn't hold the value itself — it holds the *address* where the value lives. That's the entire concept. Everything else is syntax for creating and using that address.

```c
int count = 5;        // a normal variable holding the value 5
int *ptr = &count;     // ptr holds the ADDRESS of count, not the value 5
```

- `&count` means "give me the address of `count`" (`&` = "address of").
- `int *ptr` declares `ptr` as a variable that stores an address *to an int*.
- To get back from the address to the actual value, you **dereference** with `*`: `*ptr` means "go to the address `ptr` holds, and give me the value sitting there" — in this example, `*ptr` would be `5`.

## Why bother? Why not just use the value directly?

Two big reasons, both of which show up directly in this project's code:

**1. Avoiding copies of large things.** If a function takes a huge piece of data as a parameter, passing it by value means the computer copies the *entire thing* just to hand it to the function. Passing a pointer instead just copies a small address — cheap, regardless of how big the real thing is.

**2. Letting a function change something that lives outside it.** Normally, a C function only gets *copies* of its arguments, so changes it makes don't affect the caller's original variables. If a function receives a pointer, though, it can dereference it and modify the real value at that address — which is how the original value "outside" the function gets updated.

## Pointers in this project's code

```c
GtkWidget *window;
```

`window` isn't a window — it's a pointer that will hold the memory address *of* a window object once one is created. Almost every GTK object (`GtkWidget`, `GtkApplication`, `GtkButton`) is handled this way: you work with pointers to them, never the raw objects directly. This matters because GTK manages these objects internally (allocating and freeing their memory for you); your code mostly just holds references (pointers) to them and tells GTK what to do with them.

```c
window = gtk_application_window_new(app);
```

`gtk_application_window_new` creates a new window somewhere in memory and returns a pointer to it, which gets stored in `window`.

```c
gtk_window_set_title(GTK_WINDOW(window), "My First App");
```

We pass `window` (the pointer) into this function so it knows *which* window object to modify. `GTK_WINDOW(window)` is a cast — more on that below.

## `char **argv` — a pointer to a pointer

```c
int main(int argc, char **argv)
```

`argv` is an array of C strings (command-line arguments). A C string is itself represented as `char *` (a pointer to the first character of a sequence of characters in memory). So "an array of strings" becomes "a pointer to a pointer to characters" — `char **argv`. You don't need to manipulate this yourself in this project; it's just passed straight through to `g_application_run`.

## Casts like `GTK_WINDOW(...)`

```c
GTK_WINDOW(window)
```

This looks like a function call, but it's actually a macro that performs a **type cast** — it tells the compiler "treat this generic `GtkWidget` pointer as specifically a `GtkWindow` pointer." GTK represents many different kinds of UI elements (windows, buttons, labels) using a shared base type, `GtkWidget`, and uses these `GTK_X(...)` macros to safely narrow that back down to a specific type when you need to call a function that only makes sense for, say, windows (`gtk_window_set_title`) rather than any generic widget.

## `gpointer` and `NULL`

```c
static void activate(GtkApplication *app, gpointer user_data)
```

`gpointer` is GTK's own name for a generic pointer (equivalent to C's built-in `void *`) — "a pointer to something, type unspecified." It's commonly used for the `user_data` parameter in callback functions, so you can pass any extra information you want through to your callback, of any type, without GTK needing to know what it is in advance.

```c
g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
```

`NULL` is a special pointer value meaning "points to nothing." Here it means "I'm not passing any extra `user_data` into this callback." If `on_button_clicked` tried to dereference `user_data` without checking it first, that would be a bug — dereferencing `NULL` crashes the program, since there's no real memory address to go to.

## The short version

| Syntax | Meaning |
|---|---|
| `Type *name` | Declares `name` as a pointer to a value of `Type` |
| `&value` | "Give me the address of `value`" |
| `*ptr` | "Give me the value at the address `ptr` holds" (dereference) |
| `GTK_WINDOW(x)` | Cast: "treat `x` as a `GtkWindow` pointer" |
| `gpointer` / `void *` | A pointer with no specific type attached |
| `NULL` | A pointer that points to nothing |

With pointers out of the way, you're ready for the actual UI library: [What Is GTK?](../gtk/01-what-is-gtk.md)
