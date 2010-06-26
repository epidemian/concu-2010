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
	addToByteArray(byteArray, arrayData, size);
	return byteArray;
}

void addToByteArray(ByteArray& byteArray, const void* arrayData, size_t size)
{
	char* buffer = (char*) arrayData;
	for (size_t i = 0; i < size; i++)
		byteArray.push_back(buffer[i]);
}

bool getFromByteArray(const ByteArray& byteArray, size_t startIndex,
		void* arrayData, size_t size)
{
	if (byteArray.size() < size + startIndex - 1)
		return false;

	memcpy((char*) arrayData, byteArray.data() + startIndex, size);

	return true;
}

void addStringToByteArray(ByteArray& byteArray, const std::string arrayData)
{
	addToByteArray(byteArray, arrayData.c_str(), arrayData.size() + 1);
}

const std::string getStringFromByteArray(const ByteArray& byteArray,
		size_t startIndex, size_t size)
{
	return std::string(byteArray.begin() + startIndex, byteArray.begin()
			+ startIndex + size);
}

const std::string byteArrayToString(const ByteArray& bytes)
{
	return getStringFromByteArray(bytes, 0, bytes.size());
}

ByteArray stringToByteArray(const std::string& str)
{
	ByteArray result;
	addStringToByteArray(result, str);
	return result;
}

ByteArrayWriter::ByteArrayWriter()
{
}

void ByteArrayWriter::writeInt(int int1)
{
	addToByteArray(_bytes, &int1, sizeof(int));
}

void ByteArrayWriter::writeString(const std::string& string)
{
	addStringToByteArray(_bytes, string);
}

ByteArray ByteArrayWriter::getByteArray()
{
	return _bytes;
}

ByteArrayReader::ByteArrayReader(const ByteArray& bytes) :
	_bytes(bytes)
{
	_index = 0;
}

int ByteArrayReader::readInt()
{
	int int1;
	size_t size = sizeof(int);

	getFromByteArray(_bytes, _index, &int1, size);
	_index += size;

	return int1;
}

std::string ByteArrayReader::readString()
{
	const char* data = &_bytes[0];
	std::string string(data + _index);
	_index += string.size() + 1;
	return string;
}
