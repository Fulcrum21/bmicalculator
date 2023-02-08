CC=clang
WINCC=x86_64-w64-mingw32-gcc
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.c)
OBJDIR=obj
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
INCLUDEDIR=include/
BINDIR=bin
BIN=$(BINDIR)/bmi
CFLAGS=-g -Wall $(foreach D, $(INCLUDEDIR), -iquote $(D))

default: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

win: CC = $(WINCC)
win: clean
win: $(BIN)

clean:
	rm -rf obj/* bin/*
