#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "SharedMemory.h"

using namespace std;

int calcularRandom ();


int main () {

	SharedMemory<int> memoria;

	pid_t procId = fork ();

	if ( procId == 0 ) {

		int estadoMemoria = memoria.crear ( (char*) "main.cc",'R');

		cout << "Hijo: duermo 5 segundos..." << endl;
		sleep ( 5 );

		if ( estadoMemoria == SHM_OK ) {
			int resultado = memoria.leer ();
			cout << "Hijo: leo el numero " << resultado << " de la memoria compartida" << endl;
			memoria.liberar ();
		} else {
			cout << "Hijo: error en memoria compartida: " << estadoMemoria << endl;
		}
		exit ( 0 );

	} else {

		int estadoMemoria = memoria.crear ( (char*) "main.cc",'R');
		if ( estadoMemoria == SHM_OK ) {

			// escribe un dato para el hijo
			int random = calcularRandom ();
			cout << "Padre: escribo el numero " << random << " en la memoria compartida" << endl;
			memoria.escribir ( random );

			// espera a que termine el hijo
			int estado = 0;
			wait ( &estado );

			// libera la memoria
			memoria.liberar ();
		} else {
			cout << "Padre: error en memoria compartida: " << estadoMemoria << endl;
		}

		cout << "Padre: fin del proceso" << endl;
		exit ( 0 );

	}

}

int calcularRandom () {
	srand ( time(NULL) );
	int resultado = rand() % 100 + 1;
	return resultado;
}
