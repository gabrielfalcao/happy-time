all:multi-page

multi-page:
	gcc multi-page.c `pkg-config --cflags --libs glib-2.0 cairo librsvg-2.0` -o Multi-Page

test:multi-page
	./Multi-Pageb
