#pragma once
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iterator>
#include <vector>
class Bytestream {

private:
    std::vector<uint8_t> buffer; // or unsigned char, or std::byte, or uint8_t
    int currentByteIndex{0};

public:
    Bytestream(std::string path);
    uint8_t readByte(); // or unsigned char, or std::byte, or uint8_t
    std::string readASCIIString(int length); // or char*
    // good idea to write unit tests for these:
    int32_t  readInt32();   // LEB128 decoded!
    uint32_t readUInt32(); // LEB128 decoded!
    //same as int 32 but for 64 bit
    int64_t  readInt64();   
    uint64_t readUInt64(); 
    void seek(int offset); // move the currentByteIndex by offset (can be negative!)




    // later add readInt64, readFloat64, etc.




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