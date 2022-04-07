#pragma once
#include "InstructionSet.h"

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


		if (inst == "end") return InstructionSet::end;
		if (inst == "call") return InstructionSet::call;
	}

private:


};