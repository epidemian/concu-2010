/*
 * server.h
 *
 *  Created on: 26/06/2010
 *      Author: nicolas
 */

#include "model/message.h"

#ifndef SERVER_H_
#define SERVER_H_

class Server
{
public:
	Server();
	Server(int argc, char* argv[]);

	~Server();

	int run();

private:
	string _queueFileName;

	void processMessage(const Message& message);
	void createQueueFile();
	void destroyQueueFile();
};


#endif /* SERVER_H_ */
