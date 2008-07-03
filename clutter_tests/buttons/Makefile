
all: button

button: button.c
	gcc -g -o button button.c `pkg-config --libs --cflags gdk-pixbuf-2.0 gtk+-2.0 gstreamer-0.10 clutter-0.6 clutter-gtk-0.6 clutter-gst-0.6`

test: button
	./button
