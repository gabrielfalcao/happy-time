CFLAGS=`pkg-config --cflags glib-2.0 gio-2.0 cairo librsvg-2.0` -I/usr/include/python2.5/
LIBS=`pkg-config --libs glib-2.0 gio-2.0 cairo librsvg-2.0`

all:pylib

test:clean 
	evince teste.pdf

deps:
	gcc -c libsvg2pdf.c $(CFLAGS) $(LIBS)
	gcc -c pysvg2pdfio.c $(CFLAGS) $(LIBS)

pylib:deps
	gcc pysvg2pdf.c $(CFLAGS) $(LIBS) libsvg2pdf.o -shared -fPIC -o svg2pdf.so

clean:
	rm -f *.pdf *.so *.o
