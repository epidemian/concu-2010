/*
 * peer_table.cpp
 *
 *  Created on: May 11, 2010
 *      Author: demian
 */

#include "model/peer_table.h"
#include "model/model_error.h"

#include <algorithm>
#include <iostream>

std::ostream& operator << (std::ostream& os, const Peer& peer)
{
	os << "Peer [name=" << peer.getName() << ", id=" << peer.getId() << "]";
	return os;
}

std::ostream& operator << (std::ostream& os, const PeerTable& peers)
{
	os << "Usuarios: Nombre - Dirección\n";

	vector<Peer>::const_iterator iterator = peers._peers.begin();
	for (; iterator != peers._peers.end(); iterator++)
	{
		Peer peer = *iterator;
		os << peer.getName() << " - " << peer.getId() << "\n";
	}
	return os;
}

void PeerTable::add(const Peer& peer)
{
	if (containsName(peer.getName()))
		throw ModelError("A peer with the same name already exists");
	if (containsId(peer.getId()))
		throw ModelError("A peer with the same id already exists");

	_peers.push_back(peer);
}

void PeerTable::remove(const string& peerName)
{
	PeerVector::iterator it = _peers.begin();

	for (; it != _peers.end(); it++)
	{
		Peer peer = *it;
		if (peer.getName() == peerName)
			break;
	}

	if (it == _peers.end())
		throw ModelError("Peer does not exist");
	_peers.erase(it);
}

bool PeerTable::containsId(pid_t peerId) const
{
	return bool(getById(peerId));
}

bool PeerTable::containsName(const string& peerName) const
{
	return bool(getByName(peerName));
}

const Peer* PeerTable::getById(pid_t peerId) const
{
	for (size_t i = 0; i < _peers.size(); i++)
		if (_peers[i].getId() == peerId)
			return &_peers[i];
	return 0;
}

const Peer* PeerTable::getByName(const string& peerName) const
{
	for (size_t i = 0; i < _peers.size(); i++)
		if (_peers[i].getName() == peerName)
			return &_peers[i];
	return 0;
}

ByteArray PeerTable::serialize()
{
	ByteArrayWriter writer;
	// The first element in the byte array is the size of the peer table.
	writer.write(_peers.size());
	for (size_t i = 0; i < _peers.size(); i++)
	{
		Peer peer = _peers[i];
		writer.writeString(peer.getName());
		writer.write<pid_t>(peer.getId());
	}

	return writer.getByteArray();
}

void PeerTable::deserialize(const ByteArray& bytes)
{
	_peers.clear();

	ByteArrayReader reader(bytes);
	int size = reader.read<int> ();

	for (int i = 0; i < size; i++)
	{
		std::string name = reader.readString();
		pid_t id = reader.read<pid_t>();

		Peer peer(name, id);
		_peers.push_back(peer);
	}
}
