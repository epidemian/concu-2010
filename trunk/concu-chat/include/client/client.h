/*
 * client.h
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "model/message.h"
#include "client_view.h"

#include <string>

class ClientState;
class Message;

using std::string;

class Client
{
public:

	static const string PEER_TABLE_COMMAND;
	static const string START_CHAT_COMMAND;

	Client(int argc, char* argv[]);

	~Client();

	int run();

	void changeState(ClientState* newState);
	void sendRegisterNameRequest(const string& userName);

	ClientView& getView();
private:

	string _queueFileName;
	ClientState* _state;
	ClientView _view;

	void runUserInputProcess();
	void runMainProcess();

	void createQueueFile();
	void destroyQueueFile();
	void processMessage(const Message& message, bool& exitNow);

	void sendMessageToServer(const Message& msg);
};

#endif /* CLIENT_H_ */
