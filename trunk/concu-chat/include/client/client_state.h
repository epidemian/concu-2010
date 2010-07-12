/*
 * client_state.h
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#ifndef CLIENT_STATE_H_
#define CLIENT_STATE_H_

#include "model/peer_table.h"
#include "core/byte_array.h"
#include "ipc/message_queue.h"

#include <string>

using std::string;

// Forward declaration.
class Client;

/**
 * The abstract base class of the different client states. Consists of virtual
 * methods that control the client actions to different events that must
 * be overriden by the state subclasses.
 */
class ClientState
{
public:
	virtual ~ClientState();

	virtual void processUserInputMessage(const string& userInput);
	virtual void processRegisterNameResponse(bool responseOk);
	virtual void processPeerTableResponse(const ByteArray& data);
	virtual void processStartChatRequest(const Peer& peer);
	virtual void processStartChatResponse(bool responseOk);
	virtual void processEndChat();
	virtual void processChatMessage(const string& chatMessage);
	virtual void processExit();

protected:
	ClientState(Client& client);

	Client& _client;
};

/**
 * First state of the client. Used when the user has not registered it's name
 * yet.
 * Only responds to user input events, awaiting for the user to type a
 * valid name.
 */
class NotRegisteredState: public ClientState
{
public:
	NotRegisteredState(Client& client);
	~NotRegisteredState();

	void processUserInputMessage(const string& userName);
};

/**
 * Intermediate state in between the client has sent a register name response
 * and it's arrival.
 * Only responds to register name responses from the server, and change to
 * IdleState as soon as one arrives.
 */
class WaitingRegisterNameResponseState: public ClientState
{
public:
	WaitingRegisterNameResponseState(Client& client, const string& userName);
	~WaitingRegisterNameResponseState();

	virtual void processRegisterNameResponse(bool responseOk);

private:
	string _userName;
};


/**
 * Abstract superclass of all registered states. That is: all states after the
 * user name has been registered in the server.
 * Responds to the exit event unregistering the name from the server.
 */
class RegisteredState: public ClientState
{
public:
	RegisteredState(Client& client, const string& userName);

	virtual void processExit();

protected:
	/** The user name already registered in the server. */
	string _userName;
};

/**
 * Idle state. That is: when the user is not chatting with a peer, nor waiting a
 * start chat response from a peer or from the user.
 * Responds to multiple events:
 *  - User input: The user may enter a command to start chatting with a peer or
 *    to get the peer table.
 *  - Peer table response: the server sent the peer table in response to the
 *    user.
 *  - Start chat request: a peer wants to start a chat session with the user.
 */
class IdleState: public RegisteredState
{
public:

	IdleState(Client& client, const string& userName);

	virtual void processUserInputMessage(const string& userInput);
	virtual void processPeerTableResponse(const ByteArray& data);
	virtual void processStartChatRequest(const Peer& peer);

private:

	void processStartChatCommand(const string& peerName);
	PeerTable _peerTable;
};

/**
 * State when the user has send a start chat request to a peer and the client
 * must wait for it's response.
 */
class WaitingPeerStartChatResponseState: public RegisteredState
{
public:
	WaitingPeerStartChatResponseState(Client& client, const string& userName,
			const Peer& peer);

	virtual void processUserInputMessage(const string& userInput);
	virtual void processStartChatResponse(bool responseOk);

private:
	Peer _peer;
};

/**
 * State when a request from a peer to start chatting has arrived and the client
 * must wait for the user to decide whether or not to start chatting with the
 * peer.
 */
class WaitingUserStartChatResponseState: public RegisteredState
{
public:
	WaitingUserStartChatResponseState(Client& client, const string& userName,
			const Peer& peer);

	virtual void processUserInputMessage(const string& userInput);

private:
	Peer _peer;
};

/**
 * State when the user and a peer are chatting.
 * Responds to multuiple events:
 *  - Input message: the user has entered some text. It's interpreted as a chat
 *    message and sent to the peer.
 *  - End chat: the peer left the chat session.
 *  - Start chat request by other peer: the client responds that it is busy.
 *  - CLient exists: Not only the user name has to be unregistered from the
 *    server (as with every other RegisteredState) but also a message must be
 *    sent to the peer to inform him that the user has left.
 */
class ChattingState: public RegisteredState
{
public:
	ChattingState(Client& client, const string& userName, const Peer& peer);
	virtual ~ChattingState();

	virtual void processUserInputMessage(const string& userInput);
	virtual void processEndChat();
	virtual void processChatMessage(const string& chatMessage);
	virtual void processStartChatRequest(const Peer& peer);
	virtual void processExit();


private:
	Peer _peer;
	MessageQueue _peerQueue;
};

#endif /* CLIENT_STATE_H_ */
