#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "Semaforo.h"
#include "SharedMemory.h"

#define NOMBRE	"main.cc"
#define	SHM		"Semaforo.h"
#define LETRA	'a'

using namespace std;

int calcularRandom ();

int main () {

	SharedMemory < int > buffer;
	Semaforo semaforo ( (char*)NOMBRE,0 );

	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		buffer.crear ( (char*)SHM,LETRA );
		cout << "Lector: esperando por el semaforo . . ." << endl;
		semaforo.p ();
		int resultado = buffer.leer ();
		cout << "Lector: tome el semaforo - valor leido = " << resultado << endl;
		exit ( 0 );

	} else {

		// escritor
		buffer.crear ( (char*)SHM,LETRA );
		int aDormir = calcularRandom ();
		cout << "Escritor: durmiendo " << aDormir << " segundos hasta liberar el semaforo" << endl;
		sleep ( aDormir );
		buffer.escribir ( aDormir );
		semaforo.v ();
		cout << "Escritor: libere el semaforo" << endl;
		semaforo.eliminar ();
		exit ( 0 );
	}
}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}
