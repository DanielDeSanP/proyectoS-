#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{

char comando,parametros;
int status;

while(1){

  scanf("%c",&comando);//lee entrada de la terminal
  if(fork()!=0){
 
     waitpid(-1,&status,0);//espera la finalizacion del hijo
     printf("%c: no se encontro la orden\n",comando);
     exit(1);
  }
 
  else{
     
     switch(comando){

     case 'l':
     execl("/bin/ls","ls","-l",(char*)0);//ejecuta el comando 
     exit(0);

    default:
     return 0;
     break;}
    }
}

exit(0);

}
       



