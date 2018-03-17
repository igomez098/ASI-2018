#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define ELIMCOLA	system("ipcrm -Q 0x16096153")

char fifo_name1[]="/tmp/fifo_monitor_1";
char fifo_name2[]="/tmp/fifo_monitor_2";
char ar[100]="0";
char at[11]="0";
char at1[6];//="0";

typedef struct Mi_Tipo_Mensaje
{
	long id;
	char mensaje[11];
}Mi;

typedef struct msgbuf
{
	long tipo;
	char texto[100];
}msgbuf;

void limpio();
void intro();
void error(ssize_t);
void ej1();
void ej2();
void ej3();
void ej4();
void ej5();
void ej6();

void limpio()
{
	fflush(stdout);
	__fpurge(stdin);
}

void intro()
{
	char car[2];
	int i;

	do
	{
		printf("Introduce enter para continuar: ");
		limpio();
		fgets(car,sizeof(car),stdin);
	}while(car[0]!='\n');
}

void error(ssize_t env)
{
	if(env==-1)
		printf("Cola de mensajes no  destruida\n");
	else
		printf("Cola destruida\n");
}

void ej1()
{
	int fd, nbytes;

	fd=open(fifo_name1,O_RDONLY);
	nbytes=read(fd,ar,sizeof(&ar));
	printf("El secreto 1 es: %s\n",ar);
	close(fd);
}

void ej2()
{
	int i=0,fd2,fd,n;
	char sec2[6]="0";

	for(i=0;i<sizeof(ar);i++)
	{
		if(ar[i]!=0)
		{
			sec2[i]=ar[i];
		}
	}

	mkfifo(fifo_name2,S_IRWXU);
	fd2=open(fifo_name2,O_WRONLY);
	write(fd2,sec2,sizeof(sec2));

	fd=open(fifo_name1,O_RDONLY);
	n=read(fd,ar,sizeof(ar));//n guarda el numero de bytes que lee, si es -1 error, 0 nada, 0< nºbytes
	printf("El secreto 2 es: %s\n",ar);
	write(fd2,ar,sizeof(ar));

	close(fd2);
	close(fd);
}

void ej3()
{
	msgbuf buff;
	ssize_t env;
	int ide;
	long tip=0L;

	ide=msgget(0x16096153L,0666);
	printf("Identificador: %d\n",ide);
	env=msgrcv(ide,&buff,sizeof(buff),tip,0);

	if(env==-1)
	{
		printf("Error al leer la cola de mensajes\n");
	}
	else
	{
		printf("El secreto 3 es: %ld\n",buff.tipo);
		printf("El secreto 4 es: %s\n",buff.texto);
	}

	sprintf(at,"<%03ld>%s",buff.tipo,buff.texto);

	env=msgctl(ide,IPC_RMID,(struct msqid_ds *)NULL);

	error(env);
}

void ej4()
{
	int iden;
	Mi M;
	ssize_t env;

	printf("La cadena a enviar es: %s\n",at);
	//Creamos la cola de mensajes.
	iden=msgget(0x16096153L,0666|IPC_CREAT);
	M.id=iden;
	strcpy(M.mensaje,at);

	msgsnd(iden,&M,sizeof(Mi)-sizeof(long),0);
}

void ej5()
{
	msgbuf buff;
	int iden;
	int env;
	long tip,pidcli;

	tip=pidcli=(long)getpid();
	printf("El PID del cliente es: %ld\n",pidcli);

	iden=msgget(0x16096153L,0666);
	printf("Identificador: %d\n",iden);
	env=msgrcv(iden,&buff,sizeof(buff)-sizeof(long),tip,0);
//El ultimo elemento si es cero bloquea el proceso hasta que se lea todo el tamaño establecido
//Con un IPC_NOWAIT no se bloquea el proceso si no hay nada que leer
	if(env==-1)
		printf("Fallo al leer la cola\n");
	else
	{
		printf("Pid cliente: %ld\n",buff.tipo);
		printf("Secreto 6: %s\n",buff.texto);
	}
	strcpy(at1,buff.texto);
	//env=msgctl(iden,IPC_RMID,(struct msqid_ds *)NULL);
	//error(env);
}

void ej6()
{
	int iden,jordi;
	msgbuf buff;
	char temp[6];

	printf("Introduce el pid del monitor: ");
	limpio();
	fgets(temp,6,stdin);
	sscanf(temp, "%ld", &buff.tipo);

	jordi=atoi(at1+1);
	printf("atoi: %d\n", jordi);
	sprintf(buff.texto,"<%03d>",jordi);
	printf("buffer: %s\n", buff.texto);

	iden=msgget(0x16096153L,0666);
	msgsnd(iden,(msgbuf *)&buff,sizeof(buff)-sizeof(long),0);

}

int main(int argc, char*argv[])
{

	ej1();
	intro();

	ej2();
	intro();

	ej3();
	intro();

	ej4();
	intro();

	ej5();
	intro();

	ej6();
	intro();

	return 0;
}
