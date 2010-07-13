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
#include <iosfwd>

#include "core/serializable.h"

using std::vector;
using std::string;

class Peer
{
public:
	Peer(const string& name, pid_t id);

	const string& getName() const;
	const pid_t& getId() const;

	bool operator ==(const Peer& p);
	friend std::ostream& operator <<(std::ostream& os, const Peer& peer);

private:
	string _name;
	pid_t _id;
};

class PeerTable: public Serializable
{
public:
	PeerTable();

	void add(const Peer& peer);
	void remove(const string& peerName);

	bool containsId(pid_t peerId) const;
	bool containsName(const string& peerName) const;

	const Peer* getById(pid_t peerId) const;
	const Peer* getByName(const string& peerName) const;

	ByteArray serialize();
	void deserialize(const ByteArray& bytes);

	friend std::ostream& operator <<(std::ostream& os, const PeerTable& peers);

private:
	typedef vector<Peer> PeerVector;
	PeerVector _peers;
};

#endif /* PEER_TABLE_H_ */
