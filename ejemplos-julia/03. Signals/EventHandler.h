#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

class EventHandler {

public:
	virtual int handleSignal ( int signum ) = 0;

};

#endif /* EVENTHANDLER_H_ */
