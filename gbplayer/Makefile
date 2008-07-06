all: main

main: main.c
	gcc -DIMG_PATH="\"`pwd`/imgs\"" -g -O0 -o GbPlayer gb-player-time.c gb-player.c main.c `pkg-config --libs --cflags gdk-pixbuf-2.0 gtk+-2.0 gstreamer-0.10 clutter-0.6 clutter-gtk-0.6 clutter-gst-0.6 clutter-cairo-0.6 cairo`

test: main
	./GbPlayer
