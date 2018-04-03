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
	//Ejercicio 4
	int idsem2;
	//Ejercicio 5
	struct sembuf sum={0,1,0},res={0,-1,0};
	int sec8;
	//Ejercicio 6
	struct sembuf a[2]={{0,1,0},{1,1,0}},b[2]={{0,-1,0},{1,-1,0}};
	int sec10;

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
	semctl(idsem1,0,SETVAL,153);//Inicializar con 153
	intro();

	//Ejercicio 4
	idsem2=semget(clave,2,IPC_CREAT|0666);//Tenemos un array de 2 semaforos
	semctl(idsem2,0,SETVAL,1);
	semctl(idsem2,1,SETVAL,2);
	intro();

	//Ejercicio 5
	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem1=semget(clave,1,0666);
	/*
	Necesitamos restarle 1 al semaforo ya que:
	-Si el monitor le suma 1 con la resta nos da 0
	-Si el monitor no suma nada se bloquea ya que un semaforo no tiene
	numeros negativos
	*/
	semop(idsem1,&res,1);
	sec8=*((int*)dir);//recogemos el secreto 8 de la mc
	sec8=(-1)*sec8;//Cambiamos de signo
	*((int*)dir)=sec8;//Escribir en la memoria pos 0 el sec8
	sleep(3);
	semop(idsem1,&sum,1);
	/*Sumamos 1 al semaforo para que una vez enescrito en la mc
	el monitor pueda acceder a el*/
	intro();

	//Ejercicio 6
	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem2=semget(clave,2,0666);
	/*Tenemos que restar los dos semaforos a la vez:
	El monitor los activa los dos a la vez por lo que
	creamos la estructura como esta arriba y no como el
	ejercicio anterior*/
	semop(idsem2,b,2);//Restamos a los 2 sem a la vez
	sec10=*((int*)dir);
	sec10=(-1)*sec10;
	*((int*)dir)=sec10;
	sleep(3);
	semop(idsem2,a,2);
	intro();

	//Ejercicio 7
	id=shmget(clave,1024,0666);
	dir=shmat(id,0,0);
	idsem1=semget(clave,1,IPC_CREAT|0666);
	semop(idsem1,&res,1);
	sleep(1);
	shmdt(dir);
	shmctl(id,IPC_RMID,NULL);//Eliminar mc
	semctl(idsem1,1,IPC_RMID,NULL);//Eliminar sem
	intro();

	return 0;
}
