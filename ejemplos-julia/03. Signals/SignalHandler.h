#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>
#include <stdio.h>

#include "EventHandler.h"

class SignalHandler {

	private:
		static SignalHandler* instance;
		static EventHandler* signal_handlers [ NSIG ];

		SignalHandler ( void );
		static void dispatcher ( int signum );

	public:
		static SignalHandler* getInstance ();
		EventHandler* registrarHandler ( int signum,EventHandler* eh );
		int removerHandler ( int signum );

};

#endif /* SIGNALHANDLER_H_ */
