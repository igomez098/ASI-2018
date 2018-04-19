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
		printf("Introduce intro para continuar: ");
		limpio();
		fgets(car,sizeof(car),stdin);
	}while(car[0]!='\n');
}

int main(int argc,char*argv[])
{
	int id,idsem,numcli=0;
	char*dir;
	char msg[]="Hola soy el servidor";
	char nuevo[256];
	struct sembuf sum[2]={{0,1,0},{1,1,0}},res[2]={{0,-1,0},{1,-1,0}};


	id=shmget(clave,1024,IPC_CREAT|0666);
	idsem=semget(clave,2,IPC_CREAT|0666);
	semctl(idsem,0,SETVAL,1);
	semctl(idsem,1,SETVAL,1);

	semop(idsem,&res[0],1);
	dir=shmat(id,0,0);
	sprintf(dir,"%s",msg);
	*((int*)(dir+sizeof(msg)))=numcli;
	semop(idsem,&sum[0],1);

	sleep(5);

	semop(idsem,&res[0],1);
	sscanf(dir,"%s",nuevo);
	numcli=*((int*)(dir+sizeof(nuevo)));
	semop(idsem,&sum[0],1);

	printf("El mensaje del cliente %d es: %s\n",numcli,nuevo);

	intro();
	shmdt(dir);
	shmctl(id,IPC_RMID,NULL);

	return 0;
}
