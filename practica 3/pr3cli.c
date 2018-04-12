#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


#define clave 0x16096153L

void limpio()
{
	fflush(stdout);
	__fpurge(stdin);
}


int main(int argc,char*argv[])
{
	int id,idsem;
	char*dir;

	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem=semget(clave,1,0666);

}
