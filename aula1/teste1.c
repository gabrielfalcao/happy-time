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
#include "teste1.h"
struct pessoa
{
  int id;
  char nome[256];
};

struct pessoa gabriel = {1,"Gabriel"};

int a()
{
  printf("A FUNCAO a() foi executada\n");
  return 0;
}

int echo(char *s)
{
  
  char *buffer;
  struct pessoa *ptr_gab;
  ptr_gab = &gabriel;
  void *pot;
  buffer = malloc(strlen(s)+1);
  
  pot = (void*)(buffer);
  puts("|+++++++++++++++++++++++++++++++++++++++|\n");
  puts("|      agora vou imprimir a string s    |\n");
  puts("|+++++++++++++++++++++++++++++++++++++++|\n");
  puts(s);
  puts("\n");
  
  puts("|====================8===================|\n");
  puts("| agora vou imprimir a cópia da string s|\n");
  puts("|=======================================|\n");
  strcpy(buffer,s);
  puts(buffer);
  strcpy (ptr_gab->nome, (char*)pot);
  ptr_gab->nome[0] = *(char*)pot;
  * (int*) pot = 0x41414141;
  //printf("\nVeja o nome: %s\n",(*ptr_gab).nome);
  printf("\nVeja o nome: %s\n",ptr_gab->nome);
  puts(buffer);
  //free(ptr_gab);
  free(buffer);

  return 0;
//  4002-1244
}

int main()
{
  system("clear"); //hammer clear screen!
  printf("OI %s\n", gabriel.nome);
  a();
  echo("Joaozim");
  return 0;
}
