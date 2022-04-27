#include "Compiler.h"

Compiler::Compiler(std::vector<Instruction> instructions) : instructions{ instructions } {
	compile();
}


Bytestream Compiler::compile() {
	std::vector<uint8_t> bytes{};

	foldConstants();
	//todo --> functionInLine completion
	functionInline();
	//todo --> convertion to WASM

	//this is to return our result. but we should make something of our bytes vector
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
	std::unordered_map< std::string, int> functionCounterTranslation{};		//the string is the function name ($foo for example) the value is the index corresponding with it.
	std::unordered_map<int, int> functions{}; //first int is the function index the second is the amount of times it was called
	int functioncounter{ 0 };
	//to start off we need to define the functions and how many times they are called, then if a function is called less than X times we inline it.
	for (Instruction instr : instructions) {
		if (instr.getOpcode() == (uint32_t) InstructionSet::function) {

			//if we gave the function a name
			if (instr.getStringValue() != "") {
				functionCounterTranslation.insert(std::pair<std::string, int>{instr.getStringValue(), functioncounter});
				functions.insert(std::pair<int, int>{functioncounter, 0});
				instr.setParam(functioncounter);	//here we replace the alias since it's useless after compilation so we might aswell get rid of it already
				++functioncounter;
			}
			//if the function has no name. 
			else {
				functions.insert(std::pair<int, int>{instr.getParam(), 0});
				++functioncounter;
			}
			//in here we got a function to be created
		
		}
		//if we get the opcode corresponding with call --> we add 1 to the counter.
		else if (instr.getOpcode() == (uint32_t)InstructionSet::call) {
			if (instr.getStringValue() != "") {
				int functionindex = functionCounterTranslation.at(instr.getStringValue());
				instr.setParam(functionindex);	//here we replace the alias since it's useless after compilation so we might aswell get rid of it already
				++(functions.at(functionindex));
			}
			else {
				++(functions.at(instr.getParam()));
			}
		}
		//Check for function calls here --> up counter if so. throw exception if function doens't exist
	}
	for (std::pair<int, int> function : functions) {
		if (function.second == 1) {
			//we should inline here
			std::vector<Instruction> functionInstructions {};	//these are the instructions that will get inlined at the call
			int depth = -1;
			bool isPartOfFunction{ false };

			for (int i = 0; i < instructions.size(); i++) {
				Instruction instr= instructions.at(i);
				if (isPartOfFunction && instr.getDepth() > depth) {
					functionInstructions.push_back(instr);
				}
				else if (instr.getOpcode() == (uint32_t)InstructionSet::function) {
					if (instr.getParam() == function.first) {	//here we got the function start
						isPartOfFunction = true;
						depth = instr.getDepth();	//we need the depth as we need to know what is part of our func block
					}
				}
				else if (instr.getOpcode() == (uint32_t)InstructionSet::call) {
					if (instr.getParam() == function.first) {
						//this means we got the call
						instructions.erase(instructions.begin() + (i-1));
						//add all the function parts to the place where the call is located
						instructions.insert((instructions.begin()+ (i - 1)), functionInstructions.begin(),functionInstructions.end());
						
					}
				}
			
			}
			
		}
	}
	//now we check if a function was only  called once --> if so we inline the function

}
