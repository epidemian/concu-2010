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

	enum {
		TYPE_USER_INPUT = 1,
		TYPE_USER_EXIT
	};

	Message(): _type(0)
	{ }

	Message(unsigned type, const ByteArray& data):
	_type(type), _data(data)
	{ }

	const ByteArray& getData() const { return _data; }
	unsigned getType() const { return _type; }

	virtual ByteArray serialize()
	{
		ByteArrayWriter writer;
		writer.write(_type);
		writer.writeByteArray(_data);
		return writer.getByteArray();
	}

	virtual void deserialize(const ByteArray& bytes)
	{
		ByteArrayReader reader(bytes);
		_type = reader.read<unsigned>();
		_data = reader.readAll();
	}

private:
	unsigned _type;
	ByteArray _data;
};

#endif /* MESSAGE_H_ */
