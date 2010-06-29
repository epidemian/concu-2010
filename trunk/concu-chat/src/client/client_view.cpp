/*
 * client_view.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_view.h"
#include "client_state.h"

#include <iostream>

using std::cout;

void ClientView::askUserName()
{
	cout << "Enter your name: ";
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
	// TODO: Implement me!
	cout << "Showing table!\n";
}

void ClientView::showIdleStateCommands()
{
	cout << "Commands:\n";
	cout << IdleState::PEER_TABLE_COMMAND << " to show the connected peers\n";
	cout << IdleState::START_CHAT_COMMAND
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
