#include "Parser.h"

Parser::Parser(Lexer lexer) : lexer{lexer}{}

std::vector<Instruction> Parser::parse()
{
	std::vector<Token> tokens = lexer.getTokens();
	std::vector<Instruction> result = std::vector<Instruction>();

	//depth is to indicate how many blocks deep we are (in the end it should end back at 0 else we don't close functions or we close things that don't exist)
	int depth{ 0 };


	for (int i = 0; i < tokens.size(); ++i) {
		Token token = tokens[i];
	

		
		//checks if valid instruction
		if (token.getTokenType() == TypeOfToken::Bracket_Open) {
			++depth;



		}
		else if (token.getTokenType() == TypeOfToken::Bracket_Close) {
			--depth;
		}
		else if (token.getTokenType() == TypeOfToken::Identifier) {
			InstructionSet operation = Instruction::getInstruction(token.getStringValue());
			if (operation != InstructionSet::unreachable) {
				if (Instruction::isConst(operation)) {
					Instruction inst{ InstructionType::CONST, (int)operation};

				}


			}

		}

	}

	if (depth != 0){
		//TODO: We should not return the result here but an error since we have an invalid depth (opens brackets that don't close or reverse)

	}






	return result;
}
