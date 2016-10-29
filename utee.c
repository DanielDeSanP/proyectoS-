#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc,char* argv[])
{
	FILE* archivo;
	char buffer[512];
	int pe[2],ps[2],numbyte;
	if(argc==2)
	{
		if((archivo=fopen(argv[1],"w"))!=NULL)
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
		}else
		{
			printf("\n\terror al abrir el archivo especificado");
		}
	}else
	{
		printf("\n\tnumero incorrecto de argumentos, utee <filename>");
	}
	close(ps[1]);
	close(pe[0]);
	fclose(archivo);
	return 0;
}