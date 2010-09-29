/*
 * byte_array_test.cpp
 *
 *  Created on: 26/06/2010
 *      Author: nicolas
 */

#include <iostream>
#include <string>

#include "core/byte_array.h"

void testInts();
void testStrings();
void testIntsAndStrings();

int main(int, char**)
{
	testInts();
	testStrings();
	testIntsAndStrings();

	return 0;
}

void testInts()
{

	int int1 = 10;
	int int2 = 15;
	int int3 = 30;
	int int4 = 0;

	ByteArrayWriter writer;
	writer.write<int>(int1);
	writer.write<int>(int2);
	writer.write<int>(int3);
	writer.write<int>(int4);

	ByteArray bytes = writer.getByteArray();

	ByteArrayReader reader(bytes);

	if (reader.read<int>() == int1 &&
		reader.read<int>() == int2 &&
		reader.read<int>() == int3 &&
		reader.read<int>() == int4)
		std::cout << "Green test" << std::endl;
	else
		std::cout << "Red test" << std::endl;
}

void testStrings()
{
	std::string string1 = "how";
	std::string string2 = "u";
	std::string string3 = "doing";
	std::string string4 = "?";

	ByteArrayWriter writer;
	writer.writeString(string1);
	writer.writeString(string2);
	writer.writeString(string3);
	writer.writeString(string4);

	ByteArray bytes = writer.getByteArray();

	ByteArrayReader reader(bytes);

	if (reader.readString() == string1 &&
		reader.readString() == string2 &&
		reader.readString() == string3 &&
		reader.readString() == string4)
		std::cout << "Green test" << std::endl;
	else
		std::cout << "Red test" << std::endl;
}

void testIntsAndStrings(){

	std::string name = "Mario Bross";
	int lifes = 3;
	std::string pass = "Green pipe";

	ByteArrayWriter writer;
	writer.writeString(name);
	writer.write(lifes);
	writer.writeString(pass);

	ByteArray bytes = writer.getByteArray();

	ByteArrayReader reader(bytes);

	if (reader.readString() == name &&
		reader.read<int>() == lifes &&
		reader.readString() == pass)
		std::cout << "Green test" << std::endl;
	else
		std::cout << "Red test" << std::endl;



}
