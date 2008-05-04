#include <stdio.h>

int
main(int argc, char **argv){

int i = 0;
int *j;
for (i=0;i<argc;i++){

  printf("%s\n",argv[i]);

}

j = &i;

printf("%d\n",*j);


}

static * new_Pessoa(void inst){

  typedef struct{
  
    char *nome, 
    char *email, 
    int  *idade, 
  
  }Pessoa;



}
