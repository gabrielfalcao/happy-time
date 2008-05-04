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
#include "teste2.h"

char *
abrir(char *filename)
{
  FILE *arq;
  char *ret;
  char linha[BUFSIZ];
  size_t tam;
  void *trash;
  arq = fopen(filename, "r");
  ret = malloc(BUFSIZ);
  if (!arq)
  {
    printf("Padre quevedo disse q o arquivo non ecziste!");
    return NULL;
  }
  tam = fread(linha, 1, BUFSIZ, arq);
  do{
      if(tam > sizeof(ret)){
        trash = realloc(ret, BUFSIZ);
        if (trash != NULL){
          ret = (char *) trash;
        }else{
          return NULL;
        }
      }
      strcat(ret, linha);
      /*printf("%s\n", linha);*/
      tam = fread(linha, 1, BUFSIZ, arq);
  }while(tam);
  return ret;
}


int main(int argc, char **argv)
{
  char *t;
  char *filename;
  int pos;
  if (argc>1){
    if (argc == 2){
      //printf("Tamanho: %s\n", **argv);
      filename = strdup(argv[1]);//malloc( strlen(*(argv+2)));
      if (filename != NULL){
          t = abrir(filename);
          if (t)
          {
            puts(t);
            free(t);
          }else{
            puts("DEU MERDA!");
            exit(1);
          }
        free(filename);
        return 0;
      }

    }else{
      for (pos=1;pos<argc;pos++){
        printf("%s\n", *(argv+pos));
      }
    }
  }
  return 0;
}
