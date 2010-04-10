/*#include <iostream>
#include <stdlib.h>

#include "Fifo.h"

#define BUFFSIZE		100
#define	ARCHIVO_FIFO	"/tmp/archivo_fifo"

using namespace std;

int main () {

	int pid = fork ();

	if ( pid == 0 ) {

		// lector
		char buffer [ BUFFSIZE ];
		Fifo canal ( ARCHIVO_FIFO );

		cout << "Lector: esperando para leer . . ." << endl;
		//sleep ( 5 );

		int bytesLeidos = canal.leer ( buffer,BUFFSIZE );
		buffer[bytesLeidos] = '\0';
		cout << "Lector: lei el dato [" << buffer << "] del pipe" << endl;

		cout << "Lector: fin del proceso" << endl;
		canal.cerrar ();
		exit ( 0 );

	} else {

		// escritor
		char *dato = (char *) "Hola mundo fifo!!\0";
		Fifo canal ( ARCHIVO_FIFO );

		sleep ( 5 );

		cout << "Escritor: escribo el dato [" << dato << "] en el fifo" << endl;
		canal.escribir ( dato,strlen(dato) );

		cout << "Escritor: fin del proceso" << endl;
		canal.cerrar ();
		exit ( 0 );
	}
}
*/
