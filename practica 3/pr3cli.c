#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>

#define clave 0x16096153L

void limpio()
{
	fflush(stdout);
	__fpurge(stdin);
}


int main(int argc,char*argv[])
{
	int id,idsem,var,comp;
	char*dir;
	char msg[255];
	struct sembuf sum[2]={{0,1,0},{1,1,0}},res[2]={{0,-1,0},{1,-1,0}};

	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem=semget(clave,2,0666);
	semop(idsem,&sum[1],1);

	do
	{
		semop(idsem,&res[0],1);
		printf("Introduce texto: ");
		limpio();
		fgets(msg,sizeof(msg),stdin);
		comp=strcmp(msg,"salir\n");
		sprintf(dir,"%s",msg);
		semop(idsem,&sum[0],1);

		semop(idsem,&res[0],1);
		sprintf(msg,"%s",dir);
                printf("Texto mayus: %s\n",msg);
		semop(idsem,&sum[0],1);
	}while(comp!=0);

	semop(idsem,&res[1],1);
	shmdt(dir);

	return 0;

}
