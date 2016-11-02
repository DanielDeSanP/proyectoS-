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



char prompt[5]={":v \0"};
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
{       int mite=0,contadordeargumentos=0;
	pthread_t mi_hilo;
	char auxbuf[1023]={'\0'};
	char *args[64];
	char *args2[64];
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
                        
			contadordeargumentos=parse(buf, args);
                       
                     
                        for(int i=0;i<=contadordeargumentos;i++)// viendo si hay "mite"(tee)
                        { 
                           if(strcmp(args[i],"mite")==0){
                               args2[0]=args[i+1];
				ejecutarmite(args2);           //si existe, va a la funcion ejecutarmite
            
                                                        }  
                        }
               

			/*
			* Ejecutar el comando.
			*/
		        ejecutar(args);
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
ejecutar(char **args)
{
	int pid, status;

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
		execvp(*args, args);
		perror(*args);
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
	}


	if(pid!=0)
	{
		           
		waitpid(pid,&status,'\0');
	
		/* vacio */ ;
	}
}



  
	















