#include "Instruction.h"
//comment
Instruction::Instruction(InstructionType instructionType, uint32_t opcode, int depth, uint32_t param = 0) 
	: instructionType{ instructionType }, opcode{ opcode }, depth{ depth }, param{param} {}


InstructionType Instruction::getInstructionType() const {
	return instructionType;
}
uint32_t Instruction::getOpcode() const {
	return opcode;
}

uint32_t Instruction::getParam()const {
	return param;
}
int Instruction::getDepth() const {
	return depth;
}