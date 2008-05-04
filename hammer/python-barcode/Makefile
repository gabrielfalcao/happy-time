all:
	make pyo
	cc -shared -g -Wall -lpython2.5 barcodemodule.o -lbarcode -L.. -o barcode.so

pyo:
	cc -g -Wall -I../../ -I. -I/usr/include/python2.5 -c barcodemodule.c -o barcodemodule.o

clean:
	rm -rf *.so *.o
