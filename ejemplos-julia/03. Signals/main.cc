#include <iostream>

#include "SIGINT_Handler.h"
#include "SignalHandler.h"


using namespace std;

int main () {

	// event handler para la senial SIGINT (-2)
	SIGINT_Handler sigint_handler;

	// se registra el event handler declarado antes
	SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

	// mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
	while ( sigint_handler.getGracefulQuit() == 0 ) {
		cout << "Soy el proceso " << getpid() << endl;
		sleep ( 2 );
	}

	// se recibio la senial SIGINT, el proceso termina
	cout << "Termino el proceso" << endl;
	return 0;
}
