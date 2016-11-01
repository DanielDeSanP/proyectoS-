#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX 20
#define MAX_INPUT 1000

struct comando
{
	char *comando;
	char *argumentos;
};

// Esta estructura de tipo pila va a ser usada para los comandos y para los pipes
struct pila
{
	struct comando comandos[MAX]; //Unicamente para la pila de comandos
	char pipe[MAX]; //Unicamente para la pila de pipes
	int top;
};

int main()
{
	struct pila pilaPipes;
	struct pila pilaComandos;
	char *entrada = (char *)malloc(sizeof(char)*MAX_INPUT);
	fgets(entrada,MAX_INPUT,stdin);

	pilaPipes.top = 0;
	pilaComandos.top = 0; 
	int len = strlen(entrada);

	for(int i = 0; i < len; i++)
	{
		if(entrada[i] == '|' || entrada[i] == '<')
		{
			// Es un pipe
			printf("Se hace push con pipe simple\n");
			pilaPipes.pipe[pilaPipes.top] = entrada[i];
			pilaPipes.top++;

		}
		else if (entrada[i] == '>')
		{
			// Separe este caso debido a que puede haber > y >>
			if(entrada[i+1] == '>')
			{
				// Aqui es >>
				printf("Se hace push con >>\n");
				pilaPipes.pipe[pilaPipes.top] = entrada[i];
				pilaPipes.top++;
			}
			else
			{
				//Aqui es >
				printf("Se hace push con >");
				pilaPipes.pipe[pilaPipes.top] = entrada[i];
				pilaPipes.top++;
			}
		}
		else
		{
			printf("Se almacena caracter\n");
		}

	}
	// Para comprobar de que el orden sea el correcto
	int numPipes = strlen(pilaPipes.pipe);
	for(int i = 0 ; i < numPipes ; i++)
	{
		printf("%c\n",pilaPipes.pipe[i]);
	}

	return 0;
}