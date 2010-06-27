/*
 * peer_table.cpp
 *
 *  Created on: May 11, 2010
 *      Author: demian
 */

#include "model/peer_table.h"
#include "model/model_error.h"

#include <algorithm>

void PeerTable::add(const Peer& peer)
{
	if (containsName(peer.getName()))
		throw ModelError("A peer with the same name already exists");
	if (containsId(peer.getId()))
		throw ModelError("A peer with the same id already exists");

	_peers.push_back(peer);
}

void PeerTable::remove(const Peer& peer)
{
	PeerVector::iterator it = std::find(_peers.begin(), _peers.end(), peer);
	if (it == _peers.end())
		throw ModelError("Peer does not exist");
	_peers.erase(it);
}

bool PeerTable::containsId(const string& peerId) const
{
	return bool(getById(peerId));
}

bool PeerTable::containsName(const string& peerName) const
{
	return bool(getByName(peerName));
}

const Peer* PeerTable::getById(const string& peerId) const
{
	for (size_t i = 0; i < _peers.size(); i++)
		if (_peers[i].getId() == peerId)
			return &_peers[0];
	return 0;
}

const Peer* PeerTable::getByName(const string& peerName) const
{
	for (size_t i = 0; i < _peers.size(); i++)
		if (_peers[i].getName() == peerName)
			return &_peers[0];
	return 0;
}

ByteArray PeerTable::serialize()
{
	ByteArrayWriter writer;
	// The first element in the byte array is the size of the peer table.
	writer.write(_peers.size());
	for (int i = 0; i < _peers.size(); i++)
	{
		Peer entry = _peers[i];
		writer.writeString(entry.getName());
		writer.writeString(entry.getId());
	}

	return writer.getByteArray();
}

void PeerTable::deserialize(const ByteArray& bytes)
{
	ByteArrayReader reader(bytes);
	int size = reader.read<int> ();

	for (int i = 0; i < size; i++)
	{
		std::string name = reader.readString();
		std::string id = reader.readString();

		Peer entry(name, id);
		_peers.push_back(entry);
	}
}
