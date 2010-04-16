#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "Semaforo.h"
#include "SharedMemory.h"

#define NOMBRE	"Semaforo.cpp"
#define	SHM		"main2.cc"
#define LETRA	'a'

using namespace std;

int calcularRandom ();

int main () {

	SharedMemory < int > buffer;
	Semaforo semaforo ( (char*)NOMBRE,0 );

	int pid = fork ();

	if ( pid == 0 ) {

		int pid2 = fork ();

		if ( pid2 == 0 ) {

			int pid3 = fork ();

			if ( pid3 == 0 ) {

				// lector 1
				buffer.crear ( (char*)SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
				exit ( 0 );
			} else {

				// lector 2
				buffer.crear ( (char*)SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
				exit ( 0 );
			}

		} else {

			// lector 3
			buffer.crear ( (char*)SHM,LETRA );

			cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
			semaforo.p ();

			int resultado = buffer.leer ();
			cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
			exit ( 0 );
		}

	} else {

		// escritor
		buffer.crear ( (char*)SHM,LETRA );
		int aDormir = calcularRandom ();

		cout << "Escritor (pid " << getpid () << "): duerme " << aDormir << " segundos hasta liberar el semaforo" << endl;
		sleep ( aDormir );

		buffer.escribir ( aDormir );
		semaforo.v ();
		semaforo.v ();
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
