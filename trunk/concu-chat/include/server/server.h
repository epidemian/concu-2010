/*
 * server.h
 *
 *  Created on: 26/06/2010
 *      Author: nicolas
 */

#include "model/message.h"
#include "model/peer_table.h"

#include <string>

using std::string;

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
	PeerTable _peerTable;

	void processMessage(const Message& message);
	void createQueueFile();
	void destroyQueueFile();

	void registerNameRequest(string,pid_t);
	void processPeerTableRequest(pid_t);
};


#endif /* SERVER_H_ */
