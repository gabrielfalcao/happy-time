/**
 * Copyright (C) 2008 Gabriel Falcão <gabriel@nacaolivre.org>
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
void muda_n(int *z){
  if (z != NULL){
    *z = 450;
  }
}

int
main (void)
{
  int n;
  int *p;
  n = 10;
  p = &n;
  printf ("Endereço HEXA: %x\n", p);
  printf ("Endereço tosco: %d\n", p);
  printf ("Imprimindo N diretamente: %d\n", n);
  printf ("Efetivamente, o conteudo: %d\n", *p);

  n = 500;
  printf ("----------\nEfetivamente, o conteudo: %d\n", *p);
  printf ("O endereço do ponteiro é %x\n", &p);

  /*isso aqui é o mesmo de n=450*/
  muda_n(&n);
  printf ("----------\nEfetivamente, o conteudo: %d\n", *p);
  printf ("O endereço do ponteiro é %x\n", &p);
  /* Provocando erro: */
  muda_n(NULL);
  return 0;
}
