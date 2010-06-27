/*
 * client_state.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_state.h"
#include "client.h"
#include "exception.h"
#include "core/byte_array.h"

#include <string>
#include <algorithm>
#include <iostream>

using std::cerr;
using std::string;

class NotSupportedMethodException: public Exception
{
public:
	NotSupportedMethodException() :
		Exception("Not supported method")
	{
	}
};

ClientState::ClientState(Client& client) :
	_client(client)
{

}

ClientState::~ClientState()
{
}

void ClientState::processUserInputMessage(const string& userInput)
{
	cerr << "Invalid processUserInputMessage() call\n";
}

void ClientState::processRegisterNameResponse(bool responseOk)
{
	cerr << "Invalid processRegisterNameResponse() call\n";
}

void ClientState::processPeerTableResponse(const ByteArray& data)
{
	cerr << "Invalid processPeerTableResponse() call\n";
}

NotConnectedState::NotConnectedState(Client& client) :
	ClientState(client)
{
	_client.getView().askUserName();
}

NotConnectedState::~NotConnectedState()
{
}

void NotConnectedState::processUserInputMessage(const string& userInput)
{
	// TODO:
//	string userName = trimString(userInput);
	string userName = userInput;
	bool valid = std::count_if(userName.begin(), userName.end(), isalnum) > 0;
	if (valid)
	{
		_client.sendRegisterNameRequest(userName);
		_client.changeState(new WaitingRegisterNameResponseState(_client,
				userName));
	}
	else
	{
		_client.getView().showInvalidName(userName);
	}
}

WaitingRegisterNameResponseState::WaitingRegisterNameResponseState(
		Client& client, const string& userName) :
	ClientState(client), _userName(userName)
{
}

WaitingRegisterNameResponseState::~WaitingRegisterNameResponseState()
{
}

void WaitingRegisterNameResponseState::processRegisterNameResponse(
		bool responseOk)
{
	if (responseOk)
		_client.changeState(new IdleState(_client, _userName));
	else
	{
		_client.getView().showAlreadyUsedName(_userName);
		_client.changeState(new NotConnectedState(_client));
	}
}

ConnectedState::ConnectedState(Client& client, const string& userName) :
	ClientState(client), _userName(userName)
{
}

IdleState::IdleState(Client& client, const string& userName) :
	ConnectedState(client, userName)
{
}

void IdleState::processUserInputMessage(const string& userInput)
{
	// TODO
	//string trimmedInput = trimString(userInput);
	if (userInput == Client::PEER_TABLE_COMMAND)
	{
		// TODO:
		//_client.requestPeerTable();
	}
	else if (userInput.find(Client::START_CHAT_COMMAND) == 0)
	{
		// string peerName = trimString(trimmedInput.substr(Client::START_CHAT_COMMAND.size()));
		string peerName = userInput.substr(Client::START_CHAT_COMMAND.size() + 1);

		const Peer* peer = _peerTable.getByName(peerName);
		if (peer)
		{
			// TODO:
			//_client.sendStartChatRequest(peer->getId());
		}
		else {
			// TODO
			//_client.getView().showInvalidPeerName(peer->getName());
		}
	}
}

void IdleState::processPeerTableResponse(const ByteArray& data)
{
	_peerTable.deserialize(data);
	_client.getView().showPeerTable(_peerTable);
}
