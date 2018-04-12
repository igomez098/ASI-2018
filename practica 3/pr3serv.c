#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


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
		printf("Introduce intro para continuar: ");
		limpio();
		fgets(car,sizeof(car),stdin);
	}while(car[0]!='\n');
}

int main(int argc,char*argv[])
{
	int id,idsem;
	char*dir;
	char msg[]="HOLA";
	struct sembuf sum={0,1,0},res={0,-1,0};


	id=shmget(clave,1024,IPC_CREAT|0666);
	dir=shmat(id,0,0);
	sprintf(dir,"%s",msg);
	idsem=semget(clave,1,IPC_CREAT|0666);
	semctl(idsem,0,SETVAL,1);
	while(car!='s')
	{
		printf("Pulsa s para salir: ");
		limpio();
		fgets(car,sizeof(car),stdin);
	}
	shmdt(dir);
	shmctl(id,IPC_RMID,NULL);

	return 0;
}
