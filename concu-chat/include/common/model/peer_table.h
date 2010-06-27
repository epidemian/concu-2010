/*
 * peers_table.h
 *
 *  Created on: May 11, 2010
 *      Author: ???
 */

#ifndef PEER_TABLE_H_
#define PEER_TABLE_H_

#include <vector>
#include <string>
#include "core/serializable.h"

using std::vector;
using std::string;

class Peer
{
public:
	Peer(const string& name, const string& id):
		_name(name), _id(id)
	{ }

	const string& getName() const { return _name; }
	const string& getId()   const { return _id; }

	bool operator == (const Peer& p)
	{
		return _name == p._name && _id == p._id;
	}

private:
	string _name;
	string _id;
};

class PeerTable : public Serializable
{
public:
	PeerTable()
	{ }

	void add(const Peer& peer);
	void remove(const Peer& peer);

	bool containsId(const string& peerId) const;
	bool containsName(const string& peerName) const;

	const Peer* getById(const string& peerId) const;
	const Peer* getByName(const string& peerName) const;


	ByteArray serialize();
	void deserialize(const ByteArray& bytes);

private:
	typedef vector<Peer> PeerVector;
	PeerVector _peers;
};

#endif /* PEER_TABLE_H_ */
