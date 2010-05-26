/*
 * serializable.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "core/serializable.h"

void Serializable::deserialize(const ByteArray& bytes)
{
	deserializeFromIndex(bytes,0,bytes.size());
}
