#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h> //para el tuper
#include <pthread.h>

pid_t pidp=0;
pid_t pidh1=0;
pid_t pidh2=0;
int t=1;
char cadena[100]="0";

void crear();
void menu();
void ecrfich();
void limpio();
void muero();
void paso();
void *mayus(void*);

void crear()
{
	int st1,st2;

	signal(SIGUSR1,muero);
	signal(SIGUSR2,paso);

	if((pidh1=fork())==0)
	{
		pidh1=getpid();
		while(t)
		{
			pause();
		}
	}
	else
	{
		if((pidh2=fork())==0)
		{
			pidh2=getpid();
			menu();
		}
		else
		{
			signal(SIGUSR2,SIG_IGN);
			pause();
			waitpid(pidh2,&st2,0);
		}
		waitpid(pidh1,&st1,0);
	}
}

void menu()
{
	int op=0;
	char car[3];


	do
	{
		sleep(1);
		printf("1- Enviar señal padre\n"
			"2- Enviar señal hijo 1\n"
			"3- Introducir mensaje\n"
			"4- Salir\n"
			"Elige una opcion: ");
		limpio();
		fgets(car,sizeof(car),stdin);
		op=atoi(car);

		switch(op)
		{
			case 1:
				printf("Enviando señal al padre\n");
				kill(pidp,SIGUSR2);
				break;
			case 2:
				printf("Enviando señal al hijo 1\n");
				kill(pidh1,SIGUSR2);
				break;
			case 3:
				ecrfich();
				break;
			case 4:
                                printf("Fin del programa\n");
				kill(pidp,SIGUSR1);
                                break;
			default:
				printf("Numero no valido, vuelve a intentarlo\n");
				break;
		}
	}while(op!=4);
}

void ecrfich()
{
	FILE *fp;
	char cadena[100]="0";
	int cuent= 0,i=0;

	fp=fopen("Fichero.txt","a");
	if(fp==NULL)
	{
		fputs("File error",stderr);
		exit(1);
	}
	printf("Introduce frase: ");
	limpio();
	fgets(cadena,sizeof(cadena),stdin);
	for(i=0;i<sizeof(cadena);i++)
	{
		if(cadena[i]!=0)
			cuent++;
	}
	fwrite(cadena,sizeof(char),cuent,fp);
	fclose(fp);
}

void limpio()
{
	fflush(stdout);
	__fpurge(stdin);
}

void muero()
{
	t=0;
	kill(pidh2,SIGKILL);
	kill(pidh1,SIGKILL);
}

void paso()
{
	FILE *fp;
	char aus[100];
	int cuent=0,i=0;
	pthread_t h;

	fp=fopen("Fichero.txt","r");
	if(fp==NULL)
	{
		printf("Fichero no creado\n");
		exit(1);
	}
	else
	{
		printf("\nElcontenido del archivo es \n\n");
		fread(cadena,sizeof(char),sizeof(cadena),fp);
		for(i=0;i<sizeof(cadena);i++)
		{
			if(cadena[i]!=0)
			{
				cuent++;
				printf("%c",cadena[i]);
			}
		}
		printf("\n");
		pthread_create(&h,0,mayus,0);

	}
	fclose(fp);
	pthread_exit(0);
}

void *mayus(void *a)
{
	int i=0;
	char aus[100];
	for(i=0;i<sizeof(cadena);i++)
                {
                        aus[i]=toupper(cadena[i]);
                }
                printf("%s",aus);
                printf("\n");
}

int main(int argc,char* argv[]){

	pidp=getpid();
	printf("El PID del padre es: %d\n",pidp);

	crear();

}

