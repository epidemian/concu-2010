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

	~Server();

	int run();

private:
	PeerTable _peerTable;

	void processMessage(const Message& message, bool& exit);

	static bool createQueueFile();
	static void destroyQueueFile();

	void processRegisterNameRequest(const string&,pid_t);
	void processPeerTableRequest(pid_t);
	void processUnregisterNameRequest(const string&);
	void processShowPeerTableRequest();
};


#endif /* SERVER_H_ */
