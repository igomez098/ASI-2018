#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <ctype.h>

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
	int id,idsem,numcli=0,i=0;
	char*dir;
	char msg[21]="Hola soy el servidor";
	char nuevo[255];
	struct sembuf sum[2]={{0,1,0},{1,1,0}},res[2]={{0,-1,0},{1,-1,0}};


	id=shmget(clave,1024,IPC_CREAT|0666);
	dir=shmat(id,0,0);
	idsem=semget(clave,2,IPC_CREAT|0666);
	semctl(idsem,0,SETVAL,1);
	semctl(idsem,1,SETVAL,0);

	do
	{
		numcli=semctl(idsem,1,GETVAL);
	}while(numcli==0);

	while(numcli!=0)
	{
		semop(idsem,&res[0],1);
		sprintf(nuevo,"%s",dir);

		for(i=0;i<strlen(nuevo);i++)
		{
			nuevo[i]=toupper(nuevo[i]);
		}
		sprintf(dir,"%s",nuevo);
		semop(idsem,&sum[0],1);
		numcli=semctl(idsem,1,GETVAL);
	}
	intro();
	shmdt(dir);
	shmctl(id,IPC_RMID,NULL);

	return 0;
}
