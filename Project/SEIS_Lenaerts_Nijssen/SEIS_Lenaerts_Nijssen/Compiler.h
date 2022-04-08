#pragma once
#include <vector>
#include "Instruction.h"
#include "Bytestream.h"
#include "InstructionSet.h"
#include <unordered_map>
class Compiler {
private:
	std::vector<Instruction> instructions;
	Bytestream output; //Since this will be our wasm file we can also kind off feed it straight up as a bytestream to the vm

	void foldConstants(); //folds things that are constant like sum of 2 constants
	void fold(const int &posConst1,const int &posConst2,const int &posCalc);
	void functionInline();
public:
	Compiler(std::vector<Instruction> instructions);
	Bytestream compile();
};