all:hcairo lib
	gcc -g libsvg2pdf.c test.c `pkg-config --cflags --libs glib-2.0 gio-2.0 cairo librsvg-2.0` -o TestSVG2PDF

hcairo:
	gcc hcairo.c `pkg-config --cflags --libs glib-2.0 gio-2.0 cairo librsvg-2.0` -o HCairo

test:clean hcairo
	./HCairo
	evince teste.pdf

lib:
	gcc libsvg2pdf.c `pkg-config --cflags --libs glib-2.0 gio-2.0 cairo librsvg-2.0` -shared -fPIC -o libsvg2pdf.so

clean:
	rm -f *.pdf HCairo TestSVG2PDF libsvg2pdf.so
