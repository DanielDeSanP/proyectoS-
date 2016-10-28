#include <stdio.h>
#include <stdlib.h>
int obtenercadena(char** K)
{// la funcion retornara el numero de caracteres leidos, utilizara la variable char que ya se definio en main y tambien almacenara espacio de memoria de manera dinamica del tamaño de la cadena ingresada
	char a=getchar();
	int m=0;//contador de caracteres de la cadena
	//int n=0;//contador que sirve para almacenar de manera dinamica la memoria
	if(a=='\n')
	{
		return 0;
	}else
	{
		K[0]=(char*)calloc(1,sizeof(char));
		K[0][0]=a;
		++m;
		do
		{	
			a=getchar();
			if(a=='\n')
			{
				break;
			}else
			{
			K[m]=(char*)calloc(1,sizeof(char));
			K[0][m]=a;
			++m;
			}	
		}while(a!='\n');
		K[m]=(char*)calloc(1,sizeof(char));
		K[0][m]='\0';
		fflush(stdout);
		return m;
	}
}
int main(int argc, char const *argv[])
{
	int i;
	char* k;
	int p=0;

	while(i)
	{
		printf("\n>_");
		p=obtenercadena(&k);
		printf("%s %d",k,p);
	}
	return 0;
}



void clearBuffer(void)
{
	while(getchar() != '\n');
}
}