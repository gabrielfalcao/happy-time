# Copyright (C) 2008 Gabriel Falcão <gabriel@nacaolivre.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
#Compilando tudo:
all:
	gcc -c libprint.c
	gcc -c main.c
	gcc libprint.o main.o -o Teste1
# Se o arquivo compilado nao existir, eu chamo a diretiva que compila e executa,
# se ja existir, eu só executo
test:
	if [ -f Teste1 ]; then \
		./Teste1; \
	else \
		make test-and-compile; \
	fi;
# diretiva que compila e testa
test-and-compile: all test	

#diretiva para remover os arquivos compilados
clean:
	rm -f *.o Teste1

#limpa depois cria num diretorio anterior, um tarball com a pasta atual, com a data atual
dist: clean
	tar -cjvf ../`pwd | sed 's/.*[/]\(.*\)/\1/'`_`date +%a[%d-%m-%Y]`.tar.bz2 ../`pwd | sed 's/.*[/]\(.*\)/\1/'`
