# Installing the Tools

*Previous: [What Is Linux?](01-what-is-linux.md) · Next: [A C Crash Course](../c-language/01-c-crash-course.md)*

To build anything in this repo you need four things installed on your Linux machine:

1. **A C compiler** — turns your `.c` source files into a runnable program. We use `gcc` (the GNU Compiler Collection).
2. **`make`** — reads a `Makefile` and runs the compiler for you with the right options. See [Makefiles Explained](../build-system/01-makefiles-explained.md).
3. **`pkg-config`** — a small helper tool that tells the compiler exactly where GTK's files live on your system, so you don't have to hunt for them yourself. You'll see it used inside the Makefile as `` `pkg-config --cflags gtk+-3.0` ``.
4. **The GTK development libraries** — the actual GTK code (headers and compiled library files) that your program will link against. This project's Makefile targets GTK 3 (`gtk+-3.0`); the in-repo `app.c` is written against GTK 4's API (see the note at the bottom of this page).

## Installing on Debian/Ubuntu-based systems

Most beginners are on Ubuntu or something derived from it. Open a terminal and run:

```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev
```

- `sudo` runs the command with administrator privileges — installing software system-wide requires this.
- `build-essential` is a bundle that includes `gcc`, `make`, and other core compiling tools.
- `pkg-config` is the helper tool described above.
- `libgtk-3-dev` is the GTK 3 **development** package — note the `-dev` suffix. Development packages include the header files (`.h`) and linking info needed to *compile against* a library, not just run programs that already use it.

If you need GTK 4 instead (to match `src/app.c` in this repo, which uses GTK 4 functions), install `libgtk-4-dev` instead, and change `gtk+-3.0` to `gtk4` in the relevant Makefile.

## Installing on Fedora/RHEL-based systems

```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install pkgconf-pkg-config gtk3-devel
```

(Swap `gtk3-devel` for `gtk4-devel` for GTK 4.)

## Installing on Arch-based systems

```bash
sudo pacman -S base-devel pkgconf gtk3
```

(Swap `gtk3` for `gtk4` for GTK 4.)

## Checking your installation

After installing, verify everything is in place:

```bash
gcc --version
make --version
pkg-config --modversion gtk+-3.0
```

Each should print a version number rather than a "command not found" error. If `pkg-config --modversion gtk+-3.0` fails, GTK 3's development files aren't installed (or aren't on `pkg-config`'s search path).

## A note on GTK 3 vs GTK 4 in this repo

You'll notice something slightly inconsistent as you explore this project: the scaffolding `Makefile` and `test.c` target **GTK 3** (`gtk_init`/`gtk_main`, `gtk+-3.0`), while `src/app.c` and `testGtkApp.c` are written using the **GTK 4** style (`GtkApplication`, `gtk_window_present`). This is explained fully in [The Scaffolding Files Explained](../code-walkthrough/02-scaffolding-files-explained.md) — short version: GTK went through a real API redesign between versions 3 and 4, and this project currently has one foot in each. To build `app.c` or `testGtkApp.c` you need the GTK 4 dev package and a Makefile whose `pkg-config` calls say `gtk4`, not `gtk+-3.0`.

Next: [A C Crash Course](../c-language/01-c-crash-course.md), where we start reading actual code.
