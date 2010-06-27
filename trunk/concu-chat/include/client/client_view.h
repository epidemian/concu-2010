/*
 * client_view.h
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include "model/peer_table.h"

#include <string>
using std::string;


class ClientView
{
public:
	void askUserName();
	void showInvalidName(const string& userName);
	void showAlreadyUsedName(const string& userName);
	void showPeerTable(const PeerTable& peerTable);
};

#endif /* CLIENT_VIEW_H_ */
