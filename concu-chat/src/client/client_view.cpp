/*
 * client_view.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_view.h"

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
