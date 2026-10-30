# Makefile has it's own language, So it is not a shell script. 
# It is a set of rules to build your project.

# Example of a simple action
# target: prerequisites
#	action

# It supports macro as well.
# PROG = test.c

# A command can be executed only if it is part of .PHONY: cmd-names
# Otherwise Make will assume it's a file.

PROG = testGtkApp

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c $(LIBS)


.PHONY: push

push:
	git add -A
	@read -p "Enter commit message: " msg; \
	git commit -m "$$msg"
	git push