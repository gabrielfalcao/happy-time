all:
	gcc -Wall -Werror `pkg-config --libs --cflags gobject-2.0 gio-2.0` main.c -o FileEnumerator

test: all
	./FileEnumerator

clean:
	rm -f FileEnumerator
