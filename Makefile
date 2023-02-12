CC=clang
WINCC=x86_64-w64-mingw32-gcc
BIN=bmi
LIBS=$(shell pkg-config --libs gtk+-3.0)
CPPFLAGS=$(shell pkg-config --cflags gtk+-3.0)
CFLAGS=-g -Wall $(foreach D, $(INCLUDEDIR), -iquote $(D)) $(CPPFLAGS)

default: $(BIN)

$(BIN): main.o
	$(CC) $(LIBS) -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) -o $@ $^
win: CC = $(WINCC)
win: clean
win: $(BIN)

clean:
	rm -rf main.o bmi*
