#pragma once

#include "Node.hpp"

#include <array>

namespace TIS
{
	class Port;

	enum class Opcode : unsigned char
	{
		/* NOP is a pseudoinstruction */
		MOV,
		SWP,
		SAV,
		ADD,
		SUB,
		NEG,
		JMP,
		JEZ,
		JNZ,
		JGZ,
		JLZ,
		JRO
	};

	static const std::array<Opcode, 5> LabelOpcodes = {
		Opcode::JMP, Opcode::JEZ, Opcode::JNZ, 
		Opcode::JGZ, Opcode::JLZ
	};

	struct Instruction
	{
		Opcode opcode;
		
		struct Argument
		{
			bool isOffset = false;

			union
			{
				Node::Destination destination;
				short offset;
			};
		};

		std::array<Argument, 2> arguments;
	};
}