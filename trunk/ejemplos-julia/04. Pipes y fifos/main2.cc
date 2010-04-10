/*#include <string.h>
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
		canal.setearModo ( LECTURA );

		cout << "Lector: esperando para leer . . ." << endl;
		//sleep ( 5 );

		// se redirige la entrada estandar
		dup2 ( canal.getFdLectura(),0 );
		// se lee el dato del pipe
		cin.get ( buffer,BUFFSIZE );

		cout << "Lector: lei el dato [" << buffer << "] del pipe" << endl;
		cout << "Lector: fin del proceso" << endl;

		canal.cerrar ();
		exit ( 0 );

	} else {

		// escritor
		char *dato = (char *) "Hola mundo pipes (segundo ejemplo)!!";
		canal.setearModo ( ESCRITURA );

		sleep ( 5 );

		cout << "Escritor: escribo el dato [" << dato << "] en el pipe" << endl;

		// se redirige la salida estandar
		dup2 ( canal.getFdEscritura(),1 );
		// se escribe el dato en el pipe
		cout << dato << endl;

		canal.cerrar ();
		exit ( 0 );
	}
}
*/
