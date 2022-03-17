#pragma once
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
class Bytestream {

private:
    std::vector<uint32_t> buffer; // or unsigned char, or std::byte, or uint8_t
    int currentByteIndex;

public:
    uint8_t readByte(); // or unsigned char, or std::byte, or uint8_t

    void readFile(std::string path);

    std::string readASCIIString(int length); // or char*

    // good idea to write unit tests for these:
    int32_t  readInt32();   // LEB128 decoded!
    uint32_t readUInt32(); // LEB128 decoded!

    // later add readInt64, readFloat64, etc.




    void seek(int offset); // move the currentByteIndex by offset (can be negative!)

    // optional:
    // currentByteIndex();
    // remainingByteCount();
    // totalByteCount();
    // etc. 

    // recommended (not necessarily on this class though):
    // printHex(char byte)
    // printBinary(char byte)
    // printBinary(int32_t val)
    // printBinary(uint32_t val)


};