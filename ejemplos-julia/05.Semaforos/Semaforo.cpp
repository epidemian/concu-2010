#include "Semaforo.h"

Semaforo :: Semaforo ( char* nombre,int valorInicial ) {

	this->valorInicial = valorInicial;

	key_t clave = ftok ( nombre,'a' );
	this->id = semget ( clave,1,0666 | IPC_CREAT );

	this->inicializar ();
}

Semaforo::~Semaforo() {
}

int Semaforo :: inicializar () {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->valorInicial;
	int resultado = semctl ( this->id,0,SETVAL,init );
	return resultado;
}

int Semaforo :: p () {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = 0;	// esperar hasta obtener el semaforo

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

int Semaforo :: v () {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = 0;	// esperar hasta obtener el semaforo

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

void Semaforo :: eliminar () {
	semctl ( this->id,0,IPC_RMID );
}
