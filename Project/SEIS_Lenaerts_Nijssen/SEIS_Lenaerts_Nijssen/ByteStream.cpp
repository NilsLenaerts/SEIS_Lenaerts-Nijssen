/* References for code in here
(1)	https://stackoverflow.com/questions/1647557/ifstream-how-to-tell-if-specified-file-doesnt-exist
(2)	https://stackoverflow.com/questions/1675508/reading-an-stdifstream-to-a-vector-of-lines
(3) https://en.wikipedia.org/wiki/LEB128
*/

#include "Bytestream.h"



Bytestream::Bytestream(std::string path)
{
	std::ifstream stream(path, std::ios::binary);

	if (stream.fail()) {
		//File does not exist code here (1)
		std::exit(0);
	}

	//copy all bytes into a buffer (2)
	std::copy(
		std::istream_iterator<uint8_t>(stream),
		std::istream_iterator<uint8_t>(),
		std::back_inserter(buffer));

	//stream.close(); //maybe not neccessarry

}

Bytestream::Bytestream() {};

uint8_t Bytestream::readByte()
{
	uint8_t byte = checkByte();
	++currentByteIndex;
	return byte;
}

uint8_t Bytestream::checkByte()
{
	if (currentByteIndex > buffer.size()) { //there is no item remaining in the buffer
		throw std::out_of_range("Index goes beyond buffer size");
	}

	uint8_t byte{ buffer[currentByteIndex] };
	
	return byte;
}


std::string Bytestream::readASCIIString(int length)
{
	std::string string{};

	for (int i = 0; i < length; ++i) {
		string += readByte();
	}

	return string;
}


//based on wikipedia code examples  (3)
int32_t Bytestream::readInt32()
{
	int32_t result{};
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

	//based on wikipedia code examples(3)
	while (true) {
		byte = readByte();	//we read the new byte and we need to add it to the result
		result |= ((byte & 0b01111111) << shift);
		if ((byte & 0b10000000) == 0) break;

		shift += 7;
	}


	return result;
}

int64_t Bytestream::readInt64()
{
	int64_t result{};
	uint8_t byte;
	int shift{ 0 };

	const int size{ 64 };

	while (true) {
		byte = readByte();
		result |= ((byte & 0b01111111) << shift);
		if ((byte & 0b10000000) == 0) break;

		shift += 7;
	}

	if ((shift < size) && ((byte & 0b0100'0000) == 0b0100'0000))
		/* sign extend */
		result |= (~0 << shift);

	return result;
}

uint64_t Bytestream::readUInt64()
{
	uint64_t result{ 0 };
	uint8_t byte;
	int shift{ 0 };
	//get the 4 bytes and put them into the 32bit nr

	
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

bool Bytestream::atEnd()
{
	return (currentByteIndex >= buffer.size());
}

void Bytestream::skipByte()
{
	++currentByteIndex;
}
