#include "Compiler.h"

Compiler::Compiler(std::vector<Instruction> instructions) : instructions{ instructions } {
	compile();
}


Bytestream Compiler::compile() {
	std::vector<uint8_t> bytes{};
	//todo --> check depth of instruction aswell since we can't fold over different depths
	// we also need to be aware of that because we can't use variables that are deeper than our current depth
	foldConstants();
	//todo --> functionInLine completion
	functionInline();
	//todo --> convertion to WASM


	Bytestream stream{ bytes };
	return stream;
}



void Compiler::foldConstants()
{
	bool didfold{ true };
	while (didfold) {
		didfold = false;
		for (int i = 0; i < instructions.size(); ++i) {
			Instruction inst = instructions[i];
			int depthOfinst1{inst.getDepth() };
			if (inst.getInstructionType() == InstructionType::CONST) {
				//we need to check if not out of range 
				int j = i + 1;
				int depthOfinst2{instructions[j].getDepth()};
				int depthOfinst3{instructions[j + 1].getDepth()};

				//Here we enforce that all instructions considered for folding are on the same depth
				if (depthOfinst1 <= depthOfinst2 && depthOfinst1 <= depthOfinst3 && depthOfinst2 <= depthOfinst3) {
					if (instructions[j].getInstructionType() == InstructionType::CONST) {
						if (instructions[j + 1].getInstructionType() == InstructionType::CALC) {
							//here we have to fold
							fold(i, j, j + 1);
							didfold = true;
						}
					}
				}

				
			}
		}
	}
}

void Compiler::fold(const int& posConst1, const int& posConst2, const int& posCalc)
{
	uint32_t result = 0;
	Instruction calculation = instructions[posCalc];
	uint32_t opcode = calculation.getOpcode();

	if (opcode == (uint32_t)InstructionSet::i32add) {
		result = instructions[posConst1].getParam() + instructions[posConst2].getParam();
	}
	else if (opcode == (uint32_t)InstructionSet::i32sub) {
		result = instructions[posConst1].getParam() - instructions[posConst2].getParam();
	}
	else if (opcode == (uint32_t)InstructionSet::i32mul) {
		result = instructions[posConst1].getParam() * instructions[posConst2].getParam();
	}
	else if (opcode == (uint32_t)InstructionSet::i32div_u) {
		result = instructions[posConst1].getParam() / instructions[posConst2].getParam();
	}

	Instruction inst{ InstructionType::CONST, (uint32_t) InstructionSet::i32const, calculation.getDepth(),result};

	//replace the first instruction with our new folded instruction
	instructions[posConst1] = inst;
	//erases a range but these are items in line so we just delete both items
	instructions.erase(instructions.begin() + posConst2, instructions.begin() + posCalc);
	

}

void Compiler::functionInline()
{
	std::unordered_map<std::string, int> functions{}; //in this the string is to indicate the function name and the int to count the amount of times we call it
	int functioncounter{ 0 };
	//to start off we need to define the functions and how many times they are called, then if a function is called less than X times we inline it.
	for (Instruction instr : instructions) {
		if (instr.getOpcode() == (uint32_t) InstructionSet::function) {

			functions.insert(std::pair<std::string, int>{"nameOfFunction", 0});
			//in here we got a function to be created
			//!todo insert function into map
		}
		//Check for function calls here --> up counter if so. throw exception if function doens't exist
	
	}
}
