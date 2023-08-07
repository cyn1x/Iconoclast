CC=clang
CFLAGS=-g -Wall -Wextra -Werror -pedantic -pedantic-errors -std=c17

SRCDIR=src
OBJDIR=obj
BINDIR=bin
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN=$(BINDIR)/rakija.exe
ZIP=$(BINDIR)/rakija.tar.gz

all: $(BIN)

release: CFLAGS=-Wall -pedantic -O2 -DNDEBUG -std=c17
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BINDIR)/* $(OBJDIR)/*
