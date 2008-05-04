all:
	gcc -Wall -Werror `pkg-config --libs --cflags gobject-2.0 gio-2.0` main.c -o FileInfo

test: all
	./FileInfo

clean:
	rm -f FileInfo
