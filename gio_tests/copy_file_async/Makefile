all:
	gcc -g -Wall -Werror `pkg-config --libs --cflags gthread-2.0 gobject-2.0 gio-2.0 gtk+-2.0` main.c -o CopyAsyncGIO

test: all
	./CopyAsyncGIO

clean:
	rm -f CopyAsyncGIO
