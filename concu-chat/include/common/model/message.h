/*
 * message.h
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "core/byte_array.h"
#include "core/serializable.h"
#include <string>

using std::string;

class Message: public Serializable
{
public:

	enum MessageType
	{
		TYPE_NONE = 0,
		TYPE_USER_INPUT,
		TYPE_USER_EXIT,
		TYPE_REGISTER_NAME_REQUEST,
		TYPE_REGISTER_NAME_RESPONSE,
		TYPE_UNREGISTER_NAME_REQUEST,
		TYPE_UNREGISTER_NAME_RESPONSE,
		TYPE_PEER_TABLE_REQUEST,
		TYPE_PEER_TABLE_RESPONSE,
		TYPE_START_CHAT_REQUEST,
		TYPE_START_CHAT_RESPONSE,
		TYPE_END_CHAT,
		TYPE_CHAT_MESSAGE,
		TYPE_SERVER_EXIT
	};

	Message() :
		_type(TYPE_NONE), _messengerPid(0)
	{
	}

	Message(MessageType type, pid_t messengerPid, const ByteArray& data =
			ByteArray()) :
		_type(type), _messengerPid(messengerPid), _data(data)
	{
	}

	MessageType getType() const
	{
		return _type;
	}

	pid_t getMessengerPid() const
	{
		return _messengerPid;
	}

	const ByteArray& getData() const
	{
		return _data;
	}

	virtual ByteArray serialize()
	{
		ByteArrayWriter writer;
		writer.write(_type);
		writer.write(_messengerPid);
		writer.writeByteArray(_data);
		return writer.getByteArray();
	}

	virtual void deserialize(const ByteArray& bytes)
	{
		ByteArrayReader reader(bytes);
		_type = reader.read<MessageType> ();
		_messengerPid = reader.read<pid_t> ();
		_data = reader.readAll();
	}

private:
	MessageType _type;
	pid_t _messengerPid;
	ByteArray _data;
};

#endif /* MESSAGE_H_ */
