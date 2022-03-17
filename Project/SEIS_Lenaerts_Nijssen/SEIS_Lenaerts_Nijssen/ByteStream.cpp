#include "ByteStream.h"
#include <stdlib.h>




uint8_t Bytestream::readByte()
{
	if (currentByteIndex > buffer.size()){ //there is no item remaining in the buffer
		throw std::out_of_range("Index goes beyond buffer size");
	}

	uint8_t byte{ buffer[currentByteIndex] };
	++currentByteIndex;
	return byte;
}

void Bytestream::readFile(std::string path)
{
	buffer.clear();
	currentByteIndex = 0;
	//read file in binary format
	std::ifstream stream(path, std::ifstream::binary);
	if (stream.is_open()) {
		uint8_t byte;
		while (stream.good()) {
			byte = stream.get();
			buffer.push_back(byte);
		}
		stream.close();

	}
	else {
		//close program if didn't read a file
		exit(EXIT_FAILURE);
	}
	

}


std::string Bytestream::readASCIIString(int length)
{
	std::string string{};

	for (int i = 0; i < length; ++i) {
		string += readByte();
	}

	return string;
}

int32_t Bytestream::readInt32()
{
	uint32_t result{};
	uint8_t byte;
	int shift{ 0 };

	const int size{ 32 };
	
	while (true) {
		byte = readByte();
		result |= ((byte & 0b01111111) << shift);
		if ((byte & 0b10000000) == 0) break;

		shift += 7;
	}

	if ((shift < size) && ((byte & 0b01000000) == 0b01000000))
		/* sign extend */
		result |= (~0 << shift);

	return result;
}


uint32_t Bytestream::readUInt32()
{
	uint32_t result{0};
	uint8_t byte;
	int shift{ 0 };
	//get the 4 bytes and put them into the 32bit nr

	//based on wikipedia code example
	while (true) {
		byte = readByte();	//we read the new byte and we need to add it to the result
		result |= ((byte & 0b01111111) << shift);
		if ((byte & 0b10000000) == 0) break;

		shift += 7;
	}


	return result;
}

void Bytestream::seek(int offset)
{
	currentByteIndex += offset;
}
