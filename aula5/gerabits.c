/*
 * Copyright (C) 2007 Gabriel Falcão <root@gabrielfalcao.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerabits.h"

void
bitmaker (char *buffer, int size)
{
  int x;
  char *pre_buf;
  srand(size);
  if ((pre_buf = malloc (size * sizeof (char))) != NULL)
    {
      buffer = pre_buf;

      for (x = 0; x < size; x++)
        {
          buffer[x] = (char) rand () * x + (rand () * x);
        }

    }
}
int
main ()
{

  int test;
  test = rand () * rand ();
  char *buff;
  bitmaker (buff, 96*96);
  printf ("%s\n", buff);
  printf ("%d\n", strlen (buff));
  return 0;
}
