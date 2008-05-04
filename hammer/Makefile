all:
	make pyo
	gcc -shared -g -Wall -lpython2.5 spammodule.o -o spam.so

pyo:
	gcc -g -Wall -I. -I/usr/include/python2.5 -c spammodule.c -o spammodule.o

clean:
	rm -rf *.so *.o
