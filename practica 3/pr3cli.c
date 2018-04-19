#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
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


int main(int argc,char*argv[])
{
	int id,idsem,var;
	char*dir;
	char prop[]="Hola soy el cliente 1";
	char msg[255];
	struct sembuf sum[2]={{0,1,0},{1,1,0}},res[2]={{0,-1,0},{1,-1,0}};

	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem=semget(clave,2,0666);

	semop(idsem,&res[0],1);
	sscanf(dir,"%s",msg);
	var=*((int*)(dir+sizeof(msg)));
	printf("El mensaje leido es: %s\n",msg);
	if(var!=1)
		var=1;
	sprintf(dir,"%s",prop);
	*((int*)(dir+sizeof(prop)))=var;
	semop(idsem,&sum[0],1);
	shmdt(dir);

	return 0;

}
