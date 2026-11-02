# `.gitignore` Explained

*Previous: [Compiling Manually](../build-system/02-compiling-manually.md) · Back to: [Documentation Index](../README.md)*

## What Git is, briefly

Git is a **version control system** — it tracks the history of changes to your files over time, lets you go back to earlier versions, and (via a remote like GitHub) lets you share and sync that history with others. "Cloning" this repo, as covered when this project was first set up, downloaded the full history, not just the current files.

## What `.gitignore` does

Not every file in a project folder should be tracked by Git. Build output, temporary files, personal notes, editor-specific junk — these clutter the project's history and can even cause conflicts between collaborators working on different machines. A `.gitignore` file lists **patterns for files Git should simply never track**, even if they exist in the folder. Files matching these patterns won't show up in `git status`, won't get added by `git add -A`, and won't be committed.

This project's `.gitignore` currently contains:

```
*.txt
*.patch
[^.]*
```

Let's go through each line — these use **glob patterns**, a simple wildcard syntax (not full regular expressions).

### `*.txt`

The `*` matches any sequence of characters. So `*.txt` matches any file ending in `.txt` — `notes.txt`, `todo.txt`, anything. This is a common way to keep scratch notes out of the repository's history.

### `*.patch`

Same idea — ignores any file ending in `.patch`. `.patch` files are a standard way to represent a set of changes to apply to a codebase (you'll actually receive one of these as the output of the documentation update this file is part of!). Ignoring them here means patch files generated locally while working in this repo won't accidentally get committed into the project's own history.

### `[^.]*`

This one is denser. In glob syntax, `[...]` matches any single character from the set inside the brackets, and `^` as the first character inside `[...]` means **negation** — "any character *not* in this set." So `[^.]` means "any single character that is not a literal dot." Combined with the trailing `*` ("any sequence of characters after that"), the whole pattern `[^.]*` matches **any filename that does not start with a dot**.

That's an unusually broad pattern — taken completely literally, it would tell Git to ignore almost every ordinary file in the project (`README.md`, `app.c`, the `Makefile`, all of them don't start with a dot), while files that *do* start with a dot (`.gitignore` itself, a hidden `.git/` folder) would be the exception, not matched by this pattern, and so not ignored by this particular line.

In practice, this project's actual tracked files (`README.md`, `src/app.c`, etc.) **are** present in the repository's history, which tells us Git isn't treating this line as blocking everything — most likely because those files were already tracked by Git *before* this `.gitignore` rule was added. `.gitignore` only prevents Git from automatically picking up **new, previously-untracked** files that match a pattern; it has no effect on files Git is already tracking. This is a useful, important nuance of how `.gitignore` works in general: it governs what gets *added* going forward, not what's already been committed.

If this line was meant more narrowly (e.g., "ignore stray single-word files with no extension"), it's worth revisiting — as written, it's a very broad catch-all that happens to not cause visible problems only because the important project files were tracked first.

## Practical takeaways

- `.gitignore` prevents clutter and accidents (like committing build artifacts or scratch notes) — it's a tool for keeping a repository's history clean and focused on source code.
- Patterns are matched against filenames using glob syntax: `*` for "any characters," `[...]` for "one character from this set," `[^...]` for "one character *not* in this set."
- `.gitignore` only affects **untracked** files going forward — it can't "hide" or remove files Git is already tracking. To stop tracking an already-committed file, you'd need a separate command (`git rm --cached <file>`), not just adding it to `.gitignore`.
- This repo's `*.patch` pattern is a good practical example: it keeps `.patch` files you generate while working (diffs, proposed changes) out of the committed history of the project itself.

This wraps up the documentation set. If you've read through from [What Is Linux?](../getting-started/01-what-is-linux.md) to here, you now have a working mental model of: what Linux and a native app are, enough C to read this project's source, what GTK is and how its event-driven model works, exactly what `app.c` and the scaffolding files do, how the Makefile builds them, and how this project's version control is set up. From here, the best next step is experimentation — try changing the button's label, adding a second button, or fixing the GTK 3/4 mismatch in the scaffolding Makefile, and use these docs as a reference as you go.

*Back to: [Documentation Index](../README.md)*
