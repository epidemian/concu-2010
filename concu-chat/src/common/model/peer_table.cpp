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

ByteArray PeerTable::serialize()
{
	ByteArrayWriter writer;
	// The first element in the byte array is the size of the peer table.
	writer.write(_peers.size());
	for (int i = 0; i < _peers.size(); i++)
	{
		PeerTableEntry entry = _peers[i];
		writer.writeString(entry.getName());
		writer.writeString(entry.getId());
	}

	return writer.getByteArray();
}

void PeerTable::deserialize(const ByteArray& bytes)
{
	ByteArrayReader reader(bytes);
	int size = reader.read<int>();

	for (int i = 0; i < size; i++)
	{
		std::string name = reader.readString();
		std::string id = reader.readString();

		PeerTableEntry entry(name, id);
		_peers.push_back(entry);
	}
}
