#include "Parser.h"

Parser::Parser(Lexer lexer) : lexer{lexer}{}

std::vector<Instruction> Parser::parse()
{
	std::vector<Token> tokens = lexer.getTokens();
	std::vector<Instruction> result = std::vector<Instruction>();

	for (Token token : tokens) {
	
	
	
	}






	return result;
}
