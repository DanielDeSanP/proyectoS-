#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void parse(char, char **);
void ejecutar(char **,int ,int*,int **);
void dividepipe(char*,char**);
char *prompt=":V";
int numero_hijos=0;

int main()
{
	char buf[4096]={'\0'};
	char *buffdividido[1024]={NULL};//un buffer preliminar que tendra las cadenas divididas a partir de los pipes
	char **args[64]={NULL};//contendra las instrucciones junto con sus argumentos pasados al execvp
	int *PIDhijos;
	int aux=0;
	FILE archivo;
	if(archivo=fopen("registrobuf.txt","r")==NULL)// agui la idea es que en automatico se guarde lo leido en un archivo
	{											  //que si ya esta hecho lo respete y si no lo cree, en cualquier caso 				
		archivo=fopen("registrobuf.txt","w");     //abrira el archivo en modo apend para que se agregue lo escrito al final del
		fclose(archivo);                          //archivo
		archivo=fopen("registrobuf.txt","a");
	

	}else
	{
		fclose(archivo);
		archivo=fopen("registrobuf.txt","a");
	}


	for (;;) 
	{
		/*
		* Pide y lee un comando.
		*/
		printf("%s ",prompt);

		if (gets(buf) == NULL) 
		{
			printf("\n");
			fclose(archivo);
			exit(0);
		}
		fwrite(buf,strlen(buf),1,archivo);

		/*
		* Dividir la cadena en argumentos.
		*/
		dividepipe(buf,buffdividido);// primeor divide la cadena tomando los pipes que tiene esta
		if(numero_hijos>1)//cuando acabe de dividir el numero de hijos ya debe de estar contabilizado
		{				  //si este es mayor a 1 se hace todo el desmadre de los pipes, de lo contrario no tiene caso 
		int pipes[numero_hijos][2];//declara un monton de enteros que serviran para hacer los pipes
			for(i=0;i<numero_hijos;i++)
			{
				pipe(pipes[i]);
				PIDhijos[i]=(int*)calloc(1,sizeof(int));//reserva espacio en la memoria para poder almacenar los pid 
			}											//de los procesos creados con los fork y asi poder manejar los
														//pipes de mejor manera	
		}
		for(i=0;i<numero_hijos;i++)// se dividen las cadenas de los procesos hijos para poder pasarles
		{						   // sus argumentos a cada exec 
			parse(buffdividido[i], args[i]);
		}
		for(i=0;i<numero_hijos;i++)
		{
			ejecutar(args[i],i,PIDhijos,pipes);
		}
		/*
		* Ejecutar el comando.
		* por modificar para que funcione
		*/
	}

}
/* dividepipe-- divide la cadena obtenida en buff con los pipes
* haciendo un proceso similar a parse
*/
void dividepipe(char *buf,char** buffdividido)
{
	while (*buf != (char) NULL) 
	{
		numero_hijos++;
		/*
		* quitar pipes, para que
		* el argumento previo sea terminado 
		* automaticamente.
		*/
		while ( (*buf == '+') )// se decidio que el pipe sea el caracter : +, puede ser cambiado por cualquier otro
			*buf++ = (char) NULL;
	
		/*
		* Guardar los argumentos 
		*/
		*buffdividido++ = buf;
	
		/*
		* Brincar sobre los argumentos. 
		*/
		while ((*buf != (char) NULL) && (*buf != '+'))//si se cambia tambien debera de cambiarse el simbolo del pipe aqui
			buf++;
	}
	
	*buffdividido = (char) NULL;
}



/*
* parse--divide el comando que esta en buf
*        en argumentos. 
*/
parse(char *buf, char **args)//segun yo parse deberia de trabajar correctamente :9
{
	while (*buf != (char) NULL) 
	{
		/*
		* Quitar blancos. Usar nulos, para que
		* el argumento previo sea terminado 
		* automaticamente.
		*/
		while ( (*buf == ' ') || (*buf == '\t') )
			*buf++ = (char) NULL;
	
		/*
		* Guardar los argumentos 
		*/
		*args++ = buf;
	
		/*
		* Brincar sobre los argumentos. 
		*/
		while ((*buf != (char) NULL) && (*buf != ' ') && (*buf != '\t'))
			buf++;
	}
	
	*args = (char) NULL;
}


/*
* ejecutar--genera un proceso hijo y ejecuta
*           el programa.
*/
ejecutar(char **args,int m,int* pid_hijo,int **pipes)// la idea es que cada proceso tenga acceso a todos los pipes, 
{													 //aunque basta con que tenga acceso al pipe generado anterior a el
	int pid, status,status1;                         //asi como al pid del proceso anterior a el

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
	pid_hijo[m]=pid;
	
	/*
	* El proceso hijo ejecuta el codigo dentro del if.
	*/
	if (pid == 0) //aqui intento que el proceso generado espere a la ejecucion del proceso que se genero antes de el, pues
// tiene que esperar a que de la entreda estandar le llegen los datos de despues de la ejecucion del programa anterior
	{
		if(m==1)
		{
			close(pipes[1][0]);
		}else
		{
			if(m==numero_hijos)
			{
				close(pipes[numero_hijos][0]);
			}else
			{
				close(pipes[m-1][1]);
				close(pipes[m][0]);
			}
			waitpid(pid_hijo[m-1],&status1,0);
		}
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
		if(pid_hijo[m]==numero_hijos)
		{
			waitpid(pid_hijo[m],&status,0);
		}
	}
}
