#include <string.h>
#include <iostream>
#include <stdlib.h>

#include "Pipe.h"

#define BUFFSIZE 100

using namespace std;

int main () {

	Pipe canal;

	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		char buffer [ BUFFSIZE ];

		cout << "Lector: esperando para leer..." << endl;
		//sleep ( 5 );
		int bytes = canal.leer ( buffer,BUFFSIZE );
		buffer [ bytes ] = '\0';

		cout << "Lector: lei el dato [" << buffer << "] (" << bytes << " bytes) del pipe" << endl;
		cout << "Lector: fin del proceso" << endl;

		canal.cerrar ();
		exit ( 0 );

	} else {

		// escritor
		char *dato = (char *) "Hola mundo pipes!!";
		sleep ( 5 );
		canal.escribir ( dato,strlen(dato) );

		cout << "Escritor: escribi el dato [" << dato << "] en el pipe" << endl;
		cout << "Escritor: fin del proceso" << endl;

		canal.cerrar ();
		exit ( 0 );
	}
}
