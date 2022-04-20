#pragma once
#include "Bytestream.cpp"
#include "InstructionSet.h"
#include <string>

class WASMVM
{
public:
	WASMVM();
	void loadFile(std::string path);
private:
	Bytestream byteStream;

	void readTypeSection();
	void readFuncSection();
	void readCodeSection();
	void readExportSection();
	void readMemorySection();
	void readDataSection();
	void readDataCountSection();
	void readImportSection();
};

