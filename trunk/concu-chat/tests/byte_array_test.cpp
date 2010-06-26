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

int main(int argc, char **argv)
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
	writer.writeInt(int1);
	writer.writeInt(int2);
	writer.writeInt(int3);
	writer.writeInt(int4);

	ByteArray bytes = writer.getByteArray();

	ByteArrayReader reader(bytes);

	if (reader.readInt() == int1 &&
		reader.readInt() == int2 &&
		reader.readInt() == int3 &&
		reader.readInt() == int4)
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
	writer.writeInt(lifes);
	writer.writeString(pass);

	ByteArray bytes = writer.getByteArray();

	ByteArrayReader reader(bytes);

	if (reader.readString() == name &&
		reader.readInt() == lifes &&
		reader.readString() == pass)
		std::cout << "Green test" << std::endl;
	else
		std::cout << "Red test" << std::endl;



}
