/*
 * byte_array.cpp
 *
 *  Created on: 23/05/2010
 *      Author: nicolas
 */

#include "core/byte_array.h"

#include <string.h>
#include "stdlib.h"

ByteArray toByteArray(const void* arrayData, size_t size)
{
	ByteArray byteArray;
	addToByteArray(byteArray,arrayData,size);
	return byteArray;
}

void addToByteArray(ByteArray& byteArray, const void* arrayData, size_t size)
{
	char* buffer = (char*)arrayData;
	for (size_t i = 0; i < size; i++)
		byteArray.push_back(buffer[i]);
}

bool getFromByteArray(ByteArray& byteArray, size_t startIndex, void* arrayData,
		size_t size)
{
	if (byteArray.size() < size + startIndex - 1)
		return false;

	memcpy((char*)arrayData, byteArray.data() + startIndex, size);

	return true;
}

void addStringToByteArray(ByteArray& byteArray, const std::string arrayData)
{
	addToByteArray(byteArray,arrayData.c_str(), arrayData.size()+1);
}

const std::string getStringFromByteArray(ByteArray& byteArray,
		size_t startIndex, size_t size)
{
	char* buffer = (char*)malloc(size);

	getFromByteArray(byteArray,startIndex,buffer,size);

	std::string arrayData;
	arrayData.append(buffer);

	free (buffer);

	return arrayData;
}


