#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "Semaforo.h"
#include "SharedMemory.h"

#define NOMBRE1	"main.cc"
#define NOMBRE2	"main2.cc"
#define NOMBRE3	"main3.cc"
#define	SHM		"Semaforo.cpp"
#define LETRA	'a'

using namespace std;

int calcularRandom ();

int main () {

	SharedMemory < int > buffer;
	Semaforo semaforo1 ( (char*)NOMBRE1,0 );
	Semaforo semaforo2 ( (char*)NOMBRE2,0 );
	Semaforo semaforo3 ( (char*)NOMBRE3,0 );

	int pid = fork ();

	if ( pid == 0 ) {

		int pid2 = fork ();

		if ( pid2 == 0 ) {

			int pid3 = fork ();

			if ( pid3 == 0 ) {

				// lector 1
				buffer.crear ( (char*)SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo1.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
				exit ( 0 );
			} else {

				// lector 2
				buffer.crear ( (char*)SHM,LETRA );

				cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
				semaforo2.p ();

				int resultado = buffer.leer ();
				cout << "Lector (pid " << getpid() << "): tome el semaforo - valor leido = " << resultado << endl;
				exit ( 0 );
			}

		} else {

			// lector 3
			buffer.crear ( (char*)SHM,LETRA );

			cout << "Lector (pid " << getpid () << "): esperando por el semaforo" << endl;
			semaforo3.p ();

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
		semaforo1.v ();
		semaforo2.v ();
		semaforo3.v ();
		cout << "Escritor: libere los semaforos" << endl;
		semaforo1.eliminar ();
		semaforo2.eliminar ();
		semaforo3.eliminar ();
		exit ( 0 );
	}
}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 10 + 1;
	return resultado;
}
