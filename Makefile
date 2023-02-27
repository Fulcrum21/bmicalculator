CC=clang
BIN=bmi
SRC=main.c
OBJ=main.o
LIBS=$(shell pkg-config --libs gtk4)
CPPFLAGS=$(shell pkg-config --cflags gtk4)
CFLAGS=-g -Wall 
default: $(BIN)

$(BIN): $(OBJ) resources.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^
resources.c: bmicalculator.gresources.xml bmicalculator.ui styles.css
	glib-compile-resources $< --target=$@ --generate-source
clean:
	rm -rf $(OBJ) resources.c bmi
