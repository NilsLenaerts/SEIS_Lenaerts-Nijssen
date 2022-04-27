#include "Instruction.h"




Instruction::Instruction(InstructionType instructionType, uint32_t opcode, int depth, uint32_t param) 
	: instructionType{ instructionType }, opcode{ opcode }, depth{ depth }, param{param} {}

Instruction::Instruction(InstructionType instructionType, uint32_t opcode, int depth, std::string param)
	: instructionType{ instructionType }, opcode{ opcode }, depth{ depth }, param{ param } {}

InstructionType Instruction::getInstructionType() const {
	return instructionType;
}
uint32_t Instruction::getOpcode() const {
	return opcode;
}


std::string Instruction::getStringValue()const
{
	if (auto result = std::get_if<std::string>(&param)) {
		return std::get<std::string>(param);
	}
	return "";

}

void Instruction::setParam(uint32_t parameter) {
	param = parameter;
}

uint32_t Instruction::getParam()const {
	if (auto result = std::get_if<uint32_t>(&param)) {
		return std::get<uint32_t>(param);
	}
	return 0;

}
int Instruction::getDepth() const {
	return depth;
}