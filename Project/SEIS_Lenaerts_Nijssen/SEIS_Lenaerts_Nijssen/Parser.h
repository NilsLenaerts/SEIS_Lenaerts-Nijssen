#pragma once
#include "Lexer.h"
#include "Instruction.h"

class Parser {
private:
	Lexer lexer;


public:
	Parser(Lexer lexer);
	std::vector<Instruction> parse();




};