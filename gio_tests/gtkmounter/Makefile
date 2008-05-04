all:
	gcc -Wall -Werror `pkg-config --libs --cflags gtk+-2.0 gio-2.0` ui.c main.c -o Ui
test: all
	./Ui
