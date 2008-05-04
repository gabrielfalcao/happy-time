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
all:
	gcc -g -O2 -Wall -Werror -Wextra `pkg-config --libs --cflags glib-2.0 gobject-2.0` sabor.c vegetal.c cenoura.c main.c -o Vegetal

dist: clean
	tar -czvf ../vegetal_gobject_`date +%d-%m-%Y`.tar.gz `pwd`

clean:
	rm -f Vegetal *.o

test: all
	./Vegetal

debug: all
	gdb ./Vegetal
