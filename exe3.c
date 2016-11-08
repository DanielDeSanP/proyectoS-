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


char prompt[5]={":v_\0"}; //prompt inicial
char buf[1024]={'\0'};
int salidatoarchivo=0;//servira para saber cuando desviar la salida estandar a un archivo

void limpiarstdin()
{
	while(getchar() != '\n');
}

void cambiaprompt1(int g)          //  ---------------------------------------------------
{
	strcpy(prompt,"/_< \0");    //Funciones que nos daran diferentes caracteres para el prompt
}
void cambiaprompt2(int g)
{
	strcpy(prompt,":D \0");
}
void cambiaprompt3(int g)
{
	strcpy(prompt,"3:) \0");
}
void cambiapromptOriginal(int g) //----------------------------------------------------------------------
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
	tmptr=localtime(&tiempo); //obtenemos el tiempo local para despues usarlo en el archivo historial
	strftime(cad,80,"%H:%M.%S, %A de %B de %Y",tmptr);
	int fichero;
	fichero=open("Historial.txt",O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP); // se abre archivo en donde se guardara el registro de
	write(fichero,cad,sizeof(cad));                                                          //Todo lo que se escribe en pantalla
	write(fichero,"  ",2);
	write(fichero,buf,sizeof(buf));
	write(fichero,"\n",2);
	close(fichero);
}

int main()
{       int mite=0,contadordeargumentos=0,comandos=0,i=0;
	int aux,j=0;
	pthread_t mi_hilo;
	char auxbuf[1023]={'\0'};
	char *args[64];
	char *args2[64];
	char *args_aux[256];
	fflush(stdin);
	printf("pid: %d\n",getpid());
	for (;;) 
	{
		/*
		* Pide y lee un comando.
		*/
		vaciarbuf(buf);
		signal(10,cambiaprompt1);	//señales para cambiar el prompt
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
			//fflush(stdin); <- Esta función es tramposa, luego no limpia bien el buffer, por eso 
			//aparecía dos veces el prompt
			limpiarstdin();
			strcat(buf,auxbuf);	
			if(pthread_create(&mi_hilo,NULL,toarchivo,NULL))
			{
				printf("\n\t error al guardar en archivo:escritura");
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
                        
			contadordeargumentos=divide(buf, args);	//

			
				

			while ( args[j] != NULL){ //ciclo en donde se revisa si existe "<" o ">", de ser asi, salimos del ciclo
			if ( (strcmp(args[j],">") == 0) || (strcmp(args[j],"<") == 0) ){ 
				break;
			}
			args_aux[j] = args[j];
			j++;
			}



			while (args[i] != NULL )
			{

			//Si '>' es detectado, tenemos redirección de salida.		
			if (strcmp(args[i],">") == 0){
				if (args[i+1] == NULL){
					printf("\n");
					
				}
				redireccion(args_aux,NULL,args[i+1],0); //se ejecuta la funcion redireccion para ">"
				main();
			}
			if (strcmp(args[i],"<") == 0){
				aux = i+1;
				if (args[aux] == NULL || args[aux+1] == NULL  ) // se revisa si hay argumentos suficientes
					printf("\n"); 
				
				redireccion(args_aux,args[i+1],args[i+3],1); //se ejecuta la funcion redireccion para "<"
				main();
				 }
			i++;
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
		printf("Proceso hijo no pudo ser creado\n");
		return;
		

	
	}

	/*
	* El proceso hijo ejecuta el codigo dentro del if.
	*/
	if (pid == 0) 
	{	
		valor=execvp(*args, args);
			
		perror(*args);
			
	
		exit(1);

	/*  las versiones execv() y execvp() de execl() son utiles cuando
	   el numero de argumentos es desconocido previamente.
		Los argumentos para execv() y execvp() son el nombre del archivo que
		sera ejecutado y un vector de cadenas que contienen los argumentos.
	
	*/

	}

	/*
	* El padre ejecuta el wait.
	*/
	if(pid!=0)
	{	
		
		 waitpid(pid,&status,'\0'); ejecutarpipe(args);
		
		/* vacio */ ;
	}
}





  ejecutarpipe(char * args[]){
	// Descriptores de archivos
	int filedes[2]; 
	int filedes2[2];
	
	int num_cmds = 0;
	
	char *command[256];
	
	pid_t pid;
	
	int err = -1;
	int end = 0;
	
	// Variables para diferentes loops
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	//calculamos el numero de comandos separados por un '+'
	
	while (args[l] != NULL){
		if (strcmp(args[l],"+") == 0){
			num_cmds++;
		}
		l++;
	}
	num_cmds++;
	
	// 
	//Bucle principal, por cada comando entre '+' los pipes seran configurados y los I/O estandar seran
	//remplazados
	while (args[j] != NULL && end != 1){
		k = 0;


		//Usamos un arrreglo auxiliar para guardar los comandos que seran ejecutados en cada iteracion
		while (strcmp(args[j],"+") != 0){
			command[k] = args[j];
			j++;	
			if (args[j] == NULL){
		
				//con 'end' nos aseguramos de que se termine el bucle cuando no hay mas argumentos
				
				end = 1;
				k++;
				break;
			}
			k++;
		}
		
		command[k] = NULL;
		j++;		
		


		//Dependiendo de si estamos en una iteración u otra,
		//Estableceremos diferentes descriptores para las entradas y
		//salida de los pipes. De esta manera, un pipe será compartido entre cada dos
		//Iteraciones, lo que nos permite conectar las entradas y salidas de
		//Los dos comandos diferentes.



		if (i % 2 != 0){
			pipe(filedes); // para i impar
		}else{
			pipe(filedes2); // para i par
		}
		
		pid=fork();
		
		if(pid<0){			
			if (i != num_cmds - 1){
				if (i % 2 != 0){
					close(filedes[1]); // para i impar
				}else{
					close(filedes2[1]); // para i par
				} 
			}			
			printf("Proceso hijo no pudo ser creado\n");
			return;
		}
		if(pid==0){
			// Si estamos en el primer comando
			if (i == 0){
				dup2(filedes2[1], STDOUT_FILENO);
			}
			

			//Si estamos en el último comando, dependiendo de si
			//esta en una posición impar o par, vamos a reemplazar
			//La entrada estándar para una tubería u otra. 
			//La salida Estandar estara intacta porque queremos ver la
			//Salida en la terminal


			else if (i == num_cmds - 1){
				if (num_cmds % 2 != 0){  //para numero de comando impar
					dup2(filedes[0],STDIN_FILENO);
				}else{ // para numero de comando par
					dup2(filedes2[0],STDIN_FILENO);
				}
	 

			//Si estamos en un comando que esta en medio,
			//Usaremos Dos tuberías, una para entrada y otra para
			//salida. La posición también es importante para elegir
			//Cuyo descriptor de archivo corresponde a cada entrada / salida
			}else{ // Para i impar
				if (i % 2 != 0){
					dup2(filedes2[0],STDIN_FILENO); 
					dup2(filedes[1],STDOUT_FILENO);
				}else{ // para i par
					dup2(filedes[0],STDIN_FILENO); 
					dup2(filedes2[1],STDOUT_FILENO);					
				} 
			}
			
			if (execvp(command[0],command)==err){
					perror(*args);
					exit(1);
		
			}		
		}
				
		// Cerrando los descriptores en el proceso padre
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
				
		
				
		i++;	
	} waitpid(pid,NULL,0); main();
} 
	


	




void redireccion(char * args[], char* inputFile, char* outputFile, int opcion){
	 char cwd[1024];
	int err = -1;
		pid_t pid;
	int salidatoarchivo=0; 
	
	if((pid=fork())==-1){
		printf("Error\n");
		return;
	}
	if(pid==0){
		// Opcion 0: redireccion de salida
		if (opcion == 0){
		
			//abrimos archivo para escribir
			salidatoarchivo = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600); 
			
			//Remplazamos la salida estandar con el archivo
			dup2(salidatoarchivo, STDOUT_FILENO); 
			close(salidatoarchivo);
		// Opcion 1: redireccion de entrada
		}else if (opcion == 1){
			// Abrimos para escribir
			salidatoarchivo = open(inputFile, O_RDONLY, 0600);  
				//Remplazamos la entrada estandar con el archivo
			dup2(salidatoarchivo, STDIN_FILENO);
			close(salidatoarchivo);
			// remplazamos la salida con el archivo
			salidatoarchivo = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			dup2(salidatoarchivo, STDOUT_FILENO);
			close(salidatoarchivo);		 
		}
		else if (opcion == 2){
			// Abrimos para escribir
			salidatoarchivo = open(inputFile, O_RDONLY, 0600);  
				//Remplazamos la entrada estandar con el archivo
			dup2(salidatoarchivo, STDIN_FILENO);
			close(salidatoarchivo);
			// remplazamos la salida con el archivo
			salidatoarchivo = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			dup2(salidatoarchivo, STDOUT_FILENO);
			close(salidatoarchivo);		 
		}
		 

		
		if (execvp(args[0],args)==err){
			printf("error");
		}		 
	}
	waitpid(pid,NULL,0);
}








