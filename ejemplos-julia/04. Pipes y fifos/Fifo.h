#ifndef FIFO_H_
#define FIFO_H_

#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

class Fifo {

	private:
		char nombre[255];
		int fileDes;

	public:
		Fifo ( const char* nombre );
		virtual ~Fifo();

		int escribir ( char* dato,int datoSize );
		int leer ( char* buffer,int buffSize );
		void cerrar ();
};

#endif /* FIFO_H_ */
