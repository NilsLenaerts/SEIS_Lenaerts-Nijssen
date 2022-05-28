#include "Compiler.h"

Compiler::Compiler(std::vector<Instruction> instructions) : instructions{ instructions } {
	compile();
}


Bytestream Compiler::compile() {
	//std::vector<uint8_t> bytes{};
	printInstr();

	std::cout << "start Constant folding\n";
	foldConstants();
	//todo --> functionInLine completion
	std::cout << "Constant folding done \nStarting inline\n";
	functionInline();
	std::cout << "inline Done\n";
	printInstr();
	//todo --> convertion to WASM
	Bytestream stream{ convertFunctionsToWASM() };
	std::cout << "Conversion done\n";
	//this is to return our result. but we should make something of our bytes vector
	//Bytestream stream{ bytes };
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
	
		if (instr.getOpcode() == (uint32_t) InstructionSet::func) {
			std::cout << "Functions found\n";
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
			std::cout << "Call found\n";
			if (instr.getStringValue() != "") {
				int functionindex = functionCounterTranslation.at(instr.getStringValue());
				instr.setParam(functionindex);	//here we replace the alias since it's useless after compilation so we might aswell get rid of it already
				++(functions.at(functionindex));
			}
			else {
				++(functions.at(instr.getParam()));
			}
		}
	}

	std::cout << "Functions and calls found\n";
	for (std::pair<int, int> function : functions) {
		std::cout << function.first<<" Functions called "<< function.second <<" times\n";
		if (function.second == 1) {	//now we check if a function was only  called once --> if so we inline the function
			std::vector<Instruction> functionInstructions {};	//these are the instructions that will get inlined at the call
			int depth = -1;
			bool isPartOfFunction{ false };
			int removeInAdvance{ 0 };
			for (int i = 0; i < instructions.size(); i++) {
				Instruction instr= instructions.at(i);
				std::cout << instr.getOpcode() << "\t" << " depth= " << instr.getDepth() << "\n";
				if (instr.getOpcode() == (uint32_t)InstructionSet::func && isPartOfFunction) {
				//if a function is created within a function
					isPartOfFunction = false;
				}

				if (isPartOfFunction && instr.getDepth() >= depth) {
					std::cout << "Line above is part of function \t" << function .first<<"\n";
					if (instr.getOpcode() == (uint32_t)InstructionSet::Export) {	//exported functions shouldn' t be inlined
						break;
					}
					else if (instr.getOpcode() == (uint32_t)InstructionSet::result ){
						 int amount = instr.getParam();
						 for (int k = 0; k <= amount; ++k) {
							 instructions.erase(instructions.begin() + (i));
							 
						 }
						 --i;
					}
					else if (instr.getOpcode() == (uint32_t)InstructionSet::param){
						int amount = instr.getParam();
						for (int k = 0; k <= amount; ++k) {
							instructions.erase(instructions.begin() + (i));
							++removeInAdvance;
						}
						--i;
						--removeInAdvance;
					}
					else{
						functionInstructions.push_back(instr);
						instructions.erase(instructions.begin() + (i));
						--i;
					}

				}
				else {
					isPartOfFunction = false;
				}

				if (instr.getOpcode() == (uint32_t)InstructionSet::func) {
					if (instr.getParam() == function.first) {	//here we got the function start
						std::cout << function.second << " Functions found\t"<< "depth= "<< instr.getDepth()<<"\n";
						isPartOfFunction = true;
						instructions.erase(instructions.begin() + (i));
						--i;
						depth = instr.getDepth();	//we need the depth as we need to know what is part of our func block
					}
				}
				
				else if (instr.getOpcode() == (uint32_t)InstructionSet::call) {
					if (instr.getParam() == function.first) {
						std::cout << "found\n";
						//this means we got the call
						std::cout << "removing " <<  instructions.at((i)).getOpcode() << "\n";
						
						for (int k = 0; k <= removeInAdvance; k++) {
							instructions.erase(instructions.begin() + (i--));
						}
						i++;
						//add all the function parts to the place where the call is located
						instructions.insert((instructions.begin()+ (i)), functionInstructions.begin(),functionInstructions.end());
						
					}
				}
			
			}
			
		}
	}


}
void Compiler::printInstr(){

	std::cout << "Printing instructions: \n\n";
	for (Instruction instr : instructions) {
		std::cout << std::hex <<instr.getOpcode()<<"\t";
		if (instr.getInstructionType() != InstructionType::WITHOUTPARAM) {
			std::cout << instr.getParam();
		}
		std::cout << "\n";
	}


	std::cout << "\n\n";
}
// This function converts WAT constructions to WASM.
Bytestream Compiler::convertFunctionsToWASM()
{

	std::vector<uint8_t> bytes{};
	//we skip the wasm binary and binary version for now https://webassembly.github.io/wabt/demo/wat2wasm/
	std::cout << "Compiler output: \n\n";



	//section "type"
	std::cout << "Types(1) \n";
	std::cout <<  "1 \n";
	std::cout << "0 \n";
	bytes.push_back(1);
	bytes.push_back(0);
	int typeCounter{};

	for (Instruction instr : instructions) {
		if (instr.getOpcode() == (uint32_t)InstructionSet::func) { ++typeCounter; }
	}
	std::cout <<std::hex <<typeCounter << "\n\n";
	bytes.push_back(typeCounter);

	// find the types. 
	std::cout << "type with params \n";
	int seccounter{1};
	for (int i = 0; i < instructions.size(); ++i) {
		Instruction instr{ instructions.at(i) };
		if (instr.getOpcode() == (uint32_t)InstructionSet::func) {
			std::cout << instr.getOpcode()<<"\n";
			bytes.push_back(instr.getOpcode());
			++seccounter;
		}
		else if (instr.getOpcode() == (uint32_t)InstructionSet::Export){
			instructions.erase(instructions.begin() + i);//removal of export
			--i;
		}

		else if (instr.getOpcode() == (uint32_t)InstructionSet::param|| instr.getOpcode() == (uint32_t)InstructionSet::result){
			uint32_t param = instr.getParam();
			++seccounter;
			std::cout <<std::hex << instr.getParam() << "\n";
			bytes.push_back(instr.getParam());
			instructions.erase(instructions.begin() + i);
			for (uint32_t k = 1; k <= param; ++k) {
				Instruction ins{ instructions.at(i) };
				instructions.erase(instructions.begin() + i);
				std::cout << ins.getOpcode() << "\n";
				bytes.push_back(ins.getOpcode());
				++seccounter;
			}
			--i;
		}
	}
	//fixup section size
	std::cout << std::hex << seccounter << "\n\n";
	bytes.push_back(seccounter);
	seccounter = 1 ;
	std::cout << "function (3) \n";
	//section function
	std::cout << "3 \n";
	std::cout << "0 \n";
	bytes.push_back(3);
	bytes.push_back(0);
	std::cout <<std::hex << typeCounter<< "\n";
	bytes.push_back(typeCounter);
	std::vector<int> funcIds{};
	for (Instruction instr : instructions) {
		if (instr.getOpcode() == (uint32_t)InstructionSet::func) { 
			std::cout <<std::hex << instr.getParam() << "\n";
			bytes.push_back(instr.getParam());
			funcIds.push_back(instr.getParam());
			++seccounter;
		}
	}
	std::cout << std::hex << seccounter << "\n\n";
	bytes.push_back(seccounter);

	
	std::cout << "Code (10) \n";
	std::cout << "a \n";
	std::cout << "0 \n";
	bytes.push_back(10);
	bytes.push_back(0);
	std::cout << std::hex << seccounter-1 << "\n\n";
	bytes.push_back(seccounter-1);

	
	seccounter = 0;
	for (int id : funcIds) {
		std::cout << "0 \n";
		std::cout << "0 \n";
		bytes.push_back(0);
		bytes.push_back(0);
		bool inFunc = false;
		int bodyCounter{ 1 };
		for (Instruction instr : instructions) {
			if (inFunc && instr.getOpcode() != (uint32_t)InstructionSet::func) {
				std::cout << instr.getOpcode() << "\n";
				++bodyCounter;
				bytes.push_back(instr.getOpcode());
				if (instr.getInstructionType() == InstructionType::WITHPARAM) {
					std::cout << instr.getParam() << "\n";
					bytes.push_back(instr.getParam());
					++bodyCounter;
				}	
			}
			else if (inFunc && instr.getOpcode() != (uint32_t)InstructionSet::func) {
				inFunc = false;
				std::cout << "b \n";
				bytes.push_back(11);
				++bodyCounter;
			}

			else {
				inFunc = false;
				
			}

			if (instr.getOpcode() == (uint32_t)InstructionSet::func) {
				if (id == instr.getParam()) {
					inFunc = true;
				}
			}
		}
		if (inFunc) {//end function if module ends and function not yet closed
			std::cout << "b \n";
			bytes.push_back(11);
			++bodyCounter;
		}
		//fixup func body size
		std::cout << bodyCounter << "\n";
		bytes.push_back(bodyCounter);
		seccounter += bodyCounter;
	}
	//fixup section size
	std::cout << seccounter << "\n";
	bytes.push_back(seccounter);



	Bytestream bytestreamWASM{ bytes };
	return bytestreamWASM;
}
