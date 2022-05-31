#pragma once
#include "Lexer.h"
#include "Instruction.h"

class Parser {
private:
	Lexer lexer;
	std::vector<Instruction> instructions;
	void printParser();
	Instruction parseFunc( int &index,const std::vector<Token>&tokens, const int &depth, const uint32_t& functionCounter);
	Instruction parseCall(int& index, const std::vector<Token>& tokens, const int& depth, const uint32_t& functionCounter);

public:
	Parser(Lexer lexer);
	std::vector<Instruction> parse();




};