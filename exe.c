#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int salidatoarchivo=0;//servira para saber cuando desviar la salida estandar a un archivo

int main()
{
	char buf[1024]={'\0'};
	char *args[64];
	char *prompt={":v "};
	int fichero;

	for (;;) 
	{
		/*
		* Pide y lee un comando.
		*/
		printf(prompt);
		scanf(" %[^\n]",buf);
		if (buf=='\0') 
		{
			printf("\n");
			close(fichero);
			exit(0);
		}else
		{
		fichero=open("registrobuf.txt",O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		write(fichero,buf,sizeof(buf));
		write(fichero,"\n",2);
		close(fichero);
		/*
		* Dividir la cadena en argumentos.
		*/
		parse(buf, args);

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
parse(char *buf, char **args)
{
	while (*buf !='\0') 
	{
		/*
		* Quitar blancos. Usar nulos, para que
		* el argumento previo sea terminado 
		* automaticamente.
		*/
		while ( (*buf == ' ') || (*buf == '\t') )
			*buf++ ='\0';
	
		/*
		* Guardar los argumentos 
		*/
		*args++ = buf;
	
		/*
		* Brincar sobre los argumentos. 
		*/
		while ((*buf !='\0') && (*buf != ' ') && (*buf != '\t'))
			buf++;
	}
	
	*args ='\0';
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
	while (wait(&status) != pid)
		/* vacio */ ;
}
