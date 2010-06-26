/*
 * serializable.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "core/serializable.h"

void Serializable::deserializeFromIndex(const ByteArray& bytes,
		size_t startIndex, size_t size)
{
	ByteArray::const_iterator from = bytes.begin() + startIndex;
	ByteArray::const_iterator to = bytes.begin() + startIndex + size;
	ByteArray subBytes = ByteArray(from, to);
	this->deserialize(subBytes);
}
