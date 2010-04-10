#include "Fifo.h"

Fifo :: Fifo ( const char* nombre ) {

	strcpy ( this->nombre,nombre );
	// se crea el fifo
	mknod ( nombre,S_IFIFO|0666,0 );
	// se inicializa el descriptor en -1
	this->fileDes = -1;
}

Fifo :: ~Fifo () {
}

int Fifo :: escribir ( char* dato,int datoSize ) {

	// se abre el file descriptor para escritura
	if ( this->fileDes == -1)
		this->fileDes = open ( this->nombre,O_WRONLY );

	// se escriben los datos en el fifo
	int resultado = write ( this->fileDes,(const void *)dato,datoSize );

	return resultado;
}

int Fifo :: leer ( char* buffer,int buffSize ) {

	// se abre el file descriptor para lectura
	if ( this->fileDes == -1 )
		this->fileDes = open ( this->nombre,O_RDONLY );

	// se leen los datos del fifo
	int resultado = read ( this->fileDes,(void *)buffer,buffSize );

	return resultado;
}

void Fifo :: cerrar () {
	close ( this->fileDes );
	unlink ( this->nombre );
}
