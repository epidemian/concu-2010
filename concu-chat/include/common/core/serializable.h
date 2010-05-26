/*
 * Serializable.h
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include "byte_array.h"

class Serializable
{
public:
	virtual ByteArray serialize() = 0;
	virtual void deserializeFromIndex(const ByteArray& bytes, size_t startIndex,
			size_t size) = 0;
	void deserialize(const ByteArray& bytes);
};

#endif /* SERIALIZABLE_H_ */