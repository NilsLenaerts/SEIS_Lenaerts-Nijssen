#include "Parser.h"


Parser::Parser(Lexer lexer) : lexer{lexer}{
	try {
		parse();
		printParser();
	}
	catch (int nr) {
		if (nr == 1) {
			std::exit(0);
		}
		
	}
}

void Parser::printParser()
{	
	std::cout << "\n\n" <<"Parser:" << "\n";
	for (Instruction inst : instructions) {
		
		std::cout << inst.getOpcode() << "\t";
		if (inst.getInstructionType() != InstructionType::WITHOUTPARAM) {
			std::cout << inst.getParam();
		}
		std::cout << "\n";
	}
}



std::vector<Instruction> Parser::parse()
{
	std::vector<Token> tokens = lexer.getTokens();
	instructions = std::vector<Instruction>();
	//depth is to indicate how many blocks deep we are 
	//(in the end it should end back at 0 else we don't close functions or we close things that don't exist)
	// this is done by the exception at the end

	int depth{ 0 };
	uint32_t moduleCounter{ 0 };
	uint32_t functionCounter{ 0 };
	std::cout << "Check for all tokens" << "\n";
	for (int i = 0; i < tokens.size(); ++i) {
		Token token = tokens[i];
		if (token.getTokenType() == TypeOfToken::Number) {
			std::cout << token.getI32Value() << "\t";
		}
		else {
			std::cout << token.getStringValue() <<"\t";
		}
		
		//checks if valid instruction
		if (token.getTokenType() == TypeOfToken::Bracket_Open) {

			std::cout << " brackets opened "<< "\n";
			++depth;
			
		}
		else if (token.getTokenType() == TypeOfToken::Bracket_Close) {
			--depth;
			
			std::cout << " brackets Closed "<< "\n";
		}
		else if (token.getTokenType() == TypeOfToken::Identifier) {
			InstructionSet operation = Instruction::getInstruction(token.getStringValue());
			if (operation != InstructionSet::unreachable) {
				if (Instruction::isCalc(operation)) {
					Instruction inst{ InstructionType::CALC, (uint32_t)operation, depth };
					std::cout << " isCalc" << "\n";
					instructions.push_back(inst);

 				} else if (Instruction::isConst(operation)) {
					Instruction inst{ InstructionType::CONST, (uint32_t)operation, depth, tokens[++i].getI32Value()};
					std::cout << " isConst" << "\n";
					instructions.push_back(inst);

				} else if (Instruction::hasParam(operation)) {
					std::cout << " hasParam" << "\t";
					//we also need a check for Module.
					if (operation == InstructionSet::call) {
						Instruction inst = parseCall(i, tokens, depth, functionCounter);
						
						instructions.push_back(inst);

					}
					// In the event that we have a function --> this means we need to check if it has a name
					else if (operation == InstructionSet::func) {
						Instruction inst = parseFunc(i, tokens, depth, functionCounter);
						++functionCounter;
						instructions.push_back(inst);
						
					}
					//for other functions with parameters
					else {
					Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, tokens[++i].getI32Value()};
					std::cout << tokens[i].getI32Value() << "\t is set as parameter" << "\n";
					instructions.push_back(inst);
					}
				}
				//noparam should be the last since it will catch all that wasn't caught by hasparam or at least for valid operations
				else if (!(Instruction::hasParam(operation))) {
					Instruction inst{ InstructionType::WITHOUTPARAM, (uint32_t)operation, depth };
					std::cout << " HasNoParam" << "\n";
					instructions.push_back(inst);
				}
			}
		}
	}

	if (depth != 0){
		//TODO: We should not return the result here but an error since we have an invalid depth (opens brackets that don't close or reverse)
		throw 1;
	}
	return instructions;
}
//This is a help function for the parse function to seperate how to handle func operations
Instruction Parser::parseFunc(int& index, const std::vector<Token>& tokens, const int& depth , const uint32_t &functionCounter)
{
	InstructionSet operation = InstructionSet::func;
	Token nextToken = tokens[index + 1];
	if (nextToken.getTokenType() != TypeOfToken::Number) {
		if (nextToken.getStringValue()[0] == '$') {
			Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, nextToken.getStringValue() };
			tokens[++index];
			std::cout << nextToken.getStringValue() << "\t is consumed by func" << "\n";
			return inst;
		}
		
	}
	Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, functionCounter };
	std::cout << functionCounter << "\t is set for func" << "\n";
	return inst;
}

Instruction Parser::parseCall(int& index, const std::vector<Token>& tokens, const int& depth, const uint32_t& functionCounter)
{
	InstructionSet operation = InstructionSet::call;
	Token nextToken = tokens[index + 1];
	if (nextToken.getTokenType() != TypeOfToken::Number) {
		if (nextToken.getStringValue()[0] == '$') {
			Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, nextToken.getStringValue() };
			tokens[++index];
			std::cout << nextToken.getStringValue() << "\t is consumed by Call" << "\n";
			return inst;
		}
	}
	Instruction inst{ InstructionType::WITHPARAM, (uint32_t)operation, depth, tokens[++index].getI32Value() };
	std::cout << tokens[index].getI32Value() << "\t is set for Call" << "\n";
	return inst;
}
