#include<stdio.h>

void clearBuffer();

int main(void)
{
// El shell va a ser un programa que repetirá un bucle infinito. Se terminará 
// Con ctrl + c
char a;
int b;  
	while(b)
	{
		printf(">_ ");
		scanf("%c",&a);
		clearBuffer();
	}

	return 0;
}

void clearBuffer(void)
{
	while(getchar() != '\n');
}