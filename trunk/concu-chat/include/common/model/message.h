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
		ByteArray result;
		addToByteArray(result, &_type, sizeof(_type));
		addToByteArray(result, &_data[0], _data.size());
		return result;
	}

	virtual void deserializeFromIndex(const ByteArray& bytes, size_t startIndex,
			size_t size)
	{
		getFromByteArray(bytes, 0, &_type, sizeof(_type));
		_data = ByteArray(bytes.begin() + sizeof(_type), bytes.end());
	}

private:
	unsigned _type;
	ByteArray _data;
};

#endif /* MESSAGE_H_ */
