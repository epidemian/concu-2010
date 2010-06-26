/*
 * peer_table.cpp
 *
 *  Created on: May 11, 2010
 *      Author: demian
 */

#include "model/peer_table.h"
#include "model/model_error.h"

#include <algorithm>

void PeerTable::addPeer(const PeerTableEntry& peer)
{
	for (PeerVector::iterator it = _peers.begin(); it != _peers.end(); ++it)
	{
		if (it->getName() == peer.getName())
			throw ModelError("A peer with the same name already exists");
		if (it->getId() == peer.getId())
			throw ModelError("A peer with the same id already exists");
	}

	_peers.push_back(peer);
}


void PeerTable::removePeer(const PeerTableEntry& peer)
{
	PeerVector::iterator it = std::find(_peers.begin(), _peers.end(), peer);
	if (it == _peers.end())
		throw ModelError("Peer does not exist");
	_peers.erase(it);
}


ByteArray PeerTableEntry::serialize()
{
	ByteArray bytes;
	addStringToByteArray(bytes,_name);
	addStringToByteArray(bytes,_id);
	return bytes;
}

void PeerTableEntry::deserialize(const ByteArray& bytes)
{


}
