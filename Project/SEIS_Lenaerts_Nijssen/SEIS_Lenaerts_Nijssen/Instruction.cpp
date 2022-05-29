#include "Instruction.h"




Instruction::Instruction(InstructionType instructionType, uint32_t opcode, int depth, uint32_t param)
	: instructionType{ instructionType }, opcode{ opcode }, depth{ depth }, intVal{ param } {}

Instruction::Instruction(InstructionType instructionType, uint32_t opcode, int depth, std::string param)
	: instructionType{ instructionType }, opcode{ opcode }, depth{ depth }, stringVal{ param } {}

InstructionType Instruction::getInstructionType() const {
	return instructionType;
}
uint32_t Instruction::getOpcode() const {
	return opcode;
}


std::string Instruction::getStringValue()const
{
	return stringVal;

}

void Instruction::setParam(uint32_t parameter) {
	intVal = parameter;
}

uint32_t Instruction::getParam()const {
	return intVal;


}
int Instruction::getDepth() const {
	return depth;
}