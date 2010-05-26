/*
 * byte_array.h
 *
 *  Created on: 23/05/2010
 *      Author: nicolas
 */

#ifndef BYTE_ARRAY_H_
#define BYTE_ARRAY_H_

#include <vector>
#include <string>

typedef std::vector<char> ByteArray;

ByteArray toByteArray(const void* arrayData, size_t size);

void addToByteArray(ByteArray& byteArray, const void* arrayData, size_t size);
bool getFromByteArray(const ByteArray& byteArray, size_t startIndex, void* arrayData,
		size_t size);


void addStringToByteArray(ByteArray& byteArray, const std::string arrayData);
const std::string getStringFromByteArray(const ByteArray& byteArray,
		size_t startIndex, size_t size);

ByteArray stringToByteArray(const std::string& str);
const std::string byteArrayToString(const ByteArray& bytes);

#endif /* BYTE_ARRAY_H_ */