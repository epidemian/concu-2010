#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaforo {

private:
	int id;
	int valorInicial;

	int inicializar ();

public:
	Semaforo ( char* nombre,int valorInicial );
	virtual ~Semaforo();

	int p (); // decrementa
	int v (); // incrementa
	void eliminar ();
};

#endif /* SEMAFORO_H_ */
