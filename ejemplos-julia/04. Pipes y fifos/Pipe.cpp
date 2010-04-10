#include "Pipe.h"

Pipe :: Pipe() {

	pipe ( this->descriptors );
	/*fcntl ( this->descriptors[0],F_SETFL,O_NONBLOCK );
	fcntl ( this->descriptors[1],F_SETFL,O_NONBLOCK );*/
	this->lectura = true;
	this->escritura = true;

}

void Pipe :: setearModo ( int modo ) {

	if ( modo == LECTURA ) {

		close ( this->descriptors[1] );
		this->escritura = false;

	} else if ( modo == ESCRITURA ) {

		close ( this->descriptors[0] );
		this->lectura = false;

	}
}

int Pipe :: escribir ( char* dato,int datoSize ) {

	if ( this->lectura == true ) {
		close ( this->descriptors[0] );
		this->lectura = false;
	}

	int resultado = write ( this->descriptors[1],dato,datoSize );
	return resultado;
}

int Pipe :: leer ( char* buffer,int buffSize ) {

	if ( this->escritura == true ) {
		close ( this->descriptors[1] );
		this->escritura = false;
	}

	int resultado = read ( this->descriptors[0],buffer,buffSize );
	return resultado;
}

int Pipe :: getFdLectura () {

	if ( this->lectura == true )
		return this->descriptors[0];
	else
		return -1;
}

int Pipe :: getFdEscritura () {

	if ( this->escritura == true )
		return this->descriptors[1];
	else
		return -1;
}

void Pipe :: cerrar () {

	if ( this->lectura == true ) {
		close ( this->descriptors[0] );
		this->lectura = false;
	}

	if ( this->escritura == true ) {
		close ( this->descriptors[1] );
		this->escritura = false;
	}
}

Pipe::~Pipe() {
}
