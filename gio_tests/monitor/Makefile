all:
	gcc -Wall -Werror `pkg-config --libs --cflags gobject-2.0 gio-2.0` gf-monitor.c main.c -o Monitor

test: all
	./Monitor

clean:
	rm -f Monitor
