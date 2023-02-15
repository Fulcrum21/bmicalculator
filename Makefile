CC=clang
WINCC=x86_64-w64-mingw32-gcc
BIN=bmi
SRC=main.c
LIBS=$(shell pkg-config --libs gtk4)
CPPFLAGS=$(shell pkg-config --cflags gtk4)
CFLAGS=-g -Wall 
default: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIBS)

win: CC = $(WINCC)
win: clean
win: LIBS=$(shell x86_64-w64-mingw32-pkg-config --libs gtk4)
win: CPPFLAGS=$(shell x86_64-w64-mingw32-pkg-config --cflags gtk4)
win: $(BIN)

clean:
	rm -rf bmi*
