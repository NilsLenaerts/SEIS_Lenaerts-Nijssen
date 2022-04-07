#pragma once

enum class InstructionSet {
	/*GENERAL*/
	unreachable = 0x00,
	nop = 0x01,
	/*TYPES */
	i32 = 0x7F, i64 = 0x7E, f32 = 0x7D, f64 = 0x7C,
	v128 = 0x7B, funcref = 0x70, function = 0x60,
	/*SECTIONS*/

	/*OPERATIONS*/
	i32add = 0x6A, i32sub = 0x6B, i32mul = 0x6c, i32div_s = 0x6D, i32div_u = 0x6E, i32const = 0x41,

	IF=0x04, ELSE = 0x05, 
	localget = 0x20, localset = 0x21,
	

	end= 0x0B, call=0x10,

	/*Other*/
	loop = 0x03, block = 0x02,
};