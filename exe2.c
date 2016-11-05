#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>


char prompt[5]={":v_\0"};
char buf[1024]={'\0'};
int salidatoarchivo=0;//servira para saber cuando desviar la salida estandar a un archivo
void cambiaprompt1(int g)
{
	strcpy(prompt,"/_< \0");
}
void cambiaprompt2(int g)
{
	strcpy(prompt,":D \0");
}
void cambiaprompt3(int g)
{
	strcpy(prompt,"3:) \0");
}
void cambiapromptOriginal(int g)
{
	strcpy(prompt,":v \0");
}
void vaciarbuf(char* buf)
{
	int i=0;
	for(i=0;i<1024;i++)
	{
		buf[i]='\0';
	}
}
void *toarchivo(void* arg)
{
	time_t tiempo;
	char cad[80]={'\0'};
	struct tm *tmptr;
	tiempo=time(NULL);
	tmptr=localtime(&tiempo);
	strftime(cad,80,"%H:%M.%S, %A de %B de %Y",tmptr);
	int fichero;
	fichero=open("Historial.txt",O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	write(fichero,cad,sizeof(cad));
	write(fichero,"  ",2);
	write(fichero,buf,sizeof(buf));
	write(fichero,"\n",2);
	close(fichero);
}

int main()
{       int mite=0,contadordeargumentos=0,valor=1,comandos=0,i=0;
	pthread_t mi_hilo;
	char auxbuf[1023]={'\0'};
	char *args[64];
	char *args2[64];
	char *comm[64];
	fflush(stdin);
	printf("%d\n",getpid());
	for (;;) 
	{
		/*
		* Pide y lee un comando.
		*/
		vaciarbuf(buf);
		signal(10,cambiaprompt1);
		signal(12,cambiaprompt2);
		signal(14,cambiaprompt3);
		signal(16,cambiapromptOriginal);
		printf(prompt);
		buf[0]=getchar();
		buf[1]='\0';
		if(buf[0]=='\n')
		{
			printf("\n");
			fflush(stdin);
			continue;
		}else
		{
			scanf("%[^\n]",auxbuf);
			fflush(stdin);
			strcat(buf,auxbuf);	
			if(pthread_create(&mi_hilo,NULL,toarchivo,NULL))
			{
				printf("\n\t error al gardar en archivo:escritura");
				abort();
			}
			if(pthread_join(mi_hilo,NULL))
			{
				printf("\n\t error al guardar en archivo:espera");
				abort();
			}
			
			/*
			* dividir la cadena en argumentos.
			*/
                        
			contadordeargumentos=divide(buf, args);
               
		
			/*
			* Ejecutar el comando.
			*/
		        



           		while (args[i] != NULL){


			if(strcmp(args[i],"+")==0 && strcmp(args[i+1],"mite")==0 ){ // no queda :(
                               args2[0]=args[i+2];
				ejecutarpipe(args);
				ejecutarmite(args2);
					main();
							 }



			else if (strcmp(args[i],"+") == 0){  // compara caracter + para ejecutar pipe
				ejecutarpipe(args);
					main ();}
		
						

                          else{
		
				ejecutar(args);
			
			     }

                        					
			




			

			i++;
			}

			
          


             
                        

			
		}
	}
}

/*
* parse--divide el comando que esta en buf
*        en argumentos. 
*/
int divide(char *buf, char **args)
{       int argu=0;
	while (*buf !='\0') 
	{
		/*
		* Quitar blancos. Usar nulos, para que
		* el argumento previo sea terminado 
		* automaticamente.
		*/
		while ( (*buf ==' ') || (*buf =='\t') ){
			*buf++ ='\0';argu++;
	}
		/*
		* Guardar los argumentos 
		*/
		*args++ = buf;
	
		/*
		* Brincar sobre los argumentos. 
		*/
		while ((*buf !='\0') && (*buf !=' ') && (*buf !='\t'))
			buf++;
	}
	
	*args ='\0';
	 return argu;
}        


/*
* ejecutar--genera un proceso hijo y ejecuta
*           el programa.
*/
int ejecutar(char **args)
{
	int pid, status,valor=1;


         if(strcmp(*args,"mite")==0)
			return;
	/*
	* Obtener un proceso hijo.
	*/
	if ( (pid = fork()) < 0 ) 
	{
		perror("fork");
		exit(1);

		/* NOTA: perror() genera un mensaje de error breve en la 
		* salida de errores describiendo el ultimo error encontrado
		* durante una llamada al sistema o funcion de la biblioteca.
		*/
	}

	/*
	* El proceso hijo ejecuta el codigo dentro del if.
	*/
	if (pid == 0) 
	{
		valor=execvp(*args, args);
		perror(*args);
			return valor;
		exit(1);

	/* NOTA: las versiones execv() y execvp() de execl() son utiles cuando
	   el numero de argumentos es desconocido previamente.
		Los argumentos para execv() y execvp() son el nombre del archivo que
		sera ejecutado y un vector de cadenas que contienen los argumentos.
		El ultimo argumento de cadema debera ser un apuntador a 0 (NULL)
		execlp() y execvp() son llamados con los mismos argumentos que 
		execl() y execv(), pero duplican las acciones del shell en
		la busqueda de un archivo ejecutable en un lista de directorios.
		La lista de directorios es obtenida del ambiente.
	*/

	}

	/*
	* El padre ejecuta el wait.
	*/
	if(pid!=0)
	{
		waitpid(pid,&status,'\0');
	
		/* vacio */ ;
	}
}


ejecutarmite(char **args)
{
	int pid, status;
	FILE* archivo;
	char buffer[512];
	int pe[2],ps[2],numbyte;

	/*
	* Obtener un proceso hijo.
	*/
	if ( (pid = fork()) < 0 ) 
	{
		perror("fork");
		exit(1);
        }
	/*
	* El proceso hijo ejecuta el codigo dentro del if.
	*/
	if (pid == 0) 
	{
		       if((archivo=fopen(*args,"w"))!=NULL)
		      {
			pipe(pe);
			pipe(ps);
			dup2(0,pe[0]);//la entrada de la tuberia apunta a la entrada estandar
			close(pe[1]);
			dup2(1,ps[1]);//la salida de la tuberia apunta a la salida estandar
			close(ps[0]);
			while((numbyte=read(pe[0],buffer,512))>0)
			{
				fwrite(buffer,numbyte,1,archivo);
				write(ps[1],buffer,numbyte);
			}		
		      }
		  close(ps[1]);
	          close(pe[0]);
                  fclose(archivo);

             
		exit(1);


	}
        

	if(pid!=0)
	{
	
		waitpid(pid,&status,'\0');

	
		/* vacio */ ;
	}
}



 void ejecutarpipe(char * args[]){
	// File descriptors
	int filedes[2]; // pos. 0 output, pos. 1 input of the pipe
	int filedes2[2];
	
	int num_cmds = 0;
	
	char *command[256];
	
	pid_t pid;
	
	int err = -1;
	int end = 0;
	
	// Variables used for the different loops
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	// First we calculate the number of commands (they are separated
	// by '|')
	while (args[l] != NULL){
		if (strcmp(args[l],"+") == 0){
			num_cmds++;
		}
		l++;
	}
	num_cmds++;
	
	// Main loop of this method. For each command between '|', the
	// pipes will be configured and standard input and/or output will
	// be replaced. Then it will be executed
	while (args[j] != NULL && end != 1){
		k = 0;
		// We use an auxiliary array of pointers to store the command
		// that will be executed on each iteration
		while (strcmp(args[j],"+") != 0){
			command[k] = args[j];
			j++;	
			if (args[j] == NULL){
				// 'end' variable used to keep the program from entering
				// again in the loop when no more arguments are found
				end = 1;
				k++;
				break;
			}
			k++;
		}
		// Last position of the command will be NULL to indicate that
		// it is its end when we pass it to the exec function
		command[k] = NULL;
		j++;		
		
		// Depending on whether we are in an iteration or another, we
		// will set different descriptors for the pipes inputs and
		// output. This way, a pipe will be shared between each two
		// iterations, enabling us to connect the inputs and outputs of
		// the two different commands.
		if (i % 2 != 0){
			pipe(filedes); // for odd i
		}else{
			pipe(filedes2); // for even i
		}
		
		pid=fork();
		
		if(pid==-1){			
			if (i != num_cmds - 1){
				if (i % 2 != 0){
					close(filedes[1]); // for odd i
				}else{
					close(filedes2[1]); // for even i
				} 
			}			
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0){
			// If we are in the first command
			if (i == 0){
				dup2(filedes2[1], STDOUT_FILENO);
			}
			// If we are in the last command, depending on whether it
			// is placed in an odd or even position, we will replace
			// the standard input for one pipe or another. The standard
			// output will be untouched because we want to see the 
			// output in the terminal
			else if (i == num_cmds - 1){
				if (num_cmds % 2 != 0){ // for odd number of commands
					dup2(filedes[0],STDIN_FILENO);
				}else{ // for even number of commands
					dup2(filedes2[0],STDIN_FILENO);
				}
			// If we are in a command that is in the middle, we will
			// have to use two pipes, one for input and another for
			// output. The position is also important in order to choose
			// which file descriptor corresponds to each input/output
			}else{ // for odd i
				if (i % 2 != 0){
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}else{ // for even i
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}
			
			if (execvp(command[0],command)==err){
				kill(getpid(),SIGTERM);
		
			}		
		}
				
		// CLOSING DESCRIPTORS ON PARENT
		if (i == 0){
			close(filedes2[1]);
		}
		else if (i == num_cmds - 1){
			if (num_cmds % 2 != 0){					
				close(filedes[0]);
			}else{					
				close(filedes2[0]);
			}
		}else{
			if (i % 2 != 0){					
				close(filedes2[0]);
				close(filedes[1]);
			}else{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}
				
		waitpid(pid,NULL,0);
				
		i++;	
	}
} 
	


	






























