#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>


#define clave 0x16096153L

void limpio()
{
	fflush(stdout);
	__fpurge(stdin);
}

void intro()
{
	char car[2];

	do
	{
		printf("Introduce enter para continuar: ");
		limpio();
		fgets(car,sizeof(car),stdin);
	}while(car[0]!='\n');
}


int main(int argc,char*argv[])
{
	//Ejercicio 1 y 2
	int id,secre1,offset;
	char*dir,sec2[256];
	//Ejercicio 3
	int idsem1;

	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);//Vinculacion
	secre1=*((int*)dir);//cogemos el secreto 1
	offset=*((int*)(dir+sizeof(int)));//leemos posicion
	sscanf(dir+offset,"%s",sec2);//secreto 2
	sleep(3);
	shmdt(dir);
	printf("Secreto 1: %d\n",secre1);
	printf("Secreto 2: %s\n",sec2);
	intro();

	//Ejercicio 2
	id=shmget(clave,1024,IPC_CREAT|0666);
	dir=shmat(id,0,0);

	sprintf(dir,"%s",sec2);
	sscanf(sec2,"<%03d>",&offset);
	sprintf(dir+offset+16,"<%03d>",secre1);
	sleep(3);
	shmdt(dir);
	intro();

	//Ejercicio 3
	idsem1=semget(clave,1,IPC_CREAT|0666);//creamos semaforo
	semctl(idsem1,1,SETVAL,153);//Inicializar con 153
	intro();

	return 0;
}
