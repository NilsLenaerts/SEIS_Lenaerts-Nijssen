#pragma once
#include "InstructionSet.h"
#include <variant>
#include <string>
enum class InstructionType {
	CONST,CALC,
	WITHPARAM,WITHOUTPARAM,
};
class Instruction {
public:
	static InstructionSet getInstruction(std::string inst) {
		if (inst == "i32") return InstructionSet::i32;
		if (inst == "i64") return InstructionSet::i64;
		if (inst == "f32") return InstructionSet::f32;
		if (inst == "v128") return InstructionSet::v128;

		if (inst == "if") return InstructionSet::IF;
		if (inst == "else") return InstructionSet::ELSE;


		if (inst == "i32.add") return InstructionSet::i32add;
		if (inst == "i32.sub") return InstructionSet::i32sub;
		if (inst == "i32.mul") return InstructionSet::i32mul;
		//not certain about the notation of division
		if (inst == "i32.div_s") return InstructionSet::i32div_s;
		if (inst == "i32.div_u") return InstructionSet::i32div_u;
		if (inst == "i32const") return InstructionSet::i32const;
		
		
		if (inst == "local.get") return InstructionSet::localget;
		if (inst == "local.set") return InstructionSet::localset;

		if (inst == "func") return InstructionSet::function;
		if (inst == "end") return InstructionSet::end;
		if (inst == "call") return InstructionSet::call;
	}

	static bool isConst(InstructionSet instruction) {
		//extend the () part with "||" if we have more constant instructions
		return  instruction == (InstructionSet::i32const);
	
	}
	static bool hasParam(InstructionSet inst) {
		return !(inst == InstructionSet::IF  || inst == InstructionSet::ELSE ||
				 inst == InstructionSet::end || inst == InstructionSet::unreachable);
	}

	static bool isCalc(InstructionSet inst) {
		return (inst == InstructionSet::i32add || inst == InstructionSet::i32sub   ||
				inst == InstructionSet::i32mul || inst == InstructionSet::i32div_s ||
				inst == InstructionSet::i32div_u);
	}



	Instruction(InstructionType instructionType, uint32_t opcode, int depth, uint32_t param=0);
	InstructionType getInstructionType()const;
	int getDepth() const;
	uint32_t getOpcode() const;
	uint32_t getParam()const;
private:
	uint32_t param=0;
	int depth=0;
	uint32_t opcode;
	InstructionType instructionType;

};