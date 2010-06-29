/*
 * client_view.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_view.h"
#include "client_state.h"
#include "utils.h"

#include <iostream>

using std::cout;

const string ClientView::PEER_TABLE_COMMAND = "PEERTABLE";
const string ClientView::START_CHAT_COMMAND = "CHAT";
const string ClientView::END_CHAT_COMMAND = "ENDCHAT";
const string ClientView::LOWERCASE_YES = "yes";
const string ClientView::LOWERCASE_NO = "no";

void ClientView::askUserName()
{
	cout << "Enter your name: " << std::flush;
}

void ClientView::showInvalidName(const string& userName)
{
	cout << "The name \"" << userName << "\" is not valid\n";
}

void ClientView::showAlreadyUsedName(const string& userName)
{
	cout << "The name \"" << userName
			<< "\" is already in use. Try another one =P\n";
}

void ClientView::showPeerTable(const PeerTable& peerTable)
{
	cout << peerTable;
}

void ClientView::showIdleStateCommands()
{
	cout << "Commands:\n";
	cout << PEER_TABLE_COMMAND << " to show the connected peers\n";
	cout << START_CHAT_COMMAND
			<< " <peer-name> to start chatting with a peer\n";
}

void ClientView::showInvalidPeerName(const string& peerName)
{
	cout << "Peer " << peerName
			<< " does not exist. Try updating the peer table\n";
}

void ClientView::showInvalidCommand(const string& command)
{
	cout << "Invalid command: " << command << "\n";
}

void ClientView::showWaitingPeerResponse(const string& peerName)
{
	cout << "Waiting for " << peerName << " response\n";
}

void ClientView::askUserStartChatWith(const string& peerName)
{
	cout << peerName << " wants to chat with you\n";
	cout << "Accept? (" << LOWERCASE_YES << "/" << LOWERCASE_NO << "): " << std::flush;
}

void ClientView::showPeerCanceledChat(const string& peerName)
{
	cout << peerName << " doesn't want to talk to you. Sorry =(\n";
}

void ClientView::showPeerAcceptedChat(const string& peerName)
{
	cout << peerName << " accepted!\n";
}

void ClientView::showStartChatMessage(const string& peerName)
{
	cout << "Chat session with " << peerName << " started\n";
	cout << "Type " << END_CHAT_COMMAND << " to end the chat session\n";
}

void ClientView::showPeerLeftChat(const string& peerName)
{
	cout << peerName << " has left =(\n";
}

void ClientView::showChatMessage(const string& peerName, const string& chatMessage)
{
	cout << peerName << " says: " << chatMessage << "\n";
}

bool ClientView::isYesString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_YES;
}

bool ClientView::isNoString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_NO;
}
