#include "Parser.h"

Parser::Parser(Lexer lexer) : lexer{lexer}{
	try {
		parse();
	}
	catch (int nr) {
		if (nr == 1) {
			std::exit(0);
		}
		
	}
}

std::vector<Instruction> Parser::parse()
{
	std::vector<Token> tokens = lexer.getTokens();
	std::vector<Instruction> result = std::vector<Instruction>();
	//depth is to indicate how many blocks deep we are 
	//(in the end it should end back at 0 else we don't close functions or we close things that don't exist)
	// this is done by the exception at the end

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
				if (Instruction::isCalc(operation)) {
					Instruction inst{ InstructionType::CALC, (uint32_t)operation, depth };
					result.push_back(inst);

 				} else if (Instruction::isConst(operation)) {
					Instruction inst{ InstructionType::CONST, (uint32_t)operation, depth, tokens[++i].getI32Value()};
					result.push_back(inst);

				} else if (Instruction::hasParam(operation)) {
					Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, tokens[++i].getI32Value()};
					result.push_back(inst);
				}
				//noparam should be the last since it will catch all that wasn't caught by hasparam or at least for valid operations
				else if (!(Instruction::hasParam(operation))) {
					Instruction inst{ InstructionType::WITHOUTPARAM, (uint32_t)operation, depth };
					result.push_back(inst);
				}
			}
		}
	}

	if (depth != 0){
		//TODO: We should not return the result here but an error since we have an invalid depth (opens brackets that don't close or reverse)
		throw 1;
	}
	return result;
}
