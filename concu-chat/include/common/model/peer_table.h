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

class PeerTableEntry : public Serializable
{
public:
	PeerTableEntry(const string& name, const string& id):
		_name(name), _id(id)
	{ }

	const string& getName() const { return _name; }
	const string& getId()   const { return _id; }

	bool operator == (const PeerTableEntry& p)
	{
		return _name == p._name && _id == p._id;
	}

	ByteArray serialize();
	void deserialize(const ByteArray& bytes);

private:
	string _name;
	string _id;
};

class PeerTable
{
public:
	PeerTable()
	{ }

	void addPeer(const PeerTableEntry& peer);
	void removePeer(const PeerTableEntry& peer);

private:
	typedef vector<PeerTableEntry> PeerVector;
	PeerVector _peers;
};

#endif /* PEER_TABLE_H_ */
