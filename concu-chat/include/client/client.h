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

	Client(int argc, char* argv[]);

	~Client();

	int run();

	void changeState(ClientState* newState);
	void sendRegisterNameRequest(const string& userName);
	void sendPeerTableRequest();
	void sendStartChatRequest(pid_t peerId);
	void sendStartChatResponse(pid_t peerId, bool responseOk);
	void sendChatMessage(pid_t peerId, const string& chatMessage);
	void sendEndChatMessage(pid_t peerId);

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
