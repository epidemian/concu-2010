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

#include <string>

using std::string;

class Client;

class ClientState
{
public:
	ClientState(Client& client);
	virtual ~ClientState();

	virtual void processUserInputMessage(const string& userInput);
	virtual void processRegisterNameResponse(bool responseOk);
	virtual void processPeerTableResponse(const ByteArray& data);

protected:
	Client& _client;
};

class NotConnectedState: public ClientState
{
public:
	NotConnectedState(Client& client);
	~NotConnectedState();

	void processUserInputMessage(const string& userName);
};

class WaitingRegisterNameResponseState: public ClientState
{
public:
	WaitingRegisterNameResponseState(Client& client, const string& userName);
	~WaitingRegisterNameResponseState();

	virtual void processRegisterNameResponse(bool responseOk);

private:
	string _userName;
};

class ConnectedState: public ClientState
{
public:
	ConnectedState(Client& client, const string& userName);

protected:
	string _userName;
};

class IdleState: public ConnectedState
{
public:
	IdleState(Client& client, const string& userName);

	virtual void processUserInputMessage(const string& userInput);
	virtual void processPeerTableResponse(const ByteArray& data);

private:
	PeerTable _peerTable;
};


#endif /* CLIENT_STATE_H_ */
